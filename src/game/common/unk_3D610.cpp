/* Shared by most REL modules. Reference copy: mg9101 (fn_18_3D610). */
#include "game/common/unk_3CCC0.h"

void UnkChar::vf58() {
    m1BC.reset(new UnkHeadNode((UnkNodeParent*)this, "c_head"));
}

void UnkChar::vf5C(const UnkMtx* m) {
    if (m1BC.mPtr == 0) {
        vf58();
    }
    m1BC.mPtr->mMtx = *m;
}

void UnkChar::vf60() {
    m1BC.reset();
}
