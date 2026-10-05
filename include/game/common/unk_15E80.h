#ifndef GAME_COMMON_UNK_15E80_H
#define GAME_COMMON_UNK_15E80_H

/* MSL-style basic_string<char> with short-string optimization, statically
 * linked into most REL modules. */

typedef unsigned long size_t;

struct FILE {
    unsigned char data[0x50];
};

extern "C" FILE __files[];
extern "C" int fprintf(FILE* stream, const char* format, ...);
extern "C" void abort(void);
extern "C" size_t strlen(const char* s);
extern "C" void* memmove(void* dst, const void* src, size_t n);
extern "C" int memcmp(const void* a, const void* b, size_t n);

void* operator new(unsigned long size);
void operator delete(void* p);

inline void MslError(const char* msg) {
    fprintf(&__files[2], msg);
    abort();
}

struct Allocator {
    Allocator() {}
};

template <class T>
inline const T& Min(const T& a, const T& b) {
    return (b < a) ? b : a;
}

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

    String(const char* s) {
        size_t* p = mWords;
        for (int i = 0; i < 3; i++) {
            *p++ = 0;
        }
        Init(s, strlen(s));
    }
    ~String() {
        if (L().is_long) {
            delete L().data;
        }
    }

    const Long& L() const { return *(const Long*)this; }
    const Short& S() const { return *(const Short*)this; }
    size_t size() const {
        bool l = L().is_long;
        return !l ? S().size : L().size;
    }
    const char* get(size_t& n) const {
        const char* p;
        bool l = L().is_long;
        if (!l) {
            p = S().data;
            n = S().size;
        } else {
            p = L().data;
            n = L().size;
        }
        return p;
    }
    static size_t max_size() { return 0x7FFFFFFE; }

    void reserve(size_t n); // fn_18_15E80

    int compare(const String& str) const {
        size_t n1;
        size_t len2;
        size_t n2;
        const char* s = str.get(n2);
        len2 = n2;
        n1 = size();
        size_t tmp;
        const char* p = get(tmp);
        size_t sz = tmp;
        size_t rlen = Min(sz, n1);
        int result = memcmp(p, s, Min(rlen, len2));
        if (result == 0) {
            if (rlen < n2) {
                return -1;
            }
            if (rlen == n2) {
                return 0;
            }
            return 1;
        }
        return result;
    }

    void Init(const char* s, size_t len) {
        reserve(len);
        Init(s, s + len);
    }
    void Init(const char* first, const char* last) {
        Replace(0, 0, first, last, Allocator());
    }
    // fn_18_16040
    void Replace(size_t pos, size_t n, const char* first, const char* last, Allocator alloc);

    union {
        Long mLong;
        Short mShort;
        size_t mWords[3];
    };
};

inline bool operator==(const String& lhs, const String& rhs) {
    return lhs.size() == rhs.size() && lhs.compare(rhs) == 0;
}

#endif
