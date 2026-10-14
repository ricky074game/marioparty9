/* Shared by most REL modules. Reference copy: mg9101 (fn_18_3F180). */
#include "game/common/unk_3F180.h"

extern const VoiceParam gVoiceParamA[];
extern const VoiceParam gVoiceParamB[];

void VoiceTable::SetRate(f32 rate) {
    mRate = rate;
}

void VoiceTable::Reset() {
    NameBuf* p = mNames;
    for (int i = 0; i < 30; i++) {
        p[i].clear();
    }
    NameBuf* a = mNamesA;
    NameBuf* b = mNamesB;
    NameBuf* c = mNamesC;
    for (int i = 0; i < 14; i++) {
        a->clear();
        b->clear();
        c->clear();
        a++;
        b++;
        c++;
    }
    Vf28(0, &gVoiceParamA[mType]);
    Vf28(1, &gVoiceParamB[mType]);
    for (u32 i = 3; i < 22; i++) {
        SetName(i, 0, 0);
    }
    for (u32 i = 0; i < 10; i++) {
        SetName(i + 22, 0, 0);
    }
    for (u32 i = 0; i <= 9; i++) {
        SetName(0, 0, i);
        SetName(1, 0, i);
        SetName(2, 0, i);
    }
}

const char* VoiceTable::GetVoiceName(const char* suffix) {
    static NameBuf buf;
    const char* prefix[12] = {"01_", "02_", "03_", "04_", "05_", "06_", "07_", "08_", "09_", "10_", "11_", "12_"};
    buf.assign("SEQ_VOI_PC");
    buf.append(prefix[mType]);
    buf.append(suffix);
    return buf.c_str();
}

const char* VoiceTable::GetSeName(int type, int mode, int idx) {
    const char* surface[10] = {"", "_GRASS", "_IRON", "_EARTH", "_STONE", "_SAND", "_CLOUD", "_WOOD", "_ICE", "_TURF"};
    static NameBuf buf;
    bool woman = mode == 2 || mode == 3;
    buf.clear();
    buf.assign("SEQ_SECH_");
    switch (type) {
    case 0:
        buf.append("WALK");
        break;
    case 1:
        buf.append("RUN");
        break;
    default:
        buf.append("LANDING");
        break;
    }
    buf.append2(woman ? "_WOMEN" : "_MEN");
    buf.append(surface[idx]);
    return buf.c_str();
}

void VoiceTable::SetName(int type, const char* name, int idx) {
    if (type <= 2) {
        if (name == 0) {
            name = GetSeName(type, mType, idx);
        }
        switch (type) {
        case 0: {
            NameBuf* p = &mNamesA[idx];
            p->assign(name);
            break;
        }
        case 1: {
            NameBuf* p = &mNamesB[idx];
            p->assign(name);
            break;
        }
        default: {
            NameBuf* p = &mNamesC[idx];
            p->assign(name);
            break;
        }
        }
    } else {
        int n = type - 3;
        if (name == 0) {
            if (type < 22) {
                const char* seTable[19] = {
                    "SEQ_SECH_JUMP", "SEQ_SECH_JUMP_HIGH", "SEQ_SECH_HIPDROP", "SEQ_SECH_HIPDROP_LANDING",
                    "SEQ_SECH_TSUBUSARE_ON_HIPDROP", "SEQ_SECH_TSUBUSARE_ON_JUMP", "SEQ_SECH_TSUBUSARE_FUKKI",
                    "SEQ_SECH_PUNCH", "SEQ_SECH_KICK", "SEQ_SECH_DMG00", "SEQ_SECH_DMG00", "SEQ_SECH_DMG_BULLET",
                    "SEQ_SECH_BLOW01", "SEQ_SECH_BLOW01_RM", "SEQ_SECH_FALL00", "SEQ_SECH_FALL01",
                    "SEQ_SECH_TSUKAMI01", "SEQ_SECH_TSUKAMI01_RM", "SEQ_SECH_THROW"};
                const char* s = seTable[n];
                NameBuf* p = &mNames[n];
                p->assign(s);
            } else {
                const char* idTable[10] = {"001", "050", "007", "007", "004", "004", "005", "005", "002", "003"};
                const char* s = Vf7C(idTable[type - 22]);
                NameBuf* p = &mNames[n];
                p->assign(s);
            }
        } else {
            NameBuf* p = &mNames[n];
            p->assign(name);
        }
    }
}
