/* Shared by most REL modules. Reference copy: mg9101 (fn_18_32C00). */
#include "game/common/unk_21650.h"

inline void* operator new(size_t, void* p) {
    return p;
}

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
void fn_8007CD10(const char* name, f32 vol, f32 pitch, f32 pan);
int fn_8006DBC0(void* pad, int mask);
void fn_8006E0C0(void* pad, int);
int fn_8006E280(void* pad);

extern u8 lbl_8026E3B8;
extern f32 lbl_8026E3B4;

struct Vec3f {
    f32 x, y, z;
};

/* One selectable icon: screen rect plus tint. */
struct Color4 {
    u8 r, g, b, a;
};

struct IconRect {
    Vec2f pos;
    Vec2f size;
    Color4 color;
};

struct PaneInfo {
    u8 pad[0x4C];
    f32 w;
    f32 h;
};

struct HandleVec {
    Handle* mData;
    u32 mSize;
    u32 mCap;
};

static inline Handle& HandleVecAt(HandleVec* v, u32 i) {
    if (i >= v->mSize) {
        MslError("vector::at index out of range");
    }
    return v->mData[i];
}

static inline void VecLengthError() {
    MslError("vector length error");
}

inline void VecAllocError() {
    MslError("Memory allocation failure");
}

struct IconVecBuffer {
    IconRect* first;
    u32 count;
    u32 cap;
    u32* capRef;
    u32 pos;
};

struct IconVec {
    IconRect* mData;
    u32 mSize;
    u32 mCap;

    size_t max_size() const { return (size_t)-1 / sizeof(IconRect); }

    size_t RecommendSize(size_t cap, const size_t n) const {
        const size_t m = max_size();
        if (n > m - cap) {
            VecLengthError();
        }
        if (cap < m / 3) {
            return cap + Max(3 * (cap + 1) / 5, n);
        }
        if (cap < m / 3 * 2) {
            return cap + Max((cap + 1) / 2, n);
        }
        return m;
    }

    void push_back(const IconRect& x) {
        if (mSize < mCap) {
            ++mSize;
            mData[mSize - 1] = x;
            return;
        }
        IconVecBuffer buf;
        buf.first = 0;
        buf.count = 0;
        buf.cap = 0;
        buf.capRef = &mCap;
        buf.pos = 0;
        RecommendSize(mCap, 1);

        size_t newcap = RecommendSize(mCap, mSize + 1 - mCap);
        if (newcap > max_size()) {
            VecLengthError();
        }
        IconRect* p = (IconRect*)operator new(newcap * sizeof(IconRect));
        if (p == 0) {
            VecAllocError();
        }
        buf.first = p;
        buf.cap = newcap;
        buf.pos = mSize;
        (buf.first + buf.pos)[buf.count] = x;
        ++buf.count;

        IconRect* dst = buf.first + buf.pos;
        IconRect* first = mData;
        IconRect* last = first + mSize;
        for (IconRect* src = last; src > first;) {
            --src;
            --dst;
            new (dst) IconRect(*src);
            --buf.pos;
            ++buf.count;
        }

        mSize = 0;
        u32 tmpCap = mCap;
        mCap = buf.cap;
        buf.cap = tmpCap;
        IconRect* tmpData = mData;
        mData = buf.first;
        buf.first = tmpData;
        mSize = buf.count;
        buf.count = 0;
        if (&buf != 0) {
            if (buf.first != 0) {
                buf.count = 0;
                operator delete(buf.first);
            }
        }
    }
};

struct CursorState {
    u8 a;
    u8 b;
    u32 c;
    u8 d, e, f;
    f32 g;
    u8 h;

    CursorState& operator=(const CursorState& o) {
        a = o.a;
        b = o.b;
        c = o.c;
        d = o.d;
        e = o.e;
        f = o.f;
        g = o.g;
        h = o.h;
        return *this;
    }
};

CursorState fn_80039450(CursorBase* c);
int fn_80038FF0(CursorBase* c);
int fn_80039250(CursorBase* c);
void fn_80039010(CursorBase* c, int);
void fn_80039060(CursorBase* c, int);
void fn_80038E70(CursorBase* c, void* targets);
void fn_18_21F70(HandCursor* c, int, int);
HandCursor* fn_18_215A0(void* mem, int player, int prio, int);
void SetHandPicture(HandCursor* c, int picture);

