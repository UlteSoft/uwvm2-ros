/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/

/**
 * @author      MacroModel
 * @version     2.0.0
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
# include <atomic>
# include <cstddef>
# include <cstdint>
# include <limits>
# include <type_traits>
# include <memory>
// macro
# include <uwvm2/utils/macro/push_macros.h>
// import
# include <fast_io.h>
# include <uwvm2/utils/container/impl.h>
# include <uwvm2/utils/mutex/impl.h>
# include <uwvm2/parser/wasm/standard/wasm1/type/impl.h>
# include <uwvm2/imported/wasi/wasip1/abi/impl.h>
# include "fd.h"
#endif

#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::imported::wasi::wasip1::fd_manager
{
    /// @brief [singleton]
    struct wasm_fd_storage_t
    {
        // Ensure that iterators can't fail during expansion
#if defined(UWVM_ROS_ZERO_HOST_DEFAULTS)
        static constexpr ::uwvm2::utils::container::vector<::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_unique_ptr_t> initial_reserved_slots() noexcept
        {
            ::uwvm2::utils::container::vector<::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_unique_ptr_t> result{};
            for(unsigned index{}; index != 3u; ++index)
            {
                auto& slot{result.emplace_back()};
                slot.fd_p->close_pos = SIZE_MAX - 1u;
                slot.fd_p->rights_base = {};
                slot.fd_p->rights_inherit = {};
            }
            return result;
        }
        ::uwvm2::utils::container::vector<::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_unique_ptr_t> opens{initial_reserved_slots()};
#else
        ::uwvm2::utils::container::vector<::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_unique_ptr_t> opens{};
#endif
        // Specialization of renumber, each extension requires a judgment of begin
        ::uwvm2::utils::container::map<::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t, ::uwvm2::imported::wasi::wasip1::fd_manager::wasi_fd_unique_ptr_t>
            renumber_map{};
        // Used to record the coordinates of closure for subsequent builds
        ::uwvm2::utils::container::vector<::std::size_t> closes{};
        ::uwvm2::utils::mutex::rwlock_t fds_rwlock{};  // [singleton]
        ::std::size_t fd_limit{};


        // Caller holds fds_rwlock. Allocation reuses a dense closed cell
        // before growing the table; only growth consumes another scan cell.
        [[nodiscard]] inline constexpr bool fits_allocation_scan_limit(::std::size_t limit) const noexcept
        {
            if(this->closes.size() > this->opens.size() || this->opens.size() > limit) { return false; }
            auto const remaining{limit - this->opens.size()};
            if(this->renumber_map.size() > remaining) { return false; }
            return !this->closes.empty() || this->renumber_map.size() < remaining;
        }

        // Caller holds fds_rwlock. Reserved empty cells occupy allocator slots
        // just like live descriptors; closed reusable cells do not. A sparse
        // renumbered FD costs one slot regardless of its numeric value.
        [[nodiscard]] inline constexpr bool fits_occupied_slot_limit(::std::size_t limit) const noexcept
        {
            if(this->closes.size() > this->opens.size()) { return false; }
            auto const dense_occupied{this->opens.size() - this->closes.size()};
            return dense_occupied <= limit && this->renumber_map.size() <= limit - dense_occupied;
        }
    };

    // Closed and pending cells are distinguishable to every reader. A pending
    // cell is absent from closes, so existing slot/scan accounting includes it.
    inline constexpr ::std::size_t pending_fd_slot{SIZE_MAX - 2u};

    // Same fair writer protocol as rw_fair_unique_guard_t, with cancellation
    // while waiting. Only this guard's PENDING ticket may be relinquished.
    class fd_table_write_guard final
    {
        ::uwvm2::utils::mutex::rwlock_t* lock{};
    public:
        fd_table_write_guard() noexcept = default;
        fd_table_write_guard(fd_table_write_guard const&) = delete;
        fd_table_write_guard& operator=(fd_table_write_guard const&) = delete;
        template<typename Cancel>
        [[nodiscard]] bool acquire(::uwvm2::utils::mutex::rwlock_t& value, Cancel const& cancelled) noexcept
        {
            constexpr auto pending{::uwvm2::utils::mutex::rwlock_pending_mask()};
            constexpr auto writer{::uwvm2::utils::mutex::rwlock_writer_mask()};
            constexpr auto readers{::uwvm2::utils::mutex::rwlock_reader_count_mask()};
            bool ticket{};
            unsigned spins{};
            for(;;)
            {
                if(cancelled())
                {
                    if(ticket) { value.bits.fetch_and(~pending, ::std::memory_order_release); }
                    return false;
                }
                auto old{value.bits.load(::std::memory_order_relaxed)};
                if(!ticket)
                {
                    if((old & pending) == 0u && value.bits.compare_exchange_weak(
                        old, old | pending, ::std::memory_order_release, ::std::memory_order_relaxed)) { ticket = true; }
                }
                else if((old & (writer | readers)) == 0u && value.bits.compare_exchange_weak(
                    old, old | writer, ::std::memory_order_acquire, ::std::memory_order_relaxed))
                { lock = ::std::addressof(value); return true; }
                if(++spins > 1000u) { ::fast_io::this_thread::yield(); }
                else { ::uwvm2::utils::mutex::rwlock_pause(); }
            }
        }
        ~fd_table_write_guard() noexcept
        {
            if(lock) { lock->bits.fetch_and(~(::uwvm2::utils::mutex::rwlock_writer_mask() |
                ::uwvm2::utils::mutex::rwlock_pending_mask()), ::std::memory_order_release); }
        }
    };

    // Reserve both the guest handle and its occupied-slot charge before native
    // O_CREAT/O_TRUNC. No manager/FD lock is retained across native I/O.
    class fd_slot_reservation final
    {
        wasm_fd_storage_t* map{};
        wasi_fd_t* cell{};
        ::std::size_t position{};
    public:
        fd_slot_reservation() noexcept = default;
        fd_slot_reservation(fd_slot_reservation const&) = delete;
        fd_slot_reservation& operator=(fd_slot_reservation const&) = delete;
        template<typename Cancel>
        [[nodiscard]] ::uwvm2::imported::wasi::wasip1::abi::errno_t reserve(wasm_fd_storage_t& storage, Cancel const& cancelled) noexcept
        {
            using error = ::uwvm2::imported::wasi::wasip1::abi::errno_t;
            using fd = ::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t;
            fd_table_write_guard writer{};
            if(!writer.acquire(storage.fds_rwlock, cancelled)) { return error::ecanceled; }
            if(map != nullptr) { return error::eio; }
            if constexpr((::std::numeric_limits<::std::size_t>::max)() > (::std::numeric_limits<fd>::max)())
            { if(storage.fd_limit > static_cast<::std::size_t>((::std::numeric_limits<fd>::max)())) { return error::eio; } }
            if(storage.closes.size() > storage.opens.size()) { return error::eio; }
            auto const occupied{storage.opens.size() - storage.closes.size()};
            if(storage.renumber_map.size() > storage.fd_limit || occupied >= storage.fd_limit - storage.renumber_map.size())
            { return error::emfile; }
            if(!storage.closes.empty())
            {
                auto const index{storage.closes.back_unchecked()};
                if(index >= storage.opens.size()) { return error::eio; }
                auto* selected{storage.opens.index_unchecked(index).fd_p};
                if(selected == nullptr) { return error::eio; }
                ::uwvm2::utils::mutex::mutex_guard_t held{selected->fd_mutex};
                if(selected->close_pos != storage.closes.size() - 1u) { return error::eio; }
                storage.closes.pop_back_unchecked();
                selected->close_pos = pending_fd_slot;
                cell = selected; position = index;
            }
            else
            {
                if(storage.opens.size() >= static_cast<::std::size_t>((::std::numeric_limits<fd>::max)())) { return error::emfile; }
                position = storage.opens.size();
                cell = storage.opens.emplace_back().fd_p;
                cell->close_pos = pending_fd_slot;
                // Preserve node/resource addresses when sparse handles become
                // dense. This is the existing descriptor normalization rule.
                while(storage.opens.size() <= static_cast<::std::size_t>((::std::numeric_limits<fd>::max)()))
                {
                    auto found{storage.renumber_map.find(static_cast<fd>(storage.opens.size()))};
                    if(found == storage.renumber_map.end()) { break; }
                    storage.opens.push_back(found->second.release());
                    storage.renumber_map.erase(found);
                }
            }
            map = ::std::addressof(storage);
            return error::esuccess;
        }
        [[nodiscard]] ::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t descriptor() const noexcept
        { return static_cast<::uwvm2::imported::wasi::wasip1::abi::wasi_posix_fd_t>(position); }
        void commit(wasi_fd_t& opened) noexcept
        {
            wasi_fd_ref_t retired{wasi_no_construct};
            {
                ::uwvm2::utils::mutex::rw_fair_unique_guard_t writer{map->fds_rwlock};
                ::uwvm2::utils::mutex::mutex_guard_t held{cell->fd_mutex};
                if(cell->close_pos != pending_fd_slot) { ::fast_io::fast_terminate(); }
                // Mutate a fixed node in place. A guessed closed/pending handle
                // must never hold a mutex that publication destroys/replaces.
                retired = ::std::move(cell->wasi_fd);
                cell->wasi_fd = ::std::move(opened.wasi_fd);
                cell->rights_base = opened.rights_base;
                cell->rights_inherit = opened.rights_inherit;
                cell->close_pos = SIZE_MAX;
                map = nullptr;
            }
        }
        ~fd_slot_reservation() noexcept
        {
            if(map == nullptr) { return; }
            wasi_fd_ref_t retired{wasi_no_construct};
            {
                // Cleanup cannot abandon a reserved charge on cancellation.
                // No native I/O or live descriptor mutex is held by this scope.
                ::uwvm2::utils::mutex::rw_fair_unique_guard_t writer{map->fds_rwlock};
                ::uwvm2::utils::mutex::mutex_guard_t held{cell->fd_mutex};
                if(cell->close_pos != pending_fd_slot) { ::fast_io::fast_terminate(); }
                retired = ::std::move(cell->wasi_fd);
                cell->rights_base = {}; cell->rights_inherit = {};
                cell->close_pos = map->closes.size();
                map->closes.emplace_back(position);
            }
            // Release the native resource only after both locks are gone.
        }
    };

}

#ifndef UWVM_MODULE
// macro
# include <uwvm2/utils/macro/pop_macros.h>
#endif
