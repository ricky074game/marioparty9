/* Shared by most REL modules. Reference copy: mg9101 (fn_18_21F80). */
#include "game/common/unk_21650.h"

void HandCursor::Update(TargetPane* pane) {
    /* Dead stores: they only fix the order of the translation unit's float
       constant pool (1.0f, 0.0f, 4.0f, int-to-float bias, then -1.0f), which
       the original got from fn_18_215A0 and fn_18_21B60. */
    f32 poolOrder = 1.0f;
    poolOrder = 0.0f;
    poolOrder = 4.0f;
    f64 poolOrderBias = 4503601774854144.0;

    BOOL show = fn_8006E330(fn_80070680(fn_18_1BBC0(mPlayer)), 0)->m5E > 0;
    const CursorInfo& info = GetInfo();
    if (fn_80070DC0() <= 4) {
        if (info.active || IsHeld()) {
            show = TRUE;
        }
        if (!show || mPicture == 0 || (!info.active && !info.visible)) {
            SetVisible(0);
        } else {
            SetVisible(1);
            Vec2f pos = *GetPos();
            Vec2f scale;
            if (info.active && info.useOverride) {
                scale.x = m50;
                scale.y = m54;
            } else {
                scale = *GetScale();
            }
            f32 k = lbl_8026DAB4;
            mMtx = Mtx34(scale.x, -scale.y, 0.0f, pos.x / k, scale.y, scale.x, 0.0f, pos.y, 0.0f,
                         0.0f, -1.0f, 0.0f);
            Mtx34 s;
            PSMTXScale(&s, k, 1.0f, 1.0f);
            PSMTXConcat(&s, &mMtx, &mMtx);
        }
    }
    pane->flag40 = 1;
    pane->alpha = info.scale;
    pane->flag80 = 1;
    pane->mtx = mMtx;
}
