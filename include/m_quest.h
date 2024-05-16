#ifndef M_QUEST_H
#define M_QUEST_H

#include "ultra64.h"
#include "lb_rtc.h"
#include "m_private_internals.h"
#include "m_npc_base.h"

enum QuestType {
    /* 0 */ QUEST_TYPE_DELIVERY, /* Deliver item quest */
    /* 1 */ QUEST_TYPE_ERRAND,   /* Villager 'can I help' quests */
    /* 2 */ QUEST_TYPE_CONTEST,  /* Villager send letter, plant flowers, bring ball, etc */
    /* 3 */ QUEST_TYPE_NONE,
    /* 4 */ QUEST_TYPE_NUM = QUEST_TYPE_NONE,
};

enum QuestDeliveryKind {
    /* 0 */ DELIVERY_KIND_NORMAL,  // standard delivery
    /* 1 */ DELIVERY_KIND_FOREIGN, // delivered to a foreign animal
    /* 2 */ DELIVERY_KIND_REMOVE,  // delivered to the animal who last left for another town
    /* 3 */ DELIVERY_KIND_LOST,    // assumed, probably for when a delivery is 'undeliverable'
    /* 4 */ DELIVERY_KIND_NUM,
};

enum QuestErrandEnum {
    /* 0 */ ERRAND_REQUEST,
    /* 1 */ ERRAND_REQUEST_CONTINUE,
    /* 2 */ ERRAND_REQUEST_FINAL,
    /* 3 */ ERRAND_FIRSTJOB_CHANGE_CLOTH,
    /* 4 */ ERRAND_FIRSTJOB_PLANT_FLOWER,
    /* 5 */ ERRAND_FIRSTJOB_DELIVER_FTR,
    /* 6 */ ERRAND_FIRSTJOB_SEND_LETTER,
    /* 7 */ ERRAND_FIRSTJOB_DELIVER_CARPET,
    /* 8 */ ERRAND_FIRSTJOB_DELIVER_AXE,
    /* 9 */ ERRAND_FIRSTJOB_POST_NOTICE,
    /* 10 */ ERRAND_FIRSTJOB_SEND_LETTER2,
    /* 11 */ ERRAND_FIRSTJOB_DELIVER_AXE2,
    /* 12 */ ERRAND_FIRSTJOB_INTRODUCTIONS,
    /* 13 */ ERRAND_FIRSTJOB_OPEN,
    /* 14 */ ERRAND_FIRSTJOB_START,
    /* 15 */ ERRAND_NUM,
};

enum QuestErrandType {
    /* 0 */ ERRAND_TYPE_NONE,
    /* 1 */ ERRAND_TYPE_CHAIN,
    /* 2 */ ERRAND_TYPE_FIRST_JOB,
    /* 3 */ ERRAND_TYPE_NUM,
};

enum QuestContestKind {
    /* 0 */ CONTEST_KIND_FRUIT,   /* get fruit for villager */
    /* 1 */ CONTEST_KIND_SOCCER,  /* get ball for villager */
    /* 2 */ CONTEST_KIND_SNOWMAN, /* build snowman for villager */
    /* 3 */ CONTEST_KIND_FLOWER,  /* plant flowers for villager */
    /* 4 */ CONTEST_KIND_FISH,    /* get fish for villager */
    /* 5 */ CONTEST_KIND_INSECT,  /* get insect for villager */
    /* 6 */ CONTEST_KIND_LETTER,  /* send letter to villager */
    /* 7 */ CONTEST_KIND_NUM,
};

typedef struct mQst_base_c {
    /* 0x00 */ u32 questType : 2;        /* type, 0 = delivery, 1 = errand, 2 = contest, 3 = none */
    /* 0x00 */ u32 questKind : 6;        /* kind, differs by type */
    /* 0x01 */ u32 timeLimitEnabled : 1; /* when set, the time limit will be utilized */
    /* 0x01 */ u32 progress : 4;         /* progress towards quest goal */
    /* 0x01 */ u32
        giveReward : 1; /* set to true when player cannot take the item, and will skip quest completion checks */
    /* 0x01 */ u32 unused : 2;
    /* 0x02 */ lbRTC_time_c timeLimit;
} mQst_base_c; // size = 0xC

/* 'Not Saved' Quest */
typedef struct mQst_not_saved_c {
    /* 0x00 */ s32 work;
    /* 0x04 */ u8 h;
} mQst_not_saved_c; // size = 0x5

typedef struct mQst_delivery_c {
    /* 0x00 */ mQst_base_c base;
    /* 0x0C */ AnmPersonalID_c recipient;
    /* 0x18 */ AnmPersonalID_c sender;
} mQst_delivery_c; // size = 0x24

