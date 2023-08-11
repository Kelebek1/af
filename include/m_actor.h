#ifndef M_ACTOR_H
#define M_ACTOR_H

#include "ultra64.h"
#include "z64math.h"

struct Actor;
struct PlayState;

typedef void(*ActorFunc)(struct Actor*, struct PlayState*);

typedef struct ActorInit {
    /* 0x00 */ s16 id;
    /* 0x02 */ u8 part;
    /* 0x04 */ u32 initial_flags_state;
    /* 0x08 */ u16 npc_id;
    /* 0x0A */ s16 obj_bank_id;
    /* 0x0C */ u32 instanceSize;
    /* 0x10 */ ActorFunc init;
    /* 0x14 */ ActorFunc destroy;
    /* 0x18 */ ActorFunc mova;
    /* 0x1C */ ActorFunc draw;
    /* 0x20 */ ActorFunc save;
} ActorInit; // size = 0x24

typedef enum AllocType {
    /* 0 */ ALLOCTYPE_NORMAL
} AllocType;

typedef struct PosRot {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3s rot;
} PosRot; // size = 0x14

typedef struct CC_Status_t {
  Vec3f collision_vec;
  s16 radius;
  s16 height;
  s16 offset;

  u8 weight;
  u8 hp;
  u8 damage;

  u8 damage_effect;
  u8 at_hit_effect;
  u8 ac_hit_effect;
} CC_Status_t;

typedef struct shape_info_t {
  /* 0x00 */ Vec3s rotation;
  /* 0x06 */ s16 unk_6;
  /* 0x08 */ f32 ofs_y;
  /* 0x0C */ void (*shadow_proc)();
  /* 0x10 */ f32 shadow_size_x;
  /* 0x14 */ f32 shadow_size_z;
  /* 0x18 */ f32 shadow_size_change_rate;
  /* 0x1C */ f32 shadow_alpha_change_rate;
  /* 0x20 */ int unk_20;
  /* 0x24 */ Vec3f* shadow_position;
  /* 0x28 */ int unk_28;
  /* 0x2C */ u8 draw_shadow; /* TRUE = shadow is drawn, FALSE = shadow is not drawn */
  /* 0x2D */ u8 unk_2D;
  /* 0x2E */ u8 force_shadow_position; /* TRUE = shadow's position is forcefully updated, FALSE = non-forcefully updated */
  /* 0x2F */ u8 unused_2F[0x48-0x2F];
} shape_info_t;

typedef struct Actor {
    /* 0x000 */ s16 profile;
    /* 0x002 */ u8 unk2;                            /* inferred */
    /* 0x003 */ u8 unk3;                            /* inferred */
    /* 0x004 */ s16 unk4;                           /* inferred */
    /* 0x006 */ u16 unk6;                           /* inferred */
    /* 0x008 */ s8 unk8;                            /* inferred */
    /* 0x009 */ s8 unk9;                            /* inferred */
    /* 0x00A */ s16 unkA;                           /* inferred */
    /* 0x00C */ PosRot home;
    /* 0x020 */ u32 flags;
    /* 0x024 */ s16 arg_data;
    /* 0x026 */ s16 bank; // object
    /* 0x028 */ PosRot world;
    /* 0x03C */ Vec3f prev_world;
    /* 0x048 */ PosRot eye;
    /* 0x05C */ Vec3f scale;
    /* 0x068 */ Vec3f velocity;
    /* 0x074 */ f32 speed;
    /* 0x078 */ f32 gravity; // ?
    /* 0x07C */ f32 yVelTarget; // ?
    /* 0x080 */ char unk_80[0x34];
    /* 0x0B4 */ s8 unk_B4;
    /* 0x0B5 */ u8 is_drawn;
    /* 0x0B6 */ s16 yaw_to_player;
    /* 0x0B8 */ f32 xyz_dist_sq_to_player;
    /* 0x0BC */ f32 xz_dist_to_player;
    /* 0x0C0 */ f32 height_below_player;
    /* 0x0C4 */ CC_Status_t cc_status;
    /* 0x0DC */ shape_info_t shape;
    /* 0x124 */ Vec3f prj_pos;
    /* 0x130 */ f32 prj_w;
    /* 0x134 */ f32 cull_width;
    /* 0x138 */ f32 cull_down;
    /* 0x13C */ f32 cull_forward;
    /* 0x140 */ f32 cull_scaleYZ;
    /* 0x144 */ f32 unk144; // checked against distance to player
    /* 0x148 */ u8 talking_cull; // camera culling related
    /* 0x149 */ u8 unk149; // umbrella related
    /* 0x14A */ char unk_14A[2];
    /* 0x14C */ struct Actor* parent;
    /* 0x150 */ struct Actor* child;
    /* 0x154 */ struct Actor* prev;
    /* 0x158 */ struct Actor* next;
    /* 0x15C */ ActorFunc ct;
    /* 0x160 */ ActorFunc dt;
    /* 0x164 */ ActorFunc unk164; // init/move/call?
    /* 0x168 */ ActorFunc draw;
    /* 0x16C */ ActorFunc save;
    /* 0x170 */ void* dlf_entry;
} Actor; // size = 0x174

// void func_80056380_jp();
// void projection_pos_set();
// void Actor_world_to_eye();
// void Actor_position_move();
// void Actor_position_speed_set();
// void Actor_position_moveF();
// void Actor_player_look_direction_check();
// void Actor_display_position_set();
// void Actor_data_bank_dma_end_check();
// void Shape_Info_init();
// void Actor_foot_shadow_pos_set();
// void Actor_delete();
// void Actor_ct();
// void Actor_dt();
// void Actor_draw();
// void Actor_draw_actor_no_culling_check();
// void Actor_draw_actor_no_culling_check2();
// void Actor_cull_check();
// void Actor_delete_check();
// void Actor_info_ct();
// void Actor_info_dt();
// void Actor_info_call_actor();
// void Actor_info_draw_actor();
// void Actor_info_part_new();
// void Actor_info_part_delete();
// void Actor_free_overlay_area();
// void actor_free_check();
// void Actor_get_overlay_area();
// void func_80057940_jp();
// void func_80057A8C_jp();
// void func_80057B70_jp();
// void Actor_data_bank_regist_check();
// void Actor_malloc_actor_class();
// void Actor_init_actor_class();
// void Actor_info_make_actor();
// void Actor_info_make_child_actor();
// void restore_fgdata();
// void restore_fgdata_one();
// void restore_fgdata_all();
// void Actor_info_save_actor();
// void Actor_info_delete();
// void Actor_info_name_search_sub();
// void Actor_info_name_search();
// void Actor_info_fgName_search_sub();
// void Actor_info_fgName_search();
// void Part_Break_init();
// void HiliteReflect_new();
// void HiliteReflect_init();
// void HiliteReflect_xlu_init();
// void HiliteReflect_light_init();
// void Setpos_HiliteReflect_init();
// void Setpos_HiliteReflect_xlu_init();
// void Setpos_HiliteReflect_light_init();

#endif
