#ifndef AC_QUEST_MANAGER_H
#define AC_QUEST_MANAGER_H

#include "ultra64.h"
#include "m_actor.h"
#include "overlays/actors/ovl_Npc/ac_npc.h"
#include "m_npc.h"
#include "m_quest.h"
#include "m_private_internals.h"
#include "m_submenu.h"
#include "m_choice_main.h"
#include "unk.h"

struct Game_Play;
struct QuestManager;
union aQMgr_quest_c;
struct aQMgr_regist_c;

typedef void (*QuestManagerActionFunc)(struct QuestManager*, struct Game_Play*);
typedef void (*aQMgr_CHECK_LIMIT_PROC)(struct QuestManager*, struct aQMgr_regist_c*);
typedef s32 (*aQMgr_CHECK_FINISH_PROC)(mQst_base_c*, Animal_c*);
typedef s32 (*aQMgr_TALK_COMMON_PROC)(struct QuestManager*, s32);
typedef void (*aQMgr_ACTOR_CLEAR_REGIST_PROC)(struct aQMgr_regist_c*, s32);
typedef void (*aQMgr_ACTOR_REGIST_QUEST_PROC)(struct QuestManager*, s32*, union aQMgr_quest_c*, s32);
typedef s32 (*aQMgr_GET_TIME_KIND_PROC)(s32);
typedef void (*aQMgr_TALK_INIT_PROC)(struct QuestManager*);

#define aQMgr_REGIST_NUM 35

enum QuestManagerTalkState {
    /* 0 */ aQMgr_TALK_STATE_INIT,
    /* 1 */ aQMgr_TALK_STATE_SUB,
    /* 2 */ aQMgr_TALK_STATE_NUM,
};

enum QuestManagerTalkSubState {
    /* 0 */  aQMgr_TALK_SUB_STATE_WAIT,
    /* 1 */  aQMgr_TALK_SUB_STATE_MSG_DISAPPEAR_WAIT,
    /* 2 */  aQMgr_TALK_SUB_STATE_MSG_APPEAR_WAIT,
    /* 3 */  aQMgr_TALK_SUB_STATE_CHECK_BUTTON,
    /* 4 */  aQMgr_TALK_SUB_STATE_HAND_ITEM_WAIT,
    /* 5 */  aQMgr_TALK_SUB_STATE_NPC_HAND_WAIT,
    /* 6 */  aQMgr_TALK_SUB_STATE_ITEM_WAIT,
    /* 7 */  aQMgr_TALK_SUB_STATE_DEMO_ORDER_WAIT,
    /* 8 */  aQMgr_TALK_SUB_STATE_NO_WAIT,
    /* 9 */  aQMgr_TALK_SUB_STATE_NPC_HAND_WAIT_MSG_WAIT,
    /* 10 */ aQMgr_TALK_SUB_STATE_ITEM_WAIT_END,
    /* 11 */ aQMgr_TALK_SUB_STATE_ITEM_PLAYER_WAIT,
    /* 12 */ aQMgr_TALK_SUB_STATE_NUM,
};

enum QuestManagerTalkCommon {
    /* 0 */ aQMgr_TALK_COMMON_SET_TARGET,
    /* 1 */ aQMgr_TALK_COMMON_GET_ITEM_IDX,
    /* 2 */ aQMgr_TALK_COMMON_SET_MSG_NO,
    /* 3 */ aQMgr_TALK_COMMON_CLEAR_TALK_INFO,
    /* 4 */ aQMgr_TALK_COMMON_SET_CHOICE_STR,
    /* 5 */ aQMgr_TALK_COMMON_GET_SET_DATA,
    /* 6 */ aQMgr_TALK_COMMON_SET_NPC_TAKEOUT_ITEM,
    /* 7 */ aQMgr_TALK_COMMON_SET_NPC_TAKEOUT_REWARD,
    /* 8 */ aQMgr_TALK_COMMON_TALK_INIT_OVL,
    /* 9 */ aQMgr_TALK_COMMON_CHANGE_TALK_NORMAL,
    /* 10 */ aQMgr_TALK_COMMON_CHANGE_TALK_ISLAND,
    /* 11 */ aQMgr_TALK_COMMON_NUM,
};

enum QuestManagerTalkKind {
    /* 0 */ aQMgr_TALK_KIND_QUEST,
    /* 1 */ aQMgr_TALK_KIND_FIRST_JOB,
    /* 2 */ aQMgr_TALK_KIND_NORMAL,
    /* 3 */ aQMgr_TALK_KIND_NUM,
};

