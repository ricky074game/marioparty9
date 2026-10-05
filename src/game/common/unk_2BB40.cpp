/* Shared by most REL modules. Reference copy: mg9101 (fn_18_2BB40). */
#include "types.h"

struct FalseType {};

class IntVector {
public:
    int* begin() { return mData; }
    void insert(int* pos, const int& x) { InsertImpl(pos, x, FalseType()); }
    void InsertImpl(int* pos, const int& x, FalseType);

    int* mData;
    u32 mSize;
    u32 mCapacity;
};

BOOL SplitDigits(IntVector& out, int digits, int number) {
    int value = number;
    int div = 1;
    for (int i = 0; i < digits - 1; i++) {
        div *= 10;
    }
    if (value >= div * 10) {
        return FALSE;
    }
    if (value < 0) {
        return FALSE;
    }
    while (div > 1) {
        out.insert(out.begin(), value / div);
        value -= div * (value / div);
        div /= 10;
    }
    out.insert(out.begin(), value);
    return TRUE;
}
