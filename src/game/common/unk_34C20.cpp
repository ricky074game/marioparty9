/* Shared by most REL modules. Reference copy: mg9101 (fn_18_34C20). */
#include "types.h"

void* operator new(unsigned long size);
void operator delete(void* p);

/* ---------------------------------------------------------------------- */
/* Pause-layout style object                                               */

/* main.dol animation player */
class Player {
public:
    s32 m0;
    Player();           /* fn_8002ADD0 */
    ~Player();          /* fn_8002AE40 */
    void SetMode(int);  /* fn_8002AEC0 */
    void SetSpeed(f32); /* fn_8002AED0 */
    void SetLoop(int);  /* fn_8002AF60 */
    u32 GetState();     /* fn_8002AF40 */
    void Advance();     /* fn_8002AF70 */
    void SetState(int); /* fn_8002AF00 */
};

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
    T* get() const { return mPtr; }

    void reset(T* p = 0) {
        if (p != mPtr) {
            T* old = mPtr;
            mPtr = p;
            delete old;
        }
    }

    T* mPtr;
};

/* main.dol layout handle (ctor fn_18_1C700, dtor fn_18_1C770) */
class LayoutHandle {
public:
    LayoutHandle(const char* archive, const char* name, int arg, int id);
    ~LayoutHandle();

    u32 m0;
};

int fn_8004E010(int);
int fn_80048FC0();
int fn_8004EF20();
void fn_8004EF50();
void fn_8004EF10(int);

class PauseLayout : public LayoutHandle {
public:
    PauseLayout();
    virtual ~PauseLayout();
    virtual void Update();

    ScopedPtr<Player> mPlayer;
    int mC;
};

/* ---------------------------------------------------------------------- */

class FlagsObj {
public:
    void SetFlag24();
    u8 GetFlag25();
    u8 GetFlag26();

    u8 pad[0x24];
    u8 m24;
    u8 m25;
    u8 m26;
};

void FlagsObj::SetFlag24() {
    m24 = 1;
}

u8 FlagsObj::GetFlag25() {
    return m25;
}

u8 FlagsObj::GetFlag26() {
    return m26;
}

/* Adjusting thunks for the sub-object at +4 */
void fn_18_32C00(void*);
void fn_18_32A10(void*);
void fn_18_348A0(void*);
void fn_18_34750(void*);

void Thunk32C00(char* self) {
    fn_18_32C00(self - 4);
}

void Thunk32A10(char* self) {
    fn_18_32A10(self - 4);
}

void Thunk348A0(char* self) {
    fn_18_348A0(self - 4);
}

void Thunk34750(char* self) {
    fn_18_34750(self - 4);
}

PauseLayout::PauseLayout() : LayoutHandle("/pause/layout/pause_base", "pause_base", 0, fn_8004E010(3)) {
    mPlayer.reset(new Player);
    mPlayer.get()->SetMode(8);
    mPlayer.get()->SetSpeed(2.0f);
    mPlayer.get()->SetLoop(1);
}

PauseLayout::~PauseLayout() {}

void PauseLayout::Update() {
    if (fn_80048FC0()) {
        if (mPlayer.get()->GetState() == 0) {
            mC = fn_8004EF20();
            fn_8004EF50();
            for (u32 i = 0; i < 4; i++) {
                mPlayer.get()->Advance();
            }
            mPlayer.get()->SetState(1);
        }
        mPlayer.get()->Advance();
    } else {
        if (mPlayer.get()->GetState() == 1) {
            mPlayer.get()->Advance();
            fn_8004EF10(mC);
            mPlayer.get()->SetState(0);
        }
    }
}

/* ---------------------------------------------------------------------- */
/* Large 3-base object                                                     */

struct Vec3 {
    f32 x, y, z;
};

/* Primary base: 0x68 bytes */
class Base0 {
public:
    virtual ~Base0();
    virtual void vf0C();

    u32 m4;
    u8 pad[0x68 - 8];
};

