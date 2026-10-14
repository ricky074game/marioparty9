/* Shared by most REL modules. Reference copy: mg9101 (fn_18_1E7F0). */
#include "types.h"

typedef unsigned long size_t;

struct FILE {
    u8 data[0x50];
};

extern "C" FILE __files[];
extern "C" int fprintf(FILE* stream, const char* format, ...);
extern "C" int sprintf(char* buf, const char* format, ...);
extern "C" void abort(void);
extern "C" void* memcpy(void* dst, const void* src, size_t n);
extern "C" void* memset(void* dst, int c, size_t n);
void* operator new(size_t);
void operator delete(void*);
inline void* operator new(size_t, void* p) {
    return p;
}

inline void MslError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

inline void LengthError() {
    fprintf(&__files[2], "vector length error");
    abort();
}

template <class T>
inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

/* ---------------------------------------------------------------------- */
/* Small value types                                                       */

struct Vec2 {
    Vec2(const Vec2& o);
    u32 a, b;
};

struct Vec3 {
    Vec3(f32 x, f32 y, f32 z);
    Vec3(const Vec3& o);
    f32 x, y, z;
};

struct Mtx34 {
    Mtx34(const Mtx34& o);
    f32 m[3][4];
};

/* position pushed to the V3 vector (z stays 0) */
struct V3 {
    f32 x, y;
    u32 z;
};

/* ---------------------------------------------------------------------- */
/* Metrowerks vector                                                       */

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
    size_t RecommendSize(const size_t n0) {
        size_t cap = mCapacity;
        const size_t m = max_size();
        size_t n = n0;
        size_t b;
        size_t a;
        if (n > m - cap) {
            LengthError();
        }
        if (cap < m / 3) {
            a = 3 * (cap + 1) / 5;
            return cap + Max(a, n);
        }
        if (cap < m / 3 * 2) {
            b = (cap + 1) / 2;
            return cap + Max(b, n);
        }
        return m;
    }
    static T* Allocate(size_t n) {
        if (n > max_size()) {
            LengthError();
        }
        T* p = (T*)operator new(n * sizeof(T));
        if (p == 0) {
            MslError("Memory allocation failure");
        }
        return p;
    }

    T* mData;
    u32 mSize;
    u32 mCapacity;
};

template <class T>
inline void CopyBackward(T** first, T** last, T** dst, SplitBuf<T*>& buf) {
    size_t n = last - first;
    buf.mRemaining -= n;
    memcpy(buf.mFirst + buf.mRemaining, first, n * sizeof(T*));
    memset(first, 0, n * sizeof(T*));
    buf.mCount += n;
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
    self->RecommendSize(1);
    SplitBuf<T> buf(&self->mCapacity);
    size_t n = self->mSize + 1 - self->mCapacity;
    size_t newcap = self->RecommendSize(n);
    buf.mFirst = Vector<T>::Allocate(newcap);
    buf.mCap = newcap;
    buf.mRemaining = self->mSize;
    T* end = buf.mFirst + self->mSize;
    end[buf.mCount] = x;
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
/* Types from other units / main.dol                                       */

struct SmallString {
    u32 mWord0;
    u32 mSize;
    char* mData;
    bool Empty() const;
};

class String : public SmallString {
public:
    String(const char* s); /* __ct__6StringFPCc */
    ~String();
    const char* c_str() const;
};

String operator+(const String& lhs, const char* rhs); /* fn_18_16340 */

struct Key {
    int v;
};
Key* MakeKey(Key* out, const String& s); /* fn_800882E0 */

struct XObj {
    int mVal;
    XObj();
    XObj(const Key& k, int flag); /* fn_80088400 */
    ~XObj();                      /* fn_80086F10 */
    void Copy(const XObj& o);     /* fn_80086B90 */
};

struct Handle {
    virtual ~Handle();
    void* mPtr;
};

struct Pane {
    u8 pad0[0x44];
    Vec2 mScale;
    u8 pad4C[0x84 - 0x4C];
    Mtx34 mMtx;
    const Mtx34* GetMtx() const;
    const Vec2* GetScale() const;
};

#define VF(n) virtual void v##n();

struct PaneMgr {
    virtual ~PaneMgr();
    VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10) VF(11) VF(12)
    virtual Pane* FindPane(const char* name, int flag); /* 0x3c */
};

