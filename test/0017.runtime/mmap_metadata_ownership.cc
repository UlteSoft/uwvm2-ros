// Small native reservations only. Define UWVM_TEST_WRAP_ALLOC and link the three
// C allocator wrappers in fast_io_allocation_probe.h to also exercise host ENOMEM.
// On Linux, UWVM_TEST_WRAP_MAPPING with --wrap=mmap,--wrap=mprotect checks the released VMA.
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/object/memory/linear/impl.h>
#include <cstdio>
#include <cstdlib>
#include <thread>
#if defined(UWVM_TEST_WRAP_ALLOC)
# include "fast_io_allocation_probe.h"
#endif
#if defined(__linux__)
# include <sys/wait.h>
# include <unistd.h>
#endif

namespace budget = uwvm2::utils::allocator::resource;
namespace faults = uwvm2::object::memory::signal;
namespace linear = uwvm2::object::memory::linear;
using memory = linear::mmap_memory_t;
constexpr auto metadata_pool = static_cast<unsigned>(budget::pool::initializer);
constexpr auto page_pool = static_cast<unsigned>(budget::pool::memory);
constexpr auto node_bytes = faults::detail::segment_registry::entry_bytes;
constexpr auto owner_bytes = sizeof(memory::owner_metadata);
#define CHECK(condition) do { if(!(condition)) { std::fprintf(stderr, "mmap ownership line %d: %s\n", __LINE__, #condition); std::abort(); } } while(false)

#if defined(__linux__) && defined(UWVM_TEST_WRAP_MAPPING)
static bool record_reservation{}, refuse_commit{};
static void* observed_reservation{};
extern "C" void* __real_mmap(void*, std::size_t, int, int, int, ::off_t) noexcept;
extern "C" void* __wrap_mmap(void* start, std::size_t size, int protection, int flags, int fd, ::off_t offset) noexcept
{
    auto* result{__real_mmap(start, size, protection, flags, fd, offset)};
    if(record_reservation && protection == PROT_NONE && size == 4u * 65536u)
    { observed_reservation = result; record_reservation = false; }
    return result;
}
extern "C" int __real_mprotect(void*, std::size_t, int) noexcept;
extern "C" int __wrap_mprotect(void* start, std::size_t size, int protection) noexcept
{
    if(refuse_commit && start == observed_reservation) { refuse_commit = false; errno = ENOMEM; return -1; }
    return __real_mprotect(start, size, protection);
}
static void check_unmapped()
{
    CHECK(!record_reservation && observed_reservation && observed_reservation != MAP_FAILED);
    unsigned char residency{};
    CHECK(::mincore(observed_reservation, 1, &residency) == -1 && errno == ENOMEM);
}
#endif

static void plan(memory& owner)
{
    owner.status = linear::mmap_memory_status_t::wasm64;
    owner.reservation_adm_bits = 4;
    owner.reservation_limit_bytes = 65536u;
    owner.use_planned_reservation = true;
    owner.reservation_budget_bytes = 4u * 65536u;
}

[[maybe_unused]] static void deny_physical(std::size_t bytes)
{
#if defined(UWVM_TEST_WRAP_ALLOC)
    fast_io_allocation_probe::selected_size = bytes;
    fast_io_allocation_probe::once = true;
    fast_io_allocation_probe::armed = true;
#else
    (void)bytes;
#endif
}

#if defined(__linux__)
static void check_cold_refusal(bool registry, bool physical)
{
    int output[2]{};
    CHECK(::pipe(output) == 0);
    auto child{::fork()};
    CHECK(child >= 0);
    if(child == 0)
    {
        ::close(output[0]);
        CHECK(::dup2(output[1], 2) == 2);
        ::close(output[1]);
        if(registry)
        {
            memory owner{};
            plan(owner);
            if(physical) { deny_physical(node_bytes); }
            else { budget::policies[metadata_pool] = {0, 0}; }
            owner.init_by_page_count(1, 1, 5);
        }
        else
        {
            if(physical) { deny_physical(owner_bytes); }
            else { budget::policies[metadata_pool] = {0, 0}; }
            memory owner{};
        }
        ::_Exit(1);
    }
    ::close(output[1]);
    char message[512]{};
    auto length{::read(output[0], message, sizeof(message)-1)};
    ::close(output[0]);
    int status{};
    CHECK(::waitpid(child, &status, 0) == child && WIFEXITED(status) && WEXITSTATUS(status) == 127);
    CHECK(length > 0 && std::strstr(message, "metadata-single-bytes/metadata-live-bytes"));
    if(registry) { CHECK(std::strstr(message, "mmap fault registry")); }
}
#endif

