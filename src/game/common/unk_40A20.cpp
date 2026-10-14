/* Shared by most REL modules. Reference copy: mg9101 (fn_18_40A20). */
#include "types.h"

/* Character resource path builders (static string buffers) plus a small
 * angle helper. */

typedef unsigned long size_t;
extern "C" size_t strlen(const char* s);
extern "C" double fmod(double, double);
void operator delete(void* p);

struct StrTag {
    StrTag() {}
};

/* MSL-style basic_string<char> with short string optimisation. */
class String {
public:
    String() {
        size_t* p = mWords;
        for (int i = 0; i < 3; i++) {
            *p++ = 0;
        }
    }
    ~String() {
        if (mWords[0] >> 31) {
            operator delete(mData);
        }
    }

    bool IsLong() const { return mWords[0] >> 31; }
    size_t size() const {
        bool l = IsLong();
        return !l ? (size_t)(*(u8*)this & 0x7F) : mWords[1];
    }
    const char* c_str() const { return !IsLong() ? (const char*)this + 1 : mData; }
    void Replace(int pos, int n, const char* first, const char* last, StrTag tag); /* fn_18_16040 */

    void Clear() {
        StrTag t;
        Replace(0, -1, 0, 0, t);
    }
    void Append(const char* s) {
        size_t pos = size();
        Replace(pos, 0, s, s + strlen(s), StrTag());
    }

    union {
        size_t mWords[3];
        struct {
            size_t w0;
            size_t w1;
            char* mData;
        };
    };
};

extern const char* const gCharaNames[12];    /* "pc01", ... */
extern const char* const gCharaDirNames[12]; /* "pc01_mario/", ... */
extern const char* const gModelSuffixes[5];  /* "m0", "m1", ... */

extern f32 gBaseConsts[5];
extern f32 gScaled[7];
extern f32 gDeltaTime;

void Nop() {}

/* Static initializer for gScaled. The original object has it right after the
 * first function, so it is written out by hand and registered in .ctors. */
static void InitScaled() {
    f32 s = 10.0f;
    f32 a = s * gBaseConsts[0];
    f32 b = s * gBaseConsts[1];
    f32 c = s * gBaseConsts[2];
    f32 d = s * gBaseConsts[3];
    f32 e = s * gBaseConsts[4];
    gScaled[0] = a;
    gScaled[1] = b;
    gScaled[2] = c;
    gScaled[3] = d;
    gScaled[4] = e;
    gScaled[5] = 5.0f * e;
    gScaled[6] = 4.0f * -e;
}

__declspec(section ".ctors") extern void* const InitScaledReference = (void*)InitScaled;

/* "/ch_base/actor/pc01_mario/" */
inline const char* CharaActorDir(int chara) {
    static String s;
    s.Clear();
    s.Append("/ch_base/actor/");
    s.Append(gCharaDirNames[chara]);
    return s.c_str();
}

/* "pc01" */
const char* GetCharaName(int chara) {
    static String s;
    s.Clear();
    s.Append(gCharaNames[chara]);
    return s.c_str();
}

/* "pc01_m0" */
const char* GetCharaModelName(int chara, int model) {
    static String s;
    s.Clear();
    s.Append(GetCharaName(chara));
    s.Append("_");
    s.Append(gModelSuffixes[model - 1]);
    return s.c_str();
}

/* "/ch_base/actor/pc01_mario/mdl/m0" */
const char* GetCharaModelPath(int chara, int model) {
    static String s;
    static String t;
    s.Clear();
    t.Clear();
    t.Append(CharaActorDir(chara));
    t.Append("mdl/");
    s.Append(t.c_str());
    s.Append(gModelSuffixes[model - 1]);
    return s.c_str();
}

/* "/ch_base/actor/pc01_mario/mot/" */
const char* GetCharaMotionDir(int chara) {
    static String s;
    s.Clear();
    s.Append(CharaActorDir(chara));
    s.Append("mot/");
    return s.c_str();
}

extern "C" f32 fn_8004DBA0(f32, f32);

/* Moves an angle (degrees) towards target by up to speed per frame. Returns 1
 * when the target is reached. */
int RotateToward(f32* out, f32 cur, f32 target, f32 speed) {
    f32 step = speed * gDeltaTime;
    f32 delta = fn_8004DBA0(target, cur);
    if (__fabs(delta) <= step) {
        *out = target;
        return 1;
    }
    f32 res = fmod(delta > 0.0f ? cur + step : cur - step, 360.0);
    if (res > 180.0f) {
        res -= 360.0f;
    }
    *out = res;
    return 0;
}
