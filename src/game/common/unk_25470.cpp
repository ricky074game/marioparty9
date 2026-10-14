/* Shared by most REL modules. Reference copy: mg9101 (fn_18_25470). */
#include "types.h"

#pragma inline_depth(20)

typedef unsigned long size_t;

struct FILE {
    u8 data[0x50];
};

extern "C" FILE __files[];
extern "C" int fprintf(FILE* stream, const char* format, ...);
extern "C" void abort(void);
extern "C" void* memset(void* dst, int c, size_t n);
extern "C" void* memmove(void* dst, const void* src, size_t n);

void* operator new(size_t);
void operator delete(void*);
inline void* operator new(size_t, void* p) {
    return p;
}

inline void MslError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

template <class T>
inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

/* The original translation unit's string pool starts with the vector error
   message, followed by literals of other functions (the numbered filler stands
   in for them) so the pooled strings used below land at the same offsets. */
extern const char* StringPoolHead[];
const char* StringPoolHead[] = {
    "vector length error",
    "00000000000", "00000000001", "00000000002", "00000000003", "00000000004", "00000000005",
    "00000000006", "00000000007", "00000000008", "00000000009", "00000000010", "00000000011",
    "00000000012", "00000000013", "00000000014", "00000000015", "00000000016", "00000000017",
    "00000000018", "00000000019", "00000000020", "00000000021", "00000000022", "00000000023",
    "00000000024", "00000000025", "00000000026", "00000000027", "00000000028", "00000000029",
    "00000000030", "00000000031", "00000000032", "00000000033", "00000000034", "00000000035",
    "00000000036", "00000000037", "00000000038", "00000000039", "00000000040", "00000000041",
    "00000000042", "00000000043",
    "SEQ_SESY023",
    "00000000100", "00000000101", "00000000102", "00000000103", "00000000104", "00000000105",
    "00000000106", "00000000107", "00000000108", "00000000109", "00000000110", "00000000111",
    "00000000112", "00000000113", "00000000114", "00000000115", "00000000116", "00000000117",
    "00000000118", "00000000119", "00000000120", "00000000121", "00000000122", "abc",
    "SEQ_SESY102",
    "SEQ_SESY101",
};

inline void IntVecOutOfMemory() {
    MslError("Memory allocation failure");
}
inline void VecVecOutOfMemory() {
    MslError("Memory allocation failure");
}

/* ---------------------------------------------------------------------- */
/* vector<vector<int>> and the deque that stores snapshots of it           */

inline void CopyInts(int* dst, const int* first, const int* last) {
    memmove(dst, first, (last - first) * sizeof(int));
}

struct IntVec {
    int* mData;
    u32 mSize;
    u32 mCap;

    static size_t max_size() { return (size_t)-1 / sizeof(int); }
    static size_t GrowSize(size_t cap, const size_t n) {
        const size_t m = max_size();
        if (cap < m / 3) {
            return cap + Max(3 * (cap + 1) / 5, n);
        }
        if (cap < m / 3 * 2) {
            return cap + Max((cap + 1) / 2, n);
        }
        return m;
    }

    IntVec() : mData(0), mSize(0), mCap(0) {}
    /* The "vector length error" checks are written out in Process(): the
       original code pooled that literal with the calling function's strings. */
    void Allocate(u32 n) {
        int* p = (int*)operator new(n * sizeof(int));
        if (p == 0) {
            IntVecOutOfMemory();
        }
        mData = p;
        mCap = n;
    }
    /* The int cast keeps the compiler from folding the subtraction to zero. */
    void Clear() {
        int n = mSize;
        mSize -= (u32)n;
    }
    void Destroy() {
        if (mData != 0) {
            Clear();
            operator delete(mData);
        }
    }
};

/* The original expanded each destructor through nested inline layers, each
   with its own null check on the object. */
template <class T>
inline void Destroy1(T* p) {
    if (p != 0) {
        p->Destroy();
    }
}
template <class T>
inline void Destroy2(T* p) {
    if (p != 0) {
        Destroy1(p);
    }
}
template <class T>
inline void Destroy3(T* p) {
    if (p != 0) {
        Destroy2(p);
    }
}
template <class T>
inline void Destroy4(T* p) {
    if (p != 0) {
        Destroy3(p);
    }
}

struct VecVec {
    IntVec* mData;
    u32 mSize;
    u32 mCap;

    static size_t max_size() { return (size_t)-1 / sizeof(IntVec); }

