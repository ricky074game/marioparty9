/* Shared by most REL modules. Reference copy: mg9101 (fn_18_22530). */
#include "game/common/unk_21650.h"

/* "Memory allocation failure" lives in the string pool of an earlier unit. */
extern const char kMemAllocFailure[];

/* Vector growth helpers written as macros: the original code was inlined into
   its callers, so its string literals belong to the caller's string pool. */
#define VEC_RECOMMEND_SIZE(result, cap, n, maxElems)                                               \
    do {                                                                                           \
        if ((n) > (maxElems) - (cap)) {                                                            \
            MslError("vector length error");                                                       \
        }                                                                                          \
        if ((cap) < (maxElems) / 3) {                                                              \
            (result) = (cap) + Max(3 * ((cap) + 1) / 5, (n));                                      \
        } else if ((cap) < (maxElems) / 3 * 2) {                                                   \
            (result) = (cap) + Max(((cap) + 1) / 2, (n));                                          \
        } else {                                                                                   \
            (result) = (maxElems);                                                                 \
        }                                                                                          \
    } while (0)

#define VEC_ALLOCATE(result, count, elemSize, maxElems)                                            \
    do {                                                                                           \
        if ((count) > (maxElems)) {                                                                \
            MslError("vector length error");                                                       \
        }                                                                                          \
        (result) = operator new((count) * (elemSize));                                             \
        if ((result) == 0) {                                                                       \
            MslError(kMemAllocFailure);                                                            \
        }                                                                                          \
    } while (0)

/* ---------------------------------------------------------------------- */
/* Player-input watcher process                                           */

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

/* vector<int> with its nested inline destructors */
class VecImpl : public IntVector {
public:
    void Clear() {
        int n = mSize;
        mSize -= n;
    }
    ~VecImpl() {
        if (mData != 0) {
            Clear();
            operator delete(mData);
        }
    }
};

class VecMid0 : public VecImpl {
public:
    ~VecMid0() {}
};

class VecMid : public VecMid0 {
public:
    ~VecMid() {}
};

class PlayerList : public VecMid {
public:
    PlayerList() {
        mData = 0;
        mSize = 0;
        mCapacity = 0;
    }
    ~PlayerList() {}
};

PlayerList fn_18_1B890();
BOOL fn_18_1BC20(int player);

class PlayerListener {
public:
    virtual ~PlayerListener();
    virtual void OnButton(int player);
};

void* fn_80070680(int pad);
BOOL fn_8006DBC0(void* pad, int mask);

class PlayerProc : public ProcBase {
public:
    PlayerProc();
    virtual ~PlayerProc();
    virtual void Run();
    void SetListener(PlayerListener* l);

    PlayerListener* mListener;
    PlayerList mPlayers;
};

PlayerProc::PlayerProc() : mListener(0) {
    SetFlag(1);
    SetPrio(3);
    SetAttr(0x8500);
    PlayerList tmp = fn_18_1B890();
    if (&mPlayers != &tmp) {
        int* first = tmp.mData;
        int* last = first + tmp.mSize;
        size_t n = last - first;
        size_t cap = mPlayers.mCapacity;
        if (n > cap) {
            size_t newCap;
            const size_t grow = n - cap;
            VEC_RECOMMEND_SIZE(newCap, cap, grow, 0x3FFFFFFF);
            mPlayers.Clear();
            if (mPlayers.mData != 0) {
                operator delete(mPlayers.mData);
                mPlayers.mData = 0;
                mPlayers.mCapacity = 0;
            }
            void* p;
            VEC_ALLOCATE(p, newCap, sizeof(int), 0x3FFFFFFF);
            mPlayers.mData = (int*)p;
            mPlayers.mCapacity = newCap;
        }
        memmove(mPlayers.mData, first, (last - first) * sizeof(int));
        mPlayers.mSize = n;
    }
}

PlayerProc::~PlayerProc() {}

