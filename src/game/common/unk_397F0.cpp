/* Shared by most REL modules. Reference copy: mg9101 (fn_18_397F0). */
#include "types.h"

struct Vec {
    f32 x, y, z;
    Vec() {}
    Vec(f32 px, f32 py, f32 pz) : x(px), y(py), z(pz) {}
};

extern "C" double fmod(double, double);
extern "C" f32 fn_80192D20(const Vec* v);              /* PSVECMag */
extern "C" void fn_80192CD0(const Vec* src, Vec* dst); /* PSVECNormalize */
extern "C" f32 fn_8016D6E0(f32 x, f32 z);              /* atan2 */

extern f32 lbl_46B8;
extern char lbl_46BC[];

class SubObj {
public:
    virtual void sv00();
    virtual void sv04();
    virtual void sv08();
    virtual void sv0C();
    virtual void sv10();
    virtual void sv14();
    virtual void SetName(const char* name, const char* arg);
    u8 mPad[0x20];
};

int fn_18_3CCA0(SubObj* sub);

struct Entry {
    u8 mFlagA;
    u8 mFlagB;
    u8 mPad[6];
};

class Actor {
public:
    virtual void vf8();
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
    virtual int vf118(int a, int b);
    virtual int vf11C(int a, int b);
    virtual int vf120(int a, int b);
    virtual int vf124(int a, int b);
    virtual int vf128(int a, int b);
    virtual int vf12C(int a, int b);
    virtual int vf130(int a, int b);
    virtual int vf134(int a, int b);
    virtual int vf138(int a, int b);
    virtual int vf13C(int a, int b);
    virtual int vf140(int a, int b);
    virtual int vf144(int a, int b);
    virtual int vf148(int a, int b);
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
    virtual int vf1A4();
    virtual int vf1A8();
    virtual int vf1AC();
    virtual int vf1B0(int a);
    virtual void vf1B4();
    virtual void vf1B8();
    virtual void vf1BC();
    virtual void vf1C0();
    virtual void vf1C4();
    virtual void vf1C8();
    virtual int vf1CC();
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
    virtual void vf248();
    virtual void vf24C();
    virtual void vf250(f32 a);
    virtual void vf254();
    virtual void vf258(f32 a);
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
    virtual const char* vf2C0();
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

    /* 0x004 */ u8 mPad004[0x19 - 0x4];
    /* 0x019 */ u8 m19;
    /* 0x01A */ u8 mPad01A[0x68 - 0x1A];
    /* 0x068 */ SubObj mSub;
    /* 0x08C */ u8 mPad08C[0x5F0 - 0x8C];
    /* 0x5F0 */ s32 m5F0;
    /* 0x5F4 */ u8 m5F4;
    /* 0x5F5 */ u8 mPad5F5[0x610 - 0x5F5];
    /* 0x610 */ f32 mAngle;
    /* 0x614 */ u8 mPad614[0x620 - 0x614];
    /* 0x620 */ f32 mPosX;
    /* 0x624 */ u8 mPad624[4];
    /* 0x628 */ f32 mPosZ;
    /* 0x62C */ u8 mPad62C[4];
    /* 0x630 */ f32 m630;
    /* 0x634 */ u8 mPad634[0x650 - 0x634];
    /* 0x650 */ Vec mVec;
    /* 0x65C */ u8 mPad65C[0x66C - 0x65C];
    /* 0x66C */ f32 m66C;
    /* 0x670 */ f32 m670;
    /* 0x674 */ u8 mPad674[0x69C - 0x674];
    /* 0x69C */ f32 m69C;
    /* 0x6A0 */ u8 mPad6A0[4];
    /* 0x6A4 */ u8 m6A4;
    /* 0x6A5 */ u8 mPad6A5[0x6C4 - 0x6A5];
    /* 0x6C4 */ u8 m6C4;
    /* 0x6C5 */ u8 mPad6C5[3];
    /* 0x6C8 */ Entry mEntries[15];
    /* 0x740 */ u8 mPad740[4];
    /* 0x744 */ f32 m744;
    /* 0x748 */ f32 m748;
    /* 0x74C */ u8 mPad74C[4];
    /* 0x750 */ f32 m750;
    /* 0x754 */ u8 mPad754[4];
    /* 0x758 */ f32 m758;
    /* 0x75C */ u8 mPad75C[4];
    /* 0x760 */ u8 m760;
    /* 0x761 */ u8 m761;
    /* 0x762 */ u8 mPad762[3];
    /* 0x765 */ u8 m765;
    /* 0x766 */ u8 mPad766[9];
    /* 0x76F */ u8 m76F;

