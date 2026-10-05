#include <revolution/base/PPCArch.h>
#include <revolution/os/OSCache.h>

/* HID0 bits */
#define HID0_ICE 0x00008000
#define HID0_DCE 0x00004000

/* HID2 error bits */
#define HID2_DCHERR 0x00800000
#define HID2_DNCERR 0x00400000
#define HID2_DCMERR 0x00200000
#define HID2_DQOERR 0x00100000

/* L2CR bits */
#define L2CR_L2E 0x80000000
#define L2CR_L2I 0x00200000
#define L2CR_L2IP 0x00000001

#define MSR_IR 0x00000020
#define MSR_DR 0x00000010

#define SRR1_DMA_BIT 0x00200000

#define OS_ERROR_MACHINE_CHECK 1

/* Gekko SPRs that MWCC doesn't name the way the hardware does. */
#define SPR_HID2 920
#define SPR_DMA_U 922
#define SPR_DMA_L 923
#define SPR_DBAT3U 542
#define SPR_DBAT3L 543

#define LC_BASE_PREFIX 0xE000

typedef struct OSContext {
    u8 _unk00[0x19C];
    u32 srr1;
} OSContext;

typedef u16 OSError;
typedef void (*OSErrorHandler)(OSError error, OSContext* context, ...);

BOOL OSDisableInterrupts(void);
BOOL OSRestoreInterrupts(BOOL level);
void OSReport(const char* msg, ...);
void OSDumpContext(OSContext* context);
OSErrorHandler OSSetErrorHandler(OSError error, OSErrorHandler handler);

asm void DCEnable(void) {
    nofralloc
    sync
    mfspr r3, HID0
    ori r3, r3, HID0_DCE
    mtspr HID0, r3
    blr
}

asm void DCInvalidateRange(register void* addr, register u32 nBytes) {
    nofralloc
    cmplwi nBytes, 0
    blelr
    clrlwi r5, addr, 27
    add nBytes, nBytes, r5
    addi nBytes, nBytes, 31
    srwi nBytes, nBytes, 5
    mtctr nBytes
loop:
    dcbi r0, addr
    addi addr, addr, 32
    bdnz loop
    blr
}

asm void DCFlushRange(register void* addr, register u32 nBytes) {
    nofralloc
    cmplwi nBytes, 0
    blelr
    clrlwi r5, addr, 27
    add nBytes, nBytes, r5
    addi nBytes, nBytes, 31
    srwi nBytes, nBytes, 5
    mtctr nBytes
loop:
    dcbf r0, addr
    addi addr, addr, 32
    bdnz loop
    sc
    blr
}

asm void DCStoreRange(register void* addr, register u32 nBytes) {
    nofralloc
    cmplwi nBytes, 0
    blelr
    clrlwi r5, addr, 27
    add nBytes, nBytes, r5
    addi nBytes, nBytes, 31
    srwi nBytes, nBytes, 5
    mtctr nBytes
loop:
    dcbst r0, addr
    addi addr, addr, 32
    bdnz loop
    sc
    blr
}

asm void DCFlushRangeNoSync(register void* addr, register u32 nBytes) {
    nofralloc
    cmplwi nBytes, 0
    blelr
    clrlwi r5, addr, 27
    add nBytes, nBytes, r5
    addi nBytes, nBytes, 31
    srwi nBytes, nBytes, 5
    mtctr nBytes
loop:
    dcbf r0, addr
    addi addr, addr, 32
    bdnz loop
    blr
}

asm void DCStoreRangeNoSync(register void* addr, register u32 nBytes) {
    nofralloc
    cmplwi nBytes, 0
    blelr
    clrlwi r5, addr, 27
    add nBytes, nBytes, r5
    addi nBytes, nBytes, 31
    srwi nBytes, nBytes, 5
    mtctr nBytes
loop:
    dcbst r0, addr
    addi addr, addr, 32
    bdnz loop
    blr
}

asm void DCZeroRange(register void* addr, register u32 nBytes) {
    nofralloc
    cmplwi nBytes, 0
    blelr
    clrlwi r5, addr, 27
    add nBytes, nBytes, r5
    addi nBytes, nBytes, 31
    srwi nBytes, nBytes, 5
    mtctr nBytes
loop:
    dcbz r0, addr
    addi addr, addr, 32
    bdnz loop
    blr
}

asm void ICInvalidateRange(register void* addr, register u32 nBytes) {
    nofralloc
    cmplwi nBytes, 0
    blelr
    clrlwi r5, addr, 27
    add nBytes, nBytes, r5
    addi nBytes, nBytes, 31
    srwi nBytes, nBytes, 5
    mtctr nBytes
loop:
    icbi r0, addr
    addi addr, addr, 32
    bdnz loop
    sync
    isync
    blr
}

asm void ICFlashInvalidate(void) {
    nofralloc
    mfspr r3, HID0
    ori r3, r3, 0x0800
    mtspr HID0, r3
    blr
}

asm void ICEnable(void) {
    nofralloc
    isync
    mfspr r3, HID0
    ori r3, r3, HID0_ICE
    mtspr HID0, r3
    blr
}

static asm void __LCEnable(void) {
    nofralloc
    mfmsr r5
    ori r5, r5, 0x1000
    mtmsr r5

    lis r3, 0x8000
    li r4, 0x400
    mtctr r4
loop1:
    dcbt r0, r3
    dcbst r0, r3
    addi r3, r3, 32
    bdnz loop1

    mfspr r4, SPR_HID2
    oris r4, r4, 0x100F
    mtspr SPR_HID2, r4

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    lis r3, LC_BASE_PREFIX
    ori r3, r3, 0x0002
    mtspr SPR_DBAT3L, r3
    ori r3, r3, 0x01FE
    mtspr SPR_DBAT3U, r3
    isync

    lis r3, LC_BASE_PREFIX
    li r6, 0x200
    mtctr r6
    li r6, 0
loop2:
    dcbz_l r6, r3
    addi r3, r3, 32
    bdnz loop2

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    blr
}