void PlayerProc::Run() {
    for (u32 i = 0; i < mPlayers.mSize; i++) {
        int player = mPlayers.mData[i];
        if (!fn_18_1BC20(player)) {
            if (fn_8006DBC0(fn_80070680(fn_18_1BBC0(player)), 0x10) && mListener != 0) {
                mListener->OnButton(player);
            }
        }
    }
}

void PlayerProc::SetListener(PlayerListener* l) {
    mListener = l;
}


/* ---------------------------------------------------------------------- */
/* Pause menu                                                             */

struct Vec3 {
    f32 x, y, z;
    Vec3(const Vec3& o);
};

struct Vec2 {
    f32 x, y;
    Vec2(f32 x, f32 y);
};

struct Vec {
    f32 x, y, z;
    Vec(f32 x, f32 y, f32 z);
};

struct Color {
    u8 r, g, b, a;
    Color(u8 r, u8 g, u8 b, u8 a);
    ~Color();
};

struct Pane {
    u8 pad[0x4C];
    Vec2 mSize;

    Vec2* GetSize();
};

class Layout {
public:
    Layout(const char* res, const char* name, int, int);  /* fn_80046CF0 */
    ~Layout();                                            /* fn_80046040 */
    void SetPriority(int);                                /* fn_80048860 */
    void SetLayer(int);                                   /* fn_800489D0 */
    Pane* FindPane(const char* name, int recurse);        /* fn_80048B50 */
    Pane* FindPane2(const char* name, int recurse);       /* fn_80048B70 */
    void SetTranslate(const Vec&);                        /* fn_80048BD0 */
    const Vec3* GetTranslate();                           /* fn_80048C00 */
    void SetVisible(int);                                 /* fn_80048FA0 */
    void* GetFont();                                      /* fn_800486C0 */

    void* mPtr;
};

class Handle {
public:
    Handle(Layout* layout, const char* name, int);  /* fn_80049110 */
    Handle(const Handle& o);
    virtual ~Handle();                              /* fn_18_189F0 */

    void* mPtr;
};

BOOL fn_80042D70(void* p);

inline Handle::Handle(const Handle& o) : mPtr(fn_80042D70(o.mPtr) ? o.mPtr : 0) {}

class LayoutRoot : public Layout {
public:
    LayoutRoot();           /* fn_18_34C90 */
    virtual ~LayoutRoot();
    u8 pad8[8];
};

/* Owning pointer to the menu's root layout (virtual destructor) */
class RootPtr {
public:
    RootPtr(LayoutRoot* p);
    void reset(LayoutRoot* p);
    LayoutRoot* get();

    LayoutRoot* mPtr;
};

/* Owning pointer to a plain layout */
class LayoutHolder {
public:
    LayoutHolder() : mPtr(0) {}
    ~LayoutHolder() { delete mPtr; }
    void reset(Layout* p);  /* fn_18_23480 */
    Layout* get();          /* fn_18_234A0 */

    Layout* mPtr;
};

class TextBox {
public:
    TextBox(void* font, Pane* pane, int);  /* fn_8008E970 */
    virtual ~TextBox();
    virtual void v1();
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
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void SetText(const char* text);  /* 0x74 */
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void SetTextColor(const Color& c);  /* 0x98 */
    virtual void v37();
    virtual void v38();
    virtual void SetShadowColor(const Color& c);  /* 0xA4 */
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void SetAlign(int, int);  /* 0xD4 */
    virtual void Update();            /* 0xD8 */

    u8 mData[0x1BC - 4];
};

/* Owning pointer to a text box */
class TextBoxPtr {
public:
    TextBoxPtr();   /* fn_18_1D790 */
    ~TextBoxPtr();  /* fn_18_18A30 */
    TextBox* get(); /* fn_18_1E7F0 */
    void reset(TextBox* p);  /* fn_18_1F620 */

    TextBox* mPtr;
};

/* vector<Handle> with its nested inline destructors */
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
    HandleVec();
    ~HandleVec() {}
    void push_back(const Handle& x);
};

