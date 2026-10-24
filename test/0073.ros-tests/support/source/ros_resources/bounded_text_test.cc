#include "../../../../../src/ros/gateway/common/bounded_text.h"
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <new>
#include <stdexcept>

// Count only the operation under test. Borrowed/native input storage and
// assertions are prepared outside the counting interval.
namespace allocation_probe
{
    struct totals { std::size_t calls{},bytes{},largest{}; };
    inline bool enabled{};
    inline totals current{};
    struct interval
    {
        interval() { current={};enabled=true; }
        ~interval() { enabled=false; }
        totals finish() { enabled=false;return current; }
    };
}
void* operator new(std::size_t size)
{
    auto memory=std::malloc(size?size:1);
    if(!memory) { throw std::bad_alloc{}; }
    if(allocation_probe::enabled)
    {
        ++allocation_probe::current.calls;allocation_probe::current.bytes+=size;
        allocation_probe::current.largest=std::max(allocation_probe::current.largest,size);
    }
    return memory;
}
void* operator new[](std::size_t size) { return ::operator new(size); }
void operator delete(void* memory) noexcept { std::free(memory); }
void operator delete[](void* memory) noexcept { std::free(memory); }
void operator delete(void* memory,std::size_t) noexcept { std::free(memory); }
void operator delete[](void* memory,std::size_t) noexcept { std::free(memory); }

