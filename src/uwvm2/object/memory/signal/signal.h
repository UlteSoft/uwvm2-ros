/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @author      MacroModel
 * @version     2.0.0
 * @copyright   APL-2.0 License
 */

/****************************************
 *  _   _ __        ____     __ __  __  *
 * | | | |\ \      / /\ \   / /|  \/  | *
 * | | | | \ \ /\ / /  \ \ / / | |\/| | *
 * | |_| |  \ V  V /    \ V /  | |  | | *
 *  \___/    \_/\_/      \_/   |_|  |_| *
 *                                      *
 ****************************************/

#pragma once

#ifndef UWVM_MODULE
// std
# include <cstddef>
# include <cstdlib>
# include <cstdint>
# include <cstring>
# include <limits>
# include <concepts>
# include <bit>
# include <mutex>
# include <uwvm2/utils/allocator/resource_limit.h>
# include <atomic>
# include <memory>
# include <utility>
// macro
# include <uwvm2/uwvm_predefine/utils/ansies/uwvm_color_push_macro.h>
# include <uwvm2/utils/macro/push_macros.h>
// platfrom
# include <signal.h>
# pragma push_macro("UWVM2_OBJECT_MEMORY_SIGNAL_HAS_UCONTEXT")
# undef UWVM2_OBJECT_MEMORY_SIGNAL_HAS_UCONTEXT
# if !(defined(_WIN32) || defined(__CYGWIN__)) && defined(__has_include)
#  if (defined(__APPLE__) || defined(__DARWIN_C_LEVEL)) && __has_include(<sys/ucontext.h>)
/// Darwin's <ucontext.h> exposes deprecated context routines and errors without _XOPEN_SOURCE; <sys/ucontext.h> still provides signal ucontext_t.
#   include <sys/ucontext.h>
#   define UWVM2_OBJECT_MEMORY_SIGNAL_HAS_UCONTEXT
#  elif __has_include(<ucontext.h>)
#   include <ucontext.h>
#   define UWVM2_OBJECT_MEMORY_SIGNAL_HAS_UCONTEXT
#  endif
# endif
// import
# include <fast_io.h>
# include <uwvm2/uwvm_predefine/io/impl.h>
# include <uwvm2/uwvm_predefine/utils/ansies/impl.h>
# include <uwvm2/utils/container/impl.h>
# include <uwvm2/object/memory/error/impl.h>
# include <uwvm2/extension/memtag/hardware_mte.h>
#endif

#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

#if defined(UWVM_SUPPORT_MMAP)

