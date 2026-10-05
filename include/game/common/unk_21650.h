#ifndef GAME_COMMON_UNK_21650_H
#define GAME_COMMON_UNK_21650_H

#include "types.h"

typedef unsigned long size_t;

struct FILE {
    u8 data[0x50];
};

extern "C" FILE __files[];
extern "C" int fprintf(FILE* stream, const char* format, ...);
extern "C" void abort(void);
extern "C" void* memmove(void* dst, const void* src, size_t n);

void* operator new(size_t);
void operator delete(void*);

inline void MslError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

template <class T>
inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

struct FalseType {};

class IntVector {
public:
    size_t max_size() const { return (size_t)-1 / sizeof(int); }
    size_t RecommendSize(size_t cap, const size_t n) const {
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
    int* begin() { return mData; }
    int* end() { return mData + mSize; }
    u32 size() const { return mSize; }
    int& operator[](u32 i) { return mData[i]; }
    int& back() { return mData[mSize - 1]; }
    int* InsertImpl(int* pos, const int& x, FalseType);
    void erase(int* pos) {
        memmove(pos, pos + 1, (end() - pos - 1) * sizeof(int));
        --mSize;
    }

    int* mData;
    u32 mSize;
    u32 mCapacity;
};


/* ---------------------------------------------------------------------- */
/* Hand cursor objects (one per player), ordered per screen by priority.  */

struct Mtx34 {
    Mtx34() {}
    Mtx34(f32 m00, f32 m01, f32 m02, f32 m03, f32 m10, f32 m11, f32 m12, f32 m13, f32 m20, f32 m21,
          f32 m22, f32 m23) {
        m[0][0] = m00;
        m[0][1] = m01;
        m[0][2] = m02;
        m[0][3] = m03;
        m[1][0] = m10;
        m[1][1] = m11;
        m[1][2] = m12;
        m[1][3] = m13;
        m[2][0] = m20;
        m[2][1] = m21;
        m[2][2] = m22;
        m[2][3] = m23;
    }

    f32 m[3][4];
};

extern "C" void PSMTXScale(Mtx34* m, f32 x, f32 y, f32 z);           /* fn_801923D0 */
extern "C" void PSMTXConcat(const Mtx34* a, const Mtx34* b, Mtx34* ab); /* fn_80191D30 */

extern f32 lbl_8026DAB4;

struct CursorPane {
    u8 pad[0xBB];
    u8 flags;

    void SetVisible(bool visible) { flags = (u8)(flags & ~1) | (visible ? 1 : 0); }
};

struct TargetPane {
    u8 pad0[4];
    Mtx34 mtx;
    u8 pad34[0x4C - 0x34];
    f32 alpha;
    u8 flag80 : 1;
    u8 flag40 : 1;
};

class ResHandle {
public:
    ResHandle(void* src); /* fn_80086B60 */
    ~ResHandle();         /* fn_80086F10 */
    u32 m0;
    u32 m4;
};

typedef void (*LayoutCallback)(void*, int, void*);

class CursorLayout {
public:
    ~CursorLayout();                                               /* fn_80046040 */
    void Load(const char* res, const char* name, int, int);        /* fn_800475D0 */
    void Load(const ResHandle& res, const char* name, int);        /* fn_80047E60 */
    void SetPriority(int);                                         /* fn_80048860 */
    void SetLayer(int);                                            /* fn_800489D0 */
    CursorPane* FindPane(const char* name, int recurse);           /* fn_80048B50 */
    void SetVisible(int);                                          /* fn_80048FA0 */
    void SetAnimFrame(const char* name, f32 frame, f32 speed);     /* fn_8004A5F0 */
    void SetUserData(void*);                                       /* fn_8004B5D0 */
    void SetCallback(int, LayoutCallback);                         /* fn_8004B5E0 */

    void* mPtr;
};

struct CursorInfo {
    u8 active;
    u8 visible;
    u8 pad2[8];
    u8 useOverride;
    u8 padB;
    f32 scale;
    u8 pad10[4];
};

struct Vec2f {
    f32 x, y;
};

class HandCursor;

/* vector<T*> is implemented on top of the vector<int> code */
template <class T>
class PtrVector {
public:
    PtrVector();  /* fn_18_20F30 */
    ~PtrVector(); /* fn_18_1D7C0 */

