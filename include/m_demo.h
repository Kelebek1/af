#ifndef M_DEMO_H
#define M_DEMO_H

#include "ultra64.h"

#define mDemo_ORDER_VALUE_NUM 16
#define mDemo_ORDER_VALUE_MAX 10
#define mDemo_REQUEST_NUM 32

enum mDemoOrderType {
    /* 0  */ mDemo_ORDER_PLAYER,
    /* 1  */ mDemo_ORDER_1,
    /* 2  */ mDemo_ORDER_2,
    /* 3  */ mDemo_ORDER_3,
    /* 4  */ mDemo_ORDER_NPC0,
    /* 5  */ mDemo_ORDER_NPC1,
    /* 6  */ mDemo_ORDER_NPC2,
    /* 7  */ mDemo_ORDER_7,
    /* 8  */ mDemo_ORDER_8,
    /* 9  */ mDemo_ORDER_QUEST,
    /* 10 */ mDemo_ORDER_NUM,
};

enum eDemoType {
    /* 0  */ mDemo_TYPE_NONE,
    /* 1  */ mDemo_TYPE_SCROLL,
    /* 2  */ mDemo_TYPE_2,
    /* 3  */ mDemo_TYPE_DOOR,
    /* 4  */ mDemo_TYPE_4,
    /* 5  */ mDemo_TYPE_SCROLL2,
    /* 6  */ mDemo_TYPE_DOOR2,
    /* 7  */ mDemo_TYPE_TALK,
    /* 8  */ mDemo_TYPE_SPEAK,
    /* 9  */ mDemo_TYPE_REPORT,
    /* 10 */ mDemo_TYPE_SPEECH,
    /* 11 */ mDemo_TYPE_OUTDOOR,
    /* 12 */ mDemo_TYPE_12,
    /* 13 */ mDemo_TYPE_EVENTMSG,
    /* 14 */ mDemo_TYPE_EVENTMSG2,
    /* 15 */ mDemo_TYPE_15,
    /* 16 */ mDemo_TYPE_SCROLL3,
    /* 17 */ mDemo_TYPE_NUM,
};

#endif