/* Second base: 0x1D0 bytes, 23 virtuals */
class Base68 {
public:
    virtual ~Base68();
    virtual void b0C();
    virtual void b10();
    virtual void b14();
    virtual void b18();
    virtual void b1C();
    virtual void b20();
    virtual void b24();
    virtual void b28();
    virtual void b2C();
    virtual void b30();
    virtual void b34();
    virtual void b38();
    virtual void b3C();
    virtual void b40();
    virtual void b44();
    virtual void b48();
    virtual void b4C();
    virtual void b50();
    virtual void b54();
    virtual void b58();
    virtual void b5C();
    virtual void b60();

    u8 pad[0x1D0 - 4];
};

/* Third base: 30 virtuals */
class Base238 {
public:
    virtual ~Base238();
    virtual void q0C();
    virtual void q10();
    virtual void q14();
    virtual void q18();
    virtual void q1C();
    virtual void q20();
    virtual void q24();
    virtual void q28();
    virtual void q2C();
    virtual void q30();
    virtual void q34();
    virtual void q38();
    virtual void q3C();
    virtual void q40();
    virtual void q44();
    virtual void q48();
    virtual void q4C();
    virtual void q50();
    virtual void q54();
    virtual void q58();
    virtual void q5C();
    virtual void q60();
    virtual void q64();
    virtual void q68();
    virtual void q6C();
    virtual void q70();
    virtual void q74();
    virtual void q78();
    virtual void q7C();

    u8 pad[0x5F0 - 0x238 - 4];
};

struct Mgr {
    void Release(u32);
};
void fn_18_3BA60(Mgr*, u32);
extern Mgr* lbl_18_bss_4AE0;

struct ListImpl {
    int* mData;
    u32 mSize;
    u32 mCap;
    ~ListImpl() {
        if (mData) {
            mSize = mSize - (s32)mSize;
            operator delete(mData);
        }
    }
};
struct ListMid : ListImpl {
    ~ListMid() {}
};
struct List : ListMid {
    ~List() {}
};

class Member8E4 {
public:
    ~Member8E4();
    u8 pad[0x60];
};

class Member944 {
public:
    ~Member944();
    u8 pad[0x54];
};

void fn_18_3E500(void*);
void* fn_80060100(Base68&);
void* fn_800601B0(Base68&);
void fn_18_3CBC0(Base68&);
void fn_18_3CB20(Base68&, int);

