#ifndef GAME_COMMON_UNK_38C00_H
#define GAME_COMMON_UNK_38C00_H

#include "types.h"

struct Vec3 {
    f32 x, y, z;
    Vec3() {}
    Vec3(f32 _x, f32 _y, f32 _z) {
        x = _x;
        y = _y;
        z = _z;
    }
};

/* Animation player embedded in the actor at +0x68 */
class AnimPlayer {
public:
    virtual ~AnimPlayer();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void PlayNamed(int id, const char* name); /* +0x20 */

    BOOL IsDone(); /* fn_18_3CCA0 */

    u8 pad[0x5F0 - 0x68 - 4];
};

struct StateEntry {
    u8 blocked;
    u8 queued;
    u8 pad[6];
};

/* Scripted actor with a per-state update function (arg: first call) */
class Actor {
public:
    enum { kEnterFailed = 5 };

    virtual ~Actor();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C();
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C();
    virtual void vf60();
    virtual void vf64();
    virtual void vf68();
    virtual void vf6C();
    virtual void vf70();
    virtual void vf74();
    virtual void vf78();
    virtual void vf7C();
    virtual void vf80();
    virtual void vf84();
    virtual void vf88();
    virtual void vf8C();
    virtual void vf90();
    virtual void vf94();
    virtual void vf98();
    virtual void vf9C();
    virtual void vfA0();
    virtual void vfA4();
    virtual void vfA8();
    virtual void vfAC();
    virtual void vfB0();
    virtual void vfB4();
    virtual void vfB8();
    virtual void vfBC();
    virtual void vfC0();
    virtual void vfC4();
    virtual void vfC8();
    virtual void vfCC();
    virtual void vfD0();
    virtual void vfD4();
    virtual void vfD8();
    virtual void vfDC();
    virtual void vfE0();
    virtual void vfE4();
    virtual void vfE8();
    virtual void vfEC();
    virtual void vfF0();
    virtual void vfF4();
    virtual void vfF8();
    virtual void vfFC();
    virtual void vf100();
    virtual void vf104();
    virtual void vf108();
    virtual void vf10C();
    virtual void vf110();
    virtual void vf114();
    virtual int vf118(int a, int b);
    virtual void vf11C();
    virtual void vf120();
    virtual void vf124();
    virtual void vf128();
    virtual void vf12C();
    virtual void vf130();
    virtual void vf134();
    virtual void vf138();
    virtual void vf13C();
    virtual void vf140();
    virtual void vf144();
    virtual void vf148();
    virtual void vf14C();
    virtual void vf150();
    virtual void vf154();
    virtual void vf158();
    virtual void vf15C();
    virtual void vf160();
    virtual void vf164();
    virtual void vf168();
    virtual void vf16C();
    virtual void vf170();
    virtual void vf174();
    virtual void vf178();
    virtual void vf17C();
    virtual void vf180();
    virtual void vf184();
    virtual void vf188();
    virtual void vf18C();
    virtual void vf190();
    virtual void vf194();
    virtual void vf198();
    virtual void vf19C();
    virtual int vf1A0(int a, int b);
    virtual void vf1A4();
    virtual void vf1A8();
    virtual void vf1AC();
    virtual void vf1B0();
    virtual void vf1B4();
    virtual void vf1B8();
    virtual void vf1BC();
    virtual void vf1C0();
    virtual void vf1C4();
    virtual void vf1C8();
    virtual void vf1CC();
    virtual void vf1D0();
    virtual void vf1D4();
    virtual void vf1D8();
    virtual void vf1DC();
    virtual void vf1E0();
    virtual void vf1E4();
    virtual void vf1E8();
    virtual void vf1EC();
    virtual void vf1F0();
    virtual void vf1F4();
    virtual void vf1F8();
    virtual void vf1FC();
    virtual void vf200();
    virtual void vf204();
    virtual void vf208();
    virtual void vf20C();
    virtual void vf210();
    virtual void vf214();
    virtual void vf218();
    virtual void vf21C();
    virtual void vf220();
    virtual void vf224();
    virtual void vf228();
    virtual void vf22C();
    virtual void vf230();
    virtual void vf234();
    virtual void vf238();
    virtual void vf23C();
    virtual void vf240();
    virtual void vf244();
    virtual void vf248(f32 x, f32 y, f32 z);
    virtual void vf24C();
    virtual void vf250(f32 v);
    virtual void vf254();
    virtual void vf258(f32 v);
    virtual void vf25C();
    virtual void vf260();
    virtual void vf264();
    virtual void vf268();
    virtual void vf26C();
    virtual void vf270();
    virtual void vf274();
    virtual void vf278();
    virtual void vf27C();
    virtual void vf280();
    virtual void vf284();
    virtual void vf288();
    virtual void vf28C();
    virtual void vf290();
    virtual void vf294();
    virtual void vf298();
    virtual void vf29C();
    virtual void vf2A0();
    virtual void vf2A4();
    virtual void vf2A8();
    virtual void vf2AC();
    virtual void vf2B0();
    virtual int vf2B4();
    virtual void vf2B8();
    virtual void vf2BC();
    virtual int vf2C0();
    virtual void vf2C4();
    virtual void vf2C8();
    virtual void vf2CC();
    virtual void vf2D0();
    virtual void vf2D4();
    virtual void vf2D8();
    virtual void vf2DC(Vec3 pos, int mode, f32 scale);
    virtual void vf2E0();
    virtual void vf2E4();
    virtual void vf2E8();
    virtual void vf2EC();
    virtual void vf2F0();
    virtual void vf2F4();
    virtual void vf2F8(int a);
    virtual void vf2FC();
    virtual void vf300();
    virtual void vf304();
    virtual void vf308();
    virtual void vf30C();
    virtual void vf310();
    virtual void vf314();
    virtual void vf318();
    virtual void vf31C(int a);

