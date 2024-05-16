#ifndef AC_NPC_H
#define AC_NPC_H

#include "ultra64.h"
#include "m_actor.h"
#include "m_collision_obj.h"
#include "unk.h"
#include "m_clip.h"

struct Game_Play;
struct Npc;

typedef void (*NpcActionFunc)(struct Npc*, struct Game_Play*);
typedef void (*Npc_TALK_REQUEST_PROC)(struct Actor*, struct Game_Play*);
typedef int (*Npc_TALK_INIT_PROC)(struct Actor*, struct Game_Play*);
typedef int (*Npc_TALK_END_CHECK_PROC)(struct Actor*, struct Game_Play*);

typedef struct NpcActorInfo {
    /* 0x0 */ struct Animal_c* animal;
    /* 0x4 */ struct NpcList* list;
} NpcActorInfo; // size = 0x8

typedef struct NpcCondition {
    /* 0x00 */ u8 hide_flg;
    /* 0x01 */ u8 hide_request;
    /* 0x02 */ u8 action;
    /* 0x03 */ u8 talk_condition;
    /* 0x04 */ u8 greeting_flag;
    /* 0x05 */ u8 entrance_flag;
    /* 0x06 */ u16 fatigue;
    /* 0x08 */ s32 feel_tim;
    /* 0x0C */ s32 _0C;
    /* 0x10 */ u32 demo_flg;
    /* 0x14 */ u32 talk_demo_flg_save;
    /* 0x18 */ u32 trans_demo_flg_save;
    /* 0x1C */ u8 appear_flag;
    /* 0x1D */ u8 appear_rotation;
    /* 0x1E */ u8 pitfall_flag;
    /* 0x20 */ u32 actor_state_save;
    /* 0x24 */ s32 _24;
    /* 0x28 */ s32 _28;
    /* 0x2C */ s32 _2C;
    /* 0x30 */ s8* friendship;
    /* 0x34 */ s32 over_friendship;
    /* 0x38 */ u16* under_fg_p;
    /* 0x3C */ s32 ut_x;
    /* 0x40 */ s32 ut_z;
} NpcCondition; // size = ??

typedef struct NpcActorTalkInfo {
    /* 0x00 */ Npc_TALK_REQUEST_PROC talk_request_proc;
    /* 0x04 */ Npc_TALK_INIT_PROC talk_init_proc;
    /* 0x08 */ Npc_TALK_END_CHECK_PROC talk_end_check_proc;
    /* 0x0C */ u8 type;
    /* 0x0D */ u8 default_act;
    /* 0x0E */ u8 demo_code;
    /* 0x0F */ u8 turn;
    /* 0x10 */ s16 default_animation;
    /* 0x12 */ s16 default_turn_animation;
    /* 0x14 */ s16 melody_inst;
    /* 0x16 */ s16 npc_voice_id;
    /* 0x18 */ u8 feel;
    /* 0x19 */ u8 memory;
    /* 0x1A */ u8 kutipaku_timer; // frames of mouth movement animation
} NpcActorTalkInfo; // size = ??

typedef struct Npc {
    /* 0x000 */ Actor actor;
    /* 0x174 */ NpcActorInfo npcInfo;
    /* 0x17C */ UNK_TYPE1 unk_17C[0x58C];
    /* 0x708 */ s32 unk_708;
    /* 0x70C */ UNK_TYPE1 unk_70C[0xF0];
    /* 0x7FC */ NpcCondition condition;
    /* 0x840 */ UNK_TYPE1 unk_840[0xB0];
    /* 0x8F0 */ ClObjPipe collider;
    /* 0x90C */ UNK_TYPE1 unk_90C[0x10];
    /* 0x91C */ NpcActorTalkInfo talk_info;
} Npc; // size = 0x93C

#endif
