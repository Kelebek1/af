#include "global.h"
#include "m_play.h"
#include "6A6E80.h"
#include "m_npc.h"
#include "m_handbill.h"
#include "m_field_make.h"
#include "m_malloc.h"
#include "m_msg_main.h"
#include "m_debug.h"
#include "m_player_lib.h"
#include "6F5550.h"
#include "6EC9E0.h"
#include "6EDD10.h"
#include "69E2C0.h"
#include "683030.h"
#include "6E30B0.h"
#include "6C97F0.h"
#include "6EFC20.h"
#include "67D890.h"
#include "6DA460.h"
#include "6ECD90.h"
#include "6BA500.h"
#include "libc64/qrand.h"
#include "m_controller.h"
#include "m_debug_display.h"
#include "6E0F50.h"
#include "6D2720.h"
#include "m_lights.h"
#include "m_skin_matrix.h"
#include "gfxalloc.h"
#include "version.h"
#include "libu64/gfxprint.h"
#include "libc/alloca.h"
#include "z_std_dma.h"
#include "67E840.h"
#include "overlays/gamestates/ovl_famicom_emu/famicom_emu.h"
#include "overlays/gamestates/ovl_trademark/m_trademark.h"
#include "6A7D20.h"
#include "m_scene_table.h"

void play_main(GameState* gameState);
void Gameplay_Scene_Read(PlayState* play, s16 arg1);
void Game_play_fbdemo_wipe_create(PlayState* play);
void Game_play_fbdemo_wipe_init(PlayState* play);
void Game_play_fbdemo_wipe_move(PlayState* play);

PlayStateUnkFuncsStruct D_80804320_jp = {
    fbdemo_wipe1_init,
    (void*)none_proc1,
    fbdemo_wipe1_move,
    fbdemo_wipe1_draw,
    fbdemo_wipe1_startup,
    fbdemo_wipe1_settype,
    fbdemo_wipe1_setcolor_rgba8888,
    NULL,
    fbdemo_wipe1_is_finish,
};

PlayStateUnkFuncsStruct D_80804344_jp = {
    fbdemo_triforce_init,
    (void*)none_proc1,
    fbdemo_triforce_move,
    fbdemo_triforce_draw,
    fbdemo_triforce_startup,
    fbdemo_triforce_settype,
    fbdemo_triforce_setcolor_rgba8888,
    NULL,
    fbdemo_triforce_is_finish,
};

PlayStateUnkFuncsStruct D_80804368_jp = {
    fbdemo_fade_init,
    (void*)none_proc1,
    fbdemo_fade_move,
    fbdemo_fade_draw,
    fbdemo_fade_startup,
    fbdemo_fade_settype,
    fbdemo_fade_setcolor_rgba8888,
    NULL,
    fbdemo_fade_is_finish,
};

PlayStateUnkFuncsStruct* D_8080438C_jp[] = {
    &D_80804320_jp, &D_80804344_jp, &D_80804368_jp, &D_80804368_jp, &D_80804344_jp, &D_80804344_jp, &D_80804368_jp,
};

typedef void (*PlayStateUnkFunc)(PlayState* play);
PlayStateUnkFunc D_808043A8_jp[] = {
    (void*)none_proc1,
    Game_play_fbdemo_wipe_create,
    Game_play_fbdemo_wipe_init,
    Game_play_fbdemo_wipe_move,
};

static Play80804480 fbdemo;
static void* B_8080455C_jp;
static u16 S_back_title_timer;
static u16 S_se_endcheck_timeout;

void Game_play_Reset_destiny(void) {
    CommonData10138_Sub* temp = &common_data.unk_10138->unk_A86;
    u8* one = &common_data.unk_1011F;
    u8* two = &common_data.unk_10121;

    if ((temp->unk_08 != 0) &&
        ((common_data.unk_10122 != temp->unk_06) || (*two != temp->unk_05) || (*one != temp->unk_03))) {
        temp->unk_08 = 0;
    }
}

void event_title_flag_on(void) {
    if (common_data.unk_107E4 != 0) {
        common_data.unk_10A82 = 1;
    }
}

