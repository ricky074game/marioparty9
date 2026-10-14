/* Shared by most REL modules. Reference copy: mg9101 (fn_18_198E0). */
#include "types.h"

typedef unsigned long size_t;

struct FILE {
    u8 data[0x50];
};

extern "C" FILE __files[];
extern "C" int fprintf(FILE* stream, const char* format, ...);
extern "C" void abort(void);

void operator delete(void*);

inline void MslError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

class noncopyable {
protected:
    noncopyable() {}
    ~noncopyable() {}
};

class Deletable {
public:
    virtual ~Deletable();
};

template <class T>
class ScopedPtr : noncopyable {
public:
    ~ScopedPtr() { delete mPtr; }
    T* mPtr;
};

/* Owner of a heap object with a virtual destructor */
class OwnedPtr {
public:
    ~OwnedPtr();
    ScopedPtr<Deletable> mBuf;
};

OwnedPtr::~OwnedPtr() {}

struct BitSetImpl {
    u32 pad[3];
    u32 bits; /* 0xC */
};

struct BitRef {
    BitRef(u32* w, u32 b) : word(w), bit(b) {}
    BitRef& operator=(bool on) {
        u32 mask = 1 << bit;
        if (on) {
            *word |= mask;
        } else {
            *word &= ~mask;
        }
        return *this;
    }
    u32* word;
    u32 bit;
};

class BitSet4 {
public:
    BitRef operator[](u32 bit) { return BitRef(&mImpl->bits, bit); }
    void Set(u32 bit, bool on);
    void SetAll(bool on);

    BitSetImpl* mImpl;
};

void BitSet4::Set(u32 bit, bool on) {
    BitRef r = (*this)[bit];
    if (bit >= 4) {
        MslError("index out of range of bitset::set");
    }
    r = on;
}

void BitSet4::SetAll(bool on) {
    BitSetImpl* i = mImpl;
    if (on) {
        i->bits = 0xF;
    } else {
        i->bits = 0;
    }
}
