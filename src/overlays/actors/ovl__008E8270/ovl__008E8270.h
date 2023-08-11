#ifndef OVL_008E8270_H
#define OVL_008E8270_H

#include "ultra64.h"
#include "m_actor.h"
#include "m_play.h"
#include "evw_anime.h"
#include "sys_matrix.h"
#include "sys_math_atan.h"
#include "6E0F50.h"
#include "m_lib.h"
#include "gfx.h"


typedef struct ovl__008E8270 {
    /* 0x000 */ Actor actor;
    /* 0x174 */ char unk174[0x4];
    /* 0x178 */ MtxF mtx;
    /* 0x1B8 */ s32 unk_1B8;
    /* 0x1BC */ char unk1BC[0xC];
    /* 0x1C8 */ s16 unk_1C8;
    /* 0x1CA */ s16 unk_1CA;
    /* 0x1CC */ s32 unk_1CC;
    /* 0x1D0 */ Vec3f unk_1D0;
} ovl__008E8270; // size = 0x1DC

#endif
