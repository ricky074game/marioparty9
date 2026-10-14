/* Shared by most REL modules. Reference copy: mg9101 (fn_18_37FF0). */
#include "game/common/unk_37FF0.h"

namespace unk_37FF0 {

Vec fn_18_41680(const Vec& axis, f32 speed);
extern "C" f32 fn_80192D20(const Vec* v);               /* PSVECMag */
extern "C" void fn_80192CD0(const Vec* src, Vec* dst);  /* PSVECNormalize */
f32 lbl_18_data_4620[5] = {6.0f, 3.0f, 6.0f, 40.0f, -9.8f};

/* Not referenced by the linked module; keeps the literal pool in the original order. */
__declspec(weak) void PoolOrder(f32* p) {
    p[0] = 0.0f;
    p[1] = -1.0f;
    p[2] = -0.8f;
    p[3] = 0.8f;
    p[4] = 0.01f;
    p[5] = -180.0f;
    p[6] = 180.0f;
    p[7] = 1.0f;
    p[8] = 0.5f;
    p[9] = 10.0f;
    p[10] = 5.0f;
    p[11] = 4.0f;
}

static inline void VecScale(register const Vec* src, register Vec* dst, register f32 scale) {
    register f32 r, v;
    asm {
        psq_l v, 0(src), 0, 0
        ps_muls0 r, v, scale
        psq_st r, 0(dst), 0, 0
        psq_l v, 8(src), 1, 0
        ps_muls0 r, v, scale
        psq_st r, 8(dst), 1, 0
    }
}

static inline void VecAdd(register const Vec* a, register const Vec* b, register Vec* dst) {
    register f32 va, vb, r, r2;
    asm {
        psq_l va, 0(a), 0, 0
        psq_l vb, 0(b), 0, 0
        ps_add r, va, vb
        psq_st r, 0(dst), 0, 0
        psq_l va, 8(a), 1, 0
        psq_l vb, 8(b), 1, 0
        ps_add r2, va, vb
        psq_st r2, 8(dst), 1, 0
    }
}

static inline void VecAddR(register const Vec* a, register const Vec* b, register Vec* dst) {
    register f32 va, vb, r;
    asm {
        psq_l va, 0(a), 0, 0
        psq_l vb, 0(b), 0, 0
        ps_add r, va, vb
        psq_st r, 0(dst), 0, 0
        psq_l va, 8(a), 1, 0
        psq_l vb, 8(b), 1, 0
        ps_add r, va, vb
        psq_st r, 8(dst), 1, 0
    }
}

static inline void VecSub(register const Vec* a, register const Vec* b, register Vec* dst) {
    register f32 va, vb, r;
    asm {
        psq_l va, 0(a), 0, 0
        psq_l vb, 0(b), 0, 0
        ps_sub r, va, vb
        psq_st r, 0(dst), 0, 0
        psq_l va, 8(a), 1, 0
        psq_l vb, 8(b), 1, 0
        ps_sub r, va, vb
        psq_st r, 8(dst), 1, 0
    }
}

/* Pushes two actors apart when their capsules overlap. */
int Actor::OverlapTop(Capsule* a, Capsule* b, Actor* other) {
    Vec d;
    f32 sum = a->r2 + b->r2;
    VecSub(&a->p0, &b->p1, &d);
    f32 dist = fn_80192D20(&d);
    if (dist > 0.01f && dist < sum) {
        if (!vf2B4()) {
            Vec n;
            fn_80192CD0(&d, &n);
            Vec mid;
            Vec tmp;
            VecAdd(&a->p0, &b->p1, &tmp);
            VecScale(&tmp, &mid, 0.5f);
            vel.x = n.x;
            vel.y = n.y;
            vel.z = n.z;
            VecScale(&vel, &vel, 0.5f * sum);
            VecAddR(&vel, &mid, &vel);
            for (int i = 0; i < 1; i++) {
                if (i == 0) {
                    f32 dy = ext[0] * scale;
                    vel.y = vel.y - dy;
                } else {
                    f32 dy = ext[1] * scale;
                    vel.y = vel.y - dy;
                }
            }
            other->vel.x = n.x;
            other->vel.y = n.y;
            other->vel.z = n.z;
            VecScale(&other->vel, &other->vel, 0.5f * -sum);
            VecAddR(&other->vel, &mid, &other->vel);
            for (int i = 1; i < 2; i++) {
                if (i == 0) {
                    f32 dy = other->ext[0] * other->scale;
                    other->vel.y = other->vel.y - dy;
                } else {
                    f32 dy = other->ext[1] * other->scale;
                    other->vel.y = other->vel.y - dy;
                }
            }
            fn_18_3E330(0);
            other->fn_18_3E330(0);
            if (m744 > 0.0f) {
                m744 = 0.0f;
                m750 = m69C;
            }
            vf128(1, 0);
            if (m630 > 0.0f) {
                m630 = 0.0f;
            }
            if (other->m744 > 0.0f) {
                other->m744 = 0.0f;
                other->m750 = other->m69C;
            }
            if (other->vf124(1, 1) == 5) {
                other->vf31C(4);
                other->m774++;
            }
        }
        return 1;
    }
    return 0;
}

int Actor::GetM774() { return m774; }

void Actor::SetM764(u8 v) { m764 = v; }

u8 Actor::GetM764() { return m764; }

static f32 sTable[7];

static void InitTable() {
    f32* d = lbl_18_data_4620;
    f32* t = sTable;
    f32 g = 10.0f * d[4];
    t[0] = 10.0f * d[0];
    t[1] = 10.0f * d[1];
    t[2] = 10.0f * d[2];
    t[3] = 10.0f * d[3];
    t[4] = g;
    t[5] = 5.0f * g;
    t[6] = 4.0f * -g;
}

__declspec(section ".ctors") static void (*const sInitTable)() = InitTable;

void Actor::StartIdleMotion() {
    if (!m6C5) {
        if (HasMotion(1)) {
            SetMotion(0.0f, 0.05f, 1, 1, 0);
            return;
        }
        if (HasMotion(2)) {
            SetMotion(0.0f, 0.05f, 2, 1, 0);
            return;
        }
    }
    SetMotion(0.0f, 0.05f, 0, 1, 0);
}

int Actor::State0(int init) {
    int r;
    if (init) {
        m760 = 0;
        r = BeginState(0);
        if (r != kEnterFailed) {
            return r;
        }
        vf248(0.0f, 0.0f, 0.0f);
        vf1DC();
    } else {
        vf1DC();
        return vf1A0(0x18, 0);
    }
    return r;
}

int Actor::State1(int init) {
    int r;
    if (init) {
        r = BeginState(1);
        if (r != kEnterFailed) {
            return r;
        }
    } else {
        r = vf1A0(0x28, 0);
        if (r > 3) {
            return r;
        }
    }
    vf1D0(m668, 1);
    return r;
}

int Actor::State2(int init) {
    int r;
    if (init) {
        r = BeginState(2);
        if (r != kEnterFailed) {
            return r;
        }
    } else {
        r = vf1A0(0x28, 0);
        if (r > 3) {
            return r;
        }
    }
    vf1D0(m664, 2);
    return r;
}

int Actor::State3(int init, int variant) {
    int r;
    if (init) {
        r = BeginState(3);
        if (r != kEnterFailed) {
            return r;
        }
        vf1E4(3);
        if (!variant) {
            m630 = m674;
            mStep = 0;
            vf31C(3);
            vf31C(0x16);
        } else {
            mStep = 1;
            m76F = 0;
            Vec out = fn_18_41680(VecInit(0.0f, m610, 0.0f), m668);
            m62C = out.x;
            m630 = out.y;
            m634 = out.z;
            m630 = 1.5f * m674;
        }
    } else {
        r = vf1A0(0x49, 0);
        if (r > 3) {
            return r;
        }
        if (mStep == 0) {
            if (vf21C()) {
                if (fn_18_3DD50() & 0x100) {
                    m630 = m674;
                }
            }
        }
    }
    vf1E0();
    return r;
}

int Actor::IsTimerShort() { return mTimer < 0.2f; }

int Actor::State4(int init) {
    int r;
    if (init) {
        r = BeginState(4);
        if (r != kEnterFailed) {
            return r;
        }
        m76F = 0;
        vf1E4(4);
    } else {
        r = vf1A0(9, 0);
        if (r > 3) {
            return r;
        }
    }
    vf1E0();
    return r;
}

int Actor::State5(int init) {
    int r;
    if (init) {
        r = BeginState(5);
        if (r != kEnterFailed) {
            return r;
        }
        vf31C(2);
        vf248(0.0f, 0.0f, 0.0f);
        vf1E4(5);
    } else {
        return vf1A0(0x1A, 0);
    }
    return r;
}

void Actor::SetMotionId(int id) {
    SetMotion(0.0f, 0.1f, id, 0, 1);
}

} // namespace unk_37FF0
