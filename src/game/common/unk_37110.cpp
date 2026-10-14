/* Shared by most REL modules. Reference copy: mg9101 (fn_18_37110). */
#include "game/common/unk_37110.h"

namespace unk_37110 {

struct Entry {
    u8 pad00[0x1C];
    u32 f1C;
    s32 f20;
    u8 pad24[4];
    s32 f28;
    u8 pad2C[0x44 - 0x2C];
};

extern Actor** gActorMgr;
Actor* fn_18_3C040(Actor** mgr, u32 idx); /* mgr->actors[idx] */
void fn_18_3E330(Actor* a, int flag);
void fn_18_41680(Vec* out, Vec* rot, f32 len);
void fn_801928D0(Mtx34* m, Vec* src, Vec* dst);
f32 fn_80192D20(Vec* v);
void fn_80192CD0(Vec* src, Vec* dst);
f32 fn_80070A80(f32 lo, f32 hi);


/* Paired-single vector helpers (inlined asm, as in the SDK). */
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
    register f32 r, vb, va;
    asm {
        psq_l va, 0(a), 0, 0
        psq_l vb, 0(b), 0, 0
        ps_add r, va, vb
        psq_l va, 8(a), 1, 0
        psq_l vb, 8(b), 1, 0
        psq_st r, 0(dst), 0, 0
        ps_add r, va, vb
        psq_st r, 8(dst), 1, 0
    }
}

static inline void VecSub(register const Vec* a, register const Vec* b, register Vec* dst) {
    register f32 r, vb, va;
    asm {
        psq_l va, 0(a), 0, 0
        psq_l vb, 0(b), 0, 0
        ps_sub r, va, vb
        psq_l va, 8(a), 1, 0
        psq_l vb, 8(b), 1, 0
        psq_st r, 0(dst), 0, 0
        ps_sub r, va, vb
        psq_st r, 8(dst), 1, 0
    }
}

u8 Actor::getFlag7C0() {
    return m7C0;
}

s32 Actor::get7C4() {
    return m7C4;
}

void* Actor::getObj7D8() {
    if (m7D4) {
        return 0;
    }
    return obj7D8;
}

void* Actor::getObj7C8() {
    return obj7C8;
}

s32 Actor::get7B8() {
    return m7B8;
}

void Actor::clearFlag7C0() {
    m7C0 = 0;
    m7B1 = 1;
}

void Actor::set848(f32 a, f32 b) {
    m848 = a;
    m84C = b;
}

void Actor::set8D4(f32 w, const Vec* v) {
    m8D4 = *v;
    m8E0 = w;
}

void Actor::setTarget(f32 f, const Vec* p, u8 flag) {
    Mtx34 m = *node.getMtx();
    Vec out;
    fn_801928D0(&m, (Vec*)p, &out);
    m96C = 0;
    target = out;
    m954 = f;
    m978 = flag;
    m998 = 1;
}

void Actor::setExtents(f32 a, f32 b, f32 c, s32 d) {
    ext0 = a;
    ext1 = b;
    ext2 = c;
    extId = d;
}

void Actor::getCapsuleImpl(Capsule* out) {
    f32 z = vel.z;
    f32 x = vel.x;
    f32 y = vel.y;
    f32 c = ext2;
    s32 id = extId;
    f32 b = ext1 * scale;
    f32 a = ext0 * scale;
    out->r0 = a;
    out->r1 = b;
    out->r2 = c;
    out->id = id;
    out->p0.x = x;
    out->p0.z = z;
    out->p1.x = x;
    out->p1.z = z;
    out->p0.y = y + a;
    out->p1.y = y + b;
}