    int fn_18_397F0(int doStart);
    int fn_18_399D0(int doStart);
    void fn_18_39BB0(f32 angle, const Vec* target, int flip);
    int fn_18_39E00();
    int fn_18_39ED0();
    int fn_18_39FB0();
    int fn_18_3A090(int mode);
    int fn_18_3A180(u32 flags);
    int fn_18_3A190(u32 flags);
    int fn_18_3A390(u32 flags);
};

int fn_18_3E4E0(Actor* a, int n);
void fn_18_36170(Actor* a, f32 f1, f32 f2, int a1, int a2, int a3);
void fn_18_3E330(Actor* a, int n);
u32 fn_18_3DEF0(Actor* a);
u32 fn_18_3DD50(Actor* a);
Vec fn_18_41680(const Vec& v, f32 s);

int Actor::fn_18_397F0(int doStart) {
    /* Dead stores: fix the order of the translation unit's float constant pool. */
    f32 poolOrder = 0.0f;
    poolOrder = 0.05f;
    poolOrder = 1.5f;
    poolOrder = 0.2f;
    poolOrder = 0.1f;
    poolOrder = 0.16666667f;
    poolOrder = 1.1f;
    poolOrder = 1.0f;
    poolOrder = 0.3f;
    poolOrder = 0.5f;
    poolOrder = 10.0f;
    poolOrder = 0.08f;
    poolOrder = 0.25f;
    poolOrder = 0.6666667f;
    f64 poolOrderD = 360.0;
    poolOrder = 180.0f;
    poolOrder = 360.0f;
    poolOrder = 1.40625f;
    poolOrder = 90.0f;
    poolOrder = -90.0f;
    poolOrder = 2.0f;
    int result = 4;
    if (doStart) {
        Entry* e = &mEntries[fn_18_3E4E0(this, 11)];
        if (!e->mFlagA) {
            result = 0;
        } else if (e->mFlagB) {
            result = 1;
        } else if (m5F0 == 11) {
            result = 2;
        } else {
            m5F0 = 11;
            m758 = 0.0f;
            m5F4 = 1;
            result = 5;
        }
        if (result != 5) {
            return result;
        }
        m760 = 1;
        fn_18_36170(this, 0.0f, 0.05f, 13, 0, 1);
        m744 = 0.6666667f;
        m748 = 0.6666667f;
        vf250(0.0f);
        vf258(0.0f);
        m761 = 1;
        vf31C(28);
    }
    if (m744 <= 0.0f) {
        Vec zero(0.0f, 0.0f, 0.0f);
        mVec = zero;
        if (fn_18_3CCA0(&mSub)) {
            m750 = m69C;
            return vf118(1, 0);
        }
    }
    if (!vf2B4()) {
        vf1E0();
    }
    return result;
}

int Actor::fn_18_399D0(int doStart) {
    int result = 4;
    if (doStart) {
        Entry* e = &mEntries[fn_18_3E4E0(this, 12)];
        if (!e->mFlagA) {
            result = 0;
        } else if (e->mFlagB) {
            result = 1;
        } else if (m5F0 == 12) {
            result = 2;
        } else {
            m5F0 = 12;
            m758 = 0.0f;
            m5F4 = 1;
            result = 5;
        }
        if (result != 5) {
            return result;
        }
        m760 = 1;
        fn_18_36170(this, 0.0f, 0.05f, 14, 0, 1);
        m744 = 0.6666667f;
        m748 = 0.6666667f;
        vf250(0.0f);
        vf258(0.0f);
        m761 = 1;
        vf31C(29);
    }
    if (m744 <= 0.0f) {
        Vec zero(0.0f, 0.0f, 0.0f);
        mVec = zero;
        if (fn_18_3CCA0(&mSub)) {
            m750 = m69C;
            return vf118(1, 0);
        }
    }
    if (!vf2B4()) {
        vf1E0();
    }
    return result;
}