struct TextBox {
    TextBox(int); /* fn_8008EA90 */
    virtual ~TextBox();
    VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10)
    VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19) VF(20)
    VF(21) VF(22) VF(23) VF(24) VF(25) VF(26)
    virtual void SetText(const char*); /* 0x74 */
    VF(28) VF(29) VF(30) VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF(40)
    VF(41) VF(42) VF(43) VF(44) VF(45) VF(46) VF(47) VF(48) VF(49) VF(50)
    virtual void v51(int, int); /* 0xd4 */
    virtual void v52();         /* 0xd8 */
    VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59)
    virtual void Create(void* res, Pane* pane); /* 0xf8 */

    u8 mData[0x1BC - 4];
};

struct TextHolder {
    TextBox* mObj;
    TextBox* Get() const;
    void Set(TextBox* obj); /* fn_18_1F620 */
};

void DestroyXObj(XObj* obj, int flag); /* fn_80086F10 */

struct XHolder {
    XObj* mObj;
    XObj* Get() const;
    void Set(XObj* obj);
};

struct HandleList {
    u8 mData[0x14];
    HandleList(); /* fn_18_2AD80 */
    void Create(const char* res, const char* name, int a, int b); /* fn_800475D0 */
    void SetPriority(int);                                         /* fn_80048860 */
    void SetLayer(int);                                            /* fn_800489D0 */
    Handle PlayAnim(const char* name, int);                        /* fn_18_2B2A0 */
    void fn_18_2B1F0(int);
    void fn_80048FE0(const char*, int);
    Pane* FindPane(const char*, int);                          /* fn_80048B50 */
    void SetTrans(Vec3 v);                                     /* fn_80048BD0 */
    void SetScale(Vec2 v);                                     /* fn_80048C10 */
    void SetPaneText(const char* pane, const char* text, int); /* fn_8004ABD0 */
    PaneMgr* GetPaneMgr();                                     /* fn_800486E0 */
    void* GetResource();                                       /* fn_800486C0 */
};

struct HandleListVec : Vector<HandleList*> {
    HandleList** Addr(int i);
    void push_back(HandleList* const& x);
};

struct IntVec : Vector<int> {
    u32 Size() const; /* fn_18_1E040 */
    int* At(u32 i);
};

struct V3Vector : Vector<V3> {
    void push_back(const V3& x);
};

struct BitRef {
    BitRef(u32* word, u32 mask) : mWord(word), mMask(mask) {}
    bool Test() const;
    u32* mWord;
    u32 mMask;
};

struct BitArray {
    u32* mData;
    BitRef Ref(u32 idx);
};

int fn_18_1BD40(int player);
int fn_18_1BBC0(int player);
int fn_18_1BC80(int player);
const char* fn_18_22240(int);

/* ---------------------------------------------------------------------- */
/* Layout tables                                                           */

struct ModeInfo {
    const char* name;
    int value;
};

/* Stands in for the string literals the original unit emitted ahead of its own. */
static char sPrefix[0x80] = "vector length error";

extern const ModeInfo kModeInfo[13];
const ModeInfo kModeInfo[13] = {
    {"base1", -1}, {"base2", -1}, {"base3", 4},  {"base3", 4},  {"base4", -1},
    {"base4", -1}, {"base4", -1}, {"base5", 2},  {"base6", -1}, {"base7", -1},
    {"base8", -1}, {"base9", 1},  {"base10", -1},
};