void Actor::snapshot() {
    Vec a = vel;
    Vec b = vel;
    Vec c = vel;
    f32 ha = ext0 * scale;
    f32 hb = ext1 * scale;
    a.y = a.y + ha;
    b.y = b.y + hb;
    m938 = (m763 == 0);
    m90C = 1;
    snapP0.x = a.x;
    snapP0.y = a.y;
    snapP0.z = a.z;
    snapP1.x = b.x;
    snapP1.y = b.y;
    snapP1.z = b.z;
    m900 = ext2;
    m910 = extId;
}

void Actor::resolveAll() {
    vUpdate();
    if (m930 != 0) {
        Capsule capA;
        Capsule capB;
        Entry* e;
        vGetCapsule(&capA);
        s32 minId = m910;
        if (vIsBusy() == 0) {
            for (e = (Entry*)m92C; e != (Entry*)m92C + m930; e++) {
                s32 eid = e->f28;
                if (e->f1C <= 7 && e->f20 == 0 && eid >= minId) {
                    Actor* o = fn_18_3C040(gActorMgr, e->f1C);
                    o->vGetCapsule(&capB);
                    if (vIsSolid() && o->vIsSolid() && vCheckSide(&capA, &capB, o)) {
                        vGetCapsule(&capA);
                        o->vGetCapsule(&capB);
                    } else if (capA.p1.y > capB.p0.y) {
                        if (vCheckTop(&capA, &capB, o)) {
                            vGetCapsule(&capA);
                            o->vGetCapsule(&capB);
                        }
                    } else if (capA.p0.y < capB.p1.y) {
                        if (vCheckBottom(&capA, &capB, o)) {
                            vGetCapsule(&capA);
                            o->vGetCapsule(&capB);
                        }
                    } else {
                        if (vCheckSide(&capA, &capB, o)) {
                            vGetCapsule(&capA);
                            o->vGetCapsule(&capB);
                        }
                    }
                }
            }
        }
    }
}

void Actor::resolveEnter() {
    if (m990 != 0) {
        Entry* e;
        s32 type = m978;
        Vec pos = *vGetPos();
        for (e = (Entry*)m98C; e != (Entry*)m98C + m990; e++) {
            u32 idx = e->f1C;
            if (idx <= 7 && e->f20 == 0) {
                Actor* o = fn_18_3C040(gActorMgr, idx);
                if (type == 2) {
                    Vec opos = *o->vGetPos();
                    if (o->vIsGrounded() == 0 && o->vIsSolid()) {
                        if (opos.y < pos.y) {
                            fn_18_3E330(o, 1);
                            o->vSetState(3, 0);
                            Vec t;
                            t.x = target.x;
                            t.y = target.y;
                            t.z = target.z;
                            node.nSetPos(&t, "actor_damage");
                            vSetFlag(1);
                            fn_18_3E330(this, 1);
                            o->vSetAction(7);
                            o->vSetAction(0x19);
                        }
                    } else if (opos.y < pos.y) {
                        fn_18_3E330(this, 0);
                        if (m744 > 0.0f) {
                            m744 = 0.0f;
                            m750 = m69C;
                        }
                        if (vHit124(1, 1) == 5) {
                            vSetAction(4);
                            m774++;
                        }
                    }
                } else {
                    if (o->vIsGrounded() == 0 && (u32)type <= 1) {
                        o->node.nStop();
                        o->vSetAction(0xC);
                        fn_18_3E330(this, 0);
                        fn_18_3E330(o, 0);
                        Vec t;
                        t.x = target.x;
                        t.y = target.y;
                        t.z = target.z;
                        o->vSetEffect(&t, type == 0, m610);
                    }
                }
            }
        }
    }
}

/* Weak (folded at link time) helper; only here to keep the literal pool order of the original unit. */
__declspec(weak) f32 poolOrder(f32 x) {
    if (x < -1.0f) {
        return -0.8f;
    }
    return 0.8f;
}

static inline void randomDir(Vec* out) {
    Vec rot;
    f32 yaw = fn_80070A80(-180.0f, 180.0f);
    rot.x = 0.0f;
    rot.y = yaw;
    rot.z = 0.0f;
    fn_18_41680(out, &rot, 1.0f);
}

