#include "common.h"

C2D_Image images[MAX_IMAGES];
C2D_SpriteSheet spriteSheet;
C2D_Font font;
C2D_TextBuf MissingFolder_Buf, Dynamic_Name_Buf;
C2D_Text MissingFolder_Text, Dynamic_Name_Text;
u8* audio_buffer;
ndspWaveBuf waveBuf;
touchPosition touch;

const char MissingFolder[] = "The folder '3DSoundboard'\n\n    has been created,\n\n   Press A to continue";
const char MissingFiles[] = "No audio file *.wav in :\n\n sdmc:/3ds/3DSoundboard\n\n  Press Start to exit";
const char Remember[] = "Audio file characteristics:\n\n - in *.wav format\n\n - sample rate set to\n\n   44100Hz\n\n - Channels set to mono\n\n\nIf you want to convert a\n\nfile such as mp3 or other\n\nformat, consider using\n\nAudacity ";
const char HTSP[] = "Press X to see audio files characteristics";
const char HTRTMM[] = "Press B to return on the main screen" ;
const char HTDDSP[] = "No file 'dspfirm.cdc' in :\n\nsdmc:/3ds, please dump it\n\nwith the rosalina menu\n\n\nPress X to see how to\n\ndump it\n\nPress Start to exit";
const char AudioDirectory[] = "/3ds/3DSoundboard";
const char DumpDSPfirm[] = "1 - Press L + Down + Select\n\n2 - Go to 'Miscellaneous options'\n\n3 - Dump DSP firmware\n\n\n\nIf you need to change the combo\n\nkey edit 'config.ini' in the\n\n'luma' folder";