/* Shared by most REL modules. Reference copy: mg9101 (fn_18_1B010). */
#include "game/common/unk_170B0.h"

void fn_18_26280(int);
void fn_8007FDC0(int);
void fn_80074B60(int, float);

void Scene::Unk14() {}

void Scene::Unk18() {}

void Scene::Unk1C() {}

void Scene::Unk20() {}

void Scene::Unk10() {
    Unk1C();
    fn_18_26280(0);
    fn_8007FDC0(0);
    fn_80074B60(0, 0.0f);
    Unk20();
}
