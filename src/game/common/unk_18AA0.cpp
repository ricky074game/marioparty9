/* Shared by most REL modules. Reference copy: mg9101 (fn_18_18AA0). */

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long size_t;

extern "C" size_t strlen(const char*);

struct _FILE {
    u8 pad[0x50];
};
extern "C" _FILE __files[];
extern "C" int fprintf(_FILE*, const char*, ...);
extern "C" void abort();

void* operator new(size_t);
void operator delete(void*);

/* ---------------------------------------------------------------------- */

int fn_80042D70(void* p);

extern void* lbl_18_data_1A88[];


struct Handle {
    virtual ~Handle() {}
    void* mPtr;

    Handle(const Handle& o) : mPtr(fn_80042D70(o.mPtr) ? o.mPtr : 0) {}
};

void fn_80049DC0(Handle h, int, int);
int fn_8004A220(Handle h);
void fn_80071590();

static inline void PlayWait(Handle h) {
    fn_80049DC0(h, 1, 1);
    while (!fn_8004A220(h)) {
        fn_80071590();
    }
}

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
    T* operator->() const { return mPtr; }
    T* get() const { return mPtr; }
};

struct Vec {
    float x, y, z;
    Vec(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};
struct Vec2 {
    float x, y;
    Vec2(float x_, float y_) : x(x_), y(y_) {}
};

struct Pane {
    u8 pad[0x84];
    float mtx[3][4];
};

struct Layout {
    ~Layout();
    void SetTranslate(const Vec&);              // fn_80048BD0
    void SetScale(const Vec2&);                 // fn_80048C10
    void SetIntParam(const char*, const int&);  // fn_80048E00
    void SetAnimFrame(const char*, float, float);  // fn_8004A5F0
    void SetVisible(int);                       // fn_80048FA0
};

struct TextBox {
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
    virtual void SetText(const char*);  // 0x74
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void SetNumber(int, int);  // 0x8c
};

struct Anim48 {
    int x0;
    ~Anim48();
    void Init(int);  // fn_80041A10
    void Draw(int);  // fn_80041A30
};

struct LayoutBase0 {
    int x0;
};

struct LayoutBase : LayoutBase0 {
    virtual ~LayoutBase();
    Pane* FindPane(const char*, bool);  // fn_80048B50
    int IsVisible();                    // fn_80048FC0
    void SetVisible(int);               // fn_80048FA0
};

int fn_8004EF20();
void fn_8004EF10(int);
void* fn_80070680(int);
int fn_8006DBC0(void*, int);
int fn_8006E290(void*);
int fn_18_1BDA0(int, int);
void fn_8007CD10(const char*, float, float, float);

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
    int WaitButton(int player);
    void Open(int num) {
        x44 = fn_8004EF20();
        mText->SetNumber(num, 0);
        SetVisible(1);
        const Handle& h = Handle(h8);
        fn_80049DC0(h, 1, 1);
        while (!fn_8004A220(h)) {
            fn_80071590();
        }
    }
    void Close() {
        const Handle& h = Handle(h10);
        fn_80049DC0(h, 1, 1);
        while (!fn_8004A220(h)) {
            fn_80071590();
        }
        SetVisible(0);
        fn_8004EF10(x44);
        mInit = 0;
    }
};

void UseString(const char*);

/* The original translation unit starts earlier (Sub's constructor etc.), whose
   string literals precede ours in the string pool. Reproduce the pool layout. */
__declspec(weak) void PoolStrings() {
    UseString("/layout/mess");
    UseString("ns_mess_warning");
    UseString("in");
    UseString("out");
    UseString("text");
    UseString("sys900");
    UseString("/layout/mess_button");
    UseString("a_button");
    UseString("loop");
    UseString("press_out");
}

int Sub::WaitButton(int player) {
    Pane* hook = FindPane("hook", true);
    mBtn->SetTranslate(Vec(hook->mtx[0][3], hook->mtx[1][3], hook->mtx[2][3]));
    mBtn->SetScale(Vec2(hook->mtx[0][0], hook->mtx[1][1]));
    mBtn->SetIntParam("a_button_ef", fn_18_1BDA0(player, 0));
    mBtn->SetAnimFrame("player_num", 0.0f, player);
    mBtn->SetVisible(1);
    PlayWait(h20);
    fn_80049DC0(h28, 1, 1);
    void* pad = fn_80070680(player);
    while (true) {
        if (fn_8006DBC0(pad, 0x800)) {
            fn_8007CD10("SEQ_SESY008", 0.0f, 1.0f, 0.0f);
            PlayWait(h30);
            mBtn->SetVisible(0);
            return 1;
        }
        if (!fn_8006E290(pad)) {
            PlayWait(h38);
            mBtn->SetVisible(0);
            return 0;
        }
        fn_80071590();
    }
}

