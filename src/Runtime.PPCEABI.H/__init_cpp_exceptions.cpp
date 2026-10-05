/*
 * Registers this module's exception tables with the C++ exception
 * handling runtime at startup, and unregisters them at shutdown.
 */

typedef struct __eti_init_info {
    void* eti_start;
    void* eti_end;
    void* code_start;
    unsigned long code_size;
} __eti_init_info;

extern "C" {
extern __eti_init_info _eti_init_info[];

int __register_fragment(struct __eti_init_info* info, char* TOC);
void __unregister_fragment(int fragmentID);
void __destroy_global_chain(void);

void __init_cpp_exceptions(void);
void __fini_cpp_exceptions(void);
}

static int fragmentID = -2;

extern void __init_cpp_exceptions(void) {
    if (fragmentID == -2) {
        register char* temp;
        asm { mr temp, r2 }
        fragmentID = __register_fragment(_eti_init_info, temp);
    }
}

extern void __fini_cpp_exceptions(void) {
    if (fragmentID != -2) {
        __unregister_fragment(fragmentID);
        fragmentID = -2;
    }
}

#pragma section ".ctors$10"
#pragma section ".dtors$10"
#pragma section ".dtors$15"

#pragma force_active on
__declspec(section ".ctors$10") extern void* const __init_cpp_exceptions_reference = __init_cpp_exceptions;
__declspec(section ".dtors$10") extern void* const __destroy_global_chain_reference = __destroy_global_chain;
__declspec(section ".dtors$15") extern void* const __fini_cpp_exceptions_reference = __fini_cpp_exceptions;
#pragma force_active reset
