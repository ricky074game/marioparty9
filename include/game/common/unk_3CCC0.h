#ifndef GAME_COMMON_UNK_3CCC0_H
#define GAME_COMMON_UNK_3CCC0_H

#include "types.h"

struct UnkVecBase {
    f32 x, y, z;
};

struct UnkVec : public UnkVecBase {
    UnkVec() {}
    UnkVec(f32 s) { x = s; y = s; z = s; }
    UnkVec(f32 _x, f32 _y, f32 _z) { x = _x; y = _y; z = _z; }
};

inline f32 UnkAbs(const f32& x) { return __fabsf(x); }

/* Signed component with the largest magnitude, made positive */
inline f32 UnkVecMaxAbs(UnkVecBase v) {
    f32 m = v.x;
    m = UnkAbs(m) < UnkAbs(v.y) ? v.y : m;
    m = UnkAbs(m) < UnkAbs(v.z) ? v.z : m;
    return UnkAbs(m);
}

struct UnkMtx {
    f32 m[3][4];
};

extern "C" void fn_80191CC0(UnkMtx* m); /* PSMTXIdentity */

/* main.dol model object (fn_8003xxxx) */
class UnkModel {
public:
    ~UnkModel(); /* fn_8003F020 */
    void fn_8003F480(u32);
    void fn_8003F4A0(u32);
    void fn_8003F4E0(const UnkVec*);
    void fn_8003F540(const UnkVec*);
    void fn_8003F5B0(const UnkVec*);
    void fn_8003F5F0(u32);
    void fn_8003F6E0(u32);
    BOOL fn_8003F710();
};

UnkModel* fn_8003ECB0(u32);

/* Simple owning pointer */
template <class T>
class UnkScopedPtr {
public:
    typedef T* (UnkScopedPtr::*unspecified_bool_type)() const;

    T* get() const;
    operator unspecified_bool_type() const { return mPtr == 0 ? 0 : &UnkScopedPtr::get; }
    void reset(T* p = 0) {
        if (p != mPtr) {
            T* old = mPtr;
            mPtr = p;
            delete old;
        }
    }

    T* mPtr;
};

class UnkNodeParent;

/* main.dol node base (fn_8006xxxx) */
class UnkNodeBase {
public:
    typedef BOOL (UnkNodeBase::*Callback)(void*, UnkMtx*);

    UnkNodeBase(UnkNodeParent* parent); /* fn_8006AEF0 */
    virtual ~UnkNodeBase();             /* fn_8006AFD0 */
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();

    void SetCallback(const char* name, Callback cb); /* fn_8006B920 */

    template <class T>
    void RegisterCallback(const char* name, BOOL (T::*cb)(void*, UnkMtx*)) {
        SetCallback(name, static_cast<Callback>(cb));
    }

    u8 mBase[0x84 - 0x4];
};

class UnkHeadNode : public UnkNodeBase {
public:
    UnkHeadNode(UnkNodeParent* parent, const char* name) : UnkNodeBase(parent), mB4(0.0f, 0.0f, 0.0f), mC0(0.0f, 0.0f, 0.0f), mCC(0.0f, 0.0f, 0.0f), mD8(0.0f) {
        fn_80191CC0(&mMtx);
        BOOL (UnkHeadNode::*cb)(void*, UnkMtx*) = &UnkHeadNode::CalcCallback;
        SetCallback(name, static_cast<Callback>(cb));
    }
    virtual ~UnkHeadNode();

    BOOL CalcCallback(void*, UnkMtx* out);

    UnkMtx mMtx;  // 0x84
    UnkVec mB4;   // 0xB4
    UnkVec mC0;   // 0xC0
    UnkVec mCC;   // 0xCC
    f32 mD8;      // 0xD8
};

/* main.dol character/model base (fn_8005xxxx/fn_8006xxxx) */
class UnkCharBase {
public:
    virtual ~UnkCharBase();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20(const UnkVec* a, u32 b);
    virtual void vf24(const UnkVec* a, const UnkVec& b, const UnkVec& c, u32 d);
    virtual void vf28(u32 a, u32 b);
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C(u32 v);
    virtual u32 vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C(const UnkMtx* m);
    virtual void vf60();

    u32 fn_8005D980();
    const UnkVecBase* fn_80060240();
    void fn_80062780(const char* name, UnkVec* out);

    u8 mBase[0x1A8 - 0x4];
};

class UnkChar : public UnkCharBase {
public:
    virtual ~UnkChar();
    virtual void vf20(const UnkVec* a, u32 b);
    virtual void vf24(const UnkVec* a, const UnkVec& b, const UnkVec& c, u32 d);
    virtual void vf28(u32 a, u32 b);
    virtual void vf40();
    virtual void vf4C(u32 v);
    virtual u32 vf50();
    virtual void vf58();
    virtual void vf5C(const UnkMtx* m);
    virtual void vf60();

    UnkScopedPtr<UnkModel> m1A8;   // 0x1A8
    u8 m1AC;                       // 0x1AC
    u32 m1B0;                      // 0x1B0
    u8 m1B4[0x1BC - 0x1B4];
    UnkScopedPtr<UnkHeadNode> m1BC; // 0x1BC
};

#endif
