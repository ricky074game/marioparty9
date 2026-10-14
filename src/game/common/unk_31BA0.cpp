/* Shared by most REL modules. Reference copy: mg9101 (fn_18_31BA0). */
#include "game/common/unk_3C160.h"

using namespace unk_3C160;

/* Object with two Unk154 members after the main.dol object base */
class Obj31BA0 : public ObjBase {
public:
    virtual ~Obj31BA0();

    s32 m148;
    s32 m14C;
    Unk154 m150;
    Unk154 m158;
};

Obj31BA0::~Obj31BA0() {}
