#include "common.h"
#include "sound.h"
#include "Buffer.h"
#include "tools_functions.h"

void InitSound() {
    ndspInit();
    dspInit();
}

void ExitSound() {
    linearFree(audio_buffer);
    ndspExit();
    dspExit();
}

bool LoadWav(const char* path, u8** buffer, u32* size) {
    FILE* f = fopen(path, "rb");
    if (!f) return false;

    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    rewind(f);

    *buffer = linearAlloc(*size);
    if (!*buffer) {
        fclose(f);
        return false;
    }

    fread(*buffer, 1, *size, f);
    fclose(f);
    return true;
}

void PlayWav(const char* path) {
    u32 size;
    if (!LoadWav(path, &audio_buffer, &size)) {
        //printf("NO SOUND FOUND");
        return;
    }

    ndspChnReset(0);
    ndspChnSetInterp(0, NDSP_INTERP_POLYPHASE);
    ndspChnSetRate(0, 44100.0f);
    ndspChnSetFormat(0, NDSP_FORMAT_MONO_PCM16);

    memset(&waveBuf, 0, sizeof(ndspWaveBuf));
    waveBuf.data_vaddr = audio_buffer + 44;
    waveBuf.nsamples = (size - 44) / 2;
    waveBuf.looping = false;

    DSP_FlushDataCache(waveBuf.data_vaddr, size - 44);
    ndspChnWaveBufAdd(0, &waveBuf);
}

void PlayWavFromPath(const char *name) {
    char* path = SoundPath(AudioDirectory, name, ".wav");
    PlayWav(path);
}