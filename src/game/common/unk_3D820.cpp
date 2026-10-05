/* Shared by most REL modules. Reference copy: mg9101 (fn_18_3D820). */
#include "types.h"

struct Mtx34 {
    f32 m[3][4];
};

struct Quat {
    f32 x, y, z, w;
};

extern "C" void fn_80192460(Mtx34* m, const Quat* q);                      /* PSMTXQuat */
extern "C" void fn_80193210(const Quat* p, const Quat* q, Quat* r, f32 t); /* C_QUATSlerp */

extern const f32 lbl_8026E3B4;

/* Owning pointer with a boost-style safe-bool conversion */
template <class T>
class OwnPtr {
public:
    typedef T* (OwnPtr::*unspecified_bool_type)() const;

    T* get() const; /* fn_18_3DA30 */
    operator unspecified_bool_type() const { return mPtr == 0 ? 0 : &OwnPtr::get; }
    T* operator->() const { return mPtr; }

    T* mPtr;
};

class HeadNode {
public:
    u8 mBase[0x84];
    Mtx34 mMtx;    // 0x84
    Quat mTarget;  // 0xB4
    Quat mStart;   // 0xC4
    f32 mTime;     // 0xD4
    f32 mDuration; // 0xD8

    void Step() {
        if (mDuration != 0.0f) {
            Quat q;
            if (mTime >= mDuration) {
                mDuration = 0.0f;
                mTime = 0.0f;
                q = mTarget;
            } else {
                mTime += lbl_8026E3B4;
                fn_80193210(&mStart, &mTarget, &q, mTime / mDuration);
            }
            fn_80192460(&mMtx, &q);
        }
    }
};

/* main.dol character base */
class CharBase {
public:
    void fn_80065DF0();
    s32 fn_80061A60();
    s32 fn_80061A70();
    u32 fn_80062040(s32);
    void fn_80062320(s32, f32);

    u8 mBase[0x1BC];
};

class Char : public CharBase {
public:
    void Update();

    OwnPtr<HeadNode> m1BC; // 0x1BC
    s32 m1C0;
    s32 m1C4;
    u8 m1C8;
    u8 m1C9;
    u8 m1CA;
    u8 m1CB;
    u8 m1CC;
    u8 m1CD;
};

void Char::Update() {
    fn_80065DF0();
    if (m1BC) {
        m1BC->Step();
    }
    if (!m1CD) {
        u32 b = fn_80061A70() == -1;
        if (!b) {
            m1C8 = false;
            m1CC = false;
        } else if (m1C8 != b) {
            m1C8 = b;
            if (m1C0) {
                s32 x = fn_80061A60();
                if (x != m1C4) {
                    m1C4 = x;
                    m1C9 = ((fn_80062040(x) >> 3) & 1) ^ 1;
                }
            }
            m1CC = m1C9;
        }
        u8 v = m1CC;
        if (m1C0) {
            if (!m1CA) {
                v = 0;
            }
            if (m1CB != v) {
                m1CB = v;
                fn_80062320(3, v ? 100.0f : 0.0f);
            }
        }
    }
}
