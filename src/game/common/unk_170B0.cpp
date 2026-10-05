/* Shared by most REL modules. Reference copy: mg9101 (fn_18_170B0). */
#include "game/common/unk_170B0.h"

template <>
SceneBase* Singleton<SceneBase>::sInstance;

SceneBase::SceneBase() : Singleton<SceneBase>(this) {
    mSub = new SubProc;
    fn_8008CB30(this);
}

SceneBase::~SceneBase() {
    delete mSub;
}

SubProc::~SubProc() {}
