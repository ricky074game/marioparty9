#ifndef REVOLUTION_BASE_PPCARCH_H
#define REVOLUTION_BASE_PPCARCH_H

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* HID0 bits */
#define HID0_SPD 0x00000200 /* Speculative cache access disable */

/* HID4 bits */
#define HID4_H4A 0x80000000 /* HID4 access */

/* Special-purpose register numbers */
#define HID4 1011

u32 PPCMfmsr(void);
void PPCMtmsr(u32 newMSR);
u32 PPCMfhid0(void);
void PPCMthid0(u32 newHID0);
u32 PPCMfl2cr(void);
void PPCMtl2cr(u32 newL2cr);
void PPCMtdec(u32 newDec);
void PPCSync(void);
void PPCHalt(void);
void PPCMtmmcr0(u32 newMmcr0);
void PPCMtmmcr1(u32 newMmcr1);
void PPCMtpmc1(u32 newPmc1);
void PPCMtpmc2(u32 newPmc2);
void PPCMtpmc3(u32 newPmc3);
void PPCMtpmc4(u32 newPmc4);
u32 PPCMffpscr(void);
void PPCMtfpscr(u32 newFPSCR);
u32 PPCMfhid2(void);
void PPCMthid2(u32 newhid2);
void PPCMtwpar(u32 newwpar);
void PPCDisableSpeculation(void);
void PPCSetFpNonIEEEMode(void);
void PPCMthid4(register u32 newHID4);

#ifdef __cplusplus
}
#endif

#endif