/* ---------------------------------------------------------------------- */

typedef float Mtx[3][4];
typedef float Mtx44[4][4];

struct GXVtxDescList {
    int attr;
    int type;
};

struct GXVtxAttrFmtList {
    int attr;
    int cnt;
    int compType;
    u8 frac;
};

extern "C" {
void fn_80192BB0(Mtx44, float, float, float, float, float, float);
void fn_8019C330(Mtx44, int);
void fn_80191CC0(Mtx);
void fn_8019C430(Mtx, int);
void fn_8019C550(int);
void fn_801981B0(int);
void fn_801981E0(int, int, int, int, int, int, int);
void fn_80196180(int);
void fn_80195F30(int, int, int, int, int, int);
void fn_8019A090(int);
void fn_80199F30(int, int, int, int);
void fn_80199A90(int, int, int, int, int);
void fn_80199B10(int, int, int, int, int, int);
void fn_80199AD0(int, int, int, int, int);
void fn_80199B70(int, int, int, int, int, int);
void fn_80199D00(int, int);
void fn_80196F60(int);
void fn_8019A5B0(int);
void fn_8019A600(int);
void fn_8019A690(int);
void fn_8019A650(int, int, int);
void fn_80199E60(int, int, int, int, int);
void fn_8019A560(int, int, int, int);
void fn_801957D0();
void fn_80195190(const GXVtxDescList*);
void fn_801959B0(int, const GXVtxAttrFmtList*);
void fn_80196C70(int, int, int);
}

#define GX_FIFO (*(volatile u8*)0xCC008000)