void LCEnable(void) {
    BOOL enabled = OSDisableInterrupts();
    __LCEnable();
    OSRestoreInterrupts(enabled);
}

asm void LCDisable(void) {
    nofralloc
    lis r3, LC_BASE_PREFIX
    li r4, 0x200
    mtctr r4
loop:
    dcbi r0, r3
    addi r3, r3, 32
    bdnz loop
    mfspr r4, SPR_HID2
    rlwinm r4, r4, 0, 4, 2
    mtspr SPR_HID2, r4
    blr
}

asm void LCLoadBlocks(register void* destTag, register void* srcAddr, register u32 numBlocks) {
    nofralloc
    rlwinm r6, numBlocks, 30, 27, 31
    rlwinm srcAddr, srcAddr, 0, 3, 31
    or r6, r6, srcAddr
    mtspr SPR_DMA_U, r6
    rlwinm r6, numBlocks, 2, 28, 29
    or r6, r6, destTag
    ori r6, r6, 0x12
    mtspr SPR_DMA_L, r6
    blr
}

asm void LCStoreBlocks(register void* destAddr, register void* srcTag, register u32 numBlocks) {
    nofralloc
    rlwinm r6, numBlocks, 30, 27, 31
    rlwinm destAddr, destAddr, 0, 3, 31
    or r6, r6, destAddr
    mtspr SPR_DMA_U, r6
    rlwinm r6, numBlocks, 2, 28, 29
    or r6, r6, srcTag
    ori r6, r6, 0x2
    mtspr SPR_DMA_L, r6
    blr
}

u32 LCStoreData(void* destAddr, void* srcAddr, u32 nBytes) {
    u32 numBlocks = (nBytes + 31) / 32;
    u32 numTransactions = (numBlocks + 128 - 1) / 128;

    while (numBlocks != 0) {
        if (numBlocks < 128) {
            LCStoreBlocks(destAddr, srcAddr, numBlocks);
            numBlocks = 0;
        } else {
            LCStoreBlocks(destAddr, srcAddr, 0);
            numBlocks -= 128;
            destAddr = (void*)((u32)destAddr + 4096);
            srcAddr = (void*)((u32)srcAddr + 4096);
        }
    }

    return numTransactions;
}

asm u32 LCQueueLength(void) {
    nofralloc
    mfspr r4, SPR_HID2
    rlwinm r3, r4, 8, 28, 31
    blr
}

asm void LCQueueWait(register u32 len) {
    nofralloc
loop:
    mfspr r4, SPR_HID2
    rlwinm r4, r4, 8, 28, 31
    cmpw r4, len
    bgt loop
    blr
}

static void DMAErrorHandler(OSError error, OSContext* context, ...) {
    u32 hid2 = PPCMfhid2();

    OSReport("Machine check received\n");
    OSReport("HID2 = 0x%x   SRR1 = 0x%x\n", hid2, context->srr1);
    if (!(hid2 & (HID2_DCHERR | HID2_DNCERR | HID2_DCMERR | HID2_DQOERR)) ||
        !(context->srr1 & SRR1_DMA_BIT)) {
        OSReport("Machine check was not DMA/locked cache related\n");
        OSDumpContext(context);
        PPCHalt();
    }

    OSReport("DMAErrorHandler(): An error occurred while processing DMA.\n");
    OSReport("The following errors have been detected and cleared :\n");

    if (hid2 & HID2_DCHERR) {
        OSReport("\t- Requested a locked cache tag that was already in the cache\n");
    }
    if (hid2 & HID2_DNCERR) {
        OSReport("\t- DMA attempted to access normal cache\n");
    }
    if (hid2 & HID2_DCMERR) {
        OSReport("\t- DMA missed in data cache\n");
    }
    if (hid2 & HID2_DQOERR) {
        OSReport("\t- DMA queue overflowed\n");
    }

    /* Writing HID2 back clears the error bits. */
    PPCMthid2(hid2);
}

static void L2Disable(void) {
    __sync();
    PPCMtl2cr(PPCMfl2cr() & ~L2CR_L2E);
    __sync();
}

static void L2GlobalInvalidate(void) {
    L2Disable();
    PPCMtl2cr(PPCMfl2cr() | L2CR_L2I);
    while (PPCMfl2cr() & L2CR_L2IP) {
    }
    PPCMtl2cr(PPCMfl2cr() & ~L2CR_L2I);
    while (PPCMfl2cr() & L2CR_L2IP) {
    }
}

static void L2Init(void) {
    u32 oldMSR = PPCMfmsr();
    __sync();
    PPCMtmsr(MSR_IR | MSR_DR);
    __sync();
    L2Disable();
    L2GlobalInvalidate();
    PPCMtmsr(oldMSR);
}

static void L2Enable(void) {
    PPCMtl2cr((PPCMfl2cr() | L2CR_L2E) & ~L2CR_L2I);
}

void __OSCacheInit(void) {
    if (!(PPCMfhid0() & HID0_ICE)) {
        ICEnable();
    }

    if (!(PPCMfhid0() & HID0_DCE)) {
        DCEnable();
    }

    if (!(PPCMfl2cr() & L2CR_L2E)) {
        L2Init();
        L2Enable();
    }

    OSSetErrorHandler(OS_ERROR_MACHINE_CHECK, DMAErrorHandler);
}
