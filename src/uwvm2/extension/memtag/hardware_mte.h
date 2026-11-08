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
# if defined(__linux__) && defined(__aarch64__)
#  include <cerrno>
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
        thread_mode_not_synchronous, protection_failed
    };
    struct setup_result
    {
        setup_error error{};
        int system_error{};
        [[nodiscard]] constexpr explicit operator bool() const noexcept { return error == setup_error::none; }
    };
    // Linux UAPI values are stable even when compiling against older sysroots.
    inline constexpr int protection_flag{0x20};
    [[nodiscard]] inline bool supported() noexcept
    {
#if defined(__linux__) && defined(__aarch64__)
        return (::getauxval(26 /* AT_HWCAP2 */) & (1ul << 18u)) != 0ul;
#else
        return false;
#endif
    }
    // Every actual execution/host-entry thread must call this before accessing
    // an active owner. Never request ASYNC or a set of kernel-selectable modes.
    [[nodiscard]] inline setup_result enable_current_thread_sync() noexcept
    {
#if defined(__linux__) && defined(__aarch64__)
        if(!supported()) { return {setup_error::unsupported}; }
        constexpr unsigned long flags{1ul | 2ul | (0xfffeul << 3u)};
        if(::prctl(55 /* PR_SET_TAGGED_ADDR_CTRL */, flags, 0ul, 0ul, 0ul) != 0)
        { return {setup_error::thread_control_failed, errno}; }
        auto const actual{::prctl(56 /* PR_GET_TAGGED_ADDR_CTRL */, 0ul, 0ul, 0ul, 0ul)};
        if(actual < 0) { return {setup_error::thread_control_failed, errno}; }
        if((actual & 6) != 2 || (actual & 1) != 1)
        { return {setup_error::thread_mode_not_synchronous}; }
        // A host entry must not inherit a temporarily disabled PSTATE.TCO.
        asm volatile(".arch_extension memtag\nmsr tco, #0" ::: "memory");
        return {};
#else
        return {setup_error::unsupported};
#endif
    }
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
