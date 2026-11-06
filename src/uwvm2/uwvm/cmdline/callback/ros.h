/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#if defined(UWVM_IMPORT_ROS1) || defined(UWVM_IMPORT_ROS2)
#ifndef UWVM_MODULE
# include <array>
#endif
#ifndef UWVM_MODULE
# include <string_view>
#endif
#ifndef UWVM_MODULE
# include <uwvm2/utils/cmdline/impl.h>
#endif
#ifndef UWVM_MODULE
# include <uwvm2/uwvm/io/impl.h>
#endif
#ifndef UWVM_MODULE
# include <uwvm2/uwvm/imported/ros/storage.h>
# include <uwvm2/imported/ros/common/buffer_audit.h>
# if defined(UWVM_IMPORT_ROS2)
#  include <uwvm2/imported/ros/portable/deployment.h>
#  include <uwvm2/imported/ros/portable/runtime_audit.h>
# endif
#endif
namespace uwvm2::uwvm::cmdline::params::details::ros_details
{
    namespace model = ::uwvm2::imported::ros;
    namespace storage = ::uwvm2::uwvm::imported::ros;
    enum class action { interface, create, add_module, bind_global_module, enable, disable, connection, grant, set, check, dump };
    using status = ::uwvm2::utils::cmdline::parameter_return_type;
    template<model::scope Scope>
    inline void set_pretreatment(char8_t const* const*& argv_curr,
                                char8_t const* const* argv_end,
                                ::uwvm2::utils::container::vector<::uwvm2::utils::cmdline::parameter_parsing_results>& pr) noexcept
    {
        auto const first{argv_curr + 1u};
        argv_curr = first;
        constexpr ::std::size_t leading{Scope == model::scope::global ? 2u : 3u};
        if(static_cast<::std::size_t>(argv_end - first) <= leading) { return; }
        ::std::array<::uwvm2::utils::container::u8cstring_view, leading> arguments{};
        for(::std::size_t i{}; i != leading; ++i)
        {
            if(first[i] == nullptr) { return; }
            arguments[i] = ::uwvm2::utils::container::u8cstring_view{::fast_io::mnp::os_c_str(first[i])};
            if(arguments[i].empty() || arguments[i].front_unchecked() == u8'-') { return; }
        }
        auto const key{arguments[leading-1u]};
        if(key != u8"parameter-min-value" && key != u8"parameter-max-value") { return; }
        if(first[leading] == nullptr) { return; }
        auto const value{::uwvm2::utils::container::u8cstring_view{::fast_io::mnp::os_c_str(first[leading])}};
        if(value.size() < 2u || value.front_unchecked() != u8'-') { return; }
        auto const second{value.index_unchecked(1u)};
        auto matches_special = [&value](::uwvm2::utils::container::u8string_view word) noexcept
        {
            if(value.size() != word.size() + 1u) { return false; }
            for(::std::size_t i{}; i != word.size(); ++i)
            {
                auto c{value.index_unchecked(i+1u)};
                if(c >= u8'A' && c <= u8'Z') { c = static_cast<char8_t>(c + (u8'a' - u8'A')); }
                if(c != word.index_unchecked(i)) { return false; }
            }
            return true;
        };
        if(!((second >= u8'0' && second <= u8'9') || second == u8'.' ||
             matches_special(u8"inf") || matches_special(u8"infinity") || matches_special(u8"nan"))) { return; }
        // Classify only this signed numeric-bound value and its preceding plain
        // arguments. The normal ROS callback still validates and consumes them;
        // option-looking values such as --help retain ordinary parser handling.
        for(auto argument : arguments)
        { pr.emplace_back_unchecked(argument, nullptr, ::uwvm2::utils::cmdline::parameter_parsing_results_type::arg); }
        pr.emplace_back_unchecked(value, nullptr, ::uwvm2::utils::cmdline::parameter_parsing_results_type::arg);
        argv_curr = first + leading + 1u;
    }
    inline status error(::uwvm2::utils::cmdline::parameter const& parameter, ::std::string_view detail) noexcept
    {
        ::fast_io::io::perrln(::uwvm2::uwvm::io::u8log_output, u8"uwvm: [error] ROS policy: ",
            ::uwvm2::utils::container::u8string_view{reinterpret_cast<char8_t const*>(detail.data()), detail.size()},
            u8". Usage: ", ::uwvm2::utils::cmdline::print_usage(parameter));
        return status::return_m1_imme;
    }
#if defined(UWVM_IMPORT_ROS2)
    inline status buffer_audit_limits_configuration(::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current,
        ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    {
        if(end-current<=6){return error(*current->para,"buffer journal limits require four decimal quotas, a profile SHA256, then --ros2-buffer-audit");}
        ::std::array<::std::uint64_t,4> values{};
        for(::std::size_t i{};i!=5;++i)
        {
            if(current[i+1].type!=::uwvm2::utils::cmdline::parameter_parsing_results_type::arg)
            {return error(*current->para,"missing explicit buffer journal limit/profile argument");}
            if(i==4){break;}
            auto text{current[i+1].str};auto first{reinterpret_cast<char const*>(text.data())};
            auto parsed{::std::from_chars(first,first+text.size(),values[i])};
            if(parsed.ec!=::std::errc{}||parsed.ptr!=first+text.size())
            {return error(*current->para,"buffer journal quota must be an unsigned decimal integer");}
        }
        auto next{current[6].str};auto profile{current[5].str};
        if(::std::string_view{reinterpret_cast<char const*>(next.data()),next.size()}!="--ros2-buffer-audit"||
           !model::buffer_audit::journal.configure_limits({values[0],values[1],values[2],values[3]},
               {reinterpret_cast<char const*>(profile.data()),profile.size()}))
        {return error(*current->para,"buffer journal limits/profile invalid, repeated, late, or not immediately followed by --ros2-buffer-audit");}
        for(::std::size_t i{1};i!=6;++i){current[i].type=::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg;}
        return status::def;
    }
    inline status buffer_audit_configuration(::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current,
        ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    {
        if(end-current<=2||current[1].type!=::uwvm2::utils::cmdline::parameter_parsing_results_type::arg||
           current[2].type!=::uwvm2::utils::cmdline::parameter_parsing_results_type::arg)
        {return error(*current->para,"buffer journal needs a run ID and an inherited private write-only empty regular-file descriptor");}
        auto a{current[1].str};auto b{current[2].str};unsigned descriptor{};
        auto begin{reinterpret_cast<char const*>(b.data())};auto parsed{::std::from_chars(begin,begin+b.size(),descriptor)};
        if(parsed.ec!=::std::errc{}||parsed.ptr!=begin+b.size()||!model::buffer_audit::journal.configure(
            {reinterpret_cast<char const*>(a.data()),a.size()},descriptor))
        {return error(*current->para,"buffer journal run ID/descriptor invalid, repeated, unavailable or unsupported");}
        current[1].type=current[2].type=::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg;return status::def;
    }
    inline status portable_audit_configuration(::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current,
        ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    {
        if(end-current<=1||current[1].type!=::uwvm2::utils::cmdline::parameter_parsing_results_type::arg)
        { return error(*current->para,"portable audit needs one nonzero 32-character lowercase hexadecimal run ID"); }
        auto const text{current[1].str};
        if(!model::portable::runtime_audit::runtime.configure({reinterpret_cast<char const*>(text.data()),text.size()}))
        { return error(*current->para,"portable audit run ID is invalid, repeated, or unsupported on this platform"); }
        current[1].type=::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg;return status::def;
    }
    inline status portable_audit_inputs_configuration(::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current,
        ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    {
#if defined(UWVM_CPP_EXCEPTIONS)
        try
        {
#endif
            ::std::array<::std::string,2> arguments{};
            for(::std::size_t i{};i!=arguments.size();++i)
            {
                if(static_cast<::std::size_t>(end-current-1)<=i||current[i+1].type!=::uwvm2::utils::cmdline::parameter_parsing_results_type::arg)
                { return error(*current->para,"portable audit inputs need adapter and policy absolute paths"); }
                auto const text{current[i+1].str};arguments[i].assign(reinterpret_cast<char const*>(text.data()),text.size());
            }
            if(model::portable::runtime_audit::runtime.configure_inputs(arguments[0],arguments[1])!=model::portable::error::ok)
            { return error(*current->para,"portable observation inputs are repeated, unavailable or not bounded stable regular files"); }
            for(::std::size_t i{};i!=arguments.size();++i) { current[i+1].type=::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg; }
            return status::def;
#if defined(UWVM_CPP_EXCEPTIONS)
        }
        catch(...) { return error(*current->para,"unable to retain portable observation input snapshots"); }
#endif
    }
    inline status portable_configuration(::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current,
        ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    {
#if defined(UWVM_CPP_EXCEPTIONS)
        try
        {
#endif
            ::std::array<::std::string_view,4> arguments{};
            for(::std::size_t i{};i!=arguments.size();++i)
            {
                if(static_cast<::std::size_t>(end-current-1)<=i||current[i+1].type!=::uwvm2::utils::cmdline::parameter_parsing_results_type::arg)
                { return error(*current->para,"portable deployment needs adapter path/SHA and separate policy path/SHA"); }
                auto const text{current[i+1].str};arguments[i]={reinterpret_cast<char const*>(text.data()),text.size()};
            }
            auto result{model::portable::deployment_selection::configure(::std::string{arguments[0]},arguments[1],::std::string{arguments[2]},arguments[3])};
            if(result!=model::portable::error::ok) { return error(*current->para,"portable deployment bytes, schema, mapping or separate grant refused"); }
            for(::std::size_t i{};i!=arguments.size();++i) { current[i+1].type=::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg; }
            storage::policy_configured=true;return status::def;
#if defined(UWVM_CPP_EXCEPTIONS)
        }
        catch(...) { return error(*current->para,"unable to retain bounded portable deployment inputs"); }
#endif
    }
#endif
    template<model::version Version, model::scope Scope, action Action, ::std::size_t Count>
    inline status callback([[maybe_unused]] ::uwvm2::utils::cmdline::parameter_parsing_results* begin,
                           ::uwvm2::utils::cmdline::parameter_parsing_results* current,
                           ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    {
        ::std::array<::std::string_view, Count> args{};
        for(::std::size_t i{}; i != Count; ++i)
        {
            if(static_cast<::std::size_t>(end - current - 1) <= i ||
               (current[i+1].type != ::uwvm2::utils::cmdline::parameter_parsing_results_type::arg))
            { return error(*current->para, "missing explicit argument"); }
            auto const text{current[i+1].str};
            if(text.empty()) { return error(*current->para, "empty argument"); }
            args[i] = {reinterpret_cast<char const*>(text.data()), text.size()};
        }
        constexpr ::std::size_t selector{Scope == model::scope::global ? 0u : 1u};
        ::std::string_view name{};
        if constexpr(selector != 0) { name = args[0]; }
        model::result result{};
        if constexpr(Action == action::create) { result = storage::runtime_policy.create_target(Version, Scope, name); }
        else if constexpr(Action == action::bind_global_module) { result = storage::runtime_policy.bind_global_module(Version, args[0]); }
        else if constexpr(Action == action::add_module) { result = storage::runtime_policy.bind_module(Version, name, args[1]); }
        else if constexpr(Action == action::enable || Action == action::disable)
        {
            result = storage::runtime_policy.set_enabled(Version, Scope, name, Action == action::enable);
            if(result)
            {
                storage::policy_any_enabled = false;
                for(auto const* target : storage::runtime_policy.configured_targets())
                {
                    if(target->enabled) { storage::policy_any_enabled = true; break; }
                }
            }
        }
        else if constexpr(Action == action::interface)
        { result=storage::runtime_policy.set_interface(Version,Scope,name,args[selector],args[selector+1]); }
        else if constexpr(Action == action::connection)
        {
            auto const parameter_name{current->para->name};
            ::std::string_view const full{reinterpret_cast<char const*>(parameter_name.data()), parameter_name.size()};
            constexpr ::std::size_t prefix_size{Scope == model::scope::global ? 14u : Scope == model::scope::single ? 14u : 13u};
            result = storage::runtime_policy.set_connection(Version, Scope, name, full.substr(prefix_size), args[selector]);
        }
        else if constexpr(Action == action::grant)
        {
            model::capability_spec cap{};
            cap.alias = args[selector]; cap.operation = args[selector+1]; cap.resource = args[selector+2];
            cap.type = args[selector+3]; cap.type_identity = args[selector+4];
            result = storage::runtime_policy.grant(Version, Scope, name, ::std::move(cap));
        }
        else if constexpr(Action == action::set)
        { result = storage::runtime_policy.set_capability(Version, Scope, name, args[selector], args[selector+1], args[selector+2]); }
        else
        {
            constexpr auto index{static_cast<unsigned>(Version)-1u};
            auto& flag{Action == action::check ? storage::check_policy[index] : storage::dump_policy[index]};
            if(flag) { return error(*current->para, "duplicate diagnostic switch"); }
            flag = true;
        }
        if(!result) { return error(*current->para, result.detail); }
        for(::std::size_t i{}; i != Count; ++i)
        { current[i+1].type = ::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg; }
        storage::policy_configured = true;
        return status::def;
    }
}
#endif
