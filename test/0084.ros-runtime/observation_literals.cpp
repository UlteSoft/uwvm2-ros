/** @file
 * @brief Differential canonical encoding, bounds and pagination for literal metadata.
 * @details Compare byte-exact successful output against the ordinary checked
 * writer. Every short output retains external canaries and refuses publication;
 * a failed scalar after a complete prefix remains inside the erasure extent.
 * Dynamic text and authority are not replaced by the literal encoding path.
 */
#include <uwvm2/imported/ros/scene/permission_report.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <type_traits>

namespace s=uwvm2::imported::ros::scene;
namespace p=s::permission_report;
static constexpr p::literal_row label{"compiled","interpreter"};
static constexpr p::literal_row annotated{"execution","mode","full-compile"};
static constexpr p::literal_row empty{"","",""};
static constexpr p::literal_row embedded{"a\0b","key","a\0b"};
static_assert(!std::is_constructible_v<decltype(label),std::string_view,std::string_view>);

#if defined(UWVM_TEST_INVALID_OBSERVATION_ASCII)
static constexpr p::literal_row rejected{"scope","key","\xc0\x80"};
#elif defined(UWVM_TEST_INVALID_OBSERVATION_TERMINATOR)
static constexpr char unterminated[4]{'n','a','m','e'};
static constexpr p::literal_row rejected{unterminated,"key"};
#elif defined(UWVM_TEST_INVALID_OBSERVATION_LENGTH)
static constexpr char too_long[257]{};
static constexpr p::literal_row rejected{too_long,"key"};
#endif

template<typename Literal>
static void equivalent(Literal const& literal,std::string_view scope,std::string_view key,std::string_view annotation)
{
    for(auto cursor:{0u,1u,31u,32u,33u,65536u,65537u})
    {
        std::array<std::uint8_t,p::maximum_response> actual{},reference{};
        p::page page{actual,cursor},original{reference,cursor};
        for(std::uint64_t i{};i<70;++i)
        {
            auto value=i&1?UINT64_MAX-i:i;
            assert(page.add(literal,value)==original.add(scope,key,value,annotation));
        }
        assert(page.finish(true,false)==original.finish(true,false));
        assert(page.bytes().size()==original.bytes().size());
        assert(std::equal(page.bytes().begin(),page.bytes().end(),original.bytes().begin()));
    }
    auto bytes=40+literal.bytes().size()+8;
    for(std::size_t size{};size<=bytes+1;++size)
    {
        std::array<std::uint8_t,4096> storage;storage.fill(0xa5);
        p::page page{std::span{storage}.subspan(8,size),0};
        bool added=page.add(literal,UINT64_MAX);
        assert(added==(size>=bytes));
        assert(page.finish(true,true)==added);
        for(std::size_t i{};i<8;++i)assert(storage[i]==0xa5);
        for(std::size_t i=8+size;i<storage.size();++i)assert(storage[i]==0xa5);
        if(size>=40)
        {
            assert(page.bytes().size()<=size);
            // Once invalid, neither another row nor finish can publish it.
            if(!added)assert(!page.add(literal,0)&&!page.finish(true,true));
            auto written=page.bytes().size();s::erase_bytes(std::span{storage}.subspan(8,written));
            for(std::size_t i{};i<written;++i)assert(storage[8+i]==0);
            for(std::size_t i=written;i<size;++i)assert(storage[8+i]==0xa5);
        }
    }
}
int main()
{
    equivalent(label,"compiled","interpreter",{});
    equivalent(annotated,"execution","mode","full-compile");
    equivalent(empty,{},{},{});
    equivalent(embedded,{"a\0b",3},"key",{"a\0b",3});
    std::array<std::uint8_t,p::maximum_response> bytes{};
    p::page exhausted{bytes,p::maximum_scan};
    for(std::size_t i{};i<p::maximum_scan;++i)assert(exhausted.add(label,i));
    assert(!exhausted.add(label,0)&&!exhausted.finish(true,true));
    p::page dynamic{bytes,0};
    assert(!dynamic.add("scope","key",0,{"\xff",1})&&!dynamic.finish(true,true));
    std::puts("PASS literal reports: byte-exact checked-writer equivalence, canonical embedded NUL, all short buffers, canaries, cursor/scan limits, failed-prefix erasure and dynamic UTF-8 refusal");
}
