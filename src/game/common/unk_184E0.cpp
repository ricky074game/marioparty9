/* Shared by most REL modules. Reference copy: mg9101 (fn_18_184E0). */
#include "types.h"

void* operator new(unsigned long size);
void operator delete(void* p);

extern "C" void* __register_global_object(void* object, void* destructor, void* regmem);

/* ---------------------------------------------------------------------- */
/* Static initializers (tail of the previous translation unit)             */

static f32 sFrameRate;

static void InitFrameRate() {
    sFrameRate = 60.0f;
}

__declspec(section ".ctors") static void (*const sInitFrameRate)() = InitFrameRate;

/* vector<T*> storage: data, size, capacity */
struct VecImpl {
    int* mData;
    u32 mSize;
    u32 mCap;
    ~VecImpl() {
        if (mData) {
            mSize -= (int)mSize;
            operator delete(mData);
        }
    }
};

struct VecBase : VecImpl {
    ~VecBase() {}
};

struct VecBase2 : VecBase {
    ~VecBase2() {}
};

struct Vec : VecBase2 {
    ~Vec();
};

/* storage of a Vec whose destructor is registered by InitVec */
struct VecPod {
    int* mData;
    u32 mSize;
    u32 mCap;
};
static VecPod sVec;
static u8 sVecReg[12];

void fn_18_18530(void*, short);

static void InitVec() {
    sVec.mData = 0;
    sVec.mSize = 0;
    sVec.mCap = 0;
    __register_global_object(&sVec, (void*)fn_18_18530, sVecReg);
}

__declspec(section ".ctors") static void (*const sInitVec)() = InitVec;

Vec::~Vec() {}

/* ---------------------------------------------------------------------- */
/* Message window layout                                                  */

int fn_80042D70(void* p);

struct Handle {
    Handle() : mPtr(0) {}
    Handle(const Handle& o) : mPtr(fn_80042D70(o.mPtr) ? o.mPtr : 0) {}
    virtual ~Handle() {}
    Handle& operator=(const Handle& o) {
        mPtr = fn_80042D70(o.mPtr) ? o.mPtr : 0;
        return *this;
    }

    void* mPtr;
};

class noncopyable {
protected:
    noncopyable() {}
    ~noncopyable() {}

private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

template <typename T>
struct AutoPtr : noncopyable {
    T* mPtr;
    AutoPtr(T* p) : mPtr(p) {}
    ~AutoPtr() { delete mPtr; }
    void reset(T* p) {
        if (p != mPtr) {
            T* old = mPtr;
            mPtr = p;
            delete old;
        }
    }
    T* operator->() const { return mPtr; }
    T* get() const { return mPtr; }
};

struct Pane;

#define VF(n) virtual void v##n();

struct TextBox {
    TextBox(int);  // fn_8008EA90
    virtual ~TextBox();
    VF(1) VF(2) VF(3) VF(4) VF(5) VF(6) VF(7) VF(8) VF(9) VF(10)
    VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19) VF(20)
    VF(21) VF(22) VF(23) VF(24) VF(25) VF(26)
    virtual void SetText(const char*);  // 0x74
    VF(28) VF(29) VF(30) VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF(40)
    VF(41) VF(42) VF(43) VF(44) VF(45) VF(46) VF(47) VF(48) VF(49) VF(50)
    VF(51) VF(52) VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59)
    virtual void Create(void* res, Pane* pane);  // 0xF8

    u8 mData[0x1BC - 4];
};

/* main.dol layout object */
struct Layout {
    int x0;
    Layout(const char* res, const char* name, int, int);  // fn_80046CF0
    ~Layout();                                            // fn_80046040
    void SetPriority(int);                                // fn_80048860
    void SetLayer(int);                                   // fn_800489D0
    Pane* FindPane(const char*, bool);                    // fn_80048B50
    void* GetResource();                                  // fn_800486C0
    void SetVisible(int);                                 // fn_80048FA0
    Handle GetAnim(const char*, int);                     // fn_80049110
};

struct LayoutBase : Layout {
    LayoutBase(const char* res, const char* name, int, int);  // fn_18_1C700
    virtual ~LayoutBase();
};

struct Anim48 {
    int x0;
    Anim48();            // fn_80041410
    void Set1(int);      // fn_800419D0
    void Set2(int);      // fn_80041570
    void Set3(int);      // fn_80041770
};

int fn_8004EF20();
void fn_8004A3C0(Handle h, int);

struct Sub : LayoutBase {
    Handle h8;
    Handle h10;
    AutoPtr<TextBox> mText;
    AutoPtr<Layout> mBtn;
    Handle h20;
    Handle h28;
    Handle h30;
    Handle h38;
    u8 mInit;
    int x44;
    Anim48 x48;
    Anim48 x4C;

    Sub();
    virtual ~Sub();
    virtual void Draw();
};

Sub::Sub()
    : LayoutBase("/layout/mess", "ns_mess_warning", 0, 0),
      h8(GetAnim("in", 0)),
      h10(GetAnim("out", 0)),
      mText(0),
      mBtn(0),
      mInit(0),
      x44(fn_8004EF20()) {
    SetPriority(0x9080);
    SetLayer(5);
    SetVisible(0);
    mText.reset(new TextBox(0));
    mText->Create(GetResource(), FindPane("text", true));
    mText->SetText("sys900");
    mBtn.reset(new Layout("/layout/mess_button", "a_button", 0, 0));
    mBtn->SetPriority(0x9081);
    mBtn->SetLayer(5);
    h20 = mBtn->GetAnim("in", 0);
    fn_8004A3C0(h20, 0);
    h28 = mBtn->GetAnim("loop", 0);
    fn_8004A3C0(h28, 1);
    h30 = mBtn->GetAnim("press_out", 0);
    fn_8004A3C0(h30, 0);
    h38 = mBtn->GetAnim("out", 0);
    fn_8004A3C0(h38, 0);
    mBtn->SetVisible(0);
    x48.Set1(1);
    x4C.Set2(0);
    x4C.Set3(0);
    x4C.Set1(1);
}

/* Empty class destructor emitted out of line */
struct EmptyObj {
    ~EmptyObj();
};

EmptyObj::~EmptyObj() {}

class Object {
public:
    virtual ~Object();
};

/* Owning pointer to a polymorphic object */
struct ObjectPtr {
    Object* mPtr;
    ~ObjectPtr();
};

ObjectPtr::~ObjectPtr() {
    delete mPtr;
}