class Actor : public Base0, public Base68, public Base238 {
public:
    virtual ~Actor();
    void Reset();
    virtual void vfF4();  /* 0xF4 */
    virtual void vfF8();  /* 0xF8 */
    virtual void Update();  /* 0xFC */
    virtual void vf100();  /* 0x100 */
    virtual void vf104();  /* 0x104 */
    virtual void vf108();  /* 0x108 */
    virtual void vf10C();  /* 0x10C */
    virtual void vf110();  /* 0x110 */
    virtual int GetState();  /* 0x114 */
    virtual void vf118();  /* 0x118 */
    virtual void vf11C();  /* 0x11C */
    virtual void vf120();  /* 0x120 */
    virtual void vf124();  /* 0x124 */
    virtual void vf128();  /* 0x128 */
    virtual void vf12C();  /* 0x12C */
    virtual void vf130();  /* 0x130 */
    virtual void vf134();  /* 0x134 */
    virtual void vf138();  /* 0x138 */
    virtual void vf13C();  /* 0x13C */
    virtual void vf140();  /* 0x140 */
    virtual void vf144();  /* 0x144 */
    virtual void vf148();  /* 0x148 */
    virtual void vf14C();  /* 0x14C */
    virtual void vf150();  /* 0x150 */
    virtual void vf154();  /* 0x154 */
    virtual void vf158();  /* 0x158 */
    virtual void vf15C();  /* 0x15C */
    virtual void vf160();  /* 0x160 */
    virtual void vf164();  /* 0x164 */
    virtual void vf168();  /* 0x168 */
    virtual void vf16C();  /* 0x16C */
    virtual void vf170();  /* 0x170 */
    virtual void vf174();  /* 0x174 */
    virtual void vf178();  /* 0x178 */
    virtual void vf17C();  /* 0x17C */
    virtual void vf180();  /* 0x180 */
    virtual void vf184();  /* 0x184 */
    virtual void vf188();  /* 0x188 */
    virtual void vf18C();  /* 0x18C */
    virtual void vf190();  /* 0x190 */
    virtual void vf194();  /* 0x194 */
    virtual void vf198();  /* 0x198 */
    virtual void vf19C();  /* 0x19C */
    virtual void vf1A0();  /* 0x1A0 */
    virtual void vf1A4();  /* 0x1A4 */
    virtual void vf1A8();  /* 0x1A8 */
    virtual void vf1AC();  /* 0x1AC */
    virtual void vf1B0();  /* 0x1B0 */
    virtual void vf1B4();  /* 0x1B4 */
    virtual void vf1B8();  /* 0x1B8 */
    virtual void vf1BC();  /* 0x1BC */
    virtual void vf1C0();  /* 0x1C0 */
    virtual void vf1C4();  /* 0x1C4 */
    virtual void vf1C8();  /* 0x1C8 */
    virtual void vf1CC();  /* 0x1CC */
    virtual void vf1D0();  /* 0x1D0 */
    virtual void vf1D4();  /* 0x1D4 */
    virtual void vf1D8();  /* 0x1D8 */
    virtual void vf1DC();  /* 0x1DC */
    virtual void vf1E0();  /* 0x1E0 */
    virtual void vf1E4();  /* 0x1E4 */
    virtual void vf1E8();  /* 0x1E8 */
    virtual void vf1EC();  /* 0x1EC */
    virtual void vf1F0();  /* 0x1F0 */
    virtual void vf1F4();  /* 0x1F4 */
    virtual void vf1F8();  /* 0x1F8 */
    virtual void vf1FC();  /* 0x1FC */
    virtual void vf200();  /* 0x200 */
    virtual void vf204();  /* 0x204 */
    virtual void vf208();  /* 0x208 */
    virtual void vf20C();  /* 0x20C */
    virtual void vf210();  /* 0x210 */
    virtual void vf214();  /* 0x214 */
    virtual void vf218();  /* 0x218 */
    virtual void vf21C();  /* 0x21C */
    virtual void vf220();  /* 0x220 */
    virtual void vf224();  /* 0x224 */
    virtual void vf228();  /* 0x228 */
    virtual void vf22C();  /* 0x22C */
    virtual void SetPos(const Vec3&);  /* 0x230 */
    virtual void SetPos2(const Vec3&);  /* 0x234 */
    virtual void vf238();  /* 0x238 */
    virtual void vf23C();  /* 0x23C */
    virtual void vf240();  /* 0x240 */
    virtual void vf244();  /* 0x244 */
    virtual void vf248();  /* 0x248 */
    virtual void vf24C();  /* 0x24C */
    virtual void vf250();  /* 0x250 */
    virtual void vf254();  /* 0x254 */
    virtual void vf258();  /* 0x258 */
    virtual void vf25C();  /* 0x25C */
    virtual void vf260();  /* 0x260 */
    virtual void vf264();  /* 0x264 */
    virtual void vf268();  /* 0x268 */
    virtual void vf26C();  /* 0x26C */
    virtual void vf270();  /* 0x270 */
    virtual void vf274(f32);  /* 0x274 */
    virtual void vf278();  /* 0x278 */
    virtual void vf27C();  /* 0x27C */
    virtual void vf280();  /* 0x280 */
    virtual void vf284();  /* 0x284 */
    virtual void vf288();  /* 0x288 */
    virtual void vf28C();  /* 0x28C */
    virtual void vf290();  /* 0x290 */
    virtual void vf294();  /* 0x294 */
    virtual void vf298();  /* 0x298 */
    virtual void vf29C();  /* 0x29C */
    virtual void vf2A0();  /* 0x2A0 */
    virtual void vf2A4();  /* 0x2A4 */
    virtual void vf2A8();  /* 0x2A8 */
    virtual void vf2AC();  /* 0x2AC */
    virtual void vf2B0();  /* 0x2B0 */
    virtual void vf2B4();  /* 0x2B4 */
    virtual void vf2B8();  /* 0x2B8 */
    virtual void vf2BC();  /* 0x2BC */
    virtual void vf2C0();  /* 0x2C0 */
    virtual void vf2C4();  /* 0x2C4 */
    virtual void vf2C8();  /* 0x2C8 */
    virtual void vf2CC();  /* 0x2CC */
    virtual void vf2D0();  /* 0x2D0 */
    virtual void vf2D4();  /* 0x2D4 */
    virtual void vf2D8();  /* 0x2D8 */
    virtual void vf2DC();  /* 0x2DC */
    virtual void vf2E0();  /* 0x2E0 */
    virtual void vf2E4();  /* 0x2E4 */
    virtual void vf2E8();  /* 0x2E8 */
    virtual void vf2EC(int);  /* 0x2EC */
    virtual void vf2F0();  /* 0x2F0 */
    virtual void vf2F4();  /* 0x2F4 */
    virtual void vf2F8(int);  /* 0x2F8 */
    virtual void vf2FC();  /* 0x2FC */

