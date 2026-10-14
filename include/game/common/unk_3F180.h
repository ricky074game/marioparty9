#ifndef GAME_COMMON_UNK_3F180_H
#define GAME_COMMON_UNK_3F180_H

#include "game/common/unk_15E80.h"
#include "types.h"

/* Function-local static string with the same layout as String (12 bytes,
 * short-string optimisation); the heavy lifting is String::Replace. */
class NameBuf {
public:
    NameBuf() {
        size_t* p = mWords;
        for (int i = 0; i < 3; i++) {
            *p++ = 0;
        }
    }
    ~NameBuf() {
        if (mLong.is_long) {
            delete mLong.data;
        }
    }

    size_t size() const {
        bool l = mLong.is_long;
        return !l ? mShort.size : mLong.size;
    }
    const char* c_str() const { return !mLong.is_long ? mShort.data : mLong.data; }

    void clear() {
        if (!mLong.is_long) {
            mShort.data[0] = 0;
            mShort.size = 0;
        } else {
            mLong.data[0] = 0;
            mLong.size = 0;
        }
    }

    /* The unused local keeps the stack temporaries in the original order. */
    void assign(const char* s) {
        size_t n = size();
        Allocator unused;
        ((String*)this)->Replace(0, n, s, s + strlen(s), Allocator());
    }
    void append(const char* s) {
        size_t n = size();
        Allocator unused;
        ((String*)this)->Replace(n, 0, s, s + strlen(s), Allocator());
    }

    /* Variant for a conditional argument (evaluated lazily at each use). */
    void append2(const char* s) {
        size_t n = size();
        Allocator a;
        size_t len = strlen(s);
        ((String*)this)->Replace(n, 0, s, s + len, a);
    }

    union {
        String::Long mLong;
        String::Short mShort;
        size_t mWords[3];
    };
};

struct VoiceParam {
    const char* name0;
    f32 f04;
    const char* name1;
    f32 f0C;
    f32 f10;
    f32 f14;
};

class VoiceTable {
public:
    virtual void Vf08() = 0;
    virtual void Vf0C() = 0;
    virtual void Vf10() = 0;
    virtual void Vf14() = 0;
    virtual void Vf18() = 0;
    virtual void Reset();                                               /* 0x1C, fn_18_3F190 */
    virtual void Vf20() = 0;
    virtual void SetRate(f32 rate);                                     /* 0x24, fn_18_3F180 */
    virtual void Vf28(int kind, const VoiceParam* param) = 0;           /* 0x28 */
    virtual void SetName(int type, const char* name, int idx);   /* 0x2C, fn_18_3FA70 */
    virtual void Vf30() = 0;
    virtual void Vf34() = 0;
    virtual void Vf38() = 0;
    virtual void Vf3C() = 0;
    virtual void Vf40() = 0;
    virtual void Vf44() = 0;
    virtual void Vf48() = 0;
    virtual void Vf4C() = 0;
    virtual void Vf50() = 0;
    virtual void Vf54() = 0;
    virtual void Vf58() = 0;
    virtual void Vf5C() = 0;
    virtual void Vf60() = 0;
    virtual void Vf64() = 0;
    virtual void Vf68() = 0;
    virtual void Vf6C() = 0;
    virtual void Vf70() = 0;
    virtual void Vf74() = 0;
    virtual void Vf78() = 0;
    virtual const char* Vf7C(const char* id) = 0;                       /* 0x7C */

    const char* GetVoiceName(const char* suffix);                      /* fn_18_3F4A0 */
    const char* GetSeName(int type, int mode, int idx);                /* fn_18_3F6B0 */

    int mType;      /* 0x04 */
    u8 m08[0x2C];
    f32 mRate;      /* 0x34 */
    u8 m38[4];
    NameBuf mNames[29];     /* 0x3C */
    NameBuf mNamesA[14];    /* 0x198 */
    NameBuf mNamesB[14];    /* 0x240 */
    NameBuf mNamesC[14];    /* 0x2E8 */
};

#endif