namespace
{
    namespace bounded=uwvm2::ros_gateway::bounded_text;
    namespace wire=uwvm2::imported::ros::ipc;
    unsigned checks{};
    void require(bool condition,char const* message)
    { ++checks;if(!condition) { throw std::runtime_error{message}; } }
    void refused(wire::call_result const& result,std::uint32_t error)
    { require(result.error==error,"exact refusal code");require(result.payload.empty(),"refusal discloses no partial payload"); }
    void exact(wire::call_result const& result,std::string_view text)
    {
        require(!result.error,"successful text result");
        require(result.payload.size()==4+text.size(),"exact wire size including length");
        require(wire::get(result.payload.data(),4)==text.size(),"canonical little-endian length");
        require(std::string_view{reinterpret_cast<char const*>(result.payload.data()+4),text.size()}==text,"exact canonical text bytes");
    }
    void one_allocation(allocation_probe::totals measured,std::size_t size)
    { require(measured.calls==1,"wire output allocates once");require(measured.bytes==size&&measured.largest==size,"only exact-sized wire output allocated"); }
    void no_allocation(allocation_probe::totals measured)
    { require(measured.calls==0&&measured.bytes==0,"preflight refusal allocates no output or intermediate text"); }
    void fragments()
    {
        for(auto maximum:{0u,1u,3u})
        {
            allocation_probe::interval interval;auto result=bounded::result(maximum,{});auto measured=interval.finish();
            refused(result,90);no_allocation(measured);
        }
        {
            allocation_probe::interval interval;auto result=bounded::result(4,{std::string_view{},""});auto measured=interval.finish();
            exact(result,"");one_allocation(measured,4);
        }
        {
            allocation_probe::interval interval;auto result=bounded::result(7,{"a","","bc"});auto measured=interval.finish();
            exact(result,"abc");one_allocation(measured,7);
            require(result.payload[0]==3&&result.payload[1]==0&&result.payload[2]==0&&result.payload[3]==0,"explicit LE envelope bytes");
        }
        {
            allocation_probe::interval interval;auto result=bounded::result(6,{"a","bc"});auto measured=interval.finish();
            refused(result,90);no_allocation(measured);
        }
        constexpr std::string_view descriptor{"name=value\ntype=4\nread_only=false\ndescription=hello\nconstraints=unit"};
        {
            allocation_probe::interval interval;
            auto result=bounded::result(4+descriptor.size(),{"name=","value","\ntype=","4","\nread_only=","false","\ndescription=","hello","\nconstraints=","unit"});
            auto measured=interval.finish();exact(result,descriptor);one_allocation(measured,4+descriptor.size());
        }
        std::string oversized(1024*1024,'x');
        {
            allocation_probe::interval interval;auto result=bounded::result(32,{"name=",oversized});auto measured=interval.finish();
            refused(result,90);no_allocation(measured);
        }
        for(std::string_view invalid:{std::string_view{"\xc0\xaf",2},std::string_view{"\xe2\x82",2},std::string_view{"\xed\xa0\x80",3},std::string_view{"\xf4\x90\x80\x80",4},std::string_view{"\x80",1}})
        {
            allocation_probe::interval interval;auto result=bounded::result(64,{"description=",invalid});auto measured=interval.finish();
            refused(result,22);no_allocation(measured);
        }
        {
            // Size refusal precedes UTF8 inspection, matching the old result.
            allocation_probe::interval interval;auto result=bounded::result(4,{std::string_view{"\xff",1}});auto measured=interval.finish();
            refused(result,90);no_allocation(measured);
        }
        constexpr std::string_view unicode{"\xc2\xa2\xe2\x82\xac\xf0\x90\x8d\x88",9};
        exact(bounded::result(4+unicode.size(),{unicode}),unicode);
        constexpr std::string_view with_nul{"a\0b",3};
        exact(bounded::result(7,{with_nul}),with_nul); // Metadata UTF8 preserves NUL.
        std::string ceiling(wire::max_payload_bytes-4,'x');
        {
            allocation_probe::interval interval;auto result=bounded::result(std::numeric_limits<std::uint64_t>::max(),{ceiling});auto measured=interval.finish();
            exact(result,ceiling);one_allocation(measured,wire::max_payload_bytes);
        }
        ceiling.push_back('x');
        {
            allocation_probe::interval interval;auto result=bounded::result(std::numeric_limits<std::uint64_t>::max(),{ceiling});auto measured=interval.finish();
            refused(result,90);no_allocation(measured);
        }
    }
    void rows()
    {
        bounded::sorted_rows empty{4};require(empty.empty()&&!empty.error()&&empty.text_bytes()==0,"empty rows state");
        {
            allocation_probe::interval interval;auto result=empty.result();auto measured=interval.finish();
            exact(result,"");one_allocation(measured,4);
        }
        for(auto maximum:{0u,1u,3u})
        {
            bounded::sorted_rows too_small{maximum};require(!too_small.add("/a"),"invalid envelope refuses row");
            require(too_small.error()==90&&too_small.empty()&&too_small.text_bytes()==0,"invalid envelope retains no rows");
            allocation_probe::interval interval;auto result=too_small.result();auto measured=interval.finish();
            refused(result,90);no_allocation(measured);
        }
        // The original std::set sorts complete rows: tab precedes newline.
        constexpr std::string_view expected{"/a\tA\n/a\tZ\n/a\n/z\tT\n"};
        bounded::sorted_rows sorted{4+expected.size()};
        require(sorted.add("/z","T")&&sorted.add("/a","Z")&&sorted.add("/a")&&sorted.add("/a","A"),"unsorted native names/types admitted at exact budget");
        require(sorted.text_bytes()==expected.size()&&!sorted.empty(),"retained unique row bytes equal full quota");
        for(unsigned i{};i!=1000;++i) { require(sorted.add("/a","Z"),"duplicate admitted when full without consuming quota"); }
        require(sorted.text_bytes()==expected.size(),"duplicates retain unchanged byte count");
        {
            allocation_probe::interval interval;auto result=sorted.result();auto measured=interval.finish();
            exact(result,expected);one_allocation(measured,4+expected.size());
        }
        require(!sorted.add("/b"),"new unique row exceeds cumulative quota");
        require(sorted.error()==90&&sorted.text_bytes()==expected.size(),"cumulative overflow leaves admitted byte count unchanged");
        require(!sorted.add("/a","Z"),"quota refusal remains sticky");
        {
            allocation_probe::interval interval;auto result=sorted.result();auto measured=interval.finish();
            refused(result,90);no_allocation(measured);
        }
        std::string native_oversized(1024*1024,'x');bounded::sorted_rows bounded{32};
        {
            allocation_probe::interval interval;auto admitted=bounded.add(native_oversized,"native/Type");auto measured=interval.finish();
            require(!admitted&&bounded.error()==90&&bounded.empty()&&bounded.text_bytes()==0,"single oversized borrowed row is rejected before copying");no_allocation(measured);
        }
        bounded::sorted_rows type_overflow{8};
        {
            allocation_probe::interval interval;auto admitted=type_overflow.add("a",native_oversized);auto measured=interval.finish();
            require(!admitted&&type_overflow.error()==90&&type_overflow.empty(),"oversized type rejected before concatenation");no_allocation(measured);
        }
        bounded::sorted_rows one{6};require(one.add("a")&&one.add("a"),"one two-byte row plus duplicate fits exact quota");exact(one.result(),"a\n");
        bounded::sorted_rows short_row{5};require(!short_row.add("a")&&short_row.error()==90,"one byte below row quota refused");refused(short_row.result(),90);
        bounded::sorted_rows empty_name{5};require(empty_name.add(""),"helper preserves caller-selected empty row");exact(empty_name.result(),"\n");
        bounded::sorted_rows invalid{64};require(invalid.add(std::string_view{"\xff",1}),"invalid native UTF8 row retained only within quota");
        {
            allocation_probe::interval interval;auto result=invalid.result();auto measured=interval.finish();refused(result,22);no_allocation(measured);
        }
        bounded::sorted_rows unicode{64};require(unicode.add("/\xe2\x82\xac","T"),"valid Unicode row admitted");exact(unicode.result(),"/\xe2\x82\xac\tT\n");
        std::string ceiling(wire::max_payload_bytes-5,'x');bounded::sorted_rows capped{std::numeric_limits<std::uint64_t>::max()};
        require(capped.add(ceiling)&&capped.add(ceiling),"row ceiling clamped with duplicate still admitted");
        {
            allocation_probe::interval interval;auto result=capped.result();auto measured=interval.finish();
            require(!result.error&&result.payload.size()==wire::max_payload_bytes&&result.payload.back()=='\n',"clamped row output exact IPC ceiling");
            require(wire::get(result.payload.data(),4)==wire::max_payload_bytes-4,"clamped row length exact");one_allocation(measured,wire::max_payload_bytes);
        }
        ceiling.push_back('x');bounded::sorted_rows over_ceiling{std::numeric_limits<std::uint64_t>::max()};
        {
            allocation_probe::interval interval;auto admitted=over_ceiling.add(ceiling);auto measured=interval.finish();
            require(!admitted&&over_ceiling.error()==90&&over_ceiling.empty(),"clamped oversized row rejected before allocation");no_allocation(measured);
        }
    }
}
int main()
{
    try
    {
        fragments();rows();
        // The independent native compatibility test is deliberately C++17;
        // vendored fast_io requires concepts unavailable in GCC9 ROS builds.
        std::printf("PASS bounded text %u checks: exact bytes, quota, sort, duplicate, UTF8, allocation preflight\n",checks);
        return 0;
    }
    catch(std::exception const& error) { std::fprintf(stderr,"FAIL bounded text: %s\n",error.what());return 1; }
}