void event_title_flag_off(void) {
    if (common_data.unk_107E4 == 2) {
        common_data.unk_107E4 = 0;
        common_data.unk_10A82 = 0;
    }
}

void Game_play_camera_proc(PlayState* play) {
    Camera2_ClearActorTalking_Cull(play);
    Camera2_process(play);
}

void Game_play_fbdemo_wipe_destroy(PlayState* play) {
    play->unk_1EE8.unk_21C.unk_04(&play->unk_1EE8, play);
    play->unk_1EE3 = 0;
    play->unk_1EE8.unk_218 = -1;
}

void Game_play_fbdemo_wipe_create_sub(PlayState* play) {
    s32 sp1C = play->unk_1EE1;

    bzero(&play->unk_1EE8, sizeof(PlayState_Unk_1EE8));
    if (sp1C >= 7) {
        sp1C = 1;
    }
    play->unk_1EE8.unk_218 = sp1C;

    {
        PlayStateUnkFuncsStruct* temp_v0 = D_8080438C_jp[sp1C];

        play->unk_1EE8.unk_21C.unk_00 = temp_v0->unk_00;
        play->unk_1EE8.unk_21C.unk_04 = temp_v0->unk_04;
        play->unk_1EE8.unk_21C.unk_08 = temp_v0->unk_08;
        play->unk_1EE8.unk_21C.unk_0C = temp_v0->unk_0C;
        play->unk_1EE8.unk_21C.unk_10 = temp_v0->unk_10;
        play->unk_1EE8.unk_21C.unk_14 = temp_v0->unk_14;
        play->unk_1EE8.unk_21C.unk_18 = temp_v0->unk_18;
        play->unk_1EE8.unk_21C.unk_20 = temp_v0->unk_20;
    }
}

void Game_play_fbdemo_wipe_create(PlayState* play) {
    Game_play_fbdemo_wipe_create_sub(play);
    Game_play_fbdemo_wipe_init(play);
}

void Game_play_fbdemo_wipe_init(PlayState* play) {
    PlayState_Unk_1EE8* temp_s0 = &play->unk_1EE8;
    u8 var_v0;

    play->unk_1EE8.unk_21C.unk_00(temp_s0);

    common_data.unk_1014A = 14;
    common_data.unk_10149 = 30;

    temp_s0->unk_21C.unk_18(temp_s0, 0);

    var_v0 = play->unk_1EE0;
    if (temp_s0->unk_218 == 4) {
        var_v0 |= 0x80;
    } else if (temp_s0->unk_218 == 5) {
        var_v0 |= 0x40;
    }

    temp_s0->unk_21C.unk_14(temp_s0, var_v0);
    temp_s0->unk_21C.unk_10(temp_s0);

    play->unk_1EE3 = 3;
    S_back_title_timer = 300;
    S_se_endcheck_timeout = 300;
}

void Game_play_fbdemo_fade_in_move_end(PlayState* play) {
    Game_play_fbdemo_wipe_destroy(play);
    if (play->unk_1EE2 == 3) {
        fbdemo_cleanup(&fbdemo);
        play->unk_1EE2 = 0;
        SetGameFrame(3);
    }
}

void Game_play_fbdemo_fade_out_start_emu_move_end(PlayState* play) {
    STOP_GAMESTATE(&play->state);
    SET_NEXT_GAMESTATE(&play->state, famicom_emu_init, sizeof(FamicomEmuState));
}

void Game_play_fbdemo_fade_out_game_end_move_end(PlayState* play) {
    STOP_GAMESTATE(&play->state);
    SET_NEXT_GAMESTATE(&play->state, trademark_init, sizeof(TrademarkState));
}

void Game_play_change_scene_move_end(PlayState* play) {
    game_goto_next_game_play(&play->state);
    common_data.unk_10004 = common_data.unk_00014;
    common_data.unk_00014 = play->unk_1E18;
}

