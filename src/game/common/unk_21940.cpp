/* Shared by most REL modules. Reference copy: mg9101 (fn_18_21940). */
#include "game/common/unk_21650.h"

/* In the original translation unit these literals were already pooled by
   earlier functions (see fn_18_21030), ahead of the ones used below. */
extern const char* const lbl_18_stringPoolHead[];
const char* const lbl_18_stringPoolHead[] = {
    "Picture_00",
    "Picture_01",
    "Picture_02",
    "vector length error",
};

extern const char* lbl_18_layoutNames[];

inline const char* GetLayoutName(int i) {
    return lbl_18_layoutNames[i];
}

void HandCursor::Init() {
    CursorManager* mgr = CursorManager::sInstance;
    if (mgr == 0) {
        mgr = new CursorManager;
    }
    mMgr = mgr;
    mgr->mRefCount++;

    const CursorInfo& info = GetInfo();
    SetScale(info.scale);

    HandCursor* top = mMgr->FindActive(mIndex);
    if (top != 0) {
        mPicture = top->mPicture;
        m58 = top->m58;
        m59 = top->m59;
    } else {
        mPicture = GetDefaultPicture();
        m58 = info.active;
        m59 = fn_8006DE50(fn_80070680(fn_18_1BBC0(mPlayer)));
    }
    Activate(0, m59);

    HandCursor* self = this;
    PtrVector<HandCursor>& list = List();
    HandCursor** it;
    for (it = list.begin(); it != list.end(); ++it) {
        if ((*it)->mOrder > mOrder) {
            list.insert(it, self);
            return;
        }
    }
    list.insert(it, self);
}

void HandCursor::Setup(void* res) {
    char name[128];
    sprintf(name, "hand_pc%02d", fn_18_1BD40(mPlayer) + 1);
    if (res == 0) {
        Load(GetLayoutName(fn_18_1BD40(mPlayer)), name, 0, 0);
    } else {
        Load(ResHandle(res), name, 0);
    }
    SetPriority(mPriorityLayer);
    SetLayer(4);
    SetCallback(0, UpdateCallback);
    SetUserData(this);

    f32 frame = mPlayer;
    if (fn_18_1BC20(mPlayer)) {
        frame = 4.0f;
    }
    SetAnimFrame("Picture_03", frame, 0.0f);
    SetAnimFrame("Picture_04", frame, 0.0f);
    SetAnimFrame("Picture_05", frame, 0.0f);

    mEnabled = 1;
    HAND_CURSOR_SET_PICTURE(this, mPicture);
}

const char* lbl_18_layoutNames[] = {
    "ch_base/pc_cursor", "ch_base/pc_cursor", "ch_base/pc_cursor", "ch_base/pc_cursor",
    "ch_base/pc_cursor", "ch_base/pc_cursor", "ch_base/pc_cursor", "ch_base/pc_cursor",
    "ch_base/pc_cursor", "ch_base/pc_cursor", "ch_base/pc_cursor", "ch_base/pc_cursor",
};
