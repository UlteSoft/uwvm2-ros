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
#include <uwvm2/utils/container/standard.h>
#include <uwvm2/utils/allocator/resource_limit.h>

#ifndef UWVM_MODULE
// std
# include <cstddef>
# include <cstdint>
# include <climits>
# include <type_traits>
# include <utility>
# include <string>
# include <string_view>
# include <uwvm2/utils/security/wsea.h>
// macro
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/uwvm/utils/ansies/uwvm_color_push_macro.h>
// import
# include <fast_io.h>
# include <span>
# include <uwvm2/extension/memtag/globals.h>
# include <uwvm2/utils/container/impl.h>
# include <uwvm2/utils/ansies/impl.h>
# include <uwvm2/utils/debug/impl.h>
# include <uwvm2/utils/madvise/impl.h>
# include <uwvm2/utils/utf/impl.h>
# include <uwvm2/parser/wasm/base/impl.h>
# include <uwvm2/parser/wasm/concepts/impl.h>
# include <uwvm2/parser/wasm/standard/impl.h>
# include <uwvm2/parser/wasm/binfmt/base/impl.h>
# include <uwvm2/uwvm/io/impl.h>
# include <uwvm2/uwvm/utils/ansies/impl.h>
# include <uwvm2/uwvm/utils/memory/impl.h>
# include <uwvm2/uwvm/wasm/base/impl.h>
# include <uwvm2/uwvm/wasm/type/impl.h>
# include <uwvm2/uwvm/wasm/storage/impl.h>
# include <uwvm2/uwvm/wasm/feature/impl.h>
# include <uwvm2/uwvm/wasm/custom/impl.h>
# include <uwvm2/uwvm/wasm/warning/impl.h>
#endif

