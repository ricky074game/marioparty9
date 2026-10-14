/* Shared by most REL modules. Reference copy: mg9101 (fn_18_2F820). */
#include "types.h"

void* operator new(unsigned long size);
void operator delete(void* p);

/* main.dol process base (ctor fn_80071350, dtor fn_80071420) */
class ProcBase {
public:
    ProcBase();
    virtual ~ProcBase();
    virtual void Run() = 0;

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

class Layout {
public:
    ~Layout(); /* fn_80046040 */
    void* mPtr;
};

struct LayoutHolder {
    ~LayoutHolder() { delete mLayout; }
    Layout* mLayout;
};

struct LayoutBox {
    ~LayoutBox() {}
    LayoutHolder mHolder;
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
class Owned : noncopyable {
public:
    ~Owned() { delete mPtr; }
    T* mPtr;
};

class Elem4 {
public:
    Elem4();  /* fn_18_2F820 */
    ~Elem4(); /* fn_18_2ACC0 */
    int m0;
};

class Elem4b {
public:
    Elem4b();  /* fn_18_2F830 */
    ~Elem4b(); /* fn_18_26390 */
    int m0;
};

Elem4::Elem4() : m0(0) {}

Elem4b::Elem4b() : m0(0) {}

class FrameWait : public ProcBase {
public:
    virtual ~FrameWait();
    virtual void Run();
};

class SceneTask : public ProcBase {
public:
    virtual ~SceneTask();
    virtual void Run();
};

class Fader : public ProcBase {
public:
    virtual ~Fader();
    virtual void Run();

    u8 pad8[0x1C - 8];
    Owned<LayoutBox> m1C;
};

class Obj14 {
public:
    u8 pad[0x14];
    virtual ~Obj14();
};

class Obj4 {
public:
    int m0;
    virtual ~Obj4();
};

class Obj0 {
public:
    virtual ~Obj0();
};

class OuterHolder {
public:
    ~OuterHolder(); /* fn_18_198E0 */
    void* mPtr;
};

class MgScene : public Scene {
public:
    virtual ~MgScene();
    virtual void Unk14();
    virtual void Unk1C();
    virtual void Unk20();

    s32 m0C;
    u8 m10;
    s32 m14;
    Owned<Obj0> m18;
    Elem4b mElems[0x88];
    Elem4 mElems2[4];
    Owned<Obj14> m24C;
    Owned<Obj14> m250;
    u8 pad254[0x260 - 0x254];
    Owned<Obj4> m260;
    u8 pad264[4];
    Owned<Obj0> m268;
    Owned<Obj0> m26C;
    Owned<Obj14> m270;
    Owned<Obj0> m274;
    u8 pad278[4];
    Owned<OuterHolder> m27C;
    Owned<Obj0> m280;
    Owned<Obj0> m284;
};

int fn_18_1B6D0();
void fn_80080FA0(int);

SceneTask::~SceneTask() {}

Fader::~Fader() {}

FrameWait::~FrameWait() {}

MgScene::~MgScene() {
    if (fn_18_1B6D0()) {
        if (m14 != -1) {
            fn_80080FA0(m14);
        }
    }
}

void MgScene::Unk14() {}
