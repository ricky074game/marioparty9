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
    int* InsertImpl(int* pos, const int& x, FalseType);

    int* mData;
    u32 mSize;
    u32 mCapacity;
};


/* ---------------------------------------------------------------------- */
/* Hand cursor objects (one per player), ordered per screen by priority.  */

struct Mtx34 {
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
};

struct Vec2f {
    f32 x, y;
};

class HandCursor;

template <class T>
class PtrVector {
public:
    PtrVector();  /* fn_18_20F30 */
    ~PtrVector(); /* fn_18_1D7C0 */

    T** begin() { return (T**)mImpl.mData; }
    T** end() { return (T**)mImpl.mData + mImpl.mSize; }
    u32 size() const { return mImpl.mSize; }
    T*& operator[](u32 i) { return ((T**)mImpl.mData)[i]; }
    T* back() { return ((T**)mImpl.mData)[mImpl.mSize - 1]; }
    void insert(T** pos, T* const& x) { mImpl.InsertImpl((int*)pos, (const int&)x, FalseType()); }
    void erase(T** pos) {
        memmove(pos, pos + 1, (end() - pos - 1) * sizeof(T*));
        --mImpl.mSize;
    }

    IntVector mImpl;
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

    HandCursor* Back(int i) { return mLists[i].size() != 0 ? mLists[i][mLists[i].size() - 1] : 0; }

    int mRefCount;
    PtrVector<HandCursor> mLists[4];
    CursorList2 mLists2[4];

    virtual ~CursorManager(); /* fn_18_20F50 */
};

class HandCursor {
public:
    ~HandCursor();
    void Init();
    void Setup(void* res);
    void Update(TargetPane* pane);
    static void UpdateCallback(void* self, int, void* pane); /* fn_18_22230 */

    /* main.dol */
    const Vec2f* GetPos();               /* fn_800385B0 */
    const Vec2f* GetScale();             /* fn_800385D0 */
    void Activate(u8 a, u8 b);           /* fn_800385F0 */
    void Reset(int);                     /* fn_80038710 */
    int GetDefaultPicture();             /* fn_80038750 */
    void ApplyScale(f32);                /* fn_80038790 */
    BOOL IsHeld();                       /* fn_80039040 */
    CursorInfo GetInfo();                /* fn_80039450 */

    PtrVector<HandCursor>& List() { return mMgr->mLists[mIndex]; }

    void SetPicture(int picture);
    void SetScale(f32 scale) {
        ApplyScale(scale);
        mScale = scale;
    }

    u32 m0;
    CursorLayout mLayout;
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

#endif
