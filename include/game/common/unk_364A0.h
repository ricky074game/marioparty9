#ifndef GAME_COMMON_UNK_364A0_H
#define GAME_COMMON_UNK_364A0_H

#include "types.h"

namespace unk_364A0 {

struct Vec {
    f32 x, y, z;
    Vec() {}
    Vec(f32 a, f32 b, f32 c) : x(a), y(b), z(c) {}
};

/* Something an actor can collide with: vtable slots 0x1C / 0x20 are the ray tests. */
class Collider {
public:
    virtual void v008();
    virtual void v00C();
    virtual void v010();
    virtual void v014();
    virtual void v018();
    virtual int rayTestUp(Vec* from, Vec* to, Vec* out, int arg, void* info);
    virtual int rayTestSide(f32 radius, Vec* from, Vec* to, Vec* out, int arg, void* info);
};

struct HitEntry {
    Collider* obj;
    int arg;
};

struct HitVec {
    HitEntry* mData;
    u32 mSize;
    u32 mCap;
};

class Actor {
public:
    virtual void v008();
    virtual void v00C();
    virtual void v010();
    virtual void v014();
    virtual void v018();
    virtual void v01C();
    virtual void v020();
    virtual void v024();
    virtual void v028();
    virtual void v02C();
    virtual void v030();
    virtual void v034();
    virtual void v038();
    virtual void v03C();
    virtual void v040();
    virtual void v044();
    virtual void v048();
    virtual void v04C();
    virtual void v050();
    virtual void v054();
    virtual void v058();
    virtual void v05C();
    virtual void v060();
    virtual void v064();
    virtual void v068();
    virtual void v06C();
    virtual void v070();
    virtual void v074();
    virtual void v078();
    virtual void v07C();
    virtual void v080();
    virtual void v084();
    virtual void v088();
    virtual void v08C();
    virtual void v090();
    virtual void v094();
    virtual void v098();
    virtual void v09C();
    virtual void v0A0();
    virtual void v0A4();
    virtual void v0A8();
    virtual void v0AC();
    virtual void v0B0();
    virtual void v0B4();
    virtual void v0B8();
    virtual void v0BC();
    virtual void v0C0();
    virtual void v0C4();
    virtual void v0C8();
    virtual void v0CC();
    virtual void v0D0();
    virtual void v0D4();
    virtual void v0D8();
    virtual void v0DC();
    virtual void v0E0();
    virtual void v0E4();
    virtual void v0E8();
    virtual void v0EC();
    virtual void v0F0();
    virtual void v0F4();
    virtual void v0F8();
    virtual void v0FC();
    virtual void v100();
    virtual void v104();
    virtual void v108();
    virtual void v10C();
    virtual void v110();
    virtual void v114();
    virtual void v118();
    virtual void v11C();
    virtual void v120();
    virtual void v124();
    virtual void v128();
    virtual void v12C();
    virtual void v130();
    virtual void v134();
    virtual void v138();
    virtual void v13C();
    virtual void v140();
    virtual void v144();
    virtual void v148();
    virtual void v14C();
    virtual void v150();
    virtual void v154();
    virtual void v158();
    virtual void v15C();
    virtual void v160();
    virtual void v164();
    virtual void v168();
    virtual void v16C();
    virtual void v170();
    virtual void v174();
    virtual void v178();
    virtual void v17C();
    virtual void v180();
    virtual void v184();
    virtual void v188();
    virtual void v18C();
    virtual void v190();
    virtual void v194();
    virtual void v198();
    virtual void v19C();
    virtual void v1A0();
    virtual void v1A4();
    virtual void v1A8();
    virtual void v1AC();
    virtual void v1B0();
    virtual void v1B4();
    virtual void v1B8();
    virtual void v1BC();
    virtual void v1C0();
    virtual void v1C4();
    virtual void v1C8();
    virtual void v1CC();
    virtual void v1D0();
    virtual void v1D4();
    virtual void v1D8();
    virtual void v1DC();
    virtual void v1E0();
    virtual void v1E4();
    virtual void v1E8();
    virtual void v1EC();
    virtual void v1F0();
    virtual void v1F4();
    virtual void v1F8();
    virtual void v1FC();
    virtual void v200();
    virtual void v204();
    virtual void v208();
    virtual void v20C();
    virtual void v210();
    virtual void v214();
    virtual void v218();
    virtual void v21C();
    virtual void v220();
    virtual void v224();
    virtual void v228();
    virtual void v22C();
    virtual void v230();
    virtual void v234();
    virtual void v238();
    virtual void v23C();
    virtual void v240();
    virtual void v244();
    virtual void v248();
    virtual void v24C();
    virtual void v250();
    virtual void v254();
    virtual void v258();
    virtual void v25C();
    virtual void v260();
    virtual void v264();
    virtual void v268();
    virtual void v26C();
    virtual void v270();
    virtual void v274();
    virtual void v278();
    virtual void v27C();
    virtual void v280();
    virtual void v284();
    virtual void v288();
    virtual void v28C();
    virtual int v290();
    virtual void v294();
    virtual void v298();
    virtual void v29C();
    virtual void v2A0();
    virtual void v2A4();
    virtual void v2A8();
    virtual void v2AC();
    virtual void v2B0();
    virtual void v2B4();
    virtual void v2B8();
    virtual void v2BC();
    virtual Vec* v2C0();
    virtual int v2C4();