    VecVec() : mData(0), mSize(0), mCap(0) {}
    void Allocate(u32 n) {
        IntVec* p = (IntVec*)operator new(n * sizeof(IntVec));
        if (p == 0) {
            VecVecOutOfMemory();
        }
        mData = p;
        mCap = n;
    }
    VecVec(u32 n) : mData(0), mSize(0), mCap(0) {
        Allocate(n);
        memset(mData + mSize, 0, n * sizeof(IntVec));
        mSize += n;
    }
    void Destroy() {
        if (mData != 0) {
            u32 n = mSize;
            mSize -= n;
            IntVec* p = mData + n;
            while (n != 0) {
                --p;
                Destroy4(p);
                --n;
            }
            operator delete(mData);
        }
    }
};

struct Deque {
    u32 mStart;
    u32 mSize;
    struct {
        VecVec** mData;
        u32 mStart;
        u32 mSize;
        u32 mCap;
    } mMap;

    bool empty() const { return mSize == 0; }
    VecVec& back() {
        u32 idx = mStart + mSize - 1;
        u32 blk = idx / 85;
        u32 slot = (mMap.mStart + blk) % mMap.mCap;
        return mMap.mData[slot][idx - blk * 85];
    }
};

/* bool wrapper; as a class object it gets a word-sized stack slot */
struct Flag {
    u8 v;
};
void fn_18_26A40(Deque* d, int n, Flag* flag);
void fn_18_266D0(Deque* d, VecVec* v, Flag* flag);

class SnapshotSource {
public:
    BOOL Has(int id);               /* fn_18_20FF0 */
    IntVec Get(int id);             /* fn_18_21030 */
    void Set(int id, IntVec* v);    /* fn_18_21420 */
};

extern SnapshotSource* lbl_18_bss_4730;
int fn_18_1BBC0(int player);

class Snapshots {
public:
    void Process(int restore);
    int* GetField8();
    int* GetFieldC();

    u8 pad0[8];
    int m8;
    int mC;
    u8 pad10[0x74 - 0x10];
    Deque mDeque;
};

void Snapshots::Process(int restore) {
    SnapshotSource* src = lbl_18_bss_4730;
    if (src == 0) {
        return;
    }
    if (restore != 0 && !mDeque.empty()) {
        VecVec& back = mDeque.back();
        VecVec vec;
        {
            const IntVec* first = back.mData;
            const IntVec* last = back.mData + back.mSize;
            u32 n = last - first;
            if (n != 0) {
                if (n > VecVec::max_size()) {
                    MslError("vector length error");
                }
                vec.Allocate(n);
                IntVec* dst = vec.mData + vec.mSize;
                for (; first != last; ++first, ++dst) {
                    new (dst) IntVec;
                    int* f = first->mData;
                    int* l = first->mData + first->mSize;
                    u32 m = l - f;
                    if (m != 0) {
                        if (m > IntVec::max_size()) {
                            MslError("vector length error");
                        }
                        dst->Allocate(m);
                        CopyInts(dst->mData + dst->mSize, f, l);
                        dst->mSize += l - f;
                    }
                    ++vec.mSize;
                }
            }
        }
        Flag flag = {0};
        fn_18_26A40(&mDeque, 1, &flag);
        for (u32 i = 0; i < 4; i++) {
            int id = fn_18_1BBC0(i);
            if (src->Has(id)) {
                src->Set(id, &vec.mData[i]);
            }
        }
        Destroy2(&vec);
    } else {
        VecVec vec(4);
        for (u32 i = 0; i < 4; i++) {
            int id = fn_18_1BBC0(i);
            if (src->Has(id)) {
                IntVec tmp = src->Get(id);
                IntVec& dst = vec.mData[i];
                if (&dst != &tmp) {
                    int* first = tmp.mData;
                    int* last = tmp.mData + tmp.mSize;
                    u32 n = last - first;
                    if (n > dst.mCap) {
                        u32 grow = n - dst.mCap;
                        if (grow > IntVec::max_size() - dst.mCap) {
                            MslError("vector length error");
                        }
                        u32 newCap = dst.mCap + 0;
                        newCap = IntVec::GrowSize(dst.mCap, grow);
                        dst.Clear();
                        if (dst.mData != 0) {
                            operator delete(dst.mData);
                            dst.mData = 0;
                            dst.mCap = 0;
                        }
                        if (newCap > IntVec::max_size()) {
                            MslError("vector length error");
                        }
                        dst.Allocate(newCap);
                    }
                    CopyInts(dst.mData, first, last);
                    dst.mSize = n;
                }
                Destroy3(&tmp);
            }
        }
        Flag flag = {0};
        fn_18_266D0(&mDeque, &vec, &flag);
        Destroy2(&vec);
    }
}

