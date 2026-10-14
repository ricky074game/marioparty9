/* Shared by most REL modules. Reference copy: mg9101 (fn_18_1DA00). */
#include "types.h"

typedef unsigned long size_t;

struct FILE {
    u8 data[0x50];
};

extern "C" FILE __files[];
extern "C" int fprintf(FILE* stream, const char* format, ...);
extern "C" void abort(void);
extern "C" size_t strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, size_t n);
extern "C" void* memset(void* dst, int c, size_t n);
extern "C" void* memmove(void* dst, const void* src, size_t n);
void* operator new(size_t);
void operator delete(void*);

inline void MslError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

/* Keeps "vector length error" first in the string pool (excluded from pairing). */
__declspec(weak) void PoolAnchor() {
    fprintf(&__files[2], "vector length error");
}

inline void LengthError() {
    fprintf(&__files[2], "vector length error");
    abort();
}

template <class T>
inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

struct Allocator {
    Allocator() {}
};

/* ---------------------------------------------------------------------- */
/* String (MSL basic_string<char> with short-string optimisation)         */

class String {
public:
    struct Long {
        unsigned long is_long : 1;
        unsigned long cap : 31;
        size_t size;
        char* data;
    };
    struct Short {
        unsigned char is_long : 1;
        unsigned char size : 7;
        char data[11];
    };

    ~String();
    const char* c_str() const;

    const Long& L() const { return *(const Long*)this; }
    const Short& S() const { return *(const Short*)this; }
    size_t size() const {
        bool l = L().is_long;
        return !l ? S().size : L().size;
    }

    void assign(const char* s);
    String& Replace(size_t pos, size_t n, const char* first, const char* last, Allocator alloc);

    size_t mWords[3];
};

String fn_8008D0A0();
int fn_18_1B0C0(const char*);

/* ---------------------------------------------------------------------- */
/* External objects                                                       */

class ProcBase {
public:
    ProcBase();
    virtual ~ProcBase();
    virtual void Run() = 0;

    void SetPrio(int); /* fn_800714E0 */

    int mUnk4;
};

class HandCursor {
public:
    ~HandCursor(); /* __dt__10HandCursorFv */
    void Destroy() { delete this; }
    u8 pad[0x64];
};

struct Vec2f {
    f32 x, y;
};

class CursorBase {
public:
    CursorBase(int port);    /* fn_800385A0 */
    void GetPos(Vec2f* out); /* fn_80038640 */
    u32 m0;
};

void fn_80038F00(HandCursor* cursor);

class TextBox {
public:
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
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
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
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void Hide(); /* 0xFC */
};

class TextBoxPtr {
public:
    ~TextBoxPtr(); /* fn_18_18A30 */
    TextBox* mPtr;
};

class CursorList2 {
public:
    ~CursorList2(); /* fn_18_1D860 */
    u32 m0, m4, m8;
};

struct ListObjBase {
    u8 pad[0x10];
};

class ListObj : public ListObjBase {
public:
    virtual ~ListObj();
};

class CursorMgr;
extern CursorMgr* lbl_18_bss_4730;
CursorMgr* fn_18_20EA0(CursorMgr* mgr);
void fn_18_21420(CursorMgr* mgr, int port, void* vec);

int fn_18_1BBC0(int player);
BOOL fn_18_1BC20(int player);
int fn_18_1BC80(int player);
void fn_80086F10(void* p, int flag);
void fn_800807B0();

struct ResPtr {
    void* mPtr;
    ~ResPtr() { fn_80086F10(mPtr, 1); }
};

/* ---------------------------------------------------------------------- */
/* Vectors                                                                */

template <class T>
struct Vector {
    static size_t max_size() { return (size_t)-1 / sizeof(T); }
    size_t RecommendSizeLocal(const size_t n) {
        size_t cap = mCapacity;
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
            MslError("Memory allocation failure");
        }
        return p;
    }

    u32 size() const { return mSize; }
    u32 capacity() const { return mCapacity; }
    T* begin() { return mData; }
    T* end() { return mData + mSize; }

    T* mData;
    u32 mSize;
    u32 mCapacity;
};