void Game_play_fbdemo_wipe_move(PlayState* play) {
    UNUSED s32 pad[2];
    s16 sp26 = 0;
    s32 sp20 = 1;
    PlayState_Unk_1EE8* sp18 = &play->unk_1EE8;

    if (play->unk_1EE8.unk_21C.unk_20(sp18, play) != 0) {
        if ((play->unk_1EE0 != 1) && (play->unk_1EE0 != 11)) {
            if (S_se_endcheck_timeout != 0) {
                S_se_endcheck_timeout--;
            }

            if ((func_800D2334_jp(sp18, play) == 0) && (S_se_endcheck_timeout != 0)) {
                sp20 = 0;
            } else {
                func_800D2568_jp(1);
            }
        }

        if (play->unk_1EE0 == 11) {
            func_800D2568_jp(2);
        }

        if (sp20 == 1) {
            switch (play->unk_1EE0) {
                case 1:
                    Game_play_fbdemo_fade_in_move_end(play);
                    break;

                case 3:
                    //! FAKE
                    if (1) {}
                    if (1) {}
                    Game_play_fbdemo_fade_out_start_emu_move_end(play);
                    break;

                case 4:
                case 6:
                    if (S_back_title_timer == 300) {
                        mBGMPsComp_make_ps_wipe(0x168, play);
                    }

                case 5:
                case 10:
                    if (S_back_title_timer != 0) {
                        S_back_title_timer--;
                    }

                    if ((sAdo_BgmFadeoutCheck() == 1) || (S_back_title_timer == 0)) {
                        Game_play_fbdemo_fade_out_game_end_move_end(play);
                    } else {
                        sp26 = 1;
                    }
                    break;

                case 8:
                    if (common_data.unk_100E4 != NULL) {
                        if (*common_data.unk_100E4 != NULL) {
                            (*common_data.unk_100E4)(play);
                            Game_play_change_scene_move_end(play);
                        }
                    }
                    break;

                case 9:
                    if (S_back_title_timer != 0) {
                        S_back_title_timer--;
                    }

                    if ((sAdo_BgmFadeoutCheck() == 1) || (S_back_title_timer == 0)) {
                        Game_play_change_scene_move_end(play);
                    } else {
                        sp26 = 1;
                    }
                    break;

                default:
                    Game_play_change_scene_move_end(play);
                    break;
            }

            if (sp26 == 0) {
                play->unk_1EE0 = 0;
            }
        }
    } else {
        sp18->unk_21C.unk_08(sp18, game_GameFrame);
    }
}

void Game_play_fbdemo_wipe_proc(PlayState* play) {
    if ((play->unk_1EE3 == 0) && (play->unk_1EE0 != 0)) {
        play->unk_1EE3 = 1;
    }
    D_808043A8_jp[play->unk_1EE3](play);
}

Gfx* game_play_set_fog(PlayState* play, Gfx* gfx) {
    return gfx_set_fog_nosync(gfx, play->lightCtx.fogColor.r, play->lightCtx.fogColor.g, play->lightCtx.fogColor.b, 0,
                              play->lightCtx.fogNear, play->lightCtx.zFar);
}

void Game_play_fbdemo_proc(PlayState* play) {
    UNUSED s32 pad;
    GraphicsContext* gfxCtx = play->state.gfxCtx;

    switch (play->unk_1EE2) {
        case 2:
            if (fbdemo_init(&fbdemo, 10, 7, play) == 0) {
                play->unk_1EE2 = 0;
            } else {
                B_8080455C_jp = gfxCtx->unk_008;
                play->unk_1EE2 = 3;
                SetGameFrame(1);
            }
            break;
        case 3:
            fbdemo_move(&fbdemo);
            break;
    }
}

