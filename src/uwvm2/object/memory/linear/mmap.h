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
# include <uwvm2/utils/allocator/fast_io_strict/object.h>
#endif
#include <uwvm2/utils/allocator/resource_limit.h>
#include <uwvm2/object/memory/linear/mmap_reservation.h>
#include <uwvm2/utils/allocator/sanitizer_mapping.h>

#ifndef UWVM_MODULE
// std
# include <cstddef>
# include <cstdint>
# include <climits>
# include <limits>
# include <memory>
# include <new>
# include <optional>
# include <atomic>
# include <bit>
# include <utility>
// macro
# include <uwvm2/utils/macro/push_macros.h>
// platform
# if defined(UWVM_SUPPORT_MMAP) && !(defined(_WIN32) || defined(__CYGWIN__))  // posix mmap
#  include <sys/mman.h>
# endif
// import
# include <fast_io.h>
# include <uwvm2/utils/container/impl.h>
# include <uwvm2/utils/debug/impl.h>
# include <uwvm2/utils/mutex/impl.h>
# include <uwvm2/object/memory/wasm_page/impl.h>
# include <uwvm2/extension/memtag/storage.h>
# include <uwvm2/object/memory/platform_page/impl.h>
# include <uwvm2/object/memory/signal/impl.h>
# include "threaded_grow_types.h"
#endif

#ifndef UWVM_MODULE_EXPORT
# define UWVM_MODULE_EXPORT
#endif

#if defined(UWVM_SUPPORT_MMAP)
# if CHAR_BIT != 8
#  error "mmap unsupported platform (The POSIX standard requires CHAR_BIT == 8)"
# endif
#endif

