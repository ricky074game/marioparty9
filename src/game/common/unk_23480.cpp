/* Shared by most REL modules. Reference copy: mg9101 (fn_18_23480). */
#include "types.h"

typedef unsigned long size_t;

struct FILE {
    u8 data[0x50];
};

extern "C" FILE __files[];
extern "C" int fprintf(FILE* stream, const char* format, ...);
extern "C" void abort(void);

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

/* Vector growth helpers written as macros (same as unk_22530.cpp). */
extern const char kMemAllocFailure[];

#define VEC_RECOMMEND_SIZE(result, cap, n, maxElems)                                                   do {                                                                                                   if ((n) > (maxElems) - (cap)) {                                                                        MslError("vector length error");                                                               }                                                                                                  if ((cap) < (maxElems) / 3) {                                                                          (result) = (cap) + Max(3 * ((cap) + 1) / 5, (n));                                              } else if ((cap) < (maxElems) / 3 * 2) {                                                               (result) = (cap) + Max(((cap) + 1) / 2, (n));                                                  } else {                                                                                               (result) = (maxElems);                                                                         }                                                                                              } while (0)

#define VEC_ALLOCATE(result, count, elemSize, maxElems)                                                do {                                                                                                   if ((count) > (maxElems)) {                                                                            MslError("vector length error");                                                               }                                                                                                  (result) = operator new((count) * (elemSize));                                                     if ((result) == 0) {                                                                                   MslError(kMemAllocFailure);                                                                    }                                                                                              } while (0)

/* ---------------------------------------------------------------------- */
/* Pause menu support (see unk_22530.cpp)                                  */

/* main.dol process base (ctor fn_80071350, dtor fn_80071420) */
class ProcBase {
public:
    ProcBase();
    virtual ~ProcBase();
    virtual void Run() = 0;

    void SetAttr(int);  /* fn_800714A0 */
    void SetPrio(int);  /* fn_800714E0 */
    void SetFlag(int);  /* fn_80071580 */

    int mUnk4;
};

class Layout {
public:
    ~Layout(); /* fn_80046040 */

    void* mPtr;
};

class Handle {
public:
    Handle(const Handle& o);
    virtual ~Handle() {} /* fn_18_189F0 */

    void* mPtr;
};

int fn_80042D70(void* p);

inline Handle::Handle(const Handle& o) : mPtr(fn_80042D70(o.mPtr) ? o.mPtr : 0) {}

void fn_80049DC0(Handle h, int, int);
void fn_8004A060(Handle h);

class LayoutRoot : public Layout {
public:
    virtual ~LayoutRoot();
    u8 pad8[8];
};

class noncopyable {
protected:
    noncopyable() {}
    ~noncopyable() {}

private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

class RootPtr : noncopyable {
public:
    ~RootPtr() { delete mPtr; }

    LayoutRoot* mPtr;
};

class LayoutHolder {
public:
    ~LayoutHolder() { delete mPtr; }
    void reset(Layout* p);  /* fn_18_23480 */
    Layout* get();          /* fn_18_234A0 */

    Layout* mPtr;
};

#define VF(n) virtual void v##n();

class TextBox {
public:
    virtual ~TextBox();
    VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10)
    VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19) VF(20)
    VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29) VF(30)
    VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF(40)
    VF(41) VF(42) VF(43) VF(44) VF(45) VF(46) VF(47) VF(48) VF(49) VF(50)
    VF(51) VF(52) VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59) VF(60)
    virtual void Finish(); /* 0xFC */

    u8 mData[0x1BC - 4];
};

class TextBoxPtr {
public:
    ~TextBoxPtr() { delete mPtr; }

    TextBox* mPtr;
};

class HandleVecBase {
public:
    ~HandleVecBase() {
        if (mData != 0) {
            int n = mSize;
            Handle* p = mData + n;
            mSize -= n;
            while (n != 0) {
                --p;
                p->~Handle();
                --n;
            }
            operator delete(mData);
        }
    }

    Handle* mData;
    u32 mSize;
    u32 mCapacity;
};

class HandleVecMid : public HandleVecBase {
public:
    ~HandleVecMid() {}
};

class HandleVec : public HandleVecMid {
public:
    ~HandleVec() {}
};

struct Quad {
    u32 w[5];
};

/* Temporary buffer used while reallocating a vector<Quad> */
template <class T>
inline void Swap(T& a, T& b) {
    T t = a;
    a = b;
    b = t;
}

struct QuadBufBase {
    QuadBufBase(u32* capRef_) : first(0), count(0), cap(0), capRef(capRef_), pos(0) {}

    Quad* first;
    u32 count;
    u32 cap;
    u32* capRef;
    u32 pos;

    ~QuadBufBase() {
        if (first != 0) {
            int n = count;
            count -= n;
            operator delete(first);
        }
    }
};

struct QuadBuf : QuadBufBase {
    QuadBuf(u32* capRef_) : QuadBufBase(capRef_) {}
    ~QuadBuf() {}

