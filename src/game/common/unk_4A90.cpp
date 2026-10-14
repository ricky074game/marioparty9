/* Shared by most REL modules. Reference copy: mg9101 (fn_18_4A90). */
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

template <class T>
inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

/* Iterator wrapper (single pointer) */
struct Iter {
    int** mPtr;
    Iter(const Iter& o);
};

template <class T>
inline void Swap(T& a, T& b) {
    T t = a;
    a = b;
    b = t;
}

inline size_t MaxSize() {
    return (size_t)-1 / sizeof(int*);
}

__declspec(weak) void CheckLength(bool bad) {
    if (bad) {
        MslError("vector length error");
    }
}

inline void CheckCapacity(size_t cap) {
    CheckLength(cap > MaxSize());
}

struct BufferBase {
    int** mData;
    u32 mCount;
    u32 mCap;
    u32* mCapRef;

    ~BufferBase() {
        if (mData) {
            mCount -= (int)mCount;
            operator delete(mData);
        }
    }
};

/* Temporary storage used while growing the vector */
struct SplitBuffer : BufferBase {
    u32 mIndex;

    SplitBuffer(u32* capRef) {
        mData = 0;
        mCount = 0;
        mCap = 0;
        mCapRef = capRef;
        mIndex = 0;
    }
    ~SplitBuffer() {}

    void Allocate(size_t cap) {
        CheckCapacity(cap);
        int** p = (int**)operator new(cap * sizeof(int*));
        if (p == 0) {
            MslError("Memory allocation failure");
        }
        mData = p;
        mCap = cap;
    }
    int** begin() { return mData + mIndex; }
    void push_back(int* const& x) {
        begin()[mCount] = x;
        ++mCount;
    }
};

/* vector<T*> */
class PtrVector {
public:
    PtrVector();
    void push_back(int* const& x);
    int** At(u32 i);

    size_t RecommendSize(size_t cap, const size_t n) const {
        const size_t m = MaxSize();
        CheckLength(n > m - cap);
        if (cap < m / 3) {
            return cap + Max(3 * (cap + 1) / 5, n);
        }
        if (cap < m / 3 * 2) {
            return cap + Max((cap + 1) / 2, n);
        }
        return m;
    }
    int** begin() { return mData; }
    int** end() { return mData + mSize; }
    void SwapOut(SplitBuffer& b) {
        int** first = begin();
        size_t n = (int*)end() - (int*)first;
        b.mIndex -= n;
        memcpy(b.begin(), first, n * sizeof(int*));
        memset(first, 0, n * sizeof(int*));
        b.mCount += n;
        mSize = 0;
        Swap(mCap, b.mCap);
        Swap(mData, b.mData);
        Swap(mSize, b.mCount);
    }

    int** mData;
    u32 mSize;
    u32 mCap;
};

PtrVector::PtrVector() : mData(0), mSize(0), mCap(0) {}

Iter::Iter(const Iter& o) : mPtr(o.mPtr) {}

void PtrVector::push_back(int* const& x) {
    u32 size = mSize;
    u32 cap = mCap;
    if (size < cap) {
        u32 n = size + 1;
        mSize = n;
        mData[(int)n - 1] = x;
        return;
    }
    CheckLength(MaxSize() - mCap < 1);
    SplitBuffer buf(&mCap);
    size_t n = mSize + 1 - mCap;
    buf.Allocate(RecommendSize(mCap, n));
    buf.mIndex = mSize;
    buf.push_back(x);
    SwapOut(buf);
}

int** PtrVector::At(u32 i) {
    return mData + i;
}