UWVM_MODULE_EXPORT namespace uwvm2::object::memory::signal
{
# if !(defined(_WIN32) || defined(__CYGWIN__))
    /// @brief      POSIX C signal APIs imported with their platform ABI symbol names.
    /// @details    This namespace avoids accidental calls through macro-expanded or wrapped names while
    ///             still letting the signal layer chain to the process' previous handlers.
    namespace posix
    {
        extern int raise(int) noexcept
#  if !(defined(__MSDOS__) || defined(__DJGPP__)) && !(defined(__APPLE__) || defined(__DARWIN_C_LEVEL))
            __asm__("raise")
#  else
            __asm__("_raise")
#  endif
                ;

        extern int sigaction(int, struct ::sigaction const*, struct ::sigaction*) noexcept
#  if !(defined(__MSDOS__) || defined(__DJGPP__)) && !(defined(__APPLE__) || defined(__DARWIN_C_LEVEL))
            __asm__("sigaction")
#  else
            __asm__("_sigaction")
#  endif
                ;

        extern void (*signal(int, void (*)(int)))(int) noexcept
#  if !(defined(__MSDOS__) || defined(__DJGPP__)) && !(defined(__APPLE__) || defined(__DARWIN_C_LEVEL))
            __asm__("signal")
#  else
            __asm__("_signal")
#  endif
                ;
    }  // namespace posix
# endif

    /// @brief      Reserved virtual-memory interval protected by the mmap-backed wasm memory runtime.
    /// @details    The interval [begin, end) is the complete reserved address range, including committed
    ///             pages and guard/uncommitted pages. A fault inside this interval is considered a wasm
    ///             memory bounds fault and is converted to mmap_memory_error_t.
    /// @note       @p length_p points to the currently allocated/committed byte length when the memory can
    ///             grow at runtime. When it is null, the fixed segment size is derived from end - begin.
    struct protected_memory_segment_t
    {
        /// @brief  First byte of the reserved virtual address interval.
        ::std::byte const* begin{};

        /// @brief  One-past-the-last byte of the reserved virtual address interval.
        ::std::byte const* end{};

        /// @brief  Optional live memory length used to report the valid wasm memory size.
        ::std::atomic_size_t const* length_p{};

        /// @brief  Wasm linear-memory index used in diagnostics.
        ::std::size_t memory_idx{};
# if defined(__linux__) && defined(__aarch64__)
        // Stable mmap shadow and byte base, published only during quiescent
        // activation. Both remain owned until the segment is unregistered.
        alignas(::std::atomic_ref<::std::byte const*>::required_alignment) mutable ::std::byte const* memtag_base{};
        ::std::uint8_t const* memtag_tags{};
        bool memtag_shared{};
# endif
    };
# if defined(__linux__) && defined(__aarch64__)
    struct hardware_memtag_fault
    {
        ::std::size_t memory_index{};
        ::std::uint64_t offset{}, length{};
        ::std::uintptr_t raw_address{}, instruction_address{};
        ::std::uint8_t allocation_tag{}, hardware_allocation_tag{};
        bool pointer_tag_available{};
        bool asynchronous{};
    };
# endif

    /// @brief      Callback type invoked when a protected mmap segment is faulted.
    /// @details    The callback receives a fully populated fault report. It must not throw and it is
    ///             expected to be terminal; the dispatcher terminates immediately after invoking it.
    using mmap_memory_out_of_bounds_func_t = void (*)(::uwvm2::object::memory::error::mmap_memory_error_t const&) noexcept;

    enum class registration_result { registered, metadata_refused, handler_unavailable, invalid_range };

    namespace detail
    {
# include "segment_registry.h"

        /// @brief  Optional process-wide hook for translating mmap memory faults to a runtime-specific action.
        inline mmap_memory_out_of_bounds_func_t mmap_memory_out_of_bounds_func{};  // [global]
# if defined(__linux__) && defined(__aarch64__)
        // Signal-context callback: fixed storage and async-signal-safe actions
        // only. It must not invoke debugger locks, heap allocation or unwinding.
        using hardware_memtag_fault_func_t = void (*)(hardware_memtag_fault const&) noexcept;
        inline hardware_memtag_fault_func_t hardware_memtag_fault_func{};
        inline ::std::atomic_bool expose_memtag_bits{};
# endif

        /// @brief  Internal runtime hook that receives the unchanged public fault report plus a borrowed native context.
        /// @note   The context is valid only for the synchronous callback. This separate channel preserves the ABI of
        ///         mmap_memory_error_t and mmap_memory_out_of_bounds_func_t.
        using mmap_memory_out_of_bounds_with_context_func_t =
            void (*)(::uwvm2::object::memory::error::mmap_memory_error_t const&, void const*) noexcept;
        inline mmap_memory_out_of_bounds_with_context_func_t mmap_memory_out_of_bounds_with_context_func{};  // [global]
        // A true result means the context now enters a cold, terminal landing
        // after kernel sigreturn. No ordinary runtime work runs in this handler.
        using mmap_fault_redirect_func_t = bool (*)(::uwvm2::object::memory::error::mmap_memory_error_t const&, void*) noexcept;
        inline mmap_fault_redirect_func_t mmap_fault_redirect_func{};

        /// @brief  Install the runtime-only native-context hook. The most recently installed hook owns fault delivery.
        inline constexpr void set_mmap_memory_out_of_bounds_with_context_handler(mmap_memory_out_of_bounds_with_context_func_t func) noexcept
        {
            mmap_memory_out_of_bounds_func = nullptr;
            mmap_fault_redirect_func = nullptr;
            mmap_memory_out_of_bounds_with_context_func = func;
        }

        /// @brief      Previous platform handlers saved when UWVM installs its own fault handler.
        /// @details    The signal layer only consumes faults that belong to registered protected segments.
        ///             Unrelated process faults are forwarded to the saved handlers when possible.
        struct signal_handlers_t
        {
# if defined(_WIN32) || defined(__CYGWIN__)
#  if defined(_WIN32_WINDOWS) || (defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0501)
            /// @brief  Previous Win32 unhandled-exception filter used on old Windows targets.
            ::std::atomic<::fast_io::win32::pvectored_exception_handler> previous_unhandled_exception_filter{};
#  else
            /// @brief  Handle returned by AddVectoredExceptionHandler for modern Windows targets.
            void* vectored_handler_handle{};
#  endif
# else
            struct prior_action
            {
                using action = struct ::sigaction;
                action before_install{}, displaced{};
                ::std::atomic<action const*> active{};
                bool installed{}; // only the serialized registry writer reads or changes this flag

                [[nodiscard]] bool install(int number, action const& handler) noexcept
                {
                    if(installed) { return true; }
                    if(posix::sigaction(number, nullptr, ::std::addressof(before_install)) != 0) { return false; }
                    // A fault can already arrive during the installing syscall.
                    // Chain to the observed action until it returns the actual displaced action.
                    active.store(::std::addressof(before_install), ::std::memory_order_release);
                    if(posix::sigaction(number, ::std::addressof(handler), ::std::addressof(displaced)) != 0) { return false; }
                    active.store(::std::addressof(displaced), ::std::memory_order_release);
                    installed = true;
                    return true;
                }
            };
            prior_action segv{}, bus{};
# endif
        };

        /// @brief  Process-wide storage for saved signal/exception handlers.
        inline signal_handlers_t signal_handlers{};  // [global]

        /// @brief      Return the valid wasm memory length associated with a protected segment.
        /// @details    Growable memories publish their current length through length_p. Fixed-size
        ///             segments use the reserved interval length as the reported memory length.
        inline constexpr ::std::size_t get_memory_length(protected_memory_segment_t const& seg) noexcept
        {
            if(seg.length_p != nullptr) [[likely]] { return static_cast<::std::size_t>(seg.length_p->load(::std::memory_order_acquire)); }

            // Registered VMAs are raw address-space intervals, not C++ array objects. Integer address arithmetic avoids
            // imposing unrelated-object pointer subtraction semantics on platform-owned mappings.
            return static_cast<::std::size_t>(reinterpret_cast<::std::uintptr_t>(seg.end) - reinterpret_cast<::std::uintptr_t>(seg.begin));
        }

        /// @brief      Build a user-facing mmap fault report from a segment and a raw fault address.
        /// @param      seg                 Registered protected segment containing the fault address.
        /// @param      fault_addr          Address reported by the platform fault mechanism.
        /// @param      instruction_address Best-effort program counter of the faulting instruction.
        /// @param      frame_address       Best-effort frame pointer captured at the faulting instruction.
        /// @param      stack_pointer       Best-effort stack pointer captured at the faulting instruction.
        /// @return     Diagnostic data consumed by the default reporter or a custom out-of-bounds hook.
        inline constexpr ::uwvm2::object::memory::error::mmap_memory_error_t make_mmap_memory_error(protected_memory_segment_t const& seg,
                                                                                                    ::std::byte const* fault_addr,
                                                                                                    ::std::uintptr_t instruction_address,
                                                                                                    ::std::uintptr_t frame_address,
                                                                                                    ::std::uintptr_t stack_pointer) noexcept
        {
            auto const offset{static_cast<::std::size_t>(reinterpret_cast<::std::uintptr_t>(fault_addr) -
                                                         reinterpret_cast<::std::uintptr_t>(seg.begin))};
            auto const memory_length{get_memory_length(seg)};

            return {.memory_idx = seg.memory_idx,
                    .memory_offset = static_cast<::std::uint_least64_t>(offset),
                    .memory_length = static_cast<::std::uint_least64_t>(memory_length),
                    .instruction_address = instruction_address,
                    .frame_address = frame_address,
                    .stack_pointer = stack_pointer};
        }

        /// @brief      Try to consume a platform fault as a wasm mmap memory fault.
        /// @return     False when the address is null or outside every registered protected segment.
        /// @details    For registered protected segments this function is terminal: it invokes the custom
        ///             handler when present, otherwise prints the default mmap memory error, and then
        ///             terminates. The boolean return exists for the platform handler's pass-through path.
        inline constexpr bool handle_fault_address_with_context(::std::byte const* fault_addr,
                                                                ::std::uintptr_t instruction_address,
                                                                ::std::uintptr_t frame_address,
                                                                ::std::uintptr_t stack_pointer,
                                                                void const* platform_context) noexcept
        {
            if(fault_addr == nullptr) [[unlikely]] { return false; }

            auto const fault_address{reinterpret_cast<::std::uintptr_t>(fault_addr)};
            ::uwvm2::object::memory::error::mmap_memory_error_t mmapmemerr{};
            if(!segments.any([&](protected_memory_segment_t const& seg) noexcept
            {
                auto const begin{reinterpret_cast<::std::uintptr_t>(seg.begin)};
                auto const end{reinterpret_cast<::std::uintptr_t>(seg.end)};
                if(fault_address < begin || fault_address >= end) { return false; }
                mmapmemerr = make_mmap_memory_error(seg, fault_addr, instruction_address, frame_address, stack_pointer);
                return true;
            })) { return false; }

            // Keep the public callback authoritative if direct internal slot manipulation leaves both hooks set.
            // Calls through either setter remain mutually exclusive because each setter clears the other slot.
            if(auto const handler{mmap_memory_out_of_bounds_func}; handler != nullptr) [[likely]]
            {
                handler(mmapmemerr);
                ::fast_io::fast_terminate();
                ::std::unreachable();
            }
            if(platform_context != nullptr && mmap_fault_redirect_func != nullptr &&
               mmap_fault_redirect_func(mmapmemerr, const_cast<void*>(platform_context))) { return true; }
            if(auto const handler{mmap_memory_out_of_bounds_with_context_func}; handler != nullptr) [[likely]]
            {
                handler(mmapmemerr, platform_context);
                ::fast_io::fast_terminate();
                ::std::unreachable();
            }

            ::uwvm2::object::memory::error::output_mmap_memory_error_and_terminate(mmapmemerr);
            ::std::unreachable();
        }

        /// @brief  ABI-stable fault-dispatch entry point for callers without a borrowed native context.
        inline constexpr bool handle_fault_address(::std::byte const* fault_addr,
                                                   ::std::uintptr_t instruction_address,
                                                   ::std::uintptr_t frame_address = 0u,
                                                   ::std::uintptr_t stack_pointer = 0u) noexcept
        { return handle_fault_address_with_context(fault_addr, instruction_address, frame_address, stack_pointer, nullptr); }

# if defined(_WIN32) || defined(__CYGWIN__)
        struct windows_fault_context_view
        {
            ::std::uintptr_t instruction_address{};
            ::std::uintptr_t frame_address{};
            ::std::uintptr_t stack_pointer{};
            void const* platform_context{};
        };

        /// @brief      Extract the architectural program, frame, and stack pointers from a Windows CONTEXT.
        /// @details    Only layouts defined by the vendored Win32 ABI layer are decoded. Unknown Windows targets and
        ///             Cygwin deliberately return an empty view rather than interpreting an unverified CONTEXT layout.
        ///             platform_context remains borrowed from the OS and is valid only during exception dispatch.
        [[nodiscard]] inline constexpr windows_fault_context_view get_windows_fault_context(void const* context_record) noexcept
        {
            if(context_record == nullptr) [[unlikely]] { return {}; }

#  if defined(_WIN32) && !defined(__CYGWIN__) && defined(_WIN64) &&                                                                            \
      ((defined(__x86_64__) || defined(_M_AMD64) || defined(_M_X64)) && !(defined(__arm64ec__) || defined(_M_ARM64EC)))
            ::fast_io::win32::win_current_context context{};
            ::std::memcpy(::std::addressof(context), context_record, sizeof(context));
            return {.instruction_address = static_cast<::std::uintptr_t>(context.Rip),
                    .frame_address = static_cast<::std::uintptr_t>(context.Rbp),
                    .stack_pointer = static_cast<::std::uintptr_t>(context.Rsp),
                    .platform_context = context_record};
#  elif defined(_WIN32) && !defined(__CYGWIN__) && defined(_WIN64) &&                                                                         \
      (defined(__aarch64__) || defined(_M_ARM64)) && !(defined(__arm64ec__) || defined(_M_ARM64EC))
            ::fast_io::win32::win_current_context context{};
            ::std::memcpy(::std::addressof(context), context_record, sizeof(context));
            return {.instruction_address = static_cast<::std::uintptr_t>(context.Pc),
                    .frame_address = static_cast<::std::uintptr_t>(context.X[29u]),
                    .stack_pointer = static_cast<::std::uintptr_t>(context.Sp),
                    .platform_context = context_record};
#  elif defined(_WIN32) && !defined(__CYGWIN__) && (defined(__i386__) || defined(_M_IX86))
            ::fast_io::win32::win_current_context context{};
            ::std::memcpy(::std::addressof(context), context_record, sizeof(context));
            return {.instruction_address = static_cast<::std::uintptr_t>(context.Eip),
                    .frame_address = static_cast<::std::uintptr_t>(context.Ebp),
                    .stack_pointer = static_cast<::std::uintptr_t>(context.Esp),
                    .platform_context = context_record};
#  else
            return {};
#  endif
        }

        /// @brief      Windows vectored exception entry point used to intercept access violations.
        /// @details    Access violations are translated only when the fault address falls inside a
        ///             registered protected segment. All other exceptions continue through the normal
        ///             Windows exception-dispatch chain.
        inline constexpr ::std::int_least32_t UWVM_WINSTDCALL vectored_exception_handler(::fast_io::win32::exception_pointers* exception_pointers) noexcept
        {
            if(exception_pointers == nullptr || exception_pointers->ExceptionRecord == nullptr) [[unlikely]] { return 0 /*EXCEPTION_CONTINUE_SEARCH*/; }

            auto const code{exception_pointers->ExceptionRecord->ExceptionCode};
            if(code ==
#  if defined(_WIN32_WINDOWS) || (defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0501)
               998 /*EXCEPTION_ACCESS_VIOLATION*/
#  else
               0xC0000005u /*STATUS_ACCESS_VIOLATION*/
#  endif
            )
            {
                if(exception_pointers->ExceptionRecord->NumberParameters >= 2u)
                {
                    auto const fault_addr{reinterpret_cast<::std::byte const*>(exception_pointers->ExceptionRecord->ExceptionInformation[1u])};

                    // The exception record is the architecture-neutral IP fallback. A supported CONTEXT supplies the exact
                    // fault-time IP/SP/FP and the complete native state needed by authoritative Win64 SEH unwinding.
                    auto const fault_context{get_windows_fault_context(exception_pointers->ContextRecord)};
                    auto const exception_address{reinterpret_cast<::std::uintptr_t>(exception_pointers->ExceptionRecord->ExceptionAddress)};
                    auto const instruction_address{fault_context.instruction_address == 0u ? exception_address : fault_context.instruction_address};

                    if(handle_fault_address_with_context(fault_addr,
                                                         instruction_address,
                                                         fault_context.frame_address,
                                                         fault_context.stack_pointer,
                                                         fault_context.platform_context))
                    {
                        return -1 /*EXCEPTION_CONTINUE_EXECUTION*/;
                    }
                }
            }

            return 0 /*EXCEPTION_CONTINUE_SEARCH*/;
        }

#  if defined(_WIN32_WINDOWS) || (defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0501)
        /// @brief      Compatibility unhandled-exception filter for old Windows targets.
        /// @details    Old targets do not provide vectored exception handling, so the same access-violation
        ///             translation is installed through SetUnhandledExceptionFilter and chained manually.
        inline constexpr ::std::int_least32_t UWVM_WINSTDCALL unhandled_exception_filter(::fast_io::win32::exception_pointers* exception_pointers) noexcept
        {
            auto const ret{vectored_exception_handler(exception_pointers)};
            if(ret != 0) { return ret; }

            auto const prev{signal_handlers.previous_unhandled_exception_filter.load(::std::memory_order_acquire)};
            if(prev != nullptr && prev != unhandled_exception_filter) { return prev(exception_pointers); }

            return 0 /*EXCEPTION_CONTINUE_SEARCH*/;
        }
#  endif

        /// @brief      Install the Windows access-violation handler once for the process.
        /// @details    The handler is process-wide because Windows exception dispatch is process/thread
        ///             global. register_protected_segment calls this lazily before publishing the first
        ///             protected interval.
        [[nodiscard]] inline bool install_signal_handler() noexcept
        {
            // Registry writers serialize installation and publication.
            static bool installed{};
            if(installed) { return true; }
            auto& handlers{signal_handlers};

            // NOTE: In C++17 and later, for `E1 = E2`, `E2` is sequenced before `E1`.
            // Keep handler-state evaluation separated from installing the handler to avoid re-entrancy
            // pitfalls if `signal_handlers` ever becomes lazily initialized.
#  if defined(_WIN32_WINDOWS) || (defined(_WIN32_WINNT) && _WIN32_WINNT < 0x0501)
            handlers.previous_unhandled_exception_filter = ::fast_io::win32::SetUnhandledExceptionFilter(unhandled_exception_filter);
#  else
            handlers.vectored_handler_handle = ::fast_io::win32::AddVectoredExceptionHandler(1u, vectored_exception_handler);
            if(handlers.vectored_handler_handle == nullptr) { return false; }
#  endif
            installed = true;
            return true;
        }

# else
        /// @brief      Extract the faulting instruction address from a POSIX ucontext, when supported.
        /// @details    The exact register field is OS and architecture specific. Unsupported targets return
        ///             zero, which keeps diagnostics valid while omitting the instruction address.
        [[nodiscard]] inline constexpr ::std::uintptr_t get_signal_instruction_address([[maybe_unused]] void* context) noexcept
        {
#  ifdef UWVM2_OBJECT_MEMORY_SIGNAL_HAS_UCONTEXT
            if(context == nullptr) [[unlikely]] { return 0u; }
            [[maybe_unused]] auto const uctx{static_cast<::ucontext_t const*>(context)};

#   if defined(__linux__) && defined(__x86_64__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.gregs[REG_RIP]);
#   elif defined(__linux__) && defined(__i386__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.gregs[REG_EIP]);
#   elif defined(__linux__) && defined(__aarch64__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.pc);
#   elif defined(__linux__) && defined(__arm__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.arm_pc);
#   elif defined(__linux__) && defined(__riscv)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__gregs[REG_PC]);
#   elif defined(__linux__) && (defined(__loongarch64) || (defined(__loongarch__) && defined(__loongarch_grlen) && __loongarch_grlen == 64))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__pc);
#   elif defined(__linux__) && (defined(__mips__) || defined(__mips))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.pc);
#   elif defined(__linux__) && (defined(__powerpc64__) || defined(__PPC64__))
            // Linux PowerPC stores NIP (next instruction pointer) at general-register slot 32.
            // This is PT_NIP in the kernel uapi ptrace register layout.
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.gp_regs[32u]);
#   elif defined(__linux__) && (defined(__powerpc__) || defined(__PPC__))
            // Linux PowerPC stores NIP (next instruction pointer) at general-register slot 32.
            // This is PT_NIP in the kernel uapi ptrace register layout.
            auto const regs{uctx->uc_mcontext.uc_regs};
            if(regs == nullptr) [[unlikely]] { return 0u; }
            return static_cast<::std::uintptr_t>(regs->gregs[32u]);
#   elif defined(__linux__) && (defined(__s390__) || defined(__s390x__))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.psw.addr);
#   elif defined(__linux__) && (defined(__sparc64__) || ((defined(__sparc__) || defined(__sparc)) && defined(__arch64__)))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.mc_gregs[MC_PC]);
#   elif defined(__linux__) && (defined(__sparc__) || defined(__sparc))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.gregs[REG_PC]);
#   elif defined(__linux__) && defined(__alpha__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.sc_pc);
#   elif defined(__linux__) && defined(__arc__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__stop_pc);
#   elif defined(__linux__) && defined(__csky__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__gregs.__pc);
#   elif defined(__linux__) && defined(__hppa__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.sc_iaoq[0u]);
#   elif defined(__linux__) && defined(__m68k__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.gregs[R_PC]);
#   elif defined(__linux__) && defined(__microblaze__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.regs.pc);
#   elif defined(__linux__) && defined(__or1k__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__pc);
#   elif defined(__linux__) && (defined(__sh__) || defined(__SH__))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.pc);
#   elif defined(__OpenBSD__) && (defined(__amd64__) || defined(__x86_64__))
            return static_cast<::std::uintptr_t>(uctx->sc_rip);
#   elif defined(__OpenBSD__) && defined(__i386__)
            return static_cast<::std::uintptr_t>(uctx->sc_eip);
#   elif defined(__OpenBSD__) && defined(__aarch64__)
            return static_cast<::std::uintptr_t>(uctx->sc_elr);
#   elif defined(__OpenBSD__) && defined(__arm__)
            return static_cast<::std::uintptr_t>(uctx->sc_pc);
#   elif defined(__OpenBSD__) && defined(__riscv)
            return static_cast<::std::uintptr_t>(uctx->sc_sepc);
#   elif defined(__OpenBSD__) && (defined(__powerpc64__) || defined(__PPC64__))
            return static_cast<::std::uintptr_t>(uctx->sc_pc);
#   elif defined(__OpenBSD__) && (defined(__powerpc__) || defined(__PPC__))
            return static_cast<::std::uintptr_t>(uctx->sc_frame.srr0);
#   elif defined(__OpenBSD__) && (defined(__sparc__) || defined(__sparc))
            return static_cast<::std::uintptr_t>(uctx->sc_pc);
#   elif defined(__OpenBSD__) && (defined(__mips__) || defined(__mips))
            return static_cast<::std::uintptr_t>(uctx->sc_pc);
#   elif defined(__OpenBSD__) && defined(__hppa__)
            return static_cast<::std::uintptr_t>(uctx->sc_pcoqh);
#   elif defined(__OpenBSD__) && defined(__alpha__)
            return static_cast<::std::uintptr_t>(uctx->sc_pc);
#   elif defined(__OpenBSD__) && defined(__m88k__)
            // OpenBSD/m88k sigcontext stores struct reg inline: r[32], epsr, fpsr, fpcr, sxip.
            // sxip carries low valid/exception flag bits, so apply the XIP_ADDR mask.
            return static_cast<::std::uintptr_t>(uctx->sc_regs[35u] & 0xfffffffcUL);
#   elif defined(__FreeBSD__) && (defined(__amd64__) || defined(__x86_64__))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.mc_rip);
#   elif defined(__FreeBSD__) && defined(__i386__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.mc_eip);
#   elif defined(__FreeBSD__) && defined(__aarch64__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.mc_gpregs.gp_elr);
#   elif defined(__FreeBSD__) && defined(__arm__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__gregs[_REG_PC]);
#   elif defined(__FreeBSD__) && defined(__riscv)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.mc_gpregs.gp_sepc);
#   elif defined(__FreeBSD__) && (defined(__powerpc__) || defined(__powerpc64__) || defined(__PPC__) || defined(__PPC64__))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.mc_srr0);
#   elif defined(__NetBSD__) && (defined(__amd64__) || defined(__x86_64__))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__gregs[_REG_RIP]);
#   elif defined(__NetBSD__) && defined(__i386__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__gregs[_REG_EIP]);
#   elif defined(__NetBSD__) && (defined(__aarch64__) || defined(__arm__))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__gregs[_REG_PC]);
#   elif defined(__NetBSD__) && defined(__riscv)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__gregs[_REG_PC]);
#   elif defined(__NetBSD__) && (defined(__powerpc__) || defined(__powerpc64__) || defined(__PPC__) || defined(__PPC64__))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__gregs[_REG_PC]);
#   elif defined(__NetBSD__) && (defined(__mips__) || defined(__mips))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__gregs[_REG_EPC]);
#   elif defined(__NetBSD__) && defined(__hppa__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__gregs[_REG_PCOQH]);
#   elif defined(__NetBSD__) && (defined(__sparc__) || defined(__sparc))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__gregs[_REG_PC]);
#   elif defined(__NetBSD__) && defined(__alpha__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__gregs[_REG_PC]);
#   elif defined(__NetBSD__) && defined(__m68k__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__gregs[_REG_PC]);
#   elif defined(__NetBSD__) && (defined(__sh__) || defined(__SH__))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__gregs[_REG_PC]);
#   elif defined(__NetBSD__) && defined(__vax__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.__gregs[_REG_PC]);
#   elif defined(__DragonFly__) && (defined(__amd64__) || defined(__x86_64__))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.mc_rip);
#   elif defined(__sun) && (defined(__i386__) || defined(__amd64) || defined(__amd64__) || defined(__x86_64__) || defined(__sparc__) || defined(__sparc))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.gregs[REG_PC]);
#   elif defined(__serenity__) && defined(__x86_64__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.rip);
#   elif defined(__serenity__) && (defined(__aarch64__) || defined(__riscv))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext.pc);
#   elif defined(__APPLE__) && defined(__x86_64__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext->__ss.__rip);
#   elif defined(__APPLE__) && defined(__i386__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext->__ss.__eip);
#   elif defined(__APPLE__) && (defined(__ppc__) || defined(__POWERPC__))
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext->__ss.__srr0);
#   elif defined(__APPLE__) && defined(__aarch64__)
            return static_cast<::std::uintptr_t>(uctx->uc_mcontext->__ss.__pc);
#   else
            // static_cast<void>(uctx);
            return 0u;
#   endif
#  else
            // static_cast<void>(context);
            return 0u;
#  endif
        }

        /// @brief      Forward an unhandled POSIX signal to the handler that was installed before UWVM.
        /// @details    Both SA_SIGINFO handlers and classic one-argument handlers are supported. Default
        ///             handlers are restored and re-raised so the process observes the platform's normal
        ///             crash behavior.
        inline constexpr void dispatch_previous_handler(int signal, ::siginfo_t* siginfo, void* context, struct ::sigaction const& previous) noexcept
        {
            if(previous.sa_flags & SA_SIGINFO)
            {
                if(reinterpret_cast<void*>(previous.sa_sigaction) == reinterpret_cast<void*>(SIG_DFL))
                {
                    posix::signal(signal, SIG_DFL);
                    posix::raise(signal);
                    return;
                }

                if(reinterpret_cast<void*>(previous.sa_sigaction) == reinterpret_cast<void*>(SIG_IGN)) { return; }

                previous.sa_sigaction(signal, siginfo, context);
                return;
            }

            if(reinterpret_cast<void*>(previous.sa_handler) == reinterpret_cast<void*>(SIG_DFL))
            {
                posix::signal(signal, SIG_DFL);
                posix::raise(signal);
                return;
            }

            if(reinterpret_cast<void*>(previous.sa_handler) == reinterpret_cast<void*>(SIG_IGN)) { return; }

            previous.sa_handler(signal);
        }

        /// @brief      POSIX SIGSEGV/SIGBUS entry point used to intercept protected-memory faults.
        /// @details    Faults inside registered protected segments are converted to wasm memory diagnostics.
        ///             POSIX captures only the faulting instruction address; generated frame and stack
        ///             pointers are deliberately not seeded into the POSIX CFI native replacement path.
        ///             Other faults are delegated to the previous handler, or to the platform default action
        ///             when no previous handler is available.
