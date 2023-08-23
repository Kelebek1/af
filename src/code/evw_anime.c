#include "global.h"
#include "evw_anime.h"
#include "m_lib.h"
#include "macros.h"
#include "gfx.h"
#include "6E0F50.h"

s32 func_800549C0_jp(PlayState* play, EvwAnimeScroll* scroll) {
    return func_800BDA20_jp(play->game.gfxCtx, scroll->x * play->game_frame, -(scroll->y * play->game_frame), scroll->width, scroll->height);
}

void func_80054A28_jp(PlayState* play, s32 arg1, void* scroll) {
    s32 temp_v0 = func_800549C0_jp(play, (EvwAnimeScroll*)scroll);

    OPEN_DISPS(play->game.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, arg1, temp_v0);
    gSPSegment(POLY_XLU_DISP++, arg1, temp_v0);

    CLOSE_DISPS(play->game.gfxCtx);
}

s32 func_80054A94_jp(PlayState* play, EvwAnimeScroll* scrolls) {
    return two_tex_scroll(play->game.gfxCtx, 0, 
                      scrolls[0].x * play->game_frame, -(scrolls[0].y * play->game_frame), 
                      scrolls[0].width, scrolls[0].height, 1, 
                      scrolls[1].x * play->game_frame, -(scrolls[1].y * play->game_frame), 
                      scrolls[1].width, scrolls[1].height);
}

void func_80054B44_jp(PlayState* play, s32 arg1, void* scrolls) {
    s32 temp_v0 = func_80054A94_jp(play, (EvwAnimeScroll*)scrolls);

    OPEN_DISPS(play->game.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, arg1, temp_v0);
    gSPSegment(POLY_XLU_DISP++, arg1, temp_v0);

    CLOSE_DISPS(play->game.gfxCtx);
}

void func_80054BB0_jp(PlayState* play, s32 arg1, EvwAnimeColPrim* prim, EvwAnimeColEnv* env) {
    Gfx* alloc = (Gfx*)GRAPH_ALLOC(play->game.gfxCtx, sizeof(Gfx) * 3);

    gDPSetPrimColor(&alloc[0], 0, prim->l, prim->r, prim->g, prim->b, prim->a);
    gDPSetEnvColor(&alloc[1], env->r, env->g, env->b, env->a);
    gSPEndDisplayList(&alloc[2]);

    OPEN_DISPS(play->game.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, arg1, alloc);
    gSPSegment(POLY_XLU_DISP++, arg1, alloc);

    CLOSE_DISPS(play->game.gfxCtx);
}

void func_80054C88_jp(PlayState* play, s32 arg1, void* evw_data) {
    EvwAnimeColReg* color_reg = (EvwAnimeColReg*)evw_data;
    EvwAnimeColPrim* sp18 = (EvwAnimeColPrim*)Lib_SegmentedToVirtual(color_reg->prim_colors);
    EvwAnimeColEnv* sp1C = (EvwAnimeColEnv*)Lib_SegmentedToVirtual(color_reg->env_colors);
    s32 temp_hi = play->game_frame % color_reg->frame_count;

    sp18 += temp_hi;
    sp1C += temp_hi;

    func_80054BB0_jp(play, arg1, sp18, sp1C);
}

s32 morf_calc(s32 arg0, s32 arg1, f32 arg2) {
    return arg0 + (s32)((arg1 - arg0) * arg2);
}

