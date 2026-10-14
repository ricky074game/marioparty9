/* Shared by most REL modules. Reference copy: mg9101 (fn_18_20430). */
#include "types.h"

#pragma pool_data off
#pragma pool_strings on

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

extern const char kVectorLengthError[];
const char kVectorLengthError[] = "vector length error";
extern const char kAllocFailure[];
const char kAllocFailure[] = "Memory allocation failure";

inline void LengthError() {
    fprintf(&__files[2], kVectorLengthError);
    abort();
}

template <class T>
inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

extern f32 lbl_8026DAB4;
extern u8 lbl_8026E3B8;

/* ---------------------------------------------------------------------- */
/* Layout handles                                                         */

struct Object;
bool fn_80042D70(Object* obj);

class Handle {
public:
    Handle(Object* obj) : mObj(fn_80042D70(obj) ? obj : 0) {}
    Handle(const Handle& other) : mObj(fn_80042D70(other.mObj) ? other.mObj : 0) {}
    virtual ~Handle() {}

    Object* mObj;
};

struct Rect {
    Rect() {}
    Rect(const Rect& o) : l(o.l), t(o.t), r(o.r), b(o.b) {}
    f32 l, t, r, b;
};

struct Pane;
Rect fn_8016FCB0(Pane* pane);

class HandleList {
public:
    Handle Get(int idx);                          /* fn_18_2B020 */
    void fn_18_2AE90(int idx);
    Pane* FindPane(const char* name, int recurse); /* fn_80048B50 */

    int unk0;
    Handle* mEntries;
    int mCount;
};

bool fn_8004A220(const Handle& handle);
void fn_8004A410(const Handle& handle, f32 frame);
void fn_8004A4B0(const Handle& handle, f32 frame);
f32 fn_8004A550(const Handle& handle);
void fn_18_2B0C0(HandleList* list, int idx, int arg);

/* ---------------------------------------------------------------------- */
/* Vector                                                                 */

template <class T>
struct SplitBase {
    SplitBase() : mFirst(0), mCount(0), mCap(0) {}
    ~SplitBase() {
        if (mFirst != 0) {
            mCount = 0;
            operator delete(mFirst);
        }
    }

    T* mFirst;
    u32 mCount;
    u32 mCap;
};

template <class T>
struct SplitBuf : SplitBase<T> {
    SplitBuf(u32* capRef) : mCapRef(capRef), mRemaining(0) {}

    u32* mCapRef;
    u32 mRemaining;
};

template <class T>
struct Vector {
    static size_t max_size() { return (size_t)-1 / sizeof(T); }
    size_t RecommendSize(const size_t n) {
        size_t cap = mCapacity;
        const size_t m = max_size();
        if (n > m - cap) {
            LengthError();
        }
        size_t r;
        if (cap < m / 3) {
            r = cap + Max(3 * (cap + 1) / 5, n);
        } else if (cap < m / 3 * 2) {
            r = cap + Max((cap + 1) / 2, n);
        } else {
            r = m;
        }
        return r;
    }
    static T* Allocate(size_t n) {
        if (n > max_size()) {
            LengthError();
        }
        T* p = (T*)operator new(n * sizeof(T));
        if (p == 0) {
            MslError(kAllocFailure);
        }
        return p;
    }
    ~Vector() {
        if (mData != 0) {
            mSize -= (int)mSize;
            operator delete(mData);
        }
    }

    T* mData;
    u32 mSize;
    u32 mCapacity;
};

template <class T>
inline void CopyBackward(T* first, T* last, T* dst, SplitBuf<T>& buf) {
    while (last > first) {
        --last;
        --dst;
        new (dst) T(*last);
        --buf.mRemaining;
        ++buf.mCount;
    }
}

template <class T>
inline void PushBack(Vector<T>& v, const T& x) {
    Vector<T>* self = &v;
    if (self->mSize < self->mCapacity) {
        ++self->mSize;
        T* p = self->mData + (self->mSize - 1);
        *p = x;
        return;
    }
    if (1 > Vector<T>::max_size() - self->mCapacity) {
        LengthError();
    }
    SplitBuf<T> buf(&self->mCapacity);
    size_t n = self->mSize + 1 - self->mCapacity;
    size_t newcap = self->RecommendSize(n);
    buf.mFirst = Vector<T>::Allocate(newcap);
    buf.mCap = newcap;
    buf.mRemaining = self->mSize;
    new (buf.mFirst + self->mSize + buf.mCount) T(x);
    ++buf.mCount;
    CopyBackward(self->mData, self->mData + self->mSize, buf.mFirst + buf.mRemaining, buf);
    self->mSize = 0;
    u32 c = self->mCapacity;
    self->mCapacity = buf.mCap;
    buf.mCap = c;
    T* d = self->mData;
    self->mData = buf.mFirst;
    buf.mFirst = d;
    self->mSize = buf.mCount;
    buf.mCount = 0;
}

/* ---------------------------------------------------------------------- */
/* Bit array                                                              */