# if defined(__linux__) && defined(__aarch64__)
        [[nodiscard]] inline bool dispatch_hardware_memtag_fault(::siginfo_t const* info,
            ::std::uintptr_t pc) noexcept
        {
            static_assert(::std::atomic_ref<::std::byte const*>::is_always_lock_free);
            static_assert(::std::atomic_ref<::std::uint8_t>::is_always_lock_free);
            static_assert(::std::atomic_size_t::is_always_lock_free);
            if(info==nullptr) { return false; }
            if(info->si_code==8 /* SEGV_MTEAERR */)
            {
                if(!::uwvm2::extension::memtag::hardware_mte::asynchronous_execution_armed) { return false; }
                // Linux provides neither a fault address nor the faulting PC.
                // Only an actually admitted MTE entry owns this notification;
                // do not search address zero or attribute the notification PC.
                if(::uwvm2::extension::memtag::hardware_mte::asynchronous_capture_armed)
                {
                    ::uwvm2::extension::memtag::hardware_mte::asynchronous_fault_observed=1;
                    return true;
                }
                hardware_memtag_fault const report{SIZE_MAX,UINT64_MAX,0u,0u,0u,0u,0u,false,true};
                if(hardware_memtag_fault_func) { hardware_memtag_fault_func(report); }
                ::_Exit(122);
            }
            if(info->si_code!=9 /* SEGV_MTESERR: synchronous */) { return false; }
            auto const raw{reinterpret_cast<::std::uintptr_t>(info->si_addr)};
            auto const address{raw&UINT64_C(0x00ffffffffffffff)};
            hardware_memtag_fault report{};
            if(!segments.any([&](protected_memory_segment_t const& segment) noexcept
            {
                auto const* memtag_base{::std::atomic_ref{segment.memtag_base}.load(::std::memory_order_acquire)};
                if(memtag_base == nullptr) { return false; }
                auto const base{reinterpret_cast<::std::uintptr_t>(memtag_base)};
                auto const length{segment.length_p->load(::std::memory_order_acquire)};
                if(address < base || address - base >= length) { return false; }
                auto const offset{address - base};
                auto* tag_pointer{const_cast<::std::uint8_t*>(segment.memtag_tags) + offset / 16u};
                auto const tag{segment.memtag_shared ?
                    ::std::atomic_ref<::std::uint8_t>{*tag_pointer}.load(::std::memory_order_relaxed) : *tag_pointer};
                auto const physical{::uwvm2::extension::memtag::hardware_mte::load_tag(
                    const_cast<::std::byte*>(memtag_base) + offset / 16u * 16u)};
                report = {segment.memory_idx, offset, length, raw, pc, tag, physical, expose_memtag_bits};
                return true;
            })) { return false; }
            if(hardware_memtag_fault_func) { hardware_memtag_fault_func(report); }
            ::_Exit(122);
        }
