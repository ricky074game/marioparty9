/* Shared by most REL modules. Reference copy: mg9101 (fn_18_3A510). */
#include "game/common/unk_170B0.h"
#include "game/common/unk_3A510.h"

bool Actor::IsBusy() {
    bool busy = false;
    if (m76D != 0 || m744 > 0.0f || m76F != 0 || m762 != 0) {
        busy = true;
    }
    return busy;
}

void Actor::Reset() {
    fn_18_34F70(this);
    fn_18_3E620(this);
    vf248(0.0f, 0.0f, 0.0f);
    mAnim.fn_80061D80(m75C);
    mAnim.fn_80060220(m690);
    m7B1 = 0;
    m7C0 = 0;
    m7D4 = 0;
    vf278();
    vf220();
    if (vf2B4() == 0 && vf128(1, 0) > 3) {
        fn_18_3CBC0(&mAnim);
    } else {
        vf118(1, 0);
        fn_18_36170(this, 0, 0.0f, 0.05f, 1, 0);
        fn_18_3CC90(&mAnim, 0);
        fn_18_3CBC0(&mAnim);
    }
}

void Actor::SetHeading(f32 speed, void* arg) {
    if (!vf1CC()) {
        if ((f32)m76C > 0.0f) {
            speed *= 0.5f;
        }
        vf1D4(arg);
        f32 angle = 0.0f;
        if (!GetTarget(&angle)) {
            mVel.z = 0.0f;
            mVel.x = 0.0f;
        } else {
            if (m6A4) {
                speed = m6B8;
            }
            m610 = angle;
            vf1D8(fn_18_41680(Vec3(0.0f, angle, 0.0f), speed));
        }
    }
}

void Actor::Fn_3A850(int a) {
    fn_18_36170(this, a, 0.0f, 0.05f, 1, 0);
}

void Actor::SetVelocityXZ(const Vec3* v) {
    mVel.x = v->x;
    mVel.z = v->z;
}

void Actor::UpdateVelocity() {
    if (mVel.y > 0.0f) {
        mState = 1;
    } else {
        mState = 2;
        m76F = 0;
    }
    if (m76F == 0) {
        f32 angle = 0.0f;
        f32 decel = m688 * lbl_8026E3B4;
        if (!GetTarget(&angle)) {
            Vec3 v(mVel.x, 0.0f, mVel.z);
            if (fn_80192D20(&v) < decel) {
                mVel.z = 0.0f;
                mVel.x = 0.0f;
            } else {
                fn_80192CD0(&v, &v);
                VecScale(&v, &v, -decel);
                VecAdd(&mVel, &v, &mVel);
            }
        } else if (!vf1CC()) {
            Vec3 d = fn_18_41680(Vec3(0.0f, angle, 0.0f), decel);
            mVel.x += d.x;
            mVel.z += d.z;
            Vec3 v(mVel.x, 0.0f, mVel.z);
            if (fn_80192D20(&v) > m684) {
                fn_80192CD0(&v, &v);
                VecScale(&v, &v, m684);
                mVel.x = v.x;
                mVel.z = v.z;
            }
            m610 = angle;
        }
    }
}

void Actor::CallSubVf30() {
    mSub.vf30();
}

/* Static initializer: scales the base movement parameters into a lookup table. */
extern f32 lbl_18_data_4678[5];
extern f32 lbl_18_bss_4B50[7];
extern const f32 lbl_18_rodata_11E0[];

void InitMoveTable() {
    const f32* src = lbl_18_data_4678;
    const f32* c = lbl_18_rodata_11E0;
    f32* g = lbl_18_bss_4B50;
    f32 k = c[10];
    for (int i = 0; i < 5; i++) {
        g[i] = k * *src++;
    }
    g[5] = c[24] * g[4];
    g[6] = c[25] * -g[4];
}

/* ---------------------------------------------------------------------- */

struct Object;
void fn_80042AF0(Object* obj);

/* Reference-counted handle to a main.dol object */
class Handle : noncopyable {
public:
    virtual ~Handle();
    Object* mObj;
};

Handle::~Handle() {
    fn_80042AF0(mObj);
}

/* vector<T*> storage: data, size, capacity */
struct VecImpl {
    int* mData;
    u32 mSize;
    u32 mCap;
    int* begin() { return mData; }
    int* end() { return mData + mSize; }
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
    ~Vec() {}
};

class HandleHolder {
public:
    ~HandleHolder();

    u8 pad00[0x48];
    VecBase2 mVec;   /* 0x48 */
    u8 pad54[4];
    Handle mHandle;  /* 0x58 */
};

HandleHolder::~HandleHolder() {}

/* Doubly linked list of handles (node = prev, next, value) */
struct Link {
    Link* prev;
    Link* next;
};

struct HandleNode : Link {
    Handle value;
};

struct ListImpl {
    u32 mCount;
    Link mHead;

    inline ~ListImpl() {
        HandleNode* it = (HandleNode*)mHead.next;
        HandleNode* end = (HandleNode*)&mHead;
        if (it != end) {
            HandleNode* last = (HandleNode*)end->prev;
            it->prev->next = last->next;
            last->next->prev = it->prev;
            while (it != end) {
                it->value.~Handle();
                HandleNode* n = it;
                it = (HandleNode*)it->next;
                operator delete(n);
                mCount--;
            }
        }
    }
};