    void Append(const Quad& x) {
        Quad* q = first + pos;
        q += count;
        *q = x;
        count++;
    }
};

/* vector<Quad> with nested inline destructors */
class QuadVecBase {
public:
    void SwapOut(QuadBuf& v) {
        Quad* front = v.first + v.pos;
        Quad* begin = mData;
        Quad* it = mData + mSize;
        while (it > begin) {
            --front;
            --it;
            new (front) Quad(*it);
            v.pos--;
            v.count++;
        }
        mSize -= mSize;
        Swap(mCapacity, v.cap);
        Swap(mData, v.first);
        Swap(mSize, v.count);
    }
    void Clear();                   /* fn_18_234B0 */
    void push_back(const Quad& q);  /* fn_18_234C0 */

    ~QuadVecBase() {
        if (mData != 0) {
            int n = mSize;
            mSize -= n;
            operator delete(mData);
        }
    }

    Quad* mData;
    u32 mSize;
    u32 mCapacity;
};

class QuadVecMid : public QuadVecBase {
public:
    ~QuadVecMid() {}
};

class QuadVec : public QuadVecMid {
public:
    ~QuadVec() {}
};

class PauseItem {
public:
    LayoutHolder mLayout;
    TextBoxPtr mText[2];
    HandleVec mAnims;
    int m18;
};

class PauseMenu {
public:
    virtual ~PauseMenu();
    void PlayIn(int mode);
    void PlayOut(int mode);

    RootPtr mRoot;
    HandleVec mAnims;
    int mCount;
    PauseItem mItems[7];
    QuadVec mQuads;
};

extern "C" void fn_80046040(void); /* Layout::~Layout */

/* Written in asm: the compiler places the epilogue blr after the tail call, but
   the original symbol table splits it off as the following function. */
asm void LayoutHolder::reset(Layout* p) {
    nofralloc
    lwz r0, 0(r3)
    cmplw r4, r0
    beqlr
    stw r4, 0(r3)
    mr r3, r0
    li r4, 1
    b fn_80046040
}

void PauseNoop() {}

Layout* LayoutHolder::get() {
    return mPtr;
}

void QuadVecBase::Clear() {
    mSize -= (int)mSize;
}

void QuadVecBase::push_back(const Quad& x) {
    u32 n = mSize;
    if (n < mCapacity) {
        mSize = n + 1;
        mData[mSize - 1] = x;
        return;
    }

    if (0xCCCCCCC - mCapacity < 1) {
        MslError("vector length error");
    }

    QuadBuf buf(&mCapacity);

    size_t cap = mCapacity;
    const size_t grow = mSize + 1 - cap;
    size_t newCap;
    VEC_RECOMMEND_SIZE(newCap, cap, grow, 0xCCCCCCC);

    void* mem;
    VEC_ALLOCATE(mem, newCap, sizeof(Quad), 0xCCCCCCC);

    buf.first = (Quad*)mem;
    buf.cap = newCap;
    buf.pos = mSize;

    buf.Append(x);
    SwapOut(buf);
}

PauseMenu::~PauseMenu() {
    for (int i = 0; i < mCount; i++) {
        for (int j = 0; j < 2; j++) {
            if (mItems[i].mText[j].mPtr != 0) {
                mItems[i].mText[j].mPtr->Finish();
            }
            TextBox* oldText = mItems[i].mText[j].mPtr;
            if (oldText != 0) {
                mItems[i].mText[j].mPtr = 0;
                delete oldText;
            }
        }
        Layout* oldLayout = mItems[i].mLayout.mPtr;
        if (oldLayout != 0) {
            mItems[i].mLayout.mPtr = 0;
            delete oldLayout;
        }
    }
}

void PauseMenu::PlayIn(int mode) {
    fn_80049DC0(mAnims.mData[mode], 1, 1);
    if (mode == 0) {
        for (int i = 0; i < mCount; i++) {
            mItems[i].m18 = 0;
            Handle* anims = mItems[i].mAnims.mData;
            fn_80049DC0(anims[0], 1, 1);
        }
    } else if (mode == 1) {
        for (int i = 0; i < mCount; i++) {
            if (mItems[i].m18 == 3) {
                mItems[i].m18 = 1;
                fn_80049DC0(mItems[i].mAnims.mData[1], 1, 1);
            } else {
                mItems[i].m18 = 4;
                fn_80049DC0(mItems[i].mAnims.mData[4], 1, 1);
            }
        }
    }
}

void PauseMenu::PlayOut(int mode) {
    fn_8004A060(mAnims.mData[mode]);
    if (mode == 0) {
        for (int i = 0; i < mCount; i++) {
            mItems[i].m18 = -1;
            fn_8004A060(mItems[i].mAnims.mData[0]);
        }
    } else if (mode == 1) {
        for (int i = 0; i < mCount; i++) {
            mItems[i].m18 = -1;
            if (mItems[i].m18 == 1) {
                fn_8004A060(mItems[i].mAnims.mData[1]);
            } else {
                fn_8004A060(mItems[i].mAnims.mData[4]);
            }
        }
    }
}

