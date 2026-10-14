/* Shared by most REL modules. Reference copy: mg9101 (fn_18_1B890). */
#include "types.h"

typedef unsigned long size_t;

struct FILE {
    u8 data[0x50];
};

extern "C" FILE __files[];
extern "C" int fprintf(FILE* stream, const char* format, ...);
extern "C" void abort(void);
extern "C" void* memcpy(void* dst, const void* src, size_t n);
extern "C" void* memset(void* dst, int c, size_t n);

void* operator new(size_t);
void operator delete(void*);

inline void MslError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

inline void LengthError(const char* msg = "vector length error") {
    MslError(msg);
}

inline void AllocError() {
    MslError("Memory allocation failure");
}

template <class T>
inline const T& Max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

/* Scratch buffer used while growing a vector<int>: the new storage is filled
   back to front and then swapped into the owner. */
struct SplitBase {
    SplitBase() : mFirst(0), mCount(0), mCap(0) {}
    ~SplitBase() {
        if (mFirst != 0) {
            mCount = 0;
            operator delete(mFirst);
        }
    }

    int* mFirst;
    u32 mCount;
    u32 mCap;
};

struct SplitBuf : SplitBase {
    SplitBuf(u32* capRef) : mCapRef(capRef), mRemaining(0) {}

    u32* mCapRef;
    u32 mRemaining;
};

class IntVec {
public:
    IntVec() : mData(0), mSize(0), mCap(0) {}
    ~IntVec() {
        if (mData != 0) {
            EraseTail();
            operator delete(mData);
        }
    }

    void EraseTail() { mSize -= (int)mSize; }