#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::uwvm::wasm::loader
{
    namespace source_input = ::uwvm2::uwvm::wasm::storage::input;
    namespace details
    {
        // Check guest metadata as soon as the declaration types are available.
        // The initializer later checks ranges against the actual linked owner.
        template<::uwvm2::parser::wasm::concepts::wasm_feature... Fs>
        [[nodiscard]] inline constexpr bool admit_memtag_globals(
            ::uwvm2::parser::wasm::binfmt::ver1::wasm_binfmt_ver1_module_extensible_storage_t<Fs...> const& module,
            ::uwvm2::parser::wasm::standard::wasm1p1::features::wasm_binfmt1p1_feature_parameter const& policy) noexcept
        {
            namespace mt = ::uwvm2::extension::memtag;
            namespace ops = ::uwvm2::parser::wasm::concepts::operation;
            namespace features = ::uwvm2::parser::wasm::standard::wasm1::features;
            auto const& customs{ops::get_first_type_in_tuple<features::custom_section_storage_t>(module.sections)};
            bool seen{};
            for(auto const& section : customs.customs)
            {
                if(section.custom_name == mt::obsolete_globals_section_name) { return false; }
                if(section.custom_name != mt::linear_globals_section_name) { continue; }
                if(seen || policy.disable_memtag_extension || policy.disable_adm_extension) { return false; }
                seen = true;
                auto const& imported{ops::get_first_type_in_tuple<features::import_section_storage_t<Fs...>>(module.sections).importdesc.index_unchecked(2uz)};
                auto const& local{ops::get_first_type_in_tuple<features::memory_section_storage_t<Fs...>>(module.sections).memories};
                auto const payload{::std::span<::std::byte const>{reinterpret_cast<::std::byte const*>(section.custom_begin),
                                                                  reinterpret_cast<::std::byte const*>(section.sec_span.sec_end)}};
                auto const result{mt::validate_linear_globals(payload, [&](::std::uint32_t index, ::std::uint64_t& extent) noexcept
                {
                    // Linked imports and the owner's configured initial size can
                    // exceed the declared minimum. Check the actual extent at link.
                    extent = UINT64_MAX;
                    if(index < imported.size()) { return mt::is_tagged_memory(imported.index_unchecked(index)->imports.storage.memory); }
                    auto const local_index{index - imported.size()};
                    return local_index < local.size() && mt::is_tagged_memory(local.index_unchecked(local_index));
                })};
                if(!result) { return false; }
            }
            return true;
        }

        template<typename Color>
        [[nodiscard]] inline constexpr auto diagnostic_color(Color&& color) noexcept
        {
#if defined(__linux__)
            // Conditional scatter alternatives multiply across one diagnostic
            // on some target print paths. Keep one bounded fast_io view type;
            // the static ANSI bytes and the single locked output are unchanged.
            using view = ::uwvm2::utils::container::u8string_view;
            return ::uwvm2::uwvm::utils::ansies::put_color ? view{color} : view{};
#else
            return ::uwvm2::uwvm::utils::ansies::diagnostic_color(::std::forward<Color>(color));
#endif
        }
    }

    extern "C++" {
    enum class load_wasm_file_rtl
    {
        ok,
        load_error,
        wasm_parser_error
    };

    inline constexpr load_wasm_file_rtl load_wasm_file(::uwvm2::uwvm::wasm::type::wasm_file_t & output,
                                                       ::uwvm2::utils::container::u8cstring_view load_file_name,
                                                       ::uwvm2::utils::container::u8string_view rename_module_name,
                                                       ::uwvm2::uwvm::wasm::type::wasm_parameter_t para,
                                                       ::uwvm2::utils::control::owned_file_image::owner image = {}) noexcept
    {
        // Never reload a parsed immutable image through mutable native file
        // borrows. Its actual owner must retire after all source users drain.
        if(output.has_owned_source_image()) { return load_wasm_file_rtl::load_error; }
        // Native custom handlers receive the final file address. On rejection
        // release all candidate borrows/backing and restore the previous file.
        using file_type = ::uwvm2::uwvm::wasm::type::wasm_file_t;
        auto& wf{output};
        file_type previous{::std::move(wf)};
        wf = file_type{};
        struct restore_on_failure
        {
            file_type& file;
            file_type& previous;
            bool committed{};
            constexpr ~restore_on_failure() { if(!committed) { file = ::std::move(previous); } }
        } transaction{wf, previous};
        source_input::input_policy const input{para, load_file_name, rename_module_name};
        auto const input_refusal = [] {
            ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                u8"uwvm: [error] parser: input refused by file extent, input-bytes, input-live-bytes or parser.limit.input-bytes.");
            return load_wasm_file_rtl::load_error;
        };
        wf.secure_required_ = ::uwvm2::utils::security::wsea::module_selected(
            ::std::string_view{reinterpret_cast<char const*>(load_file_name.cbegin()), load_file_name.size()});
        if(wf.secure_required_ && !image)
        {
            auto snapshot{source_input::read_wasm_image({load_file_name.cbegin(), load_file_name.size()}, rename_module_name,
                para, source_input::input_image::maximum_bytes)};
            if(!snapshot)
            {
                if(snapshot.error == ::uwvm2::utils::control::owned_image_error::quota_exceeded) { return input_refusal(); }
                ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                    u8"uwvm: [SIG_001] DENIED: unable to retain immutable secure WASM input.");
                return load_wasm_file_rtl::load_error;
            }
            image = ::std::move(snapshot.image);
        }
        if(image && !source_input::admit_owned_input(image, input)) { return input_refusal(); }
        if(wf.secure_required_)
        {
            auto authorization{::uwvm2::utils::security::wsea::verify_module(
                ::std::string_view{reinterpret_cast<char const*>(load_file_name.cbegin()), load_file_name.size()},
                reinterpret_cast<::std::byte const*>(image->cbegin()), image->size())};
            if(!authorization || !authorization.authorization)
            {
                ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                    u8"uwvm: [", ::fast_io::mnp::code_cvt(::fast_io::mnp::os_c_str(authorization.code ? authorization.code : "SIG_003")),
                    u8"] DENIED: ", ::fast_io::mnp::code_cvt(::fast_io::mnp::os_c_str(authorization.message ? authorization.message : "missing execution authorization")));
                return load_wasm_file_rtl::load_error;
            }
            wf.execution_authorization_ = ::std::move(authorization.authorization);
        }
        wf.file_name = load_file_name;
        wf.wasm_parameter = para;
        // This exact image was admitted above against its resolved scope.
        wf.source_image_ = ::std::move(image);

        // verbose
        if(::uwvm2::uwvm::io::show_verbose) [[unlikely]]
        {
            ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                u8"uwvm: ",
                                details::diagnostic_color(UWVM_COLOR_U8_LT_GREEN),
                                u8"[info]  ",
                                details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"Loading WASM File \"",
                                details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                load_file_name,
                                details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"\". ",
                                details::diagnostic_color(UWVM_COLOR_U8_GREEN),
                                u8"[",
                                ::uwvm2::uwvm::io::get_local_realtime(),
                                u8"] ",
                                details::diagnostic_color(UWVM_COLOR_U8_ORANGE),
                                u8"(verbose)\n",
                                details::diagnostic_color(UWVM_COLOR_U8_RST_ALL));
        }

        ::fast_io::unix_timestamp start_time{};
        if(::uwvm2::uwvm::io::show_verbose) [[unlikely]]
        {
#ifdef UWVM_CPP_EXCEPTIONS
            try
#endif
            {
                start_time = ::fast_io::posix_clock_gettime(::fast_io::posix_clock_id::monotonic_raw);
            }
#ifdef UWVM_CPP_EXCEPTIONS
            catch(::fast_io::error)
            {
                // do nothing
            }
#endif
        }

        if(!wf.has_owned_source_image())
        {
        auto open = [&](auto&&... path)
        {
            ::fast_io::native_file file{::std::forward<decltype(path)>(path)...,
                ::fast_io::open_mode::in | ::fast_io::open_mode::follow | ::fast_io::open_mode::no_write_attributes};
            return wf.wasm_file.load(file, input.initial_limit(),
                [&](::std::size_t extent, auto& read_at) { return input.probe(extent, read_at); });
        };
#if defined(_WIN32) && !defined(_WIN32_WINDOWS)
        if(load_file_name.starts_with(u8"::NT::"))
        {
            // nt path
            ::fast_io::u8cstring_view const load_file_name_nt_subview{::fast_io::containers::null_terminated, load_file_name.subview(6uz)};

            if(::uwvm2::uwvm::io::show_nt_path_warning)
            {
                // Output the main information and memory indication
                ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                    // 1
                                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                    u8"uwvm: ",
                                    details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                    u8"[warn]  ",
                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                    u8"Resolve to NT path: \"",
                                    details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                    load_file_name_nt_subview,
                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                    u8"\".",
                                    details::diagnostic_color(UWVM_COLOR_U8_ORANGE),
                                    u8" (nt-path)\n",
                                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL));

                if(::uwvm2::uwvm::io::nt_path_warning_fatal) [[unlikely]]
                {
                    ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                        details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                        u8"uwvm: ",
                                        details::diagnostic_color(UWVM_COLOR_U8_LT_RED),
                                        u8"[fatal] ",
                                        details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                        u8"Convert warnings to fatal errors. ",
                                        details::diagnostic_color(UWVM_COLOR_U8_ORANGE),
                                        u8"(nt-path)\n\n",
                                        details::diagnostic_color(UWVM_COLOR_U8_RST_ALL));
                    ::uwvm2::uwvm::io::fast_exit(::uwvm2::uwvm::io::diagnostic_exit_status::invalid_option);
                }
            }

