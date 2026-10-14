#ifndef GAME_COMMON_UNK_3E330_H
#define GAME_COMMON_UNK_3E330_H

#include "types.h"

void* operator new(unsigned long);
void operator delete(void*);

struct VecBase {
    f32 x, y, z;
};

struct Vec : public VecBase {
    Vec() {}
    Vec(f32 _x, f32 _y, f32 _z) {
        x = _x;
        y = _y;
        z = _z;
    }
};

/* 12-byte string with short-string optimisation (see unk_15E80.h) */
class NameStr {
public:
    NameStr() {
        u32* p = mWords;
        for (int i = 0; i < 3; i++) {
            *p++ = 0;
        }
    }
    ~NameStr() {
        if (IsLong()) {
            operator delete(mData);
        }
    }
    bool IsLong() const { return mWords[0] >> 31; }
    u32 size() const {
        bool l = IsLong();
        return !l ? (u8)(*(const u8*)this & 0x7F) : mWords[1];
    }
    const char* c_str() const {
        bool l = IsLong();
        return !l ? (const char*)this + 1 : mData;
    }

    union {
        u32 mWords[3];
        struct {
            u32 mWord0, mSize;
            char* mData;
        };
    };
};

/* main.dol node embedded in the character */
class Node {
public:
    virtual ~Node();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();

    void fn_800600E0(const VecBase* v);
    void fn_80060190(const VecBase* v);

    u8 mData[0x5A8 - 4];
};

struct BodyPart {
    u8 mKind;
    s32 mValue;
};

/* Character (extends a large main.dol base) */
class Chr {
public:
    virtual ~Chr();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C();
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C();
    virtual void vf60();
    virtual void vf64();
    virtual void vf68();
    virtual void vf6C();
    virtual void vf70();
    virtual void vf74();
    virtual void vf78();
    virtual void vf7C();
    virtual void vf80();
    virtual void vf84();
    virtual void vf88();
    virtual void vf8C();
    virtual void vf90();
    virtual void vf94();
    virtual void vf98();
    virtual void vf9C();
    virtual void vfA0();
    virtual void vfA4();
    virtual void vfA8();
    virtual void vfAC();
    virtual void vfB0();
    virtual void vfB4();
    virtual void vfB8();
    virtual void vfBC();
    virtual void vfC0();
    virtual void vfC4();
    virtual void vfC8();
    virtual void vfCC();
    virtual void vfD0();
    virtual void vfD4();
    virtual void vfD8();
    virtual void vfDC();
    virtual void vfE0();
    virtual void vfE4();
    virtual void vfE8();
    virtual void vfEC();
    virtual void vfF0();
    virtual void vfF4();
    virtual void vfF8();
    virtual void vfFC();
    virtual void vf100();
    virtual void vf104();
    virtual void vf108();
    virtual void vf10C();
    virtual void vf110();
    virtual void vf114();
    virtual void vf118();
    virtual void vf11C();
    virtual void vf120();
    virtual void vf124();
    virtual void vf128();
    virtual void vf12C();
    virtual void vf130();
    virtual void vf134();
    virtual void vf138();
    virtual void vf13C();
    virtual void vf140();
    virtual void vf144();
    virtual void vf148();
    virtual void vf14C();
    virtual void vf150();
    virtual void vf154();
    virtual void vf158();
    virtual void vf15C();
    virtual void vf160();
    virtual void vf164();
    virtual void vf168();
    virtual void vf16C();
    virtual void vf170();
    virtual void vf174();
    virtual void vf178();
    virtual void vf17C();
    virtual void vf180();
    virtual void vf184();
    virtual void vf188();
    virtual void vf18C();
    virtual void vf190();
    virtual void vf194();
    virtual void vf198();
    virtual void vf19C();
    virtual void vf1A0();
    virtual void vf1A4();
    virtual void vf1A8();
    virtual void vf1AC();
    virtual void vf1B0();
    virtual void vf1B4();
    virtual void vf1B8();
    virtual void vf1BC();
    virtual void vf1C0();
    virtual void vf1C4();
    virtual void vf1C8();
    virtual void vf1CC();
    virtual void vf1D0();
    virtual void vf1D4();
    virtual void vf1D8();
    virtual void vf1DC();
    virtual void vf1E0();
    virtual void vf1E4();
    virtual void vf1E8();
    virtual void vf1EC();
    virtual void vf1F0();
    virtual void vf1F4();
    virtual void vf1F8();
    virtual void vf1FC();
    virtual void vf200();
    virtual void vf204();
    virtual void vf208();
    virtual void vf20C();
    virtual void vf210();
    virtual void vf214();
    virtual void vf218();
    virtual void vf21C();
    virtual void vf220();
    virtual void vf224(const Vec& v, int a);
    virtual void vf228();
    virtual void vf22C();
    virtual void vf230(const VecBase* v);
    virtual void vf234();
    virtual void vf238();
    virtual void vf23C();
    virtual void vf240(const Vec& v);
    virtual void vf244();
    virtual void vf248();
    virtual void vf24C();
    virtual void vf250();
    virtual void vf254();
    virtual void vf258();
    virtual void vf25C();
    virtual void vf260();
    virtual void vf264();
    virtual void vf268();
    virtual void vf26C();
    virtual void vf270();
    virtual void vf274();
    virtual void vf278();
    virtual void vf27C();
    virtual void vf280();
    virtual void vf284();
    virtual void vf288();
    virtual void vf28C();
    virtual void vf290();
    virtual void vf294();
    virtual void vf298();
    virtual void vf29C();
    virtual void vf2A0();
    virtual void vf2A4();
    virtual void vf2A8();
    virtual void vf2AC();
    virtual void vf2B0();
    virtual void vf2B4();
    virtual void vf2B8();
    virtual void vf2BC();
    virtual void vf2C0();
    virtual void vf2C4();
    virtual void vf2C8();
    virtual void vf2CC();
    virtual void vf2D0();
    virtual void vf2D4();
    virtual void vf2D8();
    virtual void vf2DC();
    virtual void vf2E0();
    virtual void vf2E4();
    virtual void vf2E8();
    virtual void vf2EC();
    virtual void vf2F0();
    virtual int vf2F4();