struct BitRef {
    BitRef(u32* word, u32 mask) : mWord(word), mMask(mask) {}
    bool Test() const { return (*mWord & mMask) != 0; }
    u32* mWord;
    u32 mMask;
};

struct BitArray {
    u32* mData;
    BitRef Ref(u32 idx) { return BitRef(mData + (idx >> 5), 1 << (idx & 31)); }
};

/* ---------------------------------------------------------------------- */
/* Hand cursor manager (see unk_21650.h)                                   */

class HandCursor {
public:
    int GetHovered();                      /* fn_80038FF0 */
    void SetHitBoxes(void* vec);           /* fn_80038E70 */
};

void fn_18_21F30(HandCursor* cursor, f32 scale);

class PtrVector {
public:
    PtrVector();
    ~PtrVector();

    u32 size() const { return mSize; }
    HandCursor*& back() { return mData[mSize - 1]; }

    HandCursor** mData;
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
    ~Singleton() { sInstance = 0; }
    static T* sInstance;
};

class CursorManager : public Singleton<CursorManager> {
public:
    CursorManager();
    HandCursor* Back(int i);

    int mRefCount;
    PtrVector mLists[4];
    CursorList2 mLists2[4];

    virtual ~CursorManager();
};

template <>
CursorManager* Singleton<CursorManager>::sInstance;

/* ---------------------------------------------------------------------- */
/* Misc externals                                                         */

int fn_18_1BBC0(int player);
void* fn_80070680(int pad);
int fn_8006DBC0(void* pad, int mask);
void fn_8006DDE0(void* pad, u8 value);
void fn_800395D0(int);
void fn_80039660(int);
void fn_800715E0(f32);
int fn_8007CCE0(int player, int);
int fn_8007CD10(const char* name, f32, f32, f32);
void fn_8007D5C0(const char* name, int id, f32, f32, f32);
bool fn_8007E1E0(int id);
void fn_8007D8B0(int id, f32);

/* ---------------------------------------------------------------------- */
/* Cursor menu: waits for the intro, then waits for every player's pick   */

struct HitBox {
    f32 v[4]; /* x, y, w, h */
    s8 c[4];  /* r, g, b, a */
};

struct PlayerState {
    f32 x, y;
    int state;
};

#define PLAY_SELECT_SOUND(channel)                                                        do {                                                                                      if (mSound >= 0) {                                                                        if (fn_8007E1E0(mSound)) {                                                                fn_8007D8B0(mSound, 0.0f);                                                        }                                                                                     mSound = -1;                                                                      }                                                                                     mSound = fn_8007CD10("SEQ_SEMG0000_08", 0.0f, 1.0f, 0.0f);                            fn_8007D5C0("SEQ_SEMG0000_08_RM", channel, 0.0f, 1.0f, 0.0f);                     } while (0)

#define LAYOUT(p) (*(HandleList**)((u8*)mLists + ((p) + 1) * 4))

class CursorMenu {
public:
    void UpdateIntro();   /* fn_18_20430 */
    void UpdateSelect();  /* fn_18_20980 */

    int unk0;
    int unk4;
    int mState;
    int unkC;
    int mMode;
    int* mPlayers;
    u32 mPlayerCount;
    int unk1C;
    HandleList** mLists;
    int unk24;
    int unk28;
    int* mCursorPlayers;
    u32 mCursorCount;
    u8 pad34[0x4C - 0x34];
    PlayerState* mPlayerStates;
    u8 pad50[0x58 - 0x50];
    u32 mReadyMask;
    u8 pad5C[0x78 - 0x5C];
    HandCursor** mCursors;
    u8 pad7C[0x84 - 0x7C];
    BitArray mBits;
    u8 pad88[0xE0 - 0x88];
    int mSound;
};

void CursorMenu::UpdateIntro() {
    Handle first = mLists[0]->Get(0);
    bool ready = false;
    if (fn_8004A220(first)) {
        Handle second = mLists[1]->Get(0);
        if (fn_8004A220(second)) {
            ready = true;
        }
    }
    if (ready) {
        if (mMode != 0) {
            mLists[0]->fn_18_2AE90(2);
            fn_18_2B0C0(mLists[0], 2, 1);
        }
        Vector<HitBox> boxes;
        boxes.mData = 0;
        boxes.mSize = 0;
        boxes.mCapacity = 0;
        fn_800395D0(1);
        fn_80039660(1);
        for (u32 i = 0; i < mCursorCount; i++) {
            if (mBits.Ref(i).Test()) {
                boxes.mSize -= (int)boxes.mSize;
                fn_8006DDE0(fn_80070680(fn_18_1BBC0(mCursorPlayers[i])), 1);
                Pane* pane = LAYOUT(i)->FindPane("a", 1);
                HitBox box;
                box.c[3] = -1;
                box.c[2] = -1;
                box.c[1] = -1;
                box.c[0] = -1;
                box.v[3] = 0.0f;
                box.v[2] = 0.0f;
                box.v[1] = 0.0f;
                box.v[0] = 0.0f;
                if (pane) {
                    Rect rect = fn_8016FCB0(pane);
                    box.v[3] = __fabs(rect.b - rect.t);
                    box.v[0] = rect.l + mPlayerStates[i].x;
                    box.v[1] = rect.t + mPlayerStates[i].y;
                    box.v[2] = rect.r - rect.l;
                }
                PushBack(boxes, box);
                mCursors[i]->SetHitBoxes(&boxes);
                mReadyMask |= 1 << i;
            }
        }
        mState = 3;
    }
}

