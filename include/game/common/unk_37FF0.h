#ifndef GAME_COMMON_UNK_37FF0_H
#define GAME_COMMON_UNK_37FF0_H

#include "types.h"

namespace unk_37FF0 {

struct Vec {
    f32 x, y, z;
};

/* Vec with a constructor, for temporaries (Vec itself stays trivial so locals optimize like plain data). */
struct VecInit : Vec {
    VecInit(f32 _x, f32 _y, f32 _z) {
        x = _x;
        y = _y;
        z = _z;
    }
};

/* Vertical capsule description: three radii/extents, an id, and its end points. */
struct Capsule {
    f32 r0, r1, r2;
    s32 id;
    Vec p0;
    Vec p1;
};

struct StateEntry {
    u8 blocked;
    u8 queued;
    u8 pad[6];
};

/* Scripted actor: states 0-5 are the movement states entered through vf118..vf12C. */
class Actor {
public:
    enum { kEnterFailed = 5 };

    virtual ~Actor();
    virtual void vfC();
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
    virtual void vf118();
    virtual void vf11C();
    virtual void vf120();
    virtual int vf124(int a, int b);
    virtual void vf128(int a, int b);
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
    virtual void vf1D0(f32 v, int a);
    virtual void vf1D4();
    virtual void vf1D8();
    virtual void vf1DC();
    virtual void vf1E0();
    virtual void vf1E4(int a);
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
    virtual int vf21C();
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
    virtual void vf250();
    virtual void vf254();
    virtual void vf258();
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
    virtual void vf2C0();
    virtual void vf2C4();
    virtual void vf2C8();
    virtual void vf2CC();
    virtual void vf2D0();
    virtual void vf2D4();
    virtual void vf2D8();
    virtual void vf2DC();
    virtual void vf2E0();
    virtual void vf2E4();
    virtual void vf2E8();
    virtual void vf2EC();
    virtual void vf2F0();
    virtual void vf2F4();
    virtual void vf2F8();
    virtual void vf2FC();
    virtual void vf300();
    virtual void vf304();
    virtual void vf308();
    virtual void vf30C();
    virtual void vf310();
    virtual void vf314();
    virtual void vf318();
    virtual void vf31C(int a);

    int EnterState(int state);                                         /* fn_18_3E4E0 (entry index) */
    int HasMotion(int id);                                             /* fn_18_3E4D0 */
    void SetMotion(f32 blend, f32 speed, int id, int loop, int flags); /* fn_18_36170 */
    void fn_18_3E330(int a);
    int fn_18_3DD50();

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

    int GetM774();
    void SetM764(u8 v);
    u8 GetM764();
    int OverlapTop(Capsule* a, Capsule* b, Actor* other);
    void StartIdleMotion();
    int State0(int init);
    int State1(int init);
    int State2(int init);
    int State3(int init, int variant);
    int IsTimerShort();
    int State4(int init);
    int State5(int init);
    void SetMotionId(int id);

    u8 pad04[0x5F0 - 4];
    int mState;                 /* 0x5F0 */
    u8 m5F4;
    u8 pad5F5[3];
    int mStep;                  /* 0x5F8 */
    u8 pad5FC[0x610 - 0x5FC];
    f32 m610;
    u8 pad614[0x620 - 0x614];
    Vec vel;                    /* 0x620 */
    f32 m62C;
    f32 m630;
    f32 m634;
    u8 pad638[0x664 - 0x638];
    f32 m664;
    f32 m668;
    u8 pad66C[0x674 - 0x66C];
    f32 m674;
    u8 pad678[0x69C - 0x678];
    f32 m69C;
    u8 pad6A0[0x6C5 - 0x6A0];
    u8 m6C5;
    u8 pad6C6[0x6C8 - 0x6C6];
    StateEntry mEntries[8];     /* 0x6C8 */
    u8 pad708[0x744 - 0x708];
    f32 m744;
    u8 pad748[0x750 - 0x748];
    f32 m750;
    u8 pad754[0x758 - 0x754];
    f32 mTimer;                 /* 0x758 */
    u8 pad75C[0x760 - 0x75C];
    u8 m760;
    u8 pad761[0x764 - 0x761];
    u8 m764;
    u8 pad765[0x768 - 0x765];
    f32 scale;                  /* 0x768 */
    u8 pad76C[0x76F - 0x76C];
    u8 m76F;
    u8 pad770[0x774 - 0x770];
    s32 m774;
    u8 pad778[0x9A4 - 0x778];
    f32 ext[2];                 /* 0x9A4 */
};

} // namespace unk_37FF0

#endif
