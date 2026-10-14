/* Shared by most REL modules. Reference copy: mg9101 (fn_18_1C640). */
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
extern "C" void* memmove(void* dst, const void* src, size_t n);
extern "C" void* __register_global_object(void* object, void* destructor, void* regmem);

void* operator new(size_t);
void operator delete(void*);

inline void MslError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

inline void LengthError() {
    MslError("vector length error");
}

inline void AllocError() {
    MslError("Memory allocation failure");
}

template <class T>
inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

/* main.dol helpers */
struct Anim48 {
    u32 x0;
    void SetRect(int, int, int, int); /* fn_80041550 */
    void Set1(int);                   /* fn_800419D0 */
    void Init(int);                   /* fn_80041A10 */
    void Set3(int);                   /* fn_80041770 */
};

struct GameState {
    u8 pad[0x6BC4];
    Anim48 mTimer;
};
GameState* GetGameState(); /* fn_18_1BE30 */

int fn_8004EF20();
void fn_8004EF10(int);

/* ---------------------------------------------------------------------- */
/* vector<TimedObj*>                                                       */

class TimedObj;

/* Scratch buffer used while growing a vector. */
struct SplitBufferBase {
    SplitBufferBase() : first(0), count(0), cap(0) {}
    TimedObj** first;
    u32 count;
    u32 cap;

    ~SplitBufferBase() {
        if (first != 0) {
            count = 0;
            operator delete(first);
        }
    }
};

struct SplitBuffer : SplitBufferBase {
    SplitBuffer(u32* capRef) : alloc(capRef), offset(0) {}
    ~SplitBuffer() {}
    u32* alloc;
    u32 offset;
};

struct Iter {
    TimedObj** p;
    Iter(TimedObj** q) : p(q) {}
    int operator-(const Iter& o) const { return p - o.p; }
    Iter operator+(int n) const { return Iter(p + n); }
    Iter& operator++() {
        ++p;
        return *this;
    }
    bool operator!=(const Iter& o) const { return p != o.p; }
    TimedObj*& operator*() const { return *p; }
};

struct VecImpl {
    TimedObj** mData;
    u32 mSize;
    u32 mCap;

    ~VecImpl() {
        if (mData != 0) {
            mSize -= (int)mSize;
            operator delete(mData);
        }
    }
};

struct VecBase : VecImpl {
    ~VecBase() {}
};

struct ObjVec : VecBase {
    ObjVec();
    ~ObjVec();

    Iter begin() { return Iter(mData); }
    Iter end() { return Iter(mData + mSize); }
    TimedObj*& back() { return mData[mSize - 1]; }

    size_t max_size() const { return (size_t)-1 / sizeof(int); }
    size_t RecommendSize(const size_t n0) {
        size_t n = n0;
        size_t cap = mCap;
        const size_t m = max_size();
        size_t b;
        size_t a;
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
    TimedObj** Allocate(size_t n) const {
        if (n > max_size()) {
            LengthError();
        }
        TimedObj** p = (TimedObj**)operator new(n * sizeof(int));
        if (p == 0) {
            AllocError();
        }
        return p;
    }
    void Commit(SplitBuffer& buf) {
        TimedObj** old = mData;
        size_t n = end() - Iter(old);
        buf.offset -= n;
        memcpy(buf.first + buf.offset, old, n * sizeof(int));
        memset(old, 0, n * sizeof(int));
        buf.count += n;
        mSize = 0;
        u32 cap = mCap;
        mCap = buf.cap;
        buf.cap = cap;
        TimedObj** data = mData;
        mData = buf.first;
        buf.first = data;
        mSize = buf.count;
        buf.count = 0;
    }
    void push_back(TimedObj* const& x) {
        size_t sz = mSize;
        if (sz < mCap) {
            mSize = sz + 1;
            mData[mSize - 1] = x;
            return;
        }
        RecommendSize(1);
        SplitBuffer buf(&mCap);
        size_t need = mSize + 1 - mCap;
        size_t newcap = RecommendSize(need);
        buf.first = Allocate(newcap);
        buf.cap = newcap;
        buf.offset = mSize;
        (buf.first + buf.offset)[buf.count] = x;
        ++buf.count;
        Commit(buf);
    }
    void push_back2(TimedObj* const& x) {
        size_t sz = mSize;
        if (sz < mCap) {
            mSize = sz + 1;
            size_t i = mSize - 1;
            mData[i] = x;
            return;
        }
        RecommendSize(1);
        SplitBuffer buf(&mCap);
        size_t need = mSize + 1 - mCap;
        size_t newcap = RecommendSize(need);
        buf.first = Allocate(newcap);
        buf.cap = newcap;
        buf.offset = mSize;
        (buf.first + buf.offset)[buf.count] = x;
        ++buf.count;
        Commit(buf);
    }
    void erase(Iter pos) {
        memmove(pos.p, (pos + 1).p, (end() - pos - 1) * sizeof(int));
        --mSize;
    }
};

template <class It, class T>
inline It Find(It first, It last, const T& value) {
    while (first != last && !(*first == value)) {
        ++first;
    }
    return first;
}

ObjVec::ObjVec() {
    mData = 0;
    mSize = 0;
    mCap = 0;
}

ObjVec::~ObjVec() {}

/* ---------------------------------------------------------------------- */
/* Layout object with an event callback                                    */

typedef void (*LayoutCallback)(void*, int, void*);

/* main.dol layout */
struct Layout {
    Layout(const char* res, const char* name, int); /* fn_80046CF0 */
    ~Layout();                                      /* fn_80046040 */
    void SetUserData(void*);                        /* fn_8004B5D0 */
    void SetCallback(int, LayoutCallback);          /* fn_8004B5E0 */

