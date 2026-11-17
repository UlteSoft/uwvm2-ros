/** @file @brief Immutable diagnostic pages, hostile sources, cursor expiry and real allocation faults. */
#include <uwvm2/imported/ros/scene/diagnostic_snapshot.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <barrier>
#include <sys/wait.h>
namespace d=uwvm2::imported::ros::scene::diagnostics;
using d::error;
static constexpr d::owner owner{1,2,3};
static thread_local std::size_t fail_after{},allocations{};
static bool fail() noexcept{++allocations;return fail_after&&!--fail_after;}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return fail()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t s){return fail()?nullptr:__real_calloc(n,s);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return fail()?nullptr:__real_realloc(p,n);}
struct provider
{
    unsigned calls{},mode{};std::barrier<>* pause{};std::uint64_t value{10};
    static error capture(void* context,std::span<d::counter> out,std::size_t& count) noexcept
    {
        auto& self=*static_cast<provider*>(context);++self.calls;
        if(self.pause){self.pause->arrive_and_wait();self.pause->arrive_and_wait();}
        if(self.mode==1)return error::io;
        out[0]={3,self.value+2,true,false};out[1]={1,self.value,true,false};out[2]={2,0,false,false};count=3;
        if(self.mode==2)count=out.size()+1;
        if(self.mode==3)out[0].code=65;
        if(self.mode==4)out[0].code=1;
        if(self.mode==5)out[2].value=5;
        if(self.mode==6)out[2].saturated=true;
        if(self.mode==7)out[0].code=4;
        return error::success;
    }
    d::source source(){return {this,capture};}
};
static d::plan plan()
{d::plan out{123,7,3,2,0,1000000000,500000000};assert(d::snapshot::required_storage(out,out.storage_bytes));return out;}
static d::usage inspect(d::snapshot& view)
{d::usage out;assert(view.inspect(owner,out)==error::success);return out;}
static void configuration_and_exact_scope()
{
    provider source;d::snapshot view;auto p=plan();
    assert(view.configure(owner,{},source.source())==error::invalid);
    assert(view.configure({},p,source.source())==error::invalid&&view.configure(owner,p,{})==error::invalid);
    auto too_small=p;--too_small.storage_bytes;assert(view.configure(owner,too_small,source.source())==error::too_large);
    assert(view.configure(owner,p,source.source())==error::success&&source.calls==0);
    assert(view.configure(owner,p,source.source())==error::denied);
    auto sink=[](auto const&){assert(false);return true;};
    assert(view.read({2,2,3},123,{},2,sink)==error::denied&&view.read(owner,124,{},2,sink)==error::denied);
    assert(view.read(owner,123,0,2,sink)==error::invalid&&view.read(owner,123,{},3,sink)==error::invalid);
    assert(view.read(owner,123,99,2,sink)==error::bad_handle&&source.calls==0);
    assert(!inspect(view).revision);
}
static void immutable_pages_and_failed_copy()
{
    provider source;d::snapshot view;assert(view.configure(owner,plan(),source.source())==error::success);
    d::handle revision{},cursor{};std::array<d::counter,2> initial{};
    auto copy=[&](d::page const& page)
    {revision=page.revision;assert(page.cursor&&page.items.size()==2&&page.items[0].code==1&&page.items[1].code==2);cursor=*page.cursor;std::copy(page.items.begin(),page.items.end(),initial.begin());return false;};
    assert(view.read(owner,123,{},2,copy)==error::io&&source.calls==1&&inspect(view).pending);
    assert(view.read(owner,123,{},1,copy)==error::conflict&&view.read(owner,123,cursor,2,copy)==error::conflict);
    source.value=999;
    assert(view.read(owner,123,{},2,[&](auto const& page){assert(page.revision==revision&&page.cursor==cursor&&std::equal(page.items.begin(),page.items.end(),initial.begin(),initial.end()));return true;})==error::success);
    assert(view.read(owner,123,{},2,copy)==error::bad_handle&&source.calls==1);
    bool failed{};
    try{view.read(owner,123,cursor,2,[&](auto const& page)->bool{assert(page.revision==revision&&!page.cursor&&page.items.size()==1&&page.items[0].value==12);throw 42;});}
    catch(int value){assert(value==42);failed=true;}
    assert(failed&&inspect(view).pending&&!inspect(view).busy);
    assert(view.read(owner,123,cursor,1,copy)==error::conflict);
    assert(view.read(owner,123,cursor,2,[&](auto const& page){assert(page.revision==revision&&page.items[0].value==12);return true;})==error::success);
    assert(!inspect(view).pending&&!inspect(view).rows&&source.calls==1);
    assert(view.read(owner,123,cursor,2,copy)==error::bad_handle);
    assert(view.read(owner,123,{},2,[&](auto const& page){assert(page.revision==revision+1&&page.items[0].value==999);return true;})==error::success&&source.calls==2);
}
static void malicious_source_and_cross_snapshot_cursor()
{
    for(unsigned mode=1;mode<=7;++mode)
    {
        provider source;source.mode=mode;d::snapshot view;assert(view.configure(owner,plan(),source.source())==error::success);
        auto expected=mode==1?error::io:mode==2?error::too_large:error::invalid;
        assert(view.read(owner,123,{},2,[](auto const&){assert(false);return true;})==expected);
        assert(!inspect(view).revision&&!inspect(view).rows);
        source.mode=0;assert(view.read(owner,123,{},2,[](auto const&){return true;})==error::success);
    }
    provider a,b;d::snapshot left,right;assert(left.configure(owner,plan(),a.source())==error::success&&right.configure(owner,plan(),b.source())==error::success);
    d::handle first{},second{};
    assert(left.read(owner,123,{},2,[&](auto const& page){assert(page.cursor);first=*page.cursor;return true;})==error::success);
    assert(right.read(owner,123,{},2,[&](auto const& page){assert(page.cursor);second=*page.cursor;return true;})==error::success&&first!=second);
    assert(left.read(owner,123,second,2,[](auto const&){assert(false);return true;})==error::bad_handle);
}
static void deadline_and_cached_inspection()
{
    provider source;d::snapshot view;auto p=plan();p.lifetime_ns=2000000;p.capture_ns=1000000;
    assert(view.configure(owner,p,source.source())==error::success);d::handle cursor{};
    assert(view.read(owner,123,{},2,[&](auto const& page){assert(page.cursor);cursor=*page.cursor;return false;})==error::io);
    auto before=inspect(view);std::this_thread::sleep_for(std::chrono::milliseconds(5));auto after=inspect(view);
    assert(after.pending&&after.expires==before.expires&&after.rows==3&&source.calls==1);
    assert(view.read(owner,123,cursor,2,[](auto const&){assert(false);return true;})==error::timeout);
    assert(!inspect(view).pending&&source.calls==1);
    assert(view.read(owner,123,{},2,[](auto const&){std::this_thread::sleep_for(std::chrono::milliseconds(5));return true;})==error::timeout);
    assert(!inspect(view).rows&&source.calls==2);
}
static void reentry_retire_capture_and_delivery()
{
    provider source;d::snapshot view;assert(view.configure(owner,plan(),source.source())==error::success);
    assert(view.read(owner,123,{},2,[&](auto const&)
    {
        assert(inspect(view).busy&&view.read(owner,123,{},2,[](auto const&){assert(false);return true;})==error::again);
        assert(view.retire(owner)==error::success);return true;
    })==error::cancelled&&inspect(view).retired);
    provider second;d::snapshot capture;assert(capture.configure(owner,plan(),second.source())==error::success);
    std::barrier barrier{2};second.pause=&barrier;error result{};
    std::thread worker{[&]{result=capture.read(owner,123,{},2,[](auto const&){assert(false);return true;});}};
    barrier.arrive_and_wait();assert(inspect(capture).busy&&capture.read(owner,123,{},2,[](auto const&){assert(false);return true;})==error::again);
    assert(capture.retire(owner)==error::success);barrier.arrive_and_wait();worker.join();assert(result==error::cancelled&&!inspect(capture).revision);
}
static void allocation_and_fork()
{
    provider source;d::snapshot view;auto p=plan();bool caught{};allocations=0;fail_after=1;
    try{static_cast<void>(view.configure(owner,p,source.source()));}catch(std::bad_alloc const&){caught=true;}
    fail_after=0;assert(caught&&allocations==1&&view.configure(owner,p,source.source())==error::success);
    allocations=0;fail_after=1;d::handle cursor{};
    assert(view.read(owner,123,{},2,[&](auto const& page){assert(page.cursor);cursor=*page.cursor;return true;})==error::success);
    assert(view.read(owner,123,cursor,2,[](auto const&){return true;})==error::success);static_cast<void>(inspect(view));
    fail_after=0;assert(!allocations);
    auto child=::fork();assert(child>=0);if(!child)
    {
        d::usage u;bool good=view.inspect(owner,u)==error::denied&&view.retire(owner)==error::denied&&
            view.read(owner,123,{},2,[](auto const&){return true;})==error::denied;::_exit(good?0:1);
    }
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status)&&!inspect(view).retired);
}
int main()
{
    configuration_and_exact_scope();immutable_pages_and_failed_copy();malicious_source_and_cross_snapshot_cursor();
    deadline_and_cached_inspection();reentry_retire_capture_and_delivery();allocation_and_fork();
    std::cout<<"passed 6 diagnostic snapshot security groups\n";
}
