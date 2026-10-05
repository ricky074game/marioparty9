/* Shared by most REL modules. Reference copy: mg9101 (fn_18_21DF0). */
#include "types.h"

struct PictureNode {
    u8 pad[0xBB];
    u8 flags;

    void SetVisible(bool visible) { flags = (u8)(flags & ~1) | (visible ? 1 : 0); }
};

struct PictureScene {
    PictureNode* Find(const char* name, int recurse);
};

class PictureBase {
public:
    void Reset(int arg);
};

class PictureSelector : public PictureBase {
public:
    void SetPicture(int picture);

    u32 unk_00;
    PictureScene scene;
    u8 pad_08[0x44 - 0x05];
    int picture;
    u8 pad_48[4];
    u8 enabled;
};

void PictureSelector::SetPicture(int picture) {
    Reset(0);
    this->picture = picture;
    if (enabled) {
        scene.Find("Picture_00", 1)->SetVisible(false);
        scene.Find("Picture_01", 1)->SetVisible(false);
        scene.Find("Picture_02", 1)->SetVisible(false);
        switch (this->picture) {
        case 1:
            scene.Find("Picture_00", 1)->SetVisible(true);
            break;
        case 2:
            scene.Find("Picture_01", 1)->SetVisible(true);
            break;
        case 3:
            scene.Find("Picture_02", 1)->SetVisible(true);
            break;
        }
    }
}