# ifdef UWVM_CPP_EXCEPTIONS
            try
# endif
            {
# ifdef UWVM_TIMER
                ::uwvm2::utils::debug::timer parsing_timer{u8"file loader"};
# endif

                // On platforms where CHAR_BIT is greater than 8, there is no need to clear the utf-8 non-low 8 bits here
                // allow symlink
                if(!open(::fast_io::io_kernel, load_file_name_nt_subview)) { return input_refusal(); }
            }
# ifdef UWVM_CPP_EXCEPTIONS
            catch(::fast_io::error e)
            {
                ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                    u8"uwvm: ",
                                    details::diagnostic_color(UWVM_COLOR_U8_RED),
                                    u8"[error] ",
                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                    u8"Unable to open WASM file \"",
                                    details::diagnostic_color(UWVM_COLOR_U8_CYAN),
                                    load_file_name_nt_subview,
                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                    u8"\": ",
                                    e,
                                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL),
                                    u8"\n");

                return load_wasm_file_rtl::load_error;
            }
# endif
        }
        else
        {
            // win32 path

# ifdef UWVM_CPP_EXCEPTIONS
            try
# endif
            {
# ifdef UWVM_TIMER
                ::uwvm2::utils::debug::timer parsing_timer{u8"file loader"};
# endif

                // On platforms where CHAR_BIT is greater than 8, there is no need to clear the utf-8 non-low 8 bits here
                // allow symlink
                if(!open(load_file_name)) { return input_refusal(); }
            }
# ifdef UWVM_CPP_EXCEPTIONS
            catch(::fast_io::error e)
            {
                ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                    u8"uwvm: ",
                                    details::diagnostic_color(UWVM_COLOR_U8_RED),
                                    u8"[error] ",
                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                    u8"Unable to open WASM file \"",
                                    details::diagnostic_color(UWVM_COLOR_U8_CYAN),
                                    load_file_name,
                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                    u8"\": ",
                                    e,
                                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL),
                                    u8"\n");

                return load_wasm_file_rtl::load_error;
            }
# endif
        }
#else
        // win9x and posix

# ifdef UWVM_CPP_EXCEPTIONS
        try
# endif
        {
# ifdef UWVM_TIMER
            ::uwvm2::utils::debug::timer parsing_timer{u8"file loader"};
# endif

            // On platforms where CHAR_BIT is greater than 8, there is no need to clear the utf-8 non-low 8 bits here
            // allow symlink
            if(!open(load_file_name)) { return input_refusal(); }
        }
# ifdef UWVM_CPP_EXCEPTIONS
        catch(::fast_io::error e)
        {
            ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                u8"uwvm: ",
                                details::diagnostic_color(UWVM_COLOR_U8_RED),
                                u8"[error] ",
                                details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"Unable to open WASM file \"",
                                details::diagnostic_color(UWVM_COLOR_U8_CYAN),
                                load_file_name,
                                details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"\": ",
                                e,
                                details::diagnostic_color(UWVM_COLOR_U8_RST_ALL),
                                u8"\n"
#  ifndef _WIN32  // Win32 automatically adds a newline (winnt and win9x)
                                u8"\n"
#  endif
            );

            return load_wasm_file_rtl::load_error;
        }
# endif
#endif

        }

#if CHAR_BIT > 8
        // Since files are either private page mapped or memory allocated and read, they can be modified directly.
        // Prevents invalid content in non-low eight bits of char_bit not equal to 8 from causing an unknown error in the parser.
        if(!wf.has_owned_source_image())
        {
        auto const wf_wasm_file_end{wf.wasm_file.end()};
        for(auto wf_wasm_file_curr{wf.wasm_file.begin()}; wf_wasm_file_curr != wf_wasm_file_end; ++wf_wasm_file_curr) { *wf_wasm_file_curr &= 0xFFu; }
        }
#endif

        if(!input.accepts(wf.source_cbegin(), wf.source_size())) { return input_refusal(); }

        // binfmt_ver has to be modified by the change_binfmt_ver function.
        wf.change_binfmt_ver(::uwvm2::parser::wasm::binfmt::detect_wasm_binfmt_version(reinterpret_cast<::std::byte const*>(wf.source_cbegin()),
                                                                                       reinterpret_cast<::std::byte const*>(wf.source_cend())));

        // verbose
        if(::uwvm2::uwvm::io::show_verbose) [[unlikely]]
        {
            ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                u8"uwvm: ",
                                details::diagnostic_color(UWVM_COLOR_U8_LT_GREEN),
                                u8"[info]  ",
                                details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"WebAssembly File \"",
                                details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                load_file_name,
                                details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"\"\'s Binary Format Version \"",
                                details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                wf.binfmt_ver,
                                details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"\" Detected. ",
                                details::diagnostic_color(UWVM_COLOR_U8_GREEN),
                                u8"[",
                                ::uwvm2::uwvm::io::get_local_realtime(),
                                u8"] ",
                                details::diagnostic_color(UWVM_COLOR_U8_ORANGE),
                                u8"(verbose)\n",
                                details::diagnostic_color(UWVM_COLOR_U8_RST_ALL));
        }

        // After detect
        // Instructs to read the file all the way into memory
        if(!wf.has_owned_source_image())
        { ::uwvm2::utils::madvise::my_madvise(wf.source_cbegin(), wf.source_size(), ::uwvm2::utils::madvise::madvise_flag::willneed); }

        switch(wf.binfmt_ver)
        {
            [[unlikely]] case static_cast<::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32>(0u):
            {
                // ...
                // unsafe

#ifndef UWVM_DISABLE_OUTPUT_WHEN_PARSE
                ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                    // 1
                                    u8"uwvm: ",
                                    details::diagnostic_color(UWVM_COLOR_U8_RED),
                                    u8"[error] ",
                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                    u8"(offset=",
                                    ::fast_io::mnp::addrvw(nullptr),
                                    u8") Illegal WebAssembly file format.\n"
                                    // 2
                                    u8"uwvm: ",
                                    details::diagnostic_color(UWVM_COLOR_U8_LT_GREEN),
                                    u8"[info]  ",
                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                    u8"Parser Memory Indication: ",
                                    ::uwvm2::uwvm::utils::memory::print_memory{reinterpret_cast<::std::byte const*>(wf.source_cbegin()),
                                                                               reinterpret_cast<::std::byte const*>(wf.source_cbegin()),
                                                                               reinterpret_cast<::std::byte const*>(wf.source_cend())},
                                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL),
                                    u8"\n\n");
