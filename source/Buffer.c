#include "common.h"
#include "Buffer.h"

void CreateBuffer(C2D_TextBuf *g_Buffer, C2D_Text *g_Text, const char* Text, C2D_Font font) {
    *g_Buffer = C2D_TextBufNew(strlen(Text) + 1);
    C2D_TextFontParse(g_Text, font, *g_Buffer, Text);
    C2D_TextOptimize(g_Text);
}