    IntVector& Impl() { return *reinterpret_cast<IntVector*>(this); }
    T** begin() { return mData; }
    T** end() { return mData + mSize; }
    u32 size() const { return mSize; }
    T*& operator[](u32 i) { return mData[i]; }
    void insert(T** pos, T* const& x) { Impl().InsertImpl((int*)pos, (const int&)x, FalseType()); }
    void InsertBefore(T** pos, T* const& x) { insert(pos, x); }
    void erase(T** pos) { Impl().erase((int*)pos); }
    void remove(T* x) {
        for (T** it = begin(); it != end(); ++it) {
            if (*it == x) {
                erase(it);
                return;
            }
        }
    }
    T*& back() { return mData[mSize - 1]; }

    T** mData;
    u32 mSize;
    u32 mCapacity;
};

class CursorList2 {
public:
    CursorList2();  /* fn_18_1D840 */
    ~CursorList2(); /* fn_18_1D860 */
    u32 m0, m4, m8;
};

template <class T>
class Singleton {
public:
    Singleton() { sInstance = static_cast<T*>(this); }
    static T* sInstance;
};

class CursorManager : public Singleton<CursorManager> {
public:
    CursorManager() : mRefCount(0) {}

    HandCursor* FindActive(int idx);
    HandCursor* Back(int i) { return mLists[i].size() != 0 ? mLists[i].back() : 0; }

    int mRefCount;
    PtrVector<HandCursor> mLists[4];
    CursorList2 mLists2[4];

    virtual ~CursorManager(); /* fn_18_20F50 */
};

/* HandCursor::SetPicture (fn_18_21DF0) is inlined by its callers, but its
   string literals land in the translation unit's string pool, which only
   happens for literals written in the calling function itself. */
#define HAND_CURSOR_SET_PICTURE(cursor, picture)                                  do {                                                                              HandCursor* c_ = (cursor);                                                    int p_ = (picture);                                                           c_->Reset(0);                                                                 c_->mPicture = p_;                                                            if (c_->mEnabled) {                                                               c_->FindPane("Picture_00", 1)->SetVisible(false);                     c_->FindPane("Picture_01", 1)->SetVisible(false);                     c_->FindPane("Picture_02", 1)->SetVisible(false);                     switch (c_->mPicture) {                                                       case 1:                                                                           c_->FindPane("Picture_00", 1)->SetVisible(true);                      break;                                                                    case 2:                                                                           c_->FindPane("Picture_01", 1)->SetVisible(true);                      break;                                                                    case 3:                                                                           c_->FindPane("Picture_02", 1)->SetVisible(true);                      break;                                                                    }                                                                         }                                                                         } while (0)

int fn_18_1BBC0(int player);
BOOL fn_18_1BC20(int player);
int fn_18_1BD40(int player);

struct PadState {
    u8 pad[0x5E];
    s8 m5E;
};
void* fn_80070680(int pad);
u8 fn_8006DE50(void* pad);
PadState* fn_8006E330(void* pad, int);
u32 fn_80070DC0();

extern "C" int sprintf(char* buf, const char* fmt, ...);

/* main.dol cursor base */
class CursorBase {
public:
    CursorBase(int port);                /* fn_800385A0 */
    const Vec2f* GetPos();               /* fn_800385B0 */
    const Vec2f* GetScale();             /* fn_800385D0 */
    void Activate(u8 a, u8 b);           /* fn_800385F0 */
    void Reset(int);                     /* fn_80038710 */
    int GetDefaultPicture();             /* fn_80038750 */
    void ApplyScale(f32);                /* fn_80038790 */
    BOOL IsHeld();                       /* fn_80039040 */
    CursorInfo GetInfo();                /* fn_80039450 */

    u32 m0;
};

class HandCursor : public CursorBase, public CursorLayout {
public:
    ~HandCursor();
    void Init();
    void Setup(void* res);
    void Update(TargetPane* pane);
    static void UpdateCallback(void* self, int, void* pane); /* fn_18_22230 */

    PtrVector<HandCursor>& List() { return mMgr->mLists[mIndex]; }

    void SetScale(f32 scale) {
        ApplyScale(scale);
        mScale = scale;
    }

    CursorManager* mMgr;
    int mPlayer;
    int mPriorityLayer;
    Mtx34 mMtx;
    int mPicture;
    f32 mScale;
    u8 mEnabled;
    f32 m50;
    f32 m54;
    u8 m58;
    u8 m59;
    int mIndex;
    int mOrder;
};

inline HandCursor* CursorManager::FindActive(int idx) {
    for (int i = 0; i < mLists[idx].size(); i--) {
        if (mLists[idx][i]->mPicture != 0) {
            return mLists[idx][i];
        }
    }
    return 0;
}

#endif
