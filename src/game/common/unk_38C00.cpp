/* Shared by most REL modules. Reference copy: mg9101 (fn_18_38C00). */
#include "game/common/unk_38C00.h"

/* Translation-unit data: tuning values read by the state functions. */
f32 gTuneHead[5] = {6.0f, 3.0f, 6.0f, 40.0f, -9.8f};
f32 gTuneA = 0.75f;
f32 gTuneB = 0.75f;
f32 gTuneC = 0.6f;
f32 gTuneD = 0.6f;
f32 gTuneE = -0.2f;
f32 gTuneF = 1.0f;

extern f32 gTimeScale; /* lbl_18_bss_4B64 */
extern f32 lbl_8026E3B4;

static inline f32 AddF(f32 a, f32 b) { return a + b; }

int Actor::State7(int init) {
    /* Dead stores: they only fix the order of the translation unit's float
       constant pool, which the original got from earlier functions. */
    f32 poolOrder = 0.0f;
    poolOrder = 0.05f;
    poolOrder = 1.5f;
    poolOrder = 0.2f;
    poolOrder = 0.1f;

    int r;
    if (init) {
        r = BeginState(7);
        if (r != kEnterFailed) {
            return r;
        }
        vf248(0.0f, 0.0f, 0.0f);
        SetMotion(0.0f, 0.05f, 9, 0, 1);
        mTimer = 0.0f;
        vf31C(10);
        vf31C(0x1E);
    } else {
        r = vf1A0(0x1E, 0);
        if (r > 3) {
            return r;
        }
    }
    f32 t = mTimer / 0.16666667f;
    if (t < 1.1f) {
        if (t > 1.0f) {
            t = 1.0f;
        }
        f32 poolOrder2 = 0.3f;
        f32 k = 0.5f * t;
        f32 z = 10.0f * (0.3f + k);
        Vec3 pos(0.0f, 10.0f * gTuneA, z);
        vf2DC(pos, 1, z);
    }
    return r;
}

int Actor::State8(int init) {
    int r;
    if (init) {
        r = BeginState(8);
        if (r != kEnterFailed) {
            return r;
        }
        SetMotion(0.0f, 0.05f, 10, 0, 1);
        vf31C(11);
        vf31C(0x1F);
    } else {
        r = vf1A0(0xF, 0);
        if (r > 3) {
            return r;
        }
    }
    static Vec3 sPos(0.0f, 10.0f * gTuneB, 10.0f * gTuneC);
    f32 sc = 10.0f * gTuneD;
    vf2DC(sPos, 0, sc);
    vf1E0();
    return r;
}

int Actor::State6(int init) {
    int r = 4;
    if (init) {
        r = BeginState(6);
        if (r != kEnterFailed) {
            return r;
        }
        SetMotion(0.0f, 0.05f, 6, 0, 1);
        vf248(0.0f, 0.0f, 0.0f);
        m765 = 1;
        vf2F8(0);
        vf31C(5);
        vf31C(0x17);
        mStep = 0;
    }
    switch (mStep) {
    case 0:
        m5FC = 1;
        vf270();
        mStep++;
        break;
    case 1:
        m5FC = 1;
        vf270();
        if (mAnim.IsDone()) {
            mStep++;
            mTimer = 0.0f;
        }
        break;
    case 2:
        m5FC = 1;
        vf270();
        if (mTimer > 0.08f) {
            mStep++;
        }
        break;
    case 3:
        if (vf2B4()) {
            m766 = 1;
            SetMotion(0.0f, 0.05f, 7, 0, 1);
            mAnim.PlayNamed(vf2C0(), "Hit");
            fn_18_3E330(1);
            mTimer = 0.0f;
            vf31C(6);
            mStep++;
        } else {
            vf270();
            vf248(0.0f, 0.5f * gTimeScale, 0.0f);
            m5FC = 3;
            static Vec3 sPos(0.0f, AddF(0.5f * gTimeScale * lbl_8026E3B4, 10.0f * gTuneE), 0.0f);
            f32 sc = 10.0f * gTuneF;
            vf2DC(sPos, 2, sc);
        }
        break;
    case 4:
        if (mTimer > 0.25f) {
            SetMotion(0.0f, 0.05f, 8, 0, 1);
            mStep++;
        }
        break;
    case 5:
        if (mAnim.IsDone()) {
            return vf118(1, 0);
        }
        break;
    }
    return r;
}

int Actor::State9(int init) {
    int r = 4;
    if (init) {
        r = BeginState(9);
        if (r != kEnterFailed) {
            return r;
        }
        m760 = 1;
        SetMotion(0.0f, 0.05f, 11, 0, 1);
        m744 = 0.5f;
        m748 = 0.5f;
        vf250(0.0f);
        vf258(0.0f);
        m761 = 0;
        vf31C(0x1A);
    }
    if (m744 <= 0.0f) {
        mVel = Vec3(0.0f, 0.0f, 0.0f);
        if (mAnim.IsDone()) {
            m750 = m69C;
            return vf118(1, 0);
        }
    }
    if (!vf2B4()) {
        vf1E0();
    }
    return r;
}

int Actor::State10(int init) {
    int r = 4;
    if (init) {
        r = BeginState(10);
        if (r != kEnterFailed) {
            return r;
        }
        m760 = 1;
        SetMotion(0.0f, 0.05f, 12, 0, 1);
        m744 = 0.5f;
        m748 = 0.5f;
        vf250(0.0f);
        vf258(0.0f);
        m761 = 0;
        vf31C(0x1B);
    }
    if (m744 <= 0.0f) {
        mVel = Vec3(0.0f, 0.0f, 0.0f);
        if (mAnim.IsDone()) {
            m750 = m69C;
            return vf118(1, 0);
        }
    }
    if (!vf2B4()) {
        vf1E0();
    }
    return r;
}
