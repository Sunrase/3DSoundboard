#include "common.h"
#include "Graphics.h"
#include "game_functions.h"

int main(int argc, char **argv) {

    // Target render for top and bottom screen
    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    int game_mode = 1;

    // Initialisation
    initGraphics();

    // Load Images
    loadImages(spriteSheet, images, MAX_IMAGES, font);

    // Load Font
    LoadFont(&font);

    //Main Loop
    while (aptMainLoop()) {
        int render = RenderWindow(top, bottom, game_mode);
        if (render == 1) break;
        gfxFlushBuffers();
    }

    exitGraphics();

    return 0;
}