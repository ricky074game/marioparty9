/* Shared by most REL modules. Reference copy: mg9101 (fn_18_31F20). */
#include "types.h"
#include "game/common/unk_31F20.h"

typedef unsigned long size_t;

struct FILE {
    u8 data[0x50];
};

extern "C" FILE __files[];
extern "C" int fprintf(FILE* stream, const char* format, ...);
extern "C" __declspec(noreturn) void abort(void);

void* operator new(size_t);
void operator delete(void*);
inline void* operator new(size_t, void* p) { return p; }

inline void MslError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

static inline void LengthError() {
    MslError("vector length error");
}

inline void AllocError() {
    MslError("Memory allocation failure");
}

template <class T>
inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

/* ---------------------------------------------------------------------- */
/* Static data: tuning values derived from a shared table.                 */

extern f32 lbl_18_data_3450[5];

static f32 sTune[7];

static void InitTune() {
    f32 e = 10.0f * lbl_18_data_3450[4];
    sTune[0] = 10.0f * lbl_18_data_3450[0];
    sTune[1] = 10.0f * lbl_18_data_3450[1];
    sTune[2] = 10.0f * lbl_18_data_3450[2];
    sTune[3] = 10.0f * lbl_18_data_3450[3];
    sTune[4] = e;
    sTune[5] = 5.0f * e;
    sTune[6] = 4.0f * -e;
}

__declspec(section ".ctors") static void (*const sInitTune)() = InitTune;

/* ---------------------------------------------------------------------- */

int fn_80042D70(void* p);

class noncopyable {
protected:
    noncopyable() {}
    ~noncopyable() {}

private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

template <class T>
class ScopedPtr : noncopyable {
public:
    ScopedPtr() : mPtr(0) {}
    ~ScopedPtr() { delete mPtr; }
    T* operator->() const { return mPtr; }

    void reset(T* p = 0) {
        if (p != mPtr) {
            T* old = mPtr;
            mPtr = p;
            delete old;
        }
    }

    T* mPtr;
};

class Handle {
public:
    Handle(const Handle& o) : mPtr(fn_80042D70(o.mPtr) ? o.mPtr : 0) {}
    virtual ~Handle() {}

    void* mPtr;
};

/* MSL-style vector: three levels of inline destructors */
template <class T>
class VectorRep {
public:
    VectorRep() : mData(0), mSize(0), mCap(0) {}
    ~VectorRep() {
        if (mData) {
            u32 n = mSize;
            T* p = mData + n;
            mSize -= n;
            for (; n != 0; --n) {
                (--p)->~T();
            }
            operator delete(mData);
        }
    }

    T* mData;
    u32 mSize;
    u32 mCap;
};

template <class T>
class VectorBase : public VectorRep<T> {
public:
    ~VectorBase() {}
};

template <class T>
class Vector : public VectorBase<T> {
public:
    /* temporary storage used while growing */
    struct Buf : public VectorRep<T> {
        u32& capRef;
        u32 pos;

        Buf(u32& cap) : capRef(cap), pos(0) {}
        ~Buf() {}
        T* begin() { return this->mData + pos; }
        T* end() { return this->mData + pos + this->mSize; }
    };

    static size_t max_size() { return (size_t)-1 / sizeof(T); }
    static size_t RecommendSize(size_t cap, const size_t n) {
        const size_t m = max_size();
        if (n > m - cap) {
            LengthError();
        }
        if (cap < m / 3) {
            return cap + Max(3 * (cap + 1) / 5, n);
        }
        if (cap < m / 3 * 2) {
            return cap + Max((cap + 1) / 2, n);
        }
        return m;
    }
    static T* Allocate(size_t n) {
        if (n > max_size()) {
            LengthError();
        }
        T* p = (T*)operator new(n * sizeof(T));
        if (p == 0) {
            AllocError();
        }
        return p;
    }

    void push_back(const T& x) {
        size_t sz = this->mSize;
        size_t cap = this->mCap;
        if (sz < cap) {
            new (this->mData + sz) T(x);
            ++this->mSize;
        } else {
            RecommendSize(cap, 1);
            Buf buf(this->mCap);
            size_t n = this->mSize + 1 - this->mCap;
            size_t newCap = RecommendSize(this->mCap, n);
            buf.mData = Allocate(newCap);
            buf.mCap = newCap;
            buf.pos = this->mSize;
            new (buf.end()) T(x);
            ++buf.mSize;
            T* dst = buf.begin();
            T* src = this->mData + this->mSize;
            while (src > this->mData) {
                --dst;
                --src;
                new (dst) T(*src);
                --buf.pos;
                ++buf.mSize;
            }
            size_t t = this->mCap;
            this->mCap = buf.mCap;
            buf.mCap = t;
            T* tp = this->mData;
            this->mData = buf.mData;
            buf.mData = tp;
            size_t ts = this->mSize;
            this->mSize = buf.mSize;
            buf.mSize = ts;
            T* b = buf.begin();
            T* e = buf.end();
            while (e > b) {
                (--e)->~T();
            }
            buf.mSize = 0;
        }
    }
};

typedef Vector<Handle> HandleVector;

/* vector of trivially destructible elements: no element destruction */
template <class T>
class PodVectorRep {
public:
    PodVectorRep() : mData(0), mSize(0), mCap(0) {}
    ~PodVectorRep() {
        if (mData) {
            clear();
            operator delete(mData);
        }
    }