    s32 m5F0;
    u8 m5F4;
    u8 pad5F5[3];
    s32 m5F8;
    s32 m5FC;
    u8 pad600[4];
    f32 m604;
    f32 m608;
    f32 m60C;
    f32 m610;
    u8 pad614[0x630 - 0x614];
    f32 m630;
    u8 pad634[0x6A4 - 0x634];
    u8 m6A4;
    u8 m6A5;
    u8 pad6A6[0x6C4 - 0x6A6];
    u8 m6C4;
    u8 m6C5;
    u8 pad6C6[0x74C - 0x6C6];
    u8 m74C;
    u8 pad74D[3];
    f32 m750;
    u8 pad754[0x760 - 0x754];
    u8 m760;
    u8 m761;
    u8 m762;
    u8 m763;
    u8 m764;
    u8 m765;
    u8 m766;
    u8 pad767[0x76C - 0x767];
    u8 m76C;
    u8 m76D;
    u8 m76E;
    u8 m76F;
    u8 m770;
    u8 pad771[3];
    s32 m774;
    u8 pad778[4];
    f32 m77C;
    u8 pad780[0x7B1 - 0x780];
    u8 m7B1;
    u8 m7B2;
    u8 pad7B3;
    List m7B4;
    u8 pad7C0[0x8E4 - 0x7C0];
    Member8E4 m8E4;
    Member944 m944;
    u8 m998;
};

Actor::~Actor() {
    if (lbl_18_bss_4AE0) {
        fn_18_3BA60(lbl_18_bss_4AE0, m4);
    }
}

void Actor::Reset() {
    vf2EC(0);
    m760 = 0;
    m761 = 0;
    m74C = 1;
    m762 = 0;
    m764 = 0;
    m765 = 0;
    m766 = 0;
    m763 = 0;
    m76F = 0;
    vf2F8(0);
    m76E = 0;
    m76D = 0;
    m76C = 0;
    m6A4 = 0;
    m6A5 = 0;
    m6C4 = 0;
    m6C5 = 1;
    m770 = 0;
    m5F8 = 0;
    m5FC = 0;
    m998 = 0;
    m5F4 = 0;
    m774 = 0;
}

void Actor::vfF4() {
    m5FC = 0;
    m766 = 0;
    m998 = 0;
    m5F4 = 0;
}

void Actor::vfF8() {}

void Actor::Update() {
    if (!((u8*)this)[0x1C4]) {
        m7B1 = 0;
        return;
    }
    if (!m7B1 && !m7B2) {
        vf278();
        m7B2 = 1;
    }
    m7B1 = 0;
    fn_18_3E500(this);
    Vec3 pos = *(Vec3*)fn_80060100(static_cast<Base68&>(*this));
    pos.y -= m77C;
    SetPos(pos);
    SetPos2(pos);
    Vec3* p = (Vec3*)fn_800601B0(static_cast<Base68&>(*this));
    m604 = p->x;
    m608 = p->y;
    m60C = p->z;
    vf1F4();
    if (GetState() != 0x22) {
        vf110();
        vf104();
        if (m630 > 0.0f) {
            vf2C8();
        }
        fn_18_3CBC0(static_cast<Base68&>(*this));
        vf274(m610);
        vf1C4();
        if (m74C) {
            fn_18_3CB20(static_cast<Base68&>(*this), m750 > 0.0f);
        }
        vf108();
        vf10C();
    }
}

int Actor::GetState() {
    return m5F0;
}
