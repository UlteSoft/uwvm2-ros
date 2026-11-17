/** @file
 * @brief Separate unchanged XML serialization from bounded output transport.
 * @details This diagnostic keeps the frozen full-workflow baseline unchanged.
 * Serialize mode runs the same upstream parser/printer and content hash without
 * a host-output call. Transport mode prepares identical bytes before timing and
 * executes the complete authorized begin/append/commit/read/remove workflow.
 * Both validate all fourteen outputs. Their times are diagnostic components,
 * not additive accounting or substitutes for the complete-workflow benchmark.
 */
#define ARTIFACT_OUTPUT_BENCHMARK
#define main output_correctness_main
#define _start output_correctness_start
#include "upstream_artifact_output.cpp"
#undef main
#undef _start
#include <ctime>

#ifndef OUTPUT_PHASE
#error OUTPUT_PHASE must be 1 (serialize) or 2 (transport)
#endif
#ifndef OUTPUT_ITERATIONS
#define OUTPUT_ITERATIONS 1000
#endif
#ifndef OUTPUT_BATCHES
#define OUTPUT_BATCHES 31
#endif
#ifndef OUTPUT_CHUNK
#define OUTPUT_CHUNK 4096
#endif
static std::vector<std::vector<unsigned char>> prepared_bytes;
static std::uint64_t clock_now()
{
    timespec value{};require(clock_gettime(CLOCK_MONOTONIC,&value)==0);
    return static_cast<std::uint64_t>(value.tv_sec)*1000000000ULL+value.tv_nsec;
}
__attribute__((noinline)) static std::uint64_t phase_cycle()
{
    std::uint64_t checksum{};unsigned index{};
    for(auto const& fixture:output_fixtures)for(bool compact:{false,true})
    {
        auto const& expected=output_expectations[index];
#if OUTPUT_PHASE==1
        auto bytes=serialize(fixture,compact);
#elif OUTPUT_PHASE==2
        static_cast<void>(fixture);static_cast<void>(compact);
        auto const& source=prepared_bytes[index];
        auto result=artifact_output_transport::roundtrip(index+1,source.data(),source.size(),expected.sha256,OUTPUT_CHUNK);
        require(!result.denied&&result.published&&!result.durable&&result.bytes==source);
        auto const& bytes=result.bytes;
#else
#error Unknown phase
#endif
        require(bytes.size()==expected.size);
        auto hash=byte_hash(bytes.data(),bytes.size());require(hash==expected.hash);
        checksum^=hash+bytes.size();checksum=(checksum<<7)|(checksum>>(64-7));++index;
    }
    return checksum;
}
static int benchmark()
{
#if OUTPUT_PHASE==2
    for(auto const& fixture:output_fixtures)for(bool compact:{false,true})prepared_bytes.push_back(serialize(fixture,compact));
#endif
    auto expected=phase_cycle();for(unsigned warm{};warm!=100;++warm)require(phase_cycle()==expected);
    std::uint64_t samples[OUTPUT_BATCHES]{},checksum{};
    for(unsigned batch{};batch!=OUTPUT_BATCHES;++batch)
    {
        auto start=clock_now();
        for(unsigned iteration{};iteration!=OUTPUT_ITERATIONS;++iteration)
        {auto value=phase_cycle();require(value==expected);checksum+=value;}
        samples[batch]=clock_now()-start;
    }
    std::printf("phase=%u iterations=%u batches=%u outputs=14 chunk=%u expected=%llu checksum=%llu\n",
        unsigned(OUTPUT_PHASE),unsigned(OUTPUT_ITERATIONS),unsigned(OUTPUT_BATCHES),unsigned(OUTPUT_CHUNK),
        static_cast<unsigned long long>(expected),static_cast<unsigned long long>(checksum));
    for(auto sample:samples)std::printf("elapsed_ns=%llu\n",static_cast<unsigned long long>(sample));
    require(std::fflush(stdout)==0);return 0;
}
#if defined(__wasm__)
extern "C" void _start(){require(benchmark()==0);}
#else
int main(){return benchmark();}
#endif
