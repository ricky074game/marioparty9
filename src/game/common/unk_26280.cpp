/* Shared by most REL modules. Reference copy: mg9101 (fn_18_26280). */
#include "types.h"

typedef unsigned long size_t;

void* operator new(size_t);
void operator delete(void*);

/* ---------------------------------------------------------------------- */
/* Scene display task                                                      */

struct SceneBase;
class LayoutObj;

void fn_18_17270(SceneBase*, int);
void fn_800489D0(LayoutObj*, int); /* LayoutRef::SetLayer */
void fn_800714E0(void*, int);      /* ProcBase::SetX */
int fn_800751E0();
int fn_80074EA0();
void fn_8007CD10(const char*, f32, f32, f32);
void fn_8007FEF0(int, f32, f32);
void fn_80074B60(int, f32);
void fn_80074FB0();
void fn_8008CB80(const char*);

struct DisplayBase {
    int mState;
    LayoutObj** mItems;
    int mCount;
    int pad0C;
    int pad10;
};

/* Display object created by fn_18_2BD40 (vtable at +0x14, ProcBase at +0x18). */
class XDisplay : public DisplayBase {
public:
    XDisplay(int type); /* fn_18_2BD40 */
    virtual ~XDisplay();
    virtual void v1();
    virtual void v2();
    virtual void SetPriority(int priority);
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void Start(int arg);

    void Reset(int arg); /* fn_18_2BF00 */

    u8 pad[0x3C - 0x18];
};

class noncopyable {
protected:
    noncopyable() {}
    ~noncopyable() {}

private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

struct OwnDisplay : noncopyable {
    OwnDisplay() : mPtr(0) {}
    void reset(XDisplay* p) {
        if (p != mPtr) {
            XDisplay* old = mPtr;
            mPtr = p;
            delete old;
        }
    }
    ~OwnDisplay() { delete mPtr; }
    XDisplay* mPtr;
};


class DisplayTask {
public:
    DisplayTask(SceneBase* scene, int priority);
    virtual ~DisplayTask();
    virtual void Run();

    SceneBase* mScene;
    OwnDisplay mDisplay;
};

u8 sFlagA;
u8 sFlagB;

void fn_18_26280(int v) {
    sFlagA = v;
}

DisplayTask::DisplayTask(SceneBase* scene, int priority) : mScene(scene), mDisplay() {
    mDisplay.reset(new XDisplay(0x11));
    mDisplay.mPtr->Reset(0);
    fn_800714E0((u8*)mDisplay.mPtr + 0x18, 3);
    fn_800489D0(mDisplay.mPtr->mItems[0], 3);
    mDisplay.mPtr->SetPriority(0x907F);
    mDisplay.mPtr->Start(priority);
}

OwnDisplay* fn_18_26390(OwnDisplay* self, int flag) {
    if (self != 0) {
        delete self->mPtr;
        if (flag > 0) {
            operator delete(self);
        }
    }
    return self;
}

DisplayTask::~DisplayTask() {}

int fn_18_26480() {
    return sFlagB;
}

void fn_18_26490(int v) {
    sFlagB = v;
}

__declspec(weak) void PoolOrder() {
    f32 p = 0.0f;
    p = 1.0f;
    p = 0.5f;
    p = 2.0f; /* stands in for a second 0.0f the original pool keeps separate */
    p = 176.0f;
    p = -0.0f;
    p = -1.0f;
}

void DisplayTask::Run() {
    if (!fn_800751E0() && fn_80074EA0() && sFlagB) {
        sFlagB = 0;
        fn_8007CD10("SEQ_SESY029", 0.0f, 1.0f, 0.0f);
        fn_8007FEF0(-1, 0.0f, 0.3f);
        fn_80074B60(0, 0.3f);
        fn_80074FB0();
        fn_8008CB80("inst");
        fn_18_17270(mScene, 0);
    }
}

/* ---------------------------------------------------------------------- */
/* Block deque of vector<vector<int>>: 85 elements (12 bytes) per block.   */

struct IntVec {
    int* mData;
    u32 mSize;
    u32 mCap;

