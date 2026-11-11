#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>
#include <uwvm2/extension/memtag/globals.h>
namespace mt = uwvm2::extension::memtag;
static void require(bool value) { if(!value) { ::std::abort(); } }
template<typename... Bytes>
constexpr auto payload(Bytes... values) { return ::std::array{static_cast<::std::byte>(values)...}; }
constexpr bool memory_bytes(::std::uint32_t index, ::std::uint64_t& bytes) noexcept
{
    if(index > 1u) { return false; }
    bytes=index == 0u ? 64u : 128u;
    return true;
}
constexpr bool compile_checks()
{
    auto const good{payload(2,3,0,16,16,1,32,48,1,128,1,0)};
    unsigned calls{}, indices{};
    auto const result{mt::visit_linear_globals(good,memory_bytes,[&](auto descriptor) constexpr
    { ++calls; indices+=descriptor.memory_index; })};
    if(!result || calls != 3u || indices != 2u || result.descriptor_count != 3u) { return false; }
    calls=0u;
    auto const bad{payload(2,2,0,16,16,1,128,1,1)};
    auto const rejected{mt::visit_linear_globals(bad,memory_bytes,[&](auto) constexpr {++calls;})};
    if(rejected.error != mt::globals_error::memory_out_of_bounds || calls) { return false; }
    // Equal module indices on different owners do not overlap. Different
    // indices that import one owner are represented by the same identity.
    ::std::array<mt::global_tag_range,3> disjoint{{{9,1,1},{7,0,0},{7,1,2}}};
    if(!mt::validate_global_tag_ranges(disjoint)) { return false; }
    ::std::array<mt::global_tag_range,2> alias{{{7,1,1},{7,1,2}}};
    if(mt::validate_global_tag_ranges(alias)) { return false; }
    ::std::array<mt::global_tag_range,3> nested{{{7,0,20},{7,1,1},{7,10,10}}};
    if(mt::validate_global_tag_ranges(nested)) { return false; }
    ::std::array<mt::global_tag_range,3> reverse{{{7,10,10},{7,1,1},{7,0,20}}};
    return !mt::validate_global_tag_ranges(reverse);
}
static_assert(compile_checks());
int main()
{
    require(compile_checks());
    require(mt::validate_linear_globals(payload(1,0),memory_bytes).error == mt::globals_error::unsupported_version);
    require(mt::validate_linear_globals(payload(2,0,0),memory_bytes).error == mt::globals_error::trailing_bytes);
    require(mt::validate_linear_globals(payload(2,2,0,0,0),memory_bytes).error == mt::globals_error::impossible_count);
    require(mt::validate_linear_globals(payload(2,1,2,0,0),memory_bytes).error == mt::globals_error::invalid_memory_index);
    require(mt::validate_linear_globals(payload(2,1,255,255,255,255,15,0,0),memory_bytes).error == mt::globals_error::invalid_memory_index);
    require(mt::validate_linear_globals(payload(2,1,128,128,128,128,16,0,0),memory_bytes).error == mt::globals_error::integer_overflow);
    require(mt::validate_linear_globals(payload(2,1,128,128,128,128,128,0,0,0),memory_bytes).error == mt::globals_error::integer_overflow);
    require(static_cast<bool>(mt::validate_linear_globals(payload(2,1,128,128,128,128,0,0,0),memory_bytes)));
    require(mt::validate_linear_globals(payload(2,1,0,255,255,255,255,255,255,255,255,255,2,0),memory_bytes).error == mt::globals_error::integer_overflow);
    auto const maximal{[](::std::uint32_t index,::std::uint64_t& bytes) noexcept
    {bytes=~::std::uint64_t{};return index==0u;}};
    require(mt::validate_linear_globals(payload(2,1,0,255,255,255,255,255,255,255,255,255,1,1),maximal).error == mt::globals_error::memory_out_of_bounds);
    require(static_cast<bool>(mt::validate_linear_globals(payload(2,1,0,255,255,255,255,255,255,255,255,255,1,0),maximal)));
    auto const good{payload(2,3,0,16,16,1,32,48,1,128,1,0)};
    for(::std::size_t length{}; length != good.size(); ++length)
    {
        unsigned calls{};
        require(!mt::visit_linear_globals(::std::span(good).first(length),memory_bytes,[&](auto){++calls;}));
        require(calls == 0u);
    }
    ::std::array<mt::global_tag_range,2> touching{{{7,0,2},{7,2,3}}};
    require(!mt::validate_global_tag_ranges(touching));
    // Bounded malformed input must never partially publish descriptors.
    ::std::uint64_t state{0x31f732a104deull};
    ::std::array<::std::byte,96> fuzz{};
    for(unsigned iteration{}; iteration != 30000u; ++iteration)
    {
        for(auto& byte:fuzz) {state=state*6364136223846793005ull+1u;byte=static_cast<::std::byte>(state>>32u);}
        if(iteration&1u) {fuzz[0]=::std::byte{2};fuzz[1]=static_cast<::std::byte>(iteration%20u);}
        unsigned calls{};
        auto const result{mt::visit_linear_globals(::std::span(fuzz).first(iteration%97u),memory_bytes,[&](auto descriptor)
        {
            ++calls; ::std::uint64_t bytes{};
            require(memory_bytes(descriptor.memory_index,bytes));
            require(descriptor.address<=bytes && descriptor.byte_length<=bytes-descriptor.address);
        })};
        require(result ? calls==result.descriptor_count : calls==0u);
    }
    ::std::puts("PASS linear globals v2: bounded decoding, per-memory ranges, atomic validation and canonical granule overlap");
}
