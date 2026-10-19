#include "../../src/uwvm2/imported/ros/ipc/image.h"
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <new>
#include <stdexcept>

// Only the encoder expression is counted. Native input storage, golden output
// and assertions are prepared outside the interval; this is no throughput test.
namespace allocation_probe
{
    struct totals { std::size_t calls{},bytes{},largest{},blocked_large{}; };
    bool enabled{},block_large{};
    totals current{};
    struct interval
    {
        explicit interval(bool reject_large=false) { current={};block_large=reject_large;enabled=true; }
        ~interval() { enabled=false;block_large=false; }
        totals finish() { enabled=false;block_large=false;return current; }
    };
}
void* operator new(std::size_t size)
{
    if(allocation_probe::enabled)
    {
        ++allocation_probe::current.calls;allocation_probe::current.bytes+=size;
        allocation_probe::current.largest=std::max(allocation_probe::current.largest,size);
        if(allocation_probe::block_large&&size>=1024*1024)
        { ++allocation_probe::current.blocked_large;throw std::bad_alloc{}; }
    }
    auto memory=std::malloc(size?size:1);if(!memory) { throw std::bad_alloc{}; }return memory;
}
void* operator new[](std::size_t size) { return ::operator new(size); }
void operator delete(void* memory) noexcept { std::free(memory); }
void operator delete[](void* memory) noexcept { std::free(memory); }
void operator delete(void* memory,std::size_t) noexcept { std::free(memory); }
void operator delete[](void* memory,std::size_t) noexcept { std::free(memory); }

