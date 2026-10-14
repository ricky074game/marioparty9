/* Shared by most REL modules. Reference copy: mg9101 (fn_18_17270). */
#include "game/common/unk_170B0.h"

void fn_8008CB40(SceneBase*, int);
void fn_800715E0(float);

extern unsigned long lbl_8026E2A4;

/* Owner holding a heap-allocated block at +8 */
struct SceneOwner {
    int m0;
    int m4;
    int* mBlock;

    void Shutdown();
};

void fn_18_17270(SceneBase* scene, int arg) {
    fn_8008CB40(scene, arg);
    fn_800715E0(-1.0f);
}

void SceneOwner::Shutdown() {
    if (lbl_8026E2A4 & 0x80) {
        int* block = mBlock;
        if (block != 0) {
            mBlock = 0;
            operator delete(block);
        }
        SceneBase* scene = Singleton<SceneBase>::sInstance;
        scene->Unk10();
        fn_8008CB40(scene, -1);
        fn_800715E0(-1.0f);
        fn_800715E0(-1.0f);
    }
}

void fn_18_17330() {}