/* New storage is filled back to front and then swapped into the owner. */
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
inline void CopyBackward(T* first, T* last, T* dst, SplitBuf<T>& buf) {
    size_t n = ((char*)last - (char*)first) / (int)sizeof(T);
    buf.mRemaining -= n;
    memcpy(buf.mFirst + buf.mRemaining, first, n * sizeof(T));
    memset(first, 0, n * sizeof(T));
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
    self->RecommendSizeLocal(1);
    SplitBuf<T> buf(&self->mCapacity);
    size_t n = self->mSize + 1 - self->mCapacity;
    size_t newcap = self->RecommendSizeLocal(n);
    buf.mFirst = Vector<T>::Allocate(newcap);
    buf.mCap = newcap;
    buf.mRemaining = self->mSize;
    T* end = buf.mFirst + self->mSize;
    end[buf.mCount] = x;
    ++buf.mCount;
    CopyBackward(self->begin(), self->end(), buf.mFirst + buf.mRemaining, buf);
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

/* Vector of player indices (push_back, operator[] and size are out of line). */
class PlayerVecBase : public Vector<u32> {
public:
    ~PlayerVecBase() {
        if (mData != 0) {
            mSize -= (int)mSize;
            operator delete(mData);
        }
    }
    u32 size() const;
    u32& operator[](u32 i);
    void push_back(const u32& x);
};

struct PlayerVec1 : PlayerVecBase {
    ~PlayerVec1() {}
};
struct PlayerVec2 : PlayerVec1 {
    ~PlayerVec2() {}
};
struct PlayerVec3 : PlayerVec2 {
    ~PlayerVec3() {}
};

class IntVecBase : public Vector<int> {
public:
    ~IntVecBase() {
        if (mData != 0) {
            mSize -= (int)mSize;
            operator delete(mData);
        }
    }
};

struct IntVec1 : IntVecBase {
    ~IntVec1() {}
};
struct IntVec2 : IntVec1 {
    ~IntVec2() {}
};
struct IntVec3 : IntVec2 {
    ~IntVec3() {}
};

/* Plain parameter vector (only size and operator[] are out of line). */
class ParamVec {
public:
    u32 size() const;
    int& operator[](u32 i);

    int* mData;
    u32 mSize;
    u32 mCapacity;
};

struct BitRef {
    BitRef(u32* word, u32 mask) : mWord(word), mMask(mask) {}
    BitRef& operator=(bool v) {
        if (v) {
            *mWord |= mMask;
        } else {
            *mWord &= ~mMask;
        }
        return *this;
    }
    u32* mWord;
    u32 mMask;
};

class BitVecBase {
public:
    ~BitVecBase() {
        if (mData != 0) {
            operator delete(mData);
        }
    }
    void push_back(bool value);
    static u32 RecommendBits(const u32 cap, const u32 n) {
        if (n > (u32)-1 - cap) {
            MslError("bitvector length error");
        }
        return cap + Max(cap, n);
    }
    BitRef Ref(u32 idx) { return BitRef(mData + (idx >> 5), 1 << (idx & 31)); }

    u32* mData;
    u32 mSize;
    u32 mCapWords;
};

struct BitVec1 : BitVecBase {
    ~BitVec1() {}
};
struct BitVec2 : BitVec1 {
    ~BitVec2() {}
};

class CursorVecBase {
public:
    ~CursorVecBase() {
        if (mData != 0) {
            mSize -= (int)mSize;
            operator delete(mData);
        }
    }
    u32 size() const { return mSize; }

    HandCursor** mData;
    u32 mSize;
    u32 mCapacity;
};

struct CursorVec1 : CursorVecBase {
    ~CursorVec1() {}
};
struct CursorVec2 : CursorVec1 {
    ~CursorVec2() {}
};
struct CursorVec3 : CursorVec2 {
    ~CursorVec3() {}
};

class ListVecBase {
public:
    ~ListVecBase() {
        if (mData != 0) {
            mSize -= (int)mSize;
            operator delete(mData);
        }
    }
    u32 size() const { return mSize; }

    ListObj** mData;
    u32 mSize;
    u32 mCapacity;
};
struct ListVec1 : ListVecBase {
    ~ListVec1() {}
};
struct ListVec2 : ListVec1 {
    ~ListVec2() {}
};
struct ListVec3 : ListVec2 {
    ~ListVec3() {}
};

void fn_18_1D4A0(IntVec3* vec, const int& x);

/* ---------------------------------------------------------------------- */
/* Hand cursor menu process                                               */

class CursorMenu : public ProcBase {
public:
    virtual ~CursorMenu();
    virtual void Run();

    void Init(ParamVec& players); /* fn_18_1DC10 */
    void Cleanup();               /* fn_18_1E5A0 */

    int mState;
    int mType;
    int mMode;
    PlayerVec3 mPlayers;
    ListVec3 mLists;
    IntVec3 mCursorPlayers;
    IntVec3 mSlots;
    TextBoxPtr mText[2];
    IntVec2 mNames;
    u32 mCursorMask;
    u32 mCount;
    String mLabels[2];
    CursorVec3 mCursors;
    BitVec2 mBits;
    CursorList2 mSaved[4];
    Vec2f mPos[4];
    int mDefault;
    u8 mActive;
    ResPtr mRes;
};

CursorMenu::~CursorMenu() {
    Cleanup();
}

void CursorMenu::Init(ParamVec& players) {
    mActive = 0;
    SetPrio(2);
    mDefault = -1;
    mState = 0;
    mCursorMask = 0;
    mCount = 0;

    u32 n = 4;
    switch (mType) {
    case 5:
        n = 3;
        break;
    case 6:
    case 7:
        n = 2;
        break;
    case 11:
        n = 1;
        break;
    }
    if (players.size() < n) {
        n = players.size();
    }

    switch (mType) {
    case 8:
    case 10:
    case 12:
        for (u32 i = 0; i < n; i++) {
            u32 p = players[i];
            if (fn_18_1BC80(p) == 0) {
                mPlayers.push_back(p);
            }
        }
        for (u32 i = 0; i < n; i++) {
            u32 p = players[i];
            if (fn_18_1BC80(p) != 0) {
                mPlayers.push_back(p);
            }
        }
        break;
    case 9:
        for (u32 i = 0; i < n; i++) {
            u32 p = players[i];
            if (fn_18_1BC80(p) != 0) {
                mPlayers.push_back(p);
            }
        }
        for (u32 i = 0; i < n; i++) {
            u32 p = players[i];
            if (fn_18_1BC80(p) == 0) {
                mPlayers.push_back(p);
            }
        }
        break;
    default: {
        for (u32 i = 0; i < n; i++) {
            u32 p = players[i];
            mPlayers.push_back(p);
        }
        int id = fn_18_1B0C0(fn_8008D0A0().c_str());
        if (id == 0x41) {
            int counts[2] = {0, 0};
            for (u32 i = 0; i < n; i++) {
                u32 p = players[i];
                u32 team = fn_18_1BC80(p);
                if (team <= 1) {
                    mPlayers[team + counts[team] * 2] = p;
                    counts[team]++;
                }
            }
        }
        break;
    }
    }

    for (u32 i = 0; i < mPlayers.size(); i++) {
        u32 p = mPlayers[i];
        if (++mCount > n) {
            break;
        }
        if (fn_18_1BC20(p) == 0) {
            int slot = mCount - 1;
            fn_18_1D4A0(&mSlots, slot);
            fn_18_1D4A0(&mCursorPlayers, p);
            if (mType == 10) {
                if (fn_18_1BC80(p) == 0) {
                    mBits.push_back(true);
                } else {
                    mBits.push_back(mMode == 0);
                }
            } else {
                mBits.push_back(mMode == 0);
            }
        }
    }

    if (mType == 10) {
        mLabels[0].assign("mg_inst_014");
        mLabels[1].assign("mg_inst_015");
    } else {
        switch (mMode) {
        case 0:
            mLabels[0].assign("mg_inst_008");
            break;
        case 1:
            if (mType == 1) {
                mLabels[0].assign("mg_inst_016");
            } else {
                mLabels[0].assign("mg_inst_010");
            }
            break;
        case 2:
            if (mType == 1) {
                mLabels[0].assign("mg_inst_017");
            } else {
                mLabels[0].assign("mg_inst_009");
            }
            break;
        }
    }
}

u32 ParamVec::size() const {
    return mSize;
}

int& ParamVec::operator[](u32 i) {
    return mData[i];
}

void PlayerVecBase::push_back(const u32& x) {
    PushBack(*this, x);
}

u32& PlayerVecBase::operator[](u32 i) {
    return mData[i];
}

void BitVecBase::push_back(bool value) {
    u32 cap = mCapWords << 5;
    if (mSize == cap) {
        u32 words = ((RecommendBits(cap, 1) - 1) >> 5) + 1;
        u32* p = (u32*)operator new(words * sizeof(u32));
        if (p == 0) {
            MslError("Memory allocation failure");
        }
        if (mData != 0) {
            if (mSize != 0) {
                u32* last = mData + ((mSize - 1) >> 5) + 1;
                memmove(p, mData, (last - mData) * sizeof(u32));
            }
            operator delete(mData);
        }
        mData = p;
        mCapWords = words;
    }
    ++mSize;
    BitRef ref = Ref(mSize - 1);
    ref = value;
}

u32 PlayerVecBase::size() const {
    return mSize;
}

void String::assign(const char* s) {
    size_t n = size();
    Allocator unused;
    Replace(0, n, s, s + strlen(s), Allocator());
}

void CursorMenu::Cleanup() {
    for (int i = 0; i < 2; i++) {
        if (mText[i].mPtr != 0) {
            mText[i].mPtr->Hide();
        }
    }

    if (mCursors.mSize != 0) {
        for (u32 i = 0; i < mCursors.mSize; i++) {
            if (mCursorMask & (1 << i)) {
                fn_80038F00(mCursors.mData[i]);
            }
        }
        mCursorMask = 0;
    }

    for (u32 i = 0; i < mPlayers.mSize; i++) {
        if (mCursors.mSize > i) {
            int p = mPlayers.mData[i];
            if (fn_18_1BC20(p)) {
                CursorMgr* mgr = lbl_18_bss_4730;
                if (mgr == 0) {
                    mgr = (CursorMgr*)operator new(0x68);
                    if (mgr) {
                        mgr = fn_18_20EA0(mgr);
                    }
                }
                fn_18_21420(mgr, fn_18_1BBC0(p), &mSaved[i]);
            }
        }
    }

    HandCursor** it = mCursors.mData;
    u32 count = mCursors.mSize;
    HandCursor** end = it + mCursors.mSize;
    for (; it != end; ++it) {
        (*it)->Destroy();
    }
    mCursors.mSize -= (int)mCursors.mSize;

    ListObj** lit = mLists.mData;
    ListObj** lend = lit + mLists.mSize;
    for (; lit != lend; ++lit) {
        delete *lit;
    }
    mLists.mSize -= (int)mLists.mSize;

    for (u32 i = 0; i < mPlayers.mSize; i++) {
        if (count > i) {
            int p = mPlayers.mData[i];
            if (fn_18_1BC20(p)) {
                CursorBase base(fn_18_1BBC0(p));
                base.GetPos(&mPos[i]);
            }
        }
    }

    if (mActive) {
        fn_800807B0();
        mActive = 0;
    }
}
