/* Shared by most REL modules. Reference copy: mg9101 (fn_18_3FDC0). */
#include "game/common/unk_15E80.h"
#include "types.h"

#pragma inline_max_size(2000)

int fn_8007CD10(const char* name, f32 a, f32 b, f32 c);
int fn_8007D190(const char* name, int arg);
int fn_8007D5C0(const char* name, int mask, f32 a, f32 b, f32 c);

extern volatile u32 gSeMask;

struct SeName {
    const char* name;
    f32 value;
};

template <class T>
inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

struct SeVec3 {
    f32 x, y, z;
};

inline void* operator new(unsigned long, void* p) { return p; }

extern "C" void* fn_8020B514(unsigned long);

/* String with the same layout as String (unk_15E80.h), but with the
   constructors the original vector<pair<string, float>> needs. */
struct Str {
    Str() {
        mWords[0] = 0;
        mWords[1] = 0;
        mWords[2] = 0;
    }
    Str(const Str& o, Allocator a = Allocator()) {
        if (!o.mLong.is_long) {
            *this = o;
        } else {
            mWords[0] = 0;
            mWords[1] = 0;
            mWords[2] = 0;
            Str2().reserve(o.mLong.size);
            Str2().Replace(0, 0, o.mLong.data, o.mLong.data + o.mLong.size, a);
        }
    }
    ~Str() {
        if (mLong.is_long) {
            delete mLong.data;
        }
    }

    String& Str2() { return *(String*)this; }
    const String::Long& L() const { return *(const String::Long*)this; }
    const String::Short& S() const { return *(const String::Short*)this; }
    size_t size() const {
        bool l = L().is_long;
        return !l ? S().size : L().size;
    }
    void assign(const char* s) {
        size_t sz = size();
        Allocator a;
        Str2().Replace(0, sz, s, s + strlen(s), a);
    }
    int compare(const char* s) const {
        size_t n1;
        size_t len2;
        size_t n2;
        n2 = strlen(s);
        len2 = n2;
        n1 = size();
        size_t tmp;
        const char* p = Data(tmp);
        size_t sz = tmp;
        size_t rlen = Min(sz, n1);
        int result = memcmp(p, s, Min(rlen, len2));
        if (result == 0) {
            if (rlen < n2) {
                return -1;
            }
            if (rlen == n2) {
                return 0;
            }
            return 1;
        }
        return result;
    }
    const char* Data(size_t& n) const {
        const char* p;
        bool l = L().is_long;
        if (!l) {
            p = S().data;
            n = S().size;
        } else {
            p = L().data;
            n = L().size;
        }
        return p;
    }
    void clear() {
        if (!mLong.is_long) {
            mShort.data[0] = 0;
            mShort.size = 0;
        } else {
            mLong.data[0] = 0;
            mLong.size = 0;
        }
    }

    union {
        String::Long mLong;
        String::Short mShort;
        size_t mWords[3];
    };
};

struct Entry {
    Str name;
    f32 value;
};

inline void VecLenError() { MslError("vector length error"); }
inline size_t MaxSize() { return (size_t)-1 / sizeof(Entry); }

#define RECOMMEND_SIZE(out, capv, nv)                  \
    {                                                  \
        const size_t m_ = MaxSize();                   \
        const size_t cap_ = (capv);                    \
        const size_t n_ = (nv);                        \
        if (n_ > m_ - cap_) {                          \
            VecLenError();                      \
        }                                              \
        if (cap_ < m_ / 3) {                           \
            out = cap_ + Max(3 * (cap_ + 1) / 5, n_);  \
        } else if (cap_ < m_ / 3 * 2) {                \
            out = cap_ + Max((cap_ + 1) / 2, n_);      \
        } else {                                       \
            out = m_;                                  \
        }                                              \
    }

#define ALLOC_ENTRIES(out, nv)                           \
    {                                                    \
        const size_t n2_ = (nv);                         \
        if (n2_ > MaxSize()) {                           \
            VecLenError();                        \
        }                                                \
        out = (Entry*)fn_8020B514(n2_ * sizeof(Entry));  \
        if (out == 0) {                                  \
            MslError("Memory allocation failure");       \
        }                                                \
    }

inline void DestroyEntry(Entry* e) {
    e->~Entry();
}

/* Reallocation buffer: elements are filled backwards from mIndex */
struct EntryBufBase {
    Entry* mBegin;
    u32 mCount;

    ~EntryBufBase() {
        if (mBegin != 0) {
            mCount = 0;
            Entry* p = mBegin + mCount;
            for (u32 i = mCount; i != 0; i--) {
                DestroyEntry(--p);
            }
            operator delete(mBegin);
        }
    }
};

