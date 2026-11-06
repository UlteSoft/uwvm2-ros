// Actual shared DEV recorder and HOST grammar; no fabricated runtime authority.
#include <uwvm2/uwvm/debugger/host_trace.h>
#include <cassert>
#include <thread>
#include <cstdio>
namespace j=uwvm2::utils::debug::host_calls;
namespace h=uwvm2::uwvm::debugger::host_trace;
int main()
{
    h::request q{};
    for(auto text:{"trace host on","trace host off family=ros1","trace host read after=12 count=8",
        "trace host bytes call=1 segment=0 offset=65536 count=4096","trace host clear if-stop=10"})
    {assert(h::parse(::fast_io::string_view{text,std::strlen(text)},q));}
    for(auto text:{"trace host","trace host on after=1","trace host bytes call=0","trace host bytes call=1 count=4097",
        "trace host bytes call=1 after=0","trace host bytes call=1 family=all","trace host clear family=all",
        "trace host read count=0","trace host on family=wasip1 family=ros2","trace host on if-stop=0",
        "trace host bytes call=1 offset=18446744073709551616","trace host on garbage=1","trace host on\ncontinue"})
    {assert(!h::parse(::fast_io::string_view{text,std::strlen(text)},q));}
    assert(!j::observing(j::channel::wasi));
    {j::scope call{j::channel::wasi,"wasi_snapshot_preview1","fd_write"};assert(!call.value);j::capture(0,0,"hello",5,false);call.finish(true,0);}
    assert(j::records.empty()&&j::sequence==0);
    j::enabled=7;
    std::vector<std::byte> body(100000);
    for(std::size_t i{};i<body.size();++i) {body[i]=static_cast<std::byte>(i);}
    {
        j::scope outer{j::channel::wasi,"wasi_snapshot_preview1","fd_write"};
        outer.identity(std::string(100000,'m'),3);
        auto id=outer.value->id;assert(j::active==outer.value.get());
        j::capture(0,65500,body.data(),body.size(),false);
        {j::scope nested{j::channel::ros2,"uwvm_ros2_v1","log_write"};j::capture(2,64,"ros\0x",5,true);nested.finish(true,13);}
        assert(j::active==outer.value.get());outer.finish(true,0);
        assert(outer.value->segments.size()==1&&outer.value->segments[0].bytes==body);
        assert(h::parse("trace host bytes call=1 segment=0 offset=65536 count=8",q));
        auto page=h::apply(q);assert(page.find("hex=0001020304050607")!=page.npos&&page.find("size=100000")!=page.npos);
        assert(h::parse("trace host bytes call=1 segment=0 offset=100001",q));
        assert(h::apply(q).find("invalid-offset")!=std::string::npos);
        assert(h::parse("trace host read",q));auto labels=h::apply(q);
        assert(labels.size()<65536&&labels.find("module-name-truncated=1")!=labels.npos);
        assert(h::parse("trace host bytes call=1 segment=18446744073709551615 offset=99998 count=4096",q));
        assert(h::apply(q).find("next=100000 value=\"mm\"")!=std::string::npos);
        j::clear();assert(j::records.empty());outer.finish(true,99);assert(j::records.empty());
        assert(outer.value->id==id);
    }
    assert(!j::active);
    std::array<std::thread,4> workers;
    for(auto& thread:workers) {thread=std::thread([] {
        for(unsigned i{};i<3000;++i) {j::scope call{j::channel::ros1,"uwvm_ros1_v1","topic_publish"};j::capture(0,i,&i,sizeof i,false);call.finish(true,i);}
    });}
    for(auto& thread:workers) {thread.join();}
    assert(j::records.size()==12000);
    for(auto const& call:j::records) {assert(call->returned&&call->segments.size()==1&&!call->incomplete&&call->timed);}
    assert(h::parse("trace host off",q));h::apply(q);auto n=j::sequence;
    {j::scope call{j::channel::ros1,"uwvm_ros1_v1","log_write"};assert(!call.value);j::capture(0,0,nullptr,SIZE_MAX,false);call.finish(true,0);}
    assert(j::sequence==n);
    j::clear();assert(j::records.empty()&&j::allocation_failures==0&&j::invalid_ranges==0);
    std::puts("PASS journal: 12000 concurrent calls, 100000-byte payload, paging, nested/retired scopes, off path, strict grammar");
}
