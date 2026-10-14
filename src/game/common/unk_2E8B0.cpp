/* Shared by most REL modules. Reference copy: mg9101 (fn_18_2E8B0). */
#include "types.h"
#include "game/common/unk_170B0.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);

/* The original translation unit starts earlier; its string literals (0x538 bytes) precede ours in
   the string pool. Reproduce the pool layout so pool-relative offsets match. */
void UseString(const char*);
__declspec(weak) void PoolStrings() {
    UseString(
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
}

struct Object;
bool fn_80042D70(Object* obj);

class Handle {
public:
    Handle() : mObj(0) {}
    Handle(Object* obj) : mObj(fn_80042D70(obj) ? obj : 0) {}
    Handle(const Handle& other) : mObj(fn_80042D70(other.mObj) ? other.mObj : 0) {}
    Handle& operator=(const Handle& other) {
        mObj = fn_80042D70(other.mObj) ? other.mObj : 0;
        return *this;
    }
    virtual ~Handle() {}

    Object* mObj;
};

bool fn_8004A220(Handle handle);
void fn_8004A4B0(Handle handle, float time);
void fn_80049DC0(Handle handle, int a, int b);
void fn_8004A3C0(Handle handle, int a);
Handle fn_80049110(void* layout, const char* name, int a);

/* Animation list of a layout. */
class HandleList {
public:
    Handle Get(int idx) {
        if (mCount == 0) {
            return Handle(0);
        }
        return mEntries[idx];
    }

    int unk0;
    Handle* mEntries;
    int mCount;
};

struct PaneNode {
    PaneNode* next;
    u8 pad[0xB3];
    u8 flags;
};

struct Pane {
    u8 pad[0x14];
    PaneNode* children;
    u8 pad18[0xBB - 0x18];
    u8 flags;
};

Pane* fn_800486E0(void* layout);
Pane* fn_80048B50(void* layout, const char* name, int recurse);
void fn_8004ABD0(void* layout, const char* name, const char* text, int a);

struct Vec3 {
    float x, y, z;
    Vec3() {}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};

struct Vec2 {
    float x, y;
    Vec2() {}
    Vec2(float x_, float y_) : x(x_), y(y_) {}
};

Vec3 fn_8004D8E0();
extern float lbl_8026DAA8;
extern float lbl_8026DAAC;
extern float kHalf;

/* Layout object: animation list at +4, child pane list at +0x14. */
class LayoutObj : public HandleList {
};

class DisplayBase {
public:
    int mState;
    LayoutObj** mItems;
    int mCount;
    int pad0C;
    int pad10;
};

/* Numeric display built from a layout (vtable at +0x14). */
class NumberDisplay : public DisplayBase {
public:
    virtual ~NumberDisplay();
    virtual void v1();
    virtual void SetPos2D(Vec2 pos);
    virtual void v3();
    virtual void SetFlags(int flags);
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void Update();
    virtual void SetNumber(int value, int player);
    virtual void SetPos(const Vec3& pos, int arg);
    virtual void SetPosXYZ(float x, float y, float z, int arg);

    void AnimPlay(int i) {
        const Handle& h = mItems[0]->Get(i);
        fn_8004A4B0(h, 0.0f);
        fn_80049DC0(h, 1, 1);
    }

    int mNumber;
    Vec3 mPos;
    u8 mHasPos;
    int pad2C;
    int mTimer;
    int mDuration;
};

void NumberDisplay::Update() {
    if (mHasPos) {
        SetPos(mPos, pad2C);
    }
    switch (mState) {
    case 2:
        if (fn_8004A220(mItems[0]->Get(0))) {
            AnimPlay(1);
            mTimer = 0;
            mState = 4;
        }
        break;
    case 4:
        if (++mTimer >= mDuration) {
            AnimPlay(2);
            mState = 5;
        }
        break;
    case 5:
        if (fn_8004A220(mItems[0]->Get(2))) {
            LayoutObj* layout = mItems[0];
            if (fn_800486E0(layout)) {
                fn_800486E0(layout)->flags &= ~1;
            }
            mState = 6;
        }
        break;
    }
}

void NumberDisplay::SetFlags(int flags) {
    LayoutObj* layout;
    for (LayoutObj** it = mItems; it != mItems + mCount; ++it) {
        layout = *it;
        if (fn_800486E0(layout)) {
            Pane* p = fn_800486E0(layout);
            p->flags = (p->flags & 0xFE) | (u8)flags;
        }
    }
}

void NumberDisplay::SetNumber(int value, int player) {
    mNumber = value;
    const char* sign = "plus";
    if (value < 0) {
        sign = "minus";
        value = -value;
    }
    int digits[2] = {0, 0};
    u32 n;
    if (value >= 10) {
        n = 2;
        digits[0] = value % 10;
        digits[1] = value / 10;
    } else {
        n = 1;
        digits[0] = value;
    }

    Pane* pane = fn_800486E0(mItems[0]);
    for (PaneNode* node = pane->children; node != (PaneNode*)&pane->children; node = node->next) {
        node->flags &= ~1;
    }

    char name[0x20];
    sprintf(name, "Null_%d_%dp_%s", n, player + 1, sign);
    Pane* target = fn_80048B50(mItems[0], name, 1);
    target->flags = (target->flags & 0xFE) | 1;

    for (u32 i = 0; i < n; i++) {
        const char* digitNames[10] = {"w0", "w1", "w2", "w3", "w4", "w5", "w6", "w7", "w8", "w9"};
        char animName[0x20];
        sprintf(animName, "%d_%dp_%s_n%d", n, player + 1, sign, i);
        fn_8004ABD0(mItems[0], animName, digitNames[digits[i]], 1);
    }
}

void NumberDisplay::SetPosXYZ(float x, float y, float z, int arg) {
    Vec3 pos(x, y, z);
    SetPos(pos, arg);
}

void NumberDisplay::SetPos(const Vec3& pos, int arg) {
    mPos = pos;
    pad2C = arg;
    Vec3 size = fn_8004D8E0();
    float hw = lbl_8026DAA8 * kHalf;
    float hh = lbl_8026DAAC * kHalf;
    size.x = size.x - hw;
    size.y = -size.y + hh;
    Vec2 v;
    v = Vec2(size.x, size.y);
    SetPos2D(v);
}

class Unk2EFB0 {
public:
    ~Unk2EFB0();
};

Unk2EFB0::~Unk2EFB0() {}

void fn_8009ACF0(void* p);

class ProcEntry {
public:
    ~ProcEntry() {
        if (mUnk4) {
            fn_8009ACF0(mUnk4);
        }
    }

    NumberDisplay* mDisplay;
    void* mUnk4;
};

class ProcEntryBuf {
public:
    ~ProcEntryBuf() {
        if (mEntries) {
            u32 n = mCount;
            u32 end = n;
            ProcEntry* p = mEntries + end;
            mCount -= end;
            while (n != 0) {
                --p;
                p->~ProcEntry();
                --n;
            }
            operator delete(mEntries);
        }
    }

    ProcEntry* mEntries;
    u32 mCount;
};

class ProcEntryVecImpl {
public:
    ~ProcEntryVecImpl() {}
    ProcEntryBuf mBuf;
};

class ProcEntryVec {
public:
    ~ProcEntryVec() {}
    ProcEntryVecImpl mImpl;
};

/* List of displays driven once per frame (data vtable lbl_18_data_3054+0x40). */
class DisplayList : public ProcBase {
public:
    virtual ~DisplayList();
    virtual void Run();

    ProcEntryVec mVec;
};

void DisplayList::Run() {
    for (u32 i = 0; i < mVec.mImpl.mBuf.mCount; i++) {
        mVec.mImpl.mBuf.mEntries[i].mDisplay->Update();
    }
}


class DisplayHolder {
public:
    virtual ~DisplayHolder();
    virtual void Release();
    virtual void Nop();

    int mUnk4;
    int mUnk8;
    NumberDisplay* mDisplay;
};

void DisplayHolder::Release() {
    delete mDisplay;
}

void DisplayHolder::Nop() {}

DisplayList::~DisplayList() {}

class Unk2F150 {
public:
    ~Unk2F150();
};

Unk2F150::~Unk2F150() {}

/* Adjustor thunks for the ProcBase subobject at +0x18 of the display object. */
void* fn_18_2BE00(void* self, int flag);
void fn_18_2DF20(void* self);

void* Thunk_2F190(char* self, int flag) {
    return fn_18_2BE00(self - 0x18, flag);
}

void Thunk_2F1A0(char* self) {
    fn_18_2DF20(self - 0x18);
}

/* ---------------------------------------------------------------------- */
/* Minigame scene constructor and the helper objects it creates. */

extern float lbl_8026E3B0;

void fn_80070DB0(int);
int fn_18_1B6D0();
int fn_80080EA0();
void fn_18_26280(int);
void fn_18_26490(int);
void fn_18_1B370(int);
void fn_800808B0(const char*);
int fn_18_1BBC0(int);
int fn_18_1BC20(int);

/* Owning pointer: reset() deletes the old object; safe-bool conversion. */
template <class T>
class Own {
public:
    typedef T* (Own::*unspecified_bool_type)() const;

    Own() : mPtr(0) {}
    ~Own() { delete mPtr; }
    T* get() const { return mPtr; }
    T* operator->() const { return mPtr; }
    operator unspecified_bool_type() const { return mPtr == 0 ? 0 : &Own::get; }

    void reset(T* p = 0) {
        if (p != mPtr) {
            T* old = mPtr;
            mPtr = p;
            delete old;
        }
    }

    T* mPtr;
};

class LayoutRef {
public:
    LayoutRef(const char* path, const char* name, int a, int b); /* fn_80046CF0 */
    ~LayoutRef();                                                /* fn_80046040 */
    void SetPriority(int);                                       /* fn_80048860 */
    void SetLayer(int);                                          /* fn_800489D0 */
    void SetVisible(int);                                        /* fn_80048FA0 */
    Handle FindAnim(const char* name, int a);                    /* fn_80049110 */

    void* mPtr;
};

int fn_8004E010(int);

class ProcX : public ProcBase {
public:
    ProcX() : mA(1), mB(1), mC(0) {
        fn_80071580(1);
        fn_800714E0(5);
        fn_800714A0(0x8400);
    }
    virtual ~ProcX();
    virtual void Run();

    int mA;
    int mB;
    u8 mC;
};

class MgScene;

class ProcY : public ProcBase {
public:
    ProcY(MgScene* scene) : mScene(scene) { fn_80070DB0(1); }
    virtual ~ProcY();
    virtual void Run();

    MgScene* mScene;
};

class LayoutPtr {
public:
    LayoutPtr() : mPtr(0) {}
    ~LayoutPtr() { delete mPtr; }
    void reset(LayoutRef* p) {
        if (p != mPtr) {
            LayoutRef* old = mPtr;
            mPtr = p;
            delete old;
        }
    }
    LayoutRef* operator->() const { return mPtr; }
    LayoutRef* mPtr;
};

class TitleLayout {
public:
    TitleLayout() : mAnim() {
        mLayout.reset(new LayoutRef("/menu/layout/title", "demo", 0, fn_8004E010(3)));
        mLayout->SetVisible(0);
        mLayout->SetPriority(0x908F);
        mLayout->SetLayer(3);
        mAnim = mLayout->FindAnim("demo", 0);
    }

    LayoutPtr mLayout;
    Handle mAnim;
};

class ProcZ : public ProcBase {
public:
    ProcZ(MgScene* scene)
        : mC(0), mD(0), mScene(scene), mFrames((int)(20.0f * lbl_8026E3B0)), mTitle() {
        mFlag = 1;
        fn_800714E0(3);
        fn_800714A0(0x7FFFFFFF);
        mTitle.reset(new TitleLayout);
        TitleLayout* t = mTitle.get();
        t->mLayout->SetVisible(1);
        fn_80049DC0(t->mAnim, 1, 1);
        fn_8004A3C0(t->mAnim, 1);
    }
    virtual ~ProcZ();
    virtual void Run();

    u8 mFlag;
    int mC;
    int mD;
    MgScene* mScene;
    int mFrames;
    Own<TitleLayout> mTitle;
};

/* Display object created by fn_18_2BD40 (vtable at +0x14, ProcBase at +0x18). */
class XDisplay : public DisplayBase {
public:
    XDisplay(int type); /* fn_18_2BD40 */
    virtual ~XDisplay();
    virtual void Scale(float x, float y);
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void Start(int arg);

    void Reset(int arg); /* fn_18_2BF00 */

    u8 pad[0x3C - 0x18];
};

class Elem4b {
public:
    Elem4b();  /* fn_18_2F830 */
    ~Elem4b(); /* fn_18_26390 */
    u8 data[4];
};

class Elem4 {
public:
    Elem4();  /* fn_18_2F820 */
    ~Elem4(); /* fn_18_2ACC0 */
    u8 data[4];
};

class Unk268 {
public:
    Unk268(); /* fn_18_22530 */
    virtual ~Unk268();
    u8 data[0x18 - 4];
};

class OuterHolder {
public:
    OuterHolder();  /* fn_18_18AA0 */
    ~OuterHolder(); /* fn_18_198E0 */
    void* mPtr;
};

void fn_18_19A10(OuterHolder*, int);
void fn_18_19960(OuterHolder*, int, int);

/* vector<int> returned by fn_18_1B890; nested wrappers mirror the inlined destructor chain. */
class IntBuf {
public:
    ~IntBuf() {
        if (mData) {
            u32 n = mSize;
            mSize -= n;
            operator delete(mData);
        }
    }
    int* mData;
    u32 mSize;
    u32 mCap;
};

class IntVecImpl2 {
public:
    ~IntVecImpl2() {}
    IntBuf mBuf;
};

class IntVecImpl {
public:
    ~IntVecImpl() {}
    IntVecImpl2 mImpl;
};

class IntVector {
public:
    ~IntVector() {}
    u32 size() const { return mImpl.mImpl.mBuf.mSize; }
    int& operator[](u32 i) { return mImpl.mImpl.mBuf.mData[i]; }
    IntVecImpl mImpl;
};


class MgScene : public Scene {
public:
    MgScene(bool createDisplay, int arg);
    virtual ~MgScene();

    void* m0C;
    u8 m10;
    int m14;
    Own<ProcY> mProcY;
    Elem4b mElems[0x88];
    Elem4 mElems2[4];
    Own<XDisplay> mDisplay;
    int m250;
    int m254;
    u8 m258;
    int m25C;
    int m260;
    int m264;
    Own<Unk268> m268;
    int m26C;
    int m270;
    int m274;
    float m278;
    Own<OuterHolder> mOuter;
    Own<ProcZ> mProcZ;
    Own<ProcX> mProcX;
};

IntVector fn_18_1B890();

MgScene::MgScene(bool createDisplay, int arg)
    : m0C(0), m10(0), m14(-1), mProcY(), mDisplay(), m250(0), m260(0), m264(arg), m268(), m26C(0),
      m270(0), m274(0), m278(2.0f), mOuter(), mProcZ(), mProcX() {
    mProcX.reset(new ProcX);
    m268.reset(new Unk268);
    mProcY.reset(new ProcY(this));
    fn_18_26280(1);
    fn_18_26490(1);
    if (fn_18_1B6D0()) {
        m14 = fn_80080EA0();
        mProcZ.reset(new ProcZ(this));
    }
    if (createDisplay) {
        mDisplay.reset(new XDisplay(4));
        mDisplay->Reset(0);
        mDisplay->Scale(0.0f, 0.0f);
        m258 = 0;
        m254 = (int)(290.0f * lbl_8026E3B0);
        if (mDisplay) {
            mDisplay->Start(0);
        }
    }
    fn_18_1B370(0);
    fn_800808B0("GRP_SHARE_MINIGAME");
    mOuter.reset(new OuterHolder);
    fn_18_19A10(mOuter.get(), 0);
    IntVector players = fn_18_1B890();
    for (u32 i = 0; i < players.size(); i++) {
        int p = players[i];
        if (!fn_18_1BC20(p)) {
            fn_18_19960(mOuter.mPtr, fn_18_1BBC0(p), 1);
        }
    }
}