#endif
                return load_wasm_file_rtl::wasm_parser_error;  // wasm parsing error
            }
            case static_cast<::uwvm2::parser::wasm::standard::wasm1::type::wasm_u32>(1u):
            {
                // handle exec (main) module
                {
                    ::uwvm2::utils::container::u8string_view policy_identity{};
                    if(para.resolve_module_policy)
                    {
                        namespace policy = ::uwvm2::uwvm::wasm::storage::module_policy;
                        policy_identity = rename_module_name;
                        if(policy_identity.empty() && (!policy::singles.empty() || !policy::membership.empty()))
                        {
                            policy_identity = source_input::policy_module_name(reinterpret_cast<char8_t const*>(wf.source_cbegin()),
                                reinterpret_cast<char8_t const*>(wf.source_cend()), load_file_name);
                        }
                        policy::resolve(policy_identity, wf.wasm_parameter);
                    }

                    // storage wasm err
                    ::uwvm2::parser::wasm::base::error_impl execute_wasm_binfmt_ver1_storage_wasm_err{};

                    // parse wasm 1

                    // verbose
                    if(::uwvm2::uwvm::io::show_verbose) [[unlikely]]
                    {
                        ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                            details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                            u8"uwvm: ",
                                            details::diagnostic_color(UWVM_COLOR_U8_LT_GREEN),
                                            u8"[info]  ",
                                            details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                            u8"Parsing WebAssembly file \"",
                                            details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                            load_file_name,
                                            details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                            u8"\". ",
                                            details::diagnostic_color(UWVM_COLOR_U8_GREEN),
                                            u8"[",
                                            ::uwvm2::uwvm::io::get_local_realtime(),
                                            u8"] ",
                                            details::diagnostic_color(UWVM_COLOR_U8_ORANGE),
                                            u8"(verbose)\n",
                                            details::diagnostic_color(UWVM_COLOR_U8_RST_ALL));
                    }

#if defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
                    try
#endif
                    {
                        // parser
#ifdef UWVM_TIMER
                        ::uwvm2::utils::debug::timer parsing_timer{u8"parse binfmt ver1"};
#endif

                        ::fast_io::unix_timestamp parser_start_time{};
                        if(::uwvm2::uwvm::io::show_verbose) [[unlikely]]
                        {
#ifdef UWVM_CPP_EXCEPTIONS
                            try
#endif
                            {
                                parser_start_time = ::fast_io::posix_clock_gettime(::fast_io::posix_clock_id::monotonic_raw);
                            }
#ifdef UWVM_CPP_EXCEPTIONS
                            catch(::fast_io::error)
                            {
                                // do nothing
                            }
#endif
                        }

                        wf.wasm_module_storage.wasm_binfmt_ver1_storage =
                            ::uwvm2::uwvm::wasm::feature::binfmt_ver1_handler(reinterpret_cast<::std::byte const*>(wf.source_cbegin()),
                                                                              reinterpret_cast<::std::byte const*>(wf.source_cend()),
                                                                              execute_wasm_binfmt_ver1_storage_wasm_err,
                                                                              wf.wasm_parameter.binfmt1_para);

                        ::fast_io::unix_timestamp parser_end_time{};
                        if(::uwvm2::uwvm::io::show_verbose) [[unlikely]]
                        {
#ifdef UWVM_CPP_EXCEPTIONS
                            try
#endif
                            {
                                parser_end_time = ::fast_io::posix_clock_gettime(::fast_io::posix_clock_id::monotonic_raw);
                            }
#ifdef UWVM_CPP_EXCEPTIONS
                            catch(::fast_io::error)
                            {
                                // do nothing
                            }
#endif

                            // verbose
                            ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                                details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                                u8"uwvm: ",
                                                details::diagnostic_color(UWVM_COLOR_U8_LT_GREEN),
                                                u8"[info]  ",
                                                details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                u8"Parse wasm file \"",
                                                details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                                load_file_name,
                                                details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                u8"\" done. (time=",
                                                details::diagnostic_color(UWVM_COLOR_U8_GREEN),
                                                parser_end_time - parser_start_time,
                                                details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                u8"s). ",
                                                details::diagnostic_color(UWVM_COLOR_U8_ORANGE),
                                                u8"(verbose)\n",
                                                details::diagnostic_color(UWVM_COLOR_U8_RST_ALL));
                        }
                    }
