/* Shared by most REL modules. Reference copy: mg9101 (fn_18_15E00). */
#include "types.h"

void operator delete(void*);

/* Stateless allocator constructor, emitted out of line */
struct Alloc {
    Alloc();
};

Alloc::Alloc() {}

/* MSL basic_string<char> with short-string optimization */
class String {
public:
    ~String();

    struct Long {
        unsigned long is_long : 1;
        unsigned long cap : 31;
        unsigned long size;
        char* data;
    };
    Long mLong;
};

String::~String() {
    if (mLong.is_long) {
        delete mLong.data;
    }
}