void CursorMenu::UpdateSelect() {
    u32 done = 0;
    for (int i = 0; i < mCursorCount; i++) {
        int channel = fn_8007CCE0(fn_18_1BBC0(mCursorPlayers[i]), 0);
        if (!mBits.Ref(i).Test()) {
            if (mPlayerStates[i].state != 2) {
                int mask = 0x800;
                if (mMode == 1) {
                    mask = 0x100;
                }
                if (fn_8006DBC0(fn_80070680(fn_18_1BBC0(mCursorPlayers[i])), mask)) {
                    LAYOUT(i)->fn_18_2AE90(3);
                    mPlayerStates[i].state = 2;
                    PLAY_SELECT_SOUND(channel);
                }
            } else {
                Handle h = LAYOUT(i)->Get(3);
                if (fn_8004A220(h)) {
                    done++;
                }
            }
        } else {
            int hover = mCursors[i]->GetHovered();
            if (mPlayerStates[i].state != 2) {
                if (mPlayerStates[i].state == 1) {
                    if (fn_8006DBC0(fn_80070680(fn_18_1BBC0(mCursorPlayers[i])), 0x800)) {
                        LAYOUT(i)->fn_18_2AE90(3);
                        mPlayerStates[i].state = 2;
                        fn_18_21F30(mCursors[i], 0.2f);
                        PLAY_SELECT_SOUND(channel);
                    }
                }
                if (mPlayerStates[i].state != 2) {
                    if (hover >= 0) {
                        if (mPlayerStates[i].state != 1) {
                            LAYOUT(i)->fn_18_2AE90(2);
                            Handle t1 = LAYOUT(i)->Get(2);
                            fn_8004A4B0(t1, 0.0f);
                            Handle t2 = LAYOUT(i)->Get(2);
                            fn_8004A410(t2, 1.0f);
                            mPlayerStates[i].state = 1;
                            fn_8007CD10("SEQ_SEMG0000_21", 0.0f, 1.0f, 0.0f);
                        }
                    } else if (mPlayerStates[i].state != 0) {
                        LAYOUT(i)->fn_18_2AE90(2);
                        Handle a = LAYOUT(i)->Get(2);
                        Handle b = LAYOUT(i)->Get(2);
                        fn_8004A4B0(a, fn_8004A550(b));
                        Handle t3 = LAYOUT(i)->Get(2);
                        fn_8004A410(t3, -1.0f);
                        mPlayerStates[i].state = 0;
                    }
                }
            } else {
                Handle h = LAYOUT(i)->Get(3);
                if (fn_8004A220(h)) {
                    done++;
                }
            }
        }
    }
    if (done >= mCursorCount) {
        fn_800715E0(0.5f);
        mLists[0]->fn_18_2AE90(1);
        for (u32 j = 0; j < mCursorCount; j++) {
            LAYOUT(j)->fn_18_2AE90(4);
        }
        mState = 4;
    }
}

/* ---------------------------------------------------------------------- */
/* Text box setup                                                         */

struct Vec2f {
    f32 x, y;
};

struct Vec4f {
    f32 x, y, z, w;
};

class DrawParams {
public:
    DrawParams();   /* fn_801796D0 */
    ~DrawParams();  /* fn_80179750 */

    u8 pad0[0x34];
    Vec4f rect;
    Vec2f scale;
    u8 pad4C[4];
    u8 flagA : 1;
    u8 flagB : 1;
    u8 flag : 1;
};

class ResourceVTable {
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
    virtual void Apply(const DrawParams* params);
};

struct LayoutRes;
ResourceVTable* fn_800486C0(void* layout);
Vec4f fn_80171A70(ResourceVTable* res);

void fn_18_20DE0(void* self, void* layout) {
    DrawParams params;
    params.rect = fn_80171A70(fn_800486C0(layout));
    params.flag = lbl_8026E3B8;
    Vec2f s;
    s.x = lbl_8026DAB4;
    s.y = 1.0f;
    params.scale = s;
    fn_800486C0(layout)->Apply(&params);
}

/* ---------------------------------------------------------------------- */
/* Cursor manager                                                         */

CursorManager::CursorManager() : mRefCount(0) {}

PtrVector::PtrVector() : mData(0), mSize(0), mCapacity(0) {}

CursorManager::~CursorManager() {}

HandCursor* CursorManager::Back(int i) {
    return mLists[i].size() != 0 ? mLists[i].back() : 0;
}
