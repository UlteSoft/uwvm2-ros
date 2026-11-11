// Cold native entry preparation. Guest code cannot change this process policy.
#pragma once
#ifndef UWVM_MODULE
#include <atomic>
#include <uwvm2/extension/memtag/hardware_mte.h>
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif
UWVM_MODULE_EXPORT namespace uwvm2::runtime::memtag
{
#if defined(__linux__) && defined(__aarch64__)
    // Sticky after any actual owner activates. Keeping this bit set after a
    // generation retires avoids refcount races with an admitted worker entry.
    inline ::std::atomic<bool> hardware_owner_published{};
#endif
    inline void note_hardware_owner() noexcept
    {
#if defined(__linux__) && defined(__aarch64__)
        hardware_owner_published.store(true,::std::memory_order_release);
#endif
    }
    [[nodiscard]] inline ::uwvm2::extension::memtag::hardware_mte::setup_result prepare_hardware_thread() noexcept
    {
#if defined(__linux__) && defined(__aarch64__)
        if(hardware_owner_published.load(::std::memory_order_acquire))
        { return ::uwvm2::extension::memtag::hardware_mte::enable_current_thread_selected(); }
#endif
        return {};
    }
}