int* Snapshots::GetField8() {
    return &m8;
}

int* Snapshots::GetFieldC() {
    return &mC;
}

/* ---------------------------------------------------------------------- */
/* Pointer-driven menu                                                     */

bool fn_80042D70(u32 obj);

class Handle {
public:
    Handle(u32 obj) : mObj(fn_80042D70(obj) ? obj : 0) {}
    Handle(const Handle& other) : mObj(fn_80042D70(other.mObj) ? other.mObj : 0) {}
    virtual ~Handle() {}

    u32 mObj;
};

void fn_80049DC0(Handle handle, int a, int b);
void fn_8004A410(Handle handle, f32 value);

inline void StartAnim(const Handle& handle, f32 frame) {
    fn_80049DC0(handle, 0, 1);
    fn_8004A410(handle, frame);
}

struct MenuItemObj {
    Handle h0;
    Handle h8;
    Handle h10;
    Handle h18;
    Handle h20;
};

struct Vec5 {
    f32 v[5];
};

class Pointer {
public:
    int fn_80038FF0();               /* hovered item */
    void fn_800385B0();
    void fn_80038710(int);
    void fn_80038790(f32);
    void fn_80038F00();
    void fn_80039010(int);
    BOOL fn_80039250();
    void fn_80039060(int);
    Vec5 fn_800392C0(const Vec5&);
};

struct MenuItem {
    int m0;
    int m4;
    int m8;
    int mC;
    int m10;
};

/* One 0x1c-byte entry per menu item; the array overlays the start of MenuSlots
   and its fields are reached through offsets past the entry size. */
struct Slot {
    u8 raw[0x1c];
    void*& layout() { return *(void**)(raw + 0x18); }
    MenuItemObj*& obj() { return *(MenuItemObj**)(raw + 0x24); }
    int& state() { return *(int*)(raw + 0x30); }
};

struct MenuSlots {
    u8 pad0[4];
    void* m4;
    u8 pad8[0xC];
    int m14;
    u8 pad18[0xE0 - 0x18];
    int mE0;
};

void* fn_80070680(int pad);
u32 fn_8006DBC0(void* pad, int);
void fn_8006E0C0(void* pad, int);
void fn_8007CD10(const char* name, f32, f32, f32);
BOOL fn_18_1BC20(int player);
void fn_80048FA0(void* layout, int);
void fn_18_23A00(MenuSlots* slots, int);
void fn_80071590();

class WaitBase {
public:
    u8 pad[0x2c];
};

class Waiter : public WaitBase {
public:
    Waiter(int pad, int a, int b);          /* fn_18_34230 */
    virtual void Vfunc0();
    virtual void Vfunc1();
    virtual void Vfunc2();
    virtual void Vfunc3();
    virtual void Vfunc4();
    virtual ~Waiter();
    void Start();                           /* fn_18_34C20 */
    BOOL IsDone();                          /* fn_18_34C30 */
    BOOL GetResult();                       /* fn_18_34C40 */
};

class MenuCtrl;
typedef void (MenuCtrl::*MenuFn)();
extern MenuFn lbl_80237470;

class MenuCtrl {
public:
    void Update();
    void Close();
    void Callback(); /* fn_18_27A10 */

    u8 pad0[0x10];
    int mState;
    int mPad;
    int mPlayer;
    int mSel;
    MenuItem mCur;
    MenuItem* mItems;
    int pad38;
    int mHover;
    u8 pad40[4];
    MenuSlots* mSlots;
    Pointer* mPointer;
    Vec5 m4c;
    f32 mScale;
    int m64;
    u8 pad68[0x8c - 0x68];
    Waiter* mWaiter;
};

static MenuFn sCallback = &MenuCtrl::Callback;

