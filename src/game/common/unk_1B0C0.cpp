/* Shared by most REL modules. Reference copy: mg9101 (fn_18_1B0C0). */
#include "game/common/unk_15E80.h"

extern const char lbl_18_rodata_4C0[80][32];

int fn_18_1B0C0(const char* name) {
    String s(name);
    for (int i = 0; i < 80; i++) {
        String t(lbl_18_rodata_4C0[i]);
        if (!(t == s)) {
            continue;
        }
        return i;
    }
    return -1;
}
