#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <span>
#include <uwvm2/extension/memtag/globals.h>
namespace mt = uwvm2::extension::memtag;
static void require(bool value) { if(!value) { ::std::abort(); } }
template<typename... Bytes>
constexpr auto payload(Bytes... values) { return ::std::array{static_cast<::std::byte>(values)...}; }
constexpr bool compile_checks()
{
    auto const good{payload(1,2,16,32,128,1,16)};
    auto const result{mt::validate_globals(good, 144)};
    if(!result || result.descriptor_count != 2u) { return false; }
    unsigned count{};
    auto const visited{mt::visit_globals(good,144,[&](auto entry) constexpr
    { if(entry.byte_length != 0u) { ++count; } })};
    if(!visited || count != 2u) { return false; }
    auto const bad{payload(1,2,16,32,128,1,17)};
    count=0u;
    auto const rejected{mt::visit_globals(bad,144,[&](auto) constexpr {++count;})};
    return rejected.error == mt::globals_error::memory_out_of_bounds && count == 0u;
}
static_assert(compile_checks());
int main()
{
    require(compile_checks());
    require(mt::validate_globals(payload(2,0),0).error == mt::globals_error::unsupported_version);
    require(mt::validate_globals(payload(1,0,0),0).error == mt::globals_error::trailing_bytes);
    require(mt::validate_globals(payload(1,3,0,0),0).error == mt::globals_error::impossible_count);
    require(mt::validate_globals(payload(1,1,128,128),1024).error == mt::globals_error::truncated);
    require(mt::validate_globals(payload(1,1,255,255,255,255,255,255,255,255,255,2,1),1024).error == mt::globals_error::integer_overflow);
    require(mt::validate_globals(payload(1,1,255,255,255,255,255,255,255,255,255,1,1),
        ~::std::uint64_t{}).error == mt::globals_error::memory_out_of_bounds);
    require(static_cast<bool>(mt::validate_globals(payload(1,1,255,255,255,255,255,255,255,255,255,1,0),~::std::uint64_t{})));
    auto const good{payload(1,2,16,32,128,1,16)};
    for(::std::size_t length{}; length != good.size(); ++length)
    {
        unsigned calls{};
        require(!mt::visit_globals(::std::span(good).first(length),144,[&](auto){++calls;}));
        require(calls == 0u);
    }
    // Fuzz arbitrary bounded sections: all callback ranges must be valid and
    // malformed input must never invoke the callback even once.
    ::std::uint64_t state{0x98764fab};
    ::std::array<::std::byte,96> fuzz{};
    for(unsigned iteration{}; iteration != 30000u; ++iteration)
    {
        for(auto& byte:fuzz) {state=state*6364136223846793005ull+1u;byte=static_cast<::std::byte>(state>>32u);}
        if(iteration&1u) {fuzz[0]=::std::byte{1};fuzz[1]=static_cast<::std::byte>(iteration%20u);}
        unsigned calls{};
        auto const result{mt::visit_globals(::std::span(fuzz).first(iteration%97u),4096u,[&](auto entry)
        {++calls;require(entry.address<=4096u && entry.byte_length<=4096u-entry.address);})};
        if(!result) {require(calls==0u);} else {require(calls==result.descriptor_count);}
    }
}