    void setBounds(u8 on, const Vec* a, const Vec* b);
    void setCircle(u8 on, const Vec* center, f32 radius, f32 yMin, f32 yMax);
    void setFlag7B0(u8 v);
    void begin();
    int addHit(Collider* c, int arg);
    void clearHit();
    u8 probeFloor();
    u8 probeSide();
    void clampMove();
    void setHit(u8 v);

    /* 0x004 */ u8 pad004[0x68 - 4];
    /* 0x068 */ u8 node[4];
    /* 0x06C */ u8 pad06C[0x620 - 0x6C];
    /* 0x620 */ Vec vel;
    /* 0x62C */ u8 pad62C[0x6A0 - 0x62C];
    /* 0x6A0 */ f32 m6A0;
    /* 0x6A4 */ u8 pad6A4[0x780 - 0x6A4];
    /* 0x780 */ u8 m780;
    /* 0x781 */ u8 m781;
    /* 0x782 */ u8 pad782[2];
    /* 0x784 */ Vec m784;
    /* 0x790 */ Vec lo;
    /* 0x79C */ Vec hi;
    /* 0x7A8 */ f32 m7A8;
    /* 0x7AC */ u8 pad7AC[4];
    /* 0x7B0 */ u8 m7B0;
    /* 0x7B1 */ u8 m7B1;
    /* 0x7B2 */ u8 pad7B2[2];
    /* 0x7B4 */ HitVec hits;
    /* 0x7C0 */ u8 m7C0;
    /* 0x7C1 */ u8 pad7C1[3];
    /* 0x7C4 */ s32 m7C4;
    /* 0x7C8 */ Vec m7C8;
    /* 0x7D4 */ u8 m7D4;
    /* 0x7D5 */ u8 pad7D5[3];
    /* 0x7D8 */ u8 info7D8[0x848 - 0x7D8];
    /* 0x848 */ f32 m848;
    /* 0x84C */ f32 m84C;
    /* 0x850 */ u8 m850;
    /* 0x851 */ u8 pad851[3];
    /* 0x854 */ s32 m854;
    /* 0x858 */ Vec m858;
    /* 0x864 */ u8 info864[0x8D4 - 0x864];
    /* 0x8D4 */ Vec m8D4;
    /* 0x8E0 */ f32 m8E0;
};

} // namespace unk_364A0

#endif