void Actor::fn_18_39BB0(f32 angle, const Vec* target, int flip) {
    Vec d(mPosX - target->x, 0.0f, mPosZ - target->z);
    f32 diff;
    if (fn_80192D20(&d) > 0.1f) {
        Vec n;
        fn_80192CD0(&d, &n);
        f32 dz = d.z;
        f32 dx = d.x;
        angle = 1.40625f * fn_8016D6E0(dx, dz);
        diff = (f32)fmod(angle - mAngle, 360.0);
        if (diff > 180.0f) {
            diff -= 360.0f;
        }
    } else {
        diff = (f32)fmod(angle - mAngle, 360.0);
        if (diff > 180.0f) {
            diff -= 360.0f;
        }
    }
    if (diff < 90.0f && diff > -90.0f) {
        mAngle = angle;
        if (flip) {
            vf148(1, 0);
        } else {
            vf140(1, 0);
        }
    } else {
        f32 back = angle + 180.0f;
        mAngle = back;
        if (back >= 180.0f) {
            mAngle = back - 360.0f;
        }
        if (flip) {
            vf144(1, 0);
        } else {
            vf13C(1, 0);
        }
    }
    fn_18_3E330(this, 1);
    f32 speed = m66C;
    if (flip) {
        speed = 2.0f * m670 / 0.6666667f;
    } else {
        speed *= 0.25f;
    }
    Vec out = fn_18_41680(Vec(0.0f, angle, 0.0f), speed);
    mVec = out;
}

int Actor::fn_18_39E00() {
    int result = 3;
    if (vf1CC()) {
        return 3;
    }
    u32 flags = fn_18_3DEF0(this);
    if (flags & 0x200) {
        result = vf130(1, 0);
        if (result > 3) {
            return result;
        }
    }
    if (flags & 0x100) {
        return vf124(1, 0);
    }
    return result;
}

int Actor::fn_18_39ED0() {
    if (vf1CC()) {
        return 3;
    }
    int c;
    if (m19 && m6A4) {
        c = m6C4;
    } else {
        c = (fn_18_3DD50(this) & 0xF) != 0;
    }
    if (c) {
        int r = vf120(1, 0);
        if (r > 3) {
            return r;
        }
        return vf11C(1, 0);
    }
    return 3;
}

int Actor::fn_18_39FB0() {
    int result = 3;
    if (vf1CC()) {
        return 3;
    }
    u32 flags = fn_18_3DEF0(this);
    if (flags & 0x200) {
        result = vf134(1, 0);
        if (result > 3) {
            return result;
        }
    }
    if (!m765 && (flags & 0x100)) {
        return vf138(1, 0);
    }
    return result;
}

int Actor::fn_18_3A090(int mode) {
    int result;
    if (vf1CC()) {
        return 3;
    }
    result = vf1A8();
    if (result > 3) {
        return result;
    }
    result = vf1AC();
    if (result > 3) {
        return result;
    }
    switch (mode) {
    case 0:
        return vf118(1, 0);
    case 5:
        return vf12C(1, 0);
    }
    return result;
}

int Actor::fn_18_3A180(u32 flags) {
    if (flags & 1) {
        return fn_18_3A190(flags);
    }
    return fn_18_3A390(flags);
}

int Actor::fn_18_3A190(u32 flags) {
    int result = 3;
    bool expired = m630 <= 0.0f;
    int grounded;
    if (expired) {
        vf2F8(0);
        if (vf2B4()) {
            mSub.SetName(vf2C0(), lbl_46BC);
            m76F = 0;
            f32 limit = 10.0f * lbl_46B8;
            if (m630 < limit) {
                return vf1B0(5);
            }
            return vf1B0(0);
        }
    }
    if (flags & 2) {
        grounded = fn_18_3CCA0(&mSub);
    } else {
        grounded = 0;
    }
    if ((flags & 4) && !grounded) {
        return 3;
    }
    if (flags & 8) {
        result = vf1A4();
        if (result > 3) {
            vf2F8(0);
            return result;
        }
    }
    if (expired && (flags & 0x40)) {
        return vf128(1, 0);
    }
    if (grounded) {
        return vf128(1, 0);
    }
    return result;
}

int Actor::fn_18_3A390(u32 flags) {
    int hasGround;
    int result = 3;
    int ground;
    if (!vf2B4()) {
        result = vf128(1, 0);
        if (result > 3) {
            return result;
        }
    }
    hasGround = flags & 2;
    if (hasGround) {
        ground = fn_18_3CCA0(&mSub);
    } else {
        ground = 0;
    }
    if ((flags & 4) && !ground) {
        return result;
    }
    if (flags & 8) {
        result = vf1A8();
        if (result > 3) {
            return result;
        }
    }
    if (flags & 0x10) {
        result = vf1AC();
        if (result > 3) {
            return result;
        }
    }
    if (!hasGround) {
        int c;
        if (m19 && m6A4) {
            c = m6C4;
        } else {
            c = (fn_18_3DD50(this) & 0xF) != 0;
        }
        if (!c) {
            ground = 1;
        }
    }
    if (ground) {
        return vf118(1, 0);
    }
    return result;
}