struct Quad {
    f32 x, y, w, h;
    s8 n0, n1, n2, n3;
};

/* vector<Quad> */
class QuadVec {
public:
    QuadVec();
    void Clear();                  /* fn_18_234B0 */
    void push_back(const Quad& q);  /* fn_18_234C0 */

    Quad* mData;
    u32 mSize;
    u32 mCapacity;
};

struct PauseEntry {
    int m0;
    const char* text;
    int m8;
    int mC;
    int m10;
};

class PauseItem {
public:
    PauseItem();
    ~PauseItem();

    LayoutHolder mLayout;
    TextBoxPtr mText[2];
    HandleVec mAnims;
    int m18;
};

class PauseMenu {
public:
    PauseMenu(int count, const PauseEntry* entries);
    virtual ~PauseMenu();

    RootPtr mRoot;
    HandleVec mAnims;
    int mCount;
    PauseItem mItems[7];
    QuadVec mQuads;
};

int fn_8004C810();

#pragma fp_contract off
PauseMenu::PauseMenu(int n, const PauseEntry* entries) : mRoot(0), mCount(n) {
    static const char* gPauseRootAnimNames[] = {"in", "out"};
    static const char* gPauseTextPaneNames[] = {"pause_text"};
    static const char* gPauseItemAnimNames[] = {"in", "out", "on", "click", "normal_out"};
    mRoot.reset(new LayoutRoot);
    mRoot.get()->SetPriority(0x9060);
    mRoot.get()->SetLayer(3);
    for (u32 j = 0; j < 2; j++) {
        mAnims.push_back(Handle(mRoot.get(), gPauseRootAnimNames[j], 0));
    }
    mRoot.get()->SetVisible(0);

    int a;
    if (n == 3) {
        a = 100;
    } else if (n == 2) {
        a = 90;
    } else {
        a = 81;
    }
    int b;
    if (n == 3) {
        b = 70;
    } else if (n == 2) {
        b = 32;
    } else {
        b = 80;
    }

    for (u32 i = 0; i < n; i++) {
        if (fn_8004C810() == 0) {
            mItems[i].mLayout.reset(new Layout("/pause/layout/icon", "pause_icon", 0, 0));
        } else {
            mItems[i].mLayout.reset(new Layout("/pause/layout/icon", "pause_icon_l", 0, 0));
        }
        mItems[i].mLayout.get()->SetPriority(0x9060);
        mItems[i].mLayout.get()->SetLayer(3);
        int y = b - (int)i * a;
        mItems[i].mLayout.get()->SetTranslate(Vec(0.0f, (f32)y, 0.0f));
        for (int k = 0; k < 1; k++) {
            mItems[i].mText[k].reset(new TextBox(mItems[i].mLayout.get()->GetFont(),
                                                 mItems[i].mLayout.get()->FindPane(gPauseTextPaneNames[k], 1), 0));
            mItems[i].mText[k].get()->SetText(entries[i].text);
            mItems[i].mText[k].get()->SetAlign(0, 0);
            mItems[i].mText[k].get()->Update();
            {
                Color c(255, 255, 255, 255);
                mItems[i].mText[k].get()->SetTextColor(c);
            }
            {
                Color c(0, 0, 0, 255);
                mItems[i].mText[k].get()->SetShadowColor(c);
            }
        }
        mItems[i].mLayout.get()->SetVisible(0);
    }

    for (int i = 0; i < mCount; i++) {
        for (u32 j = 0; j < 5; j++) {
            mItems[i].mAnims.push_back(Handle(mItems[i].mLayout.get(), gPauseItemAnimNames[j], 0));
        }
        mItems[i].m18 = -1;
    }

    mQuads.Clear();
    Vec2 baseScale(lbl_8026DAB4, 1.0f);
    f32 half = 0.5f;
    for (int i = 0; i < n; i++) {
        Pane* pane = mItems[i].mLayout.get()->FindPane2("bd_pause_icon", 1);
        if (pane) {
            Vec3 pos(*mItems[i].mLayout.get()->GetTranslate());
            Quad q;
            q.w = baseScale.x * pane->GetSize()->x;
            q.h = baseScale.y * pane->GetSize()->y;
            q.x = pos.x - half * q.w;
            q.y = pos.y + half * q.h;
            q.n2 = i - 1;
            q.n0 = i + 1;
            q.n1 = i - 1;
            q.n3 = i + 1;
            if (i == 0) {
                q.n1 = -1;
                q.n2 = -1;
            }
            if (i == n - 1) {
                q.n3 = -1;
                q.n0 = -1;
            }
            mQuads.push_back(q);
        }
    }
}