#if defined(UWVM_CPP_EXCEPTIONS) && !defined(UWVM_TERMINATE_IMME_WHEN_PARSE)
                    catch(::fast_io::error)
                    {
# ifndef UWVM_DISABLE_OUTPUT_WHEN_PARSE

                        // catch fast_io::error (wasm parser error)
#  if (defined(_DEBUG) || defined(DEBUG)) && defined(UWVM_ENABLE_DETAILED_DEBUG_CHECK)
                        if(execute_wasm_binfmt_ver1_storage_wasm_err.err_code == ::uwvm2::parser::wasm::base::wasm_parse_error_code::ok) [[unlikely]]
                        {
                            // The `ok` exception was thrown. It's a bug in the program.
                            ::uwvm2::utils::debug::trap_and_inform_bug_pos();
                        }
#  endif

                        // default print_memory
                        ::uwvm2::uwvm::utils::memory::print_memory const memory_printer{reinterpret_cast<::std::byte const*>(wf.source_cbegin()),
                                                                                        execute_wasm_binfmt_ver1_storage_wasm_err.err_curr,
                                                                                        reinterpret_cast<::std::byte const*>(wf.source_cend())};

                        // set errout
                        ::uwvm2::parser::wasm::base::error_output_t errout;
                        errout.module_begin = reinterpret_cast<::std::byte const*>(wf.source_cbegin());
                        errout.err = execute_wasm_binfmt_ver1_storage_wasm_err;
                        errout.flag.enable_ansi = static_cast<::std::uint_least8_t>(::uwvm2::uwvm::utils::ansies::put_color);
#  if defined(_WIN32) && (_WIN32_WINNT < 0x0A00 || defined(_WIN32_WINDOWS))
                        errout.flag.win32_use_text_attr = static_cast<::std::uint_least8_t>(!::uwvm2::uwvm::utils::ansies::log_win32_use_ansi_b);
#  endif

                        // Output the main information and memory indication
                        ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                            // 1
                                            details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                            u8"uwvm: ",
                                            details::diagnostic_color(UWVM_COLOR_U8_RED),
                                            u8"[error] ",
                                            details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                            u8"Parsing error in WebAssembly File \"",
                                            details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                            load_file_name,
                                            details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                            u8"\".\n",
                                            // 2
                                            errout,
                                            details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                            u8"\n"
                                            // 3
                                            u8"uwvm: ",
                                            details::diagnostic_color(UWVM_COLOR_U8_LT_GREEN),
                                            u8"[info]  ",
                                            details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                            u8"Parser Memory Indication: ",
                                            memory_printer,
                                            details::diagnostic_color(UWVM_COLOR_U8_RST_ALL),
                                            u8"\n\n");
# endif

                        return load_wasm_file_rtl::wasm_parser_error;
                    }
