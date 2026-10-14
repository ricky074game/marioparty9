/* Shared by most REL modules. Reference copy: mg9101 (fn_18_33D30). */
#include "types.h"

/* Warning / confirmation dialog (yes-no) and the window that owns it. */

typedef unsigned long size_t;

struct FILE {
    u8 data[0x50];
};

extern "C" FILE __files[];
extern "C" int fprintf(FILE* stream, const char* format, ...);
extern "C" __declspec(noreturn) void abort(void);

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

/* ---------------------------------------------------------------------- */

/* The translation unit starts earlier in the original build. Its string,
   static-table and float-constant pools come first, so reproduce their layout
   to get the same pool offsets in the code below. */
struct PoolTarget {
    void A();
    void B();
    void C();
};
typedef void (PoolTarget::*PoolPtmf)();
void UseString(const char*);
void UseTable(const char* const*);
void UsePtmf(PoolPtmf);
void UseFloat(f32);

__declspec(weak) void Pool1() {
    static const char* t[] = {"in"};
    UseTable(t);
}
__declspec(weak) void Pool2() {
    static const char* t[] = {"on", "press", "press_out", "normal_out"};
    UseTable(t);
}
__declspec(weak) void Pool3() {
    static const char* t[] = {"bd_icon_no", "bd_icon_yes"};
    UseTable(t);
}
__declspec(weak) void Pool4() {
    static const char* t[] = {"me00_002", "me00_002", "me00_001", "me00_001"};
    UseTable(t);
}
__declspec(weak) void Pool5() {
    static const char* t[] = {"text_no_sdw", "text_no", "text_yes_sdw", "text_yes"};
    UseTable(t);
}
__declspec(weak) void Pool6() {
    UseString("/layout/icon");
    UseString("ns_icon_yesno");
    UseString("vector length error");
}
__declspec(weak) void Pool7() {
    static const char* t[] = {"bd_icon_no", "bd_icon_yes"};
    UseTable(t);
    UseString("vector::at index out of range");
}
__declspec(weak) void Pool8() {
    static const char* t[] = {"bd_icon_no", "bd_icon_yes"};
    UseTable(t);
    UsePtmf(&PoolTarget::B);
    UsePtmf(&PoolTarget::C);
    UseString("SEQ_SESY001");
    UseString("SEQ_SESY003");
    UseString("SEQ_SESY005");
}
__declspec(weak) void Pool9() {
    UsePtmf(&PoolTarget::A);
    UsePtmf(&PoolTarget::B);
    UsePtmf(&PoolTarget::C);
}
__declspec(weak) void PoolFloats() {
    UseFloat(0.0f);
    UseFloat(0.5f);
    UseFloat(1.0f);
    UseFloat(3.0f);
    UseFloat(4.0f);
    UseFloat(5.0f);
    UseFloat(0.75f);
}

/* ---------------------------------------------------------------------- */

int fn_80042D70(void* p);

struct Handle {
    virtual ~Handle() {}
    void* mPtr;

    Handle(const Handle& o) : mPtr(fn_80042D70(o.mPtr) ? o.mPtr : 0) {}
};

void fn_80049DC0(Handle h, int, int);
int fn_8004A220(Handle h);
void fn_8004A410(Handle h, f32 frame);
void fn_80071590();
void fn_800715E0(f32 seconds);
void fn_8007CD10(const char* name, f32 volume, f32 pitch, f32 pan);

int fn_18_1BBC0(int player);
BOOL fn_18_1BC20(int player);
void* fn_80070680(int pad);
void fn_8006E0C0(void* pad, int rumble);

class noncopyable {
protected:
    noncopyable() {}
    ~noncopyable() {}

private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

/* Metrowerks vector growth helper: new storage is filled back to front and
   swapped into the owner. */
template <class T>
struct SplitStorage {
    T* mFirst;
    u32 mCount;
    SplitStorage() : mFirst(0), mCount(0) {}
    ~SplitStorage() {
        if (mFirst != 0) {
            size_t n = mCount;
            T* p = mFirst + n;
            mCount = 0;
            while (n != 0) {
                --p;
                p->~T();
                --n;
            }
            operator delete(mFirst);
        }
    }
};

template <class T>
struct SplitBuf {
    SplitBuf(u32* capRef) : mStore(), mCap(0), mCapRef(capRef), mRemaining(0) {}
    void Clear() {
        T* begin = mStore.mFirst + mRemaining;
        T* end = begin + mStore.mCount;
        while (end > begin) {
            --end;
            end->~T();
        }
        mStore.mCount = 0;
    }
    ~SplitBuf() { Clear(); }

