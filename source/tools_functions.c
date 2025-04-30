#include "common.h"
#include "Buffer.h"
#include <dirent.h>

int IntFromTouchPos(int x, int y, int page) {
    int nb_x = (x-(x % NB_BOX_X))/(int)BOX_NAME_LENGTH;
    int nb_y = (y-(y % NB_BOX_Y))/(int)BOX_NAME_WIDTH*NB_BOX_Y;
    return nb_x + nb_y ;
}

char* SoundPath(const char* dir, const char* name, const char* ext) {
    size_t len = strlen(dir) + strlen(name) + strlen(ext) + 2;
    char* path = malloc(len);
    snprintf(path, len, "%s/%s%s", dir, name, ext);
    return path;
}

char* PrintNbPage(int page, char* max_page) {
    char page_str[12];  // suffisamment grand pour contenir n'importe quel entier
    sprintf(page_str, "%d", page + 1);

    // Taille : "Page : " + page_str + "/" + max_page + '\0'
    int total_length = strlen("Page : ") + strlen(page_str) + 1 + strlen(max_page) + 1;
    char* text_page = malloc(total_length * sizeof(char));
    if (text_page == NULL) return NULL; // toujours vérifier

    sprintf(text_page, "Page : %s/%s", page_str, max_page);
    return text_page;
}

bool CheckForDspfirm() {
    DIR* dir = opendir("/3ds/");
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, "dspfirm.cdc") == 0){
            return true;
        }
    }
    return false;
}

bool endsWith(const char* filename) {
    const char* dot = strrchr(filename, '.');
    return (dot && strcmp(dot, ".wav") == 0);
}

int CountFiles(const char* folder, C3D_RenderTarget* top, C3D_RenderTarget* bottom) {
    DIR* dir = opendir(folder);
    if (!dir) {
        mkdir(AudioDirectory, 0777);
        dir = opendir(folder);
        while (aptMainLoop())
        {
            hidScanInput();
            u32 kDown = hidKeysDown();
            //printf("\x1b[1;0H\x1b[33m%s\n", MissingFolder);
            CreateBuffer(&Dynamic_Name_Buf, &Dynamic_Name_Text, MissingFolder, font);
            if (kDown & KEY_A) {
                C2D_TextBufDelete(Dynamic_Name_Buf);
                break;
            }

            C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
            {
                C2D_SceneBegin(top);
                C2D_TargetClear(top, C2D_Color32(157, 150, 64, 255));

                C2D_DrawText(&Dynamic_Name_Text, C2D_AtBaseline | C2D_WithColor, 20.0f, SCREEN_SIZE_WIDTH/2-15.0f, DEPTH, 0.5f, 0.5f, TEXT_COLOR);

                C2D_SceneBegin(bottom);
                C2D_TargetClear(bottom, C2D_Color32(157, 150, 64, 255));
                
            }
            C3D_FrameEnd(0);

            gspWaitForVBlank();
            gfxFlushBuffers();
        }
        
    }

    struct dirent* entry;
    int nb_files = 0;

    while ((entry = readdir(dir)) != NULL && nb_files < MAX_FILES) {
        if (entry->d_type == DT_REG && endsWith(entry->d_name)) {
            nb_files++;
        }
    }

    closedir(dir);
    return nb_files;
}

char* EditAudioName(char* AudioName) {
    char* EditedName;
    if (strlen(AudioName) > 7) {
        if (strlen(AudioName) < 14) {
            EditedName = malloc(sizeof(char*)*7+7);
            strcpy(EditedName, AudioName);
        } else {
            EditedName = strdup(AudioName);
        }
        EditedName[7] = '\n';
        EditedName[13] = '\0';
        EditedName[8] = ' ';
        EditedName[9] = ' ';
        for (int i = 10; i < 13; i++)
        {
            EditedName[i] = '.';
        }
    } else {
        return AudioName;
    }

    return EditedName;
}

char* EditAudioNameForTopScreen(const char* AudioName) {
    int nb_car = strlen(AudioName);
    if (nb_car < 27) {
        return strdup(AudioName);
    }

    int nb_sn = (nb_car / 26);
    if (nb_car % 26 == 0) nb_sn--;

    int total_len = nb_car + nb_sn + 1;
    char* EditedName = malloc(total_len);
    if (!EditedName) return NULL;

    int j = 0;
    for (int i = 0; i < nb_car; i++) {
        if (i > 0 && i % 26 == 0) {
            EditedName[j++] = '\n';
        }
        EditedName[j++] = AudioName[i];
    }
    EditedName[j] = '\0';
    return EditedName;
}


void SetFilesName(const char* folder, char** AudioFiles, int nb_files) {
    DIR* dir = opendir(folder);
    if (!dir) {
        mkdir(AudioDirectory, 0777);
        dir = opendir(folder);
    }

    struct dirent* entry;
    int i = 0;

    while ((entry = readdir(dir)) != NULL && i < MAX_FILES) {
        if (entry->d_type == DT_REG && endsWith(entry->d_name)) {
            char* name_copy = strdup(entry->d_name);  // copy
            if (name_copy != NULL) {
                char* dot = strrchr(name_copy, '.');
                if (dot) *dot = '\0';
                AudioFiles[i] = strdup(name_copy);// to tab
                free(name_copy);
                i++;
            }
        }        
    }

    closedir(dir);
    return;
}

void SetFilesPos(int nb_files, TextBox* AudioFiles_pos) {
    float left = 0.0f;
    int j = 0;

    for (int i = 0; i < nb_files; i++)
    {
        AudioFiles_pos[i].x = (float)(i % 4)*BOX_NAME_LENGTH;
        left = (float)((int)((j/4)*BOX_NAME_WIDTH) % ((int)SCREEN_SIZE_WIDTH_BOT));
        AudioFiles_pos[i].y = left;
        j += 1;
    }
}

void DefineNumberFilesToPrint(int nb_files, int page, int* nb_audio_to_print, int* limit) {
    if (nb_files <= FILES_PER_SCREEN) {
        *nb_audio_to_print = nb_files;
        *limit = nb_files;
    } else if ((page+1)*FILES_PER_SCREEN <= nb_files) {
        *nb_audio_to_print = FILES_PER_SCREEN;
        *limit = FILES_PER_SCREEN*(page+1);
    } else {
        *nb_audio_to_print = nb_files % FILES_PER_SCREEN;
        *limit = nb_files;
    }
}