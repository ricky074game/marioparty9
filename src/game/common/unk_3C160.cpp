/* Shared by most REL modules. Reference copy: mg9101 (fn_18_3C160). */

#include "game/common/unk_3C160.h"

namespace unk_3C160 {

Obj::Obj(Owner* owner, s32 arg)
    : m148(arg), mOwner(owner), m150(fn_18_1BD40(owner)), m15C(0), m15D(0), m15E(0), m15F(0), m160(-1), m164(1), m165(0), m166(0),
      m168(0), m169(0), m16A(0), m16B(0), m16C(0), m170(-1), m174(0), m175(0), m178(0.0f), m17C(0.0f), m180(0), m184(1.0f), m188(1),
      m18C(2), m190(0), m1A4(1), m1AC(0), m1B0(0), m1B4(0), m1C0(0), m1C4(-1), m1C8(0), m1C9(0), m1CA(0), m1CB(0), m1CC(0), m1CD(0) {
    m15F = 1;
}

} // namespace unk_3C160
