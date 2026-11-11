// Startup-component benchmark, not a VM execution-speed benchmark.
// Build: clang++ -std=c++23 -O2 -I src test/0086.memtag/globals_v2_bench.cc -o globals_v2_bench
// Run: ./globals_v2_bench [odd_round_count>=5, default 31]
// Fixed inputs exercise 8 module indices aliasing 4 canonical owners. The last
// descriptor of alias-last deliberately targets the first descriptor through
// another module index. Parsing remains valid; granule planning must reject.
// Excludes loader work, actual alias-chain resolution, allocation-tag storage,
// entropy, tag writes and hardware setup. Buffers are reserved before timing.
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>
#include <vector>
#include <uwvm2/extension/memtag/globals.h>
namespace mt=uwvm2::extension::memtag;
using bytes=::std::vector<::std::byte>;
static volatile ::std::uint64_t observed{};
static void require(bool value) { if(!value) { ::std::abort(); } }
static void uleb(bytes& output,::std::uint64_t value)
{
    do
    {
        auto byte{static_cast<unsigned char>(value&127u)};
        value>>=7u;
        if(value) {byte|=128u;}
        output.push_back(static_cast<::std::byte>(byte));
    } while(value);
}
static bytes input(::std::size_t count,bool overlap)
{
    bytes result{};result.reserve(count*8u+16u);
    uleb(result,mt::linear_globals_section_version);uleb(result,count);
    for(::std::size_t index{};index!=count;++index)
    {
        auto const alias{overlap && index+1u==count};
        auto const memory_index{alias ? 1u : index%8u};
        auto const address{alias ? 0u : (index/8u*2u+index%2u)*16u};
        uleb(result,memory_index);uleb(result,address);uleb(result,16u);
    }
    return result;
}
template<typename Function>
static void measure(char const* phase,::std::size_t count,bool overlap,::std::size_t encoded_bytes,
                    unsigned rounds,Function&& function)
{
    ::std::vector<::std::uint64_t> times{};times.reserve(rounds);
    for(unsigned round{};round!=rounds+3u;++round)
    {
        function(false); // Untimed preparation (notably restoring sort input).
        ::std::atomic_signal_fence(::std::memory_order_seq_cst);
        auto const start{::std::chrono::steady_clock::now()};
        auto const accepted{function(true)};
        ::std::atomic_signal_fence(::std::memory_order_seq_cst);
        auto const end{::std::chrono::steady_clock::now()};
        observed=static_cast<::std::uint64_t>(accepted)+count;
        if(round>=3u)
        {
            times.push_back(static_cast<::std::uint64_t>(
                ::std::chrono::duration_cast<::std::chrono::nanoseconds>(end-start).count()));
        }
    }
    ::std::sort(times.begin(),times.end());
    auto const median{times[rounds/2u]};
    ::std::printf("%s,%zu,%s,%zu,%u,%llu,%llu,%llu,%.3f\n",phase,count,
        overlap?"alias-last":"valid",encoded_bytes,rounds,
        static_cast<unsigned long long>(median),
        static_cast<unsigned long long>(times[rounds/10u]),
        static_cast<unsigned long long>(times[rounds*9u/10u]),
        static_cast<double>(median)/static_cast<double>(count));
}
int main(int argc,char** argv)
{
    unsigned rounds{31u};
    if(argc>1) {rounds=static_cast<unsigned>(::std::strtoul(argv[1],nullptr,10));}
    require(rounds>=5u && rounds<=1001u && (rounds&1u));
    ::std::puts("phase,descriptors,case,encoded_bytes,rounds,median_ns,p10_ns,p90_ns,median_ns_per_descriptor");
    for(::std::size_t count:{1024u,4096u,16384u,65536u})
    {
        for(bool overlap:{false,true})
        {
            auto const encoded{input(count,overlap)};
            auto const lookup{[=](::std::uint32_t index,::std::uint64_t& memory_bytes) noexcept
            {memory_bytes=count*16u;return index<8u;}};
            ::std::vector<mt::global_tag_range> ranges{},unsorted{};
            ranges.reserve(count);unsorted.reserve(count);
            auto append{[&](mt::linear_global_descriptor descriptor)
            {
                ranges.push_back({descriptor.memory_index/2u+1u,
                    descriptor.address>>4u,(descriptor.address+descriptor.byte_length-1u)>>4u});
            }};
            require(static_cast<bool>(mt::visit_linear_globals(encoded,lookup,append)));
            unsorted=ranges;
            require(mt::validate_global_tag_ranges(ranges)==!overlap);
            measure("parse",count,overlap,encoded.size(),rounds,[&](bool timed)
            {
                if(!timed) {return true;}
                auto const result{mt::validate_linear_globals(encoded,lookup)};
                require(static_cast<bool>(result));
                return static_cast<bool>(result);
            });
            measure("sort",count,overlap,encoded.size(),rounds,[&](bool timed)
            {
                if(!timed) {ranges=unsorted;return true;}
                auto const accepted{mt::validate_global_tag_ranges(ranges)};
                require(accepted==!overlap);return accepted;
            });
            measure("admission-plan",count,overlap,encoded.size(),rounds,[&](bool timed)
            {
                if(!timed) {ranges.clear();return true;}
                auto const checked{mt::visit_linear_globals(encoded,lookup,append)};
                require(static_cast<bool>(checked));
                auto const accepted{mt::validate_global_tag_ranges(ranges)};
                require(accepted==!overlap);return accepted;
            });
        }
    }
    require(observed!=0u);
}