    /* 0x04 */ u8 m04[0x0C];
    /* 0x10 */ s32 m10;
    /* 0x14 */ u8 m14;
    /* 0x15 */ u8 m15;
    /* 0x16 */ u8 m16;
    /* 0x17 */ u8 m17;
    /* 0x18 */ u8 m18;
    /* 0x19 */ u8 m19;
    /* 0x1A */ u8 m1A[0x68 - 0x1A];
    /* 0x68 */ Node mNode;
    /* 0x610 */ f32 m610;
    /* 0x614 */ VecBase m614;
    /* 0x620 */ VecBase m620;
    /* 0x62C */ VecBase m62C;
    /* 0x638 */ VecBase m638;
    /* 0x644 */ VecBase m644;
    /* 0x650 */ u8 m650[0x65C - 0x650];
    /* 0x65C */ f32 m65C;
    /* 0x660 */ u8 m660;
    /* 0x664 */ f32 m664;
    /* 0x668 */ f32 m668;
    /* 0x66C */ f32 m66C;
    /* 0x670 */ f32 m670;
    /* 0x674 */ f32 m674;
    /* 0x678 */ f32 m678;
    /* 0x67C */ f32 m67C;
    /* 0x680 */ f32 m680;
    /* 0x684 */ f32 m684;
    /* 0x688 */ f32 m688;
    /* 0x68C */ f32 m68C;
    /* 0x690 */ VecBase m690;
    /* 0x69C */ f32 m69C;
    /* 0x6A0 */ u8 m6A0[4];
    /* 0x6A4 */ u8 m6A4;
    /* 0x6A5 */ u8 m6A5;
    /* 0x6A6 */ u8 m6A6;
    /* 0x6A7 */ u8 m6A7;
    /* 0x6A8 */ VecBase m6A8;
    /* 0x6B4 */ f32 m6B4;
    /* 0x6B8 */ f32 m6B8;
    /* 0x6BC */ f32 m6BC;
    /* 0x6C0 */ u8 m6C0[8];
    /* 0x6C8 */ BodyPart mParts[8];
    /* 0x708 */ u8 m708[0x740 - 0x708];
    /* 0x740 */ s32 m740;
    /* 0x744 */ f32 m744;
    /* 0x748 */ u8 m748[4];
    /* 0x74C */ u8 m74C;
    /* 0x74D */ u8 m74D[3];
    /* 0x750 */ f32 m750;
    /* 0x754 */ f32 m754;
    /* 0x758 */ f32 m758;
    /* 0x75C */ u8 m75C[4];
    /* 0x760 */ u8 m760;
    /* 0x761 */ u8 m761[5];
    /* 0x766 */ u8 m766;
    /* 0x767 */ u8 m767[1];
    /* 0x768 */ f32 m768;
    /* 0x76C */ u8 m76C[2];
    /* 0x76E */ u8 m76E;
    /* 0x76F */ u8 m76F[0x77C - 0x76F];
    /* 0x77C */ f32 m77C;
    /* 0x780 */ u8 m780[0x8E4 - 0x780];
    /* 0x8E4 */ u8 m8E4[0x60];
    /* 0x944 */ u8 m944[0x20];

