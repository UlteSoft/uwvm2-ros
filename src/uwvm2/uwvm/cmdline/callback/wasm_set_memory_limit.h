/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @author      MacroModel
 * @version     2.0.0
 * @date        2026-04-23
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
# include <cstdint>
# include <limits>
# include <memory>
// macro
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/uwvm/utils/ansies/uwvm_color_push_macro.h>
// import
# include <fast_io.h>
# include <uwvm2/utils/container/impl.h>
# include <uwvm2/utils/ansies/impl.h>
# include <uwvm2/utils/cmdline/impl.h>
# include <uwvm2/utils/utf/impl.h>
# include <uwvm2/uwvm/io/impl.h>
# include <uwvm2/uwvm/utils/ansies/impl.h>
# include <uwvm2/uwvm/cmdline/impl.h>
# include <uwvm2/uwvm/cmdline/params/impl.h>
# include <uwvm2/uwvm/wasm/feature/impl.h>
# include <uwvm2/uwvm/wasm/type/impl.h>
# include <uwvm2/uwvm/wasm/storage/impl.h>
#endif

#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::uwvm::cmdline::params::details
{
#if defined(UWVM_MODULE)
    extern "C++"
#else
    inline constexpr
#endif
    ::uwvm2::utils::cmdline::parameter_return_type runtime_memory_reservation_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results* current,
        ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    {
        using namespace ::uwvm2::utils::cmdline;
        namespace storage = ::uwvm2::uwvm::wasm::storage;
        auto const invalid{[]() noexcept {
            ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output, u8"uwvm: [error] Invalid memory reservation. ",
                print_usage(::uwvm2::uwvm::cmdline::params::runtime_memory_reservation));
            return parameter_return_type::return_m1_imme;
        }};
        if(end - current < 3) { return invalid(); }
        auto const setting{current[1].str};
        bool const local{setting == u8"memory"};
        auto const count{local ? 4 : 2};
        if(end - current <= count) { return invalid(); }
        for(int i{1}; i <= count; ++i)
        { if(current[i].type != parameter_parsing_results_type::arg) { return invalid(); } }
        auto const number{[](auto text, auto& value) noexcept {
            if(text.empty()) { return false; }
            for(auto ch : text) { if(ch < u8'0' || ch > u8'9') { return false; } }
            auto const parsed{::fast_io::parse_by_scan(text.cbegin(), text.cend(), value)};
            return parsed.code == ::fast_io::parse_code::ok && parsed.iter == text.cend();
        }};
        // The inclusive process pool endpoint is one byte larger than UINT64_MAX.
        // Individual mapping lengths must still be representable on the host.
        auto const amount{current[count].str};
        bool const full_pool{setting == u8"pool" && ::std::numeric_limits<::std::uintptr_t>::digits == 64 &&
                             amount == u8"18446744073709551616"};
        ::std::uint_least64_t value{};
        if(!full_pool && (!number(amount, value) || value == 0u)) { return invalid(); }
        if(setting == u8"participants")
        {
            if(value > (::std::numeric_limits<::std::size_t>::max)()) { return invalid(); }
            storage::mmap_reservation_participants = static_cast<::std::size_t>(value);
        }
        else
        {
            if(value > storage::mmap_reservation_hard_limit_bytes) { return invalid(); }
            if(setting == u8"pool")
            {
                storage::mmap_reservation_pool_bytes = value;
                storage::mmap_reservation_pool_full_address_space = full_pool;
                storage::mmap_reservation_pool_specified = true;
            }
            else if(local)
            {
                auto const name{::uwvm2::utils::container::u8string_view{current[2].str}};
                ::std::size_t index{};
                if(name.empty() || !number(current[3].str, index)) { return invalid(); }
                using namespace ::uwvm2::uwvm::wasm::feature;
                auto const [name_end, name_error]{handle_text_format(wasm_binfmt_ver1_text_format_wapper, name.cbegin(), name.cend())};
                if(name_end != name.cend() || name_error != ::uwvm2::utils::utf::utf_error_code::success) { return invalid(); }
                storage::upsert_configured_module_memory_limit(name).local_defined_mmap_reservations.insert_or_assign(index, value);
            }
            else { return invalid(); }
        }
        for(int i{1}; i <= count; ++i) { current[i].type = parameter_parsing_results_type::occupied_arg; }
        return parameter_return_type::def;
    }