void MenuCtrl::Update() {
    int sel;
    int cur = -1;
    sel = mPointer->fn_80038FF0();
    if (!mPointer->fn_80039250()) {
        if ((fn_8006DBC0(fn_80070680(mPad), -1) & 0x900) && sel != -1) {
            cur = sel;
            if (!fn_18_1BC20(mPlayer)) {
                fn_8006E0C0(fn_80070680(mPad), 1);
            }
            if (mItems[sel].m0 != 0) {
                fn_8007CD10("SEQ_SESY102", 0.0f, 1.0f, 0.0f);
            } else {
                fn_8007CD10("SEQ_SESY023", 0.0f, 1.0f, 0.0f);
            }
        }
    }
    /* Dead stores: they only fix the float constant pool order so that -1.0f
       lands at offset 0x18, as in the original translation unit. */
    f32 poolOrder = 0.5f;
    poolOrder = 7.0f;
    poolOrder = 176.0f;
    poolOrder = -0.0f;
    if (cur != -1) {
        u8* slots = (u8*)mSlots;
        int off = cur * 0x1c;
        Handle h((*(MenuItemObj**)(slots + 0x24 + off))->h18);
        *(int*)(slots + 0x30 + off) = 3;
        StartAnim(h, 1.0f);
        mCur = mItems[cur];
        mSel = cur;
        mState = 4;
    } else {
        mPointer->fn_800385B0();
        if (sel != -1) {
            for (int i = 0; i < mSlots->mE0; i++) {
                if (i == sel) {
                    MenuSlots* slots = mSlots;
                    int off = i * 0x1c;
                    MenuItemObj* obj = *(MenuItemObj**)((u8*)slots + off + 0x24);
                    Handle h(obj->h10);
                    *(int*)((u8*)slots + off + 0x30) = 2;
                    StartAnim(h, 1.0f);
                } else {
                    MenuSlots* slots = mSlots;
                    int off = i * 0x1c;
                    MenuItemObj* obj = *(MenuItemObj**)((u8*)slots + off + 0x24);
                    Handle h(obj->h10);
                    *(int*)((u8*)slots + off + 0x30) = 2;
                    StartAnim(h, -1.0f);
                }
            }
            if (sel != mHover) {
                if (!fn_18_1BC20(mPlayer)) {
                    fn_8006E0C0(fn_80070680(mPad), 0);
                }
                fn_8007CD10("SEQ_SESY101", 0.0f, 1.0f, 0.0f);
                mHover = sel;
            }
        } else {
            for (int i = 0; i < mSlots->mE0; i++) {
                MenuSlots* slots = mSlots;
                int off = i * 0x1c;
                MenuItemObj* obj = *(MenuItemObj**)((u8*)slots + off + 0x24);
                Handle h(obj->h10);
                *(int*)((u8*)slots + off + 0x30) = 2;
                StartAnim(h, -1.0f);
            }
            mHover = -1;
        }
    }
}

void MenuCtrl::Close() {
    MenuSlots* ms;
    int i;
    if (mCur.m8 == 0) {
        return;
    }
    i = 0;
    ms = mSlots;
    for (Slot* p = (Slot*)ms; i < ms->m14; p++, i++) {
        p->state() = 4;
        fn_80049DC0(p->obj()->h20, 1, 1);
    }
    Waiter* w = new Waiter(mPad, mCur.m8, 3);
    if (w != mWaiter) {
        Waiter* old = mWaiter;
        mWaiter = w;
        delete old;
    }
    mWaiter->Start();
    while (!mWaiter->IsDone()) {
        fn_80071590();
    }
    BOOL result = mWaiter->GetResult();
    if (mWaiter != 0) {
        Waiter* old = mWaiter;
        mWaiter = 0;
        delete old;
    }
    if (result) {
        if (mState != 3 && mState != 6 && mState != 4) {
            return;
        }
        mState = 5;
        ms = mSlots;
        fn_80048FA0(ms->m4, 1);
        i = 0;
        for (Slot* p = (Slot*)ms; i < ms->m14; p++, i++) {
            fn_80048FA0(p->layout(), 1);
        }
        fn_18_23A00(mSlots, 1);
        MenuFn fn = mPointer == 0 ? lbl_80237470 : sCallback;
        if (fn) {
            mPointer->fn_80039010(0);
            mPointer->fn_800392C0(m4c);
            mPointer->fn_80038790(mScale);
            mPointer->fn_80038710(m64);
            mPointer->fn_80038F00();
            if (mPointer != 0) {
                Pointer* ptr = mPointer;
                mPointer = 0;
                delete ptr;
            }
        }
    } else {
        ms = mSlots;
        i = 0;
        for (Slot* p = (Slot*)ms; i < ms->m14; p++, i++) {
            fn_80048FA0(p->layout(), 1);
        }
        i = 0;
        for (Slot* p = (Slot*)ms; i < ms->m14; p++, i++) {
            p->state() = 0;
            fn_80049DC0(p->obj()->h0, 1, 1);
        }
        mPointer->fn_80038710(1);
        mPointer->fn_80039010(1);
        if (mPointer->fn_80039250()) {
            mPointer->fn_80039060(0);
        }
        mCur.m0 = -1;
        mCur.m4 = 0;
        mCur.mC = 0;
        mCur.m8 = 0;
        mCur.m10 = -1;
        mState = 3;
    }
}