    SplitStorage<T> mStore;
    u32 mCap;
    u32* mCapRef;
    u32 mRemaining;
};

template <class T>
inline T* AllocateElems(size_t n) {
    T* p = (T*)operator new(n * sizeof(T));
    if (p == 0) {
        MslError("Memory allocation failure");
    }
    return p;
}

struct HandleVectorBase {
    HandleVectorBase() : mData(0), mSize(0), mCapacity(0) {}
    ~HandleVectorBase() {
        if (mData != 0) {
            size_t n = mSize;
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

struct HandleVectorMid : HandleVectorBase {
    ~HandleVectorMid() {}
};

class HandleVector : public HandleVectorMid {
public:
    ~HandleVector() {}
    size_t max_size() const { return (size_t)-1 / sizeof(Handle); }
    size_t RecommendSize(size_t cap, const size_t n) const;
    void CheckLength(size_t n) const;
    Handle& at(u32 i);
    Handle& operator[](u32 i) { return mData[i]; }
    void push_back(const Handle& x);
};

/* Weak so that these are not emitted as ordinary functions, while their string
   literals still end up in the translation unit's main string pool. */
__declspec(weak) size_t HandleVector::RecommendSize(size_t cap, const size_t n) const {
    const size_t m = max_size();
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

__declspec(weak) void HandleVector::CheckLength(size_t n) const {
    if (n > max_size()) {
        MslError("vector length error");
    }
}

__declspec(weak) Handle& HandleVector::at(u32 i) {
    if (mSize <= i) {
        MslError("vector::at index out of range");
    }
    return mData[i];
}

inline void HandleVector::push_back(const Handle& x) {
    HandleVector* self = this;
    if (self->mSize < self->mCapacity) {
        new (self->mData + self->mSize) Handle(x);
        ++self->mSize;
        return;
    }
    self->RecommendSize(self->mCapacity, 1);
    SplitBuf<Handle> buf(&self->mCapacity);
    size_t newcap = self->RecommendSize(self->mCapacity, self->mSize + 1 - self->mCapacity);
    self->CheckLength(newcap);
    buf.mStore.mFirst = AllocateElems<Handle>(newcap);
    buf.mCap = newcap;
    buf.mRemaining = self->mSize;
    new (buf.mStore.mFirst + buf.mRemaining + buf.mStore.mCount) Handle(x);
    ++buf.mStore.mCount;
    Handle* src = self->mData + self->mSize;
    Handle* first = self->mData;
    Handle* dst = buf.mStore.mFirst + buf.mRemaining;
    while (src > first) {
        --dst;
        --src;
        new (dst) Handle(*src);
        --buf.mRemaining;
        ++buf.mStore.mCount;
    }
    u32 c = self->mCapacity; self->mCapacity = buf.mCap; buf.mCap = c;
    Handle* d = self->mData; self->mData = buf.mStore.mFirst; buf.mStore.mFirst = d;
    u32 n = self->mSize; self->mSize = buf.mStore.mCount; buf.mStore.mCount = n;
}

/* ---------------------------------------------------------------------- */

struct UiObj {
    virtual ~UiObj();
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8();
    virtual void v9(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual UiObj* FindPane(const char* name, int recurse);
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual void v26();
    virtual void SetArg(int arg);
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void Attach(void* layout, UiObj* pane);
    virtual void Finish();
    UiObj(int);  /* fn_8008EA90 */
    u8 mData[0x1BC - 4];
};

template <typename T>
struct AutoPtr : noncopyable {
    T* mPtr;
    AutoPtr(T* p) : mPtr(p) {}
    ~AutoPtr() { delete mPtr; }
    void reset(T* p) {
        T* old = mPtr;
        if (p == old) {
            return;
        }
        mPtr = p;
        delete old;
    }
    T* operator->() const { return mPtr; }
    T* get() const { return mPtr; }
};

struct Vec3 {
    Vec3(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    f32 x, y, z;
};

/* Base of the UI layout objects (non-polymorphic). */
struct UiLayout {
    UiLayout(const char* res, const char* name, int, int);  /* fn_80046CF0 */
    ~UiLayout();                                            /* fn_80046040 */
    UiObj* GetRoot();                                       /* fn_800486E0 */
    void* GetHost();                                        /* fn_800486C0 */
    void SetPriority(int);                                  /* fn_80048860 */
    void SetLayer(int);                                     /* fn_800489D0 */
    void SetTranslate(const Vec3&);                         /* fn_80048BD0 */
    void SetVisible(int);                                   /* fn_80048FA0 */
    Handle GetAnim(const char* name, int);                  /* fn_80049110 */

    u32 mPtr;
};

int fn_8004E010(int);

struct Task {
    Task();  /* fn_80071350 */
    virtual int GetKind();
    virtual int GetType();
    virtual void Update();
    virtual ~Task();  /* fn_80071420 */
    void Start(int);         /* fn_80071580 */
    void SetOrder(int);      /* fn_800714E0 */
    int GetOrder();          /* fn_800714F0 */

    int mOrder;
};

struct CursorObj {
    void Reset(int); /* fn_80038710 */
};

struct PictureSelector {
    void SetPicture(int);
};

struct CursorInfo {
    u8 data[0x14];
};
CursorInfo fn_800392C0(void* obj, const void* mtx);

struct Dialog;
typedef void (Dialog::*DialogState)();
extern DialogState lbl_80237470;

struct DialogTask {
    DialogTask();  /* fn_80071350 */
    virtual int GetKind();
    virtual int GetType();
    virtual ~DialogTask();  /* fn_80071420 */
    void Start(int);        /* fn_80071580 */
    void SetOrder(int);     /* fn_800714E0 */
    int GetOrder();         /* fn_800714F0 */

    int mOrder;
};

struct Dialog : UiLayout, DialogTask {
    /* The vptr is placed where the first virtual function is declared. */
    u8 pad0C[0x18 - 0x0C];
    HandleVector mHandles[2];  /* 0x18 */
    u8 mDone;                  /* 0x30 */
    CursorObj* mCursor;        /* 0x34 */
    PictureSelector* mPicture; /* 0x38 */
    int mSel;                  /* 0x3C */
    int mPlayer;               /* 0x40 */
    int mState[2];             /* 0x44 */
    u8 mReset;                 /* 0x4C */
    u8 pad4D[0x60 - 0x4D];
    int mMode;                 /* 0x60 */
    u8 mLocked;                /* 0x64 */
    u8 mUsePicture;            /* 0x65 */
    u8 pad66[0x7C - 0x66];
    f32 mMtx[6];               /* 0x7C */

    Dialog(int, int, int);  /* fn_18_31FA0 */
    ~Dialog();
    void PlayAnim(u32 idx, int anim, int state) {
        fn_80049DC0(mHandles[idx].at(anim), 1, 1);
        fn_8004A410(mHandles[idx].at(anim), 1.0f);
        mState[idx] = state;
    }
    void StateOpen();    /* fn_18_32C50 */
    void StateWait();    /* fn_18_337C0 */
    void StateSelect();  /* fn_18_33D30 */

    DialogState mNextState;  /* 0x98 */
};

void Dialog::StateSelect() {
    mMode = 2;
    int pad = fn_18_1BBC0(mPlayer);
    if (!fn_18_1BC20(mPlayer)) {
        fn_8006E0C0(fn_80070680(pad), 2);
    }
    if (mSel == 1) {
        fn_8007CD10("SEQ_SESY003", 0.0f, 1.0f, 0.0f);
    } else {
        fn_8007CD10("SEQ_SESY005", 0.0f, 1.0f, 0.0f);
    }
    fn_800715E0(0.1f);

    int sel = mSel;
    if (mState[sel] != 1) {
        PlayAnim(sel, 1, 1);
    }
    while (!fn_8004A220(mHandles[mSel].at(1))) {
        fn_80071590();
    }

    if (!mUsePicture) {
        fn_800392C0(mCursor, &mMtx[0]);
    } else {
        fn_800392C0(mPicture, &mMtx[0]);
    }

    for (int i = 0; i < 2; i++) {
        if (i == mSel) {
            if (mState[i] != 2) {
                PlayAnim(i, 2, 2);
            }
        } else {
            if (mState[i] != 3) {
                PlayAnim(i, 3, 3);
            }
        }
    }

    while (!fn_8004A220(mHandles[mSel].at(2))) {
        fn_80071590();
    }
    SetVisible(0);
    mDone = 1;
    mNextState = lbl_80237470;
    fn_800715E0(-1.0f);
}

/* ---------------------------------------------------------------------- */

struct DialogWindow : UiLayout, Task {
    /* The vptr is placed where the first virtual function is declared. */
    int mArg;                  /* 0x0C */
    AutoPtr<UiObj> mObj;         /* 0x10 */
    HandleVector mHandles;     /* 0x14 */
    int m20;                   /* 0x20 */
    u8 mOpen;                  /* 0x24 */
    u8 mClosed;                /* 0x25 */
    bool mResult;                /* 0x26 */
    AutoPtr<Dialog> mDialog;     /* 0x28 */

    DialogWindow(int arg, int anim, int layer);
    int GetKind();
    int GetType();
    void Update();
    ~DialogWindow();
};

DialogWindow::DialogWindow(int arg, int anim, int layer)
    : UiLayout("/layout/warning", "warning_window", 0, fn_8004E010(3)),
      mArg(arg),
      mObj(0),
      mHandles(),
      m20(-1),
      mOpen(0),
      mClosed(0),
      mResult(0),
      mDialog(0) {
    SetPriority(0x9061);
    SetLayer(layer);

    static const char* names[2] = {"in", "out"};
    for (u32 i = 0; i < 2; i++) {
        mHandles.push_back(GetAnim(names[i], 0));
    }

    mObj.reset(new UiObj(0));
    UiObj* obj = mObj.mPtr;
    obj->Attach(GetHost(), GetRoot()->FindPane("text", 1));
    mObj->SetArg(anim);
    Task::Start(1);
    Task::SetOrder(layer);
    SetVisible(0);
}

DialogWindow::~DialogWindow() {
    mObj->Finish();
}

void DialogWindow::Update() {
    if (!mOpen) {
        return;
    }
    SetVisible(1);
    fn_80049DC0(mHandles[0], 1, 1);
    while (!fn_8004A220(mHandles[0])) {
        fn_80071590();
    }

    mDialog.reset(new Dialog(mArg, 1, 0));
    if (mDialog->mReset == 1) {
        mDialog->mReset = 0;
    }
    mDialog->SetPriority(0x9061);
    mDialog->Start(1);
    mDialog->SetOrder(Task::GetOrder());
    mDialog->SetLayer(Task::GetOrder());
    Vec3 pos(0.0f, -130.0f, 0.0f);
    mDialog->SetTranslate(pos);

    Dialog* d = mDialog.mPtr;
    if (!d->mUsePicture) {
        if (d->mCursor) {
            d->mCursor->Reset(1);
        }
    } else {
        if (d->mPicture) {
            d->mPicture->SetPicture(1);
        }
    }
    d->mLocked = 0;
    switch (d->mMode) {
    case -1:
    case 0:
        d->mNextState = &Dialog::StateOpen;
        break;
    case 1:
        d->mNextState = &Dialog::StateWait;
        break;
    case 2:
        d->mNextState = &Dialog::StateSelect;
        break;
    }
    while (!mDialog->mDone) {
        fn_80071590();
    }

    mResult = mDialog->mSel == 1;
    mDialog.reset(0);

    fn_80049DC0(mHandles[0], 1, 1);
    while (!fn_8004A220(mHandles[1])) {
        fn_80071590();
    }
    SetVisible(0);
    mClosed = 1;
    fn_800715E0(-1.0f);
}
