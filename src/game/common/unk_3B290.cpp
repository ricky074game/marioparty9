/* Shared by most REL modules. Reference copy: mg9101 (fn_18_3B290). */
/* Listener group: two sets of 8 listener slots, each mirrored into a pointer vector
   that is broadcast to through pointer-to-member virtual calls. */
#include "types.h"
#include "game/common/unk_3B290.h"
#include "game/common/unk_15E80.h"

extern "C" void* memcpy(void* dst, const void* src, size_t n);
extern "C" void* memset(void* dst, int c, size_t n);

static inline void LengthError() {
    MslError("vector length error");
}

static inline void AllocError() {
    fprintf(&__files[2], "Memory allocation failure");
    abort();
}

template <class T>
static inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

template <class T>
static inline void Swap(T& a, T& b) {
    T t = a;
    a = b;
    b = t;
}

/* Pointer vector (begin / size / capacity) with a split-buffer growth path */
template <class T>
struct PtrVec {
    enum { kMaxSize = 0x3FFFFFFF };

    struct SplitBufBase {
        T** mFirst;
        u32 mCount;
        u32 mCap;
        u32* mCapRef;
        u32 mPos;

        SplitBufBase(u32* capRef) : mFirst(0), mCount(0), mCap(0), mCapRef(capRef), mPos(0) {}
        ~SplitBufBase() {
            if (mFirst) {
                mCount = 0;
                operator delete(mFirst);
            }
        }
    };

    struct SplitBuf : SplitBufBase {
        SplitBuf(u32* capRef) : SplitBufBase(capRef) {}
        void push_back(T* const& v) {
            *(mFirst + mPos + mCount) = v;
            mCount++;
        }
    };

    T** mBegin;
    u32 mSize;
    u32 mCap;

    void clear() { mSize = mSize - (int)mSize; }

    void push_back(T* const& v) {
        if (mSize < mCap) {
            mSize += 1;
            mBegin[mSize - 1] = v;
            return;
        }
        if (kMaxSize - mCap < 1) {
            LengthError();
        }
        SplitBuf sb(&mCap);
        u32 f1, f2;
        u32 cap = mCap;
        u32 n = mSize + 1 - cap;
        if (n > kMaxSize - cap) {
            LengthError();
        }
        u32 newCap;
        if (cap < 0x15555555) {
            f1 = (cap + 1) * 3 / 5;
            newCap = cap + Max(f1, n);
        } else if (cap < 0x2AAAAAAA) {
            f2 = (cap + 1) / 2;
            newCap = cap + Max(f2, n);
        } else {
            newCap = kMaxSize;
        }
        if (newCap > kMaxSize) {
            LengthError();
        }
        T** p = (T**)operator new(newCap * sizeof(T*));
        if (p == 0) {
            AllocError();
        }
        sb.mFirst = p;
        sb.mCap = newCap;
        sb.mPos = mSize;
        sb.push_back(v);
        T** begin = mBegin;
        T** end = begin + mSize;
        u32 count = (T* const*)end - begin;
        sb.mPos -= count;
        memcpy(sb.mFirst + sb.mPos, begin, count * sizeof(T*));
        memset(begin, 0, count * sizeof(T*));
        sb.mCount += count;
        mSize = 0;
        Swap(mCap, sb.mCap);
        Swap(mBegin, sb.mFirst);
        mSize = sb.mCount;
        sb.mCount = 0;
    }
};

struct ListenerGroupBase {
    u32 m00;
    u32 m04;
};

class ListenerGroup : public ListenerGroupBase {
public:
    virtual void vf0();
    virtual void vf1();
    virtual void vf2();
    virtual void vf3();
    virtual void vf4();
    virtual void vf5();
    virtual void vf6();
    virtual void vf7();
    virtual void vf8();
    virtual void vf9();
    virtual void vf10();
    virtual void vf11();

    void BroadcastAll();
    void BroadcastA_vf62();
    void BroadcastB_vf1();
    void RunPhases(bool a, bool b, bool c);
    void BroadcastA_vf157();
    void BroadcastA_vf156();
    void BroadcastA_vf134();
    void RebuildA(int arg);
    void RemoveA(int idx);
    void RemoveB(int idx);

    s32 m0C;
    Element* mSlotsA[8];
    Element* mSlotsB[8];
    PtrVec<Element> mVecA;
    PtrVec<Element> mVecB;
};

typedef void (Element::*ElementFn)();

struct MemFun {
    ElementFn mFn;
    MemFun(ElementFn fn) : mFn(fn) {}
    void operator()(Element* e) const { (e->*mFn)(); }
};

template <class F>
static inline F ForEach(Element** first, Element** last, F f) {
    for (; first != last; ++first) {
        f(*first);
    }
    return f;
}

void ListenerGroup::BroadcastAll() {
    ForEach(mVecA.mBegin, mVecA.mBegin + mVecA.mSize, MemFun(&Element::vf59));
    ForEach(mVecA.mBegin, mVecA.mBegin + mVecA.mSize, MemFun(&Element::vf61));
    ForEach(mVecA.mBegin, mVecA.mBegin + mVecA.mSize, MemFun(&Element::vf60));
}

void ListenerGroup::BroadcastA_vf62() {
    ForEach(mVecA.mBegin, mVecA.mBegin + mVecA.mSize, MemFun(&Element::vf62));
}

void ListenerGroup::BroadcastB_vf1() {
    ForEach(mVecB.mBegin, mVecB.mBegin + mVecB.mSize, MemFun(&Element::vf1));
}

void ListenerGroup::RunPhases(bool a, bool b, bool c) {
    if (c) {
        vf10();
    }
    if (b) {
        vf9();
    }
    if (c) {
        vf10();
    }
    if (a) {
        vf11();
    }
}

void ListenerGroup::BroadcastA_vf157() {
    ForEach(mVecA.mBegin, mVecA.mBegin + mVecA.mSize, MemFun(&Element::vf157));
}

void ListenerGroup::BroadcastA_vf156() {
    ForEach(mVecA.mBegin, mVecA.mBegin + mVecA.mSize, MemFun(&Element::vf156));
}

void ListenerGroup::BroadcastA_vf134() {
    ForEach(mVecA.mBegin, mVecA.mBegin + mVecA.mSize, MemFun(&Element::vf134));
}

void ListenerGroup::RebuildA(int arg) {
    m0C = arg;
    mVecA.clear();
    Element** slots = mSlotsA;
    for (u32 i = 0; i < 8; i++) {
        if (mSlotsA[i]) {
            mVecA.push_back(slots[i]);
        }
    }
}

void ListenerGroup::RemoveA(int idx) {
    mSlotsA[idx] = 0;
    Element** slots = mSlotsA;
    mVecA.clear();
    for (u32 i = 0; i < 8; i++) {
        if (mSlotsA[i]) {
            mVecA.push_back(slots[i]);
        }
    }
}

void ListenerGroup::RemoveB(int idx) {
    mSlotsB[idx] = 0;
    Element** slots = mSlotsB;
    mVecB.clear();
    for (u32 i = 0; i < 8; i++) {
        if (mSlotsB[i]) {
            mVecB.push_back(slots[i]);
        }
    }
}