    static void shrink(u32& size, u32 n) { size -= n; }
    void clear() { shrink(mSize, mSize); }

    T* mData;
    u32 mSize;
    u32 mCap;
};

template <class T>
class PodVectorBase : public PodVectorRep<T> {
public:
    ~PodVectorBase() {}
};

template <class T>
class PodVector : public PodVectorBase<T> {
public:
    ~PodVector() {}
};

/* vector<Handle>::push_back, expanded in place: the "vector length error"
   literals have to be part of the calling function's string pool. */
#define HANDLE_VECTOR_PUSH_BACK(VEC, VALUE)                                \
    do {                                                                   \
        HandleVector& v_ = (VEC);                                          \
        const Handle& x_ = (VALUE);                                        \
        size_t sz_ = v_.mSize;                                             \
        size_t cap_ = v_.mCap;                                             \
        if (sz_ < cap_) {                                                  \
            new (v_.mData + sz_) Handle(x_);                               \
            ++v_.mSize;                                                    \
        } else {                                                           \
            if (1 > HandleVector::max_size() - cap_) {                     \
                MslError("vector length error");                           \
            }                                                              \
            HandleVector::Buf buf_(v_.mCap);                               \
            size_t n_ = v_.mSize + 1 - v_.mCap;                            \
            if (n_ > HandleVector::max_size() - v_.mCap) {                 \
                MslError("vector length error");                           \
            }                                                              \
            size_t newCap_ = HandleVector::RecommendSize(v_.mCap, n_);     \
            if (newCap_ > HandleVector::max_size()) {                      \
                MslError("vector length error");                           \
            }                                                              \
            buf_.mData = HandleVector::Allocate(newCap_);                  \
            buf_.mCap = newCap_;                                           \
            buf_.pos = v_.mSize;                                           \
            new (buf_.end()) Handle(x_);                                   \
            ++buf_.mSize;                                                  \
            Handle* dst_ = buf_.begin();                                   \
            Handle* src_ = v_.mData + v_.mSize;                            \
            while (src_ > v_.mData) {                                      \
                --dst_;                                                    \
                --src_;                                                    \
                new (dst_) Handle(*src_);                                  \
                --buf_.pos;                                                \
                ++buf_.mSize;                                              \
            }                                                              \
            size_t t_ = v_.mCap;                                           \
            v_.mCap = buf_.mCap;                                           \
            buf_.mCap = t_;                                                \
            Handle* tp_ = v_.mData;                                        \
            v_.mData = buf_.mData;                                         \
            buf_.mData = tp_;                                              \
            size_t ts_ = v_.mSize;                                         \
            v_.mSize = buf_.mSize;                                         \
            buf_.mSize = ts_;                                              \
            Handle* b_ = buf_.begin();                                     \
            Handle* e_ = buf_.end();                                       \
            while (e_ > b_) {                                              \
                (--e_)->~Handle();                                         \
            }                                                              \
            buf_.mSize = 0;                                                \
        }                                                                  \
    } while (0)

/* ---------------------------------------------------------------------- */
/* Main.dol layout / cursor objects used by the menu.                      */

class Pane;

class ResManager {
public:
    virtual void v0();
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
    virtual Pane* FindPane(const char* name, int recurse); /* 0x3c */
};

class Layout {
public:
    Layout(const char* arc, const char* name, int, void* heap); /* fn_80046CF0 */
    ~Layout();                                                  /* fn_80046040 */
    void SetPriority(int);                                      /* fn_80048860 */
    void SetVisible(int);                                       /* fn_80048FA0 */
    void* GetDrawInfo();                                        /* fn_800486C0 */
    ResManager* GetRes();                                       /* fn_800486E0 */
    Handle GetAnim(const char* anim, const char* pane);         /* fn_80049110 */

    void* mPtr;
};

void* fn_8004E010(int);

class Process {
public:
    Process();          /* fn_80071350 */
    virtual ~Process(); /* fn_80071420 */