struct EntryBuf : EntryBufBase {
    u32 mCap;
    u32* mVecCap;
    u32 mIndex;

    EntryBuf(u32* vecCap) : mCap(0), mVecCap(vecCap), mIndex(0) {
        mBegin = 0;
        mCount = 0;
    }
};

struct EntryVec {
    Entry* mData;
    u32 mSize;
    u32 mCap;

};

inline void Place(Entry* p, const Entry& x) {
    new (p) Entry(x);
}


/* Sound effect controller (main.dol sound functions fn_8007xxxx) */
class SeCtrl {
public:
    virtual ~SeCtrl();
    virtual void vf0C();
    virtual void vf10();
    virtual int vf14(int);
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual int vf34(f32);
    virtual void vf38();
    virtual int vf3C(f32);
    virtual int vf40(int, f32, f32, f32);
    virtual int vf44(int, f32, f32, const SeVec3*);
    virtual int vf48(int, f32, f32, f32);
    virtual int vf4C(int, f32, f32, const SeVec3*);
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C();
    virtual void vf60();
    virtual void vf64();
    virtual void vf68();
    virtual int vf6C();
    virtual void vf70();
    virtual void vf74();

    void SetMode(int mode);
    void SetF20(f32 v);
    void SetF24(f32 v);
    void SetF1C(f32 v);
    void SetVec(const SeVec3* v);
    int Setup(u8 a, u8 b, u8 c, int d);
    void SetE(u8 v);
    void SetD(u8 v);
    void SetIndex(int v);
    int Call34();
    int Start(int h, f32 x);
    int Flush();
    void ClearPending();
    int Call3C();
    int Start2(int h, f32 x);
    int Start3(f32 a, f32 b, f32 c);
    int Start4(int h, f32 a, f32 b, const SeVec3* v);
    int PlayName(const char* name, f32 a, f32 b, f32 c);
    int StopName(const char* name, int arg);
    const char* GetName(int i);
    void SetFlag(int idx, int set);
    int PlayFlag(const char* name, f32 a, f32 b);

    // 0x04
    int mUnk4;
    int mType;     // 0x08
    u8 mEnabled;   // 0x0C
    u8 mMute;      // 0x0D
    u8 mUnkE;      // 0x0E
    u8 mUnkF;      // 0x0F
    SeVec3 mVec;   // 0x10
    f32 m1C;       // 0x1C
    f32 m20;       // 0x20
    f32 mVol;      // 0x24
    u8 pad28[0x38 - 0x28];
    int mIndex;    // 0x38
    String mNames[29]; // 0x3C
    String mGroup[3][14]; // 0x198
    u32 mFlags;    // 0x390
    EntryVec mVecs[2]; // 0x394
    u8 mPendingEnable; // 0x3AC
    u8 mPending;       // 0x3AD
    int mPendH;        // 0x3B0
    f32 mPendX;        // 0x3B4
};

inline void PushBack(EntryVec& self, const Entry& x) {
    u32* cp = &self.mCap;
    if (self.mSize < *cp) {
        Place(self.mData + self.mSize, x);
        self.mSize++;
        return;
    }
    {
        size_t unused;
        RECOMMEND_SIZE(unused, *cp, 1);
        EntryBuf buf(cp);
        size_t cap = *cp;
        size_t newcap;
        RECOMMEND_SIZE(newcap, cap, self.mSize + 1 - cap);
        Entry* mem;
        ALLOC_ENTRIES(mem, newcap);
        buf.mCap = newcap;
        buf.mBegin = mem;
        buf.mIndex = self.mSize;
        Place(mem + buf.mIndex + buf.mCount, x);
        buf.mCount++;
        Entry* begin = self.mData;
        Entry* src = self.mData + self.mSize;
        Entry* dst = buf.mBegin + buf.mIndex;
        while (src > begin) {
            --dst;
            --src;
            Place(dst, *src);
            buf.mIndex--;
            buf.mCount++;
        }
        u32 oldCap = *cp;
        *cp = buf.mCap;
        Entry* oldData = self.mData;
        u32 oldSize = self.mSize;
        self.mData = buf.mBegin;
        buf.mCap = oldCap;
        buf.mBegin = oldData;
        self.mSize = buf.mCount;
        buf.mCount = oldSize;
        Entry* first = oldData + buf.mIndex;
        Entry* last = first + oldSize;
        while (last > first) {
            DestroyEntry(--last);
        }
        buf.mCount = 0;
    }
}

