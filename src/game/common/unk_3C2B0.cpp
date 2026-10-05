/* Shared by most REL modules. Reference copy: mg9101 (fn_18_3C2B0). */

#include "game/common/unk_3C160.h"

namespace unk_3C160 {

Obj::~Obj() {
    if (m15C) {
        for (u32 i = 0; i < 4; i++) {
            mModels[i].reset();
        }
        if (--sRefCount == 0) {
            sResource.reset();
        }
    }
}

} // namespace unk_3C160