    void* x4;
};

struct CursorState {
    CursorState(); /* fn_80039510 */
    f32 f[5];
};

class CursorBase {
public:
    void Deactivate();                       /* fn_80038F00 */
    CursorState Restore(const CursorState*); /* fn_800392C0 */
    u32 m0;
};

class HandCursor : public CursorBase {
public:
    ~HandCursor();
};

struct IconRect {
    f32 x, y, w, h;
    u8 r, g, b, a;
};

BOOL fn_18_1BC20(int player);

class ChoiceMenu;
typedef void (ChoiceMenu::*UpdateFn)();
extern UpdateFn lbl_80237470;

/* Yes/No choice menu with a hand cursor or pad cursor. */
class ChoiceMenu : public Layout, public Process {
public:
    ChoiceMenu(int player, u8 flag, u8 useHand);

    HandleVector mShowAnims;           /* 0x0C */
    HandleVector mAnims[2];            /* 0x18 */
    u8 m30;                            /* 0x30 */
    ScopedPtr<CursorBase> mCursor;     /* 0x34 */
    ScopedPtr<HandCursor> mHand;       /* 0x38 */
    int mSelected;                     /* 0x3C */
    int mPlayer;                       /* 0x40 */
    int mAnimState[2];                 /* 0x44 */
    u8 mMode;                          /* 0x4C */
    u8 mFlag;                          /* 0x4D */
    PodVector<IconRect> mIcons;           /* 0x50 */
    f32 mTimer;                        /* 0x5C */
    int mState;                        /* 0x60 */
    u8 m64;                            /* 0x64 */
    u8 mUseHand;                       /* 0x65 */
    int mHover;                        /* 0x68 */
    ScopedPtr<TextBox> mTexts[4];      /* 0x6C */
    CursorState mInfo;                 /* 0x7C */
    f32 mThreshold;                    /* 0x90 */
    virtual ~ChoiceMenu();             /* vptr at 0x94 */
    UpdateFn mUpdate;                  /* 0x98 */
};

ChoiceMenu::ChoiceMenu(int player, u8 flag, u8 useHand)
    : Layout("/layout/icon", "ns_icon_yesno", 0, fn_8004E010(3)), m30(0), mSelected(-1),
      mPlayer(player), mFlag(flag), mState(-1), m64(0), mUseHand(useHand), mHover(-1),
      mUpdate(lbl_80237470) {
    static const char* sInName = "in";
    static const char* sAnimNames[4] = {"on", "press", "press_out", "normal_out"};
    static const char* sPaneNames[2] = {"bd_icon_no", "bd_icon_yes"};
    static const char* sMsgIds[4] = {"me00_002", "me00_002", "me00_001", "me00_001"};
    static const char* sTextNames[4] = {"text_no_sdw", "text_no", "text_yes_sdw", "text_yes"};

    mMode = fn_18_1BC20(mPlayer);

    for (int z = 0; z < 1; z++) {
        mShowAnims.push_back(GetAnim(sInName, 0));
    }
    for (int i = 0; i < 2; i++) {
        for (u32 j = 0; j < 4; j++) {
            mAnims[i].push_back(GetAnim(sAnimNames[j], sPaneNames[i]));
        }
    }
    for (int k = 0; k < 4; k++) {
        mTexts[k].reset(new TextBox(0));
        TextBox* text = mTexts[k].mPtr;
        text->Attach(GetDrawInfo(), GetRes()->FindPane(sTextNames[k], 1));
        mTexts[k]->SetText(sMsgIds[k]);
        mTexts[k]->SetAlign(0, 0);
        mTexts[k]->v52();
        if (k % 2 == 0) {
            Color c1(0, 0, 0, 0x40);
            mTexts[k]->SetColorTop(&c1);
            Color c2(0, 0, 0, 0x40);
            mTexts[k]->SetColorBottom(&c2);
        } else {
            Color c3(0xFF, 0xFF, 0xFF, 0xFF);
            mTexts[k]->SetColorTop(&c3);
        }
    }
    mAnimState[0] = -1;
    mAnimState[1] = -1;
    SetPriority(0x9041);
    SetVisible(0);
    mTimer = 0.0f;
    mThreshold = 0.5f;
}

ChoiceMenu::~ChoiceMenu() {
    if (mUseHand == 0) {
        if (mCursor.mPtr) {
            mCursor.mPtr->Deactivate();
            mCursor.mPtr->Restore(&mInfo);
        }
    } else {
        if (mHand.mPtr) {
            mHand.mPtr->Deactivate();
            mHand.mPtr->Restore(&mInfo);
        }
    }
    for (int k = 0; k < 4; k++) {
        mTexts[k]->Release();
    }
    mIcons.clear();
}
