#include "common.h"
#include "Buffer.h"
#include "Graphics.h"
#include "sound.h"
#include "tools_functions.h"

void Specs() {
    CreateBuffer(&Dynamic_Name_Buf, &Dynamic_Name_Text, Remember, font);
    C2D_DrawText(&Dynamic_Name_Text, C2D_AtBaseline | C2D_WithColor, 5.0f, 18.0f, DEPTH, 0.4f, 0.4f, TEXT_COLOR);
    C2D_TextBufDelete(Dynamic_Name_Buf);
}

void NoFile(C3D_RenderTarget* top, C3D_RenderTarget* bottom) {
    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) {
            break;
        }

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        {
            C2D_SceneBegin(top);
            C2D_TargetClear(top, C2D_Color32(157, 150, 64, 255));

            CreateBuffer(&Dynamic_Name_Buf, &Dynamic_Name_Text, MissingFiles, font);
            C2D_DrawText(&Dynamic_Name_Text, C2D_AtBaseline | C2D_WithColor, 20.0f, SCREEN_SIZE_WIDTH/2-15.0f, DEPTH, 0.5f, 0.5f, TEXT_COLOR);
            C2D_TextBufDelete(Dynamic_Name_Buf);

            C2D_SceneBegin(bottom);
            C2D_TargetClear(bottom, C2D_Color32(157, 150, 64, 255));
            Specs();
                
        }
        C3D_FrameEnd(0);

        gspWaitForVBlank();
        gfxFlushBuffers();
    }
}

void HowToDumpDSP(C3D_RenderTarget* top, C3D_RenderTarget* bottom) {
    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) {
            break;
        }

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        {
            C2D_SceneBegin(top);
            C2D_TargetClear(top, C2D_Color32(157, 150, 64, 255));

            CreateBuffer(&Dynamic_Name_Buf, &Dynamic_Name_Text, DumpDSPfirm, font);
            C2D_DrawText(&Dynamic_Name_Text, C2D_AtBaseline | C2D_WithColor, 10.0f, SCREEN_SIZE_WIDTH/2-70.0f, DEPTH, 0.4f, 0.4f, TEXT_COLOR);
            C2D_TextBufDelete(Dynamic_Name_Buf);
        }
        C3D_FrameEnd(0);

        gspWaitForVBlank();
        gfxFlushBuffers();
    }
}

void NoDspfirm(C3D_RenderTarget* top, C3D_RenderTarget* bottom) {
    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) {
            break;
        }
        if (kDown & KEY_X) {
            HowToDumpDSP(top, bottom);
            break;
        }

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        {
            C2D_SceneBegin(top);
            C2D_TargetClear(top, C2D_Color32(157, 150, 64, 255));

            CreateBuffer(&Dynamic_Name_Buf, &Dynamic_Name_Text, HTDDSP, font);
            C2D_DrawText(&Dynamic_Name_Text, C2D_AtBaseline | C2D_WithColor, 15.0f, SCREEN_SIZE_WIDTH/2-75.0f, DEPTH, 0.5f, 0.5f, TEXT_COLOR);
            C2D_TextBufDelete(Dynamic_Name_Buf);

            C2D_SceneBegin(bottom);
            C2D_TargetClear(bottom, C2D_Color32(157, 150, 64, 255));
                
        }
        C3D_FrameEnd(0);

        gspWaitForVBlank();
        gfxFlushBuffers();
    }
}

