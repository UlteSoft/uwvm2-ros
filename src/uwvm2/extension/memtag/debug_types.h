// Host-only debugger copies. These selectors carry no guest or native authority.
#pragma once
#ifndef UWVM_MODULE
#include <cstddef>
#include <cstdint>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    enum class debug_tag_status : unsigned { unavailable, available, disabled, unsupported_memory, invalid_range };
    struct debug_tag_view
    {
        debug_tag_status status{debug_tag_status::unavailable};
        ::std::uint64_t memory_bytes{}, first_granule{};
        ::std::size_t count{};
        unsigned tag_bits{};
    };
}