#endif

                    // verbose
                    if(::uwvm2::uwvm::io::show_verbose) [[unlikely]]
                    {
                        ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                            details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                            u8"uwvm: ",
                                            details::diagnostic_color(UWVM_COLOR_U8_LT_GREEN),
                                            u8"[info]  ",
                                            details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                            u8"Parsing the custom section of the wasm file \"",
                                            details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                            load_file_name,
                                            details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                            u8"\". ",
                                            details::diagnostic_color(UWVM_COLOR_U8_GREEN),
                                            u8"[",
                                            ::uwvm2::uwvm::io::get_local_realtime(),
                                            u8"] ",
                                            details::diagnostic_color(UWVM_COLOR_U8_ORANGE),
                                            u8"(verbose)\n",
                                            details::diagnostic_color(UWVM_COLOR_U8_RST_ALL));
                    }

                    // handle custom section timer
                    ::fast_io::unix_timestamp custom_section_start_time{};
                    if(::uwvm2::uwvm::io::show_verbose) [[unlikely]]
                    {
#ifdef UWVM_CPP_EXCEPTIONS
                        try
#endif
                        {
                            custom_section_start_time = ::fast_io::posix_clock_gettime(::fast_io::posix_clock_id::monotonic_raw);
                        }
#ifdef UWVM_CPP_EXCEPTIONS
                        catch(::fast_io::error)
                        {
                            // do nothing
                        }
#endif
                    }

                    // handle custom section
                    if(!details::admit_memtag_globals(wf.wasm_module_storage.wasm_binfmt_ver1_storage,
                        ::uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_wasm1p1_parameter(wf.wasm_parameter.binfmt1_para)))
                    {
                        ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                            u8"uwvm: [error] Invalid or disabled WasmMTE global metadata in ", load_file_name,
                            u8". Use uwvm.memtag.linear.globals version 2 with admitted ADM-tagged memory indices.");
                        return load_wasm_file_rtl::wasm_parser_error;
                    }
                    ::uwvm2::uwvm::wasm::custom::handle_binfmtver1_custom_section(wf, ::uwvm2::uwvm::wasm::custom::custom_handle_funcs);

                    ::fast_io::unix_timestamp custom_section_end_time{};
                    if(::uwvm2::uwvm::io::show_verbose) [[unlikely]]
                    {
#ifdef UWVM_CPP_EXCEPTIONS
                        try
#endif
                        {
                            custom_section_end_time = ::fast_io::posix_clock_gettime(::fast_io::posix_clock_id::monotonic_raw);
                        }
#ifdef UWVM_CPP_EXCEPTIONS
                        catch(::fast_io::error)
                        {
                            // do nothing
                        }
#endif

                        // verbose
                        ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                            details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                            u8"uwvm: ",
                                            details::diagnostic_color(UWVM_COLOR_U8_LT_GREEN),
                                            u8"[info]  ",
                                            details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                            u8"Parse custom section of wasm file \"",
                                            details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                            load_file_name,
                                            details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                            u8"\" done. (time=",
                                            details::diagnostic_color(UWVM_COLOR_U8_GREEN),
                                            custom_section_end_time - custom_section_start_time,
                                            details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                            u8"s). ",
                                            details::diagnostic_color(UWVM_COLOR_U8_ORANGE),
                                            u8"(verbose)\n",
                                            details::diagnostic_color(UWVM_COLOR_U8_RST_ALL));
                    }

                    // handle overall module name

                    // 1st: para --wasm-set-main-module-name
                    // 2st: custom section "name": module name
                    // 3st: file path

                    auto check_module_name{
                        [load_file_name](::uwvm2::utils::container::u8string_view module_name) constexpr noexcept -> bool
                        {
                            // wasm1.0: module name may be any byte sequence; emit warning when non-utf8 or contains NUL, but do not fail
                            // check utf8 (warning only)
                            auto const [utf8pos, utf8err]{
                                ::uwvm2::uwvm::wasm::feature::handle_text_format(::uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_text_format_wapper,
                                                                                 module_name.cbegin(),
                                                                                 module_name.cend())};

                            if(utf8err != ::uwvm2::utils::utf::utf_error_code::success) [[unlikely]]
                            {
#ifndef UWVM_DISABLE_OUTPUT_WHEN_PARSE

                                // default print_memory
                                ::uwvm2::uwvm::utils::memory::print_memory const memory_printer{reinterpret_cast<::std::byte const*>(module_name.cbegin()),
                                                                                                reinterpret_cast<::std::byte const*>(utf8pos),
                                                                                                reinterpret_cast<::std::byte const*>(module_name.cend())};

                                // Output the main information and memory indication
                                ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                                    // 1
                                                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                                    u8"uwvm: ",
                                                    details::diagnostic_color(UWVM_COLOR_U8_RED),
                                                    u8"[error] ",
                                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                    u8"Parsing error in WebAssembly File \"",
                                                    details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                                    load_file_name,
                                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                    u8"\".\n"
                                                    // 2
                                                    u8"uwvm: ",
                                                    details::diagnostic_color(UWVM_COLOR_U8_RED),
                                                    u8"[error] ",
                                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                    u8"(offset=",
                                                    ::fast_io::mnp::addrvw(utf8pos - module_name.cbegin()),
                                                    u8") Module Name Is Invalid Character Sequence. Reason: \"",
                                                    details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                                    ::uwvm2::utils::utf::get_utf_error_description<char8_t>(utf8err),
                                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                    u8"\".\n"
                                                    // 3
                                                    u8"uwvm: ",
                                                    details::diagnostic_color(UWVM_COLOR_U8_LT_GREEN),
                                                    u8"[info]  ",
                                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                    u8"Parser Memory Indication: ",
                                                    memory_printer,
                                                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL),
                                                    u8"\n\n");
#endif

                                return false;
                            }

#ifndef UWVM_DISABLE_OUTPUT_WHEN_PARSE
                            if(::uwvm2::uwvm::io::show_parser_warning)
                            {
                                auto const [utf8pos, utf8err]{::uwvm2::uwvm::wasm::feature::warn_unchecked_text_format_error(
                                    ::uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_text_format_wapper,
                                    module_name.cbegin(),
                                    module_name.cend())};

                                if(utf8err != ::uwvm2::utils::utf::utf_error_code::success) [[unlikely]]
                                {
                                    // default print_memory
                                    ::uwvm2::uwvm::utils::memory::print_memory const memory_printer{reinterpret_cast<::std::byte const*>(module_name.cbegin()),
                                                                                                    reinterpret_cast<::std::byte const*>(utf8pos),
                                                                                                    reinterpret_cast<::std::byte const*>(module_name.cend())};

                                    // Output the main information and memory indication
                                    ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                                        // 1
                                                        details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                                        u8"uwvm: ",
                                                        details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                                        u8"[warn]  ",
                                                        details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                        u8"(offset=",
                                                        ::fast_io::mnp::addrvw(utf8pos - module_name.cbegin()),
                                                        u8") Module Name contains characters that are not recommended. Details: \"",
                                                        details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                                        ::uwvm2::utils::utf::get_utf_error_description<char8_t>(utf8err),
                                                        details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                        u8"\". ",
                                                        details::diagnostic_color(UWVM_COLOR_U8_ORANGE),
                                                        u8"(parser)\n",
                                                        details::diagnostic_color(UWVM_COLOR_U8_RST_ALL),
                                                        u8"\n"
                                                        // 2
                                                        u8"uwvm: ",
                                                        details::diagnostic_color(UWVM_COLOR_U8_LT_GREEN),
                                                        u8"[info]  ",
                                                        details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                        u8"Parser Memory Indication: ",
                                                        memory_printer,
                                                        details::diagnostic_color(UWVM_COLOR_U8_RST_ALL),
                                                        u8"\n");

                                    if(::uwvm2::uwvm::io::parser_warning_fatal) [[unlikely]]
                                    {
                                        ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                                            details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                                            u8"uwvm: ",
                                                            details::diagnostic_color(UWVM_COLOR_U8_LT_RED),
                                                            u8"[fatal] ",
                                                            details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                            u8"Convert warnings to fatal errors. ",
                                                            details::diagnostic_color(UWVM_COLOR_U8_ORANGE),
                                                            u8"(parser)\n\n",
                                                            details::diagnostic_color(UWVM_COLOR_U8_RST_ALL));
                                        ::uwvm2::uwvm::io::fast_exit(::uwvm2::uwvm::io::diagnostic_exit_status::invalid_option);
                                    }
                                }
                            }