void play_cleanup(GameState* gameState) {
    PlayState* play = (PlayState*)gameState;

    func_800A3304_jp(play);
    play->state.gfxCtx->unk_2F4 = 0;
    play->state.gfxCtx->unk_2F8 = 0;
    play->unk_1CBC.unk_00 = 0;
    PreRender_cleanup(&play->unk_1DC0);
    CollisionCheck_dt(play, &play->unk_2138);

    if (play->unk_1EE2 == 3) {
        fbdemo_cleanup(&fbdemo);
        play->unk_1EE2 = 0;
    }

    if (play->unk_1EE3 == 3) {
        Game_play_fbdemo_wipe_destroy(play);
    }

    Actor_info_dt(&play->actorInfo, play);
    mEv_finish(&play->unk_1EBC);
    func_800AA124_jp();
    mSM_submenu_dt(&play->unk_1CBC);
    play->unk_1DAC = -1;
    mSM_submenu_ovlptr_cleanup(&play->unk_1CBC);
    mPlib_Object_Exchange_keep_Player_dt(play);
    mHsRm_GetHuusuiRoom(0, common_data.unk_10003);
    func_80087280_jp();
    zelda_CleanupArena();
}

void play_init(GameState* gameState) {
    PlayState* play = (PlayState*)gameState;
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    u32 temp_v1;
    u32 temp_v0_2;
    UNUSED s32 pad[2];
    u8 temp;
    s32 sp30;

    //! FAKE
    if (play && play && play) {}

    game_resize_hyral(&play->state, 0x7D0000);
    func_800D2568_jp(0);
    event_title_flag_on();
    func_800C9010_jp();
    mTM_set_season();
    func_800B594C_jp();
    func_800C49D4_jp(play);
    func_8007CFD8_jp(play);
    func_8007F858_jp(&play->unk_1EBC);
    initView(&play->unk_1938, gfxCtx);
    func_80064F48_jp(play);
    func_80077620_jp(play, &play->unk_2138);
    func_8006BB64_jp();
    func_8006C8D0_jp();
    play->unk_1DAC = -1;
    Gameplay_Scene_Read(play, common_data.unk_00014);
    mSM_submenu_ct(&play->unk_1CBC);
    play->unk_1CBC.unk_00 = 0;
    PreRender_init(&play->unk_1DC0);
    PreRender_setup_savebuf(&play->unk_1DC0, 0x140, 0xF0, 0, 0, 0);
    PreRender_setup_renderbuf(&play->unk_1DC0, 0x140, 0xF0, NULL, NULL);

    //! FAKE
    if (1) {}
    play->unk_1EE2 = 0;
    play->unk_1EE3 = 0;
    play->unk_1EE0 = 1;

    temp = 1;
    if (common_data.unk_1014B != 0xFF) {
        temp = common_data.unk_1014B;
        common_data.unk_1014B = 0xFF;
    }
    play->unk_1EE1 = temp;

    Pause_ct(&play->pause);
    new_Matrix(&play->state);
    play->state.main = play_main;
    play->state.destroy = play_cleanup;
    fbdemo_fade_init(&play->unk_2128);
    fbdemo_fade_settype(&play->unk_2128, 7);
    fbdemo_fade_setcolor_rgba8888(&play->unk_2128, 0xA0A0A0FF);
    fbdemo_fade_startup(&play->unk_2128);
    play->unk_220C = 0;
    sp30 = THA_getFreeBytes(&play->state.heap);
    temp_v0_2 = (u32)THA_alloc16(&play->state.heap, sp30);
    temp_v1 = ALIGN16(temp_v0_2);
    zelda_InitArena((void*)(temp_v1 & 0xFFFFFFFF), (sp30 - temp_v1) + temp_v0_2);
    func_800C2EE0_jp();
    func_80087004_jp();
    Actor_info_ct(play, &play->actorInfo, play->unk_1EA8);
    play->unk_2208 = (void*)none_proc1;
    mMsg_ct(play);
    mEv_2nd_init(&play->unk_1EBC);
    mTD_player_keydata_init(play);
    Balloon_init(play);
    func_800A65C4_jp();
    func_80059BF0_jp();
    func_800CBF80_jp();
    event_title_flag_off();
}