void func_80054D3C_jp(PlayState* play, s32 arg1, void* evw_data) {
    EvwAnimeColReg* color_reg = (EvwAnimeColReg*)evw_data;
    EvwAnimeColPrim* last_prim;
    EvwAnimeColEnv* last_env;
    s32 i;
    EvwAnimeColPrim* temp_s1 = Lib_SegmentedToVirtual(color_reg->prim_colors);
    EvwAnimeColEnv* temp_s2 = Lib_SegmentedToVirtual(color_reg->env_colors);
    u16* var_v1 = (u16*)Lib_SegmentedToVirtual(color_reg->keyframes) + 1;
    s32 temp_hi = play->game_frame % color_reg->frame_count;
    UNUSED s32 pad[3];
    EvwAnimeColPrim sp44;
    EvwAnimeColEnv sp40;
    f32 temp_fs0;
    s32 a;

    for (i = 1; i < color_reg->key_count; i++) {
        if (temp_hi < *var_v1) {
            break;
        }
        var_v1++;
    }

    temp_s1 += i;
    temp_s2 += i;

    a = var_v1[0] - var_v1[-1];

    last_prim = temp_s1 - 1;
    last_env = temp_s2 - 1;

    temp_fs0 = (f32)(temp_hi - var_v1[-1]) / a;

    sp44.r = morf_calc(last_prim->r, temp_s1[0].r, temp_fs0);
    sp44.g = morf_calc(last_prim->g, temp_s1[0].g, temp_fs0);
    sp44.b = morf_calc(last_prim->b, temp_s1[0].b, temp_fs0);
    sp44.a = morf_calc(last_prim->a, temp_s1[0].a, temp_fs0);
    sp44.l = morf_calc(last_prim->l, temp_s1[0].l, temp_fs0);

    sp40.r = morf_calc(last_env->r, temp_s2[0].r, temp_fs0);
    sp40.g = morf_calc(last_env->g, temp_s2[0].g, temp_fs0);
    sp40.b = morf_calc(last_env->b, temp_s2[0].b, temp_fs0);
    sp40.a = morf_calc(last_env->a, temp_s2[0].a, temp_fs0);

    func_80054BB0_jp(play, arg1, &sp44, &sp40);
}

f32 func_80054F10_jp(s32 num_keyframes, f32* keyframes, f32* values, f32 now_frame) {
  f32 stack[EVW_COLOR_BUF_MAX];
  f32 res;
  f32 n;
  f32 m;
  s32 i;
  s32 j;

  /* BUG: fixed stack buffer size but no input length validation. */
  for (i = 0; i < num_keyframes; i++) {
    n = keyframes[i];
    m = 1.0f;

    /* Adjust keyframe by difference between all previous keyframes and the current keyframe */
    for (j = 0; j < i; j++) {
      m *= n - keyframes[j];
    }

    j++;
    for (; j < num_keyframes; j++) {
      m *= n - keyframes[j];
    }

    stack[i] = values[i] / m;
  }

  res = 0.0f;
  for (i = 0; i < num_keyframes; i++) {
    m = 1.0f;

    for (j = 0; j < i; j++) {
      m *= now_frame - keyframes[j];
    }

    j++;
    for (; j < num_keyframes; j++) {
      m *= now_frame - keyframes[j];
    }

    res += stack[i] * m;
  }

  return res;
}

u8 func_80055214_jp(s32 num_keyframes, f32* keyframes, f32* values, f32 now_frame) {
    return (s32)func_80054F10_jp(num_keyframes, keyframes, values, now_frame);
}

typedef struct EvwAnimeColReg_F {
    /* 0x000 */ f32 keyframe_stack[EVW_COLOR_BUF_MAX];

    /* 0x0C8 */ f32 prim_r_stack[EVW_COLOR_BUF_MAX];
    /* 0x190 */ f32 prim_g_stack[EVW_COLOR_BUF_MAX];
    /* 0x258 */ f32 prim_b_stack[EVW_COLOR_BUF_MAX];
    /* 0x320 */ f32 prim_a_stack[EVW_COLOR_BUF_MAX];
    /* 0x3E8 */ f32 prim_l_stack[EVW_COLOR_BUF_MAX];

    /* 0x4B0 */ f32 env_r_stack[EVW_COLOR_BUF_MAX];
    /* 0x578 */ f32 env_g_stack[EVW_COLOR_BUF_MAX];
    /* 0x640 */ f32 env_b_stack[EVW_COLOR_BUF_MAX];
    /* 0x708 */ f32 env_a_stack[EVW_COLOR_BUF_MAX];
} EvwAnimeColReg_F; // size = 0x7D0