struct ListBase : ListImpl {
    inline ~ListBase() {}
};

struct HandleList : ListBase {
    ~HandleList();
};

extern "C" void __register_global_object(void* obj, void* dtor, void* reg);

void fn_18_3ACC0(); /* HandleList::~HandleList */
extern HandleList lbl_18_bss_4B9C;
extern u8 lbl_18_bss_4B90[];

/* Static initializer for the global handle list */
void InitHandleList() {
    HandleList* list = &lbl_18_bss_4B9C;
    list->mCount = 0;
    Link* head = &list->mHead;
    head->prev = head->next = head;
    __register_global_object(list, (void*)fn_18_3ACC0, lbl_18_bss_4B90);
}

HandleList::~HandleList() {}

/* ---------------------------------------------------------------------- */

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

inline void MslError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

template <class T>
inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

inline size_t RecommendSize(size_t cap, const size_t n) {
    const size_t m = (size_t)-1 / sizeof(int);
    if (n > m - cap) {
        MslError("vector length error");
    }
    if (cap < m / 3) {
        return cap + Max(3 * (cap + 1) / 5, n);
    }
    if (cap < m / 3 * 2) {
        return cap + Max((cap + 1) / 2, n);
    }
    return m;
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

/* Temporary storage used while a vector grows.
   The fields are volatile so every access goes through the stack slot, like the original. */
struct VecBuf : noncopyable {
    VecBuf(u32* capRef) : mData(0), mSize(0), mCap(0), mCapRef(capRef), mPos(0) {}
    ~VecBuf() {
        if (mData) {
            mSize = 0;
            operator delete(mData);
        }
    }
    int* volatile mData;
    volatile u32 mSize;
    volatile u32 mCap;
    u32* mCapRef;
    volatile u32 mPos;
    void Grow() { mSize++; }
};

inline void Construct(int* where, int value) {
    *where = value;
}

class Item {
public:
    void Update(); /* fn_18_3DBB0 */
};

class Mgr : public Singleton<Mgr>, public ProcBase {
public:
    Mgr();
    virtual ~Mgr();
    virtual void Run();
    void Shuffle();

    int mMode;        /* 0xC */
    u8 pad10[0x50 - 0x10];
    Vec mOrder;       /* 0x50 */
    Vec mItems;       /* 0x5C */
    VecBase2 mExtra;  /* 0x68 */
};

template <>
Mgr* Singleton<Mgr>::sInstance;

Mgr::~Mgr() {}

typedef void (Item::*ItemFn)();

struct MemFun {
    MemFun(ItemFn f) : mFn(f) {}
    void operator()(Item* p) const { (p->*mFn)(); }
    ItemFn mFn;
};

template <class It, class F>
inline void ForEach(It first, It last, F f) {
    for (; first != last; ++first) {
        f(*first);
    }
}

int fn_80070840(int n); /* random number in [0, n) */

inline void RandomShuffle(int* first, int* last) {
    int n = last - first;
    if (n > 1) {
        int* end = last - 1;
        while (first < end) {
            int* j = first + fn_80070840(n);
            int tmp = *first;
            *first = *j;
            n--;
            first++;
            *j = tmp;
        }
    }
}

inline void SwapOut(Vec& v, VecBuf& buf) {
    int* old = (int*)(void*)v.mData;
    int count = v.end() - old;
    buf.mPos -= count;
    memcpy(buf.mData + buf.mPos, old, count * sizeof(int));
    memset(old, 0, count * sizeof(int));
    buf.mSize += count;
    v.mSize = 0;
    u32 tmpCap = v.mCap;
    v.mCap = buf.mCap;
    buf.mCap = tmpCap;
    int* tmpData = v.mData;
    v.mData = buf.mData;
    buf.mData = tmpData;
    v.mSize = buf.mSize;
    buf.mSize = 0;
}

inline void PushFront(Vec& v) {
    u32 size = v.mSize;
    u32 cap = v.mCap;
    int* data = v.mData;
    if (size < cap) {
        u32 n = size + 1;
        v.mSize = n;
        *(int*)((char*)data + (n - 1) * 4) = data[0];
    } else {
        RecommendSize(cap, 1);
        VecBuf buf(&v.mCap);
        size_t newCap = RecommendSize(v.mCap, v.mSize + 1 - v.mCap);
        int* p = Allocate(newCap);
        buf.mData = p;
        buf.mCap = newCap;
        u32 pos = v.mSize;
        buf.mPos = pos;
        Construct(p + pos + buf.mSize, data[0]);
        buf.Grow();
        SwapOut(v, buf);
    }
}

inline void Erase(Vec& v, void* pos) {
    int* p = (int*)pos;
    memmove(p, p + 1, (v.end() - p - 1) * sizeof(int));
    v.mSize--;
}

void Mgr::Shuffle() {
    if (*(volatile u32*)&mOrder.mSize != 0) { /* forces a separate load, like the original */
        switch (mMode) {
        case 1:
            RandomShuffle(mOrder.mData, mOrder.mData + mOrder.mSize);
            break;
        case 2:
            PushFront(mOrder);
            Erase(mOrder, (void*)mOrder.mData);
            break;
        }
    }
    ForEach((Item**)mItems.mData, (Item**)mItems.mData + mItems.mSize, MemFun(&Item::Update));
}
