#include "ac_quest_manager.h"
#include "m_lib.h"
#include "m_player_lib.h"
#include "m_actor_dlftbls.h"
#include "m_object.h"
#include "m_malloc.h"
#include "segment_symbols.h"
#include "m_common_data.h"
#include "overlays/gamestates/ovl_play/m_play.h"
#include "m_name_table.h"
#include "m_land.h"
#include "69E2C0.h"
#include "6C0690.h"
#include "8108C0.h"
#include "814FA0.h"
#include "815B70.h"
#include "m_scene_table.h"
#include "824500.h"
#include "m_msg_main.h"
#include "m_demo.h"
#include "ac_handOverItem.h"
#include "ovlmgr.h"

#define THIS ((QuestManager*)thisx)

#define aQMgr_OVL_SIZE 0x8800

void aQMgr_actor_ct(Actor* thisx, Game_Play* game_play);
void aQMgr_actor_dt(Actor* thisx, Game_Play* game_play);
void aQMgr_actor_move_main(Actor* thisx, Game_Play* game_play);
void aQMgr_actor_save(Actor* thisx, Game_Play* game_play);
void aQMgr_move_own_errand_cloth(QuestManager* this, aQMgr_regist_c* regist);
void aQMgr_move_own_errand_seed(QuestManager* this, aQMgr_regist_c* regist);
void aQMgr_move_own_errand_letter(QuestManager* this, aQMgr_regist_c* regist);
void aQMgr_move_own_errand_hello(QuestManager* this, aQMgr_regist_c* regist);
void aQMgr_contest_check_limit(QuestManager* this, aQMgr_regist_c* regist);
void aQMgr_actor_contest_snowman_clear(QuestManager* this, aQMgr_regist_c* regist);
void aQMgr_actor_contest_flower_clear(QuestManager* this, aQMgr_regist_c* regist);
void aQMgr_actor_contest_insect_clear(QuestManager* this, aQMgr_regist_c* regist);
s32 aQMgr_actor_check_fin_fruit(mQst_base_c* quest_info, Animal_c* animal);
s32 aQMgr_actor_check_fin_soccer(mQst_base_c* quest_info, Animal_c* animal);
s32 aQMgr_actor_check_fin_snowman(mQst_base_c* quest_info, Animal_c* animal);
s32 aQMgr_actor_check_fin_flower(mQst_base_c* quest_info, Animal_c* animal);
s32 aQMgr_actor_check_fin_fish(mQst_base_c* quest_info, Animal_c* animal);
s32 aQMgr_actor_check_fin_insect(mQst_base_c* quest_info, Animal_c* animal);
s32 aQMgr_actor_check_fin_contest_letter(mQst_base_c* quest_info, Animal_c* animal);

static Actor* l_client_p = NULL;

static u8 l_quest_manager_mode = aQMgr_MODE_NORMAL;

static Anmmem_c* l_quest_memory_p = NULL;

static QuestManager* l_quest_actor_p = NULL;

static s32 l_quest_manager_hello = FALSE;

typedef struct aQMgr_overlays_c {
    /* 0x00 */ RomOffset rom_start;
    /* 0x04 */ RomOffset rom_end;
    /* 0x08 */ void* vram_start;
    /* 0x0C */ void* vram_end;
    /* 0x10 */ void* vram_allocated;
} aQMgr_overlays_c; // size = 0x14

static aQMgr_overlays_c D_809571B4_jp[] = {
    {
        SEGMENT_ROM_START(ovl__008108C0),
        SEGMENT_ROM_END(ovl__008108C0),
        SEGMENT_VRAM_START(ovl__008108C0),
        SEGMENT_VRAM_END(ovl__008108C0),
        func_8091C42C_jp,
    },
    {
        SEGMENT_ROM_START(ovl__00814FA0),
        SEGMENT_ROM_END(ovl__00814FA0),
        SEGMENT_VRAM_START(ovl__00814FA0),
        SEGMENT_VRAM_END(ovl__00814FA0),
        func_8091D594_jp,
    },
    {
        SEGMENT_ROM_START(ovl__00815B70),
        SEGMENT_ROM_END(ovl__00815B70),
        SEGMENT_VRAM_START(ovl__00815B70),
        SEGMENT_VRAM_END(ovl__00815B70),
        func_809218E8_jp,
    },
};

