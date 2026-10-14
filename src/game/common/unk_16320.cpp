/* Shared by most REL modules. Reference copy: mg9101 (fn_18_16320). */
#include "types.h"

typedef unsigned long size_t;

extern "C" size_t strlen(const char* s);

extern f32 lbl_18_data_15F8[6];

static f32 sScale;

static void InitScale() {
    sScale = 10.0f * lbl_18_data_15F8[0];
}

__declspec(section ".ctors") static void (*const sInitScale)() = InitScale;

struct Allocator {
    Allocator() {}
};

class String {
public:
    enum { kShortCap = 11 };

    struct Long {
        unsigned long is_long : 1;
        unsigned long cap : 31;
        size_t size;
        char* data;
    };
    struct Short {
        unsigned char is_long : 1;
        unsigned char size : 7;
        char data[kShortCap];
    };

    String& Ins(size_t pos, const char* f, const char* l) { return Replace(pos, 0, f, l, Allocator()); }
    String& Concat(const String& lhs, const char* rhs);
    String() {
        size_t* p = mWords;
        for (int i = 0; i < 3; i++) {
            *p++ = 0;
        }
    }
    ~String();
    String(const char* s);
    const char* c_str() const;

    const Long& L() const { return *(const Long*)this; }
    const Short& S() const { return *(const Short*)this; }
    size_t size() const {
        bool l = L().is_long;
        return !l ? S().size : L().size;
    }
    const char* data() const {
        bool l = L().is_long;
        return !l ? S().data : L().data;
    }
    const char* get(size_t& n) const {
        const char* p;
        bool l = L().is_long;
        if (!l) {
            n = S().size;
            p = S().data;
        } else {
            n = L().size;
            p = L().data;
        }
        return p;
    }

    void reserve(size_t n);
    String& Replace(size_t pos, size_t n, const char* first, const char* last, Allocator alloc);

    union {
        Long mLong;
        Short mShort;
        size_t mWords[3];
    };
};

String& String::Concat(const String& lhs, const char* rhs) {
    size_t* p = mWords;
    for (int i = 0; i < 3; i++) {
        *p++ = 0;
    }
    size_t rlen = strlen(rhs);
    size_t llen;
    const char* ldata = lhs.get(llen);
    reserve(llen + rlen);
    String& t = Ins(0, ldata, ldata + llen);
    return t.Ins(llen, rhs, rhs + rlen);
}

const char* String::c_str() const {
    return !L().is_long ? (const char*)this + 1 : L().data;
}

String::String(const char* s) {
    size_t* p = mWords;
    for (int i = 0; i < 3; i++) {
        *p++ = 0;
    }
    size_t len = strlen(s);
    reserve(len);
    Replace(0, 0, s, s + len, Allocator());
}