    static size_t max_size() { return (size_t)-1 / sizeof(int); }
    size_t RecommendSize(const size_t n0) {
        size_t cap = mCap;
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
    size_t capacity() const { return mCap; }
    size_t RecommendSizeA(const size_t n) {
        size_t cap = capacity();
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
    static int* Allocate(size_t n) {
        if (n > max_size()) {
            LengthError();
        }
        int* p = (int*)operator new(n * sizeof(int));
        if (p == 0) {
            AllocError();
        }
        return p;
    }
    /* Moves the old contents behind the new elements in buf, then swaps. */
    void Commit(SplitBuf& buf) {
        int* last = mData + mSize;
        int* first = mData;
        size_t n = (int*)last - (int*)first;
        buf.mRemaining -= n;
        memcpy(buf.mFirst + buf.mRemaining, first, n * sizeof(int));
        memset(first, 0, n * sizeof(int));
        buf.mCount += n;
        mSize = 0;
        u32 c = mCap;
        mCap = buf.mCap;
        buf.mCap = c;
        int* d = mData;
        mData = buf.mFirst;
        buf.mFirst = d;
        mSize = buf.mCount;
        buf.mCount = 0;
    }
    void push_back(const int& x) {
        size_t sz = mSize;
        if (sz < mCap) {
            mSize++; int* p = mData + (mSize - 1); *p = x;
            return;
        }
        RecommendSize(1);
        SplitBuf buf(&mCap);
        size_t n = mSize + 1 - mCap;
        size_t newcap = RecommendSize(n);
        buf.mFirst = Allocate(newcap);
        buf.mCap = newcap;
        buf.mRemaining = mSize;
        int* end = buf.mFirst + mSize;
        end[buf.mCount] = x;
        ++buf.mCount;
        Commit(buf);
    }
    size_t size() const { return mSize; }
    void CheckGrow(size_t n, size_t cap) {
        size_t sz = size();
        RecommendSizeA(sz + n - cap);
    }
    void resize(size_t newSize) {
        size_t sz = mSize;
        if (sz < newSize) {
            size_t n = newSize - sz;
            size_t cap = mCap;
            if (n > cap || sz > cap - n) {
                CheckGrow(n, cap);
            } else {
                memset(mData + sz, 0, n * sizeof(int));
                mSize += n;
                return;
            }
            SplitBuf buf(&mCap);
            size_t cur = size();
            size_t newcap = RecommendSizeA(cur + n - capacity());
            buf.mFirst = Allocate(newcap);
            buf.mCap = newcap;
            buf.mRemaining = mSize;
            memset(buf.mFirst + buf.mRemaining + buf.mCount, 0, n * sizeof(int));
            buf.mCount += n;
            Commit(buf);
        } else if (sz > newSize) {
            mSize -= sz - newSize;
        }
    }

    int* mData;
    u32 mSize;
    u32 mCap;
};

struct StrAlloc {
    StrAlloc() {}
};

/* Heap string (12 bytes), empty-initialised. */
struct StrBuf {
    StrBuf() : w0(0), w1(0), w2(0) {}
    void Replace(size_t pos, size_t n, const char* first, const char* last, StrAlloc alloc); /* fn_18_16040 */
    u32 w0, w1, w2;
};

/* main.dol helpers */
class Timer {
public:
    Timer(); /* fn_80041410 */
    void Start(int); /* fn_800419D0 */
    void Update();   /* fn_800417D0 */
    u32 m0;
};
void* fn_80070680(int pad);
u32 fn_8006E270(void* pad);

int fn_18_1B330(int i);
int fn_18_1B350(int i);

struct PlayerSlot {
    int port;       /* 0x00 */
    u8 connected;   /* 0x04 */
    int unk8;       /* 0x08, -1 when unused */
    int unkC;       /* 0x0C */
    int unk10;      /* 0x10 */
    int unk14;      /* 0x14 */
    int unk18;      /* 0x18 */
    int unk1C;      /* 0x1C */
    int unk20;      /* 0x20 */
    int unk24;      /* 0x24 */
};

class GameState;

extern GameState* lbl_8026E2A8; /* singleton instance (lbl_8026E2A4 + 4) */

class GameState {
public:
    GameState();
    static GameState* Get() {
        if (lbl_8026E2A8 == 0) {
            new GameState;
        }
        return lbl_8026E2A8;
    }

    s16 mId;                 /* 0x0000 */
    u8 pad2[0x54 - 2];
    int mTableA[0x50];       /* 0x0054 */
    int mTableB[0x50];       /* 0x0194 */
    u8 pad2D4[0x6A4 - 0x2D4];
    u8 mFlags6A4;            /* 0x06A4 */
    u8 mFlags6A5;
    u8 mFlags6A6;
    u8 pad6A7[0x6C8 - 0x6A7];
    int mUnk6C8;
    u8 mUnk6CC;
    u8 pad6CD[0x9A0 - 0x6CD];
    int mUnk9A0;             /* 0x09A0 */
    int mUnk9A4;
    u8 mUnk9A8;
    u8 pad9A9[3];
    int mUnk9AC;
    int mUnk9B0;
    int mNumPlayers;         /* 0x09B4 */
    int mUnk9B8;
    u8 mUnk9BC;
    u8 mUnk9BD;
    u8 mUnk9BE[1];
    u8 mUnk9BF[2];
    u8 pad9C1[3];
    PlayerSlot mSlots[4];    /* 0x09C4 */
    int mUnkA64[4];
    int mUnkA74;
    int mUnkA78;
    u8 mBufA7C[0x6000];
    u8 mBuf6A7C[0x100];
    IntVec mVecs[4];         /* 0x6B7C */
    StrBuf mName;            /* 0x6BAC */
    IntVec mList;            /* 0x6BB8 */
    Timer mTimer;            /* 0x6BC4 */
    u8 mUnk6BC8[3];
    int mUnk6BCC;
    u8 mUnk6BD0;
    int mUnk6BD4;
    int mUnk6BD8;
    int mUnk6BDC;
    u32 mUnk6BE0;
    u32 mUnk6BE4;
    u32 mUnk6BE8;
    u8 mUnk6BEC[4];
    u8 mUnk6BF0[4];
    int mUnk6BF4;
    int mUnk6BF8[4];
    int mUnk6C08;
    u8 mUnk6C0C;
};

inline int SlotUnk8(int slot) {
    return GameState::Get()->mSlots[slot].unk8;
}

/* Collects the indices of all slots in use. */
IntVec GetActiveSlots() {
    IntVec result;
    for (int i = result.mSize; i < GameState::Get()->mNumPlayers; i++) {
        if (SlotUnk8(i) != -1) {
            result.push_back(i);
        }
    }
    return result;
}

int fn_18_1BBC0(int slot) {
    return GameState::Get()->mSlots[slot].port;
}

BOOL fn_18_1BC20(int slot) {
    return GameState::Get()->mSlots[slot].connected;
}

int GetSlotUnk8(int slot) {
    return SlotUnk8(slot);
}

int GetSlotUnkC(int slot) {
    return GameState::Get()->mSlots[slot].unkC;
}

int fn_18_1BD40(int slot) {
    return GameState::Get()->mSlots[slot].unk10;
}

extern int lbl_18_rodata_EC0[][2];

int LookupTable(int a, int b) {
    return lbl_18_rodata_EC0[a][b];
}

void SetSlotUnk14(int slot, int value) {
    GameState::Get()->mSlots[slot].unk14 = value;
}

GameState* GetGameState() {
    return GameState::Get();
}

GameState::GameState() : mUnk6BE0(0), mUnk6BE4(0), mUnk6BE8(0) {
    lbl_8026E2A8 = this;
    memset(this, 0, 0x9A0);
    mId = 0x7C10;
    mUnk9A0 = -1;
    mUnk9A4 = 0;
    for (u32 i = 0; i < 0x50; i++) {
        mTableA[i] = fn_18_1B330(i);
        mTableB[i] = fn_18_1B350(i);
    }
    mUnk9AC = 0;
    mUnk9B0 = 2;
    mNumPlayers = 4;
    mUnk9B8 = 0;
    mUnk9BC = 3;
    mUnk9BD = 0;
    memset(mUnk9BE, 0, 1);
    memset(mUnk9BF, 0, 2);
    Get()->mUnk6C8 = 1;
    Get()->mUnk6CC = 1;
    for (int i = 0; i < 4; i++) {
        int port = i % 4;
        PlayerSlot* slot = &mSlots[i];
        slot->port = port;
        slot->connected = (i == 0) ? 0 : (fn_8006E270(fn_80070680(port)) != 0);
        slot->unk8 = port;
        slot->unkC = 0;
        slot->unk10 = port;
        if (i == 1) {
            slot->unk10 = 4;
        }
        if (i == 3) {
            slot->unk10 = 6;
        }
        slot->unk14 = -1;
        slot->unk18 = 0;
        slot->unk20 = 0;
        slot->unk24 = 0;
        mUnk6BEC[i] = 1;
        mUnk6BF8[i] = 0;
        mUnk6BF0[i] = 0;
        mVecs[i].EraseTail();
    }
    mUnk6C0C = 0;
    mList.resize(2);
    mList.mData[0] = 0;
    mList.mData[1] = 1;
    mUnk6BCC = 0;
    mUnk6BD0 = 0;
    mUnkA64[0] = 0;
    mUnkA64[1] = 0;
    mUnkA64[2] = 0;
    mUnkA64[3] = 0;
    mUnk6BF4 = 0;
    mUnkA74 = 0;
    mUnkA78 = 0;
    mUnk9A8 = 4;
    mName.Replace(0, -1, 0, 0, StrAlloc());
    memset(mBufA7C, 0, 0x6000);
    memset(mBuf6A7C, 0, 0x100);
    mUnk6BD4 = 1;
    mUnk6BC8[0] = 0;
    mUnk6BC8[1] = 0;
    mUnk6BC8[2] = 0;
    Get()->mFlags6A4 |= 0x01;
    Get()->mFlags6A4 |= 0x08;
    Get()->mFlags6A4 |= 0x40;
    Get()->mFlags6A5 |= 0x02;
    Get()->mFlags6A5 |= 0x10;
    Get()->mFlags6A5 |= 0x80;
    Get()->mFlags6A6 |= 0x04;
    mUnk6BD8 = 0;
    mUnk6BDC = 0;
    mUnk6C08 = -1;
    mTimer.Start(1);
    mTimer.Update();
}