    u32 x0;
};

class LayoutObj : public Layout {
public:
    LayoutObj(const char* res, const char* name, int id);
    virtual ~LayoutObj();
    virtual void OnEvent(void* arg) = 0;

    static void Callback(LayoutObj* self, int, void* arg);
};

void LayoutObj::Callback(LayoutObj* self, int, void* arg) {
    self->OnEvent(arg);
}

LayoutObj::LayoutObj(const char* res, const char* name, int id) : Layout(res, name, 0) {
    SetCallback(id, (LayoutCallback)Callback);
    SetUserData(this);
}

LayoutObj::~LayoutObj() {}

/* ---------------------------------------------------------------------- */

struct Ref {
    void* p;
    Ref(void* p_) : p(p_) {}
};

class ObjBase {
public:
    ObjBase(const Ref&, int, int);  /* fn_80041D60 */
    virtual ~ObjBase();      /* fn_8005CE40 */
    virtual void v1();       /* fn_80060110 */
    virtual void v2();       /* fn_80060130 */
    virtual void v3();       /* fn_8005FBA0 */
    virtual void v4();       /* fn_80065DF0 */
    virtual void v5();       /* fn_800661F0 */
    virtual void Update();
    virtual void v7() = 0;

    void SetFlags(int);      /* fn_8005D970 */
    void SetMode(int);       /* fn_8005D990 */

    u8 mBase[0x148 - 0x4];
};

class Unk158 {
public:
    Unk158() : mValue(0) {}
    ~Unk158(); /* fn_80086F10 */

    s32 mValue;
};

class Unk154 {
public:
    Unk154() : mValue(0) {}

    s32 mValue;
    Unk158 mSub;
};

class TwoSubObj : public ObjBase {
public:
    virtual ~TwoSubObj();

    Unk154 m148;
    Unk154 m150;
};

TwoSubObj::~TwoSubObj() {}

/* ---------------------------------------------------------------------- */

extern f32 lbl_18_data_1BF0[6];

static f32 sTable[7];

static void InitTable() {
    f32 e = 10.0f * lbl_18_data_1BF0[4];
    sTable[0] = 10.0f * lbl_18_data_1BF0[0];
    sTable[1] = 10.0f * lbl_18_data_1BF0[1];
    sTable[2] = 10.0f * lbl_18_data_1BF0[2];
    sTable[3] = 10.0f * lbl_18_data_1BF0[3];
    sTable[4] = e;
    sTable[5] = 5.0f * e;
    sTable[6] = 4.0f * -e;
}

__declspec(section ".ctors") static void (*const sInitTable)() = InitTable;

/* ---------------------------------------------------------------------- */

struct Elem {
    virtual ~Elem();
    int x4;
};

struct ElemVecImpl {
    Elem* mData;
    u32 mSize;
    u32 mCap;