void func_80055248_jp(PlayState* play, s32 arg1, void* evw_data) {
    EvwAnimeColReg* color_reg = (EvwAnimeColReg*)evw_data;
    EvwAnimeColPrim* prim_src = Lib_SegmentedToVirtual(color_reg->prim_colors);
    EvwAnimeColEnv* env_src = Lib_SegmentedToVirtual(color_reg->env_colors);
    u16* keyframes = Lib_SegmentedToVirtual(color_reg->keyframes);
    f32 now_frame = play->game_frame % color_reg->frame_count;
    EvwAnimeColPrim nonlinear_prim;
    EvwAnimeColEnv nonlinear_env;
    EvwAnimeColReg_F sp40;
    s32 i;

    for (i = 0; i < color_reg->key_count; i++) {
        sp40.keyframe_stack[i] = (s32)keyframes[i];

        sp40.prim_r_stack[i] = (s32)prim_src[i].r;
        sp40.prim_g_stack[i] = (s32)prim_src[i].g;
        sp40.prim_b_stack[i] = (s32)prim_src[i].b;
        sp40.prim_a_stack[i] = (s32)prim_src[i].a;
        sp40.prim_l_stack[i] = (s32)prim_src[i].l;

        sp40.env_r_stack[i] = (s32)env_src[i].r;
        sp40.env_g_stack[i] = (s32)env_src[i].g;
        sp40.env_b_stack[i] = (s32)env_src[i].b;
        sp40.env_a_stack[i] = (s32)env_src[i].a;
    }

    nonlinear_prim.r = func_80055214_jp(color_reg->key_count, sp40.keyframe_stack, sp40.prim_r_stack, now_frame);
    nonlinear_prim.g = func_80055214_jp(color_reg->key_count, sp40.keyframe_stack, sp40.prim_g_stack, now_frame);
    nonlinear_prim.b = func_80055214_jp(color_reg->key_count, sp40.keyframe_stack, sp40.prim_b_stack, now_frame);
    nonlinear_prim.a = func_80055214_jp(color_reg->key_count, sp40.keyframe_stack, sp40.prim_a_stack, now_frame);
    nonlinear_prim.l = func_80055214_jp(color_reg->key_count, sp40.keyframe_stack, sp40.prim_l_stack, now_frame);

    nonlinear_env.r = func_80055214_jp(color_reg->key_count, sp40.keyframe_stack, sp40.env_r_stack, now_frame);
    nonlinear_env.g = func_80055214_jp(color_reg->key_count, sp40.keyframe_stack, sp40.env_g_stack, now_frame);
    nonlinear_env.b = func_80055214_jp(color_reg->key_count, sp40.keyframe_stack, sp40.env_b_stack, now_frame);
    nonlinear_env.a = func_80055214_jp(color_reg->key_count, sp40.keyframe_stack, sp40.env_a_stack, now_frame);

    func_80054BB0_jp(play, arg1, &nonlinear_prim, &nonlinear_env);
}

void func_800554D8_jp(PlayState* play, s32 arg1, void* evw_data) {
    EvwAnimeTexAnime* tex_anime = (EvwAnimeTexAnime*)evw_data;
    void** sp18 = Lib_SegmentedToVirtual(tex_anime->texture_tbl);
    u8* anim_pattern = Lib_SegmentedToVirtual(tex_anime->animation_pattern);
    s32 frame = play->game_frame % tex_anime->frame_count;
    void* tex_p = Lib_SegmentedToVirtual(sp18[anim_pattern[frame]]);

    OPEN_DISPS(play->game.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, arg1, tex_p);
    gSPSegment(POLY_XLU_DISP++, arg1, tex_p);

    CLOSE_DISPS(play->game.gfxCtx);
}

typedef void (*EvwAnimeProc)(PlayState*, s32, void*);

void Evw_Anime_Set(PlayState* play, EvwAnimeData* evw_anime_data) {
    static EvwAnimeProc D_80100C30_jp[] = {
        func_80054A28_jp,
        func_80054B44_jp,
        func_80054C88_jp,
        func_80054D3C_jp,
        func_80055248_jp,
        func_800554D8_jp,
    };

    if (evw_anime_data != NULL) {
        s32 segment = evw_anime_data->segment;
        s32 index;

        if (evw_anime_data->segment != 0) {
            do {
                segment = evw_anime_data->segment;
                index = ABS(segment) + 7;

                D_80100C30_jp[evw_anime_data->type](play, index, Lib_SegmentedToVirtual(evw_anime_data->data_p));
                
                evw_anime_data++;
            } while (segment >= 0);
        }
        
    }
}

