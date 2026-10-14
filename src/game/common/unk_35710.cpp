/* Shared by most REL modules. Reference copy: mg9101 (fn_18_35710). */
#include "game/common/unk_35710.h"

extern f32 lbl_8026E3B4; /* frame-time scale */

extern "C" {
void fn_80061B50(AnimPlayer*);
void fn_80060180(AnimPlayer*, f32, f32, f32);
int fn_8005DC80(AnimPlayer*);
void fn_8005DB90(AnimPlayer*, int);
void fn_80061D80(AnimPlayer*, f32);
void fn_80060220(AnimPlayer*, Vec3*);
f32 fn_80192D20(Vec3*);
f32 fn_8016D6E0(f32, f32);
int fn_18_41590(f32*, f32, f32, f32);
void fn_18_3CC90(AnimPlayer*, int);
void fn_18_3CBC0(AnimPlayer*);
void fn_18_3CC60(AnimPlayer*, f32, f32, int, int, int);
int fn_18_38430(Actor*, int, int);
int fn_18_38560(Actor*, int);
int fn_18_38660(Actor*, int);
int fn_18_38760(Actor*, int);
int fn_18_38990(Actor*, int);
int fn_18_38AB0(Actor*, int);
int fn_18_397F0(Actor*, int);
int fn_18_399D0(Actor*, int);
int fn_18_3A180(Actor*, int, int);
}

void Actor::vf100() {}

void Actor::vf10C() {
    switch (mState) {
    case 1:
        mAnim.SetOffset(&mPos, 1);
        fn_80061B50(&mAnim);
        mEmitter.Trigger(0, &mPos);
        break;
    case 0:
        if (!m6A5 || !m6A6) {
            mAnim.SetOffset(&mPos, 2);
            fn_80061B50(&mAnim);
            mEmitter.Trigger(0x20, &mPos);
            break;
        }
    case 2:
        mAnim.SetOffset(&mPos, 0);
        fn_80061B50(&mAnim);
        mEmitter.Trigger(1, &mPos);
        break;
    default:
        mAnim.SetOffset(&mPos, 2);
        fn_80061B50(&mAnim);
        mEmitter.Trigger(0x20, &mPos);
        break;
    }
}

void Actor::vf104() { UpdateMove(); }