/* The original translation unit already holds 0x110 bytes of earlier data and
   the constants 0.0f and 0.5f before our first function. */
char gPoolPad[0xE4] = {1};
void UsePoolString(const char*);
void UsePoolFloat(f32);

__declspec(weak) void PoolPrefix() {
    UsePoolString("bd_icon_no");
    UsePoolString("bd_icon_yes");
    UsePoolString("vector length error");
    UsePoolString("vector::at index out of range");
    UsePoolFloat(0.0f);
    UsePoolFloat(0.5f);
    UsePoolFloat(1.0f);
    UsePoolFloat(2.0f);
}

__declspec(weak) const char* kIconNames[2] = {"bd_icon_no", "bd_icon_yes"};
__declspec(weak) const u8 kIconColors[2][4] = {{0xFF, 0xFF, 0x01, 0x01}, {0x00, 0x00, 0xFF, 0xFF}};

class LayoutBase {
public:
    int IsVisible();                        /* fn_80048FC0 */
    void SetVisible(int);                   /* fn_80048FA0 */
    PaneInfo* FindPane(const char*, int);   /* fn_80048B70 */
    Vec3f* GetPanePos(const char*);         /* fn_80048CB0 */
    Vec3f* GetPos();                        /* fn_80048C00 */

    u8 pad[0xC];
};

class ChoiceMenu : public LayoutBase {
public:
    void ReadInfo(CursorBase* c);
    void PlayAnim(u32 idx, int anim);
    void Run();
    void Enter();
    void Select();
    void Finish();

    Handle* mShowAnim;               /* 0x0C */
    u8 pad10[8];
    HandleVec mAnims[2];             /* 0x18 */
    u8 pad30[4];
    CursorBase* mCursor;             /* 0x34 */
    HandCursor* mHand;               /* 0x38 */
    int mSelected;                   /* 0x3C */
    int mPlayer;                     /* 0x40 */
    int mAnimState[2];               /* 0x44 */
    u8 mMode;                        /* 0x4C */
    u8 mFlag;                        /* 0x4D */
    u8 pad4E[2];
    IconVec mIcons;                  /* 0x50 */
    f32 mTimer;                      /* 0x5C */
    int mState;                      /* 0x60 */
    u8 pad64;
    u8 mUseHand;                     /* 0x65 */
    u8 pad66[2];
    int mHover;                      /* 0x68 */
    u8 pad6C[0x10];
    CursorState mInfo;               /* 0x7C */
    f32 mThreshold;                  /* 0x90 */
    u8 pad94[4];
    void (ChoiceMenu::*mUpdate)();   /* 0x98 */
};

void ChoiceMenu::Run() {
    if (mUpdate) {
        (this->*mUpdate)();
    }
}

#define ADD_ICONS(scale)                                         \
    do {                                                         \
        Vec3f origin = *GetPos();                                \
        for (u32 i = 0; i < 2; i++) {                            \
            PaneInfo* pane = FindPane(kIconNames[i], 1);         \
            Vec3f pos = *GetPanePos(kIconNames[i]);              \
            f32 h = pane->h;                                     \
            f32 w = (scale) * pane->w;                           \
            pos.x = pos.x * (scale);                             \
            IconRect r;                                          \
            r.size.x = w;                                        \
            r.size.y = h;                                        \
            f32 halfH = 0.5f * h;                                \
            f32 halfW = 0.5f * w;                                \
            r.pos.x = origin.x + (pos.x - halfW);                \
            r.pos.y = origin.y + (pos.y + halfH);                \
            r.color.r = kIconColors[i][0];                       \
            r.color.g = kIconColors[i][1];                       \
            r.color.b = kIconColors[i][2];                       \
            r.color.a = kIconColors[i][3];                       \
            mIcons.push_back(r);                                 \
        }                                                        \
    } while (0)

inline void ChoiceMenu::ReadInfo(CursorBase* c) {
    mInfo = fn_80039450(c);
}

inline void ChoiceMenu::PlayAnim(u32 idx, int anim) {
    fn_80049DC0(Handle(HandleVecAt(&mAnims[idx], anim)), 1, 1);
    fn_8004A410(Handle(HandleVecAt(&mAnims[idx], anim)), 1.0f);
    mAnimState[idx] = anim;
}

template <class T>
inline void ResetPtr(T*& slot, T* p) {
    if (p != slot) {
        T* old = slot;
        slot = p;
        delete old;
    }
}

