/* Shared by most REL modules. Reference copy: mg9101 (fn_18_2BD40). */
#include "types.h"

typedef unsigned long size_t;

struct FILE {
    u8 data[0x50];
};

extern "C" FILE __files[];
extern "C" int fprintf(FILE* stream, const char* format, ...);
extern "C" void abort(void);
extern "C" size_t strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, size_t n);
extern "C" void* memset(void* dst, int c, size_t n);
void* operator new(size_t);
void operator delete(void*);

inline void MslError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

inline void LengthError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

template <class T>
inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

/* ---------------------------------------------------------------------- */
/* Metrowerks vector<T*> push_back                                         */

template <class T>
struct SplitBase {
    SplitBase() : mFirst(0), mCount(0), mCap(0) {}
    ~SplitBase() {
        if (mFirst != 0) {
            mCount = 0;
            operator delete(mFirst);
        }
    }

    T* mFirst;
    u32 mCount;
    u32 mCap;
};

template <class T>
struct SplitBuf : SplitBase<T> {
    SplitBuf(u32* capRef) : mCapRef(capRef), mRemaining(0) {}

    u32* mCapRef;
    u32 mRemaining;
};

template <class T>
struct Vector {
    static size_t max_size() { return (size_t)-1 / sizeof(T); }
    size_t RecommendSize(const size_t n0, const char* msg) {
        size_t cap = mCapacity;
        const size_t m = max_size();
        size_t n = n0;
        size_t b;
        size_t a;
        if (n > m - cap) {
            LengthError(msg);
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
    static T* Allocate(size_t n, const char* msg) {
        if (n > max_size()) {
            LengthError(msg);
        }
        T* p = (T*)operator new(n * sizeof(T));
        if (p == 0) {
            MslError("Memory allocation failure");
        }
        return p;
    }

    T* mData;
    u32 mSize;
    u32 mCapacity;
};

template <class T>
inline void CopyBackward(T** first, T** last, T** dst, SplitBuf<T*>& buf) {
    size_t n = last - first;
    buf.mRemaining -= n;
    memcpy(buf.mFirst + buf.mRemaining, first, n * sizeof(T*));
    memset(first, 0, n * sizeof(T*));
    buf.mCount += n;
}

template <class T>
inline void PushBack(Vector<T>& v, const T& x, const char* msg) {
    Vector<T>* self = &v;
    if (self->mSize < self->mCapacity) {
        ++self->mSize;
        T* p = self->mData + (self->mSize - 1);
        *p = x;
        return;
    }
    self->RecommendSize(1, msg);
    SplitBuf<T> buf(&self->mCapacity);
    size_t n = self->mSize + 1 - self->mCapacity;
    size_t newcap = self->RecommendSize(n, msg);
    buf.mFirst = Vector<T>::Allocate(newcap, msg);
    buf.mCap = newcap;
    buf.mRemaining = self->mSize;
    T* end = buf.mFirst + self->mSize;
    end[buf.mCount] = x;
    ++buf.mCount;
    CopyBackward(self->mData, self->mData + self->mSize, buf.mFirst + buf.mRemaining, buf);
    self->mSize = 0;
    u32 c = self->mCapacity;
    self->mCapacity = buf.mCap;
    buf.mCap = c;
    T* d = self->mData;
    self->mData = buf.mFirst;
    buf.mFirst = d;
    self->mSize = buf.mCount;
    buf.mCount = 0;
}

/* ---------------------------------------------------------------------- */
/* main.dol types                                                          */

class ProcBase {
public:
    ProcBase();          /* fn_80071350 */
    virtual ~ProcBase(); /* fn_80071420 */
    virtual void Run() = 0;

    int mUnk4;
};

struct Pane {
    u8 pad[0xBB];
    u8 flags;
};

/* Layout wrapper (ctor fn_80046000) */
class Layout {
public:
    Layout();
    int m0;
    int m4;
    int m8;
    int mC;
};

class LayoutObj : public Layout {
public:
    LayoutObj() {
        m4 = 0;
        m8 = 0;
        mC = 0;
    }
    virtual ~LayoutObj();

    void Create(const char* res, const char* name, int a, int b); /* fn_800475D0 */
    Pane* GetPaneMgr();                                           /* fn_800486E0 */
    void SetPriority(int);                                        /* fn_80048860 */

};

/* ---------------------------------------------------------------------- */
/* Strings                                                                 */

struct Allocator {
    Allocator() {}
};

class String {
public:
    String(const char* s) {
        size_t* p = mWords;
        for (int i = 0; i < 3; i++) {
            *p++ = 0;
        }
        size_t len = strlen(s);
        reserve(len);
        Replace(0, 0, s, s + len, Allocator());
    }
    ~String() {
        if (L().is_long) {
            delete L().data;
        }
    }

    struct Long {
        unsigned long is_long : 1;
        unsigned long cap : 31;
        size_t size;
        char* data;
    };
    struct Short {
        unsigned char is_long : 1;
        unsigned char size : 7;
        char data[11];
    };

    const Long& L() const { return *(const Long*)this; }
    const Short& S() const { return *(const Short*)this; }
    size_t size() const {
        bool l = L().is_long;
        return !l ? S().size : L().size;
    }
    const char* c_str() const {
        bool l = L().is_long;
        return !l ? S().data : L().data;
    }

    void reserve(size_t n);                                                                   /* fn_18_15E80 */
    void Replace(size_t pos, size_t n, const char* first, const char* last, Allocator alloc); /* fn_18_16040 */

    union {
        Long mLong;
        Short mShort;
        size_t mWords[3];
    };
};

String operator+(const char* lhs, const String& rhs); /* fn_18_2A400 */

struct Key {
    int v;
};
Key MakeKey(const String& s); /* fn_800882E0 */

struct XObj {
    int mVal;
    XObj() : mVal(0) {}
    XObj(const Key& k, int flag); /* fn_80088640 */
    ~XObj();                      /* fn_80086F10 */
    void Copy(const XObj& o);     /* fn_80086B90 */
};

/* ---------------------------------------------------------------------- */
/* Layout display                                                          */

template <class T>
class ItemBuf {
public:
    void Init() {
        mData = 0;
        mSize = 0;
        mCap = 0;
    }
    ~ItemBuf() {
        if (mData) {
            mSize -= (int)mSize;
            operator delete(mData);
        }
    }
    T** mData;
    u32 mSize;
    u32 mCap;
};

template <class T>
class ItemVecImpl2 {
public:
    ItemVecImpl2() {}
    ~ItemVecImpl2() {}
    ItemBuf<T> mBuf;
};

template <class T>
class ItemVecImpl {
public:
    ItemVecImpl() {}
    ~ItemVecImpl() {}
    ItemVecImpl2<T> mImpl;
};

template <class T>
class ItemVec {
public:
    ItemVec() {}
    ~ItemVec() {}
    T** begin() { return mImpl.mImpl.mBuf.mData; }
    T** end() { return mImpl.mImpl.mBuf.mData + mImpl.mImpl.mBuf.mSize; }
    ItemVecImpl<T> mImpl;
};

struct ItemRaw {
    LayoutObj** mData;
    u32 mSize;
    u32 mCap;
};

class DisplayBase {
public:
    DisplayBase() {}

    int mState;
    ItemRaw mItems;
    int mPriority;
};

class Display : public DisplayBase {
public:
    Display() {
        mState = 0;
        mItems.mData = 0;
        mItems.mSize = 0;
        mItems.mCap = 0;
        mPriority = 0x9030;
    }
    virtual ~Display() {
        LayoutObj** it = mItems.mData;
        LayoutObj** end = mItems.mData + mItems.mSize;
        for (; it != end; ++it) {
            delete *it;
            *it = 0;
        }
        reinterpret_cast<ItemVec<LayoutObj>*>(&mItems)->~ItemVec();
    }
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
};

extern const char* kLayoutNames[];

class XDisplay : public Display, public ProcBase {
public:
    XDisplay(int type); /* fn_18_2BD40 */
    virtual ~XDisplay();
    virtual void Run();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();

    void Reset(int arg); /* fn_18_2BF00 */

    int mType;
    const char* mName;
    u8 m28;
    f32 m2C;
    u32 m30;
    u8 m34;
    XObj mArc;
};

XDisplay::XDisplay(int type)
    : mType(type), mName(0), m28(0), m2C(1.0f), m34(0), mArc() {
    mName = kLayoutNames[type];
}

XDisplay::~XDisplay() {}

void XDisplay::Reset(int arg) {
    LayoutObj* layout = new LayoutObj;
    PushBack(reinterpret_cast<Vector<LayoutObj*>&>(mItems), layout, "vector length error");
    if (arg == 0) {
        String name(mName);
        String path = "/mgsystem/layout/" + name;
        mItems.mData[0]->Create(path.c_str(), mName, 0, 0);
        Pane* pane = mItems.mData[0]->GetPaneMgr();
        if (pane) {
            pane = mItems.mData[0]->GetPaneMgr();
            pane->flags &= ~1;
        }
        mItems.mData[0]->SetPriority(mPriority);
        mState = 1;
    } else {
        String name(mName);
        String path = "/mgsystem/layout/" + name;
        const char* ext = ".arc";
        path.Replace(path.size(), 0, ext, ext + strlen(ext), Allocator());
        mArc.Copy(XObj(MakeKey(path), 0));
        mState = 7;
    }
}
