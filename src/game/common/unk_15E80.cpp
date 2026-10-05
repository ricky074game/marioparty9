/* Shared by most REL modules. Reference copy: mg9101 (fn_18_15E80). */
#include "game/common/unk_15E80.h"

inline char* Allocate(size_t n) {
    char* p = (char*)operator new(n);
    if (p == 0) {
        MslError("Memory allocation failure");
    }
    return p;
}

void String::reserve(size_t n) {
    if (n > max_size()) {
        MslError("basic_string::reserve length_error");
    }
    size_t new_cap;
    size_t cap;
    unsigned long was_long;
    size_t sz;
    was_long = mLong.is_long;
    if (!was_long) {
        sz = mShort.size;
        cap = kShortCap;
    } else {
        sz = mLong.size;
        cap = mLong.cap;
    }
    if (n < sz) {
        n = sz;
    }
    new_cap = kShortCap;
    if (n + 1 > kShortCap) {
        new_cap = (n + 16) & ~15;
    }
    if (new_cap == cap) {
        return;
    }
    char* new_data;
    char* old_data;
    bool new_long;
    if (new_cap == kShortCap) {
        old_data = mLong.data;
        new_data = mShort.data;
        new_long = false;
    } else {
        if (new_cap > cap) {
            new_data = Allocate(new_cap);
        } else {
            new_data = Allocate(new_cap);
            if (new_data == 0) {
                return;
            }
        }
        new_long = true;
        old_data = was_long ? mLong.data : mShort.data;
    }
    memmove(new_data, old_data, sz);
    new_data[sz] = 0;
    if (was_long) {
        delete old_data;
    }
    mLong.is_long = new_long;
    if (!new_long) {
        mShort.size = sz;
    } else {
        mLong.data = new_data;
        mLong.size = sz;
        mLong.cap = new_cap;
    }
}
