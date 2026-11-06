// Optional recording allocation must not throw into an import or rewrite errno.
#include <uwvm2/utils/debug/host_calls.h>
#include <cassert>
#include <cstdio>
namespace j=uwvm2::utils::debug::host_calls;
static std::atomic<bool> fail_next{};
void* operator new(std::size_t n)
{
    if(fail_next.exchange(false)) {throw std::bad_alloc{};}
    if(auto p=std::malloc(n?n:1)) {return p;}
    throw std::bad_alloc{};
}
void operator delete(void* p) noexcept {std::free(p);}
void operator delete(void* p,std::size_t) noexcept {std::free(p);}
int main()
{
    j::enabled=7;
    fail_next=true;
    unsigned result{};
    {j::scope call{j::channel::wasi,"wasi_snapshot_preview1","clock_res_get"};assert(!call.value);
        result=37;call.finish(true,result);}
    assert(result==37&&j::allocation_failures==1&&j::records.empty()&&!j::active);
    {
        j::scope call{j::channel::ros2,"uwvm_ros2_v1","log_write"};assert(call.value);
        fail_next=true;j::capture(0,123,"binary\0data",11,false);
        assert(call.value->incomplete&&call.value->segments.empty());
        result=13;call.finish(true,result);
        assert(call.value->returned&&call.value->result==13&&j::allocation_failures==2);
        // A later successful observation is usable after the failure.
        j::capture(1,0,"ok",2,true);
        assert(call.value->segments.size()==1);
    }
    j::clear();assert(!j::active);
    std::puts("PASS recording allocation failures do not escape or change results");
}
