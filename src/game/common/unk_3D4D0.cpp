/* Shared by most REL modules. Reference copy: mg9101 (fn_18_3D4D0). */
#include "game/common/unk_3CCC0.h"

void UnkChar::vf40() {
    if (m1A8) {
        if (m1A8.mPtr->fn_8003F710()) {
            UnkVec v;
            fn_80062780("skl_root", &v);
            m1A8.mPtr->fn_8003F4E0(&v);
        } else {
            m1A8.reset();
        }
    }
}

void UnkChar::vf4C(u32 v) {
    if (v <= 15) {
        m1AC = 1;
        m1B0 = v;
    } else {
        m1AC = 0;
        m1B0 = 0;
    }
}

u32 UnkChar::vf50() {
    return m1AC ? m1B0 : UnkCharBase::vf50();
}
