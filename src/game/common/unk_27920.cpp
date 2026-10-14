/* Shared by most REL modules. Reference copy: mg9101 (fn_18_27920). */
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

/* Circular deque iterators (the out-of-line helpers live in unk_28590.cpp). */
struct RingIterBase {
    u32* cur;
    u32* first;
    u32* begin;
    u32* last;

    u32 operator*() const;
    RingIterBase& operator++();
    bool AfterBegin() const { return cur >= begin; }
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
};

struct RingIterPlain : RingIterBase {
    u32 Value() const { return *cur; }
};

struct RingIterP1 : RingIterBase {
    RingIterP1(const RingIterBase& o) {
        cur = o.cur;
        first = o.first;
        begin = o.begin;
        last = o.last;
    }
};
struct RingIterP2 : RingIterBase {
    RingIterP2(const RingIterBase& o) {
        cur = o.cur;
        first = o.first;
        begin = o.begin;
        last = o.last;
    }
};

struct RingIter : RingIterBase {
    RingIter() {}
    RingIter(const RingIterBase& o) {
        cur = o.cur;
        first = o.first;
        begin = o.begin;
        last = o.last;
    }
    RingIter(const RingIter& o) {
        cur = o.cur;
        first = o.first;
        begin = o.begin;
        last = o.last;
    }
    RingIter& operator=(const RingIter& o);
    u32 Value() const { return *cur; }
    RingIter& operator+=(int n) {
        Advance(n);
        return *this;
    }
    RingIter& operator-=(int n);
    RingIter operator-(int n) const;
    RingIter operator+(int n) const;
    RingIter CopyOf() const;
    u32* Cur();
};

struct FlagByte {
    u8 value;
    FlagByte() {}
    FlagByte(const FlagByte& o) { value = o.value; }
};

struct RingIterEx : RingIterBase {
    FlagByte flag;

    RingIterEx(const RingIterBase& o) {
        cur = o.cur;
        first = o.first;
        begin = o.begin;
        last = o.last;
    }
    RingIterEx CopyOf() const;
    RingIterEx& operator+=(int n) {
        Advance(n);
        return *this;
    }
    RingIterEx operator+(int n) const { return RingIterEx(*this) += n; }
    RingIterEx& Inc() {
        if (++cur == last) {
            cur = first;
        }
        return *this;
    }
    RingIterEx& operator++() { return Inc(); }
};

struct PtrBoxA {
    u32* p;
    u32* Get();
};
struct PtrBoxB {
    u32* p;
    u32* Get();
};

RingIter RingCopy(RingIter src, RingIter srcEnd, RingIter dst);
RingIter RingCopyBackward(RingIter first, RingIter last, RingIter dst);
RingIter RingCopyEx(RingIterEx first, RingIterEx last, RingIter dst);
int RingDistance(const RingIterEx& first, const RingIterEx& last);

struct FalseType {};

/* Circular buffer: data/start/size/capacity. */
struct Deque {
    u32* mData;
    u32 mStart;
    u32 mSize;
    u32 mCap;

    void ClearSize();
    RingIter Begin() const;
    RingIter End() const;
    u32 Capacity() const;
    u32* CapRef();
    u32 AllocRef();
    u32 RecommendSize(u32 n) const;

    void Insert(RingIter pos, RingIter first, RingIter last);
    void InsertB(const RingIter& pos, RingIterP2 first, RingIterP2 last) {
        InsertRange(pos, first, last, FalseType());
    }
    void InsertA(const RingIter& pos, RingIterP1 first, RingIterP1 last) {
        InsertB(pos, first, last);
    }
    void InsertRange(RingIter pos, RingIterEx first, RingIterEx last, FalseType);
};

struct CDeq {
    u32* mData;
    u32 mStart;
    u32 mSize;
    u32 mCapacity;
    u32 mAlloc;

    CDeq(u32 alloc);
    ~CDeq();
    void Allocate(void* unused, size_t n);
    u32 GetAlloc() const;
    u32* CapRef();
    void PushFrontRange(const RingIter& first, const RingIter& last);
    void PushBackRange(const RingIter& first, const RingIter& last);
};

void Deque::ClearSize() {
    mSize = 0;
}

u32* PtrBoxA::Get() {
    return p;
}

RingIter RingIter::operator-(int n) const {
    return RingIter(*this) += -n;
}

RingIter RingIter::CopyOf() const {
    return *this;
}

u32* RingIter::Cur() {
    return cur;
}

u32* PtrBoxB::Get() {
    return p;
}

void* Alloc12(void* unused, u32 n) {
    void* p = operator new(n * 12);
    if (p == 0) {
        fprintf(&__files[2], "Memory allocation failure");
        abort();
    }
    return p;
}

inline void* operator new(size_t, void* p) {
    return p;
}

struct IntVec {
    u32* mData;
    u32 mSize;
    u32 mCap;

    IntVec() : mData(0), mSize(0), mCap(0) {}
    void Allocate(u32 n) {
        u32* p = (u32*)operator new(n * sizeof(u32));
        if (p == 0) {
            MslError("Memory allocation failure");
        }
        mData = p;
        mCap = n;
    }
};

struct VecVec {
    IntVec* mData;
    u32 mSize;
    u32 mCap;

    VecVec() : mData(0), mSize(0), mCap(0) {}
    void Allocate(u32 n) {
        IntVec* p = (IntVec*)operator new(n * sizeof(IntVec));
        if (p == 0) {
            MslError("Memory allocation failure");
        }
        mData = p;
        mCap = n;
    }
};

