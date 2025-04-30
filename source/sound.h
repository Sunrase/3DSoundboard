#ifndef SOUND_H
#define SOUND_H

void InitSound();
void ExitSound();
bool LoadWav(const char* path, u8** buffer, u32* size);
void PlayWav(const char* path);
void PlayWavFromPath(const char *name);

#endif