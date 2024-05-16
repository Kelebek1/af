#ifndef M_EVENT_H
#define M_EVENT_H

#include "ultra64.h"
#include "unk.h"

/**
 * Event type definition
 *  xxxyyyyy yyyyyyyy yyyyyyyy yyyyyyyy
 *
 *  x: event type (e.g. special event, 'first job' (chores) event, holidays, ...) (0-7)
 *  y: sub-type (specific event)
 **/

#define mEv_SUBTYPE_BITS 29
#define mEv_TYPE_BITMASK (7 << mEv_SUBTYPE_BITS)
#define mEv_SUBTYPE_BITMASK ((1 << mEv_SUBTYPE_BITS) - 1)

#define mEv_GET_TYPE(event) (((event) & mEv_TYPE_BITMASK) >> mEv_SUBTYPE_BITS)
#define mEv_SET_TYPE(t) (((t) << mEv_SUBTYPE_BITS) & mEv_TYPE_BITMASK)

#define mEv_GET_SUBTYPE(event) ((event) & mEv_SUBTYPE_BITMASK)
#define mEv_SET_SUBTYPE(s) ((s) & mEv_SUBTYPE_BITMASK)

#define mEv_SET(type, subtype) (mEv_SET_TYPE(type) | mEv_SET_TYPE(subtype))

enum mEv_EventType {
    /* 0 */ mEv_SPNPC_EVENT, /* special NPC events */
    /* 1 */ mEv_SAVED_EVENT, /* events saved to data */
    /* 2 */ mEv_TYPE2_EVENT, /* unused? */
    /* 3 */ mEv_TYPE3_EVENT, /* unused? */
    /* 4 */ mEv_TYPE4_EVENT, /* unused? */
    /* 5 */ mEv_DAILY_EVENT, /* checked daily always? aSL_ReportShopOpen2Event has event 3 */
    /* 6 */ mEv_SPECL_EVENT, /* ??? secondary special npc event data? */
    /* 7 */ mEv_EVENT_TYPE_NUM,
};

enum mEv_Events {
    /* 0x00000000 */ mEv_SPNPC_SHOP = (s32)mEv_SET(mEv_SPNPC_EVENT, 0),
    /* 0x00000001 */ mEv_SPNPC_DESIGNER,
    /* 0x00000002 */ mEv_SPNPC_BROKER,
    /* 0x00000003 */ mEv_SPNPC_ARTIST,
    /* 0x00000004 */ mEv_SPNPC_ARABIAN,
    /* 0x00000005 */ mEv_SPNPC_GYPSY,
    /* 0x00000006 */ mEv_SPNPC_END,

    /* 0x20000000 */ mEv_SAVED_RENEWSHOP = (s32)mEv_SET(mEv_SAVED_EVENT, 0), /* renew shop */
    /* 0x20000001 */ mEv_SAVED_UNK1,                                         /* unused */

    /* intro through chores */
    /* 0x20000002 */ mEv_SAVED_FIRSTJOB_PLR0,
    /* 0x20000003 */ mEv_SAVED_FIRSTJOB_PLR1,
    /* 0x20000004 */ mEv_SAVED_FIRSTJOB_PLR2,
    /* 0x20000005 */ mEv_SAVED_FIRSTJOB_PLR3,

    /* selecting house */
    /* 0x20000006 */ mEv_SAVED_FIRSTINTRO_PLR0,
    /* 0x20000007 */ mEv_SAVED_FIRSTINTRO_PLR1,
    /* 0x20000008 */ mEv_SAVED_FIRSTINTRO_PLR2,
    /* 0x20000009 */ mEv_SAVED_FIRSTINTRO_PLR3,

    /* wait for next day to talk about HRA */
    /* 0x2000000A */ mEv_SAVED_HRAWAIT_PLR0,
    /* 0x2000000B */ mEv_SAVED_HRAWAIT_PLR1,
    /* 0x2000000C */ mEv_SAVED_HRAWAIT_PLR2,
    /* 0x2000000D */ mEv_SAVED_HRAWAIT_PLR3,

    /* Nook will talk about HRA when entering the shop */
    /* 0x2000000E */ mEv_SAVED_HRATALK_PLR0,
    /* 0x2000000F */ mEv_SAVED_HRATALK_PLR1,
    /* 0x20000010 */ mEv_SAVED_HRATALK_PLR2,
    /* 0x20000011 */ mEv_SAVED_HRATALK_PLR3,

    /* Do a 'favor' for a villager during chores */
    /* 0x20000012 */ mEv_SAVED_FJOPENQUEST_PLR0,
    /* 0x20000013 */ mEv_SAVED_FJOPENQUEST_PLR1,
    /* 0x20000014 */ mEv_SAVED_FJOPENQUEST_PLR2,
    /* 0x20000015 */ mEv_SAVED_FJOPENQUEST_PLR3,

    /* Player going to another town, set at train station */
    /* 0x20000016 */ mEv_SAVED_GATEWAY_PLR0,
    /* 0x20000017 */ mEv_SAVED_GATEWAY_PLR1,
    /* 0x20000018 */ mEv_SAVED_GATEWAY_PLR2,
    /* 0x20000019 */ mEv_SAVED_GATEWAY_PLR3,
    /* 0x2000001A */ mEv_SAVED_GATEWAY_FRGN, /* Foreigner */

