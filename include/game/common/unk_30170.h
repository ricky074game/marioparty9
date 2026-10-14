#ifndef GAME_COMMON_UNK_30170_H
#define GAME_COMMON_UNK_30170_H

/* Shared minigame scene base class and its helper tasks (linked into most
 * REL modules). Used by unk_30170.cpp and unk_30A00.cpp. */

#include "types.h"

void* operator new(unsigned long size);
void operator delete(void* p);

void fn_80074B60(int, f32);
void fn_8007FEF0(int, f32, f32);
void fn_80070DB0(int);
u32 fn_80070DC0();
void fn_80071590();

/* main.dol process base class */
class ProcBase {
public:
    ProcBase();
    virtual ~ProcBase();
    virtual void Run() = 0;

    void fn_800714A0(int);
    void fn_800714E0(int);
    void fn_80071580(int);

private:
    int mUnk4;
};

class Scene : public ProcBase {
public:
    virtual ~Scene();
    virtual void Run();
    virtual void Unk10();
    virtual void Unk14();
    virtual void Unk18();
    virtual void Unk1C();
    virtual void Unk20();

    void* mSub;
};

class noncopyable {
protected:
    noncopyable() {}
    ~noncopyable() {}

private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

/* Owning pointer with a boost-style safe-bool conversion */
template <class T>
class Owned : noncopyable {
public:
    typedef T* (Owned::*unspecified_bool_type)() const;

    ~Owned() { delete mPtr; }
    T* get() const { return mPtr; }
    operator unspecified_bool_type() const { return mPtr == 0 ? 0 : &Owned::get; }

    void reset(T* p = 0) {
        if (p != mPtr) {
            T* old = mPtr;
            mPtr = p;
            delete old;
        }
    }

    T* mPtr;
};

/* Reference-counted handle base (4 bytes) */
struct RefBase {
    void* m0;
};

/* Object with a virtual destructor at vtable +8 behind a 4 byte base */
class Handle260 : public RefBase {
public:
    Handle260();          /* fn_18_1CF80 */
    virtual ~Handle260(); /* fn_18_1CFA0 */
};
void fn_18_1D040(Handle260*);

/* Object owned at +0x24C (vptr at +0x14) */
struct Base14 {
    u8 pad[0x14];
};
class Obj24C : public Base14 {
public:
    virtual ~Obj24C();
    virtual void v0C();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1C();
    virtual void v20();
    virtual void v24();
    virtual void v28(int);
};

class Obj274 {
public:
    virtual ~Obj274();

    int m4;
    int m8;
};
void fn_18_1FAC0(Obj274*);

/* Player cursor set at +0x27C */
class PlayerSet {
public:
    ~PlayerSet();
};
void fn_18_19A10(PlayerSet*, int);
void fn_18_19960(PlayerSet*, int, int);

class MgScene;

/* Screen fade / scene exit task */
class Fader : public ProcBase {
public:
    virtual ~Fader();
    virtual void Run();

    /* Starts a fade out with the given exit kind unless one is running. */
    void Request(int kind) {
        if (mC != 0) {
            return;
        }
        m10 = kind;
        fn_80074B60(0, 0.5f);
        fn_8007FEF0(-1, 0.0f, 0.5f);
        mC = 1;
        fn_80070DB0(3);
    }

    u8 m8;
    volatile s32 mC;
    s32 m10;
    MgScene* m14;
    s32 m18;
    void* m1C;
};

/* Waits until the frame it was started in comes around */
class FrameWait : public ProcBase {
public:
    virtual ~FrameWait();
    virtual void Run();

    void Start(int kind) {
        m8 = fn_80070DC0();
        mC = kind;
        m10 = 1;
    }

    u32 m8;
    s32 mC;
    u8 m10;
};

/* Shared base of the minigame scenes */
class MgScene : public Scene {
public:
    virtual ~MgScene();
    virtual void Unk1C();
    virtual void Unk20();
    virtual void vf24() = 0;
    virtual void vf28() = 0;
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34() = 0;
    virtual void vf38();
    virtual void vf3C();

    void fn_30170();
    void fn_303C0();

    /* Counts down the delay and then starts the exit fade (kind 2) */
    void TickFader() {
        if (m280) {
            Fader* fader = m280.get();
            if (fader->m8) {
                if (fader->m18 > 0) {
                    fader->m18--;
                } else {
                    fader->Request(2);
                }
            }
        }
    }

    s32 m0C;
    u8 m10;
    s32 m14;
    u8 pad18[0x24C - 0x18];
    Owned<Obj24C> m24C;
    u8 pad250[4];
    s32 m254;
    u8 m258;
    u8 pad259[0x260 - 0x259];
    Owned<Handle260> m260;
    u8 pad264[0x274 - 0x264];
    Owned<Obj274> m274;
    u8 pad278[4];
    Owned<PlayerSet> m27C;
    Owned<Fader> m280;
    Owned<FrameWait> m284;
};

#endif
