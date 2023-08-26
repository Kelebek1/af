#ifndef AC_NPC_H
#define AC_NPC_H

#include "ultra64.h"
#include "m_actor.h"
#include "m_collision_obj.h"
#include "unk.h"

struct Game_Play;
struct Npc;
struct Animal;
struct NpcList;

typedef void (*NpcActionFunc)(struct Npc*, struct Game_Play*);

typedef struct Npc {
    /* 0x000 */ Actor actor;
    /* 0x174 */ struct Animal* animal;
    /* 0x178 */ struct NpcList* npcList;
    /* 0x17C */ UNK_TYPE1 unk_17C[0x58C];
    /* 0x708 */ s32 unk_708;
    /* 0x70C */ UNK_TYPE1 unk_70C[0x1E4];
    /* 0x8F0 */ ClObjPipe collider;
    /* 0x90C */ UNK_TYPE1 unk_90C[0x30];
} Npc; // size = 0x93C

#endif