int main()
{
    auto const initial{budget::live_bytes[metadata_pool].load()};
    auto const pages{budget::live_bytes[page_pool].load()};
    auto const limits{budget::policies[metadata_pool]};
    auto const entries{faults::detail::segments.size()};
    auto const reserved{linear::mmap_reservation::available_pool()};
    std::byte sample{};
    CHECK(faults::detail::segments.insert({&sample, &sample + 1, nullptr, 0}, []() noexcept { return false; }) ==
          faults::registration_result::handler_unavailable);
    CHECK(budget::live_bytes[metadata_pool].load() == initial && faults::detail::segments.size() == entries);
    constexpr memory::fallible_construction_t fallible{};
    budget::policies[metadata_pool] = {owner_bytes - 1, limits.live};
    { memory refused{fallible}; CHECK(!refused.memory_length_p && !refused.growing_mutex_p); }
    budget::policies[metadata_pool] = limits;
    CHECK(budget::live_bytes[metadata_pool].load() == initial);
#if defined(UWVM_TEST_WRAP_ALLOC)
    deny_physical(owner_bytes);
    { memory refused{fallible}; CHECK(!refused.metadata && !fast_io_allocation_probe::armed); }
    CHECK(budget::live_bytes[metadata_pool].load() == initial);
#endif
    {
        memory candidate{fallible};
        CHECK(candidate.metadata && budget::live_bytes[metadata_pool].load() == initial + owner_bytes);
        plan(candidate);
#if defined(__linux__) && defined(UWVM_TEST_WRAP_MAPPING)
        record_reservation = refuse_commit = true;
        CHECK(candidate.initialize_pages(1, 1, 5) == memory::initialization_result::mapping_failure && !refuse_commit);
        check_unmapped();
        record_reservation = true;
#endif
        budget::policies[metadata_pool] = {limits.single, initial + owner_bytes + node_bytes - 1};
        CHECK(candidate.initialize_pages(1, 1, 5) == memory::initialization_result::registry_limit);
        budget::policies[metadata_pool] = limits;
#if defined(__linux__) && defined(UWVM_TEST_WRAP_MAPPING)
        check_unmapped();
#endif
#if defined(UWVM_TEST_WRAP_ALLOC)
        deny_physical(node_bytes);
        CHECK(candidate.initialize_pages(1, 1, 5) == memory::initialization_result::registry_limit);
        CHECK(!fast_io_allocation_probe::armed);
#endif
        CHECK(!candidate.reserved_begin && !candidate.memory_begin && !candidate.mapped_reservation_bytes);
        CHECK(candidate.reservation_limit_bytes == 65536u && candidate.reservation_budget_bytes == 4u * 65536u);
        CHECK(faults::detail::segments.size() == entries && budget::live_bytes[page_pool].load() == pages);
        auto const after{linear::mmap_reservation::available_pool()};
        CHECK(after.participants == reserved.participants && after.remaining.bytes == reserved.remaining.bytes &&
              after.remaining.full_address64 == reserved.remaining.full_address64);
        CHECK(budget::live_bytes[metadata_pool].load() == initial + owner_bytes);
        CHECK(candidate.try_init_by_page_count(1, 1, 5));
        memory moved{std::move(candidate)};
        memory destination{};
        plan(destination);
        CHECK(destination.try_init_by_page_count(1, 1, 0));
        destination = std::move(moved); // retire the previous destination and retain the original signal record
        CHECK(faults::detail::segments.size() == entries+1);
        CHECK(budget::live_bytes[metadata_pool].load() == initial + owner_bytes + node_bytes);
        auto const* base{destination.memory_begin};
        auto const* length{destination.memory_length_p};
        std::atomic_bool entered{}, finish{};
        std::thread reader([&]
        {
            CHECK(faults::detail::segments.any([&](auto const& segment) noexcept
            {
                if(segment.begin != base) { return false; }
                CHECK(segment.length_p == length && segment.memory_idx == 5);
                entered.store(true);
                while(!finish.load()) { std::this_thread::yield(); }
                CHECK(segment.length_p->load() == 65536u);
                return true;
            }));
        });
        while(!entered.load()) { std::this_thread::yield(); }
        std::thread retire([&] { destination.clear_destroy(); });
        // The old reader still pins the record, mapping and control. A concurrent
        // lookup, including one interrupting the retiring writer, can already finish.
        while(faults::detail::segments.size() != entries) { std::this_thread::yield(); }
        CHECK(budget::live_bytes[metadata_pool].load() == initial + owner_bytes + node_bytes);
        CHECK(budget::live_bytes[page_pool].load() > pages);
#if defined(__linux__)
        unsigned char residency{};
        CHECK(::mincore(const_cast<std::byte*>(base), 1, &residency) == 0);
#endif
        finish.store(true);
        reader.join();
        retire.join();
#if defined(__linux__)
        CHECK(::mincore(const_cast<std::byte*>(base), 1, &residency) == -1 && errno == ENOMEM);
#endif
    }
    CHECK(budget::live_bytes[metadata_pool].load() == initial && budget::live_bytes[page_pool].load() == pages);
#if defined(__linux__)
    check_cold_refusal(false, false);
    check_cold_refusal(true, false);
# if defined(UWVM_TEST_WRAP_ALLOC)
    check_cold_refusal(false, true);
    check_cold_refusal(true, true);
# endif
#endif
    std::puts("PASS mmap owner and fault registration allocation, rollback, move, and retirement");
}