int Game(C3D_RenderTarget* top, C3D_RenderTarget* bottom) {

    //consoleInit(GFX_TOP, NULL);

    InitSound();

    if (!CheckForDspfirm()) {
        NoDspfirm(top, bottom);
        return 1;
    }

    int nb_files = CountFiles(AudioDirectory, top, bottom);
    if (nb_files <= 0) {
        NoFile(top, bottom);
        //printf("\x1b[1;0H\x1b[33mNo audio file *.wav\n");
        return 1;
    }

    char** AudioFiles = malloc(nb_files * sizeof(char*));
    SetFilesName(AudioDirectory, AudioFiles, nb_files);

    TextBox* AudioFiles_pos = malloc(nb_files * sizeof(TextBox));
    SetFilesPos(nb_files, AudioFiles_pos);

    int specs = 0;
    int touch_pos_to_int;
    int i_file = 0;
    int page = 0;
    int nb_audio_to_print = 0;
    int limit = FILES_PER_SCREEN;
    int selected = 0;
    int max_page_int = nb_files / FILES_PER_SCREEN;

    if (nb_files % FILES_PER_SCREEN != 0){
        max_page_int += 1;
    }

    char* max_page = malloc(sizeof(char)*(floor(log10(abs(max_page_int))) + 1)+1);

    sprintf(max_page, "%d", max_page_int);

    char* print_page_str = PrintNbPage(page, max_page);
    
    while (aptMainLoop()) {
        hidScanInput();
        hidTouchRead(&touch);
        u32 kDown = hidKeysDown();
        //u32 kHeld = hidKeysHeld();

        DefineNumberFilesToPrint(nb_files, page, &nb_audio_to_print, &limit);

        if (kDown & KEY_START) {
            break;
        }
        if ((kDown & KEY_L) && (page > 0)) {
            page -= 1;
            i_file = 0;
            selected = 0;
            print_page_str = PrintNbPage(page, max_page);
        }
        if ((kDown & KEY_R) && (page < nb_files/(FILES_PER_SCREEN+1))) {
            page += 1;
            i_file = 0;
            selected = 0;
            print_page_str = PrintNbPage(page, max_page);
        }
        if ((kDown & KEY_LEFT) && (i_file > 0) && (i_file % 4 != 0)){
            i_file -= 1;
            selected = 90;
        }
        if ((kDown & KEY_RIGHT) && (i_file % 4 != 3) && (i_file + 1 < nb_audio_to_print)){
            i_file += 1;
            selected = 90;
        }
        if ((kDown & KEY_UP) && (i_file - 4 >= 0)){
            i_file -= 4;
            selected = 90;
        }
        if ((kDown & KEY_DOWN) && (i_file + 4 < nb_audio_to_print)){
            i_file += 4;
            selected = 90;
        }
        if (kDown & KEY_A) {
            PlayWavFromPath(AudioFiles[i_file + page*FILES_PER_SCREEN]);
        }
        if (kDown & KEY_X) {
            specs = 1;
        }
        if (kDown & KEY_B) {
            specs = 0;
        }

        if (kDown & KEY_TOUCH){
            touch_pos_to_int = IntFromTouchPos(touch.px, touch.py, page);
            if (touch_pos_to_int < nb_audio_to_print){
                //printf("%d", i_file);
                i_file = touch_pos_to_int;
                selected = 90;
                PlayWavFromPath(AudioFiles[i_file + page*FILES_PER_SCREEN]);
            }
        }

        consoleClear();
        //DiplayFiles(AudioFiles, nb_files);
        /*int debog = page*FILES_PER_SCREEN;
        printf("\x1b[20;0H\x1b[33mnb files : %d / page : %d, nb par page : %d", nb_files, page, nb_audio_to_print);
        printf("\x1b[21;0H\x1b[33mlimit : %d i : %d i_f : %d", limit, debog, i_file);
        printf("\x1b[22;0H\x1b[33mx : %d, y : %d", touch.px, touch.py);*/

        if (selected > 0) {
            selected -= 1;
        }

        char* file_name = EditAudioNameForTopScreen(AudioFiles[i_file + page*FILES_PER_SCREEN]);

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        {
            C2D_SceneBegin(bottom);
            C2D_TargetClear(bottom, C2D_Color32(157, 150, 64, 255));

            for (int i = 0; i < nb_audio_to_print; i++)
            {
                C2D_DrawRectSolid(AudioFiles_pos[i].x + 5.0f, AudioFiles_pos[i].y + 4.0f, DEPTH, BOX_NAME_LENGTH - 10.0f, BOX_NAME_WIDTH - 8.0f, C2D_Color32(0xFD, 0x6C, 0x9E, 255));
                C2D_DrawImageAt(images[0], AudioFiles_pos[i].x, AudioFiles_pos[i].y, DEPTH, NULL, 1.0f, 1.0f);
            }
            if (selected > 0) {
                C2D_DrawImageAt(images[1], AudioFiles_pos[i_file].x, AudioFiles_pos[i_file].y, DEPTH, NULL, 1.0f, 1.0f);
            }
            for (int i = (page*FILES_PER_SCREEN) ; i < limit; i++)
            {
                char* text_to_print = EditAudioName(AudioFiles[i]);
                CreateBuffer(&Dynamic_Name_Buf, &Dynamic_Name_Text, text_to_print, font);
                C2D_DrawText(&Dynamic_Name_Text, C2D_AtBaseline | C2D_WithColor, AudioFiles_pos[i].x+10.0f, AudioFiles_pos[i].y + 30.0f, DEPTH, FONT_SIZE_X, FONT_SIZE_Y, TEXT_COLOR);
                C2D_TextBufDelete(Dynamic_Name_Buf);
            }

            C2D_SceneBegin(top);
            C2D_TargetClear(top, C2D_Color32(157, 150, 64, 255));
            
            if (specs == 1) {
                Specs();

                CreateBuffer(&Dynamic_Name_Buf, &Dynamic_Name_Text, HTRTMM, font);
                C2D_DrawText(&Dynamic_Name_Text, C2D_AtBaseline | C2D_WithColor, 43.0f, SCREEN_SIZE_WIDTH-5.0f, DEPTH, FONT_SIZE_X, FONT_SIZE_Y, TEXT_COLOR);
                C2D_TextBufDelete(Dynamic_Name_Buf);

            } else {

                CreateBuffer(&Dynamic_Name_Buf, &Dynamic_Name_Text, HTSP, font);
                C2D_DrawText(&Dynamic_Name_Text, C2D_AtBaseline | C2D_WithColor, 18.0f, SCREEN_SIZE_WIDTH-5.0f, DEPTH, FONT_SIZE_X, FONT_SIZE_Y, TEXT_COLOR);
                C2D_TextBufDelete(Dynamic_Name_Buf);

                CreateBuffer(&Dynamic_Name_Buf, &Dynamic_Name_Text, "Selected file :", font);
                C2D_DrawText(&Dynamic_Name_Text, C2D_AtBaseline | C2D_WithColor, 5.0f, 20.0f, DEPTH, 0.5f, 0.5f, TEXT_COLOR);
                C2D_TextBufDelete(Dynamic_Name_Buf);
                
                CreateBuffer(&Dynamic_Name_Buf, &Dynamic_Name_Text, file_name, font);
                C2D_DrawText(&Dynamic_Name_Text, C2D_AtBaseline | C2D_WithColor, 15.0f, 50.0f, DEPTH, 0.5f, 0.5f, TEXT_COLOR);
                C2D_TextBufDelete(Dynamic_Name_Buf);

                CreateBuffer(&Dynamic_Name_Buf, &Dynamic_Name_Text, print_page_str, font);
                C2D_DrawText(&Dynamic_Name_Text, C2D_AtBaseline | C2D_WithColor, 5.0f, 220.0f, DEPTH, 0.4f, 0.4f, TEXT_COLOR);
                C2D_TextBufDelete(Dynamic_Name_Buf);
            }
        }
        C3D_FrameEnd(0);

        gspWaitForVBlank();
        gfxFlushBuffers();

    }

    for (int i = 0; i < nb_files; i++) {
        free(AudioFiles[i]);
    }
    free(AudioFiles);
    free(print_page_str);
    free(AudioFiles_pos);

    ExitSound();
    
    return 1;
}

int RenderWindow(C3D_RenderTarget* top, C3D_RenderTarget* bottom, int window) {
    if (window == 1) {
        int game = Game(top, bottom);
        if (game == 1) {
            return 1;
        }
    }
    return 0;
}