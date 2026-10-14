/* Shared by most REL modules. Reference copy: mg9101 (fn_18_2AF90). */
#include "types.h"

typedef unsigned long size_t;

struct FILE {
    u8 data[0x50];
};

extern "C" FILE __files[];
extern "C" int fprintf(FILE* stream, const char* format, ...);
extern "C" void abort(void);
void* operator new(size_t);
void operator delete(void*);
inline void* operator new(size_t, void* p) {
    return p;
}

inline void MslError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

extern const char kVectorLengthError[];

inline void LengthError() {
    fprintf(&__files[2], kVectorLengthError);
    abort();
}

template <class T>
inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

extern f32 lbl_8026DAB4;

struct Object;
bool fn_80042D70(Object* obj);

class Handle {
public:
    Handle(Object* obj) : mObj(fn_80042D70(obj) ? obj : 0) {}
    Handle(const Handle& other) : mObj(fn_80042D70(other.mObj) ? other.mObj : 0) {}
    virtual ~Handle() {}

    Object* mObj;
};

/* ---------------------------------------------------------------------- */
/* Vector with reallocating push_back, assembled from split buffers       */

template <class T>
struct SplitBase {
    SplitBase() : mFirst(0), mCount(0), mCap(0) {}
    ~SplitBase() {
        if (mFirst != 0) {
            u32 n = mCount;
            T* p = mFirst + n;
            mCount = 0;
            while (n != 0) {
                --p;
                p->~T();
                --n;
            }
            operator delete(mFirst);
        }
    }

    T* mFirst;
    u32 mCount;
    u32 mCap;
};

template <class T>
struct SplitBuf : SplitBase<T> {
    SplitBuf(u32* capRef) : mCapRef(capRef), mRemaining(0) {}
    ~SplitBuf() {
        T* base = this->mFirst + mRemaining;
        T* end = base + this->mCount;
        while (end > base) {
            --end;
            end->~T();
        }
        this->mCount = 0;
    }

    u32* mCapRef;
    u32 mRemaining;
};

template <class T>
struct Vector {
    static size_t max_size() { return (size_t)-1 / sizeof(T); }
    size_t RecommendSize(const size_t n) {
        size_t cap = mCapacity;
        const size_t m = max_size();
        if (n > m - cap) {
            LengthError();
        }
        size_t r;
        if (cap < m / 3) {
            r = cap + Max(3 * (cap + 1) / 5, n);
        } else if (cap < m / 3 * 2) {
            r = cap + Max((cap + 1) / 2, n);
        } else {
            r = m;
        }
        return r;
    }
    static T* Allocate(size_t n) {
        if (n > max_size()) {
            LengthError();
        }
        T* p = (T*)operator new(n * sizeof(T));
        if (p == 0) {
            MslError("Memory allocation failure");
        }
        return p;
    }
    T& back() { return mData[mSize - 1]; }

    T* mData;
    u32 mSize;
    u32 mCapacity;
};

template <class T>
inline void CopyBackward(T* first, T* dst, T* last, SplitBuf<T>& buf) {
    while (last > first) {
        --last;
        --dst;
        new (dst) T(*last);
        --buf.mRemaining;
        ++buf.mCount;
    }
}

template <class T>
inline void PushBack(Vector<T>& v, const T& x) {
    Vector<T>* self = &v;
    if (self->mSize < self->mCapacity) {
        T* p = self->mData + self->mSize;
        new (p) T(x);
        ++self->mSize;
        return;
    }
    if (1 > Vector<T>::max_size() - self->mCapacity) {
        LengthError();
    }
    SplitBuf<T> buf(&self->mCapacity);
    size_t n = self->mSize + 1 - self->mCapacity;
    size_t newcap = self->RecommendSize(n);
    buf.mFirst = Vector<T>::Allocate(newcap);
    buf.mCap = newcap;
    buf.mRemaining = self->mSize;
    new (buf.mFirst + self->mSize + buf.mCount) T(x);
    ++buf.mCount;
    CopyBackward(self->mData, buf.mFirst + buf.mRemaining, self->mData + self->mSize, buf);
    u32 c = self->mCapacity;
    self->mCapacity = buf.mCap;
    buf.mCap = c;
    T* d = self->mData;
    self->mData = buf.mFirst;
    buf.mFirst = d;
    u32 sz = self->mSize;
    self->mSize = buf.mCount;
    buf.mCount = sz;
}

