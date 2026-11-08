// Native-only delayed diagnostics; no guest import exposes this state.
#pragma once
#ifndef UWVM_MODULE
# include <cstddef>
# include <cstdint>
# include <uwvm2/extension/memtag/shadow.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::memtag
{
    struct host_fault
    {
        bool pending{};
        ::std::size_t memory_index{};
        ::std::uint64_t pointer{},granule{};
        ::std::uint32_t pointer_tag{},allocation_tag{};
    };
    inline thread_local host_fault pending_host_fault{};
    inline void record_host_fault(::std::size_t memory_index,::std::uint64_t raw_pointer,
        ::uwvm2::extension::memtag::access_result const& result) noexcept
    {
        if(result.error!=::uwvm2::extension::memtag::access_error::tag_mismatch || pending_host_fault.pending)
        { return; }
        // Retain the FIRST complete proof failure. A later output preflight
        // cannot replace it with a benign result or change the original tag.
        pending_host_fault={true,memory_index,raw_pointer,result.mismatch_granule,
            result.pointer_tag,result.allocation_tag};
    }
    [[nodiscard]] inline host_fault consume_host_fault() noexcept
    { auto result{pending_host_fault}; pending_host_fault={}; return result; }
}
