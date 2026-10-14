/* Shared by most REL modules. Reference copy: mg9101 (fn_18_1F5E0). */
#include "types.h"

typedef unsigned long size_t;

struct FILE {
    u8 data[0x50];
};

extern "C" FILE __files[];
extern "C" int fprintf(FILE* stream, const char* format, ...);
extern "C" void abort(void);
extern "C" void* memcpy(void* dst, const void* src, size_t n);
extern "C" void* memset(void* dst, int c, size_t n);
extern "C" void* memmove(void* dst, const void* src, size_t n);
void* operator new(size_t);
void operator delete(void*);
inline void* operator new(size_t, void* p) {
    return p;
}

inline void MslError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

/* Not a template, so all vector instantiations share this one literal. */
extern const char kVectorLengthError[];
const char kVectorLengthError[] = "vector length error";

inline void LengthError() {
    fprintf(&__files[2], kVectorLengthError);
    abort();
}

template <class T>
inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

/* ---------------------------------------------------------------------- */
/* Small helpers                                                          */

struct PtrArray {
    int* mData;
    int** Addr(int i);
};

int** PtrArray::Addr(int i) {
    return (int**)mData + i;
}

/* MSL string with short-string optimization. */
struct SmallString {
    u32 mWord0;
    u32 mSize;
    char* mData;
    bool Empty() const;
};

bool SmallString::Empty() const {
    return (!(mWord0 >> 31) ? (u32)(*(u8*)this & 0x7F) : mSize) == 0;
}

struct Owned {
    virtual ~Owned();
};

struct OwnedHolder {
    Owned* mObj;
    void Set(Owned* obj);
};

void OwnedHolder::Set(Owned* obj) {
    Owned* old = mObj;
    if (obj == old) {
        return;
    }
    mObj = obj;
    delete old;
}

void EmptyFunc() {}

/* ---------------------------------------------------------------------- */
/* Metrowerks vector                                                      */

