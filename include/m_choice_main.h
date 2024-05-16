#ifndef M_CHOICE_MAIN_H
#define M_CHOICE_MAIN_H

#include "ultra64.h"

#define mChoice_CHOICE_STRING_LEN 10
#define mChoice_SELECT_STR_NUM 607

enum mChoice_Choice {
  /* 0 */ mChoice_CHOICE0,
  /* 1 */ mChoice_CHOICE1,
  /* 2 */ mChoice_CHOICE2,
  /* 3 */ mChoice_CHOICE3,
  /* 4 */ mChoice_CHOICE_NUM,
};

UNK_PTR mChoice_Get_base_window_p(void);
// void func_80065064_jp();
// void func_80065158_jp();
// void func_80065164_jp();
// void func_80065170_jp();
// void func_80065198_jp();
// void func_800651A4_jp();
void mChoice_Set_choice_data(UNK_PTR choice, u8* str0, s32 str0_len, u8* str1, s32 str1_len, u8* str2,
                                    s32 str2_len, u8* str3, s32 str3_len);
// void func_80065348_jp();
// void func_800653CC_jp();
// void func_800653DC_jp();
// void func_800654FC_jp();
// void func_80065508_jp();
// void func_80065518_jp();
// void func_80065528_jp();
// void func_80065668_jp();
// void func_8006568C_jp();
// void func_800656C8_jp();
// void func_80065704_jp();
// void func_80065728_jp();
// void func_8006574C_jp();
// void func_80065770_jp();
// void func_80065794_jp();
// void func_800657B8_jp();
// void func_800657DC_jp();
// void func_80065800_jp();
// void func_8006584C_jp();
// void func_8006586C_jp();
// void func_80065890_jp();
// void func_800658B4_jp();
// void func_800658D8_jp();
// void func_800658FC_jp();
// void func_80065920_jp();
// void func_80065944_jp();
// void func_80065968_jp();
// void func_8006598C_jp();
// void func_800659B0_jp();
// void func_800659D4_jp();
// void func_800659F8_jp();
// void func_80065A1C_jp();
// void func_80065A40_jp();
// void func_80065A64_jp();
// void func_80065A88_jp();
// void func_80065AAC_jp();
// void func_80065AD0_jp();
// void func_80065AF4_jp();
// void func_80065B18_jp();
// void func_80065B5C_jp();
// void func_80065B80_jp();
// void func_80065BA4_jp();
// void func_80065BF0_jp();
// void func_80065C10_jp();
// void func_80065C34_jp();
// void func_80065C58_jp();
// void func_80065C7C_jp();
// void func_80065CA0_jp();
// void func_80065CF8_jp();
void mChoice_Load_ChoseStringFromRom(UNK_PTR choice, u8* str, s32 str_no, Actor* actor);
// void func_80065EE8_jp();
// void func_80065EF8_jp();
// void func_80065F18_jp();
// void func_80065F38_jp();
// void func_80065F58_jp();
// void func_80065F78_jp();
// void func_80065F98_jp();
// void func_80066068_jp();
// void func_80066094_jp();
// void func_800660DC_jp();
// void func_80066130_jp();
// void func_80066194_jp();
// void func_800665C0_jp();
// void func_800665EC_jp();
// void func_80066634_jp();
// void func_8006666C_jp();
// void func_80066740_jp();
// void func_8006676C_jp();
// void func_800667B4_jp();
// void func_8006680C_jp();
// void func_8006685C_jp();
// void func_800668A4_jp();
// void func_800669A4_jp();
// void func_80066A60_jp();
// void func_80066A80_jp();
// void func_80066B18_jp();
// void func_80066BB0_jp();
// void func_80066D4C_jp();
// void func_80066DFC_jp();
// void func_80066E34_jp();

#endif
