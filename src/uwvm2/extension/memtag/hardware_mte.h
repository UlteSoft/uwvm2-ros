/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)           *
 * Copyright (c) 2025-present UlteSoft. All rights reserved.   *
 * Licensed under the APL-2.0 License (see LICENSE file).      *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <cstddef>
# include <cstdint>
# include <limits>
# include "execution_policy.h"
# if defined(__linux__) && defined(__aarch64__)
#  include <cerrno>
#  include <csignal>
#  include <pthread.h>
#  include <sys/auxv.h>
#  include <sys/mman.h>
#  include <sys/prctl.h>
# endif
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag::hardware_mte
{
    enum class setup_error : unsigned
    {
        none, unsupported, invalid_configuration, thread_control_failed,
        thread_mode_not_synchronous, protection_failed, thread_mode_mismatch,
        thread_signal_mask_query_failed, thread_fault_signal_blocked
    };
    struct setup_result
    {
        setup_error error{};
        int system_error{};
        [[nodiscard]] constexpr explicit operator bool() const noexcept { return error == setup_error::none; }
    };
    // Linux UAPI values are stable even when compiling against older sysroots.
    inline constexpr int protection_flag{0x20};
    struct capabilities
    {
        unsigned minimum_architectural_version{};
        bool checked_mappings{}, asymmetric_faults{}, tagged_fault_address{}, store_only_checking{};
    };
    // Linux exposes OS-usable capabilities, not a CPU marketing name. A
    // FEAT_MTE-only instruction set is insufficient for checked allocations.
    // MTE4 subfeatures are independent facts; do not infer all of MTE4 merely
    // from one HWCAP3 bit or infer availability from LLVM's +mte feature alone.
    [[nodiscard]] constexpr capabilities decode_capabilities(unsigned long hwcap2,unsigned long hwcap3) noexcept
    {
        capabilities result{};
        result.checked_mappings=(hwcap2&(1ul<<18u))!=0u;
        if(!result.checked_mappings) { return result; }
        result.asymmetric_faults=(hwcap2&(1ul<<22u))!=0u;
        result.minimum_architectural_version=result.asymmetric_faults?3u:2u;
        result.tagged_fault_address=(hwcap3&1ul)!=0u;
        result.store_only_checking=(hwcap3&2ul)!=0u;
        // These advertised features imply the relevant MTE4 functionality;
        // canonical tags/OS allocator integration remain separate capabilities.
        if(result.tagged_fault_address || result.store_only_checking) { result.minimum_architectural_version=4u; }
        return result;
    }
    [[nodiscard]] inline capabilities query_capabilities() noexcept
    {
#if defined(__linux__) && defined(__aarch64__)
        return decode_capabilities(::getauxval(26 /* AT_HWCAP2 */),::getauxval(29 /* AT_HWCAP3 */));
#else
        // Darwin EMTE uses OS allocator/entitlement integration. It does not
        // expose Linux PROT_MTE/prctl to our reserved linear-memory backend.
        return {};
#endif
    }
    [[nodiscard]] inline bool supported() noexcept
    {
#if defined(__linux__) && defined(__aarch64__)
        return query_capabilities().checked_mappings;
#else
        return false;
#endif
    }
    // Request exactly one mode, then verify the kernel's accepted control.
    // Neither a target triple nor a successful IR build proves OS activation.
    [[nodiscard]] inline setup_result enable_current_thread(bool asynchronous) noexcept
    {
#if defined(__linux__) && defined(__aarch64__)
        if(!supported()) { return {setup_error::unsupported}; }
        // MTE delivery belongs to this actual native execution thread. A
        // blocked SIGSEGV cannot satisfy our controlled fault boundary: sync
        // faults may take the kernel's default fatal action, while async
        // delivery cannot be relied on before the next host side effect.
        // Validate on EVERY entry, before changing prctl/TCO. Do not silently
        // change an embedder's signal mask or the process-wide disposition.
        ::sigset_t blocked{};
        auto const mask_error{::pthread_sigmask(SIG_BLOCK,nullptr,&blocked)};
        if(mask_error!=0) { return {setup_error::thread_signal_mask_query_failed,mask_error}; }
        auto const member{::sigismember(&blocked,SIGSEGV)};
        if(member<0) { return {setup_error::thread_signal_mask_query_failed,errno}; }
        if(member!=0) { return {setup_error::thread_fault_signal_blocked}; }
        unsigned long const requested{asynchronous?4ul:2ul};
        unsigned long const flags{1ul | requested | (0xfffeul << 3u)};
        if(::prctl(55 /* PR_SET_TAGGED_ADDR_CTRL */, flags, 0ul, 0ul, 0ul) != 0)
        { return {setup_error::thread_control_failed, errno}; }
        auto const actual{::prctl(56 /* PR_GET_TAGGED_ADDR_CTRL */, 0ul, 0ul, 0ul, 0ul)};
        if(actual < 0) { return {setup_error::thread_control_failed, errno}; }
        if((static_cast<unsigned long>(actual)&6ul)!=requested || (actual&1)!=1)
        { return {asynchronous?setup_error::thread_mode_mismatch:setup_error::thread_mode_not_synchronous}; }
        // A host entry must not inherit a temporarily disabled PSTATE.TCO.
        asm volatile(".arch_extension memtag\nmsr tco, #0" ::: "memory");
        return {};
#else
        static_cast<void>(asynchronous);
        return {setup_error::unsupported};
#endif
    }
    [[nodiscard]] inline setup_result enable_current_thread_sync() noexcept
    { return enable_current_thread(false); }
    [[nodiscard]] inline setup_result enable_current_thread_selected() noexcept
    { return enable_current_thread(selected_fault_delivery.load(::std::memory_order_relaxed)==fault_delivery::hardware_async); }
    [[nodiscard]] inline bool select_for_owner() noexcept
    { return hardware_checked_access_requested() && supported(); }
#if defined(__linux__) && defined(__aarch64__)
    // Signal-time ownership is a plain per-thread sig_atomic_t, never a heap
    // pointer or a Guest value. ASYNC has no address with which to find an owner.
    inline thread_local volatile ::std::sig_atomic_t asynchronous_execution_armed{};
#endif
    inline void arm_asynchronous_execution() noexcept
    {
#if defined(__linux__) && defined(__aarch64__)
        if(!logging_requested() && selected_fault_delivery.load(::std::memory_order_relaxed)==fault_delivery::hardware_async)
        { asynchronous_execution_armed=1; }
#endif
    }
    inline void disarm_asynchronous_execution() noexcept
    {
#if defined(__linux__) && defined(__aarch64__)
        asynchronous_execution_armed=0;
#endif
    }
    inline void synchronize_asynchronous_faults() noexcept
    {
#if defined(__linux__) && defined(__aarch64__)
        if(!asynchronous_execution_armed) { return; }
        // DSB without ST/LD completes earlier asynchronous tag-fault register
        // updates. A real EL0->EL1 transition makes Linux collect/deliver them
        // before the next host effect; do not rely on a libc cached getpid.
        register unsigned long number asm("x8"){172ul}; // __NR_getpid (AArch64)
        register unsigned long result asm("x0");
        asm volatile("dsb sy\nsvc #0" : "=r"(result) : "r"(number) : "memory","cc");
        (void)result;
#endif
    }
    // The MTE logical tag occupies bits 59:56. The complete data mapping
    // must fit below that byte; this is architectural representability, not a
    // guest quota or an alignment hint supplied by a module.
    [[nodiscard]] constexpr bool canonical_native_span(::std::uint64_t base,::std::uint64_t bytes) noexcept
    { constexpr ::std::uint64_t end{::std::uint64_t{1u}<<56u};return base<end && bytes<=end-base; }
    [[nodiscard]] inline setup_result protect_mapping(void* base, ::std::size_t bytes,
                                                       ::std::size_t page_size) noexcept
    {
#if defined(__linux__) && defined(__aarch64__)
        if(!supported()) { return {setup_error::unsupported}; }
        if(base == nullptr || page_size == 0u || (page_size & (page_size - 1u)) != 0u ||
           (reinterpret_cast<::std::uintptr_t>(base) & (page_size - 1u)) != 0u ||
           bytes > ::std::numeric_limits<::std::size_t>::max() - (page_size - 1u))
        { return {setup_error::invalid_configuration}; }
        auto const rounded{(bytes + page_size - 1u) & ~(page_size - 1u)};
        if(!canonical_native_span(reinterpret_cast<::std::uintptr_t>(base),rounded))
        { return {setup_error::invalid_configuration}; }
        if(rounded != 0u && ::mprotect(base, rounded, PROT_READ | PROT_WRITE | protection_flag) != 0)
        { return {setup_error::protection_failed, errno}; }
        return {};
#else
        static_cast<void>(base); static_cast<void>(bytes); static_cast<void>(page_size);
        return {setup_error::unsupported};
#endif
    }
    [[nodiscard]] inline ::std::byte* tagged_pointer(::std::byte* address, ::std::uint8_t tag) noexcept
    {
#if defined(__linux__) && defined(__aarch64__)
        auto const raw{reinterpret_cast<::std::uintptr_t>(address)};
        return reinterpret_cast<::std::byte*>((raw & 0x00ffffffffffffffull) |
            (static_cast<::std::uintptr_t>(tag & 15u) << 56u));
#else
        static_cast<void>(tag); return address;
#endif
    }
    // Caller proves committed PROT_MTE memory and 16-byte alignment first.
    inline void store_tag(::std::byte* granule, ::std::uint8_t tag) noexcept
    {
#if defined(__linux__) && defined(__aarch64__)
        auto* const pointer{tagged_pointer(granule, tag)};
        asm volatile(".arch_extension memtag\nstg %0, [%0]" :: "r"(pointer) : "memory");
#else
        static_cast<void>(granule); static_cast<void>(tag);
#endif
    }
    [[nodiscard]] inline ::std::uint8_t load_tag(::std::byte* granule) noexcept
    {
#if defined(__linux__) && defined(__aarch64__)
        ::std::uintptr_t result{};
        asm volatile(".arch_extension memtag\nldg %0, [%1]" : "=r"(result) : "r"(granule) : "memory");
        return static_cast<::std::uint8_t>((result >> 56u) & 15u);
#else
        static_cast<void>(granule); return 0u;
#endif
    }
    inline void finish_tag_writes() noexcept
    {
#if defined(__linux__) && defined(__aarch64__)
        // Publish allocation tags before subsequent data operations or an
        // allocator's synchronization publishes the newly tagged allocation.
        asm volatile("dmb ish" ::: "memory");
#endif
    }
    [[nodiscard]] constexpr int protection_for(bool active) noexcept
    {
#if defined(__linux__) && defined(__aarch64__)
        return active ? protection_flag : 0;
#else
        static_cast<void>(active); return 0;
#endif
    }
}
