/** @file @brief Real allocator faults, exact log replay, retained front and adversarial sink races. */
#include <uwvm2/imported/ros/scene/log_store.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <barrier>
#include <sys/wait.h>
namespace l=uwvm2::imported::ros::scene::logs;
namespace w=uwvm2::imported::ros::scene::wire;
using l::error;
static constexpr l::owner owner{1,2,3};
static thread_local std::size_t fail_after{},allocations{};
static bool fail() noexcept{++allocations;return fail_after&&!--fail_after;}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return fail()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t s){return fail()?nullptr:__real_calloc(n,s);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return fail()?nullptr:__real_realloc(p,n);}
struct record
{
    std::array<std::uint8_t,2048> storage{};std::size_t size{};
    w::bytes view() const noexcept{return {storage.data(),size};}
};
static record encoded(l::request value)
{record out;w::writer writer{out.storage};assert(l::encode(writer,value));out.size=writer.size();return out;}
static record message(std::uint64_t sequence,std::string_view text="literal %n %s \x1b[31m 日志")
{return encoded({17,sequence,l::severity::warn,"robot",text,{},{},{},0,false,false});}
static l::plan plan(l::overflow full=l::overflow::reject)
{
    l::plan p;p.records=2;p.record_bytes=1024;p.severity_mask=31;p.logger_count=2;p.full=full;
    assert(p.loggers[0].assign("robot")&&p.loggers[1].assign("robot.motor"));assert(l::store::required_storage(p,p.storage_bytes));return p;
}
static l::usage inspect(l::store& store)
{l::usage out;assert(store.inspect(owner,out)==error::success);return out;}
static l::receipt emit(l::store& store,record const& bytes)
{l::receipt out;assert(store.emit(owner,bytes.view(),out)==error::success);return out;}
static void configuration_and_owner()
{
    l::store store;l::receipt unchanged{9,9,9,9,9},out=unchanged;auto one=message(1);
    assert(store.emit(owner,one.view(),out)==error::denied&&out==unchanged);
    assert(store.configure(owner,{})==error::invalid);
    auto p=plan();--p.storage_bytes;assert(store.configure(owner,p)==error::too_large);++p.storage_bytes;
    auto duplicate=p;duplicate.loggers[1]=duplicate.loggers[0];assert(store.configure(owner,duplicate)==error::invalid);
    auto excessive=p;excessive.records=UINT64_MAX;std::uint64_t bytes{123};assert(!l::store::required_storage(excessive,bytes)&&bytes==123);
    auto invalid=p;invalid.full=static_cast<l::overflow>(7);assert(store.configure(owner,invalid)==error::invalid);
    assert(store.configure({},p)==error::invalid&&store.configure(owner,p)==error::success);
    assert(store.configure(owner,p)==error::denied);
    for(auto other:{l::owner{4,2,3},l::owner{1,4,3},l::owner{1,2,4}})
    {
        assert(store.emit(other,one.view(),out)==error::denied&&out==unchanged);
        assert(store.drain(other,[](auto,auto){assert(false);return true;})==error::denied);
        assert(store.retire(other)==error::denied);
    }
    auto value=inspect(store);assert(value.reserved_bytes==p.storage_bytes&&!value.accepted&&!value.retained);
}
static void canonical_records_and_rejection()
{
    l::store store;auto p=plan();p.severity_mask=1u<<static_cast<unsigned>(l::severity::warn);assert(store.configure(owner,p)==error::success);
    auto one=message(1);l::request parsed;assert(l::decode(one.view(),parsed)&&parsed.message=="literal %n %s \x1b[31m 日志");
    parsed.has_time=true;parsed.time={5,0,-123};parsed.has_location=true;parsed.file="../guest/source.cc";parsed.function="do_work";parsed.line=99;
    auto full=encoded(parsed);l::request copy;assert(l::decode(full.view(),copy)&&copy.time.epoch==0&&copy.time.nanoseconds==-123&&copy.line==99);
    l::receipt out{4,4,4,4,4},before=out;
    for(std::size_t n{};n<full.size;++n)assert(store.emit(owner,full.view().first(n),out)==error::invalid&&out==before);
    auto trailing=full;trailing.storage[trailing.size++]=0;assert(store.emit(owner,trailing.view(),out)==error::invalid);
    auto invalid=one;invalid.storage[one.size-9]=0xff;assert(store.emit(owner,invalid.view(),out)==error::invalid);
    auto wrong=parsed;wrong.logger="robot.motor.secret";assert(store.emit(owner,encoded(wrong).view(),out)==error::denied);
    wrong=parsed;wrong.level=l::severity::fatal;assert(store.emit(owner,encoded(wrong).view(),out)==error::denied);
    std::array<std::uint8_t,1025> large{};assert(store.emit(owner,large,out)==error::too_large);
    auto accepted=emit(store,full);assert(accepted.record==1&&accepted.sequence==1&&accepted.accepted_bytes==full.size);
    bool called{};assert(store.drain(owner,[&](auto id,auto bytes){called=true;assert(id==1&&std::equal(bytes.begin(),bytes.end(),full.view().begin(),full.view().end()));return true;})==error::success&&called);
}
static void replay_and_capacity()
{
    l::store store;assert(store.configure(owner,plan())==error::success);auto one=message(1);auto a=emit(store,one);
    assert(emit(store,one)==a&&inspect(store).accepted==1);
    l::receipt out{};assert(store.emit(owner,message(1,"changed").view(),out)==error::conflict);
    assert(store.emit(owner,message(3).view(),out)==error::conflict);
    auto two=message(2);auto b=emit(store,two);assert(b.record==2&&!b.lost_before);
    assert(store.emit(owner,message(3).view(),out)==error::again&&inspect(store).full_refusals==1);
    auto wrong=message(3);wrong.storage[0]=18;assert(store.emit(owner,wrong.view(),out)==error::conflict);
    assert(store.drain(owner,[](auto id,auto){return id==1;})==error::success);
    auto c=emit(store,message(3));assert(c.record==3&&!c.lost_before&&inspect(store).retained==2);
    assert(store.emit(owner,two.view(),out)==error::conflict);
    assert(store.drain(owner,[](auto id,auto){return id==2;})==error::success);
    assert(store.drain(owner,[](auto id,auto){return id==3;})==error::success);
    assert(emit(store,message(3))==c&&inspect(store).retained==0&&inspect(store).exported==3);
}
static void explicit_loss_and_failed_delivery()
{
    l::store store;assert(store.configure(owner,plan(l::overflow::drop_oldest))==error::success);
    emit(store,message(1));emit(store,message(2));auto three=emit(store,message(3));assert(three.lost_before==1&&inspect(store).lost==1);
    auto before=inspect(store);assert(store.drain(owner,[](auto id,auto){assert(id==2);return false;})==error::io);
    assert(inspect(store).retained_bytes==before.retained_bytes&&inspect(store).copy_failures==1);
    try{store.drain(owner,[](auto,auto)->bool{throw 17;});assert(false);}catch(int value){assert(value==17);}
    assert(inspect(store).copy_failures==2&&!inspect(store).draining);
    l::receipt out;
    assert(store.drain(owner,[&](auto id,auto bytes)
    {
        assert(id==2&&bytes.size()==message(2).size&&inspect(store).draining);
        assert(store.drain(owner,[](auto,auto){assert(false);return true;})==error::again);
        assert(store.emit(owner,message(4).view(),out)==error::again);
        assert(emit(store,message(3))==three);
        return true;
    })==error::success);
    auto four=emit(store,message(4));assert(four.lost_before==1&&inspect(store).full_refusals==1);
    assert(store.drain(owner,[](auto id,auto){return id==3;})==error::success);
}
static void concurrent_pin_and_retirement()
{
    l::store store;assert(store.configure(owner,plan(l::overflow::drop_oldest))==error::success);emit(store,message(1));
    std::barrier step{2};error result{};
    std::thread consumer{[&]{result=store.drain(owner,[&](auto id,auto bytes){assert(id==1);l::request r;assert(l::decode(bytes,r)&&r.sequence==1);step.arrive_and_wait();step.arrive_and_wait();assert(l::decode(bytes,r)&&r.sequence==1);return true;});}};
    step.arrive_and_wait();emit(store,message(2));l::receipt out;assert(store.emit(owner,message(3).view(),out)==error::again);
    assert(store.retire(owner)==error::success&&store.emit(owner,message(3).view(),out)==error::cancelled);
    step.arrive_and_wait();consumer.join();assert(result==error::cancelled);
    auto state=inspect(store);assert(state.retired&&!state.draining&&state.retained==2&&!state.exported&&!state.lost);
    assert(store.drain(owner,[](auto,auto){assert(false);return true;})==error::cancelled);
}
static void real_allocation_failures_and_no_hot_allocations()
{
    auto p=plan();std::size_t count{};
    for(std::size_t at=1;at<=2;++at)
    {
        l::store store;allocations=0;fail_after=at;bool caught{};
        try{static_cast<void>(store.configure(owner,p));}catch(std::bad_alloc const&){caught=true;}
        fail_after=0;assert(caught&&allocations==at);++count;
        l::usage ignored;assert(store.inspect(owner,ignored)==error::denied&&store.configure(owner,p)==error::success);
        auto input=message(1);allocations=0;fail_after=1;
        emit(store,input);emit(store,input);
        assert(store.drain(owner,[](auto,auto){return false;})==error::io);
        assert(store.drain(owner,[](auto,auto){return true;})==error::success);
        static_cast<void>(inspect(store));assert(store.retire(owner)==error::success);
        fail_after=0;assert(!allocations);
    }
    assert(count==2);
}
static void fork_does_not_inherit_stream()
{
    l::store store;assert(store.configure(owner,plan())==error::success);emit(store,message(1));
    auto pid=::fork();assert(pid>=0);if(!pid)
    {
        l::receipt out;l::usage counters;auto bytes=message(2);
        bool good=store.emit(owner,bytes.view(),out)==error::denied&&store.inspect(owner,counters)==error::denied&&store.retire(owner)==error::denied&&
            store.drain(owner,[](auto,auto){return true;})==error::denied;
        ::_exit(good?0:1);
    }
    int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&WEXITSTATUS(status)==0&&inspect(store).accepted==1);
}
int main()
{
    configuration_and_owner();canonical_records_and_rejection();replay_and_capacity();explicit_loss_and_failed_delivery();
    concurrent_pin_and_retirement();real_allocation_failures_and_no_hot_allocations();fork_does_not_inherit_stream();
    std::cout<<"passed 7 log store security groups\n";
}
