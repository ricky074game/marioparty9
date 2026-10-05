/* Shared by most REL modules. Reference copy: mg9101 (fn_18_21650). */
#include "game/common/unk_21650.h"

inline void HandCursor::SetPicture(int picture) {
    Reset(0);
    mPicture = picture;
    if (mEnabled) {
        mLayout.FindPane("Picture_00", 1)->SetVisible(false);
        mLayout.FindPane("Picture_01", 1)->SetVisible(false);
        mLayout.FindPane("Picture_02", 1)->SetVisible(false);
        switch (mPicture) {
        case 1:
            mLayout.FindPane("Picture_00", 1)->SetVisible(true);
            break;
        case 2:
            mLayout.FindPane("Picture_01", 1)->SetVisible(true);
            break;
        case 3:
            mLayout.FindPane("Picture_02", 1)->SetVisible(true);
            break;
        }
    }
}

HandCursor::~HandCursor() {
    PtrVector<HandCursor>& list = List();
    for (HandCursor** it = list.begin(); it != list.end(); ++it) {
        if (*it == this) {
            list.erase(it);
            break;
        }
    }

    HandCursor* last = mMgr->Back(mIndex);
    if (last != 0) {
        last->m59 = 1;
        last->Activate(last->m58, 1);
    }

    HandCursor* top;
    u32 n = List().size();
    for (u32 i = 0; i < n; i--) {
        if (List()[i]->mPicture != 0) {
            top = List()[i];
            goto found;
        }
    }
    top = 0;
found:
    if (top != 0) {
        top->SetPicture(top->mPicture);
        top->SetScale(top->mScale);
    }

    if (--mMgr->mRefCount == 0) {
        delete mMgr;
    }
}