/* Paired-single vector helpers (inline asm, as in the SDK). */
static inline void VecScale(register const Vec3* src, register Vec3* dst, register f32 scale) {
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

static inline void VecAddTo(register const Vec3* b, register Vec3* a) {
    register f32 r, tb, ta;
    asm {
        psq_l ta, 0(a), 0, 0
        psq_l tb, 0(b), 0, 0
        ps_add r, ta, tb
        psq_l ta, 8(a), 1, 0
        psq_l tb, 8(b), 1, 0
        psq_st r, 0(a), 0, 0
        ps_add r, ta, tb
        psq_st r, 8(a), 1, 0
    }
}

static inline f32 Ratio(f32 num, f32 den) {
    if (num < 0.0f) {
        num = 0.0f;
    }
    return num / den;
}

static inline f32 AddF(f32 a, f32 b) { return a + b; }

void Actor::UpdateMove() {
    Vec3 v;
    if (vf2B4() && mAccel.y <= 0.0f) {
        v = m638;
        mAccel.y = 0.0f;
        m774 = 0;
        m765 = 0;
        m5FC = 0;
    } else {
        v = m644;
        if (!m600) {
            f32 g = m678 * lbl_8026E3B4;
            mAccel.y += g;
        }
        if (mAccel.y < m67C) {
            mAccel.y = m67C;
        }
        m600 = 0;
    }
    VecAddTo(&mAccel, &v);
    if (m744 > 0.0f) {
        Vec3 push = mVel;
        if (m761) {
            VecScale(&push, &push, Ratio(m744, m748));
        }
        VecAddTo(&push, &v);
    }
    Vec3 delta;
    VecScale(&v, &delta, lbl_8026E3B4);
    VecAddTo(&delta, &mPos);
}

/* Not referenced by the linked module; kept so the constant pool keeps the
   original layout. */
__declspec(weak) void ConstPoolHead(f32* p) {
    p[0] = 10.0f;
    p[1] = 1080.0f;
    p[2] = 1.0f;
    p[3] = 1.5f;
}

void Actor::vf274(f32 target) {
    f32 out;
    if (m6A5 && !m6A4) {
        m6C5 = 0;
        if (fn_18_41590(&out, m608, m6B4, m6BC)) {
            m6C5 = 1;
            vf1F0();
        }
        m610 = out;
    } else {
        m6C5 = fn_18_41590(&out, m608, target, m680);
    }
    fn_80060180(&mAnim, 0.0f, out, 0.0f);
}

void Actor::vf270() { m600 = 1; }

int Actor::vf1B8(int mode, int force) {
    if (!force && vf2E0()) {
        return 0;
    }
    if (mode == 3) {
        m762 = 1;
        m754 = 1.84f;
        if (mAccel.y > 0.0f) {
            mAccel.y = 0.0f;
        }
        mAccel.z = 0.0f;
        mAccel.x = 0.0f;
        m600 = 1;
        m744 = 0.0f;
        vf118(1, 0);
        fn_18_3CC90(&mAnim, 0);
        fn_18_3CBC0(&mAnim);
        return 1;
    }
    if (mode == 2) {
        m762 = 0;
        m754 = 0.67f;
        fn_18_3E330(0);
        vf31C(0x18);
        vf31C(8);
        return 1;
    }
    return 0;
}

static inline f32 Ease3(f32 x, f32 k) {
    f32 y = x * k;
    if (y > 2.0f) {
        return y - 2.0f;
    }
    if (y > 1.0f) {
        return 2.0f * (y - 1.0f) / k;
    }
    return y / k;
}
static inline f32 EaseOut(f32 x) { return Ease3(x, 3.0f); }

struct PhaseTimes {
    f32 a, b, c;
};
static const PhaseTimes kPhaseA = {0.33333334f, 0.53666669f, 0.13333334f};
static const PhaseTimes kPhaseB = {0.33333334f, 1.7566667f, 0.083333336f};

void Actor::vf1C4() {
    u8 one = 0;
    m76E = one;
    m763 = one;
    m77C = 0.0f;
    f32 t = m754;
    if (t > 0.0f) {
        f32 a = 1.0f;
        f32 b = a;
        if (m762) {
            one = 1;
            m76D = one;
            if (t > 1.7566667f) {
                vf2F8(0);
                m763 = one;
                a = 1.0f - (m754 - 1.7566667f) / 0.083333336f;
                m9B4 = fn_8005DC80(&mAnim);
            } else if (t < 0.33333334f) {
                if (!m770) {
                    m770 = one;
                    vf31C(9);
                }
                a = m754 / 0.33333334f;
                f32 v = m75C;
                f32 k = v * a;
                v -= k;
                fn_80061D80(&mAnim, v);
                m76D = 0;
                fn_8005DB90(&mAnim, m9B4);
                a = EaseOut(a);
            } else {
                m77C = m778;
                fn_8005DB90(&mAnim, 0);
                m763 = one;
                m76E = one;
                fn_80061D80(&mAnim, 0.0f);
                m1C6 = one;
            }
            vf1C8(a, 1.5f, 0.05f, b);
        } else {
            one = 1;
            m76C = one;
            if (t > 0.53666669f) {
                m1C6 = one;
                a = a - (t - 0.53666669f) / 0.13333334f;
            } else if (t < 0.33333334f) {
                if (!m770) {
                    m770 = one;
                    vf31C(9);
                }
                m76C = 0;
                a = EaseOut(m754 / 0.33333334f);
            }
            vf1C8(a, 1.2f, 0.5f, b);
        }
    } else {
        vf2F8(0);
        m76C = one;
        m76D = one;
        m768 = 1.0f;
        m762 = one;
        fn_80061D80(&mAnim, m75C);
        fn_80060220(&mAnim, &mScale);
        m770 = one;
    }
}

void Actor::vf1C8(f32 a, f32 b, f32 c, f32 d) {
    Vec3 v;
    f32 bm = b - 1.0f;
    f32 z0 = mScale.z;
    f32 x0 = mScale.x;
    f32 s2 = bm * a;
    f32 id = 1.0f - d;
    f32 ic = 1.0f - c;
    f32 s = 1.0f + s2;
    v.x = x0 * s;
    f32 y0 = mScale.y;
    f32 keep = y0 * id;
    v.z = z0 * s;
    f32 ia = 1.0f - a;
    f32 t3 = ia * ic;
    f32 t = c + t3;
    v.y = y0 * t;
    m768 = t;
    f32 mixed = v.y * d;
    v.y = keep + mixed;
    fn_80060220(&mAnim, &v);
}

int Actor::vf1B4() { return m5FC; }
void Actor::vf304(int a) {
    m7AC = a;
    mEmitter.Notify(a);
}
int Actor::vf308() { return m7AC; }
void Actor::vf318(f32 v) { m778 = v; }
static inline void SetVec3(Vec3* v, f32 x, f32 y, f32 z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

void Actor::vf1F4() {
    if (m19 && m6A4) {
        Vec3 d;
        SetVec3(&d, m6A8 - mPos.x, 0.0f, m6B0 - mPos.z);
        if (fn_80192D20(&d) > m6B8 * lbl_8026E3B4) {
            m6C4 = 1;
            f32 dz = d.z;
            f32 dx = d.x;
            m6C0 = 1.40625f * fn_8016D6E0(dx, dz);
        } else {
            m6C4 = 0;
            m6A4 = 0;
            mPos.x = m6A8;
            mPos.z = m6B0;
            if (!m6A5) {
                vf1F0();
            }
        }
    }
}

void Actor::SetMotion(f32 blend, f32 speed, int id, int loop, int flags) {
    fn_18_3CC60(&mAnim, blend, speed, MotionIndex(id), loop, flags);
}

int Actor::vf118(int a, int b) { return fn_18_38430(this, a, b); }
int Actor::vf11C(int a) { return fn_18_38560(this, a); }
int Actor::vf120(int a) { return fn_18_38660(this, a); }
int Actor::vf124(int a) { return fn_18_38760(this, a); }
int Actor::vf128(int a) { return fn_18_38990(this, a); }
int Actor::vf12C(int a) { return fn_18_38AB0(this, a); }
int Actor::vf130(int init) { return State7(init); }
int Actor::vf134(int init) { return State8(init); }
int Actor::vf138(int init) { return State6(init); }
int Actor::vf13C(int init) { return State9(init); }
int Actor::vf140(int init) { return State10(init); }
int Actor::vf144(int a) { return fn_18_397F0(this, a); }
int Actor::vf148(int a) { return fn_18_399D0(this, a); }
int Actor::vf14C() { return 0; }
int Actor::vf150() { return 0; }
int Actor::vf154() { return 0; }
int Actor::vf158() { return 0; }
int Actor::vf15C() { return 0; }
int Actor::vf160() { return 0; }
int Actor::vf164() { return 0; }
int Actor::vf168() { return 0; }
int Actor::vf16C() { return 0; }
int Actor::vf170() { return 0; }
int Actor::vf174() { return 0; }
int Actor::vf178() { return 0; }
int Actor::vf17C() { return 0; }
int Actor::vf180() { return 0; }
int Actor::vf184() { return 0; }
int Actor::vf188() { return 0; }
int Actor::vf18C() { return 0; }
int Actor::vf190() { return 0; }
int Actor::vf194() { return 0; }
int Actor::vf198() { return 0; }
int Actor::vf19C() { return 0; }
int Actor::vf1A0(int a, int b) { return fn_18_3A180(this, a, b); }

/* Tuning table */
f32 gTuneHead[5] = {6.0f, 3.0f, 6.0f, 40.0f, -9.8f};

struct ScaledTune {
    f32 a, b, c, d, e, f, g;
};
ScaledTune gScaledTune;

extern "C" void __sinit_unk_35710_cpp() {
    const f32* s = gTuneHead;
    f32* d = &gScaledTune.a;
    for (int i = 0; i < 5; i++) {
        *d++ = 10.0f * *s++;
    }
    gScaledTune.f = 5.0f * gScaledTune.e;
    gScaledTune.g = 4.0f * -gScaledTune.e;
}

__declspec(section ".ctors") void (*const sCtorEntry)() = __sinit_unk_35710_cpp;
