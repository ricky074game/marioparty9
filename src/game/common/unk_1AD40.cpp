/* Shared by most REL modules. Reference copy: mg9101 (fn_18_1AD40). */
#include "game/common/unk_170B0.h"

extern unsigned long lbl_8026E2A4;

int fn_18_1B0C0(const char*);
void fn_18_1B4D0(const char*);
void fn_18_1B3D0(int);
void fn_8006D180();
void fn_18_26280(int);
void fn_80080540();
void fn_8007FC70();
void fn_80052B00();
void fn_8003CD70();
void fn_800750D0(int);
void fn_80071590();
int fn_80074EA0();
int fn_800751E0();
void fn_80074B60(int, float);
void fn_8007FDC0(int);
void fn_80074FB0();

template <>
Scene* Singleton<Scene>::sInstance;

Scene::Scene() : Singleton<Scene>(this) {
    String name(fn_8008D0A0().c_str());
    int id = fn_18_1B0C0(name.c_str());
    if (id != -1) {
        fn_18_1B4D0(name.c_str());
    }
    fn_18_1B3D0(id);
    fn_8006D180();
    fn_18_26280(1);
}

Scene::~Scene() {
    fn_80080540();
    fn_8007FC70();
    fn_80052B00();
    fn_8006D180();
    fn_8003CD70();
    fn_800750D0(0);
}

void Scene::Run() {
    Unk14();
    while (!(lbl_8026E2A4 & 0x100) || !fn_80074EA0()) {
        Unk18();
        fn_80071590();
    }
    fn_18_26280(0);
    fn_80071590();
    Unk1C();
    if (!fn_800751E0()) {
        fn_80074B60(0, 0.3f);
    }
    fn_8007FDC0(1);
    fn_80074FB0();
    Unk20();
    fn_18_17270(this, 0);
}
