/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/push_macros.h>
# include <atomic>
# include <cstddef>
# include <cstdint>
# include <cstdlib>
# include <cstring>
# include <limits>
# include <memory>
# include <new>
# include <utility>
# include <fast_io.h>
# include <uwvm2/object/memory/platform_page/impl.h>
# include <uwvm2/utils/mutex/rw_spin_lock.h>
# include "instruction.h"
# include "shadow.h"
# if defined(UWVM_SUPPORT_MMAP) && !(defined(_WIN32) || defined(__CYGWIN__))
#  include <sys/mman.h>
# endif
#endif
#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

UWVM_MODULE_EXPORT namespace uwvm2::extension::memtag
{
    enum class storage_error : unsigned
    {
        none, already_enabled, invalid_configuration, address_not_representable,
        capacity_exceeded, allocation_failed, unsupported_backend
    };
    enum class storage_backend : unsigned { disabled, growable, reserved };
    enum class storage_operation : unsigned { allocate, reserve, commit };
#if defined(UWVM_TEST)
    namespace details
    {
        // Test-only deterministic failure before any allocation/commit mutation.
        inline bool (*storage_failure_test_hook)(storage_operation, ::std::size_t){};
    }
#endif

    // One shadow belongs to one native linear-memory backing, not to an import
    // or module index. Defined-memory aliases borrow this same owner.
    // Default construction allocates nothing. Configuration/reset/move require
    // quiescence; allocator growth requires the owner's existing relocation pin.
    class shadow_storage
    {
        void* data_{};
        ::std::size_t allocated_bytes_{};
        ::std::size_t committed_bytes_{};
        ::std::uint64_t maximum_memory_bytes_{};
        ::std::atomic_size_t prepared_memory_bytes_{};
        ::std::size_t page_size_{};
        address_width width_{address_width::i32};
        storage_backend backend_{};
        bool shared_{};
        // Allocated only for enabled shared owners. All aliases borrow this same
        // control; no module index or raw byte address identifies a tag lease.
        ::uwvm2::utils::mutex::rwlock_t* shared_tag_gate_{};
#if defined(__linux__) && defined(__aarch64__)
        bool hardware_selected_{};
        ::std::byte* hardware_base_{};
#endif

        [[nodiscard]] static constexpr ::std::size_t tags_for(::std::size_t bytes) noexcept
        { return bytes / 16u + static_cast<::std::size_t>(bytes % 16u != 0u); }
        [[nodiscard]] constexpr ::std::size_t tag_stride() const noexcept
        { return shared_ ? sizeof(::std::atomic<::std::uint8_t>) : sizeof(::std::uint8_t); }
        [[nodiscard]] static bool should_fail(storage_operation operation, ::std::size_t bytes) noexcept
        {
#if defined(UWVM_TEST)
            return details::storage_failure_test_hook != nullptr && details::storage_failure_test_hook(operation, bytes);
#else
            static_cast<void>(operation); static_cast<void>(bytes); return false;
#endif
        }
        [[nodiscard]] static void* reserve(::std::size_t bytes) noexcept
        {
            if(should_fail(storage_operation::reserve, bytes)) { return nullptr; }
#if defined(UWVM_SUPPORT_MMAP) && (defined(_WIN32) || defined(__CYGWIN__))
# if !defined(__CYGWIN__) && !defined(__WINE__) && !defined(__BIONIC__) && defined(_WIN32_WINDOWS)
            return ::fast_io::win32::VirtualAlloc(nullptr, bytes, 0x00002000u, 0x01u);
# else
            void* address{};
            auto length{bytes};
            auto const error{::fast_io::win32::nt::nt_allocate_virtual_memory<false>(
                reinterpret_cast<void*>(-1), ::std::addressof(address), 0u, ::std::addressof(length), 0x00002000u, 0x01u)};
            return error ? nullptr : address;
# endif
#elif defined(UWVM_SUPPORT_MMAP)
            auto const address{::mmap(nullptr, bytes, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)};
            return address == MAP_FAILED ? nullptr : address;
#else
            static_cast<void>(bytes); return nullptr;
#endif
        }
        [[nodiscard]] bool commit(::std::size_t new_bytes) noexcept
        {
            if(new_bytes <= committed_bytes_) { return true; }
            if(should_fail(storage_operation::commit, new_bytes - committed_bytes_)) { return false; }
#if defined(UWVM_SUPPORT_MMAP) && (defined(_WIN32) || defined(__CYGWIN__))
# if !defined(__CYGWIN__) && !defined(__WINE__) && !defined(__BIONIC__) && defined(_WIN32_WINDOWS)
            auto const address{::fast_io::win32::VirtualAlloc(data_, new_bytes, 0x00001000u, 0x04u)};
            if(address != data_) { return false; }
# else
            void* address{data_};
            auto length{new_bytes};
            auto const error{::fast_io::win32::nt::nt_allocate_virtual_memory<false>(
                reinterpret_cast<void*>(-1), ::std::addressof(address), 0u, ::std::addressof(length), 0x00001000u, 0x04u)};
            if(error || address != data_) { return false; }
# endif
#elif defined(UWVM_SUPPORT_MMAP)
            if(::mprotect(static_cast<::std::byte*>(data_) + committed_bytes_,
                          new_bytes - committed_bytes_, PROT_READ | PROT_WRITE) != 0) { return false; }
#else
            static_cast<void>(new_bytes); return false;
#endif
            committed_bytes_ = new_bytes;
            return true;
        }
        constexpr void release_allocation() noexcept
        {
            if(data_ == nullptr) { return; }
            if(backend_ == storage_backend::growable) { ::std::free(data_); }
            else
            {
#if defined(UWVM_SUPPORT_MMAP) && (defined(_WIN32) || defined(__CYGWIN__))
# if !defined(__CYGWIN__) && !defined(__WINE__) && !defined(__BIONIC__) && defined(_WIN32_WINDOWS)
                static_cast<void>(::fast_io::win32::VirtualFree(data_, 0u, 0x00008000u));
# else
                void* address{data_};
                ::std::size_t length{};
                static_cast<void>(::fast_io::win32::nt::nt_free_virtual_memory<false>(
                    reinterpret_cast<void*>(-1), ::std::addressof(address), ::std::addressof(length), 0x00008000u));
# endif
#elif defined(UWVM_SUPPORT_MMAP)
                static_cast<void>(::munmap(data_, allocated_bytes_));
#endif
            }
        }
        void initialize_tags(void* storage, ::std::size_t first, ::std::size_t end) const noexcept
        {
            if(first == end) { return; }
            if(shared_)
            {
                auto* const tags{static_cast<::std::atomic<::std::uint8_t>*>(storage)};
                for(auto index{first}; index != end; ++index)
                { ::std::construct_at(tags + index, ::std::uint8_t{}); }
            }
            else
            {
                // Start lifetime without writing the reserved anonymous pages.
                // Growable allocations do need explicit zero initialization.
                auto* const tags{static_cast<::std::uint8_t*>(storage)};
                ::new(static_cast<void*>(tags + first)) ::std::uint8_t[end - first];
                if(backend_ == storage_backend::growable) { ::std::memset(tags + first, 0, end - first); }
            }
        }
        void copy_tags(void* replacement, ::std::size_t count) const noexcept
        {
            if(count == 0u) { return; }
            if(shared_)
            {
                auto* const destination{static_cast<::std::atomic<::std::uint8_t>*>(replacement)};
                auto* const source{static_cast<::std::atomic<::std::uint8_t>*>(data_)};
                for(::std::size_t index{}; index != count; ++index)
                { ::std::construct_at(destination + index, source[index].load(::std::memory_order_relaxed)); }
            }
            else { ::std::memcpy(replacement, data_, count); }
        }
        constexpr void take(shadow_storage& other) noexcept
        {
            data_ = ::std::exchange(other.data_, nullptr);
            shared_tag_gate_ = ::std::exchange(other.shared_tag_gate_, nullptr);
            allocated_bytes_ = ::std::exchange(other.allocated_bytes_, 0u);
            committed_bytes_ = ::std::exchange(other.committed_bytes_, 0u);
            maximum_memory_bytes_ = ::std::exchange(other.maximum_memory_bytes_, 0u);
            prepared_memory_bytes_.store(other.prepared_memory_bytes_.load(::std::memory_order_relaxed), ::std::memory_order_relaxed);
            other.prepared_memory_bytes_.store(0u, ::std::memory_order_relaxed);
            page_size_ = ::std::exchange(other.page_size_, 0u);
            width_ = other.width_;
            backend_ = ::std::exchange(other.backend_, storage_backend::disabled);
            shared_ = ::std::exchange(other.shared_, false);
#if defined(__linux__) && defined(__aarch64__)
            hardware_base_ = ::std::exchange(other.hardware_base_, nullptr);
            hardware_selected_ = ::std::exchange(other.hardware_selected_, false);
#endif
        }

    public:
        constexpr shadow_storage() noexcept = default;
        shadow_storage(shadow_storage const&) = delete;
        shadow_storage& operator=(shadow_storage const&) = delete;
        constexpr shadow_storage(shadow_storage&& other) noexcept { take(other); }
        constexpr shadow_storage& operator=(shadow_storage&& other) noexcept
        {
            if(this != ::std::addressof(other)) { reset(); take(other); }
            return *this;
        }
        constexpr ~shadow_storage() { reset(); }

        [[nodiscard]] constexpr bool enabled() const noexcept { return backend_ != storage_backend::disabled; }
        [[nodiscard]] constexpr address_width width() const noexcept { return width_; }
        [[nodiscard]] constexpr bool shared() const noexcept { return shared_; }
        [[nodiscard]] constexpr bool shared_tag_lease_ready() const noexcept
        { return !shared_ || shared_tag_gate_ != nullptr; }

        // The caller already owns the source and any relocation pin. The entire
        // tag proof and actual byte effect belong inside this short callback.
        // No Guest call, debugger pause, blocking Host I/O, trap or longjmp may
        // occur here. In particular, data WRITES are readers of allocation tags.
        template<bool Shared, bool Exclusive = false, typename Function>
        inline decltype(auto) with_shared_tag_lease(Function&& function) const noexcept
        {
            static_assert(noexcept(::std::forward<Function>(function)()));
            if constexpr(Shared)
            {
                if(shared_tag_gate_ != nullptr)
                {
                    if constexpr(Exclusive)
                    {
                        ::uwvm2::utils::mutex::rw_fair_unique_guard_t held{*shared_tag_gate_};
                        return ::std::forward<Function>(function)();
                    }
                    else
                    {
                        ::uwvm2::utils::mutex::rw_fair_shared_guard_t held{*shared_tag_gate_};
                        return ::std::forward<Function>(function)();
                    }
                }
            }
            // Shared=false has no gate test, lock acquisition or allocation.
            return ::std::forward<Function>(function)();
        }
#if defined(UWVM_TEST)
        [[nodiscard]] unsigned shared_tag_state_for_test() const noexcept
        { return shared_tag_gate_ == nullptr ? 0u : shared_tag_gate_->bits.load(::std::memory_order_acquire); }
        [[nodiscard]] constexpr void const* shared_tag_identity_for_test() const noexcept
        { return shared_tag_gate_; }
#endif
        [[nodiscard]] constexpr bool stable_address() const noexcept { return backend_ == storage_backend::reserved; }
        [[nodiscard]] constexpr ::std::uint64_t maximum_memory_bytes() const noexcept { return maximum_memory_bytes_; }
        [[nodiscard]] constexpr void* tag_data() const noexcept { return data_; }
        // Immutable cold plan, selected while configuring the unpublished owner.
        // Full CLI translation precedes deferred active-data initialization; JIT
        // and cache identity must not use the later activation state as policy.
        [[nodiscard]] constexpr bool hardware_selected() const noexcept
        {
#if defined(__linux__) && defined(__aarch64__)
            return hardware_selected_;
#else
            return false;
#endif
        }
        [[nodiscard]] constexpr bool hardware_active() const noexcept
        {
#if defined(__linux__) && defined(__aarch64__)
            return hardware_base_ != nullptr;
#else
            return false;
#endif
        }
        [[nodiscard]] constexpr ::std::byte* hardware_base() const noexcept
        {
#if defined(__linux__) && defined(__aarch64__)
            return hardware_base_;
#else
            return nullptr;
#endif
        }

        // Quiescent activation follows data/global initialization and precedes
        // guest publication. No allocation, tag write or mode change occurs on
        // unsupported hosts. Non-mmap owners deliberately remain software-only.
        [[nodiscard]] hardware_mte::setup_result activate_hardware(
            ::std::byte* base, ::std::size_t current_bytes) noexcept
        {
#if defined(__linux__) && defined(__aarch64__)
            using error = hardware_mte::setup_error;
            if(!enabled() || !stable_address() || base == nullptr || current_bytes > prepared_memory_bytes())
            { return {error::invalid_configuration}; }
            if(!hardware_selected_) { return {error::unsupported}; }
            if(hardware_base_ != nullptr)
            { return hardware_base_ == base ? hardware_mte::setup_result{} : hardware_mte::setup_result{error::invalid_configuration}; }
            auto const mode{hardware_mte::enable_current_thread_sync()};
            if(!mode) { return mode; }
            auto const mapping{hardware_mte::protect_mapping(base, current_bytes, page_size_)};
            if(!mapping) { return mapping; }
            auto const count{tags_for(current_bytes)};
            for(::std::size_t index{}; index != count; ++index)
            {
                auto const tag{shared_ ? static_cast<::std::atomic<::std::uint8_t>*>(data_)[index].load(::std::memory_order_relaxed)
                                       : static_cast<::std::uint8_t*>(data_)[index]};
                if((tag & 15u) != 0u) { hardware_mte::store_tag(base + index * 16u, tag); }
            }
            hardware_mte::finish_tag_writes();
            hardware_base_ = base;
            return {};
#else
            static_cast<void>(base); static_cast<void>(current_bytes);
            return {hardware_mte::setup_error::unsupported};
#endif
        }
        [[nodiscard]] ::std::size_t prepared_memory_bytes() const noexcept
        { return prepared_memory_bytes_.load(::std::memory_order_acquire); }
        [[nodiscard]] ::std::size_t tag_count() const noexcept { return tags_for(prepared_memory_bytes()); }

        // Returns a bounded borrowed snapshot. For growable storage the caller
        // must hold the native owner's memory-operation guard for its lifetime.
        // The memory length must come from that owner, never an imported alias's
        // independent counter. mmap callers load its published length acquire.
        template<::std::unsigned_integral Address, bool Shared = false>
        [[nodiscard]] shadow_view<Address, Shared> view(::std::uint64_t memory_bytes) const noexcept
        {
            using result_type = shadow_view<Address, Shared>;
            constexpr auto expected{sizeof(Address) == 4u ? address_width::i32 : address_width::i64};
            if(!enabled() || width_ != expected || shared_ != Shared || memory_bytes > prepared_memory_bytes())
            { return {nullptr, 0u, result_type::address_capacity + 1u}; }
            return {static_cast<typename result_type::tag_type*>(data_),
                    tags_for(static_cast<::std::size_t>(memory_bytes)), memory_bytes, hardware_base()};
        }

        // Call after native memory initialization and before guest publication.
        // Failed configuration leaves the storage disabled and unallocated.
        [[nodiscard]] storage_error enable(address_width width, bool shared, storage_backend backend,
                                             ::std::uint64_t current_bytes, ::std::uint64_t maximum_bytes) noexcept
        {
            if(enabled()) { return storage_error::already_enabled; }
            if((width != address_width::i32 && width != address_width::i64) ||
               (backend != storage_backend::growable && backend != storage_backend::reserved))
            { return storage_error::invalid_configuration; }
            auto const encoding_capacity{::std::uint64_t{1u} << (width == address_width::i32 ? 28u : 56u)};
            if(current_bytes > encoding_capacity || current_bytes > ::std::numeric_limits<::std::size_t>::max())
            { return storage_error::address_not_representable; }
            if(maximum_bytes > encoding_capacity) { maximum_bytes = encoding_capacity; }
            if(maximum_bytes > ::std::numeric_limits<::std::size_t>::max())
            { maximum_bytes = ::std::numeric_limits<::std::size_t>::max(); }
            if(current_bytes > maximum_bytes) { return storage_error::capacity_exceeded; }
            shadow_storage candidate;
            candidate.width_ = width;
            candidate.shared_ = shared;
            candidate.backend_ = backend;
            candidate.maximum_memory_bytes_ = maximum_bytes;
#if defined(__linux__) && defined(__aarch64__)
            candidate.hardware_selected_ = backend == storage_backend::reserved && hardware_mte::supported();
#endif
            if(backend == storage_backend::reserved)
            {
#if !defined(UWVM_SUPPORT_MMAP)
                return storage_error::unsupported_backend;
#else
                auto const [page_size, success]{::uwvm2::object::memory::platform_page::get_platform_page_size()};
                if(!success || page_size == 0u || (page_size & (page_size - 1u)) != 0u)
                { return storage_error::invalid_configuration; }
                candidate.page_size_ = page_size;
                auto const count{tags_for(static_cast<::std::size_t>(maximum_bytes))};
                auto const stride{candidate.tag_stride()};
                if(count > (::std::numeric_limits<::std::size_t>::max() - (page_size - 1u)) / stride)
                { return storage_error::address_not_representable; }
                auto const raw_bytes{count * stride};
                auto const allocation_bytes{(raw_bytes + page_size - 1u) & ~(page_size - 1u)};
                if(allocation_bytes != 0u)
                {
                    candidate.data_ = reserve(allocation_bytes);
                    if(candidate.data_ == nullptr) { return storage_error::allocation_failed; }
                    candidate.allocated_bytes_ = allocation_bytes;
                }
#endif
            }
            auto const error{candidate.prepare_growth(current_bytes)};
            if(error != storage_error::none) { return error; }
            if(shared)
            {
                using control_type = ::uwvm2::utils::mutex::rwlock_t;
                static_assert(alignof(control_type) <= alignof(::std::max_align_t));
                if(should_fail(storage_operation::allocate, sizeof(control_type)))
                { return storage_error::allocation_failed; }
                candidate.shared_tag_gate_ = static_cast<control_type*>(::std::malloc(sizeof(control_type)));
                if(candidate.shared_tag_gate_ == nullptr) { return storage_error::allocation_failed; }
                ::std::construct_at(candidate.shared_tag_gate_);
            }
            *this = ::std::move(candidate);
            return storage_error::none;
        }

        // Must run under the native owner's grow lock, before data relocation/
        // commitment and before publishing its new logical byte length.
        // A later native allocation failure may retain prepared capacity; this
        // never publishes memory and preserves every previously accessible tag.
        [[nodiscard]] storage_error prepare_growth(::std::uint64_t new_memory_bytes) noexcept
        {
            if(!enabled()) { return storage_error::none; }
            if(new_memory_bytes > maximum_memory_bytes_) { return storage_error::capacity_exceeded; }
            if(new_memory_bytes > ::std::numeric_limits<::std::size_t>::max())
            { return storage_error::address_not_representable; }
            auto const old_bytes{prepared_memory_bytes_.load(::std::memory_order_relaxed)};
            if(new_memory_bytes <= old_bytes) { return storage_error::none; }
            auto const previous{tags_for(old_bytes)};
            auto const needed{tags_for(static_cast<::std::size_t>(new_memory_bytes))};
            auto const stride{tag_stride()};
            if(needed > ::std::numeric_limits<::std::size_t>::max() / stride)
            { return storage_error::address_not_representable; }
            auto const bytes{needed * stride};
            if(backend_ == storage_backend::reserved)
            {
                if(bytes > ::std::numeric_limits<::std::size_t>::max() - (page_size_ - 1u))
                { return storage_error::address_not_representable; }
                auto const rounded{(bytes + page_size_ - 1u) & ~(page_size_ - 1u)};
                if(rounded > allocated_bytes_) { return storage_error::capacity_exceeded; }
                if(!commit(rounded)) { return storage_error::allocation_failed; }
                initialize_tags(data_, previous, needed);
            }
            else if(bytes > allocated_bytes_)
            {
                if(should_fail(storage_operation::allocate, bytes)) { return storage_error::allocation_failed; }
                auto* const replacement{::std::malloc(bytes)};
                if(replacement == nullptr) { return storage_error::allocation_failed; }
                copy_tags(replacement, previous);
                initialize_tags(replacement, previous, needed);
                ::std::free(data_);
                data_ = replacement;
                allocated_bytes_ = bytes;
            }
            else { initialize_tags(data_, previous, needed); }
            prepared_memory_bytes_.store(static_cast<::std::size_t>(new_memory_bytes), ::std::memory_order_release);
            return storage_error::none;
        }

        constexpr void reset() noexcept
        {
            // Configuration/move/reset remain quiescent operations. A tag lease
            // is not source-lifetime authority and cannot authorize retirement.
            if(shared_tag_gate_ != nullptr)
            {
                ::std::destroy_at(shared_tag_gate_);
                ::std::free(shared_tag_gate_);
                shared_tag_gate_ = nullptr;
            }
            release_allocation();
            data_ = nullptr;
            allocated_bytes_ = committed_bytes_ = page_size_ = 0u;
            maximum_memory_bytes_ = 0u;
            prepared_memory_bytes_.store(0u, ::std::memory_order_relaxed);
            width_ = address_width::i32;
            backend_ = storage_backend::disabled;
            shared_ = false;
#if defined(__linux__) && defined(__aarch64__)
            hardware_base_ = nullptr;
            hardware_selected_ = false;
#endif
        }
    };
}
#ifndef UWVM_MODULE
# include <uwvm2/utils/macro/pop_macros.h>
#endif
