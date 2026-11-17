/** @file
 * @brief Serialize real robot XML with the unmodified upstream XMLPrinter.
 * @details Input fixtures are embedded data, not host paths or implicit include
 * grants. The reference build exports exact serialized bytes. The execution
 * build passes those bytes through an explicitly bounded output adapter and
 * checks the result byte-for-byte before comparing native/Wasm summaries.
 * This qualifies a serialization/output slice, not BehaviorTree execution,
 * plugin/include resolution, disk durability or complete-package portability.
 */
#include "tinyxml2.h"
#include "artifact_output_fixtures.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

static void require(bool value)
{
    if(!value)
    {
#if defined(__wasm__)
        __builtin_trap();
#else
        std::abort();
#endif
    }
}

static std::uint64_t byte_hash(unsigned char const* bytes,std::size_t size)
{
    std::uint64_t value=1469598103934665603ULL;
    for(std::size_t index{};index<size;++index){value^=bytes[index];value*=1099511628211ULL;}
    return value;
}

/** @brief Use the upstream memory printer with no FILE* or source access. */
static std::vector<unsigned char> serialize(output_fixture const& fixture,bool compact)
{
    tinyxml2::XMLDocument document;
    if(!fixture.empty_document)
        require(document.Parse(reinterpret_cast<char const*>(fixture.bytes),fixture.size)==tinyxml2::XML_SUCCESS);
    tinyxml2::XMLPrinter printer(nullptr,compact);
    document.Print(&printer);require(printer.CStrSize()>0);
    // CStrSize includes the trailing C string NUL; the artifact does not.
    auto begin=reinterpret_cast<unsigned char const*>(printer.CStr());
    return {begin,begin+printer.CStrSize()-1};
}

#if !defined(ARTIFACT_OUTPUT_REFERENCE)
#include "artifact_output_expected.h"
#if defined(__wasm__)
#include "artifact_output_guest.h"
#else
#include "artifact_output_native.h"
#endif
#endif

static int execute()
{
    unsigned checks{};
#if !defined(ARTIFACT_OUTPUT_REFERENCE)
    unsigned index{};
#endif
    for(auto const& fixture:output_fixtures)for(bool compact:{false,true})
    {
        auto bytes=serialize(fixture,compact);auto hash=byte_hash(bytes.data(),bytes.size());
#if defined(ARTIFACT_OUTPUT_REFERENCE)
        std::printf("OUTPUT %s %u %llu %llu ",fixture.alias,unsigned(compact),
            static_cast<unsigned long long>(bytes.size()),static_cast<unsigned long long>(hash));
        for(auto byte:bytes)std::printf("%02x",unsigned(byte));
        std::printf("\n");++checks;
#else
        auto const& expected=output_expectations[index++];
        require(expected.size==bytes.size()&&expected.hash==hash);
        for(unsigned chunk:{1u,7u,64u,4096u})
        {
            auto result=artifact_output_transport::roundtrip(checks+1,bytes.data(),bytes.size(),expected.sha256,chunk);
#if defined(EXPECT_ARTIFACT_OUTPUT_DENIAL)
            require(result.denied);
#else
            require(!result.denied&&result.published&&!result.durable&&result.bytes==bytes);
            std::printf("%s compact=%u chunk=%u bytes=%llu hash=%llu\n",fixture.alias,unsigned(compact),chunk,
                static_cast<unsigned long long>(result.bytes.size()),
                static_cast<unsigned long long>(byte_hash(result.bytes.data(),result.bytes.size())));
#endif
            ++checks;
        }
#endif
    }
    std::printf("checks:%u\n",checks);require(std::fflush(stdout)==0);return 0;
}

#if defined(__wasm__)
extern "C" void _start(){require(execute()==0);}
#else
int main(){return execute();}
#endif