void Game_play_move_fbdemo_not_move(PlayState* play) {
    UNUSED s32 pad[2];

    play->state.unk_9D = 0x8F;
    play->state.unk_9C = 1;
    mSM_submenu_ctrl(play);
    if (play->unk_1CBC.unk_0C == 0) {
        play->state.unk_9C = 2;
        mDemo_Main(play);
        play->state.unk_9C = 3;
        mEv_run(&play->unk_1EBC);
    }
    play->state.unk_9C = 4;
    mDemo_stock_clear();
    play->state.unk_9C = 5;
    mSc_dmacopy_data_bank(play->unk_0110);
    play->state.unk_9C = 6;
    mSM_submenu_move(&play->unk_1CBC);
    if ((play->unk_1CBC.unk_0C == 0) && (REGADDR(IREG, 0x48) == 0)) {
        play->unk_1EA0++;
        play->state.unk_9C = 7;
        CollisionCheck_OC(play, &play->unk_2138);
        play->state.unk_9C = 8;
        CollisionCheck_clear(play, &play->unk_2138);
        play->state.unk_9D = 0x90;
        play->state.unk_9C = 0;
        Actor_info_call_actor(play, &play->actorInfo);
        play->state.unk_9D = 0x91;
        play->state.unk_9C = 1;
        mCoBG_CalcTimerDecalCircle();
        play->state.unk_9C = 2;
        mMsg_Main(play);
    }
    fbdemo_fade_move(&play->unk_2128, game_GameFrame);
}

void Game_play_move(PlayState* play) {
    PlayState_unk_0110* p = play->unk_0110;
    s32 var_v1;

    play->state.unk_9D = 0x8D;
    play->state.unk_9C = 1;
    Game_play_Reset_destiny();
    gSegments[4] = (uintptr_t)OS_K0_TO_PHYSICAL(p->segment);
    gSegments[2] = (uintptr_t)OS_K0_TO_PHYSICAL(play->unk_010C);
    play->state.unk_9C = 2;

    if (zurumode_flag >= 2) {
        var_v1 = Pause_proc(&play->pause, &play->state.input[1]) == 0;
    } else {
        var_v1 = 0;
    }

    play->state.unk_9D = 0x8E;
    play->state.unk_9C = 0;

    if (var_v1 == 0) {
        play->state.unk_9C = 1;
        Game_play_fbdemo_wipe_proc(play);
        play->state.unk_9C = 2;
        Game_play_fbdemo_proc(play);
        play->state.unk_9C = 3;
        if (play->unk_1EE2 != 3) {
            Game_play_move_fbdemo_not_move(play);
        }
    }

    if (play->unk_1CBC.unk_0C == 0) {
        play->state.unk_9D = 0x92;
        play->state.unk_9C = 1;
        Game_play_camera_proc(play);
        play->state.unk_9C = 2;
        mPO_business_proc(play);
        play->state.unk_9C = 3;
        mTRC_move(play);
        play->state.unk_9C = 4;
        Balloon_move(play);
    }

    play->state.unk_9D = 0x93;
    play->state.unk_9C = 1;
    Global_kankyo_set(play, &play->kankyo, &play->lightCtx);
    play->state.unk_9C = 2;
    mEnv_WindMove();
    play->state.unk_9C = 3;
    watch_my_step_move(play);
    play->state.unk_9C = 4;
    banti_move(play);
    play->state.unk_9C = 5;
    title_demo_move(play);
    play->state.unk_9D = 0x95;
    play->state.unk_9C = 0;
}

