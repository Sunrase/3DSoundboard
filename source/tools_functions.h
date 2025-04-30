#ifndef TOOLS_FUNCTIONS_H
#define TOOLS_FUNCTIONS_H

#include "common.h"

int IntFromTouchPos(int x, int y, int page);
char* SoundPath(const char* dir, const char* name, const char* ext);
char* PrintNbPage(int page, char* max_page);
bool CheckForDspfirm();
bool endsWith(const char* filename);
int CountFiles(const char* folder, C3D_RenderTarget* top, C3D_RenderTarget* bottom);
char* EditAudioName(char* AudioName);
char* EditAudioNameForTopScreen(char* AudioName);
void SetFilesName(const char* folder, char** AudioFiles, int nb_files);
void SetFilesPos(int nb_files, TextBox* AudioFiles_pos);
void DefineNumberFilesToPrint(int nb_files, int page, int* nb_audio_to_print, int* limit);

#endif