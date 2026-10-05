#include <revolution/os/OSCache.h>

void* memcpy(void* dst, const void* src, unsigned long n);

void __OSSystemCallVectorStart(void);
void __OSSystemCallVectorEnd(void);

#define OSPhysicalToCached(paddr) ((void*)((u32)(paddr) + 0x80000000))

/* Copied to the system call exception vector (0xC00) by __OSInitSystemCall. */
static asm void SystemCallVector(void) {
    nofralloc
    entry __OSSystemCallVectorStart
    mfspr r9, HID0
    ori r10, r9, 0x8
    mtspr HID0, r10
    isync
    sync
    mtspr HID0, r9
    rfi
    entry __OSSystemCallVectorEnd
    nop
}

void __OSInitSystemCall(void) {
    void* addr = OSPhysicalToCached(0xC00);

    memcpy(addr, __OSSystemCallVectorStart, (u32)__OSSystemCallVectorEnd - (u32)__OSSystemCallVectorStart);
    DCFlushRangeNoSync(addr, 0x100);
    __sync();
    ICInvalidateRange(addr, 0x100);
}