#endif

                            return true;
                        }};

                    // The decision-making method for module_name is not affected by disable_zero_length_string.
                    if(rename_module_name.empty())
                    {
                        if(wf.wasm_custom_name.module_name.empty())
                        {
                            wf.module_name = ::uwvm2::utils::container::u8string_view{load_file_name};
                            if(!check_module_name(wf.module_name)) [[unlikely]] { return load_wasm_file_rtl::wasm_parser_error; }
                        }
                        else
                        {
                            wf.module_name = wf.wasm_custom_name.module_name;
                            // The custom name is guaranteed to be a legal utf8 sequence.
                        }
                    }
                    else
                    {
                        wf.module_name = rename_module_name;
                        if(!check_module_name(wf.module_name)) [[unlikely]] { return load_wasm_file_rtl::wasm_parser_error; }
                    }

                    if(!policy_identity.empty() && wf.module_name != policy_identity)
                    {
                        ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output,
                            u8"uwvm: [error] Ambiguous Wasm policy identity. Supply an explicit module name for ", load_file_name);
                        return load_wasm_file_rtl::wasm_parser_error;
                    }

                    // Check whether the final result is an empty module. Whether this is an error is determined by conceptual manipulation.
                    if(wf.module_name.empty()) [[unlikely]]
                    {
                        constexpr auto get_disable_zero_length_string_from_tuple{
                            []<::uwvm2::parser::wasm::concepts::wasm_feature... Fs>(::uwvm2::utils::container::tuple<Fs...>) consteval noexcept -> bool
                            { return ::uwvm2::parser::wasm::standard::wasm1::features::disable_zero_length_string<Fs...>(); }};

                        constexpr bool disable_zero_length_string{
                            get_disable_zero_length_string_from_tuple(::uwvm2::uwvm::wasm::feature::wasm_binfmt1_features)};

                        if constexpr(disable_zero_length_string)
                        {
#ifndef UWVM_DISABLE_OUTPUT_WHEN_PARSE
                            ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                                // 1
                                                details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                                u8"uwvm: ",
                                                details::diagnostic_color(UWVM_COLOR_U8_RED),
                                                u8"[error] ",
                                                details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                u8"Parsing error in WebAssembly File \"",
                                                details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                                load_file_name,
                                                details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                u8"\".\n"
                                                // 2
                                                u8"uwvm: ",
                                                details::diagnostic_color(UWVM_COLOR_U8_RED),
                                                u8"[error] ",
                                                details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                u8"The Overall WebAssembly Module Name Length Cannot Be 0.\n\n",
                                                details::diagnostic_color(UWVM_COLOR_U8_RST_ALL));
#endif
                            return load_wasm_file_rtl::wasm_parser_error;
                        }
                        else
                        {
#ifndef UWVM_DISABLE_OUTPUT_WHEN_PARSE
                            if(::uwvm2::uwvm::io::show_parser_warning)
                            {
                                ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                                    // 1
                                                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                                    u8"uwvm: ",
                                                    details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                                    u8"[warn]  ",
                                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                    u8"Module name is empty (zero length). ",
                                                    details::diagnostic_color(UWVM_COLOR_U8_ORANGE),
                                                    u8"(parser)\n",
                                                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL));

                                if(::uwvm2::uwvm::io::parser_warning_fatal) [[unlikely]]
                                {
                                    ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                                        details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                                        u8"uwvm: ",
                                                        details::diagnostic_color(UWVM_COLOR_U8_LT_RED),
                                                        u8"[fatal] ",
                                                        details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                                        u8"Convert warnings to fatal errors. ",
                                                        details::diagnostic_color(UWVM_COLOR_U8_ORANGE),
                                                        u8"(parser)\n\n",
                                                        details::diagnostic_color(UWVM_COLOR_U8_RST_ALL));
                                    ::uwvm2::uwvm::io::fast_exit(::uwvm2::uwvm::io::diagnostic_exit_status::invalid_option);
                                }
                            }
#endif
                        }
                    }

                    // verbose
                    if(::uwvm2::uwvm::io::show_verbose) [[unlikely]]
                    {
                        ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                            details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                            u8"uwvm: ",
                                            details::diagnostic_color(UWVM_COLOR_U8_LT_GREEN),
                                            u8"[info]  ",
                                            details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                            u8"Overall WebAssembly Module Name \"",
                                            details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                            wf.module_name,
                                            details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                            u8"\". ",
                                            details::diagnostic_color(UWVM_COLOR_U8_GREEN),
                                            u8"[",
                                            ::uwvm2::uwvm::io::get_local_realtime(),
                                            u8"] ",
                                            details::diagnostic_color(UWVM_COLOR_U8_ORANGE),
                                            u8"(verbose)\n",
                                            details::diagnostic_color(UWVM_COLOR_U8_RST_ALL));
                    }
                }

                break;
            }
            /// @todo wasm component module: 0x0001000d
            [[unlikely]] default:
            {
                // The detector reports an unknown nonzero version only after
                // the complete eight-byte header, but independently bound the
                // diagnostic cursor BEFORE forming source_begin+4 below.
                if(wf.source_size() < 8uz * sizeof(char8_t)) { return load_wasm_file_rtl::wasm_parser_error; }
                // [owned source 0..4..8 ... source_size] end
                // [safe source_size>=8*sizeof(char8_t)] ^^ diagnostic offset4
                // is in the SAME pinned mapping/private image; never a guest ptr.
                // [\0 a s m ?? ?? ?? ??]
                // [        safe        ]

#ifndef UWVM_DISABLE_OUTPUT_WHEN_PARSE
                ::fast_io::io::perr(
                    ::uwvm2::uwvm::io::u8log_output,
                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                    // 1
                    u8"uwvm: ",
                    details::diagnostic_color(UWVM_COLOR_U8_RED),
                    u8"[error] ",
                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                    u8"(offset=",
                    ::fast_io::mnp::addrvw(4uz),
                    u8") Unknown Binary Format Version of WebAssembly: \"",
                    details::diagnostic_color(UWVM_COLOR_U8_CYAN),
                    wf.binfmt_ver,
                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                    u8"\".\n"
                    // 2
                    u8"uwvm: ",
                    details::diagnostic_color(UWVM_COLOR_U8_LT_GREEN),
                    u8"[info]  ",
                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                    u8"Parser Memory Indication: ",
                    ::uwvm2::uwvm::utils::memory::print_memory{reinterpret_cast<::std::byte const*>(wf.source_cbegin()),
                                                               reinterpret_cast<::std::byte const*>(wf.source_cbegin()) + 4uz * sizeof(char8_t),
                                                               reinterpret_cast<::std::byte const*>(wf.source_cend())},
                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL),
                    u8"\n\n");
