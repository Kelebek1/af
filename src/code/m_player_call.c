#include "ultra64.h"
#include "m_actor.h"
#include "m_actor_dlftbls.h"
#include "m_object.h"
#include "overlays/gamestates/ovl_play/m_play.h"
#include "overlays/actors/player_actor/m_player.h"

ActorFunc Player_actor_ct_func;
ActorFunc Player_actor_dt_func;
ActorFunc Player_actor_move_func;
ActorFunc Player_actor_draw_func;

void Player_actor_ct_call(Actor* thisx, Game_Play* gamePlay);
void Player_actor_dt_call(Actor* thisx, Game_Play* gamePlay);
void Player_actor_move_call(Actor* thisx, Game_Play* gamePlay);
void Player_actor_draw_call(Actor* thisx, Game_Play* gamePlay);

ActorProfile Player_Profile = {
    /* */ ACTOR_PLAYER,
    /* */ ACTOR_PART_PLAYER,
    /* */ ACTOR_FLAG_1 | ACTOR_FLAG_4 | ACTOR_FLAG_10 | ACTOR_FLAG_20 | ACTOR_FLAG_2000000 | ACTOR_FLAG_4000000 |
        ACTOR_FLAG_20000000,
    /* */ 0,
    /* */ GAMEPLAY_KEEP,
    /* */ sizeof(Player),
    /* */ Player_actor_ct_call,
    /* */ Player_actor_dt_call,
    /* */ Player_actor_move_call,
    /* */ Player_actor_draw_call,
    /* */ NULL,
};

void initfunc(Game_Play* gamePlay) {
    Player_actor_ct_func = mSM_ovlptr_dllcnv(Player_actor_ct, &gamePlay->submenu);
    Player_actor_dt_func = mSM_ovlptr_dllcnv(Player_actor_dt, &gamePlay->submenu);
    Player_actor_move_func = mSM_ovlptr_dllcnv(Player_actor_move, &gamePlay->submenu);
    Player_actor_draw_func = mSM_ovlptr_dllcnv(Player_actor_draw, &gamePlay->submenu);
}

void Player_actor_ct_call(Actor* thisx, Game_Play* gamePlay) {
    load_player(&gamePlay->submenu);
    initfunc(gamePlay);
    Player_actor_ct_func(thisx, gamePlay);
}

void Player_actor_dt_call(Actor* thisx, Game_Play* gamePlay) {
    load_player(&gamePlay->submenu);
    Player_actor_dt_func(thisx, gamePlay);
}

void Player_actor_move_call(Actor* thisx, Game_Play* gamePlay) {
    load_player(&gamePlay->submenu);
    Player_actor_move_func(thisx, gamePlay);
}

void Player_actor_draw_call(Actor* thisx, Game_Play* gamePlay) {
    load_player(&gamePlay->submenu);
    Player_actor_draw_func(thisx, gamePlay);
}
