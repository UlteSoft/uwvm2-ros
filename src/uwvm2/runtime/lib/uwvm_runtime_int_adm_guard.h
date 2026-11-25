#pragma once
#include <uwvm2/runtime/compiler/shared/interpreter_adm_guard.h>
#if UWVM2_HAS_INT_ADM_GUARD && defined(UWVM_RUNTIME_LLVM_JIT) && defined(UWVM_RUNTIME_UWVM_INTERPRETER)
// Each symbol starts on its sole potentially faulting instruction. The landing
// authenticates this exact PC and the fault address inside a live reservation.
#define UWVM2_ADM_LEAF(name,operation) \
    ".p2align 2\n.weak " #name "\n.hidden " #name "\n.type " #name ",%function\n" #name ":\n.cfi_startproc\n" \
    operation "\nret\n.cfi_endproc\n.size " #name ",.-" #name "\n"
__asm__(".pushsection .text.uwvm_adm_access,\"ax\",%progbits\n"
#if defined(__x86_64__)
    UWVM2_ADM_LEAF(uwvm_adm_load1,"movzbl (%rdi),%eax")
    UWVM2_ADM_LEAF(uwvm_adm_load2,"movzwl (%rdi),%eax")
    UWVM2_ADM_LEAF(uwvm_adm_load4,"movl (%rdi),%eax")
    UWVM2_ADM_LEAF(uwvm_adm_load8,"movq (%rdi),%rax")
    UWVM2_ADM_LEAF(uwvm_adm_store1,"movb %sil,(%rdi)")
    UWVM2_ADM_LEAF(uwvm_adm_store2,"movw %si,(%rdi)")
    UWVM2_ADM_LEAF(uwvm_adm_store4,"movl %esi,(%rdi)")
    UWVM2_ADM_LEAF(uwvm_adm_store8,"movq %rsi,(%rdi)")
#elif defined(__aarch64__)
    UWVM2_ADM_LEAF(uwvm_adm_load1,"ldrb w0,[x0]")
    UWVM2_ADM_LEAF(uwvm_adm_load2,"ldrh w0,[x0]")
    UWVM2_ADM_LEAF(uwvm_adm_load4,"ldr w0,[x0]")
    UWVM2_ADM_LEAF(uwvm_adm_load8,"ldr x0,[x0]")
    UWVM2_ADM_LEAF(uwvm_adm_store1,"strb w1,[x0]")
    UWVM2_ADM_LEAF(uwvm_adm_store2,"strh w1,[x0]")
    UWVM2_ADM_LEAF(uwvm_adm_store4,"str w1,[x0]")
    UWVM2_ADM_LEAF(uwvm_adm_store8,"str x1,[x0]")
#endif
    ".popsection\n");
#undef UWVM2_ADM_LEAF
#endif