enum QuestManagerMsgKind {
    /* 0 */  aQMgr_MSG_KIND_REQUEST_INIT,
    /* 1 */  aQMgr_MSG_KIND_REQUEST_END,
    /* 2 */  aQMgr_MSG_KIND_REQUEST_RECONF, // reconfirm?
    /* 3 */  aQMgr_MSG_KIND_REQUEST_REJECT, // rejected offer
    /* 4 */  aQMgr_MSG_KIND_COMPLETE_INIT,
    /* 5 */  aQMgr_MSG_KIND_COMPLETE_END,
    /* 6 */  aQMgr_MSG_KIND_FAILURE_INIT,
    /* 7 */  aQMgr_MSG_KIND_FAILURE_END,
    /* 8 */  aQMgr_MSG_KIND_FULL_ITEM, // pockets are full so can't give quests
    /* 9 */  aQMgr_MSG_KIND_AFTER_REWARD,
    /* 10 */ aQMgr_MSG_KIND_AFTER_REWARD_THANKS,
    /* 11 */ aQMgr_MSG_KIND_REWARD_FULL_ITEM,  // pockets are full so can't give reward
    /* 12 */ aQMgr_MSG_KIND_REWARD_FULL_ITEM2, // pockets are still full
    /* 13 */ aQMgr_MSG_KIND_NONE,
    /* 13 */ aQMgr_MSG_KIND_NUM = aQMgr_MSG_KIND_NONE,
};

enum QuestManagerTime {
    /* 0 */ aQMgr_TIME_MORNING,
    /* 1 */ aQMgr_TIME_DAY,
    /* 2 */ aQMgr_TIME_EVENING,
    /* 3 */ aQMgr_TIME_NIGHT,
    /* 4 */ aQMgr_TIME_NUM,
};

enum QuestManagerTarget {
    /* 0 */ aQMgr_QUEST_TARGET_RANDOM,
    /* 1 */ aQMgr_QUEST_TARGET_RANDOM_EXCLUDED,
    /* 2 */ aQMgr_QUEST_TARGET_ORIGINAL_TARGET, // same target as before?
    /* 3 */ aQMgr_QUEST_TARGET_FOREIGN,
    /* 4 */ aQMgr_QUEST_TARGET_LAST_REMOVE,
    /* 5 */ aQMgr_QUEST_TARGET_CLIENT,
    /* 6 */ aQMgr_QUEST_TARGET_NUM,
};

enum QuestManagerItem {
    /* 0 */ aQMgr_QUEST_ITEM_RANDOM,
    /* 1 */ aQMgr_QUEST_ITEM_FRUIT,
    /* 2 */ aQMgr_QUEST_ITEM_CLOTH,
    /* 3 */ aQMgr_QUEST_ITEM_FROM_DATA,
    /* 4 */ aQMgr_QUEST_ITEM_CURRENT_ITEM,
    /* 5 */ aQMgr_QUEST_ITEM_NONE,
    /* 6 */ aQMgr_QUEST_ITEM_NUM,
};

enum QuestManagerReward {
    /* 0 */ aQMgr_QUEST_REWARD_FTR,
    /* 1 */ aQMgr_QUEST_REWARD_STATIONERY,
    /* 2 */ aQMgr_QUEST_REWARD_CLOTH,
    /* 3 */ aQMgr_QUEST_REWARD_CARPET,
    /* 4 */ aQMgr_QUEST_REWARD_WALLPAPER,
    /* 5 */ aQMgr_QUEST_REWARD_MONEY,
    /* 6 */ aQMgr_QUEST_REWARD_WORN_CLOTH,
    /* 7 */ aQMgr_QUEST_REWARD_7,
    /* 8 */ aQMgr_QUEST_REWARD_NUM,
};

enum QuestManagerMode {
    /* 0 */ aQMgr_MODE_NORMAL,
    /* 1 */ aQMgr_MODE_SELECT_TALK,
    /* 2 */ aQMgr_MODE_MOVE_TALK,
    /* 3 */ aQMgr_MODE_TALK_FIN,
    /* 4 */ aQMgr_MODE_TALK_START_KAMAKURA_HELLO,
    /* 5 */ aQMgr_MODE_TALK_START_KAMAKURA,
    /* 6 */ aQMgr_MODE_TALK_START_SUMMERCAMP_HELLO,
    /* 7 */ aQMgr_MODE_TALK_START_SUMMERCAMP,
    /* 8 */ aQMgr_MODE_NUM,
};

typedef union aQMgr_quest_c {
    mQst_base_c base;
    mQst_errand_c errand;
    mQst_delivery_c delivery;
    mQst_contest_c contest;
} aQMgr_quest_c; // size = 0xC

typedef struct aQMgr_order_c {
    /* 0x00 */ u16 type;
    /* 0x02 */ u16 value;
} aQMgr_order_c; // size = 0x4

typedef struct aQMgr_set_data_c {
    /* 0x00 */ u32 to_type : 3;
    /* 0x00 */ u32 day_limit : 6;
    /* 0x00 */ u32 last_step : 4;
    /* 0x00 */ u32 handover_item : 1;
    /* 0x00 */ u32 src_item_type : 3;
    /* 0x04 */ u16 item;
    /* 0x08 */ u8 reward_percentages[aQMgr_QUEST_REWARD_NUM];
    /* 0x10 */ u32 max_pay;
    /* 0x14 */ s32 msg_start[aQMgr_MSG_KIND_NUM];
} aQMgr_set_data_c; // size = 0x48