namespace wire=uwvm2::imported::ros::ipc;
namespace legacy
{
    using wire::image;
    using wire::put;
    // Exact pre-change encoder body from image.h SHA256
    // 5a9b156dbe2414a6a7d6b6ff7f313d95e811430231029a489848b9c04f34c5d6.
    inline std::vector<std::uint8_t> encode_image(image const& in)
    {
        std::vector<std::uint8_t> b;
        put(b,in.sequence,4); put(b,in.seconds,4); put(b,in.nanoseconds,4); put(b,in.frame_id.size(),4); b.insert(b.end(),in.frame_id.begin(),in.frame_id.end());
        put(b,in.height,4); put(b,in.width,4); put(b,in.encoding.size(),4); b.insert(b.end(),in.encoding.begin(),in.encoding.end());
        put(b,in.bigendian?1:0,1); put(b,in.step,4); put(b,in.data.size(),4); b.insert(b.end(),in.data.begin(),in.data.end()); return b;
    }
}
namespace
{
    unsigned checks{},cases{};
    void require(bool condition,char const* message)
    { ++checks;if(!condition) { throw std::runtime_error{message}; } }
    wire::image_view view(wire::image const& native)
    {
        return {native.sequence,native.seconds,native.nanoseconds,native.height,native.width,native.step,
                native.frame_id,native.encoding,native.bigendian,native.data.data(),native.data.size()};
    }
    void one_allocation(allocation_probe::totals measured,std::size_t wire_size)
    {
        require(measured.calls==1,"wire encoder allocates exactly once");
        require(measured.bytes==wire_size&&measured.largest==wire_size,"encoder allocates only complete exact-sized wire");
    }
    void exact_fields(std::vector<std::uint8_t> const& bytes,wire::image const& native)
    {
        require(bytes.size()==37+native.frame_id.size()+native.encoding.size()+native.data.size(),"complete wire length");
        require(wire::get(bytes.data(),4)==native.sequence,"sequence exact little endian");
        require(wire::get(bytes.data()+4,4)==native.seconds,"seconds exact little endian");
        require(wire::get(bytes.data()+8,4)==native.nanoseconds,"nanoseconds exact little endian");
        require(wire::get(bytes.data()+12,4)==native.frame_id.size(),"frame text length exact");
        std::size_t at{16};
        require(std::string_view{reinterpret_cast<char const*>(bytes.data()+at),native.frame_id.size()}==native.frame_id,"frame raw bytes exact");at+=native.frame_id.size();
        require(wire::get(bytes.data()+at,4)==native.height,"height exact little endian");at+=4;
        require(wire::get(bytes.data()+at,4)==native.width,"width exact little endian");at+=4;
        require(wire::get(bytes.data()+at,4)==native.encoding.size(),"encoding text length exact");at+=4;
        require(std::string_view{reinterpret_cast<char const*>(bytes.data()+at),native.encoding.size()}==native.encoding,"encoding raw bytes exact");at+=native.encoding.size();
        require(bytes[at++]==static_cast<std::uint8_t>(native.bigendian),"canonical endian flag exact");
        require(wire::get(bytes.data()+at,4)==native.step,"step exact little endian");at+=4;
        require(wire::get(bytes.data()+at,4)==native.data.size(),"pixel byte length exact");at+=4;
        require(std::equal(native.data.begin(),native.data.end(),bytes.begin()+at),"all pixel bytes exact");
    }
    void compare(wire::image const& native)
    {
        ++cases;auto expected{legacy::encode_image(native)};auto borrowed{view(native)};
        require(borrowed.frame_id.data()==native.frame_id.data()&&borrowed.encoding.data()==native.encoding.data(),"text views alias live native strings");
        require(borrowed.data==native.data.data()&&borrowed.data_size==native.data.size(),"pixel view aliases live native buffer");
        allocation_probe::interval first;auto actual{wire::encode_image_view(borrowed)};auto first_measured{first.finish()};
        require(actual==expected,"borrowed encoder matches original bytes");one_allocation(first_measured,expected.size());exact_fields(actual,native);
        allocation_probe::interval second;auto owning{wire::encode_image(native)};auto second_measured{second.finish()};
        require(owning==expected,"owning API preserves exact original bytes");one_allocation(second_measured,expected.size());
    }
    void corpus()
    {
        wire::image native{0x04030201,0x08070605,0x0c0b0a09,7,9,81,"camera_frame","rgb8",false,{}};
        for(std::size_t size:{0u,1u,2u,3u,255u,256u,4096u,65536u,wire::max_payload_bytes-518u,wire::max_payload_bytes+1u})
        {
            native.data.resize(size);for(std::size_t i{};i<size;++i) { native.data[i]=static_cast<std::uint8_t>(i); }
            for(bool bigendian:{false,true}) { native.bigendian=bigendian;compare(native); }
        }
        native.data={0,1,127,128,254,255};
        for(std::size_t size:{0u,1u,15u,16u,255u,256u,4096u})
        {
            native.frame_id.assign(size,'f');native.encoding.assign(size,'e');compare(native);
        }
        native.frame_id=std::string{"x\0y",3};native.encoding=std::string{"\xff\xc0\xaf",3};compare(native);
        native.frame_id="\xc2\xa2\xe2\x82\xac\xf0\x90\x8d\x88";native.encoding="mono8";compare(native);
        native.sequence=native.seconds=native.nanoseconds=native.height=native.width=native.step=std::numeric_limits<std::uint32_t>::max();compare(native);
        // Encoding still trusts the adapter's validation. Empty/long/invalid
        // fields and values above IPC quota keep their old bytes unchanged.
        allocation_probe::interval empty;auto bytes{wire::encode_image_view({})};auto measured{empty.finish()};
        require(bytes.size()==37&&std::all_of(bytes.begin(),bytes.end(),[](std::uint8_t byte){return byte==0;}),"default empty borrowed fields produce the original 37-byte wire");one_allocation(measured,37);
    }
    void golden()
    {
        wire::image native{1,2,3,4,5,6,"x","y",true,{0xaa,0xbb}};
        std::array<std::uint8_t,41> const expected{{
            1,0,0,0,2,0,0,0,3,0,0,0,1,0,0,0,'x',
            4,0,0,0,5,0,0,0,1,0,0,0,'y',1,6,0,0,0,2,0,0,0,0xaa,0xbb}};
        auto actual{wire::encode_image_view(view(native))};require(actual.size()==expected.size()&&std::equal(expected.begin(),expected.end(),actual.begin()),"independent fixed LE golden wire");
    }
    void size_boundaries()
    {
        std::uint8_t const pixel{};auto maximum{std::vector<std::uint8_t>{}.max_size()};
        for(auto size:{maximum,std::numeric_limits<std::size_t>::max()})
        {
            wire::image_view oversized{};oversized.data=&pixel;oversized.data_size=size;
            bool rejected{};allocation_probe::interval interval{true};
            try { (void)wire::encode_image_view(oversized); }catch(std::length_error const&) { rejected=true; }
            auto measured{interval.finish()};require(rejected,"unrepresentable pixel length rejected before access");
            require(measured.blocked_large==0&&measured.largest<1024*1024,"representability preflight makes no output-sized allocation");
        }
        wire::image_view combined{};combined.frame_id="a";combined.encoding="b";combined.data=&pixel;combined.data_size=maximum-37;
        bool rejected{};allocation_probe::interval interval{true};
        try { (void)wire::encode_image_view(combined); }catch(std::length_error const&) { rejected=true; }
        auto measured{interval.finish()};require(rejected,"combined representability checked without wrapping");
        require(measured.blocked_large==0&&measured.largest<1024*1024,"combined preflight does not reserve or inspect pixels");
    }
    void native_copy_comparison()
    {
        wire::image native{7,123,456,1024,1024,1024,std::string(255,'f'),std::string(255,'e'),false,std::vector<std::uint8_t>(1024*1024)};
        for(std::size_t i{};i<native.data.size();++i) { native.data[i]=static_cast<std::uint8_t>(i); }
        auto original_data{native.data};auto original_frame{native.frame_id};auto original_encoding{native.encoding};
        allocation_probe::interval before;
        auto old{legacy::encode_image({native.sequence,native.seconds,native.nanoseconds,native.height,native.width,native.step,native.frame_id,native.encoding,native.bigendian,native.data})};
        auto old_measured{before.finish()};
        allocation_probe::interval after;
        auto current{wire::encode_image_view({native.sequence,native.seconds,native.nanoseconds,native.height,native.width,native.step,native.frame_id,native.encoding,native.bigendian,native.data.data(),native.data.size()})};
        auto current_measured{after.finish()};
        require(old==current,"native adapter before/after exact wire");one_allocation(current_measured,current.size());
        require(old_measured.calls>current_measured.calls&&old_measured.bytes>current_measured.bytes,"borrowed native expression reduces allocations and requested bytes");
        require(native.data==original_data&&native.frame_id==original_frame&&native.encoding==original_encoding,"native input unchanged after encoding");
        std::printf("IMAGE_NATIVE_ALLOCATION before_calls=%zu before_bytes=%zu after_calls=%zu after_bytes=%zu wire_bytes=%zu\n",
                    old_measured.calls,old_measured.bytes,current_measured.calls,current_measured.bytes,current.size());
    }
}
int main()
{
    try
    {
        golden();corpus();size_boundaries();native_copy_comparison();
        std::printf("IMAGE_ENCODE_PASS checks=%u differential_cases=%u\n",checks,cases);return 0;
    }
    catch(std::exception const& error) { std::fprintf(stderr,"IMAGE_ENCODE_FAIL %s\n",error.what());return 1; }
}
