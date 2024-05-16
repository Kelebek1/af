#ifndef AC_HANDOVERITEMACTOR_H
#define AC_HANDOVERITEMACTOR_H

#include "ultra64.h"
#include "m_actor.h"
#include "unk.h"

struct Game_Play;
struct HandOverItem;

typedef void (*HandOverItemActionFunc)(struct HandOverItem*, struct Game_Play*);

typedef struct HandOverItem {
    /* 0x000 */ Actor actor;
    /* 0x174 */ UNK_TYPE1 unk_174[0x84];
} HandOverItem; // size = 0x1F8

#endif
