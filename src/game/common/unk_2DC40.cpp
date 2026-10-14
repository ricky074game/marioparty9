/* Shared by most REL modules. Reference copy: mg9101 (fn_18_2DC40). */
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

/* ---------------------------------------------------------------------- */
/* vector<int> (Metrowerks layout: data, size, capacity)                   */

template <class T>
inline void Swap(T& a, T& b) {
    T t = a;
    a = b;
    b = t;
}

inline size_t MaxSize() {
    return (size_t)-1 / sizeof(int);
}

struct BufferBase {
    int* mData;
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
        if (cap > MaxSize()) {
            LengthError();
        }
        int* p = (int*)operator new(cap * sizeof(int));
        if (p == 0) {
            fprintf(&__files[2], "Memory allocation failure");
            abort();
        }
        mData = p;
        mCap = cap;
    }
    int* begin() { return mData + mIndex; }
    void push_back(const int& x) {
        begin()[mCount] = x;
        ++mCount;
    }
};

class IntVec {
public:
    IntVec() : mData(0), mSize(0), mCap(0) {}

    int* begin() { return mData; }
    int* end() { return mData + mSize; }
    void SwapOut(SplitBuffer& b) {
        int* first = begin();
        size_t n = (int*)end() - (int*)first;
        b.mIndex -= n;
        memcpy(b.begin(), first, n * sizeof(int));
        memset(first, 0, n * sizeof(int));
        b.mCount += n;
        mSize = 0;
        Swap(mCap, b.mCap);
        Swap(mData, b.mData);
        Swap(mSize, b.mCount);
    }
    void push_back(const int& x) {
        u32 size = mSize;
        u32 cap = mCap;
        if (size < cap) {
            u32 k = size + 1;
            mSize = k;
            mData[(int)k - 1] = x;
            return;
        }
        if (MaxSize() - cap < 1) {
            LengthError();
        }
        SplitBuffer buf(&mCap);
        size_t n = mSize + 1 - mCap;
        size_t b;
        size_t a;
        /* Metrowerks __recommend(): grow by 60% / 50% / to max_size */
        size_t c = mCap;
        const size_t m = MaxSize();
        if (n > m - c) {
            LengthError();
        }
        size_t newcap;
        if (c < m / 3) {
            a = 3 * (c + 1) / 5;
            newcap = c + *((a < n) ? &n : &a);
        } else if (c < m / 3 * 2) {
            b = (c + 1) / 2;
            newcap = c + *((b < n) ? &n : &b);
        } else {
            newcap = m;
        }
        buf.Allocate(newcap);
        buf.mIndex = mSize;
        buf.push_back(x);
        SwapOut(buf);
    }

    int* mData;
    u32 mSize;
    u32 mCap;
};

struct PlayerIter {
    int i;
    PlayerIter() : i(0) {}
};

/* Converts the player bits in the upper half of a mask into a list of
   player indices. */
IntVec GetPlayerList(u32 mask) {
    IntVec result;
    u32 bits = mask >> 16;
    PlayerIter it;
    for (; it.i < 4; it.i++) {
        if (bits & (1 << it.i)) {
            result.push_back(it.i);
        }
    }
    return result;
}

/* ---------------------------------------------------------------------- */
/* Layout group state machine                                              */

struct Object;
bool fn_80042D70(Object* obj);

class Handle {
public:
    Handle(Object* obj) : mObj(fn_80042D70(obj) ? obj : 0) {}
    Handle(const Handle& other) : mObj(fn_80042D70(other.mObj) ? other.mObj : 0) {}
    virtual ~Handle() {}

    Object* mObj;
};

f32 fn_8004A500(Handle h);
f32 fn_8004A460(Handle h);
int fn_8004A220(Handle h);
void fn_8004A060(Handle h);
void fn_8004A410(Handle h, f32 frame);
void fn_8004A4B0(Handle h, f32 frame);
void fn_8004A3C0(Handle h, int loop);
void fn_80049DC0(Handle h, int a, int b);
void fn_8007CD10(const char* name, f32 a, f32 b, f32 c);

struct Pane {
    u8 pad[0xBB];
    u8 flags;
};

/* main.dol string-like value */
struct NameStr {
    NameStr(const NameStr&);
    ~NameStr();
    int IsEmpty() const;
    int IsReady() const;
};

/* main.dol layout wrapper; owns a list of animation handles */
class Layout {
public:
    Handle Get(int idx) {
        if (mCount == 0) {
            return Handle(0);
        }
        return mAnims[idx];
    }
    void Restart(int idx) {
        const Handle& h = Get(idx);
        fn_8004A4B0(h, 0.0f);
        fn_80049DC0(h, 1, 1);
    }
    void SetLoop(int idx, int loop) {
        fn_8004A3C0(Get(idx), loop);
    }
    void SetFlag(u8 v);
    Pane* GetPane();                         /* fn_800486E0 */
    void SetPriority(int priority);          /* fn_80048860 */
    void Init(NameStr name, int a, int b);   /* fn_80047E60 */

    int unk0;
    Handle* mAnims;
    int mCount;
    u8 pad[4];
    virtual ~Layout();
};

inline void Layout::SetFlag(u8 v) {
    if (GetPane()) {
        Pane* pane = GetPane();
        pane->flags = (pane->flags & 0xFE) | v;
    }
}

template <class T>
struct Vector {
    u32 size() const { return mSize; }

    T* mData;
    u32 mSize;
    u32 mCapacity;
};

