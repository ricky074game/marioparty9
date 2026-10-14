/* Shared by most REL modules. Reference copy: mg9101 (fn_18_1D480). */
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

void* operator new(size_t);
void operator delete(void*);

inline void MslError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

inline void LengthError(const char* msg = "vector length error") {
    MslError(msg);
}

inline void AllocError() {
    MslError("Memory allocation failure");
}

template <class T>
inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

static f32 sUnk4728;

static void InitUnk4728() {
    sUnk4728 = 60.0f;
}

__declspec(section ".ctors") static void (*const sInitUnk4728)() = InitUnk4728;

/* Scratch buffer used while growing a vector<int>: the new storage is filled
   back to front and then swapped into the owner. */
struct SplitBase {
    SplitBase() : mFirst(0), mCount(0), mCap(0) {}
    ~SplitBase() {
        if (mFirst != 0) {
            mCount = 0;
            operator delete(mFirst);
        }
    }

    int* mFirst;
    u32 mCount;
    u32 mCap;
};

struct SplitBuf : SplitBase {
    SplitBuf(u32* capRef) : mCapRef(capRef), mRemaining(0) {}

    u32* mCapRef;
    u32 mRemaining;
};

/* vector<int> assembled through four layers of destructors. */
template <int N>
struct IntVecA {
    IntVecA() : mData(0), mSize(0), mCap(0) {}
    ~IntVecA() {
        if (mData != 0) {
            mSize -= (int)mSize;
            operator delete(mData);
        }
    }

    static size_t max_size() { return (size_t)-1 / sizeof(int); }
    size_t RecommendSize(const size_t n0) {
        size_t cap = mCap;
        const size_t m = max_size();
        size_t a;
        size_t b;
        size_t n = n0;
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
    static int* Allocate(size_t n) {
        if (n > max_size()) {
            LengthError();
        }
        int* p = (int*)operator new(n * sizeof(int));
        if (p == 0) {
            AllocError();
        }
        return p;
    }
    /* Moves the old contents behind the new elements in buf, then swaps. */
    void Commit(SplitBuf& buf) {
        int* last = mData + mSize;
        int* first = mData;
        size_t n = (int*)last - (int*)first;
        buf.mRemaining -= n;
        memcpy(buf.mFirst + buf.mRemaining, first, n * sizeof(int));
        memset(first, 0, n * sizeof(int));
        buf.mCount += n;
        mSize = 0;
        u32 c = mCap;
        mCap = buf.mCap;
        buf.mCap = c;
        int* d = mData;
        mData = buf.mFirst;
        buf.mFirst = d;
        mSize = buf.mCount;
        buf.mCount = 0;
    }

    int* mData;
    u32 mSize;
    u32 mCap;
};

template <int N>
struct IntVecB : IntVecA<N> {
    ~IntVecB() {}
};
template <int N>
struct IntVecC : IntVecB<N> {
    ~IntVecC() {}
};

struct IntVec : IntVecC<0> {
    IntVec();
    ~IntVec();
    void push_back(const int& x); /* fn_18_1D4A0 */
};

struct IntVec2 : IntVecC<1> {
    IntVec2();
    ~IntVec2();
};

void IntVec::push_back(const int& x) {
    size_t sz = mSize;
    if (sz < mCap) {
        mSize = sz + 1;
        int* p = mData + (mSize - 1);
        *p = x;
        return;
    }
    RecommendSize(1);
    SplitBuf buf(&mCap);
    size_t n = mSize + 1 - mCap;
    size_t newcap = RecommendSize(n);
    buf.mFirst = Allocate(newcap);
    buf.mCap = newcap;
    buf.mRemaining = mSize;
    int* end = buf.mFirst + mSize;
    end[buf.mCount] = x;
    ++buf.mCount;
    Commit(buf);
}

struct ZeroWord {
    ZeroWord();
    int mValue;
};

ZeroWord::ZeroWord() : mValue(0) {}

IntVec::IntVec() {}

IntVec::~IntVec() {}

IntVec2::IntVec2() {}

IntVec2::~IntVec2() {}