inline bool Valid2(const SeName* p) { if (p->name == 0) { return false; } return true; }
void fn_18_3FDC0(SeCtrl* c, int idx, const SeName* tbl) {
    EntryVec* vec = &c->mVecs[idx];
    for (const SeName* p = tbl; Valid2(p); p++) {
        Entry e;
        e.name.assign(p->name);
        if (e.name.compare("STEP") == 0) {
            e.name.clear();
        }
        e.value = p->value;
        PushBack(*vec, e);
    }
}

void SeCtrl::SetMode(int mode) { mType = mode; }
void SeCtrl::SetF20(f32 v) { m20 = v; }
void SeCtrl::SetF24(f32 v) { mVol = v; }
void SeCtrl::SetF1C(f32 v) { m1C = v; }
void SeCtrl::SetVec(const SeVec3* v) {
    mVec.x = v->x;
    mVec.y = v->y;
    mVec.z = v->z;
}
int SeCtrl::Setup(u8 a, u8 b, u8 c, int d) {
    mEnabled = a;
    mUnkF = b;
    mPendingEnable = c;
    return vf14(d);
}
void SeCtrl::SetE(u8 v) { mUnkE = v; }
void SeCtrl::SetD(u8 v) { mMute = v; }
void SeCtrl::SetIndex(int v) { mIndex = v; }
int SeCtrl::Call34() { return vf34(m20); }

int SeCtrl::Start(int h, f32 x) {
    if (mPendingEnable && h <= 2) {
        mPending = 1;
        mPendH = h;
        mPendX = x;
        return -1;
    }
    if (mMute) {
        return -1;
    }
    switch (mType) {
    case 1:
        return vf40(h, 0.0f, x, 0.0f);
    case 2:
        return vf44(h, 0.0f, x, &mVec);
    case 3:
    case 4:
        return vf40(h, 0.0f, x, m1C);
    default:
        return -1;
    }
}

int SeCtrl::Flush() {
    if (mPendingEnable && mPending) {
        vf74();
        mPending = 0;
        f32 x = mPendX;
        int h = mPendH;
        if (mMute) {
            return -1;
        }
        switch (mType) {
        case 1:
            return vf40(h, 0.0f, x, 0.0f);
        case 2:
            return vf44(h, 0.0f, x, &mVec);
        case 3:
        case 4:
            return vf40(h, 0.0f, x, m1C);
        default:
            return -1;
        }
    }
    return -1;
}

void SeCtrl::ClearPending() { mPending = 0; }
int SeCtrl::Call3C() { return vf3C(m20); }

int SeCtrl::Start2(int h, f32 x) {
    if (mMute) {
        return -1;
    }
    switch (mType) {
    case 1:
        return vf48(h, 0.0f, x, 0.0f);
    case 2:
        return vf4C(h, 0.0f, x, &mVec);
    case 3:
    case 4:
        return vf48(h, 0.0f, x, m1C);
    default:
        return -1;
    }
}

int SeCtrl::Start3(f32 a, f32 b, f32 c) {
    return vf48(vf6C(), a, b, c);
}

int SeCtrl::Start4(int h, f32 a, f32 b, const SeVec3* v) {
    return vf4C(vf6C(), a, b * mVol, v);
}

int SeCtrl::PlayName(const char* name, f32 a, f32 b, f32 c) {
    if (!mEnabled) {
        return -1;
    }
    return fn_8007CD10(name, a, b * mVol, c);
}

int SeCtrl::StopName(const char* name, int arg) {
    if (!mEnabled) {
        return -1;
    }
    return fn_8007D190(name, arg);
}

static inline const char* CStr(const String& s) {
    return !s.L().is_long ? s.S().data : s.L().data;
}

const char* SeCtrl::GetName(int i) {
    switch (i) {
    case 0:
        return CStr(mGroup[0][mIndex]);
    case 1:
        return CStr(mGroup[1][mIndex]);
    case 2:
        return CStr(mGroup[2][mIndex]);
    default:
        return CStr(mNames[i - 3]);
    }
}

void SeCtrl::SetFlag(int idx, int set) {
    if (idx == -1) {
        return;
    }
    int tbl[4] = {2, 4, 8, 16};
    if ((u32)idx >= 4) {
        idx = -1;
    } else {
        idx = tbl[idx];
    }
    if (idx < 0) {
        return;
    }
    mFlags = idx;
    u32 m;
    if (set) {
        m = gSeMask;
        m |= idx;
    } else {
        m = gSeMask;
        m &= ~idx;
    }
    gSeMask = m;
}

int SeCtrl::PlayFlag(const char* name, f32 a, f32 b) {
    int m = mFlags & gSeMask;
    if (m == 0) {
        return -1;
    }
    return fn_8007D5C0(name, m, a, b, 0.0f);
}