#endif

                return load_wasm_file_rtl::wasm_parser_error;
            }
        }

// Output warnings in the wasm module
#ifndef UWVM_DISABLE_OUTPUT_WHEN_PARSE
        if(::uwvm2::uwvm::io::show_parser_warning)
        {
            // verbose
            if(::uwvm2::uwvm::io::show_verbose) [[unlikely]]
            {
                ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                    u8"uwvm: ",
                                    details::diagnostic_color(UWVM_COLOR_U8_LT_GREEN),
                                    u8"[info]  ",
                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                    u8"Check some things that are allowed but not recommended in the WebAssembly specification. ",
                                    details::diagnostic_color(UWVM_COLOR_U8_GREEN),
                                    u8"[",
                                    ::uwvm2::uwvm::io::get_local_realtime(),
                                    u8"] ",
                                    details::diagnostic_color(UWVM_COLOR_U8_ORANGE),
                                    u8"(verbose)\n",
                                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL));
            }

            // timer
            ::fast_io::unix_timestamp start_time{};
            if(::uwvm2::uwvm::io::show_verbose) [[unlikely]]
            {
# ifdef UWVM_CPP_EXCEPTIONS
                try
# endif
                {
                    start_time = ::fast_io::posix_clock_gettime(::fast_io::posix_clock_id::monotonic_raw);
                }
# ifdef UWVM_CPP_EXCEPTIONS
                catch(::fast_io::error)
                {
                    // do nothing
                }
# endif
            }

            // warning
            ::uwvm2::uwvm::wasm::warning::show_wasm_binfmt_ver1_warning(wf);

            // finished
            ::fast_io::unix_timestamp end_time{};
            if(::uwvm2::uwvm::io::show_verbose) [[unlikely]]
            {
# ifdef UWVM_CPP_EXCEPTIONS
                try
# endif
                {
                    end_time = ::fast_io::posix_clock_gettime(::fast_io::posix_clock_id::monotonic_raw);
                }
# ifdef UWVM_CPP_EXCEPTIONS
                catch(::fast_io::error)
                {
                    // do nothing
                }
# endif

                // verbose
                ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                    u8"uwvm: ",
                                    details::diagnostic_color(UWVM_COLOR_U8_LT_GREEN),
                                    u8"[info]  ",
                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                    u8"The review of items permitted has been completed. (time=",
                                    details::diagnostic_color(UWVM_COLOR_U8_GREEN),
                                    end_time - start_time,
                                    details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                    u8"s). ",
                                    details::diagnostic_color(UWVM_COLOR_U8_ORANGE),
                                    u8"(verbose)\n",
                                    details::diagnostic_color(UWVM_COLOR_U8_RST_ALL));
            }
        }
#endif

        ::fast_io::unix_timestamp end_time{};
        if(::uwvm2::uwvm::io::show_verbose) [[unlikely]]
        {
#ifdef UWVM_CPP_EXCEPTIONS
            try
#endif
            {
                end_time = ::fast_io::posix_clock_gettime(::fast_io::posix_clock_id::monotonic_raw);
            }
#ifdef UWVM_CPP_EXCEPTIONS
            catch(::fast_io::error)
            {
                // do nothing
            }
#endif

            // verbose
            ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                details::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                u8"uwvm: ",
                                details::diagnostic_color(UWVM_COLOR_U8_LT_GREEN),
                                u8"[info]  ",
                                details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"Load wasm file \"",
                                details::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                load_file_name,
                                details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"\" done. (time=",
                                details::diagnostic_color(UWVM_COLOR_U8_GREEN),
                                end_time - start_time,
                                details::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"s). ",
                                details::diagnostic_color(UWVM_COLOR_U8_ORANGE),
                                u8"(verbose)\n",
                                details::diagnostic_color(UWVM_COLOR_U8_RST_ALL));
        }

        transaction.committed = true;
        return load_wasm_file_rtl::ok;
    }

    } // global C++ linkage: shared with the type module's private friend
}  // namespace uwvm2::uwvm::wasm::loader

#ifndef UWVM_MODULE
# include <uwvm2/uwvm/utils/ansies/uwvm_color_pop_macro.h>
# include <uwvm2/utils/macro/pop_macros.h>
#endif