static aQMgr_set_data_c l_set_delivery_data[] = {
    { aQMgr_QUEST_TARGET_RANDOM,
      2,
      0,
      TRUE,
      aQMgr_QUEST_ITEM_CLOTH,
      EMPTY_NO,
      { 40, 0, 0, 0, 0, 30, 30, 0 },
      200,
      { 0x0151, 0x024C, 0x0163, 0x025E, 0x0175, 0x0294, 0x0187, 0x02B8, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_FOREIGN,
      2,
      0,
      TRUE,
      aQMgr_QUEST_ITEM_RANDOM,
      EMPTY_NO,
      { 40, 0, 0, 10, 10, 40, 0, 0 },
      1000,
      { 0x0199, 0x024C, 0x01AB, 0x025E, 0x01BD, 0x0294, 0x01CF, 0x02CA, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_LAST_REMOVE,
      2,
      0,
      TRUE,
      aQMgr_QUEST_ITEM_RANDOM,
      EMPTY_NO,
      { 20, 0, 0, 20, 20, 40, 0, 0 },
      1000,
      { 0x0205, 0x024C, 0x0217, 0x025E, 0x10BF, 0x0294, 0x023A, 0x02CA, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_RANDOM,
      2,
      0,
      TRUE,
      aQMgr_QUEST_ITEM_RANDOM,
      EMPTY_NO,
      { 40, 0, 40, 10, 10, 0, 0, 0 },
      0,
      { 0x0A74, 0x024C, 0x0A86, 0x025E, 0x0A98, 0x0294, 0x0AAA, 0x02B8, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
};

static aQMgr_set_data_c l_set_errand_data[] = {
    { aQMgr_QUEST_TARGET_RANDOM_EXCLUDED,
      2,
      4,
      FALSE,
      aQMgr_QUEST_ITEM_RANDOM,
      EMPTY_NO,
      { 50, 0, 0, 0, 0, 0, 0, 0 },
      500,
      { 0x038C, 0x024C, 0x03D4, 0x025E, 0x03F8, 0x0294, 0x2B73, 0x02CA, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_RANDOM_EXCLUDED,
      2,
      1,
      FALSE,
      aQMgr_QUEST_ITEM_CURRENT_ITEM,
      EMPTY_NO,
      { 50, 0, 0, 0, 0, 0, 0, 0 },
      500,
      { 0x03B0, 0x024C, 0x03E6, 0x025E, 0x03F8, 0x0294, 0x041C, 0x02CA, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_ORIGINAL_TARGET,
      2,
      0,
      TRUE,
      aQMgr_QUEST_ITEM_CURRENT_ITEM,
      EMPTY_NO,
      { 50, 0, 0, 0, 0, 0, 0, 0 },
      500,
      { 0x039E, 0x024C, 0x03C2, 0x025E, 0x03F8, 0x0294, 0x040A, 0x0452, 0x17B8, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_CLIENT,
      0,
      0,
      FALSE,
      aQMgr_QUEST_ITEM_CURRENT_ITEM,
      EMPTY_NO,
      { 0, 0, 0, 0, 0, 0, 0, 100 },
      0,
      { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_CLIENT,
      0,
      0,
      FALSE,
      aQMgr_QUEST_ITEM_CURRENT_ITEM,
      EMPTY_NO,
      { 0, 0, 0, 0, 0, 0, 0, 100 },
      0,
      { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_CLIENT,
      0,
      0,
      FALSE,
      aQMgr_QUEST_ITEM_CURRENT_ITEM,
      EMPTY_NO,
      { 0, 0, 0, 0, 0, 0, 0, 100 },
      0,
      { 0x0000, 0x0000, 0x0000, 0x0000, 0x08EF, 0x08F0, 0x0000, 0x0000, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_CLIENT,
      0,
      0,
      FALSE,
      aQMgr_QUEST_ITEM_CURRENT_ITEM,
      EMPTY_NO,
      { 0, 0, 0, 0, 0, 0, 0, 100 },
      0,
      { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_CLIENT,
      0,
      0,
      FALSE,
      aQMgr_QUEST_ITEM_CURRENT_ITEM,
      EMPTY_NO,
      { 0, 0, 0, 0, 0, 0, 0, 100 },
      0,
      { 0x0000, 0x0000, 0x0000, 0x0000, 0x08FB, 0x08FC, 0x0000, 0x0000, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_CLIENT,
      0,
      0,
      FALSE,
      aQMgr_QUEST_ITEM_CURRENT_ITEM,
      EMPTY_NO,
      { 0, 0, 0, 0, 0, 0, 0, 100 },
      0,
      { 0x0000, 0x0000, 0x0000, 0x0000, 0x0907, 0x0908, 0x0000, 0x0000, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_CLIENT,
      0,
      0,
      FALSE,
      aQMgr_QUEST_ITEM_CURRENT_ITEM,
      EMPTY_NO,
      { 0, 0, 0, 0, 0, 0, 0, 100 },
      0,
      { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_CLIENT,
      0,
      0,
      FALSE,
      aQMgr_QUEST_ITEM_CURRENT_ITEM,
      EMPTY_NO,
      { 0, 0, 0, 0, 0, 0, 0, 100 },
      0,
      { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_CLIENT,
      0,
      0,
      FALSE,
      aQMgr_QUEST_ITEM_CURRENT_ITEM,
      EMPTY_NO,
      { 0, 0, 0, 0, 0, 0, 0, 100 },
      0,
      { 0x0000, 0x0000, 0x0000, 0x0000, 0x0907, 0x0908, 0x0000, 0x0000, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_CLIENT,
      0,
      0,
      FALSE,
      aQMgr_QUEST_ITEM_CURRENT_ITEM,
      EMPTY_NO,
      { 0, 0, 0, 0, 0, 0, 0, 100 },
      0,
      { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_CLIENT,
      0,
      0,
      FALSE,
      aQMgr_QUEST_ITEM_CURRENT_ITEM,
      EMPTY_NO,
      { 0, 0, 0, 0, 0, 0, 0, 100 },
      0,
      { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_CLIENT,
      0,
      0,
      FALSE,
      aQMgr_QUEST_ITEM_CURRENT_ITEM,
      EMPTY_NO,
      { 0, 0, 0, 0, 0, 0, 0, 100 },
      0,
      { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
};

static aQMgr_set_data_c l_set_contest_data[] = {
    { aQMgr_QUEST_TARGET_CLIENT,
      1,
      1,
      FALSE,
      aQMgr_QUEST_ITEM_FRUIT,
      EMPTY_NO,
      { 0, 0, 0, 30, 30, 40, 0, 0 },
      500,
      { 0x01E1, 0x0000, 0x01E1, 0x025E, 0x01F3, 0x117C, 0x0000, 0x02CA, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_CLIENT,
      1,
      2,
      FALSE,
      aQMgr_QUEST_ITEM_NONE,
      EMPTY_NO,
      { 40, 0, 0, 30, 30, 0, 0, 0 },
      0,
      { 0x0D79, 0x0000, 0x0D79, 0x025E, 0x0D91, 0x0DD9, 0x0000, 0x02CA, 0x0440, 0x0DFD, 0x0DD9, 0x0DEB, 0x0E0F } },
    { aQMgr_QUEST_TARGET_CLIENT,
      1,
      1,
      FALSE,
      aQMgr_QUEST_ITEM_NONE,
      EMPTY_NO,
      { 60, 0, 0, 20, 20, 0, 0, 0 },
      0,
      { 0x0E33, 0x0000, 0x0E33, 0x025E, 0x0E45, 0x0E8D, 0x0000, 0x02CA, 0x0440, 0x0EB1, 0x0E8D, 0x0E9F, 0x0EC3 } },
    { aQMgr_QUEST_TARGET_CLIENT,
      3,
      1,
      FALSE,
      aQMgr_QUEST_ITEM_NONE,
      EMPTY_NO,
      { 60, 0, 0, 20, 20, 0, 0, 0 },
      0,
      { 0x0FB5, 0x0000, 0x0FB5, 0x025E, 0x0FC7, 0x100F, 0x0000, 0x02CA, 0x0440, 0x1033, 0x100F, 0x1021, 0x1045 } },
    { aQMgr_QUEST_TARGET_CLIENT,
      3,
      1,
      FALSE,
      aQMgr_QUEST_ITEM_NONE,
      EMPTY_NO,
      { 80, 0, 0, 10, 10, 0, 0, 0 },
      0,
      { 0x158C, 0x0000, 0x158C, 0x025E, 0x159E, 0x15B0, 0x0000, 0x02CA, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_CLIENT,
      3,
      1,
      FALSE,
      aQMgr_QUEST_ITEM_NONE,
      EMPTY_NO,
      { 80, 0, 0, 10, 10, 0, 0, 0 },
      0,
      { 0x160A, 0x0000, 0x160A, 0x025E, 0x161C, 0x162E, 0x0000, 0x02CA, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
    { aQMgr_QUEST_TARGET_CLIENT,
      2,
      2,
      FALSE,
      aQMgr_QUEST_ITEM_NONE,
      EMPTY_NO,
      { 80, 0, 0, 10, 10, 0, 0, 0 },
      0,
      { 0x1AE1, 0x0000, 0x1AE1, 0x025E, 0x1B17, 0x0294, 0x0000, 0x02CA, 0x0440, 0x035E, 0x034C, 0x0370, 0x033A } },
};

static aQMgr_set_data_c* l_set_data[QUEST_TYPE_NUM] = {
    l_set_delivery_data,
    l_set_errand_data,
    l_set_contest_data,
};

static aQMgr_CHECK_LIMIT_PROC l_errand_proc[] = {
    (void*)none_proc1,
    (void*)none_proc1,
    (void*)none_proc1,
    aQMgr_move_own_errand_cloth,
    aQMgr_move_own_errand_seed,
    (void*)none_proc1,
    aQMgr_move_own_errand_letter,
    (void*)none_proc1,
    (void*)none_proc1,
    (void*)none_proc1,
    aQMgr_move_own_errand_letter,
    (void*)none_proc1,
    aQMgr_move_own_errand_hello,
    (void*)none_proc1,
    (void*)none_proc1,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

static aQMgr_CHECK_LIMIT_PROC l_contest_proc[] = {
    aQMgr_contest_check_limit,        aQMgr_contest_check_limit, aQMgr_actor_contest_snowman_clear,
    aQMgr_actor_contest_flower_clear, aQMgr_contest_check_limit, aQMgr_actor_contest_insect_clear,
    aQMgr_contest_check_limit,
};

static aQMgr_CHECK_FINISH_PROC l_contest_check[] = {
    aQMgr_actor_check_fin_fruit,          aQMgr_actor_check_fin_soccer, aQMgr_actor_check_fin_snowman,
    aQMgr_actor_check_fin_flower,         aQMgr_actor_check_fin_fish,   aQMgr_actor_check_fin_insect,
    aQMgr_actor_check_fin_contest_letter,
};

static s32 l_proc_max[] = {
    DELIVERY_KIND_NUM,
    ERRAND_NUM,
    CONTEST_KIND_NUM,
};

void aQMgr_actor_set_memory(Actor* actor) {
    Animal_c* animal;

    if ((actor != NULL) && (actor->part == ACTOR_PART_NPC)) {
        animal = ((Npc*)actor)->npcInfo.animal;
    } else {
        animal = NULL;
    }

    if (animal != NULL) {
        PrivateInfo* private_info = common_data.privateInfo;
        Anmmem_c* memory = animal->memories;
        s32 animal_index = mNpc_GetAnimalMemoryIdx(&private_info->playerId, memory, ANIMAL_MEMORY_NUM);

        if (animal_index != -1) {
            memory += animal_index;
            l_quest_memory_p = memory;
        } else {
            l_quest_memory_p = NULL;
        }
    }
}

void aQMgr_actor_set_hello_free_str(Animal_c* animal) {
    s32 index = mNpc_GetAnimalMemoryIdx(&common_data.privateInfo->playerId, animal->memories, ANIMAL_MEMORY_NUM);

    if (index != -1) {
        mLd_SetFreeStrLandMuraName(animal->memories[index].land.name, mMsg_FREE_STR6);
    }

    mLd_SetFreeStrLandMuraName(animal->previousLandName, mMsg_FREE_STR10);
    mLd_SetFreeStrLandMuraName(animal->id.landName, mMsg_FREE_STR11);
}

s32 aQMgr_take_hello_msg_no(QuestManager* quest_manager) {
    Npc* npc = (Npc*)*quest_manager->client;
    UNUSED s32 pad;
    s32 (*sp24)(Actor*, aQMgr_GET_TIME_KIND_PROC);

    ovlmgr_Load(SEGMENT_ROM_START(ovl__00824500), SEGMENT_ROM_END(ovl__00824500), SEGMENT_VRAM_START(ovl__00824500),
                SEGMENT_VRAM_END(ovl__00824500), quest_manager->procs.allocated_vram);
    sp24 = (s32(*)(Actor*, aQMgr_GET_TIME_KIND_PROC))SEGMENT_VRAM_RESOLVE_ADDR(
        ovl__00824500, quest_manager->procs.allocated_vram, aQMgr_get_hello_msg_no);

    {
        Animal_c* animal = npc->npcInfo.animal;

        if (mNpc_GetAnimalMemoryIdx(&common_data.privateInfo->playerId, animal->memories, ANIMAL_MEMORY_NUM) == -1) {
            l_quest_manager_hello = TRUE;
        } else {
            l_quest_manager_hello = FALSE;
        }
    }

    return sp24(&npc->actor, quest_manager->procs.get_time_kind_proc);
}

void aQMgr_set_talk_info(Actor* client) {
    QuestManager* manager = l_quest_actor_p;
    Npc* npc = (Npc*)client;
    Animal_c* animal = npc->npcInfo.animal;

    l_client_p = client;
    mDemo_Set_msg_num(aQMgr_take_hello_msg_no(manager));

    if (animal->mood == NPC_FEEL_SLEEPY) {
        animal->mood = NPC_FEEL_NORMAL;
        npc->condition.feel_tim = 0;
    }

    npc->talk_info.feel = animal->mood;
    aQMgr_actor_set_hello_free_str(animal);

    if (animal != NULL) {
        manager->cloth = animal->cloth;
    }

    mNpc_SetTalkAnimalIdx_fdebug(&animal->id);
}

s32 aQMgr_actor_talk_request(Actor* client) {
    s32 ret = FALSE;

    if ((client != NULL) && (l_quest_manager_mode == aQMgr_MODE_NORMAL)) {
        if (mDemo_Check(mDemo_TYPE_TALK, client) == FALSE) {
            mDemo_Request(mDemo_TYPE_TALK, client, aQMgr_set_talk_info);
        } else if (mDemo_Check_ListenAble() == FALSE) {
            ret = TRUE;
        }
    }

    return ret;
}

s32 aQMgr_actor_talk_start(Actor* client) {
    s32 ret = FALSE;

    if ((client != NULL) && (l_quest_manager_mode == aQMgr_MODE_NORMAL) &&
        (mDemo_Check(mDemo_TYPE_TALK, client) == TRUE) && (mDemo_Check_ListenAble() == FALSE) &&
        (mDemo_Check_SpeakerAble() == TRUE) && (client == l_client_p)) {
        mDemo_Set_ListenAble();
        if (client->fgName == SP_NPC_EV_KAMAKURA_0) {
            if (l_quest_manager_hello == TRUE) {
                l_quest_manager_mode = aQMgr_MODE_TALK_START_KAMAKURA_HELLO;
            } else {
                l_quest_manager_mode = aQMgr_MODE_TALK_START_KAMAKURA;
            }
        } else {
            l_quest_manager_mode = aQMgr_MODE_SELECT_TALK;
        }
        aQMgr_actor_set_memory(client);
        mMld_ActorMakeMelody(client);
        ret = TRUE;
    }

    return ret;
}

s32 aQMgr_actor_talk_check(Actor* client) {
    s32 ret = FALSE;

    if ((client != NULL) && (client == l_client_p) && (l_quest_manager_mode == aQMgr_MODE_TALK_FIN)) {
        if (mDemo_Get_talk_actor() != client) {
            l_client_p = NULL;
            l_quest_manager_mode = aQMgr_MODE_NORMAL;
            ret = TRUE;
        }
    }

    return ret;
}

s32 aQMgr_get_time_kind(s32 hour) {
    s32 ret = aQMgr_TIME_MORNING;

    if ((hour >= 10) && (hour <= 16)) {
        ret = aQMgr_TIME_DAY;
    } else if ((hour > 16) && (hour < 24)) {
        ret = aQMgr_TIME_EVENING;
    } else if ((hour >= 0) && (hour < 5)) {
        ret = aQMgr_TIME_NIGHT;
    }

    return ret;
}

void aQMgr_clear_talk_init_ovl(QuestManager* this) {
    aQMgrProcs* procs = &this->procs;

    if (procs->allocated_vram != NULL) {
        bzero(procs->allocated_vram, aQMgr_OVL_SIZE);
    }

    this->procs.talk_init_proc = NULL;
}

s32 aQMgr_talk_common_talk_init_ovl(QuestManager* this) {
    UNUSED s32 pad;
    s32 talk_type = this->talk_type;
    aQMgr_overlays_c* overlays = &D_809571B4_jp[talk_type];
    aQMgrProcs* procs = &this->procs;

    ovlmgr_Load(overlays->rom_start, overlays->rom_end, overlays->vram_start, overlays->vram_end,
                procs->allocated_vram);
    procs->talk_init_proc = (aQMgr_TALK_INIT_PROC)SEGMENT_VRAM_RESOLVE_ADDR2(
        overlays->vram_start, procs->allocated_vram, overlays->vram_allocated);
    return TRUE;
}

void aQMgr_actor_change_talk_init_ovl(QuestManager* this) {
    if (this->talk_change_type < aQMgr_TALK_KIND_NUM) {
        this->talk_type = this->talk_change_type;
        if (this->procs.talk_common_proc(this, aQMgr_TALK_COMMON_TALK_INIT_OVL) == TRUE) {
            this->talk_change_type = aQMgr_TALK_KIND_NUM;
            this->sub_talk_state = aQMgr_TALK_SUB_STATE_NO_WAIT;
        }
    }
}

void aQMgr_actor_clear_regist(aQMgr_regist_c* regist, s32 count) {
    for (; count != 0; count--) {
        regist->check_limit_proc = (void*)none_proc1;
        regist->check_finish_proc = NULL;
        regist->quest_info = NULL;
        regist->pid = NULL;

        mNpc_ClearAnimalPersonalID(&regist->from_id);
        mNpc_ClearAnimalPersonalID(&regist->to_id);

        regist->animal_idx = -1;
        regist++;
    }
}

s32 aQMgr_actor_check_fin_item(mQst_base_c* quest_info, UNUSED Animal_c* animal) {
    mQst_contest_c* contest = (mQst_contest_c*)quest_info;
    u16 requested_item = contest->requested_item;
    PrivateInfo* private_info = common_data.privateInfo;
    s32 ret = FALSE;

    if (mPr_GetPossessionItemIdx(private_info, requested_item) != -1) {
        ret = TRUE;
    }

    return ret;
}

s32 aQMgr_actor_free_regist(QuestManager* this, s32 start_index) {
    aQMgr_regist_c* regist = &this->regist[start_index];
    s32 i;
    s32 index = -1;

    if (this->num_regist_in_use < aQMgr_REGIST_NUM) {
        for (i = start_index; i < aQMgr_REGIST_NUM; i++, regist++) {
            if (regist->quest_info == NULL) {
                index = i;
                break;
            }
        }
    }

    return index;
}

void aQMgr_actor_regist_quest(QuestManager* this, s32* index, aQMgr_quest_c* quest, s32 animal_index) {
    UNUSED s32 pad;
    u32 type = quest->base.questType;

    if (type < QUEST_TYPE_NUM) {
        *index = aQMgr_actor_free_regist(this, *index);
        if (*index != -1) {
            s32 kind = quest->base.questKind;
            s32 max_kind = l_proc_max[type];

            if ((kind >= 0) && (kind < max_kind)) {
                aQMgr_regist_c* regist = &this->regist[*index];
                AnmPersonalID_c* from_id;
                AnmPersonalID_c* to_id;

                if (type == QUEST_TYPE_CONTEST) {
                    Animal_c* animal = &common_data.animals[animal_index];

                    regist->pid = &quest->contest.player_id;
                    regist->check_limit_proc = l_contest_proc[kind];
                    regist->check_finish_proc = l_contest_check[kind];
                    from_id = &animal->id;
                    to_id = &animal->id;
                } else {
                    if (type == QUEST_TYPE_ERRAND) {
                        regist->check_limit_proc = l_errand_proc[kind];
                    }
                    from_id = &quest->errand.sender;
                    to_id = &quest->errand.recipient;
                    regist->pid = &common_data.privateInfo->playerId;
                }

                mNpc_CopyAnimalPersonalID(&regist->from_id, from_id);
                mNpc_CopyAnimalPersonalID(&regist->to_id, to_id);
                regist->quest_info = &quest->base;

                switch (type) {
                    case QUEST_TYPE_DELIVERY:
                        regist->item = common_data.privateInfo->inventory.pockets[animal_index];
                        break;

                    case QUEST_TYPE_ERRAND:
                        regist->item = quest->errand.item;
                        break;

                    case QUEST_TYPE_CONTEST:
                        regist->item = quest->errand.recipient.npcId;
                        break;
                }

                regist->animal_idx = animal_index;
                this->num_regist_in_use++;
            }
        }
    }
}

void aQMgr_talk_clear_talk_order(aQMgr_order_c* demo_order) {
    demo_order->type = mDemo_TYPE_NONE;
    demo_order->value = 0;
}

s32 aQMgr_talk_common_clear_talk_info(QuestManager* this) {
    aQMgr_target_c* target = &this->target;
    aQMgr_choice_c* choice = &this->choice;
    s32 i;

    *this->memory = NULL;
    this->talk_step = 0;
    this->sub_talk_state = aQMgr_TALK_SUB_STATE_WAIT;
    this->handover_item = EMPTY_NO;
    this->msg_no = 0;
    this->category_msg_no_start = 0;
    this->msg_category = aQMgr_MSG_KIND_NONE;

    mQst_ClearQuestInfo(&target->quest_info);
    target->from_id = NULL;
    target->to_id = NULL;
    target->quest_inv_item_idx = -1;
    target->quest_item = EMPTY_NO;
    target->reward_kind = -1;
    target->reward_item = EMPTY_NO;
    target->pay = 0;
    target->set_data_p = NULL;
    target->free_data_idx = -1;
    target->free_data_p = NULL;
    target->errand_type = ERRAND_TYPE_NONE;

    bzero(&target->work, sizeof(aQMgr_work_data_c));

    choice->choice_num = 0;
    choice->talk_action = -1;

    for (i = 0; i < mChoice_CHOICE_NUM; i++) {
        choice->choice_ids[i] = -1;
    }

    this->regist_idx = -1;
    this->talk_type = aQMgr_TALK_KIND_QUEST;
    this->talk_change_type = aQMgr_TALK_KIND_NUM;

    aQMgr_talk_clear_talk_order(&this->demo_order);
    return TRUE;
}

s32 aQMgr_talk_common_get_item_idx(QuestManager* this) {
    s32 regist_index = this->regist_idx;
    s32 index = -1;
    aQMgr_regist_c* regist = &this->regist[regist_index];
    mQst_base_c* quest_info = regist->quest_info;

    if (quest_info != NULL) {
        s32 quest_type = quest_info->questType;
        s32 quest_kind = quest_info->questKind;

        if (quest_type == QUEST_TYPE_DELIVERY) {
            index = regist->animal_idx;
        } else if (quest_type == QUEST_TYPE_ERRAND) {
            index = ((mQst_errand_c*)quest_info)->pocketIdx;
        } else if (quest_type == QUEST_TYPE_CONTEST) {
            u16 item = ((mQst_contest_c*)quest_info)->requested_item;
            PrivateInfo* private_info = common_data.privateInfo;

            switch (quest_kind) {
                case CONTEST_KIND_FISH:
                    index = mPr_GetPossessionItemIdxItem1Category(private_info, ITEM1_CAT_FISH);
                    break;

                case CONTEST_KIND_INSECT:
                    index = mPr_GetPossessionItemIdxItem1Category(private_info, ITEM1_CAT_INSECT);
                    break;

                default:
                    index = mPr_GetPossessionItemIdx(private_info, item);
                    break;
            }
        }
    }

    return index;
}

s32 aQMgr_talk_common_set_choice_str(QuestManager* this) {
    aQMgr_choice_c* choice_p = &this->choice;
    u8 str[mChoice_CHOICE_NUM][mChoice_CHOICE_STRING_LEN];
    u8* str_p_tbl[mChoice_CHOICE_NUM];
    s32 i;

    for (i = 0; i < mChoice_CHOICE_NUM; i++) {
        str_p_tbl[i] = NULL;
    }

    for (i = 0; i < choice_p->choice_num; i++) {
        if (i >= mChoice_CHOICE_NUM) {
            break;
        }

        mChoice_Load_ChoseStringFromRom(mChoice_Get_base_window_p(), str[i], choice_p->choice_ids[i], NULL);
        str_p_tbl[i] = str[i];
    }

    mChoice_Set_choice_data(mChoice_Get_base_window_p(), str_p_tbl[0], mChoice_CHOICE_STRING_LEN, str_p_tbl[1],
                            mChoice_CHOICE_STRING_LEN, str_p_tbl[2], mChoice_CHOICE_STRING_LEN, str_p_tbl[3],
                            mChoice_CHOICE_STRING_LEN);
    return TRUE;
}

s32 aQMgr_talk_common_get_set_data_p(QuestManager* this) {
    aQMgr_target_c* target = &this->target;
    mQst_base_c* quest_info = &target->quest_info;
    aQMgr_set_data_c* set_data = NULL;

    if (target != NULL) {
        s32 quest_type = quest_info->questType;
        s32 quest_kind = quest_info->questKind;

        aQMgr_set_data_c* temp = l_set_data[quest_type];
        set_data = &temp[quest_kind];
    }

    this->target.set_data_p = set_data;
    return TRUE;
}

void aQMgr_actor_init_quest(QuestManager* this) {
    Animal_c* animal = common_data.animals;
    PrivateInfo* private_info = common_data.privateInfo;
    s32 i;
    s32 free_index = 0;
    mQst_delivery_c* delivery = private_info->deliveries;
    mQst_errand_c* errand = private_info->errands;

    for (i = 0; i < mPr_DELIVERY_QUEST_NUM; i++, delivery++) {
        if (free_index == -1) {
            break;
        }

        aQMgr_actor_regist_quest(this, &free_index, (aQMgr_quest_c*)delivery, i);
    }

    for (i = 0; i < mPr_ERRAND_QUEST_NUM; i++, errand++) {
        if (free_index == -1) {
            break;
        }

        aQMgr_actor_regist_quest(this, &free_index, (aQMgr_quest_c*)errand, i);
    }

    for (i = 0; i < ANIMAL_NUM_MAX; i++, animal++) {
        if (free_index == -1) {
            break;
        }

        aQMgr_actor_regist_quest(this, &free_index, (aQMgr_quest_c*)&animal->contestQuest, i);
    }
}

void aQMgr_actor_regist_quest_move(QuestManager* this) {
    aQMgr_regist_c* regist = this->regist;
    s32 i;

    for (i = aQMgr_REGIST_NUM; i > 0; i--, regist++) {
        if (regist->quest_info != NULL) {
            if (regist->check_limit_proc != NULL) {
                regist->check_limit_proc(this, regist);
            }
        }
    }
}

void aQMgr_actor_move_wait(QuestManager* this) {
    aQMgr_actor_init_quest(this);
    aQMgr_actor_regist_quest_move(this);
    this->procs.clear_regist_proc(this->regist, aQMgr_REGIST_NUM);
    this->regist_idx = -1;
    this->num_regist_in_use = 0;
    mNpc_TalkInfoMove();
}

void aQMgr_actor_move_talk_fin(QuestManager* this) {
    aQMgr_actor_init_quest(this);
    aQMgr_actor_regist_quest_move(this);
    this->procs.clear_regist_proc(this->regist, aQMgr_REGIST_NUM);
    this->regist_idx = -1;
    this->num_regist_in_use = 0;
}

s32 aQMgr_actor_get_my_msg(s32 base_msg, s32 looks) {
    s32 sp18 = base_msg + looks * 3;

    sp18 = sp18 + mQst_GetRandom(3);
    return sp18;
}

s32 aQMgr_actor_decide_quest_message_no(aQMgr_target_c* target, s32 msg_type) {
    aQMgr_set_data_c* set_data = target->set_data_p;

    return set_data->msg_start[msg_type];
}

s32 aQMgr_talk_common_set_msg_no(QuestManager* this) {
    s32 base_msg;
    s32 msg_no;

    if (this->msg_category != aQMgr_MSG_KIND_NONE) {
        this->category_msg_no_start = aQMgr_actor_decide_quest_message_no(&this->target, this->msg_category);
    }

    base_msg = this->category_msg_no_start;
    if ((base_msg != 15) && (base_msg != 0)) {
        s32 looks = mNpc_GetNpcLooks(*this->client);

        msg_no = aQMgr_actor_get_my_msg(base_msg, looks);
    } else {
        msg_no = base_msg;
    }

    this->msg_no = msg_no;
    this->msg_category = aQMgr_MSG_KIND_NONE;
    this->category_msg_no_start = 0;
    return TRUE;
}

s32 aQMgr_talk_common_regist_set_target(QuestManager* this) {
    aQMgr_target_c* target = &this->target;
    s32 regist_index = this->regist_idx;
    aQMgr_regist_c* regist = &this->regist[regist_index];
    mQst_base_c* src_info = regist->quest_info;

    target->quest_info.questType = src_info->questType;
    target->quest_info.questKind = src_info->questKind;

    this->procs.talk_common_proc(this, aQMgr_TALK_COMMON_GET_SET_DATA);

    target->quest_item = regist->item;
    target->to_id = &regist->to_id;
    target->from_id = &regist->from_id;

    return TRUE;
}

s32 aQMgr_talk_common_change_talk_normal(QuestManager* this) {
    this->talk_state = aQMgr_TALK_STATE_INIT;
    this->talk_change_type = aQMgr_TALK_KIND_NORMAL;
    this->talk_step = 0;
    this->choice.talk_action = -1;
    mMsg_Set_LockContinue(mMsg_Get_base_window_p());

    return TRUE;
}

s32 aQMgr_set_npc_putaway(void) {
    s32 ret = FALSE;

    if (common_data.clip.handOverItem_clip->request_mode == aHOI_REQUEST_TRANS_WAIT) {
        mDemo_Set_OrderValue(mDemo_ORDER_NPC0, 1, 3);
        ret = TRUE;
    }

    return ret;
}

s32 aQMgr_talk_common_set_npc_takeout(u16 item) {
    mDemo_Set_OrderValue(mDemo_ORDER_NPC0, 1, 2);
    mDemo_Set_OrderValue(mDemo_ORDER_NPC1, 0, item);
    mDemo_Set_OrderValue(mDemo_ORDER_NPC1, 1, 7);
    mDemo_Set_OrderValue(mDemo_ORDER_NPC1, 2, 0);
    return TRUE;
}

s32 aQMgr_talk_common_set_npc_takeout_item(QuestManager* this) {
    aQMgr_talk_common_set_npc_takeout(this->target.quest_item);
    return TRUE;
}

s32 aQMgr_talk_common_set_npc_takeout_reward(QuestManager* this) {
    aQMgr_talk_common_set_npc_takeout(this->target.reward_item);
    return TRUE;
}

s32 aQMgr_check_npc_hand_item(void) {
    static s32 l_aQMgr_hand_start = FALSE;

    s32 ret = FALSE;

    if ((l_aQMgr_hand_start == FALSE) && (common_data.clip.handOverItem_clip->master_actor != NULL)) {
        l_aQMgr_hand_start = TRUE;
    }

    if ((l_aQMgr_hand_start != FALSE) && (common_data.clip.handOverItem_clip->master_actor == NULL)) {
        mMsg_Unset_LockContinue(mMsg_Get_base_window_p());
        l_aQMgr_hand_start = FALSE;
        ret = TRUE;
    }

    return ret;
}

typedef s32 (*aQMgr_COMMON_PROC)(QuestManager*);

s32 aQMgr_talk_common_proc(QuestManager* this, s32 proc) {
    static aQMgr_COMMON_PROC D_80957A0C_jp[] = {
        aQMgr_talk_common_regist_set_target,    aQMgr_talk_common_get_item_idx,
        aQMgr_talk_common_set_msg_no,           aQMgr_talk_common_clear_talk_info,
        aQMgr_talk_common_set_choice_str,       aQMgr_talk_common_get_set_data_p,
        aQMgr_talk_common_set_npc_takeout_item, aQMgr_talk_common_set_npc_takeout_reward,
        aQMgr_talk_common_talk_init_ovl,        aQMgr_talk_common_change_talk_normal,
    };

    return D_80957A0C_jp[proc](this);
}

void aQMgr_move_own_errand_cloth(UNUSED QuestManager* this, aQMgr_regist_c* regist) {
    u16 player_cloth = common_data.privateInfo->cloth.item;
    mQst_errand_c* errand = (mQst_errand_c*)regist->quest_info;
    u16 cloth = errand->item;

    switch (errand->base.progress) {
        case 2:
            if (cloth == player_cloth) {
                errand->base.progress = 0;
            }
            break;

        case 0:
            if (cloth != player_cloth) {
                errand->base.progress = 2;
            }
            break;
    }
}

void aQMgr_move_own_errand_seed(UNUSED QuestManager* this, aQMgr_regist_c* regist) {
    u16* item_p = &common_data.privateInfo->inventory.pockets[0];
    mQst_errand_c* errand = (mQst_errand_c*)regist->quest_info;

    if (errand->base.progress == 2) {
        s32 i;

        for (i = 0; i < mPr_POCKETS_SLOT_COUNT; i++, item_p++) {
            if (GET_NAME_ITEM_TYPE(*item_p) == NAME_TYPE_ITEM1 && GET_NAME_ITEM_CATEGORY(*item_p) == ITEM1_CAT_PLANT) {
                break;
            }
        }

        if (i >= mPr_POCKETS_SLOT_COUNT) {
            errand->base.progress = 0;
        }
    }
}

void aQMgr_move_own_errand_letter(UNUSED QuestManager* this, aQMgr_regist_c* regist) {
    mQst_errand_c* errand = (mQst_errand_c*)regist->quest_info;
    PrivateInfo* private_info = common_data.privateInfo;
    Animal_c* animal = common_data.animals;
    s32 i;
    Anmmem_c* memory;
    s32 idx;

    if ((errand->base.progress == 2) || (errand->base.progress == 3)) {
        for (i = 0; i < mPr_POCKETS_SLOT_COUNT; i++, animal++) {
            if (mNpc_CheckCmpAnimalPersonalID(&errand->recipient, &animal->id) == TRUE) {
                memory = animal->memories;
                idx = mNpc_GetAnimalMemoryIdx(&private_info->playerId, memory, ANIMAL_MEMORY_NUM);

                if (idx != -1) {
                    memory += idx;
                    if (memory->letterInfo.exists == TRUE) {
                        errand->base.progress = 0;
                        break;
                    }
                }
            }
        }
    }
}

void aQMgr_move_own_errand_hello(UNUSED QuestManager* this, aQMgr_regist_c* regist) {
    mQst_errand_c* errand = (mQst_errand_c*)regist->quest_info;

    if (errand->base.progress == 2) {
        if (mNpc_CheckFriendAllAnimal(&common_data.privateInfo->playerId) == TRUE) {
            errand->base.progress = 0;
        }
    }
}

void aQMgr_actor_check_limit_and_clear_quest(mQst_contest_c* contest) {
    if (mQst_CheckLimitOver(&contest->base) == TRUE) {
        mQst_ClearContest(contest);
    }
}

void aQMgr_contest_check_limit(UNUSED QuestManager* this, aQMgr_regist_c* regist) {
    aQMgr_actor_check_limit_and_clear_quest((mQst_contest_c*)regist->quest_info);
}

s32 aQMgr_actor_check_fin_fruit(mQst_base_c* quest_info, Animal_c* animal) {
    UNUSED s32 pad;
    s32 ret = FALSE;

    if (quest_info->progress == 1) {
        if (aQMgr_actor_check_fin_item(quest_info, animal) == TRUE) {
            ret = TRUE;
        }
    }

    return ret;
}

s32 aQMgr_actor_check_fin_soccer(mQst_base_c* quest_info, UNUSED Animal_c* animal) {
    s32 ret = FALSE;

    if (quest_info->progress == 1) {
        ret = TRUE;
    }

    return ret;
}

void aQMgr_actor_contest_snowman_clear(UNUSED QuestManager* this, aQMgr_regist_c* regist) {
    lbRTC_time_c* rtc_time = &common_data.time.rtcTime;

    if (regist != NULL) {
        if (regist->quest_info != NULL) {
            mQst_base_c* quest_info = regist->quest_info;

            if (((rtc_time->month == lbRTC_FEBRUARY) && (rtc_time->day >= 18)) ||
                ((rtc_time->month >= lbRTC_MARCH) && (rtc_time->month <= lbRTC_NOVEMBER)) ||
                ((rtc_time->month == lbRTC_DECEMBER) && (rtc_time->day < 25)) ||
                (mQst_CheckLimitOver(quest_info) == TRUE)) {
                mQst_ClearContest((mQst_contest_c*)quest_info);
            }
        }
    }
}

s32 aQMgr_actor_check_fin_snowman(mQst_base_c* quest_info, Animal_c* animal) {
    mQst_contest_c* contest = (mQst_contest_c*)quest_info;
    s32 ret = FALSE;

    if ((animal != NULL) && (quest_info != NULL)) {
        if (quest_info->progress == 1) {
            if ((mPr_NullCheckPersonalID(&contest->player_id) == FALSE) &&
                (mPr_CheckCmpPersonalID(&contest->player_id, &common_data.privateInfo->playerId) == TRUE)) {
                ret = TRUE;
            }
        }
    }

    return ret;
}

void aQMgr_actor_contest_flower_clear(UNUSED QuestManager* this, aQMgr_regist_c* regist) {
    lbRTC_time_c* rtc_time = &common_data.time.rtcTime;

    if (regist != NULL) {
        if (regist->quest_info != NULL) {
            mQst_base_c* quest_info = regist->quest_info;

            if ((rtc_time->month == lbRTC_JANUARY) || ((rtc_time->month == lbRTC_FEBRUARY) && (rtc_time->day < 25)) ||
                (rtc_time->month >= lbRTC_SEPTEMBER) || (mQst_CheckLimitOver(quest_info) == TRUE)) {
                mQst_ClearContest((mQst_contest_c*)quest_info);
            }
        }
    }
}

s32 aQMgr_actor_check_flower(mQst_contest_c* contest, s32 bx, s32 bz) {
    s32 flower_num;
    s32 ret = FALSE;

    flower_num = mQst_GetFlowerSeedNum(bx, bz);

    if (contest->base.progress == 1) {
        if (flower_num >= contest->info.flower_data.flowers_requested) {
            if ((mPr_NullCheckPersonalID(&contest->player_id) == TRUE) ||
                (mPr_CheckCmpPersonalID(&contest->player_id, &common_data.privateInfo->playerId) == TRUE)) {
                ret = TRUE;
            }
        } else {
            mPr_ClearPersonalID(&contest->player_id);
        }
    }
    return ret;
}

s32 aQMgr_actor_check_fin_flower(mQst_base_c* quest_info, Animal_c* animal) {
    return aQMgr_actor_check_flower((mQst_contest_c*)quest_info, animal->homeInfo.blockX, animal->homeInfo.blockZ);
}

s32 aQMgr_actor_check_fin_quest_have_item1(mQst_contest_c* contest, u8 item1_category) {
    s32 ret = FALSE;

    if (contest->base.progress == 1) {
        if ((mPr_NullCheckPersonalID(&contest->player_id) == TRUE) &&
            (mPr_GetPossessionItemIdxItem1Category(common_data.privateInfo, item1_category) != -1)) {
            ret = TRUE;
        }
    }

    return ret;
}

s32 aQMgr_actor_check_fin_fish(mQst_base_c* quest_info, UNUSED Animal_c* animal) {
    mQst_contest_c* contest = (mQst_contest_c*)quest_info;

    return aQMgr_actor_check_fin_quest_have_item1(contest, ITEM1_CAT_FISH);
}

void aQMgr_actor_contest_insect_clear(UNUSED QuestManager* this, aQMgr_regist_c* regist) {
    lbRTC_time_c* rtc_time = &common_data.time.rtcTime;

    if (regist != NULL) {
        if (regist->quest_info != NULL) {
            mQst_base_c* quest_info = regist->quest_info;

            if ((rtc_time->month < lbRTC_MARCH) || ((rtc_time->month == lbRTC_NOVEMBER) && (rtc_time->day > 28)) ||
                (rtc_time->month == lbRTC_DECEMBER) || (mQst_CheckLimitOver(quest_info) == TRUE)) {
                mQst_ClearContest((mQst_contest_c*)quest_info);
            }
        }
    }
}

s32 aQMgr_actor_check_fin_insect(mQst_base_c* quest_info, UNUSED Animal_c* animal) {
    mQst_contest_c* contest = (mQst_contest_c*)quest_info;

    return aQMgr_actor_check_fin_quest_have_item1(contest, ITEM1_CAT_INSECT);
}

s32 aQMgr_actor_check_fin_contest_letter(mQst_base_c* quest_info, UNUSED Animal_c* animal) {
    s32 ret = FALSE;

    if (quest_info->progress == 1) {
        ret = TRUE;
    }

    return ret;
}

void aQMgr_save_contest_flower(aQMgr_regist_c* regist) {
    mQst_contest_c* contest = (mQst_contest_c*)regist->quest_info;
    Animal_c* animal = common_data.animals;
    Anmhome_c* home = &animal[regist->animal_idx].homeInfo;

    if (aQMgr_actor_check_flower(contest, home->blockX, home->blockZ) == TRUE) {
        mPr_ClearPersonalID(&contest->player_id);
        mPr_CopyPersonalID(&contest->player_id, &common_data.privateInfo->playerId);
    }
}

typedef void (*aQMgr_CONTEST_SAVE_PROC)(struct aQMgr_regist_c*);

void aQMgr_save_contest(aQMgr_regist_c* regist) {
    static aQMgr_CONTEST_SAVE_PROC contest_save_proc[] = {
        (void*)none_proc1, (void*)none_proc1, (void*)none_proc1, aQMgr_save_contest_flower,
        (void*)none_proc1, (void*)none_proc1, (void*)none_proc1,
    };
    s32 quest_kind = regist->quest_info->questKind;

    contest_save_proc[quest_kind](regist);
}

void aQMgr_clear_talk_wait_info(aQMgr_talk_wait_info_c* wait_info) {
    bzero(wait_info, sizeof(aQMgr_talk_wait_info_c));
}

s32 aQMgr_actor_move_talk_sub_no_wait(UNUSED QuestManager* this) {
    return TRUE;
}

s32 aQMgr_actor_move_talk_sub_wait(QuestManager* this) {
    aQMgr_talk_wait_info_c* wait_info = &this->wait_info;
    s32 ret = FALSE;

    if (mMsg_Check_MainNormalContinue(mMsg_Get_base_window_p()) == TRUE) {
        if (wait_info->flags[2] == FALSE) {
            wait_info->flags[2] = TRUE;
            ret = TRUE;
        }
    } else {
        wait_info->flags[2] = FALSE;
    }

    return ret;
}

s32 aQMgr_actor_move_talk_sub_msg_disappear_wait(UNUSED QuestManager* this) {
    s32 ret = FALSE;

    if (mMsg_Check_main_wait(mMsg_Get_base_window_p()) == TRUE) {
        ret = TRUE;
    }

    return ret;
}

s32 aQMgr_actor_move_talk_sub_msg_appear_wait(UNUSED QuestManager* this) {
    s32 ret = FALSE;

    if (mMsg_Check_not_series_main_wait(mMsg_Get_base_window_p()) == TRUE) {
        ret = TRUE;
    }

    return ret;
}

s32 aQMgr_actor_move_talk_sub_check_button(UNUSED QuestManager* this) {
    s32 ret = FALSE;

    if (mMsg_Check_MainNormal(mMsg_Get_base_window_p()) == FALSE) {
        ret = TRUE;
    }

    return ret;
}

s32 aQMgr_actor_move_talk_sub_hand_item_wait(QuestManager* this) {
    Submenu* submenu = this->submenu;
    s32 ret = FALSE;

    if (submenu->open_flag == FALSE) {
        ret = TRUE;
        this->handover_item = submenu->item;
    }

    return ret;
}

s32 aQMgr_actor_move_talk_sub_npc_hand_wait(UNUSED QuestManager* this) {
    return aQMgr_set_npc_putaway();
}

s32 aQMgr_actor_move_talk_sub_item_wait(UNUSED QuestManager* this) {
    return aQMgr_check_npc_hand_item();
}

s32 aQMgr_actor_move_talk_sub_demo_order_wait(QuestManager* this) {
    UNUSED s32 pad;
    s32 i;
    s32 ret = FALSE;
    aQMgr_order_c* order = &this->demo_order;

    for (i = 0; i < mDemo_ORDER_VALUE_MAX; i++) {
        u16 value = mDemo_Get_OrderValue(mDemo_ORDER_QUEST, i);

        if (value != 0) {
            order->type = i;
            order->value = value;
            mDemo_Set_OrderValue(mDemo_ORDER_QUEST, i, 0);
            ret = TRUE;
            break;
        }
    }

    return ret;
}

s32 aQMgr_actor_move_talk_sub_npc_hand_wait_msg_wait(QuestManager* this) {
    aQMgr_talk_wait_info_c* wait_info = &this->wait_info;
    s32 ret = FALSE;

    if (wait_info->flags[0] == FALSE) {
        wait_info->flags[0] = aQMgr_set_npc_putaway();
        mMsg_Set_LockContinue(mMsg_Get_base_window_p());
    }

    if (wait_info->flags[1] == FALSE) {
        if (mMsg_Check_MainNormalContinue(mMsg_Get_base_window_p()) == TRUE) {
            wait_info->flags[1] = TRUE;
        }
    }

    if ((wait_info->flags[0] == TRUE) && (wait_info->flags[1] == TRUE)) {
        wait_info->flags[0] = FALSE;
        wait_info->flags[1] = FALSE;
        ret = TRUE;
    }

    return ret;
}

s32 aQMgr_actor_move_talk_sub_item_wait_end(QuestManager* this) {
    aQMgr_talk_wait_info_c* wait_info = &this->wait_info;
    s32 ret = FALSE;

    if (wait_info->flags[0] == FALSE) {
        wait_info->flags[0] = aQMgr_set_npc_putaway();
    }

    if (wait_info->flags[0] == TRUE) {
        wait_info->flags[1] = aQMgr_check_npc_hand_item();
    }

    if ((wait_info->flags[0] == TRUE) && (wait_info->flags[1] == TRUE)) {
        wait_info->flags[0] = FALSE;
        wait_info->flags[1] = FALSE;
        ret = TRUE;
    }

    return ret;
}

s32 aQMgr_actor_move_talk_sub_item_player_wait(QuestManager* this) {
    UNUSED s32 pad;
    aQMgr_talk_wait_info_c* wait_info = &this->wait_info;
    s32 ret = FALSE;

    if (wait_info->flags[0] == FALSE) {
        wait_info->flags[0] = aQMgr_check_npc_hand_item();
    }

    mMsg_Set_LockContinue(mMsg_Get_base_window_p());

    if ((wait_info->flags[0] == TRUE) && (mPlib_check_player_actor_main_index_RecieveMove(gamePT) == FALSE)) {
        mMsg_Unset_LockContinue(mMsg_Get_base_window_p());
        ret = TRUE;
        wait_info->flags[0] = FALSE;
    }

    return ret;
}

void aQMgr_select_talk(QuestManager* this) {
    u32 event_id = common_data.playerNumber + mEv_SAVED_FJOPENQUEST_PLR0;

    aQMgr_actor_init_quest(this);
    aQMgr_clear_talk_wait_info(&this->wait_info);

    if ((mLd_PlayerManKindCheck() == FALSE) && (mEv_CheckEvent(event_id) == TRUE)) {
        this->talk_type = aQMgr_TALK_KIND_QUEST;
    } else if (mEv_CheckFirstJob() == TRUE) {
        this->talk_type = aQMgr_TALK_KIND_FIRST_JOB;
    } else {
        this->talk_type = aQMgr_TALK_KIND_QUEST;
    }

    *this->mode = aQMgr_MODE_MOVE_TALK;
    this->procs.talk_common_proc(this, aQMgr_TALK_COMMON_TALK_INIT_OVL);
}

s32 aQMgr_talk_init(QuestManager* this) {
    s32 ret = FALSE;

    if (this->procs.talk_init_proc != NULL) {
        this->procs.talk_init_proc(this);
        ret = TRUE;
    }

    return ret;
}

typedef s32 (*aQMgr_TALK_SUB_PROC)(QuestManager*);

s32 aQMgr_talk_sub(QuestManager* this) {
    static aQMgr_TALK_SUB_PROC D_80957A50_jp[] = {
        aQMgr_actor_move_talk_sub_wait,
        aQMgr_actor_move_talk_sub_msg_disappear_wait,
        aQMgr_actor_move_talk_sub_msg_appear_wait,
        aQMgr_actor_move_talk_sub_check_button,
        aQMgr_actor_move_talk_sub_hand_item_wait,
        aQMgr_actor_move_talk_sub_npc_hand_wait,
        aQMgr_actor_move_talk_sub_item_wait,
        aQMgr_actor_move_talk_sub_demo_order_wait,
        aQMgr_actor_move_talk_sub_no_wait,
        aQMgr_actor_move_talk_sub_npc_hand_wait_msg_wait,
        aQMgr_actor_move_talk_sub_item_wait_end,
        aQMgr_actor_move_talk_sub_item_player_wait,
    };

    s32 sub_mode = this->sub_talk_state;
    s32 ret = D_80957A50_jp[sub_mode](this);

    if (ret == TRUE) {
        this->talk_state = aQMgr_TALK_STATE_INIT;
    }

    return ret;
}

typedef s32 (*aQMgr_TALK_PROC)(QuestManager*);

void aQMgr_move_talk(QuestManager* this) {
    static aQMgr_TALK_PROC talk_proc[] = {
        aQMgr_talk_init,
        aQMgr_talk_sub,
    };

    Actor* client = *this->client;
    s32 sub_res;

    if (mDemo_Get_talk_actor() == client) {
        aQMgr_actor_change_talk_init_ovl(this);
        sub_res = talk_proc[aQMgr_TALK_STATE_SUB](this);
        mDemo_Set_OrderValue(mDemo_ORDER_QUEST, 0, 0);
        if (sub_res == TRUE) {
            talk_proc[aQMgr_TALK_STATE_INIT](this);
        }
        aQMgr_talk_clear_talk_order(&this->demo_order);
    }

    if (mDemo_CheckDemo() == FALSE) {
        this->procs.talk_common_proc(this, aQMgr_TALK_COMMON_CLEAR_TALK_INFO);
        *this->mode = aQMgr_MODE_TALK_FIN;
        aQMgr_clear_talk_init_ovl(this);
        if ((client != NULL) && (client->part == ACTOR_PART_NPC)) {
            Animal_c* animal = ((Npc*)client)->npcInfo.animal;

            if (animal != NULL) {
                s32 memory_index =
                    mNpc_GetAnimalMemoryIdx(&common_data.privateInfo->playerId, animal->memories, ANIMAL_MEMORY_NUM);
                UNUSED s32 pad;
                Anmmem_c* memory;

                if (memory_index != -1) {
                    memory = animal->memories + memory_index;
                    lbRTC_TimeCopy(&memory->lastSpeakTime, &common_data.time.rtcTime);
                    mLd_CopyLandName(memory->land.name, common_data.landInfo.name);
                    memory->land.id = common_data.landInfo.id;
                    memory->savedTownTune = common_data.melody;
                }
            }
        }

        {
            Npc* npc = (Npc*)*this->client;
            Animal_c* animal = npc->npcInfo.animal;
            mNpc_TalkEndMove(mNpc_SearchAnimalPersonalID(&animal->id), animal->id.looks);
        }
    }
}

void aQMgr_talk_start_kamakura_common(QuestManager* this, u8 step) {
    aQMgr_clear_talk_wait_info(&this->wait_info);
    *this->mode = aQMgr_MODE_MOVE_TALK;
    this->talk_step = step;
    this->talk_type = aQMgr_TALK_KIND_NORMAL;
    this->procs.talk_common_proc(this, aQMgr_TALK_COMMON_TALK_INIT_OVL);
    aQMgr_talk_init(this);
}

void aQMgr_talk_start_kamakura_hello(QuestManager* this) {
    aQMgr_talk_start_kamakura_common(this, 8);
}

void aQMgr_talk_start_kamakura(QuestManager* this) {
    aQMgr_talk_start_kamakura_common(this, 9);
}

typedef void (*aQMgr_MOVE_PROC)(struct QuestManager*);

void aQMgr_actor_move_main(Actor* thisx, UNUSED Game_Play* game_play) {
    static aQMgr_MOVE_PROC move_proc[] = {
        aQMgr_actor_move_wait,           aQMgr_select_talk,         aQMgr_move_talk, aQMgr_actor_move_talk_fin,
        aQMgr_talk_start_kamakura_hello, aQMgr_talk_start_kamakura,
    };

    QuestManager* this = THIS;
    u8 sp1B = *this->mode;

    if (mEv_CheckTitleDemo() <= 0) {
        move_proc[sp1B](this);
    }
}

void aQMgr_actor_ct(Actor* thisx, Game_Play* game_play) {
    QuestManager* this = THIS;

    if (common_data.clip.quest_manager == NULL) {
        common_data.clip.quest_manager = (ClipQuestManager*)zelda_malloc(sizeof(*common_data.clip.quest_manager));
    }

    common_data.clip.quest_manager->talk_request_proc = aQMgr_actor_talk_request;
    common_data.clip.quest_manager->talk_start_proc = aQMgr_actor_talk_start;
    common_data.clip.quest_manager->talk_check_proc = aQMgr_actor_talk_check;

    this->client = &l_client_p;
    this->memory = &l_quest_memory_p;
    this->submenu = &game_play->submenu;
    this->mode = &l_quest_manager_mode;
    this->talk_state = aQMgr_TALK_STATE_INIT;
    this->sub_talk_state = aQMgr_TALK_SUB_STATE_WAIT;
    aQMgr_talk_common_clear_talk_info(this);
    aQMgr_actor_clear_regist(this->regist, aQMgr_REGIST_NUM);
    this->regist_idx = -1;
    this->num_regist_in_use = 0;
    aQMgr_actor_init_quest(this);

    this->procs.talk_common_proc = aQMgr_talk_common_proc;
    this->procs.clear_regist_proc = aQMgr_actor_clear_regist;
    this->procs.regist_quest_proc = aQMgr_actor_regist_quest;
    this->procs.get_time_kind_proc = aQMgr_get_time_kind;

    bzero(this->errand_next, sizeof(this->errand_next));

    if ((mEv_CheckTitleDemo() == FALSE) || (common_data.sceneNo != SCENE_TITLE_DEMO)) {
        this->procs.allocated_vram = (void*)zelda_malloc(aQMgr_OVL_SIZE);
    } else {
        this->procs.allocated_vram = NULL;
    }

    aQMgr_clear_talk_init_ovl(this);
    aQMgr_clear_talk_wait_info(&this->wait_info);
    l_quest_actor_p = this;
}

void aQMgr_actor_dt(UNUSED Actor* thisx, UNUSED Game_Play* game_play) {
    QuestManager* this = THIS;

    if (common_data.clip.quest_manager != NULL) {
        zelda_free(common_data.clip.quest_manager);
        common_data.clip.quest_manager = NULL;
    }

    if (this->procs.allocated_vram != NULL) {
        zelda_free(this->procs.allocated_vram);
    }
}

typedef void (*aQMgr_SAVE_PROC)(struct aQMgr_regist_c*);

void aQMgr_actor_save(UNUSED Actor* thisx, UNUSED Game_Play* game_play) {
    static aQMgr_SAVE_PROC save_proc[] = {
        (void*)none_proc1,
        (void*)none_proc1,
        aQMgr_save_contest,
    };

    QuestManager* this = THIS;
    aQMgr_regist_c* regist = this->regist;
    s32 i;

    aQMgr_actor_init_quest(this);

    for (i = 0; i < aQMgr_REGIST_NUM; i++, regist++) {
        if (regist->quest_info != NULL) {
            s32 type = regist->quest_info->questType;

            if ((type >= 0) && (type < QUEST_TYPE_NUM)) {
                save_proc[type](regist);
            }
        }
    }
}

ActorProfile Quest_Manager_Profile = {
    /* */ ACTOR_QUEST_MANAGER,
    /* */ ACTOR_PART_7,
    /* */ ACTOR_FLAG_10 | ACTOR_FLAG_20 | ACTOR_FLAG_20000000,
    /* */ 0x0000,
    /* */ GAMEPLAY_KEEP,
    /* */ sizeof(QuestManager),
    /* */ aQMgr_actor_ct,
    /* */ aQMgr_actor_dt,
    /* */ aQMgr_actor_move_main,
    /* */ (void*)none_proc1,
    /* */ aQMgr_actor_save,
};