UWVM_MODULE_EXPORT namespace uwvm2::object::memory::linear
{
#if defined(UWVM_SUPPORT_MMAP)

    enum class mmap_memory_status_t : unsigned
    {
        wasm32,
        wasm64
    };

# if defined(UWVM_TEST) && defined(UWVM_CPP_EXCEPTIONS) && !(defined(_WIN32) || defined(__CYGWIN__))
    namespace details
    {
        using strict_grow_mprotect_test_hook_t = void (*)(void*, ::std::size_t, int);

        // Test-only interception point for deterministic partial-mprotect failure injection. Real kernels differ in
        // whether a particular invalid range fails before or after changing a prefix, so munmap-based tests are not
        // portable enough to exercise both strict-growth recovery branches.
        inline strict_grow_mprotect_test_hook_t strict_grow_mprotect_test_hook{};

        inline void strict_grow_mprotect(void* address, ::std::size_t length, int protection)
        {
            if(strict_grow_mprotect_test_hook != nullptr) [[unlikely]]
            {
                strict_grow_mprotect_test_hook(address, length, protection);
                return;
            }
            ::fast_io::details::sys_mprotect(address, length, protection);
        }
    }
# endif

    inline constexpr unsigned max_full_protection_wasm32_index{33u};
    inline constexpr auto max_full_protection_wasm32_length{mmap_reservation::full_wasm32_window};

    // wasm32 full-protection layout (64-bit host only):
    //   | 4GiB usable window (u32 address space) | 4GiB back guard |
    //   + trailing guard (platform-page aligned) to cover custom-page alignment + max foreseeable type size (64 bytes).
    //
    // Both address operands are unsigned u32, so their widened sum is at most 2^33-2. With an access width of at most
    // 64 bytes, the last byte is at most 2^33+61, inside the trailing reservation. Only the first 4 GiB can be committed.
    // Thus even an overflowing effective address faults inside this memory's reservation without a software u32 check.
    // No front guard is needed for unsigned addresses. Keeping the total reservation unchanged also makes the signal
    // registry base equal to the Wasm base, so fault offsets are not incorrectly increased by a front-guard displacement.
    inline constexpr ::std::uint_least64_t wasm32_full_protection_front_guard_u64{0ull};
    inline constexpr ::std::uint_least64_t wasm32_full_protection_usable_u64{1ull << 32u};       // 4 GiB
    inline constexpr ::std::uint_least64_t wasm32_full_protection_back_guard_u64{1ull << 32u};   // 4 GiB
    inline constexpr ::std::size_t mmap_guard_max_access_size{static_cast<::std::size_t>(mmap_reservation::max_access_size)};
    inline constexpr ::std::uint_least64_t wasm32_max_effective_offset{0x1fffffffeull};
    static_assert(wasm32_full_protection_front_guard_u64 + wasm32_full_protection_usable_u64 + wasm32_full_protection_back_guard_u64 ==
                  max_full_protection_wasm32_length);

    // This is a reservation-domain proof, not a logical-bounds test. Callers may use it only for the full-protection
    // wasm32 layout with custom_page >= platform_page; partial mappings and byte-granular custom pages remain checked.
    [[nodiscard]] inline constexpr bool wasm32_full_protection_covers_access(::std::uint_least64_t effective_offset,
                                                                            ::std::size_t access_size) noexcept
    {
        return sizeof(::std::size_t) >= sizeof(::std::uint_least64_t) && sizeof(::std::uintptr_t) >= sizeof(::std::uint_least64_t) &&
               effective_offset <= wasm32_max_effective_offset && access_size <= mmap_guard_max_access_size;
    }
    static_assert(wasm32_max_effective_offset + mmap_guard_max_access_size <= max_full_protection_wasm32_length + mmap_guard_max_access_size);

    /// @note      Memory safety model for the mmap-backed linear memory:
    ///            - The base pointer `memory_begin` is stable for the lifetime of the instance; growth commits additional virtual address space instead of
    ///              reallocating or moving the buffer.
    ///            - In “full protection” configurations (typically when the custom page size is greater than or equal to the platform page size), most
    ///              out-of-bounds accesses are rejected by guard pages, so hot paths can omit explicit software bounds checks and only validate extreme
    ///              offsets that might escape the protected region against `memory_length_p`.
    ///            - When the custom page size is smaller than the platform page size, the base address is still stable, but page protection alone cannot
    ///              distinguish all intra-page overflows. In this mode, the `check_memory_bounds_*` helpers perform explicit `(offset, size)` range checks
    ///              using the atomically updated `memory_length_p`.
    ///            - Growth keeps `memory_begin` stable. The grow mutex and acquire/release operations on `memory_length_p` publish newly committed pages.
    ///            - WebAssembly linear memory is grow-only: once a given `(offset, size)` range has been validated against some length, that range remains
    ///              valid after subsequent grows, so callers do not need to re-run bounds checks for the same range after a successful grow.

    // Keep immutable access metadata together even in a vector of memory
    // instances. An additional memory must not split another instance's base
    // and page-policy fields across cache lines on common 64-byte-line hosts.
    struct alignas(64) mmap_memory_t
    {
        inline static constexpr ::uwvm2::utils::container::u8string_view name{u8"mmap"};

        // Both addresses are retained by native code or signal readers across owner moves.
        struct owner_metadata
        {
            ::std::atomic_size_t length{};
            ::uwvm2::utils::mutex::mutex_t growing{};
        };
        // `reserved_begin` is the base address of the entire reserved VMA (including guard windows).
        // `memory_begin` is the base address of the *usable* window visible to wasm memory accesses.
        ::std::byte* reserved_begin{};
        ::std::byte* memory_begin{};
        ::std::atomic_size_t* memory_length_p{};
        // This memory_size is not used in the program unless custom_page is smaller than platform_page.
        // The number of platform pages can be obtained by rounding up the memory size to the nearest integer multiple of the platform page size.

        unsigned custom_page_size_log2{};

        // Cached answer for `require_dynamic_determination_memory_size()`.
        // Hot paths (load/store) call this through bounds checks, so it must not query platform page size per access.
        bool require_dynamic_determination_memory_size_cached{};

        // Uses existing alignment padding; following hot-field offsets are unchanged.
        // Immutable during execution. Only size/grow use this policy; ordinary
        // memory access and mmap hardware protection keep their existing paths.
        bool sequentially_consistent_size{};

        mmap_memory_status_t status{};

        // This lock is used to prevent multithreaded growth.
        ::uwvm2::utils::mutex::mutex_t* growing_mutex_p{};

        // Maximum logical byte length for this owner. Growth and memtag never
        // enter the guard, even when a caller supplies a larger maximum.
        ::std::size_t reservation_limit_bytes{::std::numeric_limits<::std::size_t>::max()};

        // Immutable owner index for cold software-fault diagnostics, matching
        // the hardware-fault registry. Imported aliases retain their owner's
        // identity. This trailing field does not displace hot access metadata.
        ::std::size_t diagnostic_owner_memory_index{};

        // Optional owner-bound shadow; all pre-existing hot-field offsets stay unchanged.
        ::uwvm2::extension::memtag::shadow_storage memtag_shadow{};
        ::uwvm2::utils::allocator::resource::reservation<::uwvm2::utils::allocator::resource::pool::memory> allocation_charge{};


        // Cold configuration and exact ownership extent, after all pre-existing hot fields.
        mmap_reservation::bytes_type reservation_budget_bytes{mmap_reservation::maximum_bytes};
        ::std::size_t mapped_reservation_bytes{};
        bool allow_reservation_fallback{true};
        bool use_planned_reservation{};
        ::std::uint_least8_t reservation_adm_bits{};
        bool reservation_tagged{}, reservation_manual{}, full_protection{};
        ::std::size_t reservation_alias_count{};

        ::std::unique_ptr<owner_metadata, ::uwvm2::utils::allocator::resource::initializer_object_deleter<owner_metadata>> metadata{};

        /// @brief This macro is used to control the behavior of the non-img compiler.
        inline static constexpr bool can_mmap{true};

        [[nodiscard]] static constexpr ::std::size_t charged_memory_bytes(::std::size_t length) noexcept
        {
            auto const [page,success]{::uwvm2::object::memory::platform_page::get_platform_page_size()};
            if(!success || page==0uz) { return SIZE_MAX; }
            auto const tail{length%page};
            if(tail==0uz) { return length; }
            auto const padding{page-tail};
            return length > SIZE_MAX-padding ? SIZE_MAX : length+padding;
        }

        /// @brief When handling multithreaded growth, only the atomic variable `memory_length_p` is required. When `custom_page_size >= platform_page_size`,
        ///        dynamic determination of `memory_size` is unnecessary.
        inline static constexpr bool support_multi_thread{true};

        struct fallible_construction_t { explicit fallible_construction_t() = default; };
        explicit mmap_memory_t(fallible_construction_t) noexcept
            : custom_page_size_log2{static_cast<unsigned>(::std::countr_zero(
                  static_cast<::std::size_t>(::uwvm2::object::memory::wasm_page::default_wasm32_page_size)))},
              metadata{::uwvm2::utils::allocator::resource::try_initializer_object<owner_metadata>(
                  [](owner_metadata* raw) noexcept { return ::new(static_cast<void*>(raw)) owner_metadata{}; })}
        {
            if(metadata)
            {
                memory_length_p = ::std::addressof(metadata->length);
                growing_mutex_p = ::std::addressof(metadata->growing);
            }
            auto const [page, known]{::uwvm2::object::memory::platform_page::get_platform_page_size()};
            require_dynamic_determination_memory_size_cached = !known || page == 0uz || (1uz << custom_page_size_log2) < page;
        }

        mmap_memory_t() noexcept : mmap_memory_t(fallible_construction_t{})
        {
            if(!metadata) [[unlikely]] { ::uwvm2::utils::allocator::resource::initializer_metadata_limit(); }
        }

        mmap_memory_t(::std::size_t custom_page_size, mmap_memory_status_t memory_status) noexcept : mmap_memory_t()
        {
            if(!::std::has_single_bit(custom_page_size)) [[unlikely]] { ::fast_io::fast_terminate(); }
            custom_page_size_log2 = ::std::countr_zero(custom_page_size);
            status = memory_status;
            auto const [page, known]{::uwvm2::object::memory::platform_page::get_platform_page_size()};
            if(!known || page == 0uz) [[unlikely]] { ::fast_io::fast_terminate(); }
            require_dynamic_determination_memory_size_cached = custom_page_size < page;
        }

        // Configure only after native initialization and before guest publication.
        [[nodiscard]] inline ::uwvm2::extension::memtag::storage_error enable_memtag(
            ::uwvm2::extension::memtag::address_width width, bool shared, unsigned ignored_bits = 4u,
            ::uwvm2::extension::memtag::owner_backend backend = ::uwvm2::extension::memtag::default_owner_backend,
            ::std::optional<unsigned> saved_tag_start = {}) noexcept
        {
            namespace mt = ::uwvm2::extension::memtag;
            if(this->memory_begin == nullptr) { return mt::storage_error::invalid_configuration; }
            if(this->memory_length_p == nullptr ||
               (width == mt::address_width::i32) != (this->status == mmap_memory_status_t::wasm32))
            { return mt::storage_error::invalid_configuration; }
            if constexpr(mmap_reservation::host_bits > 48u)
            {
                if(width == mt::address_width::i64 && static_cast<mmap_reservation::bytes_type>(this->mapped_reservation_bytes) >
                    mmap_reservation::tagged_wasm64_span_limit) { return mt::storage_error::capacity_exceeded; }
            }
            return this->memtag_shadow.enable(width, shared, mt::storage_backend::reserved,
                this->memory_length_p->load(::std::memory_order_acquire), this->reservation_limit_bytes, ignored_bits, backend, saved_tag_start);
        }

        inline constexpr bool require_dynamic_determination_memory_size() const noexcept
        {
            // When `custom_page_size` is less than one platform page, `mmap` can only guarantee the stability of the starting address and cannot provide
            // page-protection checks at the custom-page granularity. Additional dynamic verification is required.
            if UWVM_IF_NOT_CONSTEVAL { return this->require_dynamic_determination_memory_size_cached; }
            return false;
        }

        inline constexpr bool is_full_page_protection() const noexcept
        {
            return this->full_protection;
        }

        [[nodiscard]] inline constexpr bool reservation_participates() const noexcept
        { return mmap_reservation::host_bits < 64u || this->status == mmap_memory_status_t::wasm64 || this->reservation_adm_bits; }

        [[nodiscard]] inline constexpr mmap_reservation::layout planned_layout(::std::size_t maximum_pages, ::std::size_t os_page,
            mmap_reservation::bytes_type retry_cap = mmap_reservation::maximum_bytes) const noexcept
        {
            if(this->custom_page_size_log2 >= ::std::numeric_limits<::std::size_t>::digits) { return {}; }
            auto cap{(::std::min)(this->reservation_budget_bytes, retry_cap)};
            if(!this->use_planned_reservation)
            {
                // Standalone owners also share the process pool with all live generations.
                auto const state{mmap_reservation::available_pool()};
                if(!state.valid) { return {}; }
                if(!state.remaining.full_address64) { cap = (::std::min)(cap, state.remaining.bytes); }
            }
            auto const data_quota{static_cast<mmap_reservation::bytes_type>(this->reservation_limit_bytes)};
            auto const maximum_data_pages{this->reservation_manual ? maximum_pages :
                (::std::min)(maximum_pages, this->reservation_limit_bytes >> this->custom_page_size_log2)};
            auto layout{mmap_reservation::make_layout(mmap_reservation::host_bits, this->status == mmap_memory_status_t::wasm64,
                cap, maximum_data_pages, ::std::size_t{1} << this->custom_page_size_log2, os_page, this->reservation_tagged,
                this->reservation_adm_bits, 0u, this->reservation_manual ? &data_quota : nullptr)};
            if(layout && retry_cap == mmap_reservation::maximum_bytes && this->use_planned_reservation)
            {
                if(this->reservation_budget_bytes > SIZE_MAX || this->reservation_budget_bytes % os_page) { return {}; }
                layout.reserved = this->reservation_budget_bytes;
            }
            return layout;
        }

        [[nodiscard]] inline constexpr ::std::size_t maximum_initial_page_count() const noexcept
        {
            if(this->custom_page_size_log2 >= ::std::numeric_limits<::std::size_t>::digits) { return 0uz; }
            if(this->reserved_begin != nullptr) { return this->reservation_limit_bytes >> this->custom_page_size_log2; }
            auto const [page, success]{::uwvm2::object::memory::platform_page::get_platform_page_size()};
            return success ? static_cast<::std::size_t>(this->planned_layout(SIZE_MAX, page).data >> this->custom_page_size_log2) : 0uz;
        }

        enum class initialization_result
        {
            initialized, invalid_state, invalid_capacity, memory_limit, reservation_limit, mapping_failure, registry_limit, handler_unavailable
        };

        [[nodiscard]] static constexpr char const* initialization_error(initialization_result error) noexcept
        {
            switch(error)
            {
                case initialization_result::invalid_state: return "uwvm: [error] initializer: invalid native linear-memory initialization state.\n";
                case initialization_result::invalid_capacity: return "uwvm: [error] initializer: invalid linear memory reservation or insufficient data capacity.\n";
                case initialization_result::memory_limit:
                    return ::uwvm2::utils::allocator::resource::initializer_memory_limit_message;
                case initialization_result::reservation_limit:
                    return "uwvm: [error] initializer: live linear memory mappings exceed the configured process reservation pool or participant count.\n";
                case initialization_result::mapping_failure:
                    return ::uwvm2::utils::allocator::resource::initializer_memory_mapping_message;
                case initialization_result::registry_limit:
                    return "uwvm: [error] initializer: mmap fault registry allocation refused; check --wasm-set-resource-limit initializer metadata-single-bytes/metadata-live-bytes and available host memory.\n";
                case initialization_result::handler_unavailable: return "uwvm: [error] initializer: cannot install the native mmap fault handler.\n";
                case initialization_result::initialized: break;
            }
            ::std::unreachable();
        }

        [[nodiscard]] inline constexpr initialization_result initialize_pages(::std::size_t init_page_count,
            ::std::size_t declared_max_pages, ::std::size_t memory_index) noexcept
        {
            if(memory_begin || reserved_begin || !metadata || init_page_count > declared_max_pages) { return initialization_result::invalid_state; }
            auto const [platform_page, known_page]{::uwvm2::object::memory::platform_page::get_platform_page_size()};
            auto layout{this->planned_layout(declared_max_pages, platform_page)};
            if(!known_page || !layout || init_page_count > (layout.data >> this->custom_page_size_log2)) [[unlikely]]
            {
                return initialization_result::invalid_capacity;
            }
            auto const custom_page_size{1uz << this->custom_page_size_log2};
            auto const memory_length{init_page_count << this->custom_page_size_log2};
            auto const committed_bytes{charged_memory_bytes(memory_length)};
            if(memory_length > ::uwvm2::utils::allocator::resource::initializer_allocation_bytes ||
               !this->allocation_charge.resize(committed_bytes))
            {
                return initialization_result::memory_limit;
            }

            auto reserve_native{[](::std::size_t bytes) noexcept -> ::std::byte*
            {
# if defined(_WIN32) || defined(__CYGWIN__)
#  if !defined(__CYGWIN__) && !defined(__WINE__) && !defined(__BIONIC__) && defined(_WIN32_WINDOWS)
                return reinterpret_cast<::std::byte*>(::fast_io::win32::VirtualAlloc(
                    nullptr, bytes, 0x00002000u /*MEM_RESERVE*/, 0x01u /*PAGE_NOACCESS*/));
#  else
                ::std::byte* address{};
                auto length{bytes};
                auto const result{::fast_io::win32::nt::nt_allocate_virtual_memory<false>(
                    reinterpret_cast<void*>(-1), reinterpret_cast<void**>(::std::addressof(address)),
                    0u, ::std::addressof(length), 0x00002000u, 0x01u)};
                return result ? nullptr : address;
#  endif
# else
                // Use the nonthrowing OS API, including exception-disabled
                // builds. Address-space exhaustion is a resource refusal,
                // not a broken VM invariant.
                constexpr auto flags{MAP_PRIVATE | MAP_ANONYMOUS
#  if defined(MAP_NORESERVE)
                    | MAP_NORESERVE
#  endif
                };
# if defined(__linux__)
                auto* address{::uwvm2::utils::allocator::sanitizer_mapping::reserve(bytes,flags)};
# else
                auto* address{::mmap(nullptr, bytes, PROT_NONE, flags, -1, 0)};
# endif
                return address == MAP_FAILED ? nullptr : static_cast<::std::byte*>(address);
# endif
            }};
            for(;;)
            {
                auto const bytes{static_cast<::std::size_t>(layout.reserved)};
                bool const charged{mmap_reservation::acquire(bytes, this->reservation_participates())};
                if(charged)
                {
                    this->reserved_begin = reserve_native(bytes);
                    if(this->reserved_begin != nullptr)
                    {
                        this->mapped_reservation_bytes = bytes;
                        auto const full{mmap_reservation::full_window(mmap_reservation::host_bits, this->status == mmap_memory_status_t::wasm64,
                            this->reservation_adm_bits, custom_page_size, platform_page)};
                        this->full_protection = full && bytes >= full;
                        break;
                    }
                    mmap_reservation::release(bytes, this->reservation_participates());
                }
                if(this->allow_reservation_fallback && (mmap_reservation::host_bits < 64u || this->reservation_adm_bits))
                {
                    // A smaller guard can still cover the entire manual data quota.
                    // An automatic quota can shrink to the initial page count.
                    auto const range{mmap_reservation::layout_bounds(mmap_reservation::host_bits,
                        this->status == mmap_memory_status_t::wasm64, init_page_count, declared_max_pages, custom_page_size,
                        platform_page, this->reservation_manual ? &layout.data : nullptr, this->reservation_tagged, this->reservation_adm_bits)};
                    if(range && layout.reserved > range.minimum_span)
                    {
                        auto const smaller{(::std::max)(range.minimum_span, layout.reserved / 2u)};
                        layout = this->planned_layout(declared_max_pages, platform_page, smaller);
                        if(layout && init_page_count <= (layout.data >> this->custom_page_size_log2)) { continue; }
                    }
                }
                this->clear_unregistered_mapping();
                return charged ? initialization_result::mapping_failure : initialization_result::reservation_limit;
            }
            this->memory_begin = this->reserved_begin;

            // A zero-page owner only needs the inaccessible reservation. Windows
            // rejects a zero commit size and POSIX need not accept mprotect(...,0,...).
            if(memory_length != 0uz)
            {
                bool committed{};
# if defined(_WIN32) || defined(__CYGWIN__)
#  if !defined(__CYGWIN__) && !defined(__WINE__) && !defined(__BIONIC__) && defined(_WIN32_WINDOWS)
                auto* address{::fast_io::win32::VirtualAlloc(this->memory_begin, memory_length, 0x00001000u /* MEM_COMMIT */, 0x04u /* PAGE_READWRITE */)};
                committed = address != nullptr;
#  else
                auto* address{this->memory_begin};
                auto length{memory_length};
                committed = !::fast_io::win32::nt::nt_allocate_virtual_memory<false>(reinterpret_cast<void*>(-1),
                    reinterpret_cast<void**>(::std::addressof(address)), 0, ::std::addressof(length), 0x00001000u, 0x04u);
#  endif
                if(committed && address != this->memory_begin) [[unlikely]] { ::fast_io::fast_terminate(); }
# else
                committed = ::mprotect(this->memory_begin, committed_bytes, PROT_READ | PROT_WRITE) == 0;
# endif
                if(!committed) [[unlikely]] { this->clear_unregistered_mapping(); return initialization_result::mapping_failure; }
            }

            // The OS reservation is still private. Only publish it to fault readers
            // once its first commit succeeded. Imports never register another interval.
            namespace faults = ::uwvm2::object::memory::signal;
            auto const registration{faults::try_register_protected_segment(
                reserved_begin, reserved_begin + mapped_reservation_bytes, memory_length_p, memory_index)};
            if(registration != faults::registration_result::registered) [[unlikely]]
            {
                this->clear_unregistered_mapping();
                if(registration == faults::registration_result::metadata_refused) { return initialization_result::registry_limit; }
                if(registration == faults::registration_result::handler_unavailable) { return initialization_result::handler_unavailable; }
                ::fast_io::fast_terminate();
            }
# if defined(__linux__)
            ::uwvm2::utils::allocator::sanitizer_mapping::accessible(this->memory_begin,memory_length);
# endif
            this->diagnostic_owner_memory_index = memory_index;
            this->reservation_limit_bytes = static_cast<::std::size_t>(layout.data);
            this->require_dynamic_determination_memory_size_cached = custom_page_size < platform_page;
            this->memory_length_p->store(memory_length, this->sequentially_consistent_size ? ::std::memory_order_seq_cst : ::std::memory_order_release);
            return initialization_result::initialized;
        }

        inline constexpr void init_by_page_count(::std::size_t pages,
            ::std::size_t maximum=::std::numeric_limits<::std::size_t>::max(),::std::size_t index=0u) noexcept
        {
            if(memory_begin != nullptr) { return; }
            auto const result{initialize_pages(pages, maximum, index)};
            if(result != initialization_result::initialized) { ::uwvm2::utils::allocator::resource::reject_allocation(initialization_error(result)); }
        }
        [[nodiscard]] inline constexpr bool try_init_by_page_count(::std::size_t pages,
            ::std::size_t maximum,::std::size_t index) noexcept
        { return initialize_pages(pages, maximum, index) == initialization_result::initialized; }

        /// @brief      Grow the memory.
        /// @param      max_limit_memory_length     This maximum value is derived from the maximum memory limit.
        inline constexpr bool try_grow_silently(::std::size_t page_grow_size,
                                                ::std::size_t max_limit_memory_length = ::std::numeric_limits<::std::size_t>::max(),
                                                ::std::size_t* old_page_size_out = nullptr) noexcept
        {
            if(page_grow_size == 0uz) [[unlikely]]
            {
                if(old_page_size_out != nullptr) [[likely]] { *old_page_size_out = this->get_page_size(); }
                return true;
            }

            if(this->memory_begin == nullptr) [[unlikely]]
            {
                // this is a bug
                ::uwvm2::utils::debug::trap_and_inform_bug_pos();
            }

            if(page_grow_size > ::std::numeric_limits<::std::size_t>::max() >> this->custom_page_size_log2) [[unlikely]]
            {
                // This situation cannot occur; it is due to user input error.
                return false;
            }

            // Once initialized, the custom_page_size_log2 will not change.
            auto const memory_grow_size{page_grow_size << this->custom_page_size_log2};

            // Acquiring internal data requires locking.
            if(this->growing_mutex_p == nullptr || this->memory_length_p == nullptr) [[unlikely]]
            {
                // this is a bug
                ::uwvm2::utils::debug::trap_and_inform_bug_pos();
            }

            ::uwvm2::utils::mutex::mutex_guard_t growing_mutex_guard_1{*this->growing_mutex_p};

            // This atomic operation prevents the dynamic check from retrieving an erroneous value when reading the length.
            // Here, with the lock's support, memory access is already guaranteed to be relaxed. However, during dynamic checks, an acquire is still required.
            auto const current_length{this->memory_length_p->load(this->sequentially_consistent_size ? ::std::memory_order_seq_cst : ::std::memory_order_relaxed)};
            if(old_page_size_out != nullptr) [[likely]] { *old_page_size_out = current_length >> this->custom_page_size_log2; }

            // Select the smaller value between the manually set maximum and the maximum already mmapped by the platform.
            // Beyond the boundaries of the VMA (adjacent virtual address space), protection is not automatically enforced; these regions are simply “unmapped.”
            // These unmapped areas remain as potentially available address space. Subsequent mmap operations may automatically allocate memory within these
            // spaces. Failing to impose such restrictions during memory expansion risks corrupting memory allocated to other parts of the program.
            max_limit_memory_length = (::std::min)(max_limit_memory_length, this->reservation_limit_bytes);

            // start checking
            if(max_limit_memory_length < current_length) [[unlikely]] { return false; }

            auto const left_memory_size{max_limit_memory_length - current_length};
            if(memory_grow_size > left_memory_size) [[unlikely]] { return false; }

            // grown region [current_length, grow_final_memory_length)
            auto const grow_final_memory_length{current_length + memory_grow_size};
            ::uwvm2::utils::allocator::resource::resize_transaction<::uwvm2::utils::allocator::resource::pool::memory>
                allocation_change{this->allocation_charge, charged_memory_bytes(grow_final_memory_length)};
            if(!allocation_change) { return false; }

            if(this->memtag_shadow.ignored_bits() && this->memtag_shadow.prepare_growth(grow_final_memory_length) !=
               ::uwvm2::extension::memtag::storage_error::none) [[unlikely]] { return false; }

            // Set pages according to the initialized size
            // The minimum allocation unit for mmap is a page. Even if you request a size smaller than a page, the kernel will ultimately manage and
            // map the memory in page-sized units.

# if defined(_WIN32) || defined(__CYGWIN__)                                                          // windows
#  if !defined(__CYGWIN__) && !defined(__WINE__) && !defined(__BIONIC__) && defined(_WIN32_WINDOWS)  // win32
            {
                auto const vamemory{reinterpret_cast<::std::byte*>(
                    ::fast_io::win32::VirtualAlloc(this->memory_begin, grow_final_memory_length, 0x00001000u /*MEM_COMMIT*/, 0x04u /*PAGE_READWRITE*/))};
                if(vamemory != this->memory_begin) [[unlikely]] { ::fast_io::fast_terminate(); }
            }
#  else  // nt
            {
                ::std::byte* vamemory{this->memory_begin};

                // Since const values cannot be used, a copy must be made.
                ::std::size_t grow_final_memory_length_tmp{grow_final_memory_length};

                constexpr bool zw{false};
                auto const status{::fast_io::win32::nt::nt_allocate_virtual_memory<zw>(reinterpret_cast<void*>(-1),
                                                                                       reinterpret_cast<void**>(::std::addressof(vamemory)),
                                                                                       0u,
                                                                                       ::std::addressof(grow_final_memory_length_tmp),
                                                                                       0x00001000u /*MEM_COMMIT*/,
                                                                                       0x04u /*PAGE_READWRITE*/)};

                if(status || vamemory != this->memory_begin) [[unlikely]] { ::fast_io::fast_terminate(); }
            }
#  endif
# elif !defined(__NEWLIB__) && !(defined(__MSDOS__) || defined(__DJGPP__)) && (!defined(__wasm__) || (defined(__wasi__) && defined(_WASI_EMULATED_MMAN))) &&   \
     __has_include(<sys/mman.h>)  // posix
            {

                // sys_mprotect may throw ::fast_io::error

#  ifdef UWVM_CPP_EXCEPTIONS
                try
#  endif
                {
                    // The length argument must be a multiple of the page size as returned by sysconf(_SC_PAGE_SIZE).
                    // Since custom_page may appear, it must be aligned to the top here.

                    auto const [page_size, success]{::uwvm2::object::memory::platform_page::get_platform_page_size()};
                    if(!success) [[unlikely]] { ::fast_io::fast_terminate(); }
                    if(page_size == 0uz) [[unlikely]] { ::fast_io::fast_terminate(); }

                    auto const page_size_minus_1{(page_size - 1uz)};
                    if(grow_final_memory_length > ::std::numeric_limits<::std::size_t>::max() - page_size_minus_1) [[unlikely]] { ::fast_io::fast_terminate(); }
                    auto const grow_final_memory_length_ceil{(grow_final_memory_length + page_size_minus_1) & ~page_size_minus_1};

                    ::fast_io::details::sys_mprotect(this->memory_begin, grow_final_memory_length_ceil, (PROT_READ | PROT_WRITE | ::uwvm2::extension::memtag::hardware_mte::protection_for(this->memtag_shadow.hardware_active())));
                }
#  ifdef UWVM_CPP_EXCEPTIONS
                catch(::fast_io::error)
                {
                    ::fast_io::fast_terminate();
                }
#  endif
            }
# endif

            // Only after the memory has been successfully "committed" is the new length released to other threads.
            allocation_change.commit();
# if defined(__linux__)
            ::uwvm2::utils::allocator::sanitizer_mapping::accessible(this->memory_begin+current_length,memory_grow_size);
# endif
            this->memory_length_p->store(grow_final_memory_length, this->sequentially_consistent_size ? ::std::memory_order_seq_cst : ::std::memory_order_release);

            // growing_mutex_guard_1 destruct here
            return true;
        }

        /// @brief      Grow memory in the default fail-fast/silent policy.
        /// @details    "Silent" means silent with respect to the host allocation result. A request that exceeds the configured Wasm limit must be rejected by
        ///             the caller as a Wasm `-1` result before this function is entered. Once this function is used, a host allocation/commit/protection
        ///             failure is handled by immediate `fast_terminate()`, not by returning an error to the Wasm program.
        /// @note       On overcommit systems, especially common Linux configurations, virtual memory admission can keep succeeding up to the architecture or
        ///             user-VA limit (for example, about 128 TiB on common x86-64 layouts). The real OOM may occur only when guest code later writes to the
        ///             admitted pages, at which point the kernel can kill the process without giving the runtime a recoverable error. This is why the default
        ///             policy is silent/fail-fast. Do not replace this path with `grow_strictly()` merely to make host allocation failure observable.
        inline constexpr void grow_silently(::std::size_t page_grow_size,
                                            ::std::size_t max_limit_memory_length = ::std::numeric_limits<::std::size_t>::max()) noexcept
        {
            if(!this->try_grow_silently(page_grow_size, max_limit_memory_length)) [[unlikely]] { ::fast_io::fast_terminate(); }
        }

        /// @brief      Grow the memory (non-silent).
        /// @return     true if the grow succeeds; false if the grow exceeds the limit or the platform refuses to commit/protect more pages.
        /// @details    Strict growth is selected only when the strict flag is enabled. In this policy, host allocation/commit/protection failure is converted
        ///             into the Wasm `memory.grow` failure result (`-1`) and execution may continue. This is mainly meaningful on systems without overcommit,
        ///             where the host can report allocation failure at grow time. On overcommit systems, a successful strict grow still cannot protect the
        ///             process from a later kernel OOM kill on actual guest writes.
        /// @param      max_limit_memory_length     This maximum value is derived from the maximum memory limit.
        inline constexpr bool grow_strictly(::std::size_t page_grow_size,
                                            ::std::size_t max_limit_memory_length = ::std::numeric_limits<::std::size_t>::max(),
                                            ::std::size_t* old_page_size_out = nullptr) noexcept
        {
            if(page_grow_size == 0uz) [[unlikely]]
            {
                if(old_page_size_out != nullptr) [[likely]] { *old_page_size_out = this->get_page_size(); }
                return true;
            }

            if(this->memory_begin == nullptr) [[unlikely]]
            {
                // this is a bug
                ::uwvm2::utils::debug::trap_and_inform_bug_pos();
            }

            if(page_grow_size > ::std::numeric_limits<::std::size_t>::max() >> this->custom_page_size_log2) [[unlikely]]
            {
                // This situation cannot occur; it is due to user input error.
                return false;
            }

            // Once initialized, the custom_page_size_log2 will not change.
            auto const memory_grow_size{page_grow_size << this->custom_page_size_log2};

            // Acquiring internal data requires locking.
            if(this->growing_mutex_p == nullptr || this->memory_length_p == nullptr) [[unlikely]]
            {
                // this is a bug
                ::uwvm2::utils::debug::trap_and_inform_bug_pos();
            }

            ::uwvm2::utils::mutex::mutex_guard_t growing_mutex_guard_1{*this->growing_mutex_p};

            auto const current_length{this->memory_length_p->load(this->sequentially_consistent_size ? ::std::memory_order_seq_cst : ::std::memory_order_relaxed)};
            if(old_page_size_out != nullptr) [[likely]] { *old_page_size_out = current_length >> this->custom_page_size_log2; }

            max_limit_memory_length = (::std::min)(max_limit_memory_length, this->reservation_limit_bytes);

            if(max_limit_memory_length < current_length) [[unlikely]]
            {
                // This situation cannot occur; it is due to user input error.
                // this is a bug
                ::fast_io::fast_terminate();
            }

            auto const left_memory_size{max_limit_memory_length - current_length};
            if(memory_grow_size > left_memory_size) [[unlikely]] { return false; }

            auto const grow_final_memory_length{current_length + memory_grow_size};
            ::uwvm2::utils::allocator::resource::resize_transaction<::uwvm2::utils::allocator::resource::pool::memory>
                allocation_change{this->allocation_charge, charged_memory_bytes(grow_final_memory_length)};
            if(!allocation_change) { return false; }

            if(this->memtag_shadow.ignored_bits() && this->memtag_shadow.prepare_growth(grow_final_memory_length) !=
               ::uwvm2::extension::memtag::storage_error::none) [[unlikely]] { return false; }

# if defined(_WIN32) || defined(__CYGWIN__)  // windows
            {
                auto const [page_size, success]{::uwvm2::object::memory::platform_page::get_platform_page_size()};
                if(!success) [[unlikely]]
                {
                    // this never occurs; it is due to user input error.
                    ::fast_io::fast_terminate();
                }
                if(page_size == 0uz) [[unlikely]]
                {
                    // this never occurs; it is due to user input error.
                    ::fast_io::fast_terminate();
                }

                auto const page_size_minus_1{(page_size - 1uz)};

                if(current_length > ::std::numeric_limits<::std::size_t>::max() - page_size_minus_1) [[unlikely]] { return false; }
                auto const current_length_ceil{(current_length + page_size_minus_1) & ~page_size_minus_1};

                if(grow_final_memory_length > ::std::numeric_limits<::std::size_t>::max() - page_size_minus_1) [[unlikely]] { return false; }
                auto const grow_final_memory_length_ceil{(grow_final_memory_length + page_size_minus_1) & ~page_size_minus_1};

                if(grow_final_memory_length_ceil > current_length_ceil)
                {
                    auto const commit_delta{grow_final_memory_length_ceil - current_length_ceil};
                    auto const commit_begin{this->memory_begin + current_length_ceil};

#  if !defined(__CYGWIN__) && !defined(__WINE__) && !defined(__BIONIC__) && defined(_WIN32_WINDOWS)  // win32
                    auto const vamemory{reinterpret_cast<::std::byte*>(
                        ::fast_io::win32::VirtualAlloc(commit_begin, commit_delta, 0x00001000u /*MEM_COMMIT*/, 0x04u /*PAGE_READWRITE*/))};
                    if(vamemory != commit_begin) [[unlikely]] { return false; }
#  else  // nt
                    ::std::byte* vamemory{commit_begin};
                    ::std::size_t commit_delta_tmp{commit_delta};
                    constexpr bool zw{false};
                    auto const status{::fast_io::win32::nt::nt_allocate_virtual_memory<zw>(reinterpret_cast<void*>(-1),
                                                                                           reinterpret_cast<void**>(::std::addressof(vamemory)),
                                                                                           0u,
                                                                                           ::std::addressof(commit_delta_tmp),
                                                                                           0x00001000u /*MEM_COMMIT*/,
                                                                                           0x04u /*PAGE_READWRITE*/)};
                    if(status || vamemory != commit_begin) [[unlikely]] { return false; }
#  endif
                }
            }
# elif !defined(__NEWLIB__) && !(defined(__MSDOS__) || defined(__DJGPP__)) && (!defined(__wasm__) || (defined(__wasi__) && defined(_WASI_EMULATED_MMAN))) &&   \
     __has_include(<sys/mman.h>)  // posix
            {
                auto const [page_size, success]{::uwvm2::object::memory::platform_page::get_platform_page_size()};
                if(!success) [[unlikely]]
                {
                    // this never occurs; it is due to user input error.
                    ::fast_io::fast_terminate();
                }
                if(page_size == 0uz) [[unlikely]]
                {
                    // this never occurs; it is due to user input error.
                    ::fast_io::fast_terminate();
                }

                auto const page_size_minus_1{(page_size - 1uz)};

                if(current_length > ::std::numeric_limits<::std::size_t>::max() - page_size_minus_1) [[unlikely]] { return false; }
                auto const current_length_ceil{(current_length + page_size_minus_1) & ~page_size_minus_1};

                if(grow_final_memory_length > ::std::numeric_limits<::std::size_t>::max() - page_size_minus_1) [[unlikely]] { return false; }
                auto const grow_final_memory_length_ceil{(grow_final_memory_length + page_size_minus_1) & ~page_size_minus_1};

                if(grow_final_memory_length_ceil > current_length_ceil)
                {
                    auto const protect_delta{grow_final_memory_length_ceil - current_length_ceil};
                    auto const protect_begin{this->memory_begin + current_length_ceil};

                    // sys_mprotect may throw ::fast_io::error
#  ifdef UWVM_CPP_EXCEPTIONS
                    try
#  endif
                    {
#  if defined(UWVM_TEST) && defined(UWVM_CPP_EXCEPTIONS)
                        ::uwvm2::object::memory::linear::details::strict_grow_mprotect(reinterpret_cast<void*>(protect_begin),
                                                                                       protect_delta,
                                                                                       (PROT_READ | PROT_WRITE | ::uwvm2::extension::memtag::hardware_mte::protection_for(this->memtag_shadow.hardware_active())));
#  else
                        ::fast_io::details::sys_mprotect(reinterpret_cast<void*>(protect_begin), protect_delta, (PROT_READ | PROT_WRITE | ::uwvm2::extension::memtag::hardware_mte::protection_for(this->memtag_shadow.hardware_active())));
#  endif
                    }
#  ifdef UWVM_CPP_EXCEPTIONS
                    catch(::fast_io::error)
                    {
                        // Full and partial guard paths omit the live-length check on at least part of their access
                        // domain. POSIX does not guarantee that a failed range mprotect left every page unchanged, so
                        // another guest thread could already have observed or modified a newly writable prefix. That
                        // state cannot be made transactionally recoverable; only the per-access dynamically checked
                        // layout may roll back and report memory.grow failure.
                        if(!this->require_dynamic_determination_memory_size()) [[likely]] { ::fast_io::fast_terminate(); }
#   ifdef UWVM_CPP_EXCEPTIONS
                        try
#   endif
                        {
                            // Ensure failure is not partially committed; otherwise, OOB may stop faulting in full-protection mode.
#   if defined(UWVM_TEST) && defined(UWVM_CPP_EXCEPTIONS)
                            ::uwvm2::object::memory::linear::details::strict_grow_mprotect(reinterpret_cast<void*>(protect_begin),
                                                                                           protect_delta,
                                                                                           PROT_NONE);
#   else
                            ::fast_io::details::sys_mprotect(reinterpret_cast<void*>(protect_begin), protect_delta, PROT_NONE);
#   endif
                        }
#   ifdef UWVM_CPP_EXCEPTIONS
                        catch(::fast_io::error)
                        {
                            // A failed rollback can leave pages beyond the published logical length accessible.
                            // Continuing would violate the fault-based bounds invariant; fail closed instead.
                            ::fast_io::fast_terminate();
                        }
#   endif
                        return false;
                    }
#  endif
                }
            }
# endif

            allocation_change.commit();
# if defined(__linux__)
            ::uwvm2::utils::allocator::sanitizer_mapping::accessible(this->memory_begin+current_length,memory_grow_size);
# endif
            this->memory_length_p->store(grow_final_memory_length, this->sequentially_consistent_size ? ::std::memory_order_seq_cst : ::std::memory_order_release);
            return true;
        }

# include "threaded_grow_member.h"

        inline constexpr ::std::size_t get_page_size() const noexcept
        {
            if(this->memory_length_p == nullptr) [[unlikely]]
            {
                // this is a bug
                ::uwvm2::utils::debug::trap_and_inform_bug_pos();
            }

            // This can be used in the WASM runtime.
# if defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64)
            // x86 acquire and SC loads both lower to MOV. Select SC statically:
            // a runtime ordering choice leaves an otherwise dead policy-byte CMP
            // in Clang's machine code, penalizing every unshared memory.size too.
            auto const all_memory_length{this->memory_length_p->load(::std::memory_order_seq_cst)};
# else
            auto const all_memory_length{this->memory_length_p->load(this->sequentially_consistent_size ? ::std::memory_order_seq_cst : ::std::memory_order_acquire)};
# endif

            return all_memory_length >> this->custom_page_size_log2;
        }

        inline constexpr mmap_memory_t(mmap_memory_t const& other) noexcept = delete;

        inline constexpr mmap_memory_t& operator= (mmap_memory_t const& other) noexcept = delete;

        /// @note       This function is designed to be lock-free and cannot be executed during WASM execution (multi-threaded). It can only be done before the
        ///             WASM execution.
        inline constexpr mmap_memory_t(mmap_memory_t&& other) noexcept
        {
            this->reserved_begin = other.reserved_begin;
            this->memory_begin = other.memory_begin;
            this->memory_length_p = other.memory_length_p;
            this->custom_page_size_log2 = other.custom_page_size_log2;
            this->require_dynamic_determination_memory_size_cached = other.require_dynamic_determination_memory_size_cached;
            this->reservation_limit_bytes = other.reservation_limit_bytes;
            this->reservation_budget_bytes = other.reservation_budget_bytes;
            this->allow_reservation_fallback = other.allow_reservation_fallback;
            this->use_planned_reservation = other.use_planned_reservation;
            this->reservation_adm_bits = other.reservation_adm_bits;
            this->reservation_tagged = other.reservation_tagged;
            this->reservation_manual = other.reservation_manual;
            this->reservation_alias_count = other.reservation_alias_count;
            this->full_protection = ::std::exchange(other.full_protection, false);
            this->mapped_reservation_bytes = ::std::exchange(other.mapped_reservation_bytes, 0uz);
            this->diagnostic_owner_memory_index = other.diagnostic_owner_memory_index;
            this->memtag_shadow = ::std::move(other.memtag_shadow);
            this->allocation_charge = ::std::move(other.allocation_charge);
            this->metadata = ::std::move(other.metadata);
            this->sequentially_consistent_size = other.sequentially_consistent_size;
            this->status = other.status;
            this->growing_mutex_p = other.growing_mutex_p;

            // clear destory other
            other.reserved_begin = nullptr;
            other.memory_begin = nullptr;
            other.memory_length_p = nullptr;
            other.custom_page_size_log2 = 0u;
            other.require_dynamic_determination_memory_size_cached = false;
            other.reservation_limit_bytes = ::std::numeric_limits<::std::size_t>::max();
            other.diagnostic_owner_memory_index = 0uz;
            other.sequentially_consistent_size = false;
            other.status = mmap_memory_status_t{};
            other.growing_mutex_p = nullptr;
        }

        /// @note      This function is designed to be lock-free and cannot be executed during WASM execution (multi-threaded). It can only be done before the
        ///             WASM execution.
        inline constexpr mmap_memory_t& operator= (mmap_memory_t&& other) noexcept
        {
            if(::std::addressof(other) == this) [[unlikely]] { return *this; }

            clear_destroy();

            this->reserved_begin = other.reserved_begin;
            this->memory_begin = other.memory_begin;
            this->memory_length_p = other.memory_length_p;
            this->custom_page_size_log2 = other.custom_page_size_log2;
            this->require_dynamic_determination_memory_size_cached = other.require_dynamic_determination_memory_size_cached;
            this->reservation_limit_bytes = other.reservation_limit_bytes;
            this->reservation_budget_bytes = other.reservation_budget_bytes;
            this->allow_reservation_fallback = other.allow_reservation_fallback;
            this->use_planned_reservation = other.use_planned_reservation;
            this->reservation_adm_bits = other.reservation_adm_bits;
            this->reservation_tagged = other.reservation_tagged;
            this->reservation_manual = other.reservation_manual;
            this->reservation_alias_count = other.reservation_alias_count;
            this->full_protection = ::std::exchange(other.full_protection, false);
            this->mapped_reservation_bytes = ::std::exchange(other.mapped_reservation_bytes, 0uz);
            this->diagnostic_owner_memory_index = other.diagnostic_owner_memory_index;
            this->memtag_shadow = ::std::move(other.memtag_shadow);
            this->allocation_charge = ::std::move(other.allocation_charge);
            this->metadata = ::std::move(other.metadata);
            this->sequentially_consistent_size = other.sequentially_consistent_size;
            this->status = other.status;
            this->growing_mutex_p = other.growing_mutex_p;

            // clear destory other
            other.reserved_begin = nullptr;
            other.memory_begin = nullptr;
            other.memory_length_p = nullptr;
            other.custom_page_size_log2 = 0u;
            other.require_dynamic_determination_memory_size_cached = false;
            other.reservation_limit_bytes = ::std::numeric_limits<::std::size_t>::max();
            other.diagnostic_owner_memory_index = 0uz;
            other.sequentially_consistent_size = false;
            other.status = mmap_memory_status_t{};
            other.growing_mutex_p = nullptr;

            return *this;
        }

        [[nodiscard]] inline constexpr ::std::size_t get_acquire_reserved_space() const noexcept { return this->mapped_reservation_bytes; }
        [[nodiscard]] inline constexpr ::std::size_t get_acquire_reserved_space_ceil() const noexcept { return this->mapped_reservation_bytes; }
        // Every start below this exclusive limit is covered for a scalar access of up to 64 bytes.
        [[nodiscard]] inline constexpr ::std::size_t partial_guard_limit_bytes() const noexcept
        {
            return this->mapped_reservation_bytes >= mmap_guard_max_access_size ?
                this->mapped_reservation_bytes - mmap_guard_max_access_size + 1uz : 0uz;
        }

        /// @brief      Clear the memory.
        /// @note       Simply clearing the memory without altering the page size.
        /// @note       This function is designed to be lock-free and cannot be executed during WASM execution (multi-threaded). It can only be done after the
        ///             WASM execution.
        inline constexpr void clear() noexcept
        {
            if(reserved_begin != nullptr)
            {
                // A fault reader may hold the old length, tag shadow, or native base.
                ::uwvm2::object::memory::signal::unregister_protected_segment(reserved_begin, reserved_begin + mapped_reservation_bytes);
                if(!reservation_manual) { reservation_limit_bytes = SIZE_MAX; }
            }
            clear_unregistered_mapping();
        }

    private:
        inline constexpr void clear_unregistered_mapping() noexcept
        {
            this->memtag_shadow.reset();
            this->diagnostic_owner_memory_index = 0uz;
            if(this->reserved_begin == nullptr) [[unlikely]]
            {
# if (defined(_DEBUG) || defined(DEBUG)) && defined(UWVM_ENABLE_DETAILED_DEBUG_CHECK)
                if(this->memory_begin != nullptr) [[unlikely]] { ::uwvm2::utils::debug::trap_and_inform_bug_pos(); }
# endif
                this->reserved_begin = nullptr;
                this->memory_begin = nullptr;
                if(this->memory_length_p == nullptr) [[unlikely]] { return; }
                this->memory_length_p->store(0uz, ::std::memory_order_relaxed);
                this->allocation_charge.reset();
                return;
            }

            auto const acquire_reserved_space_ceil{get_acquire_reserved_space_ceil()};

# if defined(_WIN32) || defined(__CYGWIN__)                                                          // windows
#  if !defined(__CYGWIN__) && !defined(__WINE__) && !defined(__BIONIC__) && defined(_WIN32_WINDOWS)  // win32
            {
                if(!::fast_io::win32::VirtualFree(this->reserved_begin, 0u, 0x00008000u /*MEM_RELEASE*/)) [[unlikely]] { ::fast_io::fast_terminate(); }
            }
#  else  // nt
            {
                ::std::byte* vfmemory{this->reserved_begin};
                ::std::size_t free_type{0uz};

                constexpr bool zw{false};
                if(::fast_io::win32::nt::nt_free_virtual_memory<zw>(reinterpret_cast<void*>(-1),
                                                                    reinterpret_cast<void**>(::std::addressof(vfmemory)),
                                                                    ::std::addressof(free_type),
                                                                    0x00008000u /*MEM_RELEASE*/)) [[unlikely]]
                {
                    ::fast_io::fast_terminate();
                }
            }
#  endif
# elif !defined(__NEWLIB__) && !(defined(__MSDOS__) || defined(__DJGPP__)) && (!defined(__wasm__) || (defined(__wasi__) && defined(_WASI_EMULATED_MMAN))) &&   \
     __has_include(<sys/mman.h>)  // posix
            {
                // The length argument must be a multiple of the page size as returned by sysconf(_SC_PAGE_SIZE).
                // Since custom_page may appear, it must be aligned to the top here.

                auto const [page_size, success]{::uwvm2::object::memory::platform_page::get_platform_page_size()};
                if(!success) [[unlikely]] { ::fast_io::fast_terminate(); }

                // fast_io::details::sys_munmap_nothrow is noexcept, manually throws exceptions.
                if(::fast_io::details::sys_munmap_nothrow(this->reserved_begin, acquire_reserved_space_ceil)) [[unlikely]] { ::fast_io::fast_terminate(); }
            }
# endif

            mmap_reservation::release(::std::exchange(this->mapped_reservation_bytes, 0uz), this->reservation_participates());
            this->full_protection = false;
            this->reserved_begin = nullptr;
            this->memory_begin = nullptr;

# if (defined(_DEBUG) || defined(DEBUG)) && defined(UWVM_ENABLE_DETAILED_DEBUG_CHECK)
            // The clear operation completes before the object is destroyed, so nullptr causes a crash.
            if(this->memory_length_p == nullptr) [[unlikely]] { ::uwvm2::utils::debug::trap_and_inform_bug_pos(); }
# endif

            // This can only be used after the WASM execution has completed, so use relaxed instead of acquired.
            this->memory_length_p->store(0uz, ::std::memory_order_relaxed);
            this->allocation_charge.reset();
        }

    public:
        inline constexpr void clear_destroy() noexcept
        {
            clear();
            metadata.reset();
            memory_length_p = nullptr;
            growing_mutex_p = nullptr;
            custom_page_size_log2 = 0u;
            require_dynamic_determination_memory_size_cached = false;
            reservation_limit_bytes = ::std::numeric_limits<::std::size_t>::max();
            status = mmap_memory_status_t{};
        }

        inline constexpr ~mmap_memory_t() { clear(); }
    };

#endif

}  // namespace uwvm2::object::memory::linear

/// @brief Define container optimization operations for use with fast_io
UWVM_MODULE_EXPORT namespace fast_io::freestanding
{
#if defined(UWVM_SUPPORT_MMAP)
    template <>
    struct is_trivially_copyable_or_relocatable<::uwvm2::object::memory::linear::mmap_memory_t>
    {
        inline static constexpr bool value = true;
    };
#endif
}

#ifndef UWVM_MODULE
// macro
# include <uwvm2/utils/macro/pop_macros.h>
#endif
