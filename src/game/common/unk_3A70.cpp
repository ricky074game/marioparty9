/* Shared by most REL modules. Reference copy: mg9101 (fn_18_3A70). */
#include "types.h"

void operator delete(void*);

/* vector<T*> storage: data, size, capacity */
struct VecImpl {
    int* mData;
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

struct Vec : VecBase2 {
    ~Vec();
};

Vec::~Vec() {}