    int EnterState(int state);                                    /* fn_18_3E4E0 (index) */
    void SetMotion(f32 blend, f32 speed, int id, int loop, int flags); /* fn_18_36170 */
    void fn_18_3E330(int a);

    /* inlined state entry check */
    int BeginState(int state) {
        StateEntry* e = &mEntries[EnterState(state)];
        int r;
        if (!e->blocked) {
            r = 0;
        } else if (e->queued) {
            r = 1;
        } else if (mState == state) {
            r = 2;
        } else {
            mState = state;
            mTimer = 0.0f;
            m5F4 = 1;
            r = kEnterFailed;
        }
        return r;
    }

    int State6(int init);  /* fn_18_38FF0 */
    int State7(int init);  /* fn_18_38C00 */
    int State8(int init);  /* fn_18_38E00 */
    int State9(int init);  /* fn_18_39430 */
    int State10(int init); /* fn_18_39610 */

    u8 pad04[0x68 - 4];
    AnimPlayer mAnim;           /* 0x68 */
    int mState;                 /* 0x5F0 */
    u8 m5F4;
    int mStep;                  /* 0x5F8 */
    int m5FC;
    u8 pad600[0x650 - 0x600];
    Vec3 mVel;                  /* 0x650 */
    u8 pad65C[0x69C - 0x65C];
    f32 m69C;
    u8 pad6A0[0x6C8 - 0x6A0];
    StateEntry mEntries[8];     /* 0x6C8 */
    u8 pad708[0x744 - 0x708];
    f32 m744, m748;
    u8 pad74C[0x750 - 0x74C];
    f32 m750;
    u8 pad754[0x758 - 0x754];
    f32 mTimer;                 /* 0x758 */
    u8 pad75C[0x760 - 0x75C];
    u8 m760, m761;
    u8 pad762[0x765 - 0x762];
    u8 m765, m766;
};

#endif
