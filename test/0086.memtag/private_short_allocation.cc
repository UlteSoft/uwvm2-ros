// Compare the native private shortcut with its complete diagnostic fallback.
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/runtime/memtag/impl.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>

namespace mt = uwvm2::extension::memtag;
namespace rt = uwvm2::runtime::memtag;
namespace linear = uwvm2::object::memory::linear;
using op = mt::opcode;

static void check(bool ok, unsigned line)
{
    if(!ok) { std::fprintf(stderr,"private short allocation assertion at %u\n",line); std::abort(); }
}
#define CHECK(...) check(static_cast<bool>((__VA_ARGS__)),__LINE__)

#if defined(__linux__)
static void buffer_word(std::uint16_t word)
{
    auto& buffer{rt::details::entropy_buffer};
    buffer.bytes.fill(0xffu);
    buffer.bytes[0]=static_cast<std::uint8_t>(word);
    buffer.bytes[1]=static_cast<std::uint8_t>(word >> 8u);
    buffer.position=0u;
}

template<typename Address>
static void cases()
{
    using view = mt::shadow_view<Address,false>;
    constexpr auto width{sizeof(Address)==4u ? mt::address_width::i32 : mt::address_width::i64};
    constexpr Address address{24u*16u};
    constexpr auto status{sizeof(Address)==4u ? linear::mmap_memory_status_t::wasm32 : linear::mmap_memory_status_t::wasm64};
    linear::mmap_memory_t candidate{65536u,status},reference{65536u,status};
    for(auto* owner : {&candidate,&reference})
    {
        owner->init_by_page_count(1,1);
        CHECK(owner->enable_memtag(width,false) == mt::storage_error::none);
        auto& directory{owner->memtag_shadow.allocation_directory()};
        auto const shadow{owner->memtag_shadow.template view<Address>(65536u)};
        // The left window begins in a reservation that started earlier. The
        // right neighbor has an issued color and a distinct raw backing color.
        CHECK(directory.add_private({17,20,2,0,false,false}));
        CHECK(directory.add_private({32,32,4,0,true,false}));
        CHECK(shadow.store(view::insert(32u*16u,6u),16u));
        CHECK(shadow.store(view::insert(21u*16u,3u),16u));
    }
    auto& directory{candidate.memtag_shadow.allocation_directory()};
    auto const shadow{candidate.memtag_shadow.template view<Address>(65536u)};
    auto& used{rt::details::entropy_buffer.position};
    for(Address length{16u}; length<=128u; length+=16u)
    {
        // Include a rejected zero word and ranks throughout the legal palette.
        for(unsigned sample{}; sample<=256u; ++sample)
        {
            auto const word{static_cast<std::uint16_t>((65535u*sample)/256u)};
            buffer_word(word);
            auto const expected{rt::execute_private_allocation<op::claimz>(reference,address,length)};
            auto const expected_used{used};
            CHECK(expected);
            buffer_word(word);
            std::fill_n(candidate.memory_begin+address-1u,length+2u,std::byte{0x5a});
            Address tagged{};
            auto const quick{rt::try_short_private_allocation<op::claimz>(candidate,address,length,tagged)};
            CHECK(quick == (expected_used == 2u));
            if(!quick)
            {
                CHECK(used==0u && directory.exact(24u,24u+length/16u-1u)==nullptr && shadow.check(address,0u,length));
                CHECK(candidate.memory_begin[address]==std::byte{0x5a});
                auto const complete{rt::execute_private_allocation<op::claimz>(candidate,address,length)};
                CHECK(complete);
                tagged=complete.value;
            }
            CHECK(tagged==expected.value && used==expected_used && shadow.check(tagged,0u,length));
            CHECK(std::all_of(candidate.memory_begin+address,candidate.memory_begin+address+length,
                              [](std::byte b) { return b==std::byte{}; }));
            CHECK(candidate.memory_begin[address-1u]==std::byte{0x5a} && candidate.memory_begin[address+length]==std::byte{0x5a});
            Address parent{};
            CHECK(rt::try_short_private_allocation<op::release>(candidate,tagged,length,parent));
            CHECK(parent==address && shadow.check(address,0u,length) && used==expected_used);
            CHECK(rt::execute_private_allocation<op::release>(reference,expected.value,length));
        }
    }

    // Failures precede all mutations and the general transition locates the
    // first bad granule even when another tag is wrong further into the range.
    buffer_word(0xffffu);
    Address tagged{},unused{};
    CHECK(rt::try_short_private_allocation<op::claim>(candidate,address,Address{64u},tagged));
    auto const* record{directory.exact(24u,27u)};
    CHECK(record);
    for(unsigned first{}; first!=4u; ++first)
    {
        CHECK(shadow.store(address+16u*first,64u-16u*first));
        CHECK(!rt::try_short_private_allocation<op::release>(candidate,tagged,Address{64u},unused));
        auto const failure{rt::execute_private_allocation<op::release>(candidate,tagged,Address{64u})};
        CHECK(failure.error==mt::operation_error::memory_access && failure.access.error==mt::access_error::tag_mismatch);
        CHECK(failure.access.address==address && failure.access.mismatch_granule==address+16u*first);
        CHECK(failure.access.pointer_tag==view::extract(tagged) && failure.access.allocation_tag==0u);
        CHECK(directory.exact(24u,27u)==record && record->active && used==2u);
        CHECK(shadow.store(tagged,64u));
    }
    CHECK(!rt::try_short_private_allocation<op::release>(candidate,tagged,Address{48u},unused));
    CHECK(rt::try_short_private_allocation<op::release>(candidate,tagged,Address{64u},unused));
    CHECK(!rt::try_short_private_allocation<op::release>(candidate,tagged,Address{64u},unused));
    CHECK(directory.add_private({35u,37u,7u,9u,true,false}));
    auto const child{view::insert(35u*16u,7u)}, parent{view::insert(35u*16u,9u)};
    CHECK(shadow.store(child,48u));
    CHECK(rt::try_short_private_allocation<op::release>(candidate,child,Address{48u},unused));
    CHECK(unused==parent && shadow.check(parent,0u,48u) && !directory.exact(35u,37u));
    // Inactive reservations, cross-page neighborhoods, missing pages and a
    // possible tree intersection all retain the original lifecycle path.
    CHECK(!rt::try_short_private_allocation<op::release>(candidate,view::insert(17u*16u,2u),Address{64u},unused));
    CHECK(!rt::try_short_private_allocation<op::claim>(candidate,Address{63u*16u},Address{32u},unused));
    CHECK(!rt::try_short_private_allocation<op::claim>(candidate,Address{80u*16u},Address{16u},unused));
    CHECK(directory.add_permanent(23u,23u,7u));
    CHECK(!rt::try_short_private_allocation<op::claim>(candidate,address,Address{16u},unused));
    CHECK(shadow.check(address,0u,64u) && used==2u);
}