void func_80803810_jp(PlayState* play, GraphicsContext* gfxCtx) {
    void* temp_v0 = play->unk_0110[0].segment;

    gSegments[4] = (uintptr_t)OS_K0_TO_PHYSICAL(temp_v0);
    gSegments[2] = (uintptr_t)OS_K0_TO_PHYSICAL(play->unk_010C);

    OPEN_DISPS(gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x00, NULL);
    gSPSegment(POLY_XLU_DISP++, 0x00, NULL);
    gSPSegment(OVERLAY_DISP++, 0x00, NULL);
    gSPSegment(UNK_2B0_DISP++, 0x00, NULL);
    gSPSegment(UNK_2C0_DISP++, 0x00, NULL);
    gSPSegment(LIGHT_DISP++, 0x00, NULL);

    gSPSegment(POLY_OPA_DISP++, 0x04, temp_v0);
    gSPSegment(POLY_XLU_DISP++, 0x04, temp_v0);
    gSPSegment(OVERLAY_DISP++, 0x04, temp_v0);
    gSPSegment(UNK_2B0_DISP++, 0x04, temp_v0);
    gSPSegment(UNK_2C0_DISP++, 0x04, temp_v0);
    gSPSegment(LIGHT_DISP++, 0x04, temp_v0);

    gSPSegment(POLY_OPA_DISP++, 0x02, play->unk_010C);
    gSPSegment(POLY_XLU_DISP++, 0x02, play->unk_010C);
    gSPSegment(OVERLAY_DISP++, 0x02, play->unk_010C);
    gSPSegment(UNK_2B0_DISP++, 0x02, play->unk_010C);
    gSPSegment(UNK_2C0_DISP++, 0x02, play->unk_010C);
    gSPSegment(LIGHT_DISP++, 0x02, play->unk_010C);

    CLOSE_DISPS(gfxCtx);
}

void setupFog(PlayState* play, GraphicsContext* gfxCtx) {
    OPEN_DISPS(gfxCtx);

    POLY_OPA_DISP = game_play_set_fog(play, POLY_OPA_DISP);
    POLY_XLU_DISP = game_play_set_fog(play, POLY_XLU_DISP);

    CLOSE_DISPS(gfxCtx);
}

void setupViewer(PlayState* play) {
    showView(&play->unk_1938, 0xF, play);
}

void setupViewMatrix(PlayState* play, GraphicsContext* __gfxCtx, GraphicsContext* gfxCtx2) {
    Matrix_MtxtoMtxF(&play->unk_1938.unk_0A0, &play->billboardMtxF);
    Matrix_MtxtoMtxF(&play->unk_1938.unk_060, &play->viewProjectionMtxF);
    Skin_Matrix_MulMatrix(&play->viewProjectionMtxF, &play->billboardMtxF, &play->viewProjectionMtxF);

    play->billboardMtxF.mf[0][3] = 0.0f;
    play->billboardMtxF.mf[1][3] = 0.0f;
    play->billboardMtxF.mf[2][3] = 0.0f;
    play->billboardMtxF.mf[3][0] = 0.0f;
    play->billboardMtxF.mf[3][1] = 0.0f;
    play->billboardMtxF.mf[3][2] = 0.0f;

    Matrix_reverse(&play->billboardMtxF);

    play->unk_1E9C = _MtxF_to_Mtx(&play->billboardMtxF, (Mtx*)GRAPH_ALLOC(gfxCtx2, sizeof(MtxF) * 1));

    gSPSegment(POLY_OPA_DISP++, 0x01, play->unk_1E9C);
}

