#ifndef GAME_COMMON_UNK_3A510_H
#define GAME_COMMON_UNK_3A510_H

#include "types.h"

struct Vec3 {
    Vec3() {}
    Vec3(f32 _x, f32 _y, f32 _z) { x = _x; y = _y; z = _z; }
    f32 x, y, z;
};

/* Secondary object embedded in Actor (vtable at offset 0) */
class ActorSub {
public:
    virtual void vf08();
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
};

/* main.dol animation/motion controller embedded in Actor at 0x68 */
class AnimCtrl {
public:
    void fn_80061D80(f32 v);
    void fn_80060220(const void* p);
    u8 mData[0x5C];
};

/* Paired-single vector helpers (inline asm) */
inline void VecScale(register const Vec3* src, register Vec3* dst, register f32 scale) {
    register f32 r, t;
    asm {
        psq_l t, 0(src), 0, 0
        ps_muls0 r, t, scale
        psq_l t, 8(src), 1, 0
        psq_st r, 0(dst), 0, 0
        ps_muls0 r, t, scale
        psq_st r, 8(dst), 1, 0
    }
}

inline void VecAdd(register const Vec3* a, register const Vec3* b, register Vec3* dst) {
    register f32 tb, ta;
    asm {
        psq_l tb, 0(b), 0, 0
        psq_l ta, 0(a), 0, 0
        ps_add tb, ta, tb
        psq_l ta, 8(a), 1, 0
        psq_st tb, 0(dst), 0, 0
        psq_l tb, 8(b), 1, 0
        ps_add tb, ta, tb
        psq_st tb, 8(dst), 1, 0
    }
}

class Actor;

void fn_18_34F70(Actor* a);
void fn_18_3E620(Actor* a);
void fn_18_36170(Actor* a, int arg, f32 x, f32 y, int c, int d);
BOOL fn_18_3E070(Actor* a, f32 f, f32* out);
void fn_18_3CBC0(AnimCtrl* a);
void fn_18_3CC90(AnimCtrl* a, int arg);
Vec3 fn_18_41680(const Vec3& axis, f32 speed);
extern "C" f32 fn_80192D20(const Vec3* v);                   /* PSVECMag */
extern "C" void fn_80192CD0(const Vec3* src, Vec3* dst);     /* PSVECNormalize */
extern const f32 lbl_8026E3B4;

/* Moving actor (virtual interface reaches 0x2B4) */
class Actor {
public:
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
    virtual void vf118(int a, int b);
    virtual void vf11C();
    virtual void vf120();
    virtual void vf124();
    virtual int vf128(int a, int b);
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
    virtual void vf1A0();
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
    virtual bool vf1CC();
    virtual void vf1D0();
    virtual void vf1D4(void* a);
    virtual void vf1D8(const Vec3& v);
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
    virtual void vf248(f32 a, f32 b, f32 c);
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

    bool IsBusy();
    void Reset();
    void SetHeading(f32 speed, void* arg);
    void Fn_3A850(int a);
    void SetVelocityXZ(const Vec3* v);
    void UpdateVelocity();
    void CallSubVf30();

    /* Resolves the current target heading (angle) into 'out'; returns nonzero if one exists. */
    BOOL GetTarget(f32* out) {
        if (vf1CC()) {
            *out = 0.0f;
            return FALSE;
        }
        if (m19 && m6A4) {
            *out = m6C0;
            return m6C4;
        }
        return fn_18_3E070(this, m65C, out);
    }

    u8 pad04[0x19 - 0x4];
    u8 m19;                               /* 0x19 */
    u8 pad1A[0x68 - 0x1A];
    AnimCtrl mAnim;                       /* 0x68 */
    u8 padC4[0x238 - 0xC4];
    ActorSub mSub;                        /* 0x238 */
    u8 pad23C[0x5FC - 0x23C];
    s32 mState;                           /* 0x5FC */
    u8 pad600[0x610 - 0x600];
    f32 m610;                             /* 0x610 */
    u8 pad614[0x62C - 0x614];
    Vec3 mVel;                            /* 0x62C */
    u8 pad638[0x65C - 0x638];
    f32 m65C;                             /* 0x65C */
    u8 pad660[0x684 - 0x660];
    f32 m684;                             /* 0x684 */
    f32 m688;                             /* 0x688 */
    u8 pad68C[0x690 - 0x68C];
    u8 m690[0x6A4 - 0x690];               /* 0x690 */
    u8 m6A4;                              /* 0x6A4 */
    u8 pad6A5[0x6B8 - 0x6A5];
    f32 m6B8;                             /* 0x6B8 */
    u8 pad6BC[0x6C0 - 0x6BC];
    f32 m6C0;                             /* 0x6C0 */
    u8 m6C4;                              /* 0x6C4 */
    u8 pad6C5[0x744 - 0x6C5];
    f32 m744;                             /* 0x744 */
    u8 pad748[0x75C - 0x748];
    f32 m75C;                             /* 0x75C */
    u8 pad760[0x762 - 0x760];
    u8 m762;                              /* 0x762 */
    u8 pad763[0x76C - 0x763];
    u8 m76C;                              /* 0x76C */
    u8 m76D;                              /* 0x76D */
    u8 pad76E;
    u8 m76F;                              /* 0x76F */
    u8 pad770[0x7B1 - 0x770];
    u8 m7B1;                              /* 0x7B1 */
    u8 pad7B2[0x7C0 - 0x7B2];
    u8 m7C0;                              /* 0x7C0 */
    u8 pad7C1[0x7D4 - 0x7C1];
    u8 m7D4;                              /* 0x7D4 */
};

#endif
