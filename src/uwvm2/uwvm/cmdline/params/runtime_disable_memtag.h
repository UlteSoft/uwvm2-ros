#pragma once
#ifndef UWVM_MODULE
# include <memory>
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/utils/cmdline/impl.h>
# include <uwvm2/uwvm/runtime/runtime_mode/impl.h>
# include <uwvm2/uwvm/wasm/storage/mode.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::cmdline::params
{
    inline constexpr ::uwvm2::utils::cmdline::parameter runtime_disable_memtag{
        .name{u8"--runtime-disable-memtag"},
        .describe{u8"Default WasmMTE access comparisons to off. Address masking, tag operations and sandbox bounds stay active."},
        .handle = [](::uwvm2::utils::cmdline::parameter_parsing_results*, ::uwvm2::utils::cmdline::parameter_parsing_results*,
                     ::uwvm2::utils::cmdline::parameter_parsing_results*) noexcept
        {
            ::uwvm2::uwvm::wasm::storage::wasm_parameter.memory_policy.check_tags = false;
            return ::uwvm2::utils::cmdline::parameter_return_type::def;
        },
        .is_exist = ::std::addressof(::uwvm2::uwvm::runtime::runtime_mode::runtime_disable_memtag),
        .cate = ::uwvm2::utils::cmdline::categorization::runtime};
}
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/pop_macros.h>
#endif