s32 makeBumpTexture(PlayState* play, GraphicsContext* __gfxCtx, GraphicsContext* gfxCtx2) {
    {
        Gfx* sp194;
        Gfx* sp190;
        sp190 = POLY_OPA_DISP;
        sp194 = gfxopen(sp190);

        gSPDisplayList(OVERLAY_DISP++, sp194);

        if (play->unk_1EE3 == 3) {
            PlayState1938 sp60;
            ScissorViewArg1 sp50;

            initView(&sp60, gfxCtx2);
            {
                sp60.unk_120 = 10;
            //! FAKE
            label2:;
            }
            sp50.unk_04 = 0xF0, sp50.unk_0C = 0x140;
            sp50.unk_00 = 0;
            sp50.unk_08 = 0;
            setScissorView(&sp60, &sp50);
            showView1(&sp60, 0xF, &sp194);
            play->unk_1EE8.unk_21C.unk_0C(&play->unk_1EE8, &sp194);
        }

        fbdemo_fade_draw(&play->unk_2128, &sp194);
        fade_rgba8888_draw(&sp194, play->unk_220C);

        gSPEndDisplayList(sp194++);

        gfxclose(sp190, sp194);
        POLY_OPA_DISP = sp194;
    }

    if (play->unk_1EE2 == 3) {
        Gfx* sp48 = POLY_OPA_DISP;

        fbdemo_draw(&fbdemo, &sp48);
        POLY_OPA_DISP = sp48;
        return 1;
    }

    PreRender_setup_renderbuf(&play->unk_1DC0, 0x140, 0xF0, gfxCtx2->unk_2E4, gfxCtx2->unk_008);
    if (play->unk_1CBC.unk_00 == 2) {
        func_800B0010_jp();
        PreRender_ConvertFrameBuffer_fg(&play->unk_1DC0);
        play->unk_1CBC.unk_00 = 3;
    } else if (play->unk_1CBC.unk_00 >= 5) {
        play->unk_1CBC.unk_00 = 0;
    }

    if (play->unk_1CBC.unk_00 == 3) {
        Gfx* sp44 = POLY_OPA_DISP;

        PreRender_loadFrameBufferCopy(&play->unk_1DC0, &sp44);
        POLY_OPA_DISP = sp44;
        return 1;
    }

    {
        Lights* sp40 = Global_light_read(&play->lightCtx, gfxCtx2);

        LightsN_list_check(sp40, play->lightCtx.listHead, NULL);
        LightsN_disp(sp40, gfxCtx2);
    }

    Actor_info_draw_actor(play, &play->actorInfo);
    Camera2_draw(play);
    mMsg_Draw(play);
    Debug_Display_output(play);

    if ((play->unk_1CBC.unk_00 == 1) || (play->unk_1EE2 == 1)) {
        Gfx* sp3C;
        Gfx* sp38;

        sp38 = POLY_OPA_DISP;
        sp3C = gfxopen(sp38);

        gSPDisplayList(OVERLAY_DISP++, sp3C);

        play->unk_1DC0.unk_10 = gfxCtx2->unk_2E4;
        play->unk_1DC0.unk_14 = gfxCtx2->unk_008;
        PreRender_saveFrameBuffer(&play->unk_1DC0, &sp3C);

        if (play->unk_1CBC.unk_00 == 1) {
            play->unk_1DC0.unk_18 = gfxCtx2->unk_2E4;
            PreRender_saveCVG(&play->unk_1DC0, &sp3C);
            play->unk_1CBC.unk_00 = 2;
        } else {
            play->unk_1EE2 = 2;
        }

        gSPEndDisplayList(sp3C++);

        gfxclose(sp38, sp3C);
        POLY_OPA_DISP = sp3C;

        REGADDR(SREG, 0x21) |= 1;
        return 0;
    }
    return 1;
}

void draw_version(GraphicsContext* gfxCtx) {
    Gfx* temp_s0;
    gfxprint* printer;

    OPEN_DISPS(gfxCtx);

    temp_s0 = func_800BD720_jp(OVERLAY_DISP);
    printer = (gfxprint*)alloca(sizeof(gfxprint));

    gfxprint_init(printer);
    gfxprint_open(printer, temp_s0);
    gfxprint_color(printer, 255, 155, 255, 255);
    gfxprint_color(printer, 255, 255, 255, 255);
    if (temp_s0 && temp_s0 && temp_s0) {}
    gfxprint_locate8x8(printer, 7, 0x1A);
    gfxprint_printf(printer, "[Creator:%s]", __Creator__);
    gfxprint_locate8x8(printer, 7, 0x1B);
    gfxprint_printf(printer, "[Date:%s]", __DateTime__);
    temp_s0 = gfxprint_close(printer);
    gfxprint_cleanup(printer);
    OVERLAY_DISP = temp_s0;

    CLOSE_DISPS(gfxCtx);
}

