/* Shared by most REL modules. Reference copy: mg9101 (fn_18_21650). */
#include "game/common/unk_21650.h"

HandCursor::~HandCursor() {
    List().remove(this);

    HandCursor* last = mMgr->Back(mIndex);
    if (last != 0) {
        last->Activate(last->m58, last->m59 = 1);
    }

    HandCursor* top = mMgr->FindActive(mIndex);
    if (top != 0) {
        HAND_CURSOR_SET_PICTURE(top, top->mPicture);
        top->SetScale(top->mScale);
    }

    CursorManager* mgr = mMgr;
    if (--mgr->mRefCount == 0) {
        delete mgr;
    }
}