/* ---------------------------------------------------------------------- */
/* Animation handle list of a layout                                      */

struct Vec2 {
    f32 x, y;
};

struct Vec3 {
    f32 x, y, z;
};

struct Rect {
    Rect() {}
    Rect(const Rect& o) : l(o.l), t(o.t), r(o.r), b(o.b) {}
    f32 l, t, r, b;
};

struct PaneLink {
    PaneLink* next;
    PaneLink* prev;
};

struct Pane {
    u8 pad0[4];
    PaneLink link;
    u8 pad0C[0x14 - 0xC];
    PaneLink children;
    u8 pad1C[0x2C - 0x1C];
    Vec3 pos;
    u8 pad38[0x44 - 0x38];
    Vec2 size;
    u8 pad4C[0xBB - 0x4C];
    u8 flags;
};

class HandleList {
public:
    Handle Peek(int idx) {
        if (mAnims.mSize == 0) {
            return Handle(0);
        }
        return mAnims.mData[idx];
    }
    Handle Get(int idx);
    Handle AddAnim(const char* name, int arg);

    int unk0;
    Vector<Handle> mAnims;
};

class LayoutObj : public HandleList {
public:
    virtual ~LayoutObj();
};

void fn_8004A060(const Handle& handle);
void fn_8004A3C0(const Handle& handle, int arg);
bool fn_8004A220(const Handle& handle);
Handle fn_80049110(void* layout, const char* name, int arg);
Pane* fn_800486E0(LayoutObj* layout);
void fn_80048860(LayoutObj* layout, int priority);
Rect fn_8016FCB0(Pane* pane);

void fn_18_2AF90(HandleList* list, int idx) {
    const Handle& handle = list->Peek(idx);
    fn_8004A060(handle);
}

Handle HandleList::Get(int idx) {
    if (mAnims.mSize == 0) {
        return Handle(0);
    }
    return mAnims.mData[idx];
}

void fn_18_2B0C0(HandleList* list, int idx, int arg) {
    const Handle& handle = list->Peek(idx);
    fn_8004A3C0(handle, arg);
}

bool fn_18_2B160(HandleList* list, int idx) {
    const Handle& handle = list->Peek(idx);
    return fn_8004A220(handle);
}

void fn_18_2B1F0(LayoutObj* layout, int visible) {
    if (fn_800486E0(layout)) {
        Pane* pane = fn_800486E0(layout);
        pane->flags = (pane->flags & 0xFE) | (u8)visible;
    }
}

u8 fn_18_2B250(LayoutObj* layout) {
    if (fn_800486E0(layout)) {
        return fn_800486E0(layout)->flags & 1;
    }
    return 0;
}

Handle HandleList::AddAnim(const char* name, int arg) {
    Handle h = fn_80049110(this, name, arg);
    PushBack(mAnims, h);
    return mAnims.back();
}

/* ---------------------------------------------------------------------- */
/* Pane bounds                                                            */

static inline void VecAddTo(register Vec3* a, register const Vec3* b) {
    register f32 tb, ta;
    asm {
        psq_l ta, 0(a), 0, 0
        psq_l tb, 0(b), 0, 0
        ps_add tb, ta, tb
        psq_l ta, 8(a), 1, 0
        psq_st tb, 0(a), 0, 0
        psq_l tb, 8(b), 1, 0
        ps_add tb, ta, tb
        psq_st tb, 8(a), 1, 0
    }
}

