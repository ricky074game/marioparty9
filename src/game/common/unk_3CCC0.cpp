/* Shared by most REL modules. Reference copy: mg9101 (fn_18_3CCC0). */
#include "game/common/unk_3CCC0.h"

void UnkChar::vf20(const UnkVec* a, u32 b) {
    static UnkVec sZero(0.0f, 0.0f, 0.0f);
    f32 m = UnkVecMaxAbs(*fn_80060240());
    UnkVec s(m, m, m);
    vf24(a, sZero, s, b);
}

void UnkChar::vf24(const UnkVec* a, const UnkVec& b, const UnkVec& c, u32 d) {
    UnkModel* model = fn_8003ECB0(d);
    if (model) {
        model->fn_8003F480(fn_8005D980());
        model->fn_8003F4A0(vf50());
        model->fn_8003F4E0(a);
        model->fn_8003F540(&b);
        model->fn_8003F5B0(&c);
    }
}

void UnkChar::vf28(u32 a, u32 b) {
    UnkModel* model = fn_8003ECB0(b);
    if (model) {
        model->fn_8003F480(fn_8005D980());
        model->fn_8003F4A0(vf50());
        model->fn_8003F6E0(1);
        model->fn_8003F5F0(a);
    }
}