void Game_play_draw(PlayState* play) {
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    u8 sp2B = 0;
    u8 sp2A = 0;
    u8 sp29 = 0;

    //! FAKE
label:;
    func_80803810_jp(play, gfxCtx);
    if (common_data.unk_10001 == 0) {
        if (mEv_CheckTitleDemo() != -9) {
            sp2B = play->kankyo.unk_AE;
            sp2A = play->kankyo.unk_AF;
            sp29 = play->kankyo.unk_B0;
        }
    }

    DisplayList_initialize(gfxCtx, sp2B, sp2A, sp29, play);
    setupFog(play, gfxCtx);
    setupViewer(play);
    setupViewMatrix(play, gfxCtx, gfxCtx);

    //! FAKE
    if (1) {}
    if (1) {}
    if (1) {}

    if (makeBumpTexture(play, gfxCtx, gfxCtx) == 1) {
        watch_my_step_draw(play);
        banti_draw(play);
        mSM_submenu_draw(&play->unk_1CBC, play);
    }

    if (zurumode_flag != 0) {
        switch (mEv_CheckTitleDemo()) {
            case -9:
            case -1:
            default:
                draw_version(gfxCtx);
                break;

            case 0:
                break;
        }
    }
}

void play_main(GameState* gameState) {
    PlayState* play = (PlayState*)gameState;

    play->state.unk_9D = 0x6E;
    play->state.unk_9C = 0;
    fqrand();
    play->state.unk_9D = 0x78;
    play->state.unk_9C = 0;
    mCon_main(&play->state);
    play->state.unk_9D = 0x82;
    play->state.unk_9C = 0;
    Debug_Display_init();
    play->state.unk_9D = 0x8C;
    play->state.unk_9C = 0;
    Game_play_move(play);
    play->state.unk_9D = 0xAA;
    play->state.unk_9C = 0;
    Game_play_draw(play);
    play->state.unk_9D = 0xB4;
    play->state.unk_9C = 0;
    if (play->unk_1CBC.unk_00 != 2) {
        GraphicsContext* gfxCtx = play->state.gfxCtx;

        game_debug_draw_last(&play->state, gfxCtx);
        game_draw_last(gfxCtx);
        play->state.unk_9C = 0;
    }
    play->state.unk_9D = 0xBE;
}

void* func_80804138_jp(PlayState* play, Struct_8010EAA0* arg1) {
    u32 sp24 = arg1->unk_04 - arg1->unk_00;
    void* sp20 = THA_alloc16(&play->state.heap, sp24);

    func_80026E10_jp(sp20, arg1->unk_00, sp24, "../m_play.c", 2302);
    return sp20;
}

void func_808041A4_jp(PlayState* play) {
    Global_kankyo_ct(play, &play->kankyo);
}

void Gameplay_Scene_Init(PlayState* play) {
    play->unk_1EA8 = NULL;
    play->unk_1EA5 = 0;
    play->unk_1EA6 = 0;
    play->unk_1EA7 = 0;
    play->unk_1EB8 = 0;
    mSc_data_bank_ct(play, play->unk_0110);
    Global_light_ct(&play->lightCtx);
    Door_info_ct(&play->unk_1E10);
    common_data_clear();
    Scene_ct(play, play->unk_010C);
    mSc_decide_exchange_bank(play->unk_0110);
    func_808041A4_jp(play);
}

s32 mPl_SceneNo2SoundRoomType(s32 arg0) {
    switch (arg0) {
        case 20:
            return 1;
        case 6:
        case 9:
        case 12:
        case 14:
        case 18:
        case 21:
        case 31:
            return 2;
        case 17:
        case 22:
        case 23:
        case 24:
        case 25:
        case 29:
            return 3;
        default:
            return 0;
    }
}

void Gameplay_Scene_Read(PlayState* play, s16 arg1) {
    Struct_8010EAA0* sp1C = &scene_data_status[arg1];

    sp1C->unk_13 = 0;
    play->unk_2210 = sp1C;
    play->unk_00E0 = arg1;
    play->unk_010C = func_80804138_jp(play, sp1C);
    sp1C->unk_13 = 0;
    gSegments[2] = (uintptr_t)OS_K0_TO_PHYSICAL(play->unk_010C);
    Gameplay_Scene_Init(play);
    sAdo_RoomType(mPl_SceneNo2SoundRoomType(common_data.unk_00014));
}
