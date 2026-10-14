/* Shared by most REL modules. Reference copy: mg9101 (fn_18_1B330). */
#include "types.h"

typedef unsigned long size_t;

extern "C" size_t strlen(const char*);
void* operator new(size_t);

struct StrTag {
    StrTag() {}
};

/* MSL-style string with short-string optimization (12 bytes). */
struct StrBuf {
    bool IsLong() const { return w0 >> 31; }
    size_t size() const {
        bool l = IsLong();
        return !l ? (((const u8*)this)[0] & 0x7F) : w1;
    }
    void Clear() { Replace(0, -1, 0, 0, StrTag()); }
    void Append(const char* s) {
        size_t n = size();
        Replace(n, 0, s, s + strlen(s), StrTag());
    }
    void Assign(const char* s) {
        Clear();
        Append(s);
    }
    void Replace(size_t pos, size_t n, const char* first, const char* last, StrTag tag); /* fn_18_16040 */
    u32 w0, w1, w2;
};

struct SlotNameEntry {
    char name[16];
    int valueA;
    int valueB;
    char pad[8];
};
extern SlotNameEntry lbl_18_rodata_4C0[];

class GameState;
extern GameState* lbl_8026E2A8; /* singleton instance (lbl_8026E2A4 + 4) */

class GameState {
public:
    GameState();
    static GameState* Get() {
        if (lbl_8026E2A8 == 0) {
            new GameState;
        }
        return lbl_8026E2A8;
    }

    u8 pad0[0x9A0];
    int mUnk9A0;
    int mUnk9A4;
    u8 mUnk9A8;
    u8 pad9A9[3];
    int mUnk9AC;
    int mUnk9B0;
    int mNumPlayers;
    int mUnk9B8;
    u8 mUnk9BC;
    u8 mUnk9BD;
    u8 mUnk9BE[1];
    u8 pad9BF[0x6BAC - 0x9BF];
    StrBuf mName; /* 0x6BAC */
    u8 pad6BB8[0x6BC8 - 0x6BB8];
    u8 mUnk6BC8[3];
    u8 pad6BCB[0x6BDC - 0x6BCB];
    int mUnk6BDC;
    u8 pad6BE0[0x6C10 - 0x6BE0];
};

int fn_18_1B330(int i) {
    return lbl_18_rodata_4C0[i].valueA;
}

int fn_18_1B350(int i) {
    return lbl_18_rodata_4C0[i].valueB;
}

void SetUnk9A4(int v) {
    GameState::Get()->mUnk9A4 = v;
}

void SetUnk9A0(int v) {
    GameState::Get()->mUnk9A0 = v;
}

int GetUnk9A0() {
    return GameState::Get()->mUnk9A0;
}

int GetUnk9AC() {
    return GameState::Get()->mUnk9AC;
}

void SetName(const char* s) {
    GameState* g = GameState::Get();
    g->mName.Replace(0, -1, 0, 0, StrTag());
    g->mName.Append(s);
}

u8 GetUnk9BC() {
    return GameState::Get()->mUnk9BC;
}

BOOL TestUnk9BE(int bit) {
    int byte = bit >> 3;
    int b = bit & 7;
    return ((1 << b) & GameState::Get()->mUnk9BE[byte]) != 0;
}

u8 GetUnk6BC8() {
    return GameState::Get()->mUnk6BC8[0];
}

u8 GetUnk6BC9() {
    return GameState::Get()->mUnk6BC8[1];
}

void SetUnk6BDC(int v) {
    GameState::Get()->mUnk6BDC = v;
}

int GetNumPlayers() {
    return GameState::Get()->mNumPlayers;
}