#if defined(UWVM_MODULE)
    extern "C++" UWVM_GNU_COLD
#else
    UWVM_GNU_COLD inline constexpr
#endif
        ::uwvm2::utils::cmdline::parameter_return_type wasm_set_memory_limit_callback([[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results *
                                                                                          para_begin,
                                                                                      ::uwvm2::utils::cmdline::parameter_parsing_results * para_curr,
                                                                                      ::uwvm2::utils::cmdline::parameter_parsing_results * para_end) noexcept
    {
        auto currp1{para_curr + 1u};
        auto currp2{para_curr + 2u};
        auto currp3{para_curr + 3u};

        if(currp1 == para_end || currp2 == para_end || currp3 == para_end || currp1->type != ::uwvm2::utils::cmdline::parameter_parsing_results_type::arg ||
           currp2->type != ::uwvm2::utils::cmdline::parameter_parsing_results_type::arg ||
           currp3->type != ::uwvm2::utils::cmdline::parameter_parsing_results_type::arg) [[unlikely]]
        {
            ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                u8"uwvm: ",
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_RED),
                                u8"[error] ",
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"Usage: ",
                                ::uwvm2::utils::cmdline::print_usage(::uwvm2::uwvm::cmdline::params::wasm_set_memory_limit),
                                u8"\n\n");
            return ::uwvm2::utils::cmdline::parameter_return_type::return_m1_imme;
        }

        auto const module_name{::uwvm2::utils::container::u8string_view{currp1->str}};
        auto const target_text{::uwvm2::utils::container::u8string_view{currp2->str}};
        if(module_name.empty()) [[unlikely]]
        {
            ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                u8"uwvm: ",
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_RED),
                                u8"[error] ",
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"Module name cannot be empty. Usage: ",
                                ::uwvm2::utils::cmdline::print_usage(::uwvm2::uwvm::cmdline::params::wasm_set_memory_limit),
                                u8"\n\n");
            return ::uwvm2::utils::cmdline::parameter_return_type::return_m1_imme;
        }

        // This option creates/updates configuration keyed by a Wasm module name,
        // so validate it here instead of deferring to a later loader path.
        if(auto const [module_name_pos,
                       module_name_err]{::uwvm2::uwvm::wasm::feature::handle_text_format(::uwvm2::uwvm::wasm::feature::wasm_binfmt_ver1_text_format_wapper,
                                                                                         module_name.cbegin(),
                                                                                         module_name.cend())};
           module_name_err != ::uwvm2::utils::utf::utf_error_code::success) [[unlikely]]
        {
            static_cast<void>(module_name_pos);
            ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                u8"uwvm: ",
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_RED),
                                u8"[error] ",
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"Invalid module name: \"",
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_CYAN),
                                module_name,
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"\". WebAssembly names must be valid UTF-8. Reason: \"",
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                ::uwvm2::utils::utf::get_utf_error_description<char8_t>(module_name_err),
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"\". Usage: ",
                                ::uwvm2::utils::cmdline::print_usage(::uwvm2::uwvm::cmdline::params::wasm_set_memory_limit),
                                u8"\n\n");
            return ::uwvm2::utils::cmdline::parameter_return_type::return_m1_imme;
        }

        bool apply_to_all{};
        ::std::size_t parsed_index{};
        if(target_text == u8"all") { apply_to_all = true; }
        else
        {
            auto const [next, err]{::fast_io::parse_by_scan(currp2->str.cbegin(), currp2->str.cend(), parsed_index)};
            if(err != ::fast_io::parse_code::ok || next != currp2->str.cend()) [[unlikely]]
            {
                ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                    ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                    u8"uwvm: ",
                                    ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_RED),
                                    u8"[error] ",
                                    ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                    u8"Invalid memory selector: \"",
                                    ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_CYAN),
                                    currp2->str,
                                    ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                    u8"\". Expected `all` or `<index:size_t>`. Usage: ",
                                    ::uwvm2::utils::cmdline::print_usage(::uwvm2::uwvm::cmdline::params::wasm_set_memory_limit),
                                    u8"\n\n");
                return ::uwvm2::utils::cmdline::parameter_return_type::return_m1_imme;
            }
        }

        ::std::size_t parsed_min{};
        if(auto const [next, err]{::fast_io::parse_by_scan(currp3->str.cbegin(), currp3->str.cend(), parsed_min)};
           err != ::fast_io::parse_code::ok || next != currp3->str.cend()) [[unlikely]]
        {
            ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                u8"uwvm: ",
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_RED),
                                u8"[error] ",
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"Invalid minimum memory limit: \"",
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_CYAN),
                                currp3->str,
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"\". Usage: ",
                                ::uwvm2::utils::cmdline::print_usage(::uwvm2::uwvm::cmdline::params::wasm_set_memory_limit),
                                u8"\n\n");
            return ::uwvm2::utils::cmdline::parameter_return_type::return_m1_imme;
        }

        currp1->type = ::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg;
        currp2->type = ::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg;
        currp3->type = ::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg;

        ::std::size_t parsed_max{};
        bool parsed_max_present{};
        if(auto currp4{para_curr + 4u}; currp4 != para_end && currp4->type == ::uwvm2::utils::cmdline::parameter_parsing_results_type::arg)
        {
            auto const [next, err]{::fast_io::parse_by_scan(currp4->str.cbegin(), currp4->str.cend(), parsed_max)};
            if(err == ::fast_io::parse_code::ok && next == currp4->str.cend())
            {
                currp4->type = ::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg;
                parsed_max_present = true;
            }
        }

        if(parsed_max_present && parsed_max < parsed_min) [[unlikely]]
        {
            ::fast_io::io::perr(::uwvm2::uwvm::io::u8log_output,
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_RST_ALL_AND_SET_WHITE),
                                u8"uwvm: ",
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_RED),
                                u8"[error] ",
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"Invalid memory limit override: max (",
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                parsed_max,
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8") is smaller than min (",
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_YELLOW),
                                parsed_min,
                                ::uwvm2::uwvm::utils::ansies::diagnostic_color(UWVM_COLOR_U8_WHITE),
                                u8"). Usage: ",
                                ::uwvm2::utils::cmdline::print_usage(::uwvm2::uwvm::cmdline::params::wasm_set_memory_limit),
                                u8"\n\n");
            return ::uwvm2::utils::cmdline::parameter_return_type::return_m1_imme;
        }

        auto& limit{::uwvm2::uwvm::wasm::storage::upsert_configured_module_memory_limit(module_name)};
        ::uwvm2::uwvm::wasm::type::module_memory_limit_t parsed_limit{.min = parsed_min,
                                                                      .max = parsed_max_present ? parsed_max
                                                                                                : ::uwvm2::uwvm::wasm::type::module_memory_limit_t::default_max,
                                                                      .present_max = parsed_max_present};

        using result = ::uwvm2::utils::cmdline::parameter_return_type;
        if(apply_to_all)
        {
            // The scoped options were collected before ordinary callbacks, in either CLI order.
            namespace policy = ::uwvm2::uwvm::wasm::storage::module_policy;
            auto const single{policy::singles.find(module_name)};
            if(!limit.apply_to_all_memories && (single == policy::singles.end() || !single->second.memory_pages))
            {
                limit.apply_to_all_memories = true;
                limit.all_limits = parsed_limit;
                return result::def;
            }
        }
        else if(limit.local_defined_memory_limits.try_emplace(parsed_index, parsed_limit).second) { return result::def; }

        ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output, u8"uwvm: [error] Duplicate memory page policy for module ",
            module_name, u8", local definition selector ", target_text, u8".");
        return result::return_m1_imme;
    }
}

#ifndef UWVM_MODULE
# include <uwvm2/uwvm/utils/ansies/uwvm_color_pop_macro.h>
# include <uwvm2/utils/macro/pop_macros.h>
#endif