typedef struct aQMgr_flower_data_c {
    /* 0x00 */ s32 goal_num;
    /* 0x04 */ s32 exist_num;
    /* 0x08 */ s32 remain_num;
} aQMgr_flower_data_c; // size = 0xC

typedef union aQMgr_work_data_c {
    aQMgr_flower_data_c flower;
} aQMgr_work_data_c; // size = 0xC

typedef struct aQMgr_target_c {
    /* 0x00 */ mQst_base_c quest_info;
    /* 0x0C */ AnmPersonalID_c* from_id;
    /* 0x10 */ AnmPersonalID_c* to_id;
    /* 0x14 */ s32 quest_inv_item_idx;
    /* 0x18 */ u16 quest_item;
    /* 0x1C */ s32 reward_kind;
    /* 0x20 */ u16 reward_item;
    /* 0x24 */ u32 pay;
    /* 0x28 */ lbRTC_time_c limit;
    /* 0x30 */ aQMgr_set_data_c* set_data_p;
    /* 0x34 */ s32 free_data_idx;
    /* 0x38 */ mQst_base_c* free_data_p;
    /* 0x3C */ s8 errand_type;
    /* 0x40 */ aQMgr_work_data_c work;
} aQMgr_target_c; // size = 0x4C

typedef struct aQMgr_regist_c {
    /* 0x00 */ aQMgr_CHECK_LIMIT_PROC check_limit_proc;
    /* 0x04 */ aQMgr_CHECK_FINISH_PROC check_finish_proc;
    /* 0x08 */ mQst_base_c* quest_info;
    /* 0x0C */ PersonalID_c* pid;
    /* 0x10 */ AnmPersonalID_c from_id;
    /* 0x1C */ AnmPersonalID_c to_id;
    /* 0x28 */ u16 item;
    /* 0x2C */ s32 animal_idx;
} aQMgr_regist_c; // size = 0x30

typedef struct aQMgr_normal_info_c {
    /* 0x00 */ Anmmem_c* memory;
    /* 0x04 */ Anmplmail_c* anmplmail;
    /* 0x08 */ u32 pay;
    /* 0x0C */ s32 player_num_items;          // valid items to trade
    /* 0x10 */ s32 player_give_item_idx;      // index to give item away
    /* 0x14 */ u16 player_items[4]; // items which the player has in their inventory which match 'player_num_items'
    /* 0x1C */ u16 selected_item;
} aQMgr_normal_info_c; // size = 0x20

typedef struct aQMgr_talk_wait_info_c {
    /* 0x00 */ s32 flags[3];
} aQMgr_talk_wait_info_c; // size = 0xC

typedef struct aQMgr_choice_c {
    /* 0x00 */ s32 choice_ids[mChoice_CHOICE_NUM];
    /* 0x10 */ s32 choice_num;
    /* 0x14 */ s32 talk_action;
} aQMgr_choice_c; // size = 0x18

typedef struct aQMgrProcs {
    /* 0x8B0 */ void* allocated_vram;
    /* 0x8B4 */ aQMgr_TALK_INIT_PROC talk_init_proc;
    /* 0x8B8 */ aQMgr_TALK_COMMON_PROC talk_common_proc;
    /* 0x8BC */ aQMgr_ACTOR_CLEAR_REGIST_PROC clear_regist_proc;
    /* 0x8C0 */ aQMgr_ACTOR_REGIST_QUEST_PROC regist_quest_proc;
    /* 0x8C4 */ aQMgr_GET_TIME_KIND_PROC get_time_kind_proc;
} aQMgrProcs;

typedef struct QuestManager {
    /* 0x000 */ Actor actor;
    /* 0x174 */ Submenu* submenu;
    /* 0x178 */ Actor** client;
    /* 0x17C */ Anmmem_c** memory;
    /* 0x180 */ u8* mode;
    /* 0x184 */ u8 talk_state;
    /* 0x185 */ u8 sub_talk_state;
    /* 0x186 */ u8 talk_step;
    /* 0x188 */ aQMgr_choice_c choice;
    /* 0x1A0 */ s32 msg_no;
    /* 0x1A4 */ s32 category_msg_no_start;
    /* 0x1A8 */ s32 msg_category;
    /* 0x1AC */ aQMgr_order_c demo_order;
    /* 0x1B0 */ aQMgr_talk_wait_info_c wait_info;
    /* 0x1BC */ u16 handover_item;
    /* 0x1C0 */ aQMgr_target_c target;
    /* 0x20C */ aQMgr_regist_c regist[aQMgr_REGIST_NUM];
    /* 0x89C */ s32 regist_idx;
    /* 0x8A0 */ s32 num_regist_in_use;
    /* 0x8A4 */ UNK_TYPE1 unk_8A4[0xC];
    /* 0x8B0 */ aQMgrProcs procs;
    /* 0x8C8 */ u16 cloth;
    /* 0x8CA */ u8 talk_type;
    /* 0x8CB */ u8 talk_change_type;
    /* 0x8CC */ u8 errand_next[mPr_ERRAND_QUEST_NUM];
    /* 0x8D1 */ UNK_TYPE1 unk_8D1[0x7];
} QuestManager; // size = 0x8D8



#endif
