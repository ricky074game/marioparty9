/* Shared by most REL modules. Reference copy: mg9101 (fn_18_3E330). */
#include "game/common/unk_3E330.h"

extern const f32 lbl_8026E3B4; /* frame step */

/* main.dol */
void* fn_80070680(int pad);
void fn_8006E0C0(void* pad, int arg);
Vec fn_8004D800(int arg, int flag);
void fn_80080AF0(const char* name, u16 flags);
void fn_800808B0(const char* name);

extern const int sMotionTable[];

void Chr::NotifyPad(int arg) {
    int idx = GetPadIndex();
    if (idx != -1) {
        fn_8006E0C0(fn_80070680(idx), arg);
    }
}

u8 Chr::GetM766() { return m766; }
void Chr::SetM660(u8 v) { m660 = v; }
void Chr::SetM74C(u8 v) { m74C = v; }

bool Chr::IsBusy() {
    bool r = false;
    if (vf2F4() != 0 || m660 != 0) {
        r = true;
    }
    return r;
}

bool Chr::IsMoving() {
    bool r = false;
    if (m760 != 0 || m754 > 0.0f || m750 > 0.0f || m744 > 0.0f) {
        r = true;
    }
    return r;
}

u8 Chr::GetM76E() { return m76E; }
u8* Chr::GetM8E4() { return m8E4; }
u8* Chr::GetM944() { return m944; }
int Chr::GetPartValue(int i) { return mParts[i].mValue; }
u8 Chr::GetPartKind(int i) { return mParts[i].mKind; }

const int sMotionTable[] = {0, 1, 2, 3, 4, 5, 6, 9, 10, 11, 12, 13, 14};

int Chr::GetMotionId(int i) { return sMotionTable[i]; }

static inline bool Tick(f32& t) {
    if (t > 0.0f) {
        t -= lbl_8026E3B4;
        if (t <= 0.0f) {
            t = 0.0f;
        }
        return true;
    }
    return false;
}

void Chr::UpdateTimers() {
    if (Tick(m744)) {
        m754 = m750 = 0.0f;
    }
    if (Tick(m754)) {
        m744 = m750 = 0.0f;
        if (m754 <= 0.0f) {
            m750 = m69C;
        }
    }
    Tick(m750);
    m758 += lbl_8026E3B4;
}

void Chr::ResetMotion() {
    m768 = 1.0f;
    m740 = 0;
    f32 z = 0.0f;
    m758 = z;
    m754 = z;
    m744 = z;
    vf2E8();
}

void Chr::SetM750(f32 v) { m750 = v; }

void Chr::SetScale(f32 s) {
    m68C = s;
    Vec t(s, s, s);
    m690 = t;
}

void Chr::SetM664(f32 a, f32 b) {
    m664 = a;
    m668 = b;
}
void Chr::SetM66C(f32 v) { m66C = v; }
void Chr::SetM69C(f32 v) { m69C = v; }
void Chr::SetM670(f32 v) { m670 = v; }
void Chr::SetM674(f32 a, f32 b, f32 c) {
    m674 = a;
    m678 = b;
    m67C = c;
}
void Chr::SetM684(f32 a, f32 b) {
    m684 = a;
    m688 = b;
}
void Chr::SetM680(f32 v) { m680 = v; }

void Chr::SyncNode() {
    VecBase t = m620;
    t.y += m77C;
    mNode.fn_800600E0(&t);
    mNode.vf40();
}

void Chr::SetPosition(const VecBase* v, bool apply) {
    vf234();
    if (apply) {
        vf230(v);
        mNode.fn_800600E0(v);
        mNode.vf40();
    }
}

void Chr::CallVf224(f32 x, f32 y, f32 z, int a) {
    Vec t(x, y, z);
    vf224(t, a);
}

/* Position += v * frame step, using paired singles (the temporary lives on the stack) */
asm void Chr::Translate(const VecBase* v) {
    nofralloc
    stwu r1, -32(r1)
    lis r5, lbl_8026E3B4@ha
    psq_l f1, 0(r4), 0, 0
    lfs f0, lbl_8026E3B4@l(r5)
    addi r5, r1, 8
    psq_l f3, 0x620(r3), 0, 0
    ps_muls0 f2, f1, f0
    psq_l f1, 8(r4), 1, 0
    psq_st f2, 0(r5), 0, 0
    ps_muls0 f2, f1, f0
    psq_l f0, 0(r5), 0, 0
    psq_st f2, 8(r5), 1, 0
    ps_add f0, f3, f0
    psq_l f3, 0x628(r3), 1, 0
    psq_st f0, 0x620(r3), 0, 0
    ps_add f0, f3, f2
    psq_st f0, 0x628(r3), 1, 0
    addi r1, r1, 32
    blr
}