    ~IntVec() {
        if (mData != 0) {
            mSize -= (int)mSize;
            operator delete(mData);
        }
    }
};

template <class T>
inline void DestroyAt(T* p) {
    if (p != 0) {
        p->~T();
    }
}
template <class T>
inline void DestroyOne(T* p) {
    if (p != 0) {
        DestroyAt(p);
    }
}

struct VecVec {
    IntVec* mData;
    u32 mSize;
    u32 mCap;

    ~VecVec() {
        if (mData != 0) {
            u32 n = mSize;
            IntVec* p = mData + n;
            mSize -= n;
            while (n != 0) {
                --p;
                DestroyOne(p);
                --n;
            }
            operator delete(mData);
        }
    }
};

template <class T>
inline const T& Min(const T& a, const T& b) {
    return (a < b) ? a : b;
}

static const u32 kBlockSize = 85;

/* Circular iterator over the block map (see unk_28590 for the same shape). */
struct MapIterBase {
    VecVec** cur;
    VecVec** first;
    VecVec** begin;
    VecVec** last;

    void Advance(int n) {
        int cap = last - first;
        n += cur - first;
        if (n < 0) {
            n += cap;
        } else if (n >= cap) {
            n -= cap;
        }
        cur = first + n;
    }
    void AdvanceBlocks(u32 index) { Advance(index / kBlockSize); }
};

struct MapIter : MapIterBase {
    MapIter() {}
    MapIter(VecVec** c, VecVec** f, VecVec** b, VecVec** l) {
        cur = c;
        first = f;
        begin = b;
        last = l;
    }
    MapIter(const MapIter& o) {
        cur = o.cur;
        first = o.first;
        begin = o.begin;
        last = o.last;
    }
    MapIter& operator+=(int n) {
        Advance(n);
        return *this;
    }
    MapIter& operator-=(int n) { return *this += -n; }
    MapIter operator+(int n) const { return MapIter(*this) += n; }
};

inline MapIter Plus(const MapIter& it, int n) {
    return MapIter(it) += n;
}

/* Circular vector of block pointers */
struct BlockMap : noncopyable {
    VecVec** mData;
    u32 mStart;
    u32 mSize;
    u32 mCap;

    void Clear() { mSize -= (int)mSize; }
    void Free() {
        if (this != 0) {
            if (mData != 0) {
                Clear();
                operator delete(mData);
            }
        }
    }
    void Destroy() {
        if (this != 0) {
            Free();
        }
    }
    void Release() {
        if (this != 0) {
            Destroy();
        }
    }
    void Dispose() {
        if (this != 0) {
            Release();
        }
    }
    ~BlockMap() {
        if (this != 0) {
            Dispose();
        }
    }
    u32 EndIndex() const {
        if (mCap != 0) {
            return (mStart + mSize) % mCap;
        }
        return 0;
    }
    MapIter Begin() const {
        return MapIter(mData + mStart, mData, mData + mStart, mData + mCap);
    }
    MapIter End() const {
        return MapIter(mData + EndIndex(), mData, mData + mStart, mData + mCap);
    }
};

struct MapIterRange {
    MapIterBase first;
    MapIterBase last;
};

struct Alloc;

struct Deque {
    u32 mStart;
    u32 mSize;
    BlockMap mMap;

