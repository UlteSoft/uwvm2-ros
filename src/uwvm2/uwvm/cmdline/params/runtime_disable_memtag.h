#pragma once
#ifndef UWVM_MODULE
# include <memory>
# include <uwvm2/utils/macro/push_macros.h>
# include <uwvm2/utils/cmdline/impl.h>
# include <uwvm2/uwvm/runtime/runtime_mode/impl.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::uwvm::cmdline::params
{
    inline constexpr ::uwvm2::utils::cmdline::parameter runtime_disable_memtag{
        .name{u8"--runtime-disable-memtag"},
        .describe{u8"Disable WasmMTE tag execution and checks while retaining memory-zeroing semantics; extension validation remains enabled."},
        .is_exist = ::std::addressof(::uwvm2::uwvm::runtime::runtime_mode::runtime_disable_memtag),
        .cate = ::uwvm2::utils::cmdline::categorization::runtime};
}
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/pop_macros.h>
#endif