int Actor::overlapA(Capsule* a, Capsule* b, Actor* other) {
    Vec d;
    Vec n;
    Vec dir;
    f32 dz = a->p0.z - b->p0.z;
    f32 dx = a->p0.x - b->p0.x;
    f32 sum = a->r2 + b->r2;
    d.x = dx;
    d.y = 0.0f;
    d.z = dz;
    f32 dist = fn_80192D20(&d);
    if (dist < sum) {
        if (dist > 0.01f) {
            fn_80192CD0(&d, &n);
        } else {
            randomDir(&dir);
            fn_80192CD0(&dir, &n);
        }
        if (a->id == b->id) {
            f32 s = 0.5f * (a->r2 + b->r2);
            Vec mid;
            mid.x = 0.5f * (a->p0.x + b->p0.x);
            mid.y = 0.0f;
            mid.z = 0.5f * (a->p0.z + b->p0.z);
            VecScale(&n, &n, s);
            vel.x = mid.x + n.x;
            vel.z = mid.z + n.z;
            Vec move = *other->vGetPos();
            move.x = mid.x - n.x;
            move.z = mid.z - n.z;
            other->vSetMove(&move);
        } else {
            f32 s = a->r2 + b->r2;
            VecScale(&n, &n, s);
            vel.x = b->p0.x + n.x;
            vel.z = b->p0.z + n.z;
        }
        return 1;
    }
    return 0;
}

int Actor::overlapB(Capsule* a, Capsule* b, Actor* other) {
    Vec d;
    f32 sum = a->r2 + b->r2;
    VecSub(&a->p1, &b->p0, &d);
    f32 dist = fn_80192D20(&d);
    if (dist > 0.01f && dist < sum) {
        Vec n;
        fn_80192CD0(&d, &n);
        if (other->vIsSolid()) {
            vel.x = n.x;
            vel.y = n.y;
            vel.z = n.z;
            VecScale(&vel, &vel, sum);
            VecAdd(&vel, &b->p0, &vel);
            f32 dy = ext1 * scale;
            vel.y = vel.y - dy;
            if (vGetState() == 2) {
                fn_18_3E330(this, 0);
                other->vSetState(2, 0);
                if (m744 > 0.0f) {
                    m744 = 0.0f;
                    m750 = m69C;
                }
                if (vHit124(1, 1) == 5) {
                    vSetAction(4);
                    m774++;
                }
            }
        } else {
            Vec mid;
            Vec tmp;
            vel.x = n.x;
            vel.y = n.y;
            vel.z = n.z;
            VecAdd(&a->p1, &b->p0, &tmp);
            VecScale(&tmp, &mid, 0.5f);
            VecScale(&vel, &vel, 0.5f * sum);
            VecAdd(&vel, &mid, &vel);
            f32 dy = ext1 * scale;
            vel.y = vel.y - dy;
            other->vel.x = n.x;
            other->vel.y = n.y;
            other->vel.z = n.z;
            VecScale(&other->vel, &other->vel, -0.5f * sum);
            VecAdd(&other->vel, &mid, &other->vel);
            f32 oy = other->ext0 * other->scale;
            other->vel.y = other->vel.y - oy;
            fn_18_3E330(this, 0);
            fn_18_3E330(other, 0);
            if (m744 > 0.0f) {
                m744 = 0.0f;
                m750 = m69C;
            }
            if (vHit124(1, 1) == 5) {
                vSetAction(4);
                m774++;
            }
            if (other->m744 > 0.0f) {
                other->m744 = 0.0f;
                other->m750 = other->m69C;
            }
            other->vHit128(1, 0);
            if (other->m630 > 0.0f) {
                other->m630 = 0.0f;
            }
        }
        return 1;
    }
    return 0;
}

} // namespace unk_37110
