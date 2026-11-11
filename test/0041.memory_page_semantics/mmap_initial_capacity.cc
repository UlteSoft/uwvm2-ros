// Geometry queries never reserve a VMA. The executable allocation checks use
// at most three small pages and an explicitly bounded reservation. In the
// native64 memory32 case, byte/16-byte pages retain software bounds and avoid
// the default full-domain 8 GiB virtual reservation. This is a backend capacity
// regression test, not a promise that the OS can allocate the theoretical cap.
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/object/memory/linear/impl.h>
#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>

#if !defined(UWVM_SUPPORT_MMAP)
# error "mmap_initial_capacity must exercise the actual mmap backend"
#endif
namespace linear=::uwvm2::object::memory::linear;
static ::std::size_t checks{}, queries{}, initializations{};
static void require(bool value,unsigned line)
{
    ++checks;
    if(!value)
    {
        ::std::fprintf(stderr,"mmap capacity failed at line %u, check %zu\n",line,checks);
        ::std::abort();
    }
}
#define CHECK(...) require(static_cast<bool>((__VA_ARGS__)),__LINE__)
static constexpr auto native_bits{::std::numeric_limits<::std::size_t>::digits};
static constexpr auto native_max{(::std::numeric_limits<::std::size_t>::max)()};

static void query_geometry(linear::mmap_memory_status_t status)
{
    // Independent byte-domain oracle: use division, not the implementation's
    // shift/count expression. Both 32-bit-host address domains share 256 MiB.
    auto const domain_bytes{native_bits==32 ? (::std::uint64_t{1}<<28u) :
        (::std::uint64_t{1}<<(status==linear::mmap_memory_status_t::wasm32 ? 32u : 40u))};
    for(unsigned log2{};log2<static_cast<unsigned>(native_bits);++log2)
    {
        auto const page_bytes{::std::size_t{1}<<log2};
        linear::mmap_memory_t memory{page_bytes,status};
        ::std::array<::std::size_t,8> limits{
            0u,1u,page_bytes-1u,page_bytes,page_bytes+1u,65535u,
            static_cast<::std::size_t>(domain_bytes),native_max};
        for(auto const limit:limits)
        {
            memory.reservation_limit_bytes=limit;
            auto const bytes{static_cast<::std::uint64_t>(limit)<domain_bytes ?
                static_cast<::std::uint64_t>(limit) : domain_bytes};
            auto const expected{bytes/static_cast<::std::uint64_t>(page_bytes)};
            auto const actual{memory.maximum_initial_page_count()};
            ++queries;
            CHECK(actual==expected);
            CHECK(memory.reserved_begin==nullptr && memory.memory_begin==nullptr);
            CHECK(memory.memory_length_p->load(::std::memory_order_relaxed)==0u);
            CHECK(memory.reservation_limit_bytes==limit && memory.custom_page_size_log2==log2 && memory.status==status);
        }
        memory.custom_page_size_log2=static_cast<unsigned>(native_bits);
        CHECK(memory.maximum_initial_page_count()==0u);
        memory.custom_page_size_log2=(::std::numeric_limits<unsigned>::max)();
        CHECK(memory.maximum_initial_page_count()==0u);
        memory.custom_page_size_log2=log2;
    }
}

static void small_initialization(linear::mmap_memory_status_t status)
{
    for(::std::size_t page_bytes:{1u,16u,4096u,65536u})
    {
        bool const native64_memory32{native_bits>32 && status==linear::mmap_memory_status_t::wasm32};
        if(native64_memory32 && page_bytes>=4096u) {continue;}
        for(auto const [initial,capacity]:{::std::array<::std::size_t,2>{0u,0u},
                                          ::std::array<::std::size_t,2>{0u,3u},
                                          ::std::array<::std::size_t,2>{2u,3u},
                                          ::std::array<::std::size_t,2>{3u,3u}})
        {
            linear::mmap_memory_t memory{page_bytes,status};
            if(native64_memory32)
            {
                CHECK(memory.require_dynamic_determination_memory_size());
                memory.reservation_limit_bytes=capacity*page_bytes;
            }
            auto const before{memory.maximum_initial_page_count()};
            CHECK(capacity<=before);
            memory.init_by_page_count(initial,capacity,17u);
            ++initializations;
            CHECK(memory.reserved_begin!=nullptr && memory.memory_begin!=nullptr);
            CHECK(memory.maximum_initial_page_count()==capacity);
            CHECK(memory.reservation_limit_bytes==capacity*page_bytes);
            CHECK(memory.get_page_size()==initial);
            CHECK(memory.memory_length_p->load(::std::memory_order_relaxed)==initial*page_bytes);
            CHECK(memory.diagnostic_owner_memory_index==17u);
            auto* const base{memory.memory_begin};
            if(initial) {CHECK(base[0]==::std::byte{});base[0]=::std::byte{0x57};}
            CHECK(memory.try_grow_silently(capacity-initial,native_max));
            CHECK(memory.get_page_size()==capacity && memory.memory_begin==base);
            if(capacity)
            {
                CHECK(base[0]==(initial ? ::std::byte{0x57} : ::std::byte{}));
                if(capacity*page_bytes>1u) {CHECK(base[capacity*page_bytes-1u]==::std::byte{});}
            }
            // Even a caller supplying SIZE_MAX cannot grow beyond the actual
            // owner reservation. Failure preserves bytes, base and length.
            CHECK(!memory.try_grow_silently(1u,native_max));
            CHECK(memory.get_page_size()==capacity && memory.memory_begin==base);
            CHECK(memory.maximum_initial_page_count()==capacity);
        }
    }
}
int main()
{
    for(auto status:{linear::mmap_memory_status_t::wasm32,linear::mmap_memory_status_t::wasm64})
    {
        query_geometry(status);
        small_initialization(status);
    }
    ::std::printf("PASS mmap initial capacity: native_bits=%u queries=%zu small_initializations=%zu checks=%zu\n",
        static_cast<unsigned>(native_bits),queries,initializations,checks);
}
#include <uwvm2/utils/macro/pop_macros.h>
