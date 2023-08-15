#ifndef Evw_ANIME_H
#define Evw_ANIME_H

#include "ultra64.h"
#include "m_play.h"

#define EVW_COLOR_BUF_MAX 50

enum {
  EVW_ANIME_TYPE_SCROLL1,
  EVW_ANIME_TYPE_SCROLL2,
  EVW_ANIME_TYPE_COLREG_MANUAL,
  EVW_ANIME_TYPE_COLREG_LINEAR,
  EVW_ANIME_TYPE_COLREG_NONLINEAR,
  EVW_ANIME_TYPE_TEXANIME,

  EVW_ANIME_TYPE_NUM
};

typedef struct EvwAnimeColPrim {
    /* 0x00 */ u8 r;
    /* 0x01 */ u8 g;
    /* 0x02 */ u8 b;
    /* 0x03 */ u8 a;
    /* 0x04 */ u8 l;
} EvwAnimeColPrim; // size = 0x05

typedef struct EvwAnimeColEnv {
    /* 0x00 */ u8 r;
    /* 0x01 */ u8 g;
    /* 0x02 */ u8 b;
    /* 0x03 */ u8 a;
} EvwAnimeColEnv; // size = 0x4

typedef struct EvwAnimeColReg {
    /* 0x00 */ u16 frame_count; /* total frames in the animation */
    /* 0x02 */ u16 key_count; /* number of animation keyframes */

    /* 0x04 */ EvwAnimeColPrim* prim_colors; /* list of primitive colors per-keyframe */
    /* 0x08 */ EvwAnimeColEnv* env_colors; /* list of environment colors per-keyframe */

    /* 0x0C */ u16* keyframes; /* list of keyframe frame indices */
} EvwAnimeColReg; // size = 0x10

typedef struct EvwAnimeScroll {
    /* 0x00 */ s8 x; /* x (s) texture position */
    /* 0x01 */ s8 y; /* y (t) texture position */
    /* 0x02 */ u8 width; /* texture width */
    /* 0x03 */ u8 height; /* texture height */
} EvwAnimeScroll; // size = 0x4

typedef struct EvwAnimeTexAnime {
    /* 0x00 */ u16 frame_count; /* total frames in the animation */
    /* 0x02 */ u16 key_count; /* number of animation keyframes */

    /* 0x04 */ void** texture_tbl; /* list of each texture */
    /* 0x08 */ u8* animation_pattern; /* list of which texture to use per keyframe (runs at 30 FPS) */

    /* 0x0C */ u16* keyframes; /* list of keyframe frame indices */
} EvwAnimeTexAnime; // size = 0x10

typedef struct EvwAnimeData {
    /* 0x00 */ s8 segment; /* Negative segments signal the end of the animation data array */
    /* 0x02 */ s16 type;
    /* 0x04 */ void* data_p;
} EvwAnimeData; // size = 0x8

s32 func_800549C0_jp(PlayState* play, EvwAnimeScroll* arg1);
void func_80054A28_jp(PlayState* play, s32 arg1, void* arg2);
// void func_80054A94_jp();
// void func_80054B44_jp();
// void func_80054BB0_jp();
// void func_80054C88_jp();
// void morf_calc();
// void func_80054D3C_jp();
// void func_80054F10_jp();
// void func_80055214_jp();
// void func_80055248_jp();
// void func_800554D8_jp();
void Evw_Anime_Set(PlayState* play, EvwAnimeData* evw_anime_data);

#endif
