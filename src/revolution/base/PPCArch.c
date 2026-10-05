#include <revolution/base/PPCArch.h>

void OSReport(const char* msg, ...);

typedef union FpscrUnion {
    f64 f;
    struct {
        u32 fpscr_pad;
        u32 fpscr;
    } u;
} FpscrUnion;

asm u32 PPCMfmsr(void) {
    nofralloc
    mfmsr r3
    blr
}

asm void PPCMtmsr(register u32 newMSR) {
    nofralloc
    mtmsr newMSR
    blr
}

asm u32 PPCMfhid0(void) {
    nofralloc
    mfspr r3, HID0
    blr
}

asm void PPCMthid0(register u32 newHID0) {
    nofralloc
    mtspr HID0, newHID0
    blr
}

asm u32 PPCMfl2cr(void) {
    nofralloc
    mfspr r3, L2CR
    blr
}

asm void PPCMtl2cr(register u32 newL2cr) {
    nofralloc
    mtspr L2CR, newL2cr
    blr
}

__declspec(weak) asm void PPCMtdec(register u32 newDec) {
    nofralloc
    mtdec newDec
    blr
}

asm void PPCSync(void) {
    nofralloc
    sc
    blr
}

__declspec(weak) asm void PPCHalt(void) {
    nofralloc
    sync
loop:
    nop
    li r3, 0
    nop
    b loop
}

asm void PPCMtmmcr0(register u32 newMmcr0) {
    nofralloc
    mtspr MMCR0, newMmcr0
    blr
}

asm void PPCMtmmcr1(register u32 newMmcr1) {
    nofralloc
    mtspr MMCR1, newMmcr1
    blr
}

asm void PPCMtpmc1(register u32 newPmc1) {
    nofralloc
    mtspr PMC1, newPmc1
    blr
}

asm void PPCMtpmc2(register u32 newPmc2) {
    nofralloc
    mtspr PMC2, newPmc2
    blr
}

asm void PPCMtpmc3(register u32 newPmc3) {
    nofralloc
    mtspr PMC3, newPmc3
    blr
}

asm void PPCMtpmc4(register u32 newPmc4) {
    nofralloc
    mtspr PMC4, newPmc4
    blr
}

u32 PPCMffpscr(void) {
    FpscrUnion m;

    asm {
        mffs f31
        stfd f31, m.f
    }

    return m.u.fpscr;
}

void PPCMtfpscr(register u32 newFPSCR) {
    FpscrUnion m;

    asm {
        li r4, 0
        stw r4, m.u.fpscr_pad
        stw newFPSCR, m.u.fpscr
        lfd f31, m.f
        mtfsf 0xff, f31
    }
}

/* MWCC's "HID2" name is the Broadway SPR 1011, so use Gekko's HID2 (920). */
asm u32 PPCMfhid2(void) {
    nofralloc
    mfspr r3, 920
    blr
}

asm void PPCMthid2(register u32 newhid2) {
    nofralloc
    mtspr 920, newhid2
    blr
}

asm void PPCMtwpar(register u32 newwpar) {
    nofralloc
    mtspr WPAR, newwpar
    blr
}

void PPCDisableSpeculation(void) {
    PPCMthid0(PPCMfhid0() | HID0_SPD);
}

asm void PPCSetFpNonIEEEMode(void) {
    nofralloc
    mtfsb1 29
    blr
}

void PPCMthid4(register u32 newHID4) {
    if (newHID4 & HID4_H4A) {
        asm { mtspr HID4, newHID4 }
    } else {
        OSReport("H4A should not be cleared because of Broadway errata.\n");
        newHID4 |= HID4_H4A;
        asm { mtspr HID4, newHID4 }
    }
}