typedef struct QuestFirstJob {
    /* 0x00 */ AnmPersonalID_c usedId[2]; /* villagers already used for first job quest (furniture, then letter) */
    /* 0x1C */ u8 usedNum : 7;            /* used count for 'used_ids' */
    /* 0x1C */ u8 wrongNum : 1;           /* set to TRUE if player changes out of work uniform during chores */
} QuestFirstJob;

typedef struct QuestErrandChain {
    /* 0x00 */ AnmPersonalID_c usedId[3];
    /* 0x24 */ u8 usedNum;
} QuestErrandChain; // size 0x26

typedef union ErrandInfo {
    /* 0x00 */ QuestErrandChain chain;
    /* 0x12 */ QuestFirstJob firstJob;
} ErrandInfo; // size = 0x24

typedef struct mQst_errand_c {
    /* 0x00 */ mQst_base_c base;            /* quest base info */
    /* 0x0C */ AnmPersonalID_c recipient; /* villager who will receive it */
    /* 0x1A */ AnmPersonalID_c sender;    /* villager who sent it */
    /* 0x28 */ u16 item;                  /* errand item */
    /* 0x2A */ s8 pocketIdx : 5;          /* index in player pockets where the errand item is */
    /* 0x2A */ s8 errandType : 3;         /* errand type */
    /* 0x2C */ ErrandInfo info;           /* errand type-specific data */
} mQst_errand_c;                            // size = 0x50

typedef union mQst_contest_info_u {
    struct {
        /* 0x00 */ u8 flowers_requested; /* number of flowers village requests be planted in acre */
    } flower_data;

    struct {
        /* 0x00 */ u8 score;              /* score rank of letter */
        /* 0x02 */ u16 present; /* present sent with letter */
    } letter_data;
} mQst_contest_info_u; // size = 0x4

typedef struct mQst_contest_c {
    /* 0x00 */ mQst_base_c base;         /* quest base struct */
    /* 0x0C */ u16 requested_item;     /* item (if any) requested by the villager */
    /* 0x0E */ PersonalID_c player_id; /* personal id of the player */
    /* 0x1E */ s8 type;                /* type of quest, seems to be repeat of data in quest base */
    /* 0x20 */ mQst_contest_info_u info;  /* contest info for flower & letter quests */
} mQst_contest_c;                        // size = 0x24

void mQst_ClearQuestInfo(mQst_base_c*);
void mQst_ClearDelivery(mQst_delivery_c* deliveries, s32 count);
void mQst_ClearErrand(mQst_errand_c* errands, s32 count);
void mQst_ClearContest(mQst_contest_c* contest);
void mQst_ClearNotSaveQuest(mQst_not_saved_c* quest);
// void func_800BA7E8_jp();
// void func_800BA8A0_jp();
// void func_800BA8F4_jp();
// void func_800BAA10_jp();
// void func_800BAA7C_jp();
s32 mQst_CheckLimitOver(mQst_base_c* base);
// void func_800BABE8_jp();
// void func_800BAC84_jp();
// void func_800BACEC_jp();
// void func_800BADBC_jp();
// void func_800BAE78_jp();
UNK_RET mQst_CheckLimitbyPossessionIdx(s32 arg0);
// void func_800BB0A8_jp();
// void func_800BB0E8_jp();
// void func_800BB10C_jp();
// void func_800BB270_jp();
// void func_800BB3A4_jp();
// void func_800BB4B0_jp();
s32 mQst_GetOccuredContestIdx(s32);
s32 mQst_GetFlowerSeedNum(s32 bx, s32 bz);
// void func_800BB658_jp();
// void func_800BB67C_jp();
// void func_800BB6A0_jp();
// void func_800BB6F0_jp();
// void func_800BB740_jp();
// void func_800BB86C_jp();
// void func_800BB990_jp();
// void func_800BBAB0_jp();
void mQst_SetReceiveLetter(mQst_contest_c*, PersonalID_c*, u8*, u16);
mQst_errand_c* mQst_GetFirstJobData(void);
// void func_800BBCBC_jp();
// void func_800BBD00_jp();
// void func_800BBD68_jp();
// void func_800BBD90_jp();
// void func_800BBDE0_jp();
// void func_800BBE78_jp();
// void func_800BBEE0_jp();
// void func_800BBF3C_jp();
// void func_800BBF98_jp();
// void func_800BC064_jp();
// void func_800BC10C_jp();
// void func_800BC12C_jp();
// void func_800BC14C_jp();
// void func_800BC1E4_jp();
// void func_800BC28C_jp();
// void func_800BC33C_jp();
// void func_800BC368_jp();
// void func_800BC394_jp();
s32 mQst_GetRandom(s32);
// void func_800BC454_jp();
// void func_800BC528_jp();
// void func_800BC5D4_jp();
// void func_800BC6B0_jp();
// void func_800BC724_jp();
// void func_800BC830_jp();
// void mQst_PrintQuestInfo();

#endif
