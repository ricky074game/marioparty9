/* Shared by most REL modules. Reference copy: mg9101 (fn_18_28590). */
#include "types.h"

typedef unsigned long size_t;

struct FILE {
    u8 data[0x50];
};

extern "C" FILE __files[];
extern "C" int fprintf(FILE* stream, const char* format, ...);
extern "C" void abort(void);
extern "C" void* memmove(void* dst, const void* src, size_t n);
void* operator new(size_t);
void operator delete(void*);

inline void MslError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

template <class T>
inline const T& Min(const T& a, const T& b) {
    return (b < a) ? b : a;
}

inline size_t MaxSize() {
    return (size_t)-1 / sizeof(void*) - 1;
}

/* Metrowerks circular deque iterator (without the trailing flag byte). */
struct RingIterBase {
    u32* cur;
    u32* first;
    u32* begin;
    u32* last;

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
    bool AfterBegin() const { return cur >= begin; }
};

struct RingIterX : RingIterBase {
    RingIterX(u32* c, u32* f, u32* b, u32* l) {
        cur = c;
        first = f;
        begin = b;
        last = l;
    }
    RingIterX(const RingIterX& o) {
        cur = o.cur;
        first = o.first;
        begin = o.begin;
        last = o.last;
    }
};

struct RingIter : RingIterBase {
    RingIter() {}
    RingIter(const RingIter& o)  {
        cur = o.cur;
        first = o.first;
        begin = o.begin;
        last = o.last;
    }
    RingIter& operator+=(int n) {
        Advance(n);
        return *this;
    }
    u32& operator*() { return *cur; }
    RingIter& operator-=(int n) { return *this += -n; }
};

struct RingIterEx : RingIterBase {
    bool flag;

    RingIterEx(const RingIterEx& o) : RingIterBase(o), flag(o.flag) {}
};

inline u32* CopyRange(u32* first, u32* last, u32* out) {
    size_t bytes = (last - first) * sizeof(u32);
    memmove(out, first, bytes);
    return (u32*)((char*)out + bytes);
}

/* Copies the ring range [src, srcEnd) to dst in contiguous chunks. */
RingIter RingCopy(RingIter src, RingIter srcEnd, RingIter dst) {
    while (src.cur != srcEnd.cur) {
        int dstSpace = dst.last - dst.cur;
        int n;
        if (src.AfterBegin() == srcEnd.AfterBegin()) {
            n = Min((int)(srcEnd.cur - src.cur), dstSpace);
        } else {
            n = Min((int)(src.last - src.cur), dstSpace);
        }
        CopyRange(src.cur, src.cur + n, dst.cur);
        src.cur += n - 1;
        if (++src.cur == src.last) {
            src.cur = src.first;
        }
        dst.cur += n - 1;
        if (++dst.cur == dst.last) {
            dst.cur = dst.first;
        }
    }
    return dst;
}

inline int Distance(RingIterEx first, RingIterEx last) {
    int d = last.cur - first.cur;
    if (last.cur >= last.begin) {
        if (first.cur < last.begin) {
            d -= last.last - last.first;
        }
    } else {
        if (first.cur >= last.begin) {
            d += last.last - last.first;
        }
    }
    return d;
}

int RingDistance(const RingIterEx& first, const RingIterEx& last) {
    return Distance(first, last);
}

void RingIterEx_Copy(RingIterEx* dst, const RingIterEx* src) {
    *dst = *src;
}

void RingIter_Sub(RingIter* it, int n) {
    *it -= n;
}

RingIter RingIter_Add(const RingIter& it, int n) {
    return RingIter(it) += n;
}

RingIter RingIter_Copy(const RingIter& it) {
    return it;
}

RingIter RingIter_SubN(const RingIter& it, int n) {
    return RingIter(it) -= n;
}

inline u32 Deref(RingIterBase it) {
    return *it.cur;
}
u32 RingIter_Deref(const RingIter* it) {
    return Deref(*it);
}

void RingIter_Inc(RingIter* it) {
    if (++it->cur == it->last) {
        it->cur = it->first;
    }
}

/* Circular buffer storage: data/start/size/capacity, followed by an allocator slot. */
struct CDeq {
    u32* mData;
    u32 mStart;
    u32 mSize;
    u32 mCapacity;
    u32 mAlloc;

    CDeq(u32 alloc);
    void Allocate(void* unused, size_t n);
    u32 GetAlloc() const;
    void PushFrontRange(const RingIter& first, const RingIter& last);
    void PushBackRange(const RingIter& first, const RingIter& last);
};

void CDeq::Allocate(void* unused, size_t n) {
    static const size_t m = MaxSize();
    if (n > m) {
        MslError("cdeque length error");
    }
    u32* p = (u32*)operator new((n + 1) * sizeof(u32));
    if (p == 0) {
        fprintf(&__files[2], "Memory allocation failure");
        abort();
    }
    mData = p;
    mCapacity = n + 1;
}

u32 CDeq::GetAlloc() const {
    return mAlloc;
}