/* New storage is filled back to front and then swapped into the owner. */
template <class T>
struct SplitBase {
    SplitBase() : mFirst(0), mCount(0), mCap(0) {}
    ~SplitBase() {
        if (mFirst != 0) {
            mCount = 0;
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

    u32* mCapRef;
    u32 mRemaining;
};

template <class T>
struct Vector {
    static size_t max_size() { return (size_t)-1 / sizeof(T); }
    /* vector::operator= flavour */
    size_t RecommendSize(const size_t n) {
        size_t cap = mCapacity;
        const size_t m = max_size();
        if (n > m - cap) {
            LengthError();
        }
        if (cap < m / 3) {
            return cap + Max(3 * (cap + 1) / 5, n);
        }
        if (cap < m / 3 * 2) {
            return cap + Max((cap + 1) / 2, n);
        }
        return m;
    }
    /* push_back flavour: the compiler keeps n and the Max operands in locals */
    size_t RecommendSizeLocal(const size_t n0) {
        size_t cap = mCapacity;
        const size_t m = max_size();
        size_t n = n0;
        size_t b;
        size_t a;
        if (n > m - cap) {
            LengthError();
        }
        if (cap < m / 3) {
            a = 3 * (cap + 1) / 5;
            return cap + Max(a, n);
        }
        if (cap < m / 3 * 2) {
            b = (cap + 1) / 2;
            return cap + Max(b, n);
        }
        return m;
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
    u32 size() const { return mSize; }
    T& operator[](u32 i) { return mData[i]; }
    T& back() { return (*this)[mSize - 1]; }

    T* mData;
    u32 mSize;
    u32 mCapacity;
};

template <class T>
inline void CopyBackward(T* first, T* last, T* dst, SplitBuf<T>& buf) {
    while (last > first) {
        --last;
        --dst;
        new (dst) T(*last);
        --buf.mRemaining;
        ++buf.mCount;
    }
}

template <class T>
inline void CopyBackward(T** first, T** last, T** dst, SplitBuf<T*>& buf) {
    size_t n = last - first;
    buf.mRemaining -= n;
    memcpy(buf.mFirst + buf.mRemaining, first, n * sizeof(T*));
    memset(first, 0, n * sizeof(T*));
    buf.mCount += n;
}

template <class T>
inline void PushBack(Vector<T>& v, const T& x) {
    Vector<T>* self = &v;
    if (self->mSize < self->mCapacity) {
        ++self->mSize;
        T* p = self->mData + (self->mSize - 1);
        *p = x;
        return;
    }
    self->RecommendSizeLocal(1);
    SplitBuf<T> buf(&self->mCapacity);
    size_t n = self->mSize + 1 - self->mCapacity;
    size_t newcap = self->RecommendSizeLocal(n);
    buf.mFirst = Vector<T>::Allocate(newcap);
    buf.mCap = newcap;
    buf.mRemaining = self->mSize;
    T* end = buf.mFirst + self->mSize;
    end[buf.mCount] = x;
    ++buf.mCount;
    CopyBackward(self->mData, self->mData + self->mSize, buf.mFirst + buf.mRemaining, buf);
    self->mSize = 0;
    u32 c = self->mCapacity;
    self->mCapacity = buf.mCap;
    buf.mCap = c;
    T* d = self->mData;
    self->mData = buf.mFirst;
    buf.mFirst = d;
    self->mSize = buf.mCount;
    buf.mCount = 0;
}

struct V3 {
    V3& operator=(const V3& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
    f32 x, y;
    u32 z;
};

struct V3Vector : Vector<V3> {
    void push_back(const V3& x);
};

void V3Vector::push_back(const V3& x) {
    PushBack(*this, x);
}

/* ---------------------------------------------------------------------- */
/* Bit array                                                              */

struct BitRef {
    BitRef(u32* word, u32 mask) : mWord(word), mMask(mask) {}
    bool Test() const;
    u32* mWord;
    u32 mMask;
};

struct BitArray {
    u32* mData;
    BitRef Ref(u32 idx);
};

BitRef BitArray::Ref(u32 idx) {
    return BitRef(mData + (idx >> 5), 1 << (idx & 31));
}

bool BitRef::Test() const {
    return (*mWord & mMask) != 0;
}

/* ---------------------------------------------------------------------- */
/* Menu with one hand cursor per player                                   */

struct Object;
bool fn_80042D70(Object* obj);

class HandleList;

class Handle {
public:
    virtual ~Handle() {}

    Object* mObj;
};

bool fn_8004A220(const Handle& handle);

class HandleList {
public:
    Handle Get(int idx); /* fn_18_2B020 */
    void fn_18_2AE90(int idx);

    int unk0;
    Handle* mEntries;
    int mCount;
};

void fn_18_2B1F0(HandleList* list, int arg);

struct Vec2f {
    f32 x, y;
};

class HandCursor {
public:
    HandCursor(int player, int priority, int flag); /* fn_18_215A0 */
    const Vec2f* GetPos();                          /* fn_800385B0 */
    void Setup(void* res);                          /* fn_18_21940 */
};

/* HandCursor::SetPicture / fn_18_21DF0, fn_18_21F70, fn_80039010, fn_18_21F30 */
void fn_18_21DF0(HandCursor* cursor, int picture);
void fn_18_21F70(HandCursor* cursor, int a, int b);
void fn_80039010(HandCursor* cursor, int a);
void fn_18_21F30(HandCursor* cursor, f32 scale);
HandCursor* fn_18_215A0(HandCursor* cursor, int player, int priority, int flag);

BOOL fn_18_1BC20(int player);
int fn_18_1BBC0(int player);
void fn_800806C0();
void fn_18_1E5A0(void* menu);
void fn_18_20430(void* menu);
void fn_18_20980(void* menu);

/* vector<int> assembled through four layers of destructors. */
struct IntVecA : Vector<int> {
    ~IntVecA() {
        if (mData != 0) {
            mSize -= size();
            operator delete(mData);
        }
    }
    void Release() {
        mSize -= size();
        if (mData != 0) {
            operator delete(mData);
            mData = 0;
            mCapacity = 0;
        }
    }
};
struct IntVecB : IntVecA {
    ~IntVecB() {}
};
struct IntVecC : IntVecB {
    ~IntVecC() {}
};
struct IntVec : IntVecC {
    ~IntVec() {}
};

inline int* CopyInts(int* first, int* last, int* dst) {
    size_t n = last - first;
    memmove(dst, first, n * sizeof(int));
    return dst + n;
}

inline void Assign(IntVecA& dst, const IntVecA& rhs) {
    if (&dst != &rhs) {
        int* first = rhs.mData;
        int* last = first + rhs.mSize;
        size_t cap = dst.mCapacity;
        size_t n = last - first;
        if (n > cap) {
            size_t diff = n - cap;
            size_t newcap = dst.RecommendSize(diff);
            dst.Release();
            dst.mData = Vector<int>::Allocate(newcap);
            dst.mCapacity = newcap;
        }
        CopyInts(first, last, dst.mData);
        dst.mSize = n;
    }
}

struct CursorMgr;
extern CursorMgr* lbl_18_bss_4730;
CursorMgr* fn_18_20EA0(CursorMgr* mgr);
IntVec fn_18_21030(CursorMgr* mgr, int player);

class CursorMenu {
public:
    void Update();     /* fn_18_1F9E0 */
    void StartPlay();  /* fn_18_1FAC0 */

    int unk0;
    int unk4;
    int mState;
    int unkC;
    int mMode;
    int* mPlayers;
    u32 mPlayerCount;
    int unk1C;
    HandleList** mLists;
    int unk24;
    int unk28;
    int* mCursorPlayers;
    u32 mCursorCount;
    u8 pad34[0x78 - 0x34];
    Vector<HandCursor*> mCursors;
    BitArray mBits;
    u8 pad88[0x90 - 0x88];
    IntVec mVecs[4];
    Vec2f mPos[4];
    u8 padE0[4];
    u8 mActive;
    u8 padE5[3];
    void* mRes;
};

void CursorMenu::Update() {
    switch (mState) {
    case 2:
        fn_18_20430(this);
        break;
    case 3:
        fn_18_20980(this);
        break;
    case 4: {
        Handle first = mLists[0]->Get(1);
        bool done = false;
        if (fn_8004A220(first)) {
            Handle second = mLists[1]->Get(4);
            if (fn_8004A220(second)) {
                done = true;
            }
        }
        if (done) {
            fn_18_2B1F0(mLists[0], 0);
            fn_18_1E5A0(this);
            mState = 5;
        }
        break;
    }
    }
}

void CursorMenu::StartPlay() {
    if (mState == 1) {
        if (mCursorCount != 0) {
            fn_18_2B1F0(mLists[0], 1);
            mLists[0]->fn_18_2AE90(0);

            for (u32 i = 0; i < mCursorCount; i++) {
                u32 k = i + 1;
                fn_18_2B1F0(mLists[k], 1);
                mLists[k]->fn_18_2AE90(0);
                if (mBits.Ref(i).Test()) {
                    HandCursor* c = (HandCursor*)operator new(0x64);
                    if (c) {
                        c = fn_18_215A0(c, mCursorPlayers[i], 0x905F, 1);
                    }
                    PushBack(mCursors, c);
                    mCursors.back()->Setup(mRes);
                    fn_18_21F70(mCursors.back(), 0, 1);
                    fn_80039010(mCursors.back(), 0);
                    fn_18_21F30(mCursors.back(), 1.0f);
                    fn_18_21DF0(mCursors.back(), 1);
                }
            }

            if (mMode == 0) {
                for (u32 j = 0; j < mPlayerCount; j++) {
                    int player = mPlayers[j];
                    if (fn_18_1BC20(player)) {
                        HandCursor* c = (HandCursor*)operator new(0x64);
                        if (c) {
                            c = fn_18_215A0(c, player, 0x905F, 1);
                        }
                        PushBack(mCursors, c);
                        fn_18_21DF0(mCursors.back(), 0);
                        CursorMgr* mgr = lbl_18_bss_4730;
                        if (mgr == 0) {
                            mgr = (CursorMgr*)operator new(0x68);
                            if (mgr) {
                                mgr = fn_18_20EA0(mgr);
                            }
                        }
                        Assign(mVecs[j], fn_18_21030(mgr, fn_18_1BBC0(player)));
                        const Vec2f* pos = mCursors.back()->GetPos();
                        mPos[j] = *pos;
                    }
                }
            }
            fn_800806C0();
            mActive = 1;
            mState = 2;
        } else {
            mState = 5;
        }
    }
}
