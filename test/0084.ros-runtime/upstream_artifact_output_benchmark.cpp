/** @file
 * @brief Compare the same upstream XML serializer and complete volatile output.
 * @details Native uses finite staged storage plus an explicit readback copy.
 * Wasm begins, appends, publishes, reads and removes one explicitly granted
 * volatile cache artifact at a time. Deliberate retry/query stress is disabled
 * for timing; the full correctness fixture qualifies those paths separately.
 * Publication is process-local, never crash-durable. Timing excludes compiler
 * startup and reporting; profiler collection covers the complete process.
 */
#define ARTIFACT_OUTPUT_BENCHMARK
#define main output_correctness_main
#define _start output_correctness_start
#include "upstream_artifact_output.cpp"
#undef main
#undef _start
#include <ctime>

#ifndef OUTPUT_ITERATIONS
#define OUTPUT_ITERATIONS 1000
#endif
#ifndef OUTPUT_BATCHES
#define OUTPUT_BATCHES 31
#endif
#ifndef OUTPUT_CHUNK
#define OUTPUT_CHUNK 4096
#endif

static std::uint64_t now()
{
    timespec value{};require(clock_gettime(CLOCK_MONOTONIC,&value)==0);
    return static_cast<std::uint64_t>(value.tv_sec)*1000000000ULL+value.tv_nsec;
}

__attribute__((noinline)) static std::uint64_t output_cycle()
{
    std::uint64_t checksum{};unsigned index{};
    for(auto const& fixture:output_fixtures)for(bool compact:{false,true})
    {
        auto bytes=serialize(fixture,compact);auto const& expected=output_expectations[index++];
        require(bytes.size()==expected.size);
        auto result=artifact_output_transport::roundtrip(index,bytes.data(),bytes.size(),expected.sha256,OUTPUT_CHUNK);
        require(!result.denied&&result.published&&!result.durable&&result.bytes==bytes);
        auto hash=byte_hash(result.bytes.data(),result.bytes.size());require(hash==expected.hash);
        checksum^=hash+result.bytes.size();checksum=(checksum<<7)|(checksum>>(64-7));
    }
    return checksum;
}

static int benchmark()
{
    auto expected=output_cycle();for(unsigned warm{};warm!=100;++warm)require(output_cycle()==expected);
    std::uint64_t samples[OUTPUT_BATCHES]{},checksum{};
    for(unsigned batch{};batch!=OUTPUT_BATCHES;++batch)
    {
        auto start=now();
        for(unsigned iteration{};iteration!=OUTPUT_ITERATIONS;++iteration)
        {auto value=output_cycle();require(value==expected);checksum+=value;}
        samples[batch]=now()-start;
    }
    std::printf("iterations=%u batches=%u outputs=%u chunk=%u expected=%llu checksum=%llu\n",
        unsigned(OUTPUT_ITERATIONS),unsigned(OUTPUT_BATCHES),14u,unsigned(OUTPUT_CHUNK),
        static_cast<unsigned long long>(expected),static_cast<unsigned long long>(checksum));
    for(auto sample:samples)std::printf("elapsed_ns=%llu\n",static_cast<unsigned long long>(sample));
    require(std::fflush(stdout)==0);return 0;
}

#if defined(__wasm__)
extern "C" void _start(){require(benchmark()==0);}
#else
int main(){return benchmark();}
#endif