# endif

        inline constexpr void posix_signal_handler(int signal, ::siginfo_t* siginfo, void* context) noexcept
        {
            auto const fault_addr{siginfo == nullptr ? nullptr : reinterpret_cast<::std::byte const*>(siginfo->si_addr)};
            auto const instruction_address{get_signal_instruction_address(context)};

# if defined(__linux__) && defined(__aarch64__)
            if(signal==SIGSEGV && siginfo!=nullptr && (siginfo->si_code==9 || siginfo->si_code==8))
            {
                if(dispatch_hardware_memtag_fault(siginfo,instruction_address)) { return; }
                // Unowned tag faults are host faults, never
                // reclassified as Wasm out-of-bounds merely by address.
            }
            else
# endif
            // User-generated SIGSEGV/SIGBUS does not carry a kernel memory
            // fault and must retain the previous handler's semantics.
            if(siginfo != nullptr && siginfo->si_code > 0 &&
               handle_fault_address_with_context(fault_addr, instruction_address, 0u, 0u, context)) { return; }

            signal_handlers_t::prior_action::action const* previous{};
            if(signal == SIGSEGV) { previous = signal_handlers.segv.active.load(::std::memory_order_acquire); }
#  ifdef SIGBUS
            if(signal == SIGBUS) { previous = signal_handlers.bus.active.load(::std::memory_order_acquire); }
#  endif
            if(previous != nullptr) { dispatch_previous_handler(signal, siginfo, context, *previous); return; }

            posix::signal(signal, SIG_DFL);
            posix::raise(signal);
        }

        /// @brief      Install POSIX SIGSEGV and, where available, SIGBUS handlers once for the process.
        /// @details    The handler is installed lazily by register_protected_segment. It uses SA_SIGINFO so
        ///             the fault address and optional machine context are available for diagnostics.
        [[nodiscard]] inline bool install_signal_handler() noexcept
        {
            // Keep successful individual installations across a partial failure.
            // Each retry preserves the original host chain.
            static bool installed{};
            if(installed) { return true; }
            struct ::sigaction act{};
            act.sa_sigaction = posix_signal_handler;
            sigemptyset(::std::addressof(act.sa_mask));
            act.sa_flags = SA_SIGINFO;
# if defined(__linux__) && defined(__aarch64__)
            // Linux UAPI SA_UNSUPPORTED|SA_EXPOSE_TAGBITS. Probe the installed
            // flags below; do not claim a logical tag when the kernel strips it.
            act.sa_flags |= 0x400 | 0x800;
# endif
#ifdef SA_ONSTACK
            // A later memory-provider installation may sit above the runtime's
            // native-stack handler. Preserve alternate-stack delivery so a
            // stack fault can still be forwarded with an exhausted native SP.
            act.sa_flags |= SA_ONSTACK;
#endif

            if(!signal_handlers.segv.install(SIGSEGV, act)) { return false; }
# if defined(__linux__) && defined(__aarch64__)
            struct ::sigaction current{};
            expose_memtag_bits = posix::sigaction(SIGSEGV, nullptr, ::std::addressof(current)) == 0 &&
                (current.sa_flags & 0x400) == 0 && (current.sa_flags & 0x800) != 0;
# endif
#  ifdef SIGBUS
            if(!signal_handlers.bus.install(SIGBUS, act)) { return false; }
#  endif
            installed = true;
            return true;
        }
