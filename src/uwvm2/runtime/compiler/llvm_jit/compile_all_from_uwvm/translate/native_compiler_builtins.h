/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)
 * Copyright (c) 2025-present UlteSoft. All rights reserved.
 * Licensed under the APL-2.0 License (see LICENSE file).
 *************************************************************/
#pragma once
#include <cstdint>
#include <string>

// These names are introduced by native LLVM lowering, not by Wasm imports.
// Static compiler runtimes do not put all of their helpers in the process
// dynamic symbol table. Take their real addresses here so the native linker
// retains them, and resolve them only in the owning JIT section loader.
//
// The aliases are ADDRESS-ONLY declarations, never C++ call targets. In
// particular ARM divmod returns multiple core registers, and ARM soft-float
// helpers use base AAPCS even in an armhf process. Calling C++ wrappers with
// the host ABI would corrupt their results. The LLVM backend emits the
// required ABI and calls the actual compiler-runtime implementation.
// ARM runtime ABI: https://github.com/ARM-software/abi-aa/blob/main/rtabi32/rtabi32.rst
#if defined(UWVM_RUNTIME_LLVM_JIT) && defined(__linux__) && (defined(__i386__) || defined(__arm__))
namespace uwvm2::runtime::compiler::llvm_jit::details::native_compiler_builtins
{
    extern "C" void address_divdi3() asm("__divdi3");
    extern "C" void address_udivdi3() asm("__udivdi3");
    extern "C" void address_moddi3() asm("__moddi3");
    extern "C" void address_umoddi3() asm("__umoddi3");
    extern "C" void address_muldi3() asm("__muldi3");
    extern "C" void address_ashldi3() asm("__ashldi3");
    extern "C" void address_ashrdi3() asm("__ashrdi3");
    extern "C" void address_lshrdi3() asm("__lshrdi3");
    extern "C" void address_floatdidf() asm("__floatdidf");
    extern "C" void address_floatundidf() asm("__floatundidf");
    extern "C" void address_floatdisf() asm("__floatdisf");
    extern "C" void address_floatundisf() asm("__floatundisf");
    extern "C" void address_fixdfdi() asm("__fixdfdi");
    extern "C" void address_fixunsdfdi() asm("__fixunsdfdi");
    extern "C" void address_fixsfdi() asm("__fixsfdi");
    extern "C" void address_fixunssfdi() asm("__fixunssfdi");
# if defined(__arm__)
    extern "C" void address_aeabi_l2d() asm("__aeabi_l2d");
    extern "C" void address_aeabi_ul2d() asm("__aeabi_ul2d");
    extern "C" void address_aeabi_l2f() asm("__aeabi_l2f");
    extern "C" void address_aeabi_ul2f() asm("__aeabi_ul2f");
    extern "C" void address_aeabi_d2lz() asm("__aeabi_d2lz");
    extern "C" void address_aeabi_d2ulz() asm("__aeabi_d2ulz");
    extern "C" void address_aeabi_f2lz() asm("__aeabi_f2lz");
    extern "C" void address_aeabi_f2ulz() asm("__aeabi_f2ulz");
    extern "C" void address_aeabi_lmul() asm("__aeabi_lmul");
    extern "C" void address_aeabi_llsl() asm("__aeabi_llsl");
    extern "C" void address_aeabi_llsr() asm("__aeabi_llsr");
    extern "C" void address_aeabi_lasr() asm("__aeabi_lasr");
    extern "C" void address_aeabi_lcmp() asm("__aeabi_lcmp");
    extern "C" void address_aeabi_ulcmp() asm("__aeabi_ulcmp");
    extern "C" void address_aeabi_ldivmod() asm("__aeabi_ldivmod");
    extern "C" void address_aeabi_uldivmod() asm("__aeabi_uldivmod");
    extern "C" void address_aeabi_idiv() asm("__aeabi_idiv");
    extern "C" void address_aeabi_uidiv() asm("__aeabi_uidiv");
    extern "C" void address_aeabi_idivmod() asm("__aeabi_idivmod");
    extern "C" void address_aeabi_uidivmod() asm("__aeabi_uidivmod");
    extern "C" void address_aeabi_dadd() asm("__aeabi_dadd");
    extern "C" void address_aeabi_dsub() asm("__aeabi_dsub");
    extern "C" void address_aeabi_dmul() asm("__aeabi_dmul");
    extern "C" void address_aeabi_ddiv() asm("__aeabi_ddiv");
    extern "C" void address_aeabi_dneg() asm("__aeabi_dneg");
    extern "C" void address_aeabi_dcmpeq() asm("__aeabi_dcmpeq");
    extern "C" void address_aeabi_dcmplt() asm("__aeabi_dcmplt");
    extern "C" void address_aeabi_dcmple() asm("__aeabi_dcmple");
    extern "C" void address_aeabi_dcmpge() asm("__aeabi_dcmpge");
    extern "C" void address_aeabi_dcmpgt() asm("__aeabi_dcmpgt");
    extern "C" void address_aeabi_dcmpun() asm("__aeabi_dcmpun");
    extern "C" void address_aeabi_d2iz() asm("__aeabi_d2iz");
    extern "C" void address_aeabi_d2uiz() asm("__aeabi_d2uiz");
    extern "C" void address_aeabi_fadd() asm("__aeabi_fadd");
    extern "C" void address_aeabi_fsub() asm("__aeabi_fsub");
    extern "C" void address_aeabi_fmul() asm("__aeabi_fmul");
    extern "C" void address_aeabi_fdiv() asm("__aeabi_fdiv");
    extern "C" void address_aeabi_fneg() asm("__aeabi_fneg");
    extern "C" void address_aeabi_fcmpeq() asm("__aeabi_fcmpeq");
    extern "C" void address_aeabi_fcmplt() asm("__aeabi_fcmplt");
    extern "C" void address_aeabi_fcmple() asm("__aeabi_fcmple");
    extern "C" void address_aeabi_fcmpge() asm("__aeabi_fcmpge");
    extern "C" void address_aeabi_fcmpgt() asm("__aeabi_fcmpgt");
    extern "C" void address_aeabi_fcmpun() asm("__aeabi_fcmpun");
    extern "C" void address_aeabi_f2iz() asm("__aeabi_f2iz");
    extern "C" void address_aeabi_f2uiz() asm("__aeabi_f2uiz");
    extern "C" void address_aeabi_i2d() asm("__aeabi_i2d");
    extern "C" void address_aeabi_ui2d() asm("__aeabi_ui2d");
    extern "C" void address_aeabi_i2f() asm("__aeabi_i2f");
    extern "C" void address_aeabi_ui2f() asm("__aeabi_ui2f");
    extern "C" void address_aeabi_d2f() asm("__aeabi_d2f");
    extern "C" void address_aeabi_f2d() asm("__aeabi_f2d");
# endif
    [[nodiscard]] inline ::std::uintptr_t find(::std::string const& name) noexcept
    {
        if(name == "__divdi3") { return reinterpret_cast<::std::uintptr_t>(&address_divdi3); }
        if(name == "__udivdi3") { return reinterpret_cast<::std::uintptr_t>(&address_udivdi3); }
        if(name == "__moddi3") { return reinterpret_cast<::std::uintptr_t>(&address_moddi3); }
        if(name == "__umoddi3") { return reinterpret_cast<::std::uintptr_t>(&address_umoddi3); }
        if(name == "__muldi3") { return reinterpret_cast<::std::uintptr_t>(&address_muldi3); }
        if(name == "__ashldi3") { return reinterpret_cast<::std::uintptr_t>(&address_ashldi3); }
        if(name == "__ashrdi3") { return reinterpret_cast<::std::uintptr_t>(&address_ashrdi3); }
        if(name == "__lshrdi3") { return reinterpret_cast<::std::uintptr_t>(&address_lshrdi3); }
        if(name == "__floatdidf") { return reinterpret_cast<::std::uintptr_t>(&address_floatdidf); }
        if(name == "__floatundidf") { return reinterpret_cast<::std::uintptr_t>(&address_floatundidf); }
        if(name == "__floatdisf") { return reinterpret_cast<::std::uintptr_t>(&address_floatdisf); }
        if(name == "__floatundisf") { return reinterpret_cast<::std::uintptr_t>(&address_floatundisf); }
        if(name == "__fixdfdi") { return reinterpret_cast<::std::uintptr_t>(&address_fixdfdi); }
        if(name == "__fixunsdfdi") { return reinterpret_cast<::std::uintptr_t>(&address_fixunsdfdi); }
        if(name == "__fixsfdi") { return reinterpret_cast<::std::uintptr_t>(&address_fixsfdi); }
        if(name == "__fixunssfdi") { return reinterpret_cast<::std::uintptr_t>(&address_fixunssfdi); }
# if defined(__arm__)
        if(name == "__aeabi_l2d") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_l2d); }
        if(name == "__aeabi_ul2d") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_ul2d); }
        if(name == "__aeabi_l2f") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_l2f); }
        if(name == "__aeabi_ul2f") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_ul2f); }
        if(name == "__aeabi_d2lz") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_d2lz); }
        if(name == "__aeabi_d2ulz") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_d2ulz); }
        if(name == "__aeabi_f2lz") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_f2lz); }
        if(name == "__aeabi_f2ulz") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_f2ulz); }
        if(name == "__aeabi_lmul") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_lmul); }
        if(name == "__aeabi_llsl") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_llsl); }
        if(name == "__aeabi_llsr") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_llsr); }
        if(name == "__aeabi_lasr") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_lasr); }
        if(name == "__aeabi_lcmp") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_lcmp); }
        if(name == "__aeabi_ulcmp") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_ulcmp); }
        if(name == "__aeabi_ldivmod") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_ldivmod); }
        if(name == "__aeabi_uldivmod") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_uldivmod); }
        if(name == "__aeabi_idiv") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_idiv); }
        if(name == "__aeabi_uidiv") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_uidiv); }
        if(name == "__aeabi_idivmod") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_idivmod); }
        if(name == "__aeabi_uidivmod") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_uidivmod); }
        if(name == "__aeabi_dadd") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_dadd); }
        if(name == "__aeabi_dsub") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_dsub); }
        if(name == "__aeabi_dmul") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_dmul); }
        if(name == "__aeabi_ddiv") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_ddiv); }
        if(name == "__aeabi_dneg") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_dneg); }
        if(name == "__aeabi_dcmpeq") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_dcmpeq); }
        if(name == "__aeabi_dcmplt") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_dcmplt); }
        if(name == "__aeabi_dcmple") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_dcmple); }
        if(name == "__aeabi_dcmpge") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_dcmpge); }
        if(name == "__aeabi_dcmpgt") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_dcmpgt); }
        if(name == "__aeabi_dcmpun") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_dcmpun); }
        if(name == "__aeabi_d2iz") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_d2iz); }
        if(name == "__aeabi_d2uiz") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_d2uiz); }
        if(name == "__aeabi_fadd") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_fadd); }
        if(name == "__aeabi_fsub") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_fsub); }
        if(name == "__aeabi_fmul") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_fmul); }
        if(name == "__aeabi_fdiv") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_fdiv); }
        if(name == "__aeabi_fneg") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_fneg); }
        if(name == "__aeabi_fcmpeq") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_fcmpeq); }
        if(name == "__aeabi_fcmplt") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_fcmplt); }
        if(name == "__aeabi_fcmple") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_fcmple); }
        if(name == "__aeabi_fcmpge") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_fcmpge); }
        if(name == "__aeabi_fcmpgt") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_fcmpgt); }
        if(name == "__aeabi_fcmpun") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_fcmpun); }
        if(name == "__aeabi_f2iz") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_f2iz); }
        if(name == "__aeabi_f2uiz") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_f2uiz); }
        if(name == "__aeabi_i2d") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_i2d); }
        if(name == "__aeabi_ui2d") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_ui2d); }
        if(name == "__aeabi_i2f") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_i2f); }
        if(name == "__aeabi_ui2f") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_ui2f); }
        if(name == "__aeabi_d2f") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_d2f); }
        if(name == "__aeabi_f2d") { return reinterpret_cast<::std::uintptr_t>(&address_aeabi_f2d); }
# endif
        return 0u;
    }
}
#endif