void ChoiceMenu::Enter() {
    mState = 0;
    f32 scale = 1.0f;
    if (lbl_8026E3B8 == 1) {
        scale = 0.75f;
    }

    if (!IsVisible()) {
        SetVisible(1);
        fn_80049DC0(*mShowAnim, 1, 1);
        while (!fn_8004A220(*mShowAnim)) {
            fn_80071590();
        }
    }

    int player = fn_18_1BBC0(mPlayer);
    if (!mUseHand) {
        if (mCursor == 0) {
            ResetPtr(mCursor, new CursorBase(player));
            mInfo = fn_80039450(mCursor);
            fn_80039010(mCursor, 1);
            ADD_ICONS(scale);
            fn_80038E70(mCursor, &mIcons);
            if (!mMode) {
                mCursor->Activate(0, 1);
                while (!fn_8006E280(fn_80070680(player))) {
                    fn_80071590();
                }
            } else {
                mCursor->Activate(1, 0);
                fn_80039060(mCursor, 0);
            }
            mCursor->Reset(1);
        }
    } else {
        if (mHand == 0) {
            HandCursor* h = (HandCursor*)operator new(0x64);
            if (h) {
                h = fn_18_215A0(h, mPlayer, 0x9050, 0);
            }
            ResetPtr(mHand, h);
            mInfo = fn_80039450(mHand);
            mHand->Setup(0);
            if (!mMode) {
                fn_18_21F70(mHand, 0, 1);
            } else {
                fn_18_21F70(mHand, 1, 0);
            }
            fn_80039010(mHand, 1);
            ADD_ICONS(scale);
            fn_80038E70(mHand, &mIcons);
            if (!mMode) {
                fn_18_21F70(mHand, 0, 1);
                while (!fn_8006E280(fn_80070680(player))) {
                    fn_80071590();
                }
            } else {
                fn_18_21F70(mHand, 1, 0);
                fn_80039060(mHand, 0);
            }
            SetHandPicture(mHand, 1);
        }
    }

    mTimer = 0.0f;
    mUpdate = &ChoiceMenu::Select;
}

#define POLL_CURSOR(cursor)                                      \
    do {                                                         \
        hit = fn_80038FF0(cursor);                               \
        int busy = fn_80039250(cursor);                          \
        if (busy == 0) {                                         \
            if (mMode == 0) {                                    \
                if (fn_8006DBC0(fn_80070680(player), -1) & 0x900) { \
                    if (hit != -1) {                             \
                        next = hit;                              \
                    }                                            \
                }                                                \
            } else {                                             \
                if (mTimer > mThreshold) {                       \
                    if (hit != -1) {                             \
                        next = hit;                              \
                    }                                            \
                }                                                \
            }                                                    \
        }                                                        \
        if (fn_8006DBC0(fn_80070680(player), -1) & 0x400) {      \
            hit = 0;                                             \
            next = 0;                                            \
            if ((u32)busy == 1) {                                \
                fn_80039060(cursor, 0);                          \
            }                                                    \
        }                                                        \
        if (mMode == 1) {                                        \
            fn_80039060(cursor, mFlag == 1);                     \
        }                                                        \
    } while (0)

void ChoiceMenu::Select() {
    mState = 1;
    int player = fn_18_1BBC0(mPlayer);
    int next = -1;
    int hit;
    if (!mUseHand) {
        POLL_CURSOR(mCursor);
    } else {
        POLL_CURSOR(mHand);
    }

    if (hit != -1) {
        for (int i = 0; i < 2; i++) {
            if (hit == i) {
                if (mAnimState[i] != 0) {
                    PlayAnim(i, 0);
                }
            } else if (mAnimState[i] != 3) {
                PlayAnim(i, 3);
            }
        }
        if (hit != mHover) {
            if (mMode == 0) {
                fn_8006E0C0(fn_80070680(player), 0);
            }
            fn_8007CD10("SEQ_SESY001", 0.0f, 1.0f, 0.0f);
            mHover = hit;
        }
    } else {
        for (int i = 0; i < 2; i++) {
            if (mAnimState[i] != 3) {
                PlayAnim(i, 3);
            }
        }
        mHover = -1;
    }

    mTimer += lbl_8026E3B4;
    if (next != -1) {
        mSelected = next;
        mUpdate = &ChoiceMenu::Finish;
    }
}
