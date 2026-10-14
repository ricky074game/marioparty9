/* Shared by most REL modules. Reference copy: mg9101 (fn_18_364A0). */
#include "game/common/unk_364A0.h"
#include "game/common/unk_21650.h"

inline void* operator new(unsigned long, void* p) {
    return p;
}

namespace unk_364A0 {

/* Weak helper, only here to keep the literal pool order of the original unit. */
__declspec(weak) void PoolOrder(f32* o) {
    o[0] = 0.0f;
    o[1] = -1.0f;
    o[2] = -0.8f;
    o[3] = 0.8f;
}

void __dt__5ActorFv(void* self, int flag);   /* Actor::~Actor */
void fn_8016D940(Vec* out, const Vec* a, const Vec* b);  /* component-wise min */
void fn_8016D980(Vec* out, const Vec* a, const Vec* b);  /* component-wise max */
f32 fn_80192D20(Vec* v);                                  /* length */
void fn_80192CD0(Vec* src, Vec* dst);                    /* normalize */
Vec* fn_80060100(void* node);                             /* node position */

/* Adjusting thunks for the secondary bases of Actor (+0x68 and +0x238) */
void Thunk364A0(char* self, int flag) {
    __dt__5ActorFv(self - 0x68, flag);
}

void Thunk364B0(char* self, int flag) {
    __dt__5ActorFv(self - 0x238, flag);
}

struct Mgr;
extern Mgr* lbl_18_bss_4AE0;
void fn_18_3BD50(Mgr*, u32);

class Root {
public:
    virtual ~Root();
    u32 m4;
};

/* Primary base of Actor */
class Base0 : public Root {
public:
    virtual ~Base0();
    virtual void vf0C();
};

Base0::~Base0() {
    if (lbl_18_bss_4AE0) {
        fn_18_3BD50(lbl_18_bss_4AE0, m4);
    }
}

void Base0::vf0C() {}

/* Tuning constants (data_45F8) scaled into a derived table at load time. */
struct Params {
    f32 a, b, c, d, g, h;
};

extern Params lbl_18_data_45F8; /* {6, 3, 6, 40, -9.8, 0} */

struct Table {
    f32 v0, v1, v2, v3, v4, v5, v6;
    void init(const Params& p) {
        f32 k = 10.0f;
        v0 = k * p.a;
        v1 = k * p.b;
        v2 = k * p.c;
        v3 = k * p.d;
        v4 = k * p.g;
        v5 = 5.0f * v4;
        v6 = 4.0f * -v4;
    }
};

static Table sTable;

static void InitScaled() {
    sTable.init(lbl_18_data_45F8);
}

__declspec(section ".ctors") static void (*const sInitScaled)() = InitScaled;

/* Paired-single vector helpers (inline asm, as in the SDK). */
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

/* Accumulating variants (two temporaries). */
static inline void VecAddT(register const Vec* a, register const Vec* b, register Vec* dst) {
    register f32 tb, ta;
    asm {
        psq_l ta, 0(a), 0, 0
        psq_l tb, 0(b), 0, 0
        ps_add ta, ta, tb
        psq_st ta, 0(dst), 0, 0
        psq_l ta, 8(a), 1, 0
        psq_l tb, 8(b), 1, 0
        ps_add ta, ta, tb
        psq_st ta, 8(dst), 1, 0
    }
}

static inline void VecSubT(register const Vec* a, register const Vec* b, register Vec* dst) {
    register f32 tb, ta;
    asm {
        psq_l ta, 0(a), 0, 0
        psq_l tb, 0(b), 0, 0
        ps_sub tb, ta, tb
        psq_l ta, 8(a), 1, 0
        psq_st tb, 0(dst), 0, 0
        psq_l tb, 8(b), 1, 0
        ps_sub tb, ta, tb
        psq_st tb, 8(dst), 1, 0
    }
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

/* Sets the box limits (lo/hi) the actor is clamped into. */
void Actor::setBounds(u8 on, const Vec* a, const Vec* b) {
    if (on) {
        m781 = 0;
    }
    m780 = on;
    fn_8016D940(&lo, a, b);
    fn_8016D980(&hi, a, b);
}

/* Sets a cylinder limit: center, radius and a y range. */
void Actor::setCircle(u8 on, const Vec* center, f32 radius, f32 yMin, f32 yMax) {
    Vec a;
    Vec b;
    if (on) {
        m780 = 0;
    }
    m781 = on;
    m784.x = center->x;
    m784.y = center->y;
    m784.z = center->z;
    m7A8 = radius;
    a.x = 0.0f;
    a.y = yMin;
    a.z = 0.0f;
    b.x = 0.0f;
    b.y = yMax;
    b.z = 0.0f;
    fn_8016D940(&lo, &a, &b);
    fn_8016D980(&hi, &a, &b);
}

void Actor::setFlag7B0(u8 v) {
    m7B0 = v;
}

void Actor::begin() {
    m7C0 = 0;
    m850 = 0;
    m7D4 = 0;
    if (m7B0) {
        v2A4();
    }
    if (m7B1 == 0 && v290()) {
        Vec* p = v2C0();
        vel.x = p->x;
        vel.y = p->y;
        vel.z = p->z;
    }
    v294();
}


struct HitBuffer {
    HitEntry* first;
    u32 count;
    u32 cap;
    u32* capRef;
    u32 pos;
};

inline size_t HitMaxSize() {
    return (size_t)-1 / sizeof(HitEntry);
}

inline size_t HitRecommendSize(size_t cap, const size_t n) {
    const size_t m = HitMaxSize();
    if (n > m - cap) {
        MslError("vector length error");
    }
    if (cap < m / 3) {
        return cap + Max(3 * (cap + 1) / 5, n);
    }
    if (cap < m / 3 * 2) {
        return cap + Max((cap + 1) / 2, n);
    }
    return m;
}

int Actor::addHit(Collider* c, int arg) {
    int result = (int)v2C4();
    HitEntry x;
    x.obj = c;
    x.arg = arg;
    if (hits.mSize < hits.mCap) {
        ++hits.mSize;
        hits.mData[hits.mSize - 1] = x;
    } else {
        HitRecommendSize(hits.mCap, 1);
        HitBuffer buf;
        buf.first = 0;
        buf.count = 0;
        buf.cap = 0;
        buf.capRef = &hits.mCap;
        buf.pos = 0;

        size_t need = hits.mSize + 1 - hits.mCap;
        size_t oldcap = hits.mCap;
        size_t newcap = HitRecommendSize(oldcap, need);
        if (newcap > HitMaxSize()) {
            MslError("vector length error");
        }
        HitEntry* p = (HitEntry*)operator new(newcap * sizeof(HitEntry));
        if (p == 0) {
            MslError("Memory allocation failure");
        }
        buf.first = p;
        buf.cap = newcap;
        buf.pos = hits.mSize;
        (buf.first + buf.pos)[buf.count] = x;
        ++buf.count;

        HitEntry* dst = buf.first + buf.pos;
        HitEntry* first = hits.mData;
        HitEntry* last = first + hits.mSize;
        for (HitEntry* src = last; src > first;) {
            --src;
            --dst;
            new (dst) HitEntry(*src);
            --buf.pos;
            ++buf.count;
        }

        hits.mSize = 0;
        u32 tmpCap = hits.mCap;
        hits.mCap = buf.cap;
        buf.cap = tmpCap;
        HitEntry* tmpData = hits.mData;
        hits.mData = buf.first;
        buf.first = tmpData;
        hits.mSize = buf.count;
        buf.count = 0;
        if (&buf != 0) {
            if (buf.first != 0) {
                buf.count = 0;
                operator delete(buf.first);
            }
        }
    }
    return result;
}

void Actor::clearHit() {
    m7C0 = 0;
    hits.mSize -= (int)hits.mSize;
    m850 = 0;
    m7D4 = 0;
    m7C4 = 0;
    Vec zero;
    zero.x = 0.0f;
    zero.y = 0.0f;
    zero.z = 0.0f;
    m7C8.x = zero.x;
    m7C8.y = zero.y;
    m7C8.z = zero.z;
}

struct Vec3Init {
    Vec v;
    Vec3Init(f32 x, f32 y, f32 z) {
        v.x = x;
        v.y = y;
        v.z = z;
    }
};

static inline void CopyVec(Vec* dst, const Vec& src) {
    dst->x = src.x;
    dst->y = src.y;
    dst->z = src.z;
}

u8 Actor::probeFloor() {
    f32 fall = m848 - m84C;
    Vec pos(vel.x, vel.y + m848, vel.z);
    Vec ground(vel.x, vel.y + m84C + (m850 ? m6A0 : 0.0f), vel.z);
    f32 py = pos.y;
    if (m780 || m781) {
        if (ground.y < hi.y) {
            Vec c;
            c.x = vel.x;
            c.z = vel.z;
            c.y = hi.y;
            fall = pos.y - hi.y;
            m7C0 = 1;
            m7C4 = 0;
            CopyVec(&m7C8, c);
            m7D4 = 1;
        }
    }
    if (m7B0) {
        Vec a;
        Vec b;
        Vec out;
        Vec* pp = &pos;
        Vec* ap = &a;
        Vec* gp = &ground;
        Vec* bp = &b;
        for (u32 i = 0; i < hits.mSize; i++) {
            for (u32 j = 0; j < 3; j++) {
                static Vec3Init offs[3] = {
                    Vec3Init(0.0f, 0.0f, -1.0f),
                    Vec3Init(-0.8f, 0.0f, 0.8f),
                    Vec3Init(0.8f, 0.0f, 0.8f),
                };
                VecAdd(pp, &offs[j].v, ap);
                VecAdd(gp, &offs[j].v, bp);
                if (hits.mData[i].obj->rayTestUp(ap, bp, &out, hits.mData[i].arg, info7D8)) {
                    f32 d = py - out.y;
                    if (fall > d) {
                        m7C0 = 1;
                        fall = d;
                        m7C4 = i;
                        Vec w(vel.x, out.y, vel.z);
                        CopyVec(&m7C8, w);
                        m7D4 = 0;
                    }
                }
            }
        }
    }
    return m7C0;
}

u8 Actor::probeSide() {
    Vec move;
    Vec hit;
    Vec out;
    VecAddT(&vel, &m8D4, &move);
    for (u32 i = 0; i < hits.mSize; i++) {
        Collider* o = hits.mData[i].obj;
        if (o->rayTestSide(m8E0, &move, &hit, &out, hits.mData[i].arg, info864)) {
            move.x = hit.x;
            move.y = hit.y;
            move.z = hit.z;
            m850 = 1;
            m854 = i;
            m858.x = out.x;
            m858.y = out.y;
            m858.z = out.z;
        }
    }
    if (m850) {
        VecSubT(&move, &m8D4, &move);
        vel.x = move.x;
        vel.y = move.y;
        vel.z = move.z;
    }
    return m850;
}

void Actor::clampMove() {
    if (m780) {
        fn_8016D940(&vel, &vel, &hi);
        fn_8016D980(&vel, &vel, &lo);
    } else if (m781) {
        Vec d;
        f32 dz = vel.z - m784.z;
        f32 dx = vel.x - m784.x;
        d.x = dx;
        d.y = 0.0f;
        d.z = dz;
        if (fn_80192D20(&d) > m7A8) {
            fn_80192CD0(&d, &d);
            VecScale(&d, &d, m7A8);
            vel.x = d.x + m784.x;
            vel.z = d.z + m784.z;
        }
    }
    if (m780 || m781) {
        if (vel.y > lo.y) {
            vel.y = lo.y;
        }
        if (m7B1 == 0) {
            if (vel.y <= hi.y) {
                vel.y = hi.y;
                m7C0 = 1;
                m7D4 = 1;
                m7C8.x = vel.x;
                m7C8.y = hi.y;
                m7C8.z = vel.z;
            }
        }
    }
}

void Actor::setHit(u8 v) {
    m7C0 = v;
    m7C4 = 0;
    if (v) {
        Vec* p = fn_80060100(node);
        m7C8.x = p->x;
        m7C8.y = p->y;
        m7C8.z = p->z;
        m7D4 = 1;
    }
}

}
