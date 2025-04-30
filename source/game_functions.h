#ifndef GAME_FUNCTIONS
#define GAME_FUNCTIONS

#include "common.h"

void Specs();
void NoFile(C3D_RenderTarget* top, C3D_RenderTarget* bottom);
void HowToDumpDSP(C3D_RenderTarget* top, C3D_RenderTarget* bottom);
void NoDspfirm(C3D_RenderTarget* top, C3D_RenderTarget* bottom);
int Game(C3D_RenderTarget* top, C3D_RenderTarget* bottom);
int RenderWindow(C3D_RenderTarget* top, C3D_RenderTarget* bottom, int window);

#endif