/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @file        output.h
 * @brief       Process-wide diagnostic output stream for UWVM.
 * @details     This header defines `u8log_output`, the UTF-8 stream used for user-facing UWVM diagnostics.  It receives
 *              command-line usage text, errors, warnings, fatal messages, verbose records, and similar runtime messages.
 *              Runtime compiler logs are intentionally routed through `u8runtime_log_output` instead.
 *
 *              The stream is configured by `--log-output`.  Hosted targets default to a duplicate of stderr so the
 *              diagnostic stream owns a stable output handle.  Targets without descriptor duplication or regular file
 *              support use standard-output/standard-error observers.  All variants are wrapped in
 *              `fast_io::basic_io_lockable_nonmovable` to support serialized writes from concurrent UWVM code paths.
 *
 * @author      MacroModel
 * @version     2.0.0
 * @date        2025-04-16
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
// import
# include <cstdlib>
# include <fast_io.h>
# include <fast_io_device.h>
# if defined(__linux__)
#  include <fcntl.h>
#  include <uwvm2/runtime/diagnostic/crash_policy.h>
# endif
#endif

#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

#ifdef UWVM
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::io
{
    enum class diagnostic_exit_status : int
    {
#ifdef __wasi__
        invalid_option = 1, invalid_module = 1
#else
        invalid_option = 126, invalid_module = 127
#endif
    };
    // For an already-diagnosed input/policy refusal, not a VM invariant.
    // Process-wide exit is essential even if compilation workers exist.
    // Do not replace this with Linux fast_io::fast_exit (SYS_exit/thread only).
    [[noreturn]] inline void fast_exit(diagnostic_exit_status status) noexcept
    { ::std::_Exit(static_cast<int>(status)); }

# if defined(__linux__)
    // All CLI phases use the same best-effort writer as terminal reports.
    // Keep the owned FD and the existing lock/reopen API, including --log-output,
    // while preventing failed diagnostics from changing the program result.
    struct diagnostic_observer
    {
        using char_type=char8_t;
        using output_char_type=char8_t;
        int descriptor{-1};
    };
    [[nodiscard]] inline int duplicate_diagnostic_descriptor(int source) noexcept
    {
        int const saved_errno=errno;
        // Standard diagnostic streams are best effort, including an explicit
        // --log-output/--runtime-compiler-log selection after startup. Do not
        // let EBADF/EMFILE from dup replace the eventual Guest exit status.
        int const descriptor=::fcntl(source,F_DUPFD_CLOEXEC,3);
        errno=saved_errno;return descriptor;
    }
    class diagnostic_file
    {
        ::fast_io::u8native_file file;
    public:
        using char_type=char8_t;
        using output_char_type=char8_t;
        diagnostic_file() noexcept = default;
        explicit diagnostic_file(::fast_io::io_dup_t,::fast_io::u8native_io_observer source) noexcept :
            file{duplicate_diagnostic_descriptor(source.native_handle())} {}
        template<typename... Args>
            requires requires(Args&&... args) {::fast_io::u8native_file(static_cast<Args&&>(args)...);}
        explicit diagnostic_file(Args&&... args) : file{static_cast<Args&&>(args)...} {}
        diagnostic_file(diagnostic_file const&)=delete;
        diagnostic_file& operator=(diagnostic_file const&)=delete;
        diagnostic_file(diagnostic_file&&) noexcept=default;
        diagnostic_file& operator=(diagnostic_file&&) noexcept=default;
        [[nodiscard]] int native_handle() const noexcept {return file.native_handle();}
        void reopen(::fast_io::io_dup_t,::fast_io::u8native_io_observer source) noexcept
        {
            int const saved_errno=errno;
            file.reset(duplicate_diagnostic_descriptor(source.native_handle()));
            errno=saved_errno;
        }
        template<typename... Args>
        void reopen(Args&&... args) {file=::fast_io::u8native_file{static_cast<Args&&>(args)...};}
        void close() {file.close();}
    };
    inline constexpr diagnostic_observer io_stream_ref_define(diagnostic_observer value) noexcept {return value;}
    inline diagnostic_observer io_stream_ref_define(diagnostic_file& value) noexcept {return {value.native_handle()};}
    inline void write_all_overflow_define(diagnostic_observer value,char8_t const* first,char8_t const* last) noexcept
    {static_cast<void>(::uwvm2::runtime::diagnostic::crash::write_descriptor(value.descriptor,
        reinterpret_cast<char const*>(first),static_cast<::std::size_t>(last-first)));}
    inline void scatter_write_all_overflow_define(diagnostic_observer value,
        ::fast_io::basic_io_scatter_t<char8_t> const* scatters,::std::size_t count) noexcept
    {
        // Consume every fragment synchronously through the same safe writer.
        // The owning file is deliberately not a native observer: ADL must never
        // select a native write/writev operation around this boundary.
        for(::std::size_t i{};i!=count;++i)
        {
            auto const& fragment{scatters[i]};
            if(fragment.len!=0) write_all_overflow_define(value,fragment.base,fragment.base+fragment.len);
        }
    }
    inline constexpr ::std::true_type print_semantic_optional_scatter_plan_stream(
        ::fast_io::io_reserve_type_t<char8_t,diagnostic_observer>) noexcept
    {
        // This exact observer owns no whole-record status-print customization.
        // The scatter operation consumes the same ordered nonempty fragments as
        // the scalar fallback, without buffering, throwing, or retaining them.
        // Closed color conditions can therefore use fast_io's linear plan
        // instead of enumerating every possible active record type.
        return {};
    }
    inline constexpr ::std::true_type print_semantic_optional_scatter_barrier_plan_stream(
        ::fast_io::io_reserve_type_t<char8_t,diagnostic_observer>) noexcept
    {
        // No status-print operation in this observer's namespace can own a
        // record crossing a proven direct-only source boundary either.
        return {};
    }
    inline constexpr ::std::true_type print_synchronous_direct_scatter_output(
        ::fast_io::io_reserve_type_t<char8_t,diagnostic_observer>) noexcept
    {return {};}
    [[nodiscard]] inline int duplicate_diagnostic_stderr() noexcept
    {
        // A closed inherited stderr must also survive static initialization.
        // Invalid handles simply drop diagnostics until an explicit reopen.
        return duplicate_diagnostic_descriptor(2);
    }
# endif
    /// @brief Process-wide UTF-8 diagnostic output stream.
    /// @details `u8log_output` is the sink for normal UWVM diagnostics and verbose messages.  It can be rebound by the
    ///          `--log-output` command-line option to stdout, stderr, or a file on hosted targets.  The lockable wrapper
    ///          allows callers to take a stream lock and emit multi-part diagnostic records without interleaving output
    ///          from other threads.
    /// @see u8runtime_log_output
    /// @see ::uwvm2::uwvm::cmdline::params::log_output
    /// @see ::uwvm2::uwvm::cmdline::params::details::log_output_callback
# if defined(__AVR__)
    // AVR targets use C stderr because POSIX/native descriptor facilities are unavailable.
    inline ::fast_io::basic_io_lockable_nonmovable<::fast_io::u8c_io_observer> u8log_output{
        ::fast_io::u8c_stderr()};  // [global] No global variable dependencies from other translation units
# elif ((defined(_WIN32) && !defined(__WINE__)) && defined(_WIN32_WINDOWS)) || (defined(__MSDOS__) || defined(__DJGPP__)) ||                                   \
     (defined(__NEWLIB__) && !defined(__CYGWIN__)) || defined(_PICOLIBC__) || defined(__wasm__)
    // These constrained targets cannot reliably duplicate stderr, so keep a native stderr observer.
    // win9x cannot dup stderr
    inline ::fast_io::basic_io_lockable_nonmovable<::fast_io::u8native_io_observer> u8log_output{
        ::fast_io::u8err()};  // [global] No global variable dependencies from other translation units
# elif defined(__linux__)
    inline ::fast_io::basic_io_lockable_nonmovable<diagnostic_file> u8log_output{duplicate_diagnostic_stderr()};
# else
    // Hosted targets own a duplicate of stderr until `--log-output` reopens the stream.
    inline ::fast_io::basic_io_lockable_nonmovable<::fast_io::u8native_file> u8log_output{
        ::fast_io::io_dup,
        ::fast_io::u8err()};  // [global] No global variable dependencies from other translation units
# endif

}  // namespace uwvm2::uwvm::io
#endif
