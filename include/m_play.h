#ifndef M_PLAY_H
#define M_PLAY_H

#include "ultra64.h"
#include "game.h"

typedef struct PlayState {
    /* 0x00000 */ GameState state;
    /* 0x000A4 */ char unk000A4[0x1DFC];
    /* 0x01EA0 */ u32 game_frame;
} PlayState; // size = ??

#endif