/* ---------------------------------------------------------------------- */
/* Pointer-driven menu controller (see unk_25470.cpp)                      */

struct Vec5 {
    Vec5();  /* fn_80039510 */
    f32 v[5];
};

class Pointer {
public:
    void fn_80038710(int);
    void fn_80038790(f32);
    void fn_80038F00();
    void fn_80039010(int);
    Vec5 fn_800392C0(const Vec5&);
};

class Waiter {
public:
    u8 pad[0x2c];
    virtual void Vfunc0();
    virtual void Vfunc1();
    virtual void Vfunc2();
    virtual void Vfunc3();
    virtual void Vfunc4();
    virtual ~Waiter();
};

class WaiterPtr : noncopyable {
public:
    WaiterPtr() : mPtr(0) {}
    ~WaiterPtr() { delete mPtr; }

    Waiter* mPtr;
};

class PointerPtr : noncopyable {
public:
    PointerPtr() : mPtr(0) {}
    ~PointerPtr() { delete mPtr; }

    Pointer* mPtr;
};

class PauseMenuPtr : noncopyable {
public:
    PauseMenuPtr() : mPtr(0) {}
    ~PauseMenuPtr() { delete mPtr; }

    PauseMenu* mPtr;
};

/* vector<int> with nested inline destructors */
class IntVecBase {
public:
    IntVecBase() : mData(0), mSize(0), mCapacity(0) {}
    ~IntVecBase() {
        if (mData != 0) {
            int n = mSize;
            mSize -= n;
            operator delete(mData);
        }
    }

    int* mData;
    u32 mSize;
    u32 mCapacity;
};

class IntVecMid0 : public IntVecBase {
public:
    ~IntVecMid0() {}
};

class IntVecMid : public IntVecMid0 {
public:
    ~IntVecMid() {}
};

class IntVec : public IntVecMid {
public:
    ~IntVec() {}
};

/* Container whose real destructor lives in another unit (fn_18_26560) */
class DequeBase {
public:
    DequeBase() : m0(0), m4(0), m8(0), mC(0), m10(0), m14(0) {}
    ~DequeBase();

    u32 m0, m4, m8, mC, m10, m14;
};

class DequeMid0 : public DequeBase {
public:
    ~DequeMid0() {}
};

class DequeMid : public DequeMid0 {
public:
    ~DequeMid() {}
};

class Deque : public DequeMid {
public:
    ~Deque() {}
};

class MenuBase {
public:
    virtual ~MenuBase();
    virtual void Vfunc() = 0;
};

struct MenuItem {
    MenuItem() : m0(-1), m4(0), m8(0), mC(0), m10(-1) {}
    int m0, m4, m8, mC, m10;
};

class MenuCtrl;
typedef void (MenuCtrl::*MenuFn)();
extern MenuFn lbl_80237470;
extern MenuFn lbl_18_data_22F4;

u32 fn_80070DC0();
void fn_800807B0();
void fn_18_240A0(MenuCtrl*);

class MenuCtrl : public MenuBase, public ProcBase {
public:
    MenuCtrl(int arg);
    virtual ~MenuCtrl();
    virtual void Vfunc();
    virtual void Run();
    void Callback();

    u8 m0C;
    int mState;
    int mPad;
    int mPlayer;
    int mSel;
    MenuItem mCur;
    MenuItem* mItems;
    u32 m38;
    int mHover;
    int mArg;
    PauseMenuPtr mSlots;
    PointerPtr mPointer;
    Vec5 m4c;
    f32 mScale;
    int m64;
    IntVec mVec;
    Deque mDeque;
    WaiterPtr mWaiter;
    u8 m90;
    u8 m91;
    f32 m94;
    f32 m98;
};

MenuCtrl::MenuCtrl(int arg) : m0C(0), mState(0), mItems(0), mArg(arg), m90(0), m91(0), m94(0.5f), m98(0.3f) {
    SetFlag(0x11);
    m38 = fn_80070DC0();
    SetPrio(3);
    SetAttr(0x8600);
    fn_18_240A0(this);
}

MenuBase::~MenuBase() {}

MenuCtrl::~MenuCtrl() {
    MenuFn fn = mPointer.mPtr == 0 ? lbl_80237470 : lbl_18_data_22F4;
    if (fn) {
        mPointer.mPtr->fn_80039010(0);
        mPointer.mPtr->fn_800392C0(m4c);
        mPointer.mPtr->fn_80038790(mScale);
        mPointer.mPtr->fn_80038710(m64);
        mPointer.mPtr->fn_80038F00();
        if (mPointer.mPtr != 0) {
            Pointer* ptr = mPointer.mPtr;
            mPointer.mPtr = 0;
            delete ptr;
        }
    }
    if (m90) {
        fn_800807B0();
    }
}
