#ifndef GAME_COMMON_UNK_35710_H
#define GAME_COMMON_UNK_35710_H

#include "types.h"

struct Vec3 {
    f32 x, y, z;
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
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void SetOffset(Vec3* offset, int mode); /* +0x30 */
};

/* Effect/sound emitter embedded in the actor at +0x238 */
class Emitter {
public:
    virtual ~Emitter();
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
    virtual void Notify(int a); /* +0x58 */
    virtual void vf5C();
    virtual void vf60();
    virtual void vf64();
    virtual void vf68();
    virtual void vf6C();
    virtual void vf70();
    virtual void vf74();
    virtual void Trigger(int id, Vec3* pos); /* +0x78 */
};

/* Scripted actor (full layout of the fields used by the common actor code) */
class Actor {
public:
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
    virtual int vf11C(int a);
    virtual int vf120(int a);
    virtual int vf124(int a);
    virtual int vf128(int a);
    virtual int vf12C(int a);
    virtual int vf130(int init);
    virtual int vf134(int init);
    virtual int vf138(int init);
    virtual int vf13C(int init);
    virtual int vf140(int init);
    virtual int vf144(int a);
    virtual int vf148(int a);
    virtual int vf14C();
    virtual int vf150();
    virtual int vf154();
    virtual int vf158();
    virtual int vf15C();
    virtual int vf160();
    virtual int vf164();
    virtual int vf168();
    virtual int vf16C();
    virtual int vf170();
    virtual int vf174();
    virtual int vf178();
    virtual int vf17C();
    virtual int vf180();
    virtual int vf184();
    virtual int vf188();
    virtual int vf18C();
    virtual int vf190();
    virtual int vf194();
    virtual int vf198();
    virtual int vf19C();
    virtual int vf1A0(int a, int b);
    virtual void vf1A4();
    virtual void vf1A8();
    virtual void vf1AC();
    virtual void vf1B0();
    virtual int vf1B4();
    virtual int vf1B8(int mode, int force);
    virtual void vf1BC();
    virtual void vf1C0();
    virtual void vf1C4();
    virtual void vf1C8(f32 a, f32 b, f32 c, f32 d);
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
    virtual void vf250();
    virtual void vf254();
    virtual void vf258();
    virtual void vf25C();
    virtual void vf260();
    virtual void vf264();
    virtual void vf268();
    virtual void vf26C();
    virtual void vf270();
    virtual void vf274(f32 target);
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
    virtual int vf2E0();
    virtual void vf2E4();
    virtual void vf2E8();
    virtual void vf2EC();
    virtual void vf2F0();
    virtual void vf2F4();
    virtual void vf2F8(int a);
    virtual void vf2FC();
    virtual void vf300();
    virtual void vf304(int a);
    virtual int vf308();
    virtual void vf30C();
    virtual void vf310();
    virtual void vf314();
    virtual void vf318(f32 v);
    virtual void vf31C(int a);

    int MotionIndex(int id);                                       /* fn_18_3E4C0 */
    void SetMotion(f32 blend, f32 speed, int id, int loop, int flags); /* fn_18_36170 */
    void fn_18_3E330(int a);
    void UpdateMove();                                             /* fn_18_358A0 */
    int State6(int init);
    int State7(int init);
    int State8(int init);
    int State9(int init);
    int State10(int init);

    u8 pad04[0x19 - 4];
    u8 m19;
    u8 pad1A[0x68 - 0x1A];
    AnimPlayer mAnim;           /* 0x68 */
    u8 pad6C[0x1C6 - 0x6C];
    u8 m1C6;
    u8 pad1C7[0x238 - 0x1C7];
    Emitter mEmitter;           /* 0x238 */
    u8 pad23C[0x5F0 - 0x23C];
    int mState;                 /* 0x5F0 */
    u8 m5F4;
    u8 pad5F5[0x5F8 - 0x5F5];
    int mStep;                  /* 0x5F8 */
    int m5FC;
    u8 m600;
    u8 pad601[0x608 - 0x601];
    f32 m608;
    u8 pad60C[0x610 - 0x60C];
    f32 m610;
    u8 pad614[0x620 - 0x614];
    Vec3 mPos;                  /* 0x620 */
    Vec3 mAccel;                /* 0x62C */
    Vec3 m638;
    Vec3 m644;
    Vec3 mVel;                  /* 0x650 */
    u8 pad65C[0x678 - 0x65C];
    f32 m678, m67C, m680;
    u8 pad684[0x690 - 0x684];
    Vec3 mScale;                /* 0x690 */
    u8 pad69C[0x6A4 - 0x69C];
    u8 m6A4, m6A5, m6A6;
    u8 pad6A7;
    f32 m6A8;
    u8 pad6AC[0x6B0 - 0x6AC];
    f32 m6B0, m6B4, m6B8, m6BC, m6C0;
    u8 m6C4, m6C5;
    u8 pad6C6[0x744 - 0x6C6];
    f32 m744, m748;
    u8 pad74C[0x754 - 0x74C];
    f32 m754;
    u8 pad758[0x75C - 0x758];
    f32 m75C;
    u8 pad760[0x761 - 0x760];
    u8 m761, m762, m763;
    u8 pad764;
    u8 m765;
    u8 pad766[0x768 - 0x766];
    f32 m768;
    u8 m76C, m76D, m76E;
    u8 pad76F;
    u8 m770;
    u8 pad771[0x774 - 0x771];
    int m774;
    f32 m778, m77C;
    u8 pad780[0x7AC - 0x780];
    int m7AC;
    u8 pad7B0[0x9B4 - 0x7B0];
    int m9B4;
};

#endif
