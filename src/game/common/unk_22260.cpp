/* Shared by most REL modules. Reference copy: mg9101 (fn_18_22260). */
#include "game/common/unk_21650.h"

inline int* Copy(int* first, int* last, int* result) {
    size_t n = last - first;
    memmove(result, first, n * sizeof(int));
    return result + n;
}

inline int* CopyBackward(int* first, int* last, int* result) {
    size_t n = last - first;
    result -= n;
    memmove(result, first, n * sizeof(int));
    return result;
}

inline int* Allocate(size_t n) {
    if (n > (size_t)-1 / sizeof(int)) {
        MslError("vector length error");
    }
    int* p = (int*)operator new(n * sizeof(int));
    if (p == 0) {
        MslError("Memory allocation failure");
    }
    return p;
}

inline int* DoInsert(IntVector* v, int* pos, const int& x) {
    size_t sz = v->mSize;
    size_t cap = v->mCapacity;
    if (sz >= cap) {
        v->RecommendSize(cap, 1);
    } else {
        int* e = v->mData + sz;
        const int* px = &x;
        if (pos <= px && px < e) {
            ++px;
        }
        ++v->mSize;
        CopyBackward(pos, e, e + 1);
        *pos = *px;
        return pos;
    }
    int* old = v->mData;
    size_t idx = pos - old;
    size_t newcap = v->RecommendSize(v->mCapacity, 1);
    v->mData = Allocate(newcap);
    v->mCapacity = newcap;
    v->mData[idx] = x;
    if (old != 0) {
        Copy(old + idx, old + v->mSize, Copy(old, old + idx, v->mData) + 1);
        operator delete(old);
    }
    ++v->mSize;
    return v->mData + idx;
}

int* IntVector::InsertImpl(int* pos, const int& x, FalseType) {
    return DoInsert(this, pos, x);
}