void Chr::SetM614(const VecBase* v) { m614 = *v; }
void Chr::SetM620(const VecBase* v) { m620 = *v; }
void Chr::SetM62C(const VecBase* v) { m62C = *v; }
void Chr::SetM62C3(f32 x, f32 y, f32 z) {
    m62C.x = x;
    m62C.y = y;
    m62C.z = z;
}
void Chr::SetM62Cx(f32 v) { m62C.x = v; }
void Chr::SetM630(f32 v) { m62C.y = v; }
void Chr::SetM634(f32 v) { m62C.z = v; }
void Chr::SetM638(const VecBase* v) { m638 = *v; }
void Chr::SetM644(const VecBase* v) { m644 = *v; }
const VecBase* Chr::GetM614() { return &m614; }
const VecBase* Chr::GetM620() { return &m620; }
const VecBase* Chr::GetM62C() { return &m62C; }
const VecBase* Chr::GetM638() { return &m638; }
const VecBase* Chr::GetM644() { return &m644; }

void Chr::SetNodePos(const VecBase* v) {
    mNode.fn_80060190(v);
    m610 = v->y;
}

void Chr::CallVf240(f32 x, f32 y, f32 z) {
    Vec t(x, y, z);
    vf240(t);
}

void Chr::SetAngle(f32 a) {
    if (a > 180.0f) {
        a -= 360.0f;
    }
    if (a < -180.0f) {
        a += 360.0f;
    }
    m65C = a;
}

void Chr::StartA(const VecBase* v, f32 f) {
    m19 = 1;
    m6A4 = 1;
    m6A8 = *v;
    m6B8 = f;
}

void Chr::StartB(f32 a, f32 b, u8 c) {
    m19 = 1;
    m6A5 = 1;
    m6B4 = a;
    m6BC = b;
    m6A6 = c;
}

void Chr::StopAB() {
    m19 = 0;
    m6A4 = 0;
    m6A5 = 0;
}

/* Tuning table */
f32 gTuneIn[6] = {6.0f, 3.0f, 6.0f, 40.0f, -9.8f, 0.0f};

struct Tuning {
    f32 a, b, c, d, e, f, g;
};
Tuning gTuning;

extern "C" void __sinit_unk_3E330_cpp() {
    gTuning.a = 10.0f * gTuneIn[0];
    gTuning.b = 10.0f * gTuneIn[1];
    gTuning.c = 10.0f * gTuneIn[2];
    gTuning.d = 10.0f * gTuneIn[3];
    gTuning.e = 10.0f * gTuneIn[4];
    gTuning.f = 5.0f * gTuning.e;
    gTuning.g = 4.0f * -gTuning.e;
}

__declspec(section ".ctors") void (*const sCtorEntry)() = __sinit_unk_3E330_cpp;

const char* sSoundGroupNames[] = {
    "GRP_PC01_MARIO",  "GRP_PC02_LUIGI",    "GRP_PC03_PEACH",   "GRP_PC04_DAISY",
    "GRP_PC05_WARIO",  "GRP_PC06_WALUIGI",  "GRP_PC07_YOSHI",   "GRP_PC08_CATHERINE",
    "GRP_PC09_KINOPIO", "GRP_PC10_NOKONOKO", "GRP_PC11_HEYHO",   "GRP_PC12_KAMECK",
};

VoiceCtrl::VoiceCtrl() : m0C(0), m0D(0), m0E(0), m0F(0), m28(32), m390(0), m3AC(0), m3AD(0) {
    vf18(0);
    m10 = 0.0f;
    m14 = 0.0f;
    m18 = 0.0f;
    m1C = 0.0f;
    m20 = 1.0f;
    m24 = 1.0f;
    m2C = 0.0f;
    m30 = -1;
    VecBase zero = {0.0f, 0.0f, 0.0f};
    vf24(1.5f);
}

CueList::CueList() {
    mData = 0;
    mSize = 0;
    mCap = 0;
}

CueList::~CueList() {}

VoiceCtrl::~VoiceCtrl() {}

void VoiceCtrl::LoadSoundGroup(int idx, bool loop, bool fade) {
    m04 = idx;
    if (fade) {
        if (loop) {
            fn_80080AF0(sSoundGroupNames[idx], 0x8000);
        } else {
            fn_800808B0(sSoundGroupNames[idx]);
        }
    }
    vf1C();
}

void VoiceCtrl::Update(f32 time, int state, int arg) {
    if (m0F) {
        vf5C(arg);
        if (m08 == 4) {
            Vec v = fn_8004D800(arg, 0);
            v.x = v.x * m34;
            f32 r;
            if (v.x > 1.0f) {
                r = 1.0f;
            } else if (v.x < -1.0f) {
                r = -1.0f;
            } else {
                r = v.x;
            }
            v.x = r;
            vf60(v.x);
        }
    }
    if (!m0C || !m0E) {
        return;
    }
    if (m28 != state) {
        m28 = state;
        m2C = time;
        m30 = -1;
    }
    if (state <= 1) {
        bool found = false;
        u32 idx = 0;
        u32 n = mCues[state].mSize;
        if (time < m2C) {
            m2C = -1.0f;
            m30 = -1;
        }
        for (u32 i = 0; i < n; i++) {
            f32 t = mCues[state].mData[i].mTime;
            if (m2C <= t && t <= time) {
                idx = i;
                found = true;
                break;
            }
        }
        if (found && m30 != idx) {
            m30 = idx;
            Cue* c = &mCues[state].mData[idx];
            if (c->mName.size() == 0) {
                vf30(state);
            } else {
                vf38(c->mName.c_str());
            }
        }
        m2C = time;
    }
}
