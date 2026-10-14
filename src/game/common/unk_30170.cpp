/* Shared by most REL modules. Reference copy: mg9101 (fn_18_30170). */
#include "game/common/unk_30170.h"

BOOL fn_18_1B6D0();
int fn_18_1B480();
BOOL fn_18_1B680();
BOOL fn_18_26480();
void fn_18_26490(int);
void fn_18_26280(int);
void fn_8008CB80(const char*);
void fn_80074D70(int);

/* Player list returned by fn_18_1B890 (vector<int> layout) */
struct IntListImpl {
    int* mData;
    u32 mSize;
    u32 mCap;
    ~IntListImpl() {
        if (mData) {
            mSize = mSize - (s32)mSize;
            operator delete(mData);
        }
    }
};
struct IntListMid : IntListImpl {
    ~IntListMid() {}
};
struct IntListTop : IntListMid {
    ~IntListTop() {}
};
struct IntList : IntListTop {
    ~IntList() {}
    u32 size() const { return mSize; }
    int& operator[](u32 i) { return mData[i]; }
};

IntList fn_18_1B890();
BOOL fn_18_1BC20(int);
int fn_18_1BBC0(int);

void MgScene::fn_30170() {
    if (!fn_18_1B6D0()) {
        if (fn_18_1B480() != 7) {
            if (fn_18_1B680()) {
                if (fn_18_26480()) {
                    fn_8008CB80("inst");
                    fn_18_26490(0);
                }
            } else {
                fn_8008CB80("mgresult");
            }
        }
        fn_18_26280(0);
        m10 = 1;
    } else {
        Fader* fader = m280.get();
        if (fader->mC == 0) {
            fader->Request(1);
        }
    }
}

void MgScene::Unk1C() {
    if (!fn_18_1B6D0()) {
        if (m260.mPtr == 0) {
            m260.reset(new Handle260);
        }
        if (m260) {
            fn_18_1D040(m260.get());
        }
        if (fn_18_1B680()) {
            fn_80074B60(1, 0.3f);
        } else {
            fn_80074D70(0);
        }
    }
}

/* ScopedPtr<Handle260>::get */
Handle260* fn_18_303A0(Owned<Handle260>* p) {
    return p->mPtr;
}

void MgScene::Unk20() {}

void MgScene::fn_303C0() {
    fn_18_19A10(m27C.get(), 0);
    IntList players = fn_18_1B890();
    for (u32 i = 0; i < players.size(); i++) {
        int player = players[i];
        if (!fn_18_1BC20(player)) {
            int id = fn_18_1BBC0(player);
            fn_18_19960(m27C.get(), id, 1);
        }
    }
}

void MgScene::vf2C() {
    if (m274) {
        fn_800714E0(2);
        FrameWait* wait = m284.get();
        wait->Start(2);
        fn_80071590();
        fn_18_1FAC0(m274.get());
        while (m274.get()->m8 < 5) {
            fn_80071590();
        }
        fn_800714E0(1);
        wait = m284.get();
        wait->Start(1);
        fn_80071590();
        m274.reset();
    }
    m10 = 1;
}

void MgScene::vf30() {
    m10 = 1;
}

void MgScene::vf38() {
    m10 = 1;
}

void MgScene::vf3C() {}

void fn_18_30610(MgScene* scene) {
    scene->m10 = 1;
}