static void SetupGX() {
    static const GXVtxDescList sDesc[] = {
        {9, 1},
        {13, 1},
        {255, 0},
    };
    static const GXVtxAttrFmtList sFmt[] = {
        {9, 0, 0, 0},
        {13, 1, 0, 0},
        {255, 0, 0, 0},
    };
    Mtx44 proj;
    Mtx mtx;
    fn_80192BB0(proj, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    fn_8019C330(proj, 1);
    fn_80191CC0(mtx);
    fn_8019C430(mtx, 0);
    fn_8019C550(0);
    fn_801981B0(0);
    fn_801981E0(4, 0, 0, 0, 0, 0, 2);
    fn_801981E0(5, 0, 0, 0, 0, 0, 2);
    fn_80196180(2);
    fn_80195F30(0, 1, 4, 60, 0, 125);
    fn_80195F30(1, 1, 4, 60, 0, 125);
    fn_8019A090(2);
    fn_80199F30(0, 0, 0, 255);
    fn_80199A90(0, 15, 15, 15, 8);
    fn_80199B10(0, 1, 0, 0, 0, 0);
    fn_80199AD0(0, 7, 7, 7, 7);
    fn_80199B70(0, 0, 0, 0, 0, 0);
    fn_80199D00(1, 1);
    fn_80199F30(1, 1, 1, 255);
    fn_80199A90(1, 0, 8, 14, 15);
    fn_80199B10(1, 0, 0, 3, 0, 0);
    fn_80199AD0(1, 7, 7, 7, 7);
    fn_80199B70(1, 0, 0, 0, 0, 0);
    fn_80196F60(0);
    fn_8019A5B0(1);
    fn_8019A600(0);
    fn_8019A690(1);
    fn_8019A650(0, 7, 0);
    fn_80199E60(7, 0, 0, 7, 0);
    fn_8019A560(0, 1, 0, 3);
    fn_801957D0();
    fn_80195190(sDesc);
    fn_801959B0(0, sFmt);
    fn_80196C70(0x80, 0, 4);
    GX_FIFO = 0;
    GX_FIFO = 0;
    GX_FIFO = 0;
    GX_FIFO = 0;
    GX_FIFO = 1;
    GX_FIFO = 0;
    GX_FIFO = 1;
    GX_FIFO = 0;
    GX_FIFO = 1;
    GX_FIFO = 1;
    GX_FIFO = 1;
    GX_FIFO = 1;
    GX_FIFO = 0;
    GX_FIFO = 1;
    GX_FIFO = 0;
    GX_FIFO = 1;
}

extern "C" {
void fn_8019C3F0(float*);
void fn_8019C3A0(float*);
}

void Sub::Draw() {
    if (IsVisible()) {
        float saved[7];
        fn_8019C3F0(saved);
        if (!mInit) {
            x48.Init(0);
            x4C.Init(0);
            mInit = 1;
            fn_8004EF10(0);
        }
        x48.Draw(0);
        x4C.Draw(1);
        SetupGX();
        fn_8019C3A0(saved);
    }
}

Sub::~Sub() {}

/* ---------------------------------------------------------------------- */

template <size_t N>
struct bitset {
    struct reference {
        bitset* mSet;
        size_t mPos;
        reference(bitset& b, size_t pos) : mSet(&b), mPos(pos) {}
        operator bool() const { return mSet->test(mPos); }
    };

    unsigned long mBits;

    bitset() : mBits(0) {}
    bool test(size_t pos) const {
        if (pos >= N) {
            fprintf(&__files[2], "index out of range of bitset::test");
            abort();
        }
        return (mBits & (1 << pos)) != 0;
    }
    reference operator[](size_t pos) { return reference(*this, pos); }
    bool any() const { return mBits != 0; }
    void set() { mBits = (1 << N) - 1; }
};

struct iter_tag {};

struct String {
    union {
        struct {
            u32 cap;
            u32 size;
            char* ptr;
        } l;
        struct {
            u8 size;
            char buf[11];
        } s;
    };

    void reserve(size_t);                                                 // fn_18_15E80
    void replace(size_t, size_t, const char*, const char*, iter_tag);     // fn_18_16040

    String(const char* str) {
        l.cap = 0;
        l.size = 0;
        l.ptr = 0;
        size_t len = strlen(str);
        reserve(len);
        iter_tag tag;
        replace(0, 0, str, str + len, tag);
    }
    ~String() {
        if (is_long()) {
            operator delete(l.ptr);
        }
    }
    bool is_long() const { return l.cap >> 31; }
    const char* c_str() const { return !is_long() ? (const char*)this + 1 : l.ptr; }
};

struct Process {
    Process();                // fn_80071350
    virtual ~Process();       // fn_80071420
    void* x4;

    void SetPrio(int);        // fn_800714E0
    void SetFlag(int);        // fn_80071580
    void SetAttr(int);        // fn_800714A0
    int GetGroup();           // fn_800714F0
};

int fn_80074EA0();
int fn_800751E0();
int fn_80070DB0(int);
u8 fn_8006DE50(void*);
void fn_8006DDE0(void*, u8);
void fn_8006DD40(int);
void fn_800806C0();
void fn_800807B0();
struct PadState {
    u8 pad[8];
    int state;
};
void* fn_8014B7E0();
PadState* fn_8014B880(void*, int);
int fn_8007CCE0(int, int);
void fn_8007D5C0(const char*, int, float, float, float);

struct Outer : Process {
    AutoPtr<Sub> mSub;
    bitset<4> mMask;
    String mMsg;
    int mPartner;

    Outer();
    virtual ~Outer();
    virtual void Run();
};

void Outer::Run() {
    if (!mMask.any() || !fn_80074EA0() || fn_800751E0()) {
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (!mMask[i]) {
            continue;
        }
        if (fn_8006E290(fn_80070680(i))) {
            continue;
        }
        int saved = fn_80070DB0(GetGroup());
        u8 states[4];
        for (int j = 0; j < 4; j++) {
            states[j] = fn_8006DE50(fn_80070680(j));
        }
        fn_8006DD40(0);
        fn_800806C0();
        int partner = mPartner;
        bool hasPartner = partner >= 0 && partner != i && mMask[partner];
        while (!fn_8006E290(fn_80070680(i)) || (hasPartner && !fn_8006E290(fn_80070680(mPartner)))) {
            const char* msg = mMsg.c_str();
            int player = i;
            int other = mPartner;
            if (hasPartner && !fn_8006E290(fn_80070680(other))) {
                player = mPartner;
                msg = "sys900";
                other = -1;
            }
            fn_8007CD10("SEQ_SESY014", 0.0f, 1.0f, 0.0f);
            mSub->mText->SetText(msg);
            mSub->Open(player + 1);
            bool done = false;
            if (other < 0) {
                while (!fn_8006E290(fn_80070680(player)) || fn_8014B880(fn_8014B7E0(), player)->state != 4) {
                    fn_80071590();
                }
                fn_8007CD10("SEQ_SESY015", 0.0f, 1.0f, 0.0f);
                fn_8007D5C0("SEQ_SESY015_RM", fn_8007CCE0(player, 0), 0.0f, 1.0f, 0.0f);
            } else if (mSub->WaitButton(other)) {
                done = true;
            }
            mSub->Close();
            if (done) {
                break;
            }
        }
        fn_80070DB0(saved);
        for (int j = 0; j < 4; j++) {
            fn_8006DDE0(fn_80070680(j), states[j]);
        }
        fn_800807B0();
    }
}

struct OuterHolder {
    Outer* mPtr;
    OuterHolder();
};

__declspec(weak) Outer::Outer() : mSub(new Sub), mMsg("sys900"), mPartner(-1) {
    SetPrio(5);
    SetFlag(1);
    SetAttr(0x10);
    mMask.set();
}

OuterHolder::OuterHolder() : mPtr(new Outer) {}

Outer::~Outer() {}
