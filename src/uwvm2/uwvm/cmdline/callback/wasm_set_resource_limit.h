#pragma once
#ifndef UWVM_MODULE
#include <limits>
#include <fast_io.h>
#include <uwvm2/utils/cmdline/impl.h>
#include <uwvm2/utils/allocator/resource_limit.h>
#include <uwvm2/utils/control/host_diagnostics.h>
#include <uwvm2/uwvm/io/impl.h>
#include <uwvm2/uwvm/cmdline/params/wasm_set_resource_limit.h>
#endif
#ifndef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::cmdline::params::details
{
#if defined(UWVM_MODULE)
    extern "C++"
#else
    inline constexpr
#endif
    ::uwvm2::utils::cmdline::parameter_return_type wasm_set_resource_limit_callback(
        ::uwvm2::utils::cmdline::parameter_parsing_results*,
        ::uwvm2::utils::cmdline::parameter_parsing_results* current,
        ::uwvm2::utils::cmdline::parameter_parsing_results* end) noexcept
    {
        using result=::uwvm2::utils::cmdline::parameter_return_type;
        auto error{[]() noexcept
        {
            // The ordinary host diagnostic sink preserves the selected log
            // destination without blocking on a full pipe or replacing 126.
            if(auto const sink{::uwvm2::utils::control::host_diagnostics::sink}; sink != nullptr)
            { sink("uwvm: [error] Invalid resource budget. Usage: --wasm-set-resource-limit <parser|validation|initializer|runtime> <resource> <bytes>. See --help and documents/runtime/resource-limits.md."); }
            return result::return_m1_imme;
        }};
        if(end-current < 4) { return error(); }
        for(unsigned i{1};i!=4;++i)
        { if(current[i].type!=::uwvm2::utils::cmdline::parameter_parsing_results_type::arg) { return error(); } }
        auto const stage{current[1].str}, name{current[2].str}, text{current[3].str};
        ::std::size_t value{};
        auto const parsed{::fast_io::parse_by_scan(text.cbegin(),text.cend(),value)};
        if(parsed.code!=::fast_io::parse_code::ok || parsed.iter!=text.cend() ||
           value>static_cast<::std::size_t>((::std::numeric_limits<::std::ptrdiff_t>::max)())) { return error(); }
        namespace resource=::uwvm2::utils::allocator::resource;
        ::std::size_t* setting{};
        if(stage==u8"parser")
        {
            if(name==u8"input-bytes") { setting=&resource::module_file_bytes; }
            else if(name==u8"type-metadata-bytes") { setting=&resource::parser_metadata_bytes; }
            else if(name==u8"function-body-bytes") { setting=&resource::function_body_bytes; }
            else if(name==u8"module-code-bytes") { setting=&resource::module_code_bytes; }
        }
        else if(stage==u8"validation")
        {
            auto& policy{resource::policies[static_cast<unsigned>(resource::pool::validation)]};
            if(name==u8"single-bytes") { setting=&policy.single; }
            else if(name==u8"live-bytes") { setting=&policy.live; }
        }
        else if(stage==u8"initializer")
        { if(name==u8"allocation-bytes") { setting=&resource::initializer_allocation_bytes; } }
        else if(stage==u8"runtime")
        {
            auto select{[&](resource::pool p,bool live) noexcept
            { auto& policy{resource::policies[static_cast<unsigned>(p)]}; setting=live?&policy.live:&policy.single; }};
            if(name==u8"gc-single-bytes")select(resource::pool::gc,false);
            else if(name==u8"gc-live-bytes")select(resource::pool::gc,true);
            else if(name==u8"table-single-bytes")select(resource::pool::table,false);
            else if(name==u8"table-live-bytes")select(resource::pool::table,true);
            else if(name==u8"memory-single-bytes")select(resource::pool::memory,false);
            else if(name==u8"memory-live-bytes")select(resource::pool::memory,true);
            else if(name==u8"memtag-single-bytes")select(resource::pool::memtag,false);
            else if(name==u8"memtag-live-bytes")select(resource::pool::memtag,true);
            else if(name==u8"auxiliary-single-bytes")select(resource::pool::auxiliary,false);
            else if(name==u8"auxiliary-live-bytes")select(resource::pool::auxiliary,true);
            else if(name==u8"threads-single-bytes")select(resource::pool::threads,false);
            else if(name==u8"threads-live-bytes")select(resource::pool::threads,true);
        }
        if(setting==nullptr) { return error(); }
        *setting=value;
        for(unsigned i{1};i!=4;++i)current[i].type=::uwvm2::utils::cmdline::parameter_parsing_results_type::occupied_arg;
        return result::def;
    }
}