# endif
    }  // namespace detail

    /// @brief      Set the process-wide callback used for protected mmap memory faults.
    /// @note       The callback should be installed before guest execution begins. Updating it concurrently
    ///             with fault handling is not synchronized.
    inline constexpr void set_mmap_memory_out_of_bounds_handler(mmap_memory_out_of_bounds_func_t func) noexcept
    {
        detail::mmap_memory_out_of_bounds_with_context_func = nullptr;
        detail::mmap_fault_redirect_func = nullptr;
        detail::mmap_memory_out_of_bounds_func = func;
    }

    /// @brief      Register a reserved memory interval whose guard faults should be reported as wasm mmap faults.
    /// @param      begin      First byte of the reserved virtual address interval.
    /// @param      end        One-past-the-last byte of the reserved virtual address interval.
    /// @param      length_p   Optional pointer to the live committed/valid length for growable memories.
    /// @param      memory_idx Wasm linear-memory index used in diagnostics.
    /// @note       The owner and its optional live length must remain valid until unregistration returns.
    [[nodiscard]] inline registration_result try_register_protected_segment(::std::byte const* begin, ::std::byte const* end,
        ::std::atomic_size_t const* length_p = nullptr, ::std::size_t memory_idx = 0uz) noexcept
    {
        if(begin == nullptr || end == nullptr || reinterpret_cast<::std::uintptr_t>(begin) >= reinterpret_cast<::std::uintptr_t>(end))
        { return registration_result::invalid_range; }
        return detail::segments.insert({begin, end, length_p, memory_idx}, detail::install_signal_handler);
    }

    inline void register_protected_segment(::std::byte const* begin, ::std::byte const* end,
        ::std::atomic_size_t const* length_p = nullptr, ::std::size_t memory_idx = 0uz) noexcept
    {
        switch(try_register_protected_segment(begin, end, length_p, memory_idx))
        {
            case registration_result::registered: return;
            case registration_result::metadata_refused: ::uwvm2::utils::allocator::resource::initializer_metadata_limit();
            case registration_result::handler_unavailable:
                ::uwvm2::utils::allocator::resource::reject_allocation("uwvm: [error] initializer: cannot install the native mmap fault handler.\n");
            case registration_result::invalid_range:
# ifdef UWVM
                ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output, u8"uwvm: [fatal] Invalid protected memory segment.\n\n");
# else
                ::fast_io::io::perr(::fast_io::u8err(), u8"uwvm: [fatal] Invalid protected memory segment.\n\n");
# endif
                ::fast_io::fast_terminate();
        }
    }

