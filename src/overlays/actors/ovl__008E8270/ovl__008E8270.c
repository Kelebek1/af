#include "global.h"
#include "ovl__008E8270.h"

#define THIS ((ovl__008E8270*)thisx)

void func_80A20A1C_jp(Actor* thisx, PlayState* play);

extern UNK_TYPE D_6000000;
extern Gfx D_60000F0[];
extern Gfx D_60001A0[];
extern UNK_TYPE D_60003F0;

ActorInit _008E8270_Profile = {
    0x008B, 0x04,
    0x00000030,
    0x0000, 0x0006,
    0x000001DC,
    none_proc1,
    none_proc1,
    none_proc1,
    func_80A20A1C_jp,
    NULL,
};

Vec3f D_80A20C04_jp = {
    0.0f,
    0.0f,
    0.0f,
};

void func_80A207B0_jp(s16* arg0, s16 arg1, s16* arg2, s16 arg3) {
    if (arg1 < *arg0) {
        *arg2 -= 0x1E0;
        if (*arg2 < -arg3) {
            *arg2 = -arg3;
        }
    } else {
        *arg2 += 0x1E0;
        if (arg3 < *arg2) {
            *arg2 = arg3;
        }
    }
    *arg0 += *arg2;
}

typedef struct ovl__008E8270UnkStruct {
    /* 0x00 */ char unk00[0x90];
    /* 0x90 */ s16 unk_90;
    /* 0x92 */ s16 unk_92;
    /* 0x94 */ char unk94[0xC];
    /* 0xA0 */ s16 unk_A0;
    /* 0xA2 */ s16 unk_A2;
    /* 0xA4 */ char unkA4[0xC];
    /* 0xB0 */ s16 unk_B0;
    /* 0xB2 */ s16 unk_B2;
    /* 0xB4 */ char unkB4[0xC];
    /* 0xC0 */ s16 unk_C0;
    /* 0xC2 */ s16 unk_C2;
    /* 0xC4 */ char unkC4[0xC];
    /* 0xD0 */ char unkD0[0x20];
} ovl__008E8270UnkStruct; // size = 0xF0

void func_80A2083C_jp(ovl__008E8270UnkStruct* arg0, ovl__008E8270* this) {
    f32 cos;
    s16 sp4A = 0;
    Vec3f sp3C;
    f32 sin;
    s16 temp_s0;
    s16 temp_ft5;

    if (this->unk_1B8 == 1) {
        Matrix_put(&this->mtx);
        Matrix_translate(0.0f, 0.0f, 48.1f, 1);
        Matrix_Position(&D_80A20C04_jp, &sp3C);
        if (this->unk_1CC == 0) {
            xyz_t_move(&this->unk_1D0, &sp3C);
            this->unk_1CC = 1;
        } else {
            f32 x = sp3C.x - this->unk_1D0.x;
            f32 y = sp3C.y - this->unk_1D0.y;
            sp4A = atans_table(y, -x);
            xyz_t_move(&this->unk_1D0, &sp3C);
        }
    }

    mem_copy(arg0, Lib_SegmentedToVirtual(&D_6000000), 0xF0);
    func_80A207B0_jp(&this->unk_1C8, sp4A, &this->unk_1CA, 0x9C4);

    cos = cos_s(this->unk_1C8);
    temp_s0 = arg0->unk_90 * cos;
    sin = sin_s(this->unk_1C8);
    temp_ft5 = arg0->unk_90 * sin;

    arg0->unk_90 = temp_s0;
    arg0->unk_92 = temp_ft5;

    arg0->unk_C0 = temp_s0;
    arg0->unk_C2 = temp_ft5;

    cos = cos_s(this->unk_1C8);
    temp_s0 = arg0->unk_A0 * cos;
    sin = sin_s(this->unk_1C8);
    temp_ft5 = arg0->unk_A0 * sin;

    arg0->unk_A0 = temp_s0;
    arg0->unk_B0 = temp_s0;

    arg0->unk_A2 = temp_ft5;
    arg0->unk_B2 = temp_ft5;
}

void func_80A20A1C_jp(Actor* thisx, PlayState* play) {
    ovl__008E8270* this = THIS;
    s32 pad;
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    ovl__008E8270UnkStruct* sp48;

    OPEN_DISPS(gfxCtx);

    sp48 = GRAPH_ALLOC(gfxCtx, 0xF0);
    if (sp48 != NULL) {
        func_80A2083C_jp(sp48, this);
    } else {
        sp48 = Lib_SegmentedToVirtual(&D_6000000);
    }

    if (this->unk_1B8 == 1) {
        Matrix_put(&this->mtx);
        Matrix_Position(&D_80A20C04_jp, &this->actor.world.pos);
        this->unk_1B8 = 0;
    } else {
        Matrix_translate(this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0);
        Matrix_scale(0.01f, 0.01f, 0.01f, 1);
    }
    func_800BD5E8_jp(gfxCtx);

    if ((gfxCtx && gfxCtx) && gfxCtx)
    {
    }

    {
        Gfx* temp = POLY_OPA_DISP;
        s32 pad[1];

        gSPMatrix(temp++, _Matrix_to_Mtx_new(gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(temp++, D_60000F0);
        POLY_OPA_DISP = temp;
    }
    if (1) { } if (1) { } if (1) { } if (1) { } if (1) { } if (1) { }

    if (((!play) && (!play)) && (!play))
    {
    }

    Evw_Anime_Set(play, Lib_SegmentedToVirtual(&D_60003F0));
    func_800BD598_jp(gfxCtx);

    if (1) { } if (1) { } if (1) { } if (1) { } if (1) { } if (1) { }
    {
        s32 pad[1];
        Gfx* temp = POLY_XLU_DISP;
        s32 pad2[2];

        gSPMatrix(temp++, _Matrix_to_Mtx_new(gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPSegment(temp++, 0x0A, sp48);
        gSPDisplayList(temp++, D_60001A0);
        POLY_XLU_DISP = temp;
    }

    CLOSE_DISPS(gfxCtx);
}