struct Vec3Raw {
    u32 w[3];
};

Vec3::Vec3(const Vec3& o) {
    *(Vec3Raw*)this = *(const Vec3Raw*)&o;
}

Vec2* Pane::GetSize() {
    return &mSize;
}

RootPtr::RootPtr(LayoutRoot* p) : mPtr(p) {}

HandleVec::HandleVec() {
    mData = 0;
    mSize = 0;
    mCapacity = 0;
}

PauseItem::PauseItem() {}

PauseItem::~PauseItem() {}

QuadVec::QuadVec() {
    mData = 0;
    mSize = 0;
    mCapacity = 0;
}

/* Written in asm: the compiler places the epilogue blr after the tail call, but
   the original symbol table splits it off as the following function. */
asm void RootPtr::reset(LayoutRoot* p) {
    nofralloc
    lwz r0, 0(r3)
    cmplw r4, r0
    beqlr
    cmpwi r0, 0
    stw r4, 0(r3)
    beqlr
    mr r3, r0
    li r4, 1
    lwz r12, 4(r3)
    lwz r12, 8(r12)
    mtctr r12
    bctr
}

void RootPtrNoop() {}

LayoutRoot* RootPtr::get() {
    return mPtr;
}

inline void* operator new(size_t, void* p) {
    return p;
}

/* Temporary buffer used while reallocating a vector<Handle> */
template <class T>
inline void Swap(T& a, T& b) {
    T t = a;
    a = b;
    b = t;
}

struct HandleBuf {
    HandleBuf(u32* capRef_) : first(0), count(0), cap(0), capRef(capRef_), pos(0) {}

    Handle* first;
    u32 count;
    u32 cap;
    u32* capRef;
    u32 pos;

    ~HandleBuf() {
        if (first != 0) {
            int n = count;
            count -= n;
            Handle* p = first + n;
            while (n != 0) {
                --p;
                p->~Handle();
                --n;
            }
            operator delete(first);
        }
    }
};

void HandleVec::push_back(const Handle& x) {
    if (mSize < mCapacity) {
        Handle* p = mData + mSize;
        new (p) Handle(x);
        mSize++;
        return;
    }

    if (0x1FFFFFFF - mCapacity < 1) {
        MslError("vector length error");
    }

    HandleBuf buf(&mCapacity);

    size_t cap = mCapacity;
    const size_t grow = mSize + 1 - cap;
    size_t newCap;
    VEC_RECOMMEND_SIZE(newCap, cap, grow, 0x1FFFFFFF);

    void* mem;
    VEC_ALLOCATE(mem, newCap, sizeof(Handle), 0x1FFFFFFF);

    buf.first = (Handle*)mem;
    buf.cap = newCap;
    buf.pos = mSize;

    new (buf.first + buf.pos + buf.count) Handle(x);
    buf.count++;

    Handle* front = buf.first + buf.pos;
    Handle* begin = mData;
    Handle* it = mData + mSize;
    while (it > begin) {
        --front;
        --it;
        new (front) Handle(*it);
        buf.pos--;
        buf.count++;
    }

    Swap(mCapacity, buf.cap);
    Swap(mData, buf.first);
    Swap(mSize, buf.count);

    Handle* b = buf.first + buf.pos;
    Handle* e = b + buf.count;
    while (e > b) {
        --e;
        e->~Handle();
    }
    buf.count = 0;
}
