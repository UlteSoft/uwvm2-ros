// Pointer reconstruction is separate from validation: callers already hold
// the native owner's pin and complete access_result proof for this same owner.
#pragma once
#ifndef UWVM_MODULE
# include <atomic>
# include <cstddef>
# include <uwvm2/extension/memtag/hardware_mte.h>
# include <uwvm2/extension/memtag/shadow.h>
# include "hardware_thread.h"
# include <uwvm2/object/memory/signal/impl.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::memtag
{
    template<typename Memory>
    [[nodiscard]] inline ::std::byte* native_pointer_unlocked(
        Memory const& owner, ::uwvm2::extension::memtag::access_result const& proof) noexcept
    {
        if(!proof || owner.memory_begin == nullptr) { return nullptr; }
        auto* const pointer{owner.memory_begin + static_cast<::std::size_t>(proof.address)};
#if defined(__linux__) && defined(__aarch64__)
        if(owner.memtag_shadow.hardware_active())
        { return ::uwvm2::extension::memtag::hardware_mte::tagged_pointer(pointer, proof.pointer_tag); }
#endif
        return pointer;
    }

    // Startup-only. The caller excludes guest execution, growth and relocation.
    // Repeated activation of the same mmap owner is successful and preserves
    // tags. Unsupported backends/hosts leave the software owner unchanged.
    template<typename Memory>
    [[nodiscard]] inline ::uwvm2::extension::memtag::hardware_mte::setup_result
        activate_owner_hardware(Memory& owner) noexcept
    {
        namespace hw = ::uwvm2::extension::memtag::hardware_mte;
        if constexpr(!Memory::can_mmap) { return {hw::setup_error::unsupported}; }
        else
        {
            if(owner.memory_length_p == nullptr) { return {hw::setup_error::invalid_configuration}; }
            auto const result{owner.memtag_shadow.activate_hardware(owner.memory_begin,
                owner.memory_length_p->load(::std::memory_order_acquire))};
            if(result)
            {
#if defined(__linux__) && defined(__aarch64__) && defined(UWVM_SUPPORT_MMAP)
                if(!::uwvm2::object::memory::signal::register_hardware_memtag_owner(
                    owner.reserved_begin,owner.memory_begin,static_cast<::std::uint8_t const*>(owner.memtag_shadow.tag_data()),owner.memtag_shadow.shared()))
                { return {hw::setup_error::invalid_configuration}; }
#endif
                note_hardware_owner();
            }
            return result;
        }
    }
}
