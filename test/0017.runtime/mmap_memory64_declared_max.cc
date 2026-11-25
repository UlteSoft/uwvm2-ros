// Real bounded-memory64 instantiation/growth regression; Linux cgroup only.
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/object/memory/linear/impl.h>
#include <fast_io.h>
#include <cstddef>
#include <limits>
#include <utility>

#define CHECK(condition) do { if(!(condition)) { ::fast_io::io::perr("FAIL ", __LINE__, ": ", #condition, "\n"); ::fast_io::fast_terminate(); } } while(false)
namespace linear = ::uwvm2::object::memory::linear;
static_assert(sizeof(::std::size_t) >= 8);
#if !defined(UWVM_SUPPORT_MMAP)
#error This regression requires the actual mmap backend.
#endif

int main()
{
    auto const [os_page, success]{::uwvm2::object::memory::platform_page::get_platform_page_size()};
    CHECK(success && os_page <= 65536u);
    auto const tail{(65536ull + 64u + os_page - 1u) / os_page * os_page};
    auto const floor{(1ull << 33u) + tail};
    for(bool shared : {false, true})
    {
        linear::mmap_memory_t memory{65536, linear::mmap_memory_status_t::wasm64};
        memory.sequentially_consistent_size = shared;
        memory.init_by_page_count(1, 3);
        CHECK(memory.reservation_limit_bytes == 3 * 65536);
        CHECK(!memory.require_dynamic_determination_memory_size());
        CHECK(memory.get_acquire_reserved_space_ceil() == floor);
        CHECK(!memory.is_full_page_protection() && memory.get_page_size() == 1);
        CHECK(memory.memory_begin[0] == ::std::byte{});
        memory.memory_begin[0] = ::std::byte{0x71};
        ::std::size_t old{};
        CHECK(memory.try_grow_silently(1, ::std::numeric_limits<::std::size_t>::max(), &old) && old == 1);
        CHECK(memory.grow_strictly(1, ::std::numeric_limits<::std::size_t>::max(), &old) && old == 2);
        CHECK(memory.get_page_size() == 3 && memory.memory_begin[0] == ::std::byte{0x71});
        CHECK(memory.memory_begin[2 * 65536] == ::std::byte{});
        CHECK(!memory.try_grow_silently(1, ::std::numeric_limits<::std::size_t>::max(), &old));
        CHECK(!memory.grow_strictly(1, ::std::numeric_limits<::std::size_t>::max(), &old));
        CHECK(memory.get_page_size() == 3);
        CHECK(memory.try_grow_silently(0, 3 * 65536, &old) && old == 3);
        linear::mmap_memory_t moved{::std::move(memory)};
        CHECK(moved.reservation_limit_bytes == 3 * 65536 && moved.get_page_size() == 3);
        CHECK(!moved.require_dynamic_determination_memory_size());
        CHECK(moved.sequentially_consistent_size == shared && memory.memory_begin == nullptr);
        memory = ::std::move(moved);
        CHECK(memory.reservation_limit_bytes == 3 * 65536 && memory.get_page_size() == 3);
        CHECK(moved.memory_begin == nullptr);
        memory.clear();
        memory.init_by_page_count(1, 1);
        CHECK(memory.reservation_limit_bytes == 65536 && memory.get_page_size() == 1);
        CHECK(!memory.require_dynamic_determination_memory_size());
        CHECK(!memory.try_grow_silently(1, 2 * 65536));
    }
    {
        linear::mmap_memory_t empty{65536, linear::mmap_memory_status_t::wasm64};
        empty.init_by_page_count(0, 0);
        CHECK(empty.reservation_limit_bytes == 0 && empty.get_page_size() == 0);
        CHECK(!empty.require_dynamic_determination_memory_size());
        CHECK(empty.get_acquire_reserved_space_ceil() == floor);
        CHECK(!empty.try_grow_silently(1, 65536));
        CHECK(empty.try_grow_silently(0, 0));
    }
    {
        linear::mmap_memory_t full_partial{65536, linear::mmap_memory_status_t::wasm64};
        full_partial.init_by_page_count(0);
        auto const default_pool{16ull << 40u};
        CHECK(full_partial.get_acquire_reserved_space_ceil() == default_pool);
        CHECK(full_partial.reservation_limit_bytes == (default_pool - tail) / 65536u * 65536u);
        CHECK(!full_partial.require_dynamic_determination_memory_size());
    }
    {
        linear::mmap_memory_t memory32{};
        memory32.init_by_page_count(0, 0);
        CHECK(memory32.is_full_page_protection());
        CHECK(!memory32.require_dynamic_determination_memory_size());
        CHECK(memory32.reservation_limit_bytes == 0 && !memory32.try_grow_silently(1));
        CHECK(memory32.get_acquire_reserved_space_ceil() == floor);
    }
    {
        namespace reservation = linear::mmap_reservation;
        auto const saved_pool{reservation::pool_bytes};
        auto const saved_count{reservation::fixed_participants};
        reservation::pool_bytes = 2 * floor;
        reservation::fixed_participants = 2;
        linear::mmap_memory_t first{65536, linear::mmap_memory_status_t::wasm64};
        linear::mmap_memory_t second{65536, linear::mmap_memory_status_t::wasm64};
        linear::mmap_memory_t staged{65536, linear::mmap_memory_status_t::wasm64};
        CHECK(first.try_init_by_page_count(1, 1, 0));
        auto const first_state{reservation::available_pool()};
        CHECK(first_state.valid && first_state.remaining.bytes == floor && first_state.participants == 1);
        CHECK(second.try_init_by_page_count(1, 1, 1));
        CHECK(!staged.try_init_by_page_count(0, 1, 2) && staged.memory_begin == nullptr);
        linear::mmap_memory_t moved{::std::move(first)};
        CHECK(reservation::available_pool().participants == 2 && reservation::available_pool().remaining.bytes == 0);
        moved.clear();
        CHECK(staged.try_init_by_page_count(0, 1, 2));
        second.clear();
        staged.clear();
        CHECK(reservation::available_pool().remaining.bytes == 2 * floor && !reservation::available_pool().participants);
        reservation::pool_bytes = saved_pool;
        reservation::fixed_participants = saved_count;
    }
    {
        namespace reservation = linear::mmap_reservation;
        auto const saved_pool{reservation::pool_bytes};
        constexpr ::std::size_t page{65536u}, data{1uz << 28u};
        auto const partial{data + tail}, full{2u * data + tail};
        linear::mmap_memory_t memory{page, linear::mmap_memory_status_t::wasm32};
        memory.reservation_adm_bits = 4u;
        memory.reservation_manual = true;
        memory.use_planned_reservation = true;
        memory.reservation_budget_bytes = full;
        memory.reservation_limit_bytes = data;

        // Less pool space is available at acquisition than when the owner was planned.
        reservation::pool_bytes = partial - 1u;
        CHECK(!memory.try_init_by_page_count(0u, data / page, 0u));
        CHECK(!memory.memory_begin && reservation::available_pool().remaining.bytes == partial - 1u);
        reservation::pool_bytes = partial;
        CHECK(memory.try_init_by_page_count(0u, data / page, 0u));
        CHECK(!memory.is_full_page_protection() && memory.get_acquire_reserved_space_ceil() == partial);
        CHECK(memory.reservation_limit_bytes == data && memory.try_grow_silently(data / page));
        CHECK(!memory.try_grow_silently(1u));
        memory.clear();
        CHECK(reservation::available_pool().remaining.bytes == partial);
        reservation::pool_bytes = saved_pool;
    }
    {
        namespace reservation = linear::mmap_reservation;
        auto const saved_pool{reservation::pool_bytes};
        constexpr reservation::bytes_type requested{1ull << 62u};
        constexpr ::std::size_t page{65536u}, pages{3u};
        auto const owner{reservation::bounds(64u, true, 0u, pages, page, os_page, &requested, false, 2u)};
        auto const full{owner.span(UINT64_MAX)};
        CHECK(full == (1ull << 63u) + tail && owner.maximum_data == pages * page);
        // The pool admits this full window. Its 8 EiB VMA exceeds Linux user
        // address space, so the OS forces the retry at the clipped data quota.
        reservation::pool_bytes = full;
        linear::mmap_memory_t memory{page, linear::mmap_memory_status_t::wasm64};
        memory.reservation_adm_bits = 2u;
        memory.reservation_manual = true;
        memory.use_planned_reservation = true;
        memory.reservation_budget_bytes = full;
        memory.reservation_limit_bytes = static_cast<::std::size_t>(owner.maximum_data);
        CHECK(memory.try_init_by_page_count(0u, pages, 0u));
        CHECK(!memory.is_full_page_protection() && memory.get_acquire_reserved_space_ceil() == owner.minimum_span);
        CHECK(memory.try_grow_silently(pages) && !memory.try_grow_silently(1u));
        memory.clear();
        CHECK(memory.maximum_initial_page_count() == pages);
        CHECK(reservation::available_pool().remaining.bytes == full);
        reservation::pool_bytes = saved_pool;
    }
    ::fast_io::io::println("PASS memory64 declared reservation: shared/unshared, grow limit, zero maximum, move, memory32 full guard, manual ADM quota on retry");
}
