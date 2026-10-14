/* Shared by most REL modules. Reference copy: mg9101 (fn_18_3E40). */
#include "types.h"

class Manager {
public:
    virtual ~Manager();
};

struct FrameRate {
    f32 value;
    FrameRate() : value(60.0f) {}
};

static FrameRate sFrameRate;

extern Manager* lbl_18_bss_466C;
extern "C" void (*_dtors[])();

extern "C" void _epilog() {
    delete lbl_18_bss_466C;
    void (**dtor)() = _dtors;
    while (*dtor != 0) {
        (*dtor)();
        dtor++;
    }
}

extern "C" void _unresolved() {}