struct PtrVecA : Vector<Layout*> {
    ~PtrVecA() {
        if (mData != 0) {
            mSize -= (int)mSize;
            operator delete(mData);
        }
    }
};
struct PtrVecB : PtrVecA {
    ~PtrVecB() {}
};
struct PtrVecC : PtrVecB {
    ~PtrVecC() {}
};
struct PtrVec : PtrVecC {
    ~PtrVec() {}
};

struct GroupData {
    int mState;
    Vector<Layout*> mObjs;
    int mPriority;
};

class LayoutGroup : public GroupData {
public:
    virtual ~LayoutGroup() {
        Layout** end = mObjs.mData + mObjs.mSize;
        for (Layout** p = mObjs.mData; p != end; p++) {
            delete *p;
            *p = 0;
        }
        ((PtrVec*)&mObjs)->~PtrVec();
    }
    virtual void SetPos(f32 x, f32 y);
    virtual void SetPos(const f32* xy);
    virtual void SetPriority(int priority);
    virtual void OnPause(bool pause);
    virtual void v5();
    virtual void v6();
    virtual void OnDone();
    virtual void v8();
    virtual void v9();

    int m18;
    int m1C;
};

class TimedLayoutGroup : public LayoutGroup {
public:
    bool IsNameReady() {
        bool ready = false;
        if (!mName.IsEmpty()) {
            if (mName.IsReady()) {
                ready = true;
            }
        }
        return ready;
    }
    void Update();                 /* fn_18_2DF20 */
    void SetFlags(int flags);      /* fn_18_2E3E0 */
    void SetPaused(u32 paused);    /* fn_18_2E480 */

    int mMode;
    int m24;
    u8 mPaused;
    f32 mFrame;
    f32 mTimer;
    u8 pad34[4];
    NameStr mName;
};

class NumberDisplay : public LayoutGroup {
public:
    virtual ~NumberDisplay();      /* fn_18_2E610 */
    void Start();                  /* fn_18_2E6F0 */
};

/* The original translation unit starts earlier; its float constants precede
   ours in the constant pool. Reproduce the pool layout. */
__declspec(weak) void PoolFloatsA(volatile int* q) {
    volatile f32 f;
    f = 0.0f;
    f = (f32)*q;
}

__declspec(weak) void PoolFloatsB() {
    volatile f32 f;
    f = 9999999.0f;
    f = -9999999.0f;
    f = 1.0f;
    f = 8.40779e-40f;
    f = 8.40779e-41f;
    f = 0.5f;
    f = -74.0f;
    f = -124.0f;
}

void TimedLayoutGroup::Update() {
    if (mState == 4) {
        if (mPaused == 0) {
            switch (mMode) {
            case 1: {
                f32 frame = fn_8004A500(mObjs.mData[0]->Get(0));
                if (frame >= 0.0f && mTimer == 0.0f) {
                    fn_8007CD10("SEQ_SEMG0000_12", 0.0f, 1.0f, 0.0f);
                    mTimer += 1.0f;
                } else if (frame >= 60.0f && mTimer == 1.0f) {
                    fn_8007CD10("SEQ_SEMG0000_12", 0.0f, 1.0f, 0.0f);
                    mTimer += 1.0f;
                } else if (frame >= 120.0f && mTimer == 2.0f) {
                    fn_8007CD10("SEQ_SEMG0000_12", 0.0f, 1.0f, 0.0f);
                    mTimer += 1.0f;
                } else if (frame >= 180.0f && mTimer == 3.0f) {
                    fn_8007CD10("SEQ_VOI_SYSTEM_001", 0.0f, 1.0f, 0.0f);
                    mTimer += 1.0f;
                }
                break;
            }
            case 4: {
                f32 frame = fn_8004A500(mObjs.mData[0]->Get(0));
                f32 t = mTimer;
                if (t < 10.0f && frame >= 10.0f + (f32)(60.0f * t)) {
                    fn_8007CD10("SEQ_SEMG0000_12", 0.0f, 1.0f, 0.0f);
                    mTimer += 1.0f;
                }
                break;
            }
            }
            if (fn_8004A220(mObjs.mData[0]->Get(0))) {
                fn_8004A060(mObjs.mData[0]->Get(0));
                int mode = mMode;
                if ((u32)(mode - 6) <= 11 || mode == 0x13 || mode == 0x1C || mode == 0x21) {
                    mState = 6;
                } else {
                    Layout* l = mObjs.mData[0];
                    l->SetFlag(0);
                    OnDone();
                }
            }
        }
    } else if (mState == 7) {
        if (IsNameReady()) {
            mObjs.mData[0]->Init(mName, m24, 0);
            Layout* l = mObjs.mData[0];
            l->SetFlag(0);
            mObjs.mData[0]->SetPriority(mPriority);
            mState = 1;
        }
    }
}

void TimedLayoutGroup::SetFlags(int flags) {
    for (Layout** p = mObjs.mData; p != mObjs.mData + mObjs.mSize; ++p) {
        (*p)->SetFlag(flags);
    }
}

void TimedLayoutGroup::SetPaused(u32 paused) {
    if (mPaused == paused) {
        return;
    }
    if (mState == 0 || mState == 1 || mState == 6) {
        return;
    }
    OnPause(paused == 0);
    f32 frame;
    if (paused) {
        mFrame = fn_8004A460(mObjs.mData[0]->Get(0));
        frame = 0.0f;
    } else {
        frame = mFrame;
    }
    fn_8004A410(mObjs.mData[0]->Get(0), frame);
    mPaused = paused;
}

NumberDisplay::~NumberDisplay() {}

void NumberDisplay::Start() {
    if (mState >= 1) {
        mObjs.mData[0]->Restart(0);
        mObjs.mData[0]->SetFlag(1);
        mObjs.mData[0]->SetLoop(0, 0);
        mState = 2;
    }
}
