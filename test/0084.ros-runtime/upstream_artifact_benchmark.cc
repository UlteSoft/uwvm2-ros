/** @file
 * @brief Compare immutable input loading and the same upstream XML parser.
 * @details Native snapshots are loaded before timing and copied in identical
 * bounded chunks; Wasm reads deployment-sealed artifact handles. Each cycle
 * parses all seven correctness inputs. Timing excludes initialization, guest
 * compilation and reporting. Whole-process profiler collection includes them.
 * This is an in-memory native baseline, not native filesystem/ROS execution.
 */
#define main artifact_correctness_main
#define _start artifact_correctness_start
#include "upstream_artifact_xml.cc"
#undef main
#undef _start
#include <algorithm>
#include <ctime>

#ifndef ARTIFACT_ITERATIONS
#define ARTIFACT_ITERATIONS 2000
#endif
#ifndef ARTIFACT_BATCHES
#define ARTIFACT_BATCHES 31
#endif
#ifndef ARTIFACT_CHUNK
#define ARTIFACT_CHUNK 64
#endif

struct prepared_input
{
    artifact_fixture const* fixture{};
#if defined(__wasm__)
    std::uint64_t capability{},object{};
#else
    std::vector<char> snapshot;
#endif
};

static std::uint64_t now()
{
    timespec value{};require(clock_gettime(CLOCK_MONOTONIC,&value)==0);
    return static_cast<std::uint64_t>(value.tv_sec)*1000000000ULL+value.tv_nsec;
}

static std::vector<prepared_input> prepare()
{
    std::vector<prepared_input> result;
    for(auto const& fixture:artifact_fixtures)
    {
        prepared_input value;value.fixture=&fixture;
#if defined(__wasm__)
        std::uint64_t required{};
        require(scene_capability_open(0,address(fixture.alias),std::strlen(fixture.alias),address(&value.capability))==0);
        std::uint64_t opening[5]{fixture.generation};std::memcpy(opening+1,fixture.digest,32);
        require(scene_artifact_open(value.capability,0,address(opening),40,address(&value.object),8,address(&required))==0&&required==8);
#else
        value.snapshot=load(fixture,64);
#endif
        result.push_back(std::move(value));
    }
    return result;
}

__attribute__((noinline)) static std::vector<char> read_snapshot(prepared_input const& input)
{
    std::vector<char> bytes(input.fixture->size);
    for(std::uint64_t offset{};offset<input.fixture->size;)
    {
        char block[ARTIFACT_CHUNK]{};auto expected=std::min<std::uint64_t>(sizeof(block),input.fixture->size-offset);
#if defined(__wasm__)
        std::uint64_t words[3]{input.object,offset,expected},required{};
        require(scene_artifact_read(input.capability,0,address(words),24,address(block),sizeof(block),address(&required))==0&&required==expected);
#else
        std::memcpy(block,input.snapshot.data()+offset,expected);
#endif
        std::memcpy(bytes.data()+offset,block,expected);offset+=expected;
    }
    return bytes;
}

__attribute__((noinline)) static std::uint64_t cycle(std::vector<prepared_input> const& inputs)
{
    std::uint64_t checksum{};
    for(auto const& input:inputs)
    {
        auto bytes=read_snapshot(input);tinyxml2::XMLDocument document;
        auto error=document.Parse(bytes.empty()?"":bytes.data(),bytes.size());
        require((error==tinyxml2::XML_SUCCESS)==input.fixture->valid);
        summary parsed;if(!error)require(document.Accept(&parsed));
        checksum^=parsed.hash+static_cast<unsigned>(error)+parsed.elements+parsed.attributes+parsed.texts+parsed.includes;
        checksum=(checksum<<7)|(checksum>>(64-7));
    }
    return checksum;
}

static int benchmark()
{
    auto inputs=prepare();auto expected=cycle(inputs);
    for(unsigned warm{};warm!=100;++warm)require(cycle(inputs)==expected);
    std::uint64_t samples[ARTIFACT_BATCHES]{},checksum{};
    for(unsigned batch{};batch!=ARTIFACT_BATCHES;++batch)
    {
        auto start=now();
        for(unsigned iteration{};iteration!=ARTIFACT_ITERATIONS;++iteration)
        {auto value=cycle(inputs);require(value==expected);checksum+=value;}
        samples[batch]=now()-start;
    }
#if defined(__wasm__)
    for(auto const& input:inputs)
    {
        std::uint64_t required{};
        require(scene_resource_close(input.capability,0,address(&input.object),8,0,0,address(&required))==0);
    }
#endif
    std::printf("iterations=%u batches=%u fixtures=%u chunk=%u expected=%llu checksum=%llu\n",
        unsigned(ARTIFACT_ITERATIONS),unsigned(ARTIFACT_BATCHES),unsigned(inputs.size()),
        unsigned(ARTIFACT_CHUNK),
        static_cast<unsigned long long>(expected),static_cast<unsigned long long>(checksum));
    for(auto sample:samples)std::printf("elapsed_ns=%llu\n",static_cast<unsigned long long>(sample));
    require(std::fflush(stdout)==0);return 0;
}

#if defined(__wasm__)
extern "C" void _start(){require(benchmark()==0);}
#else
int main(){return benchmark();}
#endif