template<typename Address>
static void reservations()
{
    constexpr auto width{sizeof(Address)==4u ? mt::address_width::i32 : mt::address_width::i64};
    constexpr unsigned bits{sizeof(Address)*8u};
    constexpr auto status{sizeof(Address)==4u ? linear::mmap_memory_status_t::wasm32 : linear::mmap_memory_status_t::wasm64};
    constexpr Address base{56u*16u}, input{(Address{1} << (bits-1u)) | base};
    linear::mmap_memory_t candidate{65536u,status},reference{65536u,status};
    for(auto* owner : {&candidate,&reference})
    {
        owner->init_by_page_count(1,1);
        CHECK(owner->enable_memtag(width,false,16u,mt::owner_backend::software,bits-8u) == mt::storage_error::none);
        auto& directory{owner->memtag_shadow.allocation_directory()};
        auto const view{owner->memtag_shadow.template view<Address>(65536u)};
        CHECK(directory.add_private({50,52,2,0,false,false})); // starts before the exclusion window
        CHECK(directory.add_private({65,65,4,0,true,false})); // next page, distinct issued and backing colors
        CHECK(view.store(view.with_tag(65u*16u,6u),16u));
    }
    auto const view{candidate.memtag_shadow.template view<Address>(65536u)};
    auto& directory{candidate.memtag_shadow.allocation_directory()};
    auto& used{rt::details::entropy_buffer.position};
    std::fill_n(candidate.memory_begin+base-1u,130u,std::byte{0x5a});
    for(Address length{16u}; length<=128u; length+=16u)
    {
        for(auto word : {0u,0x1234u,0xffffu})
        {
            buffer_word(static_cast<std::uint16_t>(word));
            auto const expected{rt::execute_private_allocation<op::reserve>(reference,input,length)};
            CHECK(expected);
            auto const expected_used{used};
            buffer_word(static_cast<std::uint16_t>(word));
            Address tagged{};
            auto const quick{rt::try_short_private_allocation<op::reserve>(candidate,input,length,tagged)};
            CHECK(quick == (expected_used == 2u));
            if(!quick)
            {
                CHECK(!used && !directory.exact(56u,56u+length/16u-1u) && view.check(input,0u,length));
                auto const full{rt::execute_private_allocation<op::reserve>(candidate,input,length)};
                CHECK(full);
                tagged=full.value;
            }
            auto const* item{directory.exact(56u,56u+length/16u-1u)};
            CHECK(tagged==expected.value && used==expected_used && item && !item->active);
            CHECK(view.check(input,0u,length) && !view.check(tagged,0u,length));
            Address result{};
            if(word==0xffffu)
            {
                for(Address offset{};offset<length;offset+=16u)
                {
                    CHECK(view.store(view.with_tag(base+offset,7u),16u));
                    CHECK(!rt::try_short_private_allocation<op::activate>(candidate,tagged,length,result));
                    auto const failure{rt::execute_private_allocation<op::activate>(candidate,tagged,length)};
                    CHECK(failure.error==mt::operation_error::memory_access && failure.access.error==mt::access_error::tag_mismatch);
                    CHECK(failure.access.mismatch_granule==base+offset && !item->active && used==expected_used);
                    CHECK(view.store(base+offset,16u));
                }
            }
            CHECK(rt::try_short_private_allocation<op::activate>(candidate,tagged,length,result));
            CHECK(rt::execute_private_allocation<op::activate>(reference,expected.value,length));
            CHECK(result==tagged && item->active && view.check(tagged,0u,length));
            CHECK(view.load(base-16u).allocation_tag==0u && view.load(base+length).allocation_tag==0u);
            CHECK(!rt::try_short_private_allocation<op::activate>(candidate,tagged,length,result));
            CHECK(rt::try_short_private_allocation<op::release>(candidate,tagged,length,result));
            CHECK(rt::execute_private_allocation<op::release>(reference,expected.value,length));
            CHECK(result==input && view.check(input,0u,length) && !view.check(tagged,0u,length) && used==expected_used);
        }
    }
    CHECK(std::all_of(candidate.memory_begin+base-1u,candidate.memory_begin+base+129u,
                      [](std::byte b) { return b==std::byte{0x5a}; }));
    buffer_word(0u);
    rt::details::entropy_buffer.bytes.fill(0u);
    Address result{};
    CHECK(!rt::try_short_private_allocation<op::reserve>(candidate,input,Address{16u},result) && used==0u);
    auto const exhausted{rt::execute_private_allocation<op::reserve>(candidate,input,Address{16u})};
    CHECK(exhausted.error==mt::operation_error::allocation_entropy && used==32u && !directory.exact(56u,56u));
}
#endif

int main()
{
#if defined(__linux__)
    cases<std::uint32_t>();
    cases<std::uint64_t>();
    reservations<std::uint32_t>();
    reservations<std::uint64_t>();
#endif
}
#include <uwvm2/utils/macro/pop_macros.h>
