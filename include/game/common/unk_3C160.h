#ifndef GAME_COMMON_UNK_3C160_H
#define GAME_COMMON_UNK_3C160_H

#include "types.h"

namespace unk_3C160 {

/* main.dol model object */
class Model {
public:
    ~Model(); /* fn_8003F020 */
};

/* main.dol resource object */
class Resource {
public:
    ~Resource(); /* fn_8003E110 */
};

class Effect {
public:
    virtual ~Effect();
};

/* boost::noncopyable-style empty base: needed so inlined member dtors keep
   "addic. r3,rX,off; beq; lwz r3,0(r3)" instead of folding the offset. */
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

class Unk158 {
public:
    Unk158() : mValue(0) {}
    ~Unk158(); /* fn_80086F10 */

    s32 mValue;
};

class Unk154 {
public:
    Unk154() : mValue(0) {}

    s32 mValue;
    Unk158 mSub;
};

/* main.dol base object */
class ObjBase {
public:
    ObjBase();          /* fn_8005BDB0 */
    virtual ~ObjBase(); /* fn_8005CE40 */
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();

    u8 mBase[0x148 - 0x4];
};

class Owner;
s32 fn_18_1BD40(Owner* owner);

class Obj : public ObjBase {
public:
    Obj(Owner* owner, s32 arg);
    virtual ~Obj();

    static s32 sRefCount;
    static ScopedPtr<Resource> sResource;

    /* 0x148 */ s32 m148;
    /* 0x14C */ Owner* mOwner;
    /* 0x150 */ s32 m150;
    /* 0x154 */ Unk154 m154;
    /* 0x15C */ u8 m15C;
    /* 0x15D */ u8 m15D;
    /* 0x15E */ u8 m15E;
    /* 0x15F */ u8 m15F;
    /* 0x160 */ s32 m160;
    /* 0x164 */ u8 m164;
    /* 0x165 */ u8 m165;
    /* 0x166 */ u8 m166;
    /* 0x167 */ u8 m167;
    /* 0x168 */ u8 m168;
    /* 0x169 */ u8 m169;
    /* 0x16A */ u8 m16A;
    /* 0x16B */ u8 m16B;
    /* 0x16C */ u8 m16C;
    /* 0x170 */ s32 m170;
    /* 0x174 */ u8 m174;
    /* 0x175 */ u8 m175;
    /* 0x178 */ f32 m178;
    /* 0x17C */ f32 m17C;
    /* 0x180 */ u8 m180;
    /* 0x184 */ f32 m184;
    /* 0x188 */ u8 m188;
    /* 0x18C */ s32 m18C;
    /* 0x190 */ s32 m190;
    /* 0x194 */ ScopedPtr<Model> mModels[4];
    /* 0x1A4 */ u8 m1A4;
    /* 0x1A8 */ ScopedPtr<Model> mModel;
    /* 0x1AC */ u8 m1AC;
    /* 0x1B0 */ s32 m1B0;
    /* 0x1B4 */ u8 m1B4;
    /* 0x1B8 */ s32 m1B8;
    /* 0x1BC */ ScopedPtr<Effect> mEffect;
    /* 0x1C0 */ s32 m1C0;
    /* 0x1C4 */ s32 m1C4;
    /* 0x1C8 */ u8 m1C8;
    /* 0x1C9 */ u8 m1C9;
    /* 0x1CA */ u8 m1CA;
    /* 0x1CB */ u8 m1CB;
    /* 0x1CC */ u8 m1CC;
    /* 0x1CD */ u8 m1CD;
};

} // namespace unk_3C160

#endif