inline void CopyInts(u32* dst, const u32* first, const u32* last) {
    memmove(dst, first, (last - first) * sizeof(u32));
}

/* Copy-constructs a vector<vector<u32>> in place. */
void CopyVecVec(void* unused, VecVec* vec, const VecVec* src) {
    if (vec != 0) {
        vec->mData = 0;
        vec->mSize = 0;
        vec->mCap = 0;
        const IntVec* first = src->mData;
        const IntVec* last = src->mData + src->mSize;
        u32 n = last - first;
        if (n != 0) {
            if (n > (size_t)-1 / sizeof(IntVec)) {
                MslError("vector length error");
            }
            vec->Allocate(n);
            IntVec* dst = vec->mData + vec->mSize;
            for (; first != last; ++first, ++dst) {
                new (dst) IntVec;
                const u32* f = first->mData;
                const u32* l = first->mData + first->mSize;
                if (l - f != 0) {
                    if ((u32)(l - f) > (size_t)-1 / sizeof(u32)) {
                        MslError("vector length error");
                    }
                    dst->Allocate(l - f);
                    CopyInts(dst->mData + dst->mSize, f, l);
                    dst->mSize += l - f;
                }
                ++vec->mSize;
            }
        }
    }
}

void Construct(u32 alloc, u32* dst, const u32* src) {
    if (dst != 0) {
        *dst = *src;
    }
}

void Deque::Insert(RingIter pos, RingIter first, RingIter last) {
    InsertA(pos, RingIterP1(first), RingIterP1(last));
}

const u32& MaxRef(const u32& a, const u32& b) {
    return (a < b) ? b : a;
}

void SwapPtr(u32*& a, u32*& b) {
    u32* t = a;
    a = b;
    b = t;
}

void SwapU32(u32& a, u32& b) {
    u32 t = a;
    a = b;
    b = t;
}

#pragma dont_inline on
void Deque::InsertRange(RingIter pos, RingIterEx first, RingIterEx last, FalseType) {
    u32 n = RingDistance(RingIterEx(first), RingIterEx(last));
    if (n != 0) {
        u32* base = mData + mStart;
        u32 idx = pos.cur - base;
        if (pos.cur < base) {
            idx += *CapRef();
        }
        u32 cap = Capacity();
        if (n <= cap && mSize <= cap - n) {
            RingIter it = Begin();
            u32 after = mSize - idx;
            if (idx < after) {
                RingIter oldBegin(it);
                it -= n;
                mStart = it.Cur() - mData;
                mSize += n;
                if (idx != 0) {
                    it = RingCopy(oldBegin, oldBegin + idx, it);
                }
            } else {
                mSize += n;
                it = End();
                if (after != 0) {
                    it = RingCopyBackward(it - (n + after), it - n, it);
                }
                it -= n;
            }
            RingCopyEx(first, last, it);
        } else {
            CDeq tmp(AllocRef());
            tmp.Allocate((void*)(mSize + n), RecommendSize(n));
            tmp.mStart = idx;
            u32* dst = tmp.mData + idx;
            while (n != 0) {
                u32 v = *first;
                Construct(tmp.GetAlloc(), dst, &v);
                ++tmp.mSize;
                --n;
                ++dst;
                ++first;
            }
            tmp.PushBackRange(pos, End());
            tmp.PushFrontRange(Begin(), pos);
            ClearSize();
            SwapPtr(mData, tmp.mData);
            SwapU32(mStart, tmp.mStart);
            SwapU32(mSize, tmp.mSize);
            SwapU32(*CapRef(), *tmp.CapRef());
        }
    }
}

#pragma dont_inline reset
inline u32* CopyElems(RingIterEx a, RingIterEx b, u32* out) {
    while (a.cur != b.cur) {
        RingIterPlain old = *(RingIterPlain*)&a;
        a.Inc();
        *out = old.Value();
        ++out;
    }
    return out;
}

inline int Distance(const RingIterEx& first, const RingIterEx& last) {
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

RingIter RingCopyEx(RingIterEx first, RingIterEx last, RingIter dst) {
    if (!(first.cur == last.cur)) {
        int count = Distance(first, last);
        for (;;) {
            int space = dst.last - dst.cur;
            int chunk = Min(count, space);
            RingIterEx mid = first + chunk;
            CopyElems(first, mid, dst.cur);
            count -= chunk;
            dst.cur += chunk - 1;
            if (++dst.cur == dst.last) {
                dst.cur = dst.first;
            }
            if (count == 0) {
                break;
            }
            first = mid;
        }
    }
    return dst;
}

inline void CopyBackward(u32* first, u32* last, u32* outEnd) {
    size_t bytes = (last - first) * sizeof(u32);
    memmove(outEnd - (last - first), first, bytes);
}

RingIter RingCopyBackward(RingIter first, RingIter last, RingIter dst) {
    while (first.cur != last.cur) {
        if (last.cur == last.first) {
            last.cur = last.last;
        }
        --last.cur;
        if (dst.cur == dst.first) {
            dst.cur = dst.last;
        }
        --dst.cur;
        int room = dst.cur - (dst.cur >= dst.begin ? dst.begin : dst.first);
        int n;
        if (first.AfterBegin() == last.AfterBegin()) {
            n = Min((int)(last.cur - first.cur), room);
        } else {
            n = Min((int)(last.cur - (last.cur >= last.begin ? last.begin : last.first)), room);
        }
        last.cur -= n;
        CopyBackward(last.cur, last.cur + (n + 1), dst.cur + 1);
        dst.cur -= n;
    }
    return dst;
}
