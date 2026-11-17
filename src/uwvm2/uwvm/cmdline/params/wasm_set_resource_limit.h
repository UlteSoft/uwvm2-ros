#pragma once
#ifndef UWVM_MODULE
#include <uwvm2/utils/cmdline/impl.h>
#endif
#ifndef UWVM_MODULE_EXPORT
#define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::cmdline::params
{
    namespace details
    {
#if defined(UWVM_MODULE)
        extern "C++"
#else
        inline constexpr
#endif
        ::uwvm2::utils::cmdline::parameter_return_type wasm_set_resource_limit_callback(
            ::uwvm2::utils::cmdline::parameter_parsing_results*,
            ::uwvm2::utils::cmdline::parameter_parsing_results*,
            ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept;
    }
    inline constexpr ::uwvm2::utils::cmdline::parameter wasm_set_resource_limit{
        .name{u8"--wasm-set-resource-limit"},
        .describe{u8"Set host allocation budgets before loading: parser (input-bytes, type-metadata-bytes, function-body-bytes, module-code-bytes), validation (single-bytes, live-bytes), initializer (allocation-bytes), runtime (gc/table/memory/memtag/auxiliary/threads-single-bytes or -live-bytes). Bytes; 0 refuses new allocation. Shared by actual process backings, independent of import permissions."},
        .usage{u8"<stage:str> <resource:str> <bytes:size_t>"},
        .handle = ::std::addressof(details::wasm_set_resource_limit_callback),
        .cate = ::uwvm2::utils::cmdline::categorization::wasm};
}
