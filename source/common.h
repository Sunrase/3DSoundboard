#ifndef COMMON_H
#define COMMON_H

#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <citro3d.h>
#include <citro2d.h>
#include <3ds/ndsp/ndsp.h>
#include <3ds/services/fs.h>
#include <3ds/srv.h>
#include <3ds/synchronization.h>
#include <3ds/services/dsp.h>

#define MAX_IMAGES 2
#define SCREEN_SIZE_WIDTH   240.0f
#define SCREEN_SIZE_LENGTH  400.0f
#define SCREEN_SIZE_WIDTH_BOT   240.0f
#define SCREEN_SIZE_LENGTH_BOT  320.0f
#define FONT_SIZE_X 0.3f
#define FONT_SIZE_Y 0.3f
#define DEPTH 1.0f
#define MAX_FILES 64
#define FILES_PER_SCREEN 16
#define NB_BOX_X 4
#define NB_BOX_Y 4
#define BOX_NAME_WIDTH 60.0f
#define BOX_NAME_LENGTH 80.0f
#define TEXT_COLOR C2D_Color32(0x36, 0x3c, 0x48, 0xFF)

typedef struct {
    float x;
    float y;
} TextBox;

extern C2D_Image images[MAX_IMAGES];
extern C2D_SpriteSheet spriteSheet;
extern C2D_Font font;
extern C2D_TextBuf Dynamic_Name_Buf;
extern C2D_Text Dynamic_Name_Text;
extern u8* audio_buffer;
extern ndspWaveBuf waveBuf;
extern touchPosition touch;

extern const char MissingFolder[];
extern const char MissingFiles[];
extern const char Remember[];
extern const char HTSP[];
extern const char HTRTMM[];
extern const char HTDDSP[];
extern const char AudioDirectory[];
extern const char DumpDSPfirm[];

#endif