    int GetPadIndex() {
        if (m15 == 0) {
            bool b = false;
            if (m17 != 0 || m18 != 0) {
                b = true;
            }
            if (b) {
                return -1;
            }
        }
        return m10;
    }

    void NotifyPad(int arg);
    u8 GetM766();
    void SetM660(u8 v);
    void SetM74C(u8 v);
    bool IsBusy();
    bool IsMoving();
    u8 GetM76E();
    u8* GetM8E4();
    u8* GetM944();
    int GetPartValue(int i);
    u8 GetPartKind(int i);
    int GetMotionId(int i);
    void UpdateTimers();
    void ResetMotion();
    void SetM750(f32 v);
    void SetScale(f32 s);
    void SetM664(f32 a, f32 b);
    void SetM66C(f32 v);
    void SetM69C(f32 v);
    void SetM670(f32 v);
    void SetM674(f32 a, f32 b, f32 c);
    void SetM684(f32 a, f32 b);
    void SetM680(f32 v);
    void SyncNode();
    void SetPosition(const VecBase* v, bool apply);
    void CallVf224(f32 x, f32 y, f32 z, int a);
    void Translate(const VecBase* v);
    void SetM614(const VecBase* v);
    void SetM620(const VecBase* v);
    void SetM62C(const VecBase* v);
    void SetM62C3(f32 x, f32 y, f32 z);
    void SetM62Cx(f32 v);
    void SetM630(f32 v);
    void SetM634(f32 v);
    void SetM638(const VecBase* v);
    void SetM644(const VecBase* v);
    const VecBase* GetM614();
    const VecBase* GetM620();
    const VecBase* GetM62C();
    const VecBase* GetM638();
    const VecBase* GetM644();
    void SetNodePos(const VecBase* v);
    void CallVf240(f32 x, f32 y, f32 z);
    void SetAngle(f32 a);
    void StartA(const VecBase* v, f32 f);
    void StartB(f32 a, f32 b, u8 c);
    void StopAB();
};

/* sorted (name, time) cue list */
struct Cue {
    ~Cue() {}

    NameStr mName;
    f32 mTime;
};

struct CueBuf {
    ~CueBuf() {
        if (mData != 0) {
            u32 n = mSize;
            Cue* e = mData + n;
            mSize -= n;
            while (n != 0) {
                --e;
                e->~Cue();
                n--;
            }
            operator delete(mData);
        }
    }

    Cue* mData;
    u32 mSize;
    u32 mCap;
};

struct CueVec : public CueBuf {
    ~CueVec() {}
};

struct CueList : public CueVec {
    CueList();
    ~CueList();
};

class VoiceCtrl {
public:
    VoiceCtrl();
    virtual ~VoiceCtrl();
    virtual void LoadSoundGroup(int idx, bool loop, bool fade);
    virtual void vf10();
    virtual void vf14();
    virtual void vf18(int a);
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24(f32 a);
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30(int a);
    virtual void vf34();
    virtual void vf38(const char* name);
    virtual void vf3C();
    virtual void vf40();
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C();
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C(int a);
    virtual void vf60(f32 v);
    virtual void vf64();
    virtual void vf68();
    virtual void vf6C();
    virtual void vf70();
    virtual void Update(f32 time, int state, int arg);

    /* 0x04 */ s32 m04;
    /* 0x08 */ s32 m08;
    /* 0x0C */ u8 m0C;
    /* 0x0D */ u8 m0D;
    /* 0x0E */ u8 m0E;
    /* 0x0F */ u8 m0F;
    /* 0x10 */ f32 m10;
    /* 0x14 */ f32 m14;
    /* 0x18 */ f32 m18;
    /* 0x1C */ f32 m1C;
    /* 0x20 */ f32 m20;
    /* 0x24 */ f32 m24;
    /* 0x28 */ s32 m28;
    /* 0x2C */ f32 m2C;
    /* 0x30 */ u32 m30;
    /* 0x34 */ f32 m34;
    /* 0x38 */ u8 m38[4];
    /* 0x3C */ NameStr mNames0[0x1D];
    /* 0x198 */ NameStr mNames1[0xE];
    /* 0x240 */ NameStr mNames2[0xE];
    /* 0x2E8 */ NameStr mNames3[0xE];
    /* 0x390 */ s32 m390;
    /* 0x394 */ CueList mCues[2];
    /* 0x3AC */ u8 m3AC;
    /* 0x3AD */ u8 m3AD;
};

#endif