    /* 0xA0000000 */ mEv_DAILY_0 = (s32)mEv_SET(mEv_DAILY_EVENT, 0), /* unused */
    /* 0xA0000001 */ mEv_DAILY_1,                                    /* unused */
    /* 0xA0000002 */ mEv_DAILY_2,                                    /* unused */
    /* 0xA0000003 */ mEv_DAILY_OPEN_SHOP,                            /* shop opened */

    /* 0xC0000000 */ mEv_SPECL_DESIGNER_COMPLETE = (s32)mEv_SET(mEv_SPECL_EVENT, 0)
};

typedef struct EventSaveInfo {
    /* 0x00 */ char unk00[0x9C];
} EventSaveInfo; // size >= 0x9C

typedef struct Event {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
} Event; // size = 0x10

// void func_8007D140_jp();
// void func_8007D180_jp();
void mEv_ClearEventSaveInfo(EventSaveInfo* eventSaveInfo);
void mEv_ClearEventInfo(void);
// void func_8007D25C_jp();
// void func_8007D2B8_jp();
s32 mEv_CheckEvent(u32 event);
// void func_8007D36C_jp();
// void func_8007D3F0_jp();
// void func_8007D408_jp();
// void func_8007D418_jp();
// void func_8007D42C_jp();
// void func_8007D43C_jp();
// void func_8007D44C_jp();
// void func_8007D460_jp();
// void func_8007D470_jp();
// void func_8007D490_jp();
// void func_8007D4A0_jp();
// void func_8007D4C4_jp();
// void func_8007D4D0_jp();
// void func_8007D4F4_jp();
// void func_8007D52C_jp();
// void func_8007D554_jp();
// void func_8007D578_jp();
// void func_8007D58C_jp();
// void func_8007D59C_jp();
s32 mEv_CheckRealArbeit(void);
// void func_8007D650_jp();
s32 mEv_CheckFirstJob(void);
// void func_8007D734_jp();
// void func_8007D784_jp();
UNK_RET mEv_CheckFirstIntro(void);
// void func_8007D810_jp();
void mEv_SetGateway(void);
// void func_8007D880_jp();
void mEv_UnSetGateway(void);
s32 mEv_CheckTitleDemo(void);
// void func_8007D91C_jp();
// void func_8007D930_jp();
// void func_8007D9C8_jp();
// void func_8007DA64_jp();
void mEv_GetEventWeather(s16*,s16*);
// void func_8007DACC_jp();
// void func_8007DB20_jp();
// void func_8007DBB0_jp();
// void func_8007DD84_jp();
// void func_8007DDE8_jp();
// void func_8007DE2C_jp();
// void func_8007DF04_jp();
// void func_8007DF98_jp();
// void func_8007E0FC_jp();
// void func_8007E140_jp();
// void func_8007E184_jp();
// void func_8007E1D8_jp();
// void func_8007E22C_jp();
// void func_8007E264_jp();
// void func_8007E3E8_jp();
// void func_8007E518_jp();
// void func_8007E60C_jp();
// void func_8007E690_jp();
// void func_8007E714_jp();
// void func_8007E780_jp();
// void func_8007EB6C_jp();
// void func_8007EC28_jp();
// void func_8007EC54_jp();
// void func_8007ECA0_jp();
// void func_8007ECD4_jp();
// void func_8007ED08_jp();
// void func_8007ED70_jp();
// void func_8007EED4_jp();
// void func_8007EF18_jp();
// void func_8007F1A8_jp();
// void func_8007F28C_jp();
// void func_8007F2D8_jp();
// void func_8007F318_jp();
// void func_8007F358_jp();
// void func_8007F6A0_jp();
// void func_8007F764_jp();
void mEv_init(Event* event);
void mEv_init_force(void*);
void mEv_2nd_init(Event* event);
// void func_8007F950_jp();
void mEv_run(Event* event);
void mEv_finish(Event* event);
// void func_8007FCB8_jp();
// void func_8007FD40_jp();
// void func_8007FDA8_jp();
// void func_8007FE0C_jp();
void mEv_clear_status(s32, s16);
// void func_8007FEBC_jp();
s32 mEv_check_status(s32, s16);
// void func_8007FF8C_jp();
// void func_8007FFC4_jp();
// void func_80080000_jp();
// void func_80080040_jp();
// void func_80080080_jp();
// void func_80080200_jp();
u8* func_8008033C_jp(s32,s32);
// void func_800804AC_jp();
// void func_800806A0_jp();
// void func_800807E0_jp();
// void func_800808E0_jp();
// void func_80080A50_jp();
// void func_80080C68_jp();
// void func_80080D68_jp();
// void func_80080F0C_jp();
// void func_8008114C_jp();
// void func_80081360_jp();
// void func_80081424_jp();
// void func_80081434_jp();
// void func_80081460_jp();
// void func_800814B8_jp();
// void func_800815F0_jp();
// void func_800816C0_jp();
// void func_8008172C_jp();
// void func_80081800_jp();
// void func_8008180C_jp();
// void func_80081818_jp();
// void mEv_debug_print4f();
// void mEv_sp_debug_print4f();
// void func_80081B98_jp();
// void func_80081BA4_jp();
// void func_80081D24_jp();
// void func_80081E38_jp();
// void func_80081E48_jp();

#endif
