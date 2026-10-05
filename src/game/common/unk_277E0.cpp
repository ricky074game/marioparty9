/* Shared by most REL modules. Reference copy: mg9101 (fn_18_277E0). */
#include "types.h"

typedef unsigned long size_t;

struct FILE {
    u8 data[0x50];
};

extern "C" FILE __files[];
extern "C" int fprintf(FILE* stream, const char* format, ...);
extern "C" void abort(void);

inline void MslError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

template <class T>
inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

class CDeque {
public:
    size_t max_size() const { return (size_t)-1 / sizeof(void*) - 1; }
    size_t capacity() const { return mCapacity == 0 ? 0 : mCapacity - 1; }
    size_t RecommendSize(size_t n) const;

    void* mData;
    size_t mStart;
    size_t mSize;
    size_t mCapacity;
};

size_t CDeque::RecommendSize(size_t n) const {
    static const size_t m = max_size();
    size_t cap = capacity();
    if (n > m - cap) {
        MslError("cdeque length error");
    }
    if (cap < m / 3) {
        return cap + Max(3 * (cap + 1) / 5, n);
    }
    if (cap < m / 3 * 2) {
        return cap + Max((cap + 1) / 2, n);
    }
    return m;
}