/* "out2" is kept out of the string pool: the setup code has its own copy. */
static char sOut2[] = "out2";


/* ---------------------------------------------------------------------- */
/* Menu setup                                                              */

class CursorMenu {
public:
    void Setup(int arg);                  /* fn_18_1E800 */
    void SetupLayout(HandleList* layout); /* fn_18_20DE0 */
    void SetState(int s);                 /* fn_18_1F2B0 */

    u8 pad0[8];
    int mState;
    int mMode;
    int mFlag;
    u8 pad14[0x20 - 0x14];
    HandleListVec mLists;
    IntVec mCursorPlayers;
    IntVec mPlayers;
    TextHolder mTexts[2];
    V3Vector mPos;
    u8 pad58[4];
    int mCount;
    String mNames[2];
    u8 pad78[0x84 - 0x78];
    BitArray mBits;
    u8 pad88[0xE8 - 0x88];
    XHolder mHolder;
};

#pragma dont_inline on

TextBox* TextHolder::Get() const {
    return mObj;
}

void CursorMenu::Setup(int arg) {
    static const char* sTextNames[2] = {"x_text", "x_text_b"};
    static const char* sAnimNames[5] = {"in", "normal", "on", "click2", sOut2};
    V3 pos;
    int i;
    if (mCursorPlayers.Size() != 0) {
        mHolder.Set(new XObj());
        {
            String path(fn_18_22240(0));
            String file = path + ".arc";
            Key key;
            XObj obj(*MakeKey(&key, file), 0);
            mHolder.Get()->Copy(obj);
        }
        mLists.push_back(new HandleList());
        (*mLists.Addr(0))->Create("mgsystem/layout/mgstartcheck", kModeInfo[mMode].name, 0, arg);
        (*mLists.Addr(0))->SetPriority(0x905D);
        (*mLists.Addr(0))->SetLayer(2);

        if (mMode >= 8 && mMode <= 10) {
            if (mFlag == 1) {
                (*mLists.Addr(0))->PlayAnim("in2", 0);
                (*mLists.Addr(0))->PlayAnim("out2", 0);
                (*mLists.Addr(0))->PlayAnim("loop2", 0);
            } else {
                (*mLists.Addr(0))->PlayAnim("in1", 0);
                (*mLists.Addr(0))->PlayAnim("out1", 0);
                (*mLists.Addr(0))->PlayAnim("loop1", 0);
            }
        } else {
            if (mFlag == 0) {
                (*mLists.Addr(0))->PlayAnim("in3", 0);
                (*mLists.Addr(0))->PlayAnim("out3", 0);
            } else if (mFlag == 2) {
                (*mLists.Addr(0))->PlayAnim("in1", 0);
                (*mLists.Addr(0))->PlayAnim("out1", 0);
                (*mLists.Addr(0))->PlayAnim("loop1", 0);
            } else {
                (*mLists.Addr(0))->PlayAnim("in2", 0);
                (*mLists.Addr(0))->PlayAnim("out2", 0);
                (*mLists.Addr(0))->PlayAnim("loop2", 0);
            }
        }

        int v = kModeInfo[mMode].value;
        if (v < 0) {
            v = mCount;
            if (mMode >= 8 && mMode <= 10 && v < 3) {
                v = 3;
            }
        }

        SetupLayout(*mLists.Addr(0));
        (*mLists.Addr(0))->fn_18_2B1F0(0);

        for (i = 0; i < 2; i++) {
            if (!mNames[i].Empty()) {
                mTexts[i].Set(new TextBox(0));
                mTexts[i].Get()->Create((*mLists.Addr(0))->GetResource(),
                                        (*mLists.Addr(0))->GetPaneMgr()->FindPane(sTextNames[i], 1));
                mTexts[i].Get()->SetText(mNames[i].c_str());
                mTexts[i].Get()->v52();
                mTexts[i].Get()->v51(-1, 0);
            }
        }

        for (i = 0; i < mCursorPlayers.Size(); i++) {
            pos.z = 0;
            char buf[0x80];
            mLists.push_back(new HandleList());
            (*mLists.Addr(i + 1))->Create("mgsystem/layout/mgstartcheck", "frame", 0, arg);
            (*mLists.Addr(i + 1))->fn_18_2B1F0(0);
            (*mLists.Addr(i + 1))->SetPriority(0x905E);
            (*mLists.Addr(i + 1))->SetLayer(2);
            for (u32 j = 0; j < 5; j++) {
                Handle h = (*mLists.Addr(i + 1))->PlayAnim(sAnimNames[j], 0);
            }
            sprintf(buf, "%d_%d", v, *mPlayers.At(i) + 1);
            Pane* p = (*mLists.Addr(0))->FindPane(buf, 1);
            if (p) {
                Mtx34 m(*p->GetMtx());
                pos.x = m.m[0][3];
                pos.y = m.m[1][3];
                (*mLists.Addr(i + 1))->SetTrans(Vec3(Vec3(pos.x, pos.y, 0.0f)));
                (*mLists.Addr(i + 1))->SetScale(*p->GetScale());
            }
            mPos.push_back(pos);

            int player = *mCursorPlayers.At(i);
            int ch = fn_18_1BD40(player);
            fn_18_1BBC0(player);
            const char* frame;
            const char* face;
            const char* fmt;
            if (mBits.Ref(i).Test()) {
                frame = "x_frame_a";
                face = "x_face_a";
                fmt = "p%d";
                (*mLists.Addr(i + 1))->fn_80048FE0("b", 0);
            } else {
                frame = "x_frame_b";
                face = "x_face_b";
                fmt = "p%db";
                (*mLists.Addr(i + 1))->fn_80048FE0("a", 0);
            }
            sprintf(buf, fmt, *mCursorPlayers.At(i) + 1);
            (*mLists.Addr(i + 1))->SetPaneText(frame, buf, 0);
            sprintf(buf, "face_pc%02d", ch + 1);
            (*mLists.Addr(i + 1))->SetPaneText(face, buf, 0);
        }

        if (mMode == 10) {
            int a = 0;
            int b = 0;
            for (u32 k = 0; k < mCursorPlayers.Size(); k++) {
                int f = fn_18_1BC80(*mCursorPlayers.At(k)) == 0;
                if (f) {
                    a++;
                }
                if (!f) {
                    b++;
                }
            }
            if (a == 0) {
                (*mLists.Addr(0))->fn_80048FE0("Window_00", 0);
            }
            if (b == 0) {
                (*mLists.Addr(0))->fn_80048FE0("Window_01", 0);
            }
        }
    }
    SetState(1);
}

XObj::XObj() {
    mVal = 0;
}

int* IntVec::At(u32 i) {
    return mData + i;
}

Mtx34::Mtx34(const Mtx34& o) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] = o.m[i][j];
        }
    }
}

const Mtx34* Pane::GetMtx() const {
    return &mMtx;
}

Vec3::Vec3(const Vec3& o) {
    x = o.x;
    y = o.y;
    z = o.z;
}

Vec3::Vec3(f32 x_, f32 y_, f32 z_) {
    x = x_;
    y = y_;
    z = z_;
}

Vec2::Vec2(const Vec2& o) {
    a = o.a;
    b = o.b;
}

const Vec2* Pane::GetScale() const {
    return &mScale;
}

void CursorMenu::SetState(int s) {
    mState = s;
}

void XHolder::Set(XObj* obj) {
    XObj* old = mObj;
    if (obj != old) {
        mObj = obj;
        DestroyXObj(old, 1);
    }
}

void EmptyFunc() {}

XObj* XHolder::Get() const {
    return mObj;
}

#pragma dont_inline off

void HandleListVec::push_back(HandleList* const& x) {
    PushBack(*this, x);
}
