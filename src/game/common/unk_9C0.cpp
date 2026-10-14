/* Shared by most REL modules. Reference copy: mg9101 (fn_18_9C0). */
#include "types.h"

extern f32 lbl_18_data_0[6];

/* Scaled physics-style parameters, computed once at load time */
struct Params {
    f32 v[7];
    Params() {
        for (int i = 0; i < 5; i++) {
            v[i] = 10.0f * lbl_18_data_0[i];
        }
        v[5] = 5.0f * v[4];
        v[6] = 4.0f * -v[4];
    }
};

static Params sParams;