    Deque(Alloc* a); /* fn_18_28DE0 */
    ~Deque();
};

inline void FreeBlocks(MapIterBase first, MapIterBase last) {
    while (first.cur != last.cur) {
        if (*first.cur != 0) {
            operator delete(*first.cur);
        }
        if (++first.cur == first.last) {
            first.cur = first.first;
        }
    }
}

void fn_18_26CE0(Deque* d, u8* flag);

u32 sTwo = 2;

Deque::~Deque() {
    u8 flag = 0;
    fn_18_26CE0(this, &flag);
    if (mMap.mSize != 0) {
        MapIter e = mMap.End();
        MapIterBase b = e;
        b.cur = e.begin;
        while (b.cur != e.cur) {
            if (*b.cur != 0) {
                operator delete(*b.cur);
            }
            if (++b.cur == b.last) {
                b.cur = b.first;
            }
        }
    }
}

/* The helpers below live in neighbouring units (map accessors, iterators, allocator). */
BlockMap* fn_18_25AC0(Deque* d);
VecVec** fn_18_27090(BlockMap* m);
void fn_18_26F70(BlockMap* m);
VecVec** fn_18_270B0(BlockMap* m);
void fn_18_26F60(BlockMap* m, int* value);
void fn_18_27020(MapIter* out, BlockMap* m);
void fn_18_27940(MapIter* out, MapIter* it, int n);
VecVec** fn_18_27930(MapIter* it);
void fn_18_26FF0(MapIter* out, BlockMap* m);
void fn_18_26FA0(BlockMap* a, BlockMap* b);
void fn_18_277D0(BlockMap* m);
u32 fn_18_27100(BlockMap* m);
u32 fn_18_270E0(BlockMap* m);
int fn_18_270F0(BlockMap* m);
void fn_18_27120(BlockMap* m, u32 n);
void fn_18_27C90(BlockMap* m, MapIter* a, MapIter* b, MapIter* c, int flag);
u32* fn_18_27D90(u32* a, const u32* b);
VecVec* fn_18_27A20(Alloc* a, u32 n, int hint);
void fn_18_27A80(Alloc* a, VecVec* where, VecVec* value);
void fn_18_26F20(const char* msg);
int GetStaticLimit(Deque* d);
struct DequeIter {
    u32 w[6];
};
void fn_18_293A0(DequeIter* out, Deque* d);
VecVec* fn_18_26F10(DequeIter* it);

u32 fn_18_269E0(Deque* d);
u32 fn_18_26A00(Deque* d);
int fn_18_26A10(Deque* d);

#pragma dont_inline on
Alloc* fn_18_266C0(Deque* d) {
    return (Alloc*)&d->mMap;
}
#pragma dont_inline reset

#pragma dont_inline on
void fn_18_266D0(Deque* d, VecVec* v, u8* flag) {
    Alloc* a = fn_18_266C0(d);
    if (fn_18_26A10(d) == 0) {
        BlockMap* m = fn_18_25AC0(d);
        if (fn_18_26A00(d) >= kBlockSize) {
            VecVec* front = *fn_18_27090(m);
            fn_18_26F70(m);
            *fn_18_270B0(m) = front;
            int zero = 0;
            fn_18_26F60(m, &zero);
            d->mStart = d->mStart - kBlockSize;
            MapIter e2;
            MapIter e;
            fn_18_27020(&e, m);
            fn_18_27940(&e2, &e, 2);
            fn_18_27A80(a, *fn_18_27930(&e2), v);
        } else {
            u32 limit = GetStaticLimit(d) - kBlockSize;
            if (fn_18_269E0(d) > limit) {
                fn_18_26F20("deque:: length error");
            }
            u32 maxIdx = fn_18_27100(m);
            if (fn_18_270E0(m) < maxIdx) {
                VecVec* blk = fn_18_27A20(a, kBlockSize, 0);
                *fn_18_270B0(m) = blk;
                int zero = 0;
                fn_18_26F60(m, &zero);
                MapIter e2;
                MapIter e;
                fn_18_27020(&e, m);
                fn_18_27940(&e2, &e, 2);
                fn_18_27A80(a, *fn_18_27930(&e2), v);
            } else {
                Deque tmp(a);
                BlockMap* tm = fn_18_25AC0(&tmp);
                u32 dbl = fn_18_27100(m) * 2;
                fn_18_27120(tm, *fn_18_27D90(&dbl, &sTwo));
                int zero = 0;
                fn_18_26F60(tm, &zero);
                VecVec* blk = fn_18_27A20(a, kBlockSize, 0);
                *fn_18_270B0(tm) = blk;
                int zero2 = 0;
                fn_18_26F60(tm, &zero2);
                MapIter e2;
                MapIter e;
                fn_18_27020(&e, tm);
                fn_18_27940(&e2, &e, 2);
                fn_18_27A80(a, *fn_18_27930(&e2), v);
                if (fn_18_270F0(m) == 0) {
                    MapIter b2;
                    MapIter b1;
                    MapIter x2;
                    MapIter x;
                    fn_18_27020(&x, m);
                    fn_18_27940(&x2, &x, 1);
                    fn_18_26FF0(&b1, m);
                    fn_18_26FF0(&b2, tm);
                    fn_18_27C90(tm, &b2, &b1, &x2, 0);
                }
                fn_18_26FA0(m, tm);
                fn_18_277D0(tm);
            }
        }
    } else {
        DequeIter it;
        fn_18_293A0(&it, d);
        fn_18_27A80(a, fn_18_26F10(&it), v);
    }
    d->mSize++;
}
#pragma dont_inline reset

u32 fn_18_269E0(Deque* d) {
    u32 n = d->mMap.mSize;
    if (n == 0) {
        return 0;
    }
    return (n - 1) * kBlockSize;
}

u32 fn_18_26A00(Deque* d) {
    return d->mStart;
}

int fn_18_26A10(Deque* d) {
    u32 n = d->mMap.mSize;
    u32 cap = n == 0 ? 0 : (n - 1) * kBlockSize;
    return cap - (d->mStart + d->mSize);
}

/* Destroys the last n elements and frees blocks that became unused. */
void fn_18_26A40(Deque* d, u32 n) {
    if (n != 0) {
        u32 end = d->mStart + d->mSize;
        MapIter t1 = d->mMap.Begin();
        MapIterBase itv = t1;
        MapIterBase* it = &itv;
        it->Advance(end / kBlockSize);
        VecVec** cur = it->cur;
        VecVec** base = it->first;
        VecVec** lim = it->last;
        u32 off = end % kBlockSize;
        if (off == 0) {
            off = kBlockSize;
            if (cur == base) {
                cur = lim;
            }
            --cur;
        }
        VecVec* p = *cur + off;
        off = Min(n, off);
        for (;;) {
            for (u32 i = 0; i < off; i++) {
                --p;
                DestroyOne(p);
            }
            d->mSize -= off;
            n -= off;
            if (n == 0) {
                break;
            }
            if (cur == base) {
                cur = lim;
            }
            p = *--cur + kBlockSize;
            off = Min(n, kBlockSize);
        }
        u32 spare = fn_18_26A10(d);
        while (spare >= 2 * kBlockSize) {
            BlockMap& m = d->mMap;
            u32 newSize = m.mSize - 1;
            u32 idx = m.mStart + newSize;
            m.mSize = newSize;
            idx = (idx - 1) % m.mCap;
            VecVec** data = m.mData;
            operator delete(data[idx]);
            data[idx] = 0;
            spare -= kBlockSize;
        }
    }
}

/* Destroys every element. */
void fn_18_26CE0(Deque* d, u8* flag) {
    if (d->mSize != 0) {
        MapIter t1 = d->mMap.Begin();
        MapIterBase itv = t1;
        MapIterBase* it = &itv;
        it->Advance(d->mStart / kBlockSize);
        VecVec** cur = it->cur;
        VecVec** base = it->first;
        VecVec** lim = it->last;
        u32 off = d->mStart % kBlockSize;
        u32 count = kBlockSize - off;
        VecVec* first = *cur + off;
        VecVec* last = first + Min(count, d->mSize);
        for (;;) {
            d->mSize -= last - first;
            for (; first < last; first++) {
                DestroyOne(first);
            }
            if (d->mSize == 0) {
                break;
            }
            if (++cur == lim) {
                cur = base;
            }
            first = *cur;
            last = first + Min(kBlockSize, d->mSize);
        }
    }
}