    ~ElemVecImpl() {
        if (mData != 0) {
            u32 n = mSize;
            Elem* e = mData + n;
            mSize -= n;
            for (; n != 0; n--) {
                --e;
                e->~Elem();
            }
            operator delete(mData);
        }
    }
};

struct ElemVecBase : ElemVecImpl {
    ~ElemVecBase() {}
};

struct ElemVec : ElemVecBase {
    ~ElemVec();
};

ElemVec::~ElemVec() {}

/* storage of a vector whose destructor is registered by InitVec */
struct VecPod {
    int* mData;
    u32 mSize;
    u32 mCap;
};
static VecPod sVec;
static u8 sVecReg[12];

void fn_18_3A70(void*, short);

static void InitVec() {
    sVec.mData = 0;
    sVec.mSize = 0;
    sVec.mCap = 0;
    __register_global_object(&sVec, (void*)fn_18_3A70, sVecReg);
}

__declspec(section ".ctors") static void (*const sInitVec)() = InitVec;

/* ---------------------------------------------------------------------- */

struct CbArg {
    u32 v;
    CbArg() : v(0) {}
};
typedef void (*TimedCallback)(CbArg);

class ObjMid : public ObjBase {
public:
    ObjMid(void* owner, int a, int b) : ObjBase(Ref(owner), a, b) {}
    virtual ~ObjMid() {}
};

class TimedObj : public ObjMid {
public:
    TimedObj(CbArg arg = CbArg()) : ObjMid(this, 0, 1) {
        mActive = 0;
        mTimer = 0;
        m150 = 0;
        m151 = 0;
        mCallback = 0;
        mArg = arg;
        SetFlags(0x1000);
        SetMode(0xF);
    }
    virtual ~TimedObj();
    virtual void Update();
    virtual void v7() {}

    static TimedObj* Create(Anim48* timer) {
        TimedObj* o = new TimedObj;
        o->Start(timer);
        return o;
    }
    void Start(Anim48* timer) {
        mActive = 1;
        mTimer = timer;
        m150 = 0;
        fn_8004EF10(fn_8004EF20() | 0x1000);
    }

    u8 mActive;
    Anim48* mTimer;
    u8 m150;
    u8 m151;
    TimedCallback mCallback;
    CbArg mArg;
    u32 m15C;
};

void TimedObj::Update() {
    if (mActive) {
        mTimer->Set1(1);
        mTimer->Init(0);
        mActive = m150;
        if (!mActive) {
            m151 = 1;
        }
        if (mCallback != 0) {
            mCallback(mArg);
        }
    }
}

/* ---------------------------------------------------------------------- */
/* Manager owning all live TimedObj instances                              */

/* main.dol process base (ctor fn_80071350, dtor fn_80071420) */
class ProcBase {
public:
    ProcBase();
    virtual ~ProcBase();
    virtual void Run() = 0;

    int mUnk4;
};

template <class T>
class Singleton {
public:
    Singleton(T* inst) { sInstance = inst; }
    ~Singleton() { sInstance = 0; }

    static T* sInstance;
};

struct ObjList {
    ObjVec mObjs;
    int mRefCount;
};

class Mgr : public Singleton<Mgr>, public ProcBase, public ObjList {
public:
    Mgr() : Singleton<Mgr>(this) {
        mObjs.mData = 0;
        mObjs.mSize = 0;
        mObjs.mCap = 0;
        mRefCount = 0;
    }
    virtual void Run();
    virtual ~Mgr();
};

template <>
Mgr* Singleton<Mgr>::sInstance;

Mgr::~Mgr() {
    for (Iter it = mObjs.begin(); it != mObjs.end(); ++it) {
        delete *it;
    }
}

TimedObj::~TimedObj() {}

void Mgr::Run() {
    ObjVec dead;
    for (Iter it = mObjs.begin(); it != mObjs.end(); ++it) {
        if ((*it)->m151) {
            dead.push_back(*it);
            delete *it;
        }
    }
    while (dead.mSize != 0) {
        u32 idx = dead.mSize - 1;
        mObjs.erase(Find(mObjs.begin(), mObjs.end(), dead.mData[idx]));
        --dead.mSize;
    }
}

struct HandleData {
    Mgr* mPtr;
};

class MgrHandle : public HandleData {
public:
    MgrHandle();
    virtual ~MgrHandle();
    void Spawn();
};

MgrHandle::MgrHandle() {
    mPtr = 0;
}

MgrHandle::~MgrHandle() {
    Mgr* p = mPtr;
    if (p != 0) {
        if (--p->mRefCount == 0) {
            delete p;
        }
    }
}

extern f32 lbl_8026DAA8;
extern f32 lbl_8026DAAC;

void MgrHandle::Spawn() {
    Anim48* timer = &GetGameState()->mTimer;
    timer->SetRect((int)((lbl_8026DAA8 - lbl_8026DAA8) / 2.0f), (int)((lbl_8026DAAC - lbl_8026DAAC) / 2.0f),
                   (int)lbl_8026DAA8, (int)lbl_8026DAAC);
    timer->Set3(1);

    Anim48* timer2 = &GetGameState()->mTimer;
    TimedObj* obj = TimedObj::Create(timer2);

    Mgr* mgr = Singleton<Mgr>::sInstance;
    if (mgr == 0) {
        mgr = new Mgr;
    }
    mgr->mRefCount++;
    mPtr = mgr;
    mgr->mObjs.push_back2(obj);
}