Rect fn_18_2B6B0(LayoutObj* layout, Pane* pane, const Vec3* offset) {
    Rect r;
    r.l = 9999999.0f;
    r.t = -9999999.0f;
    r.r = -9999999.0f;
    r.b = 9999999.0f;
    if (pane == 0) {
        pane = fn_800486E0(layout);
    }
    Vec3 pos = pane->pos;
    Vec3* pp = &pos;
    if (offset) {
        VecAddTo(pp, offset);
        pp->x *= lbl_8026DAB4;
    }
    if (pane != fn_800486E0(layout)) {
        Vec2 size = pane->size;
        if (pane->flags & 1) {
            r = fn_8016FCB0(pane);
            r.l *= size.x;
            r.r *= size.x;
            r.t *= size.y;
            r.b *= size.y;
            if (r.l > r.r) {
                f32 tmp = r.l;
                r.l = r.r;
                r.r = tmp;
            }
            if (r.b > r.t) {
                r.t = r.b;
                r.b = r.t;
            }
            f32 k = lbl_8026DAB4;
            r.l *= k;
            r.r *= k;
            r.l += pos.x;
            r.r += pos.x;
            r.t += pos.y;
            r.b += pos.y;
        }
    }
    PaneLink* n = pane->children.next;
    PaneLink* end = &pane->children;
    for (; n != end; n = n->next) {
        Rect c = fn_18_2B6B0(layout, (Pane*)((u8*)n - 4), &pos);
        if (c.l < r.l) {
            r.l = c.l;
        }
        if (c.r > r.r) {
            r.r = c.r;
        }
        if (c.t > r.t) {
            r.t = c.t;
        }
        if (c.b < r.b) {
            r.b = c.b;
        }
    }
    return r;
}

/* ---------------------------------------------------------------------- */
/* Group of layouts positioned together                                   */

struct VecImpl {
    LayoutObj** mData;
    u32 mSize;
    u32 mCap;
    ~VecImpl() {
        if (mData) {
            mSize -= (int)mSize;
            operator delete(mData);
        }
    }
};

struct VecBase : VecImpl {
    ~VecBase() {}
};

struct VecBase2 : VecBase {
    ~VecBase2() {}
};

struct LayoutVec : VecBase2 {
    ~LayoutVec() {}
};

class LayoutGroupBase {
public:
    int mState;
    LayoutObj** mData;
    u32 mSize;
    u32 mCap;
    int mPriority;
};

class LayoutGroup : public LayoutGroupBase {
public:
    LayoutGroup();
    virtual ~LayoutGroup();
    virtual void SetPos(f32 x, f32 y);
    virtual void SetPos(const Vec2* pos);
    virtual void SetPriority(int priority);
    virtual void Pure() = 0;
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();

    void SetPosImpl(const Vec2* pos) {
        Pane* pane = fn_800486E0(mData[0]);
        Vec3 v;
        v.x = pos->x;
        v.y = pos->y;
        v.z = 0.0f;
        pane->pos = v;
    }
};

LayoutGroup::LayoutGroup() {
    mState = 0;
    mData = 0;
    mSize = 0;
    mCap = 0;
    mPriority = 0x9030;
}

LayoutGroup::~LayoutGroup() {
    LayoutObj** end = mData + mSize;
    LayoutObj** it = mData;
    for (; it != end; ++it) {
        delete *it;
        *it = 0;
    }
    ((LayoutVec*)&mData)->~LayoutVec();
}

void LayoutGroup::SetPos(f32 x, f32 y) {
    Vec2 pos;
    pos.x = x;
    pos.y = y;
    SetPosImpl(&pos);
}

void LayoutGroup::SetPos(const Vec2* pos) {
    SetPosImpl(pos);
}

void LayoutGroup::SetPriority(int priority) {
    mPriority = priority;
    for (u32 i = 0; i < mSize; i++) {
        fn_80048860(mData[i], priority);
    }
}