void CDeq::PushFrontRange(const RingIter& first, const RingIter& last) {
    u32* dst = mData + mStart;
    if (first.cur <= last.cur) {
        CopyRange(first.cur, last.cur, dst - (last.cur - first.cur));
        int n = last.cur - first.cur;
        mSize += n;
        mStart -= n;
    } else {
        u32* tail = dst - (last.cur - last.first);
        CopyRange(last.first, last.cur, tail);
        CopyRange(first.cur, first.last, tail - (first.last - first.cur));
        int n = (last.cur - last.first) + (first.last - first.cur);
        mStart -= n;
        mSize += n;
    }
}

CDeq::CDeq(u32 alloc) {
    mData = 0;
    mStart = 0;
    mSize = 0;
    mCapacity = 0;
    mAlloc = alloc;
}

void CDeq::PushBackRange(const RingIter& first, const RingIter& last) {
    u32* dst = mData + (mStart + mSize) % mCapacity;
    if (first.cur <= last.cur) {
        CopyRange(first.cur, last.cur, dst);
        mSize += last.cur - first.cur;
    } else {
        CopyRange(last.first, last.cur, CopyRange(first.cur, first.last, dst));
        mSize += (first.last - first.cur) + (last.cur - last.first);
    }
}

struct CapHolderA {
    u32 pad[3];
    u32 mCapacity;
    u32* CapRef();
};
struct CapHolderB {
    u32 pad[3];
    u32 mCapacity;
    u32* CapRef();
};

u32* CapHolderA::CapRef() {
    return &mCapacity;
}

u32* CapHolderB::CapRef() {
    return &mCapacity;
}

inline int StaticLimit() {
    return -0x56;
}

int GetStaticLimit() {
    static const int m = StaticLimit();
    return m;
}

inline int DistanceImpl(RingIterBase first, RingIterBase last) {
    int d = last.cur - first.cur;
    if (last.cur >= last.begin) {
        if (first.cur < last.begin) {
            d -= last.last - last.first;
        }
    } else {
        if (first.cur >= last.begin) {
            d += last.last - last.first;
        }
    }
    return d;
}

inline int Distance(RingIterBase first, RingIterBase last) {
    return DistanceImpl(first, last);
}

inline int DistanceRef(const RingIterBase& first, const RingIterBase& last) {
    int d = last.cur - first.cur;
    if (last.cur >= last.begin) {
        if (first.cur < last.begin) {
            d -= last.last - last.first;
        }
    } else {
        if (first.cur >= last.begin) {
            d += last.last - last.first;
        }
    }
    return d;
}

inline u32* CopyToPtr(RingIterBase first, RingIterBase last, u32* out) {
    while (first.cur != last.cur) {
        int n;
        if (first.AfterBegin() == last.AfterBegin()) {
            n = last.cur - first.cur;
        } else {
            n = first.last - first.cur;
        }
        u32* next = CopyRange(first.cur, first.cur + n, out);
        first.cur += n - 1;
        if (++first.cur == first.last) {
            first.cur = first.first;
        }
        out = next;
    }
    return out;
}

struct RingBuf {
    u32* mData;
    u32 mStart;
    u32 mSize;
    u32 mCap;

    RingBuf() : mData(0), mStart(0), mSize(0), mCap(0) {}
    void Erase(const u32& n) { mSize -= n; }
    void Clear() { Erase(mSize); }
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
    ~RingBuf() {
        if (this != 0) {
            Destroy();
        }
    }

    u32 EndIndex() const {
        if (mSize != 0) {
            return (mStart + mSize) % mCap;
        }
        return 0;
    }
    RingIterX Begin() const {
        return RingIterX(mData + mStart, mData, mData + mStart, mData + mCap);
    }
    RingIterX End() const {
        return RingIterX(mData + EndIndex(), mData, mData + mStart, mData + mCap);
    }
};

inline size_t MaxSize2() {
    static const size_t m = MaxSize();
    return m;
}

struct RingHolder {
    u32 mA;
    u32 mB;
    RingBuf mBuf;

    RingHolder();
};

inline void ConstructFrom(RingBuf& d, RingIterBase first, RingIterBase last) {
    int n = Distance(first, last);
    if (n != 0) {
        if (n > MaxSize2()) {
            MslError("cdeque length error");
        }
        u32* p = (u32*)operator new((n + 1) * sizeof(u32));
        if (p == 0) {
            fprintf(&__files[2], "Memory allocation failure");
            abort();
        }
        d.mData = p;
        d.mCap = n + 1;
        if (d.mData != 0) {
            u32* dst = d.mData + (d.mStart + d.mSize) % d.mCap;
            int count = DistanceRef(first, last);
            int space = (d.mData + d.mCap) - dst;
            int chunk = Min(count, space);
            d.mSize += chunk;
            RingIterBase mid = first;
            mid.Advance(chunk);
            dst = CopyToPtr(first, mid, dst);
            count -= chunk;
            if (count != 0) {
                RingIterBase second = first;
                second.Advance(chunk);
                CopyToPtr(second, last, d.mData);
            }
        }
    }
}

RingHolder::RingHolder() : mA(0), mB(0) {
    RingBuf x;
    mBuf.mData = 0;
    mBuf.mStart = 0;
    mBuf.mSize = 0;
    mBuf.mCap = 0;
    ConstructFrom(mBuf, x.Begin(), x.End());
}