# if defined(__linux__) && defined(__aarch64__)
    // Quiescent owner activation only; no guest supplies these native ranges.
    [[nodiscard]] inline bool register_hardware_memtag_owner(::std::byte const* reservation,
        ::std::byte const* base, ::std::uint8_t const* tags, bool shared) noexcept
    {
        return detail::segments.attach_memtag(reservation, base, tags, shared);
    }
# endif

    /// @brief      Remove a previously registered protected memory interval.
    /// @details    The interval is matched by the exact begin/end pair used at registration time. Missing
    ///             entries are ignored because teardown paths may be reached after partial initialization.
    /// @note       Stop guest use of this owner before teardown. Other mappings may still execute.
    inline void unregister_protected_segment(::std::byte const* begin, ::std::byte const* end) noexcept
    {
        if(begin != nullptr && end != nullptr) { detail::segments.erase(begin, end); }
    }

    /// @brief      Remove all protected intervals during whole-runtime quiescent teardown.
    inline void clear_protected_segments() noexcept { detail::segments.clear(); }

}  // namespace uwvm2::object::memory::signal

#endif

#ifndef UWVM_MODULE
// platfrom
# pragma pop_macro("UWVM2_OBJECT_MEMORY_SIGNAL_HAS_UCONTEXT")
// macro
# include <uwvm2/utils/macro/pop_macros.h>
# include <uwvm2/uwvm_predefine/utils/ansies/uwvm_color_pop_macro.h>
#endif
