/** @file @brief Bounded actual sensor storage, ownership, timestamps, loss and retained delivery. */
#include <uwvm2/imported/ros/scene/sensor_core.h>
#include <cassert>
#include <iostream>
#include <barrier>
#include <thread>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;namespace n=s::sensors;using n::error;
static std::uint64_t time_now=1000;static std::uint64_t clock_now()noexcept{return time_now;}
static constexpr n::owner owner{1,2,3};static constexpr n::source_identity source{4,5,6,7,8,9};
static constexpr std::array<std::uint8_t,4> payload{1,2,3,4};
#if defined(UWVM_SENSOR_FAULT_INJECTION)
static thread_local std::size_t fail_after{};static thread_local bool allocation_failed{};
extern "C" void* __real_malloc(std::size_t);extern "C" void* __real_calloc(std::size_t,std::size_t);extern "C" void* __real_realloc(void*,std::size_t);
static bool fail()noexcept{if(fail_after&&!--fail_after){allocation_failed=true;return true;}return false;}
extern "C" void* __wrap_malloc(std::size_t value){return fail()?nullptr:__real_malloc(value);}
extern "C" void* __wrap_calloc(std::size_t a,std::size_t b){return fail()?nullptr:__real_calloc(a,b);}
extern "C" void* __wrap_realloc(void* a,std::size_t b){return fail()?nullptr:__real_realloc(a,b);}
#endif
static s::buffers::profile profile(std::string_view name)
{s::buffers::profile out;assert(out.identity.assign(name));out.sha256.fill(42);return out;}
static n::plan plan()
{
    n::plan out;out.profile=profile("sensor.plan");out.schema=profile("sensor.frame");assert(out.type.assign("bounded.sample")&&out.frame.assign("base")&&out.units.assign("metres")&&out.encoding.assign("bytes"));
    out.handles=2;out.records=2;out.body_bytes=4;out.source_sequence=out.source_stamp=true;out.validate_body=[](auto bytes)noexcept{return bytes.size()==4&&bytes[0]==1;};
    assert(n::core::required_storage(out,out.storage_bytes));return out;
}
static n::source_sample sample(std::uint64_t sequence=1){return {4,5,sequence,8,9,static_cast<std::int64_t>(sequence),true};}
static n::usage inspect(n::core& core){n::usage value;assert(core.inspect(owner,value)==error::success);return value;}
static n::handle open(n::core& core,std::uint64_t resources=2)
{n::handle result{};assert(core.open(owner,plan().profile,resources,[&](auto id){result=id;return true;})==error::success);return result;}
static void configuration_owner_zero_and_no_side_effects()
{
    n::core core{clock_now};auto policy=plan();std::uint64_t needed{};auto bad=policy;bad.handles=65;assert(!n::core::required_storage(bad,needed));
    bad=policy;bad.body_bytes=s::buffers::maximum_transfer+1;assert(!n::core::required_storage(bad,needed));
    bad=policy;bad.storage_bytes--;assert(core.configure(owner,bad,source)==error::too_large);assert(core.configure(owner,policy,source)==error::success);
    assert(core.configure(owner,policy,source)==error::denied);assert(core.ingest({1,2,4},sample(),payload)==error::denied);
    assert(core.open(owner,policy.profile,0,[](auto){assert(false);return true;})==error::too_large);
    assert(core.open(owner,profile("other"),2,[](auto){assert(false);return true;})==error::denied);
    assert(core.open(owner,policy.profile,2,[](auto){return false;})==error::io&&!inspect(core).retained_handles);
    auto id=open(core);assert(core.close({1,2,4},id)==error::denied&&inspect(core).handles==1);
    assert(core.close(owner,id)==error::success&&core.close(owner,id)==error::bad_handle);
    n::core zero;policy.handles=policy.records=policy.body_bytes=0;assert(n::core::required_storage(policy,policy.storage_bytes));assert(zero.configure(owner,policy,source)==error::success);
    assert(zero.open(owner,policy.profile,1,[](auto){assert(false);return true;})==error::too_large&&zero.ingest(owner,sample(),{})==error::again);
}
static void complete_delivery_loss_and_failed_copy()
{
    n::core core{clock_now};assert(core.configure(owner,plan(),source)==error::success);auto id=open(core);
    assert(core.ingest(owner,sample(1),payload)==error::success&&core.ingest(owner,sample(3),payload)==error::success);
    assert(inspect(core).losses==1&&inspect(core).captures==2);
    assert(core.ingest(owner,sample(4),payload)==error::again&&inspect(core).full_refusals==1);
    assert(core.take(owner,id,[](auto const& frame){assert(frame.metadata.source.sequence==1&&frame.body.size()==4&&frame.frame=="base"&&frame.units=="metres");return false;})==error::io);
    assert(core.take(owner,id,[](auto const& frame){assert(frame.metadata.source.sequence==1&&frame.metadata.capture_sequence==1&&frame.metadata.captured_steady_ns==1000);return true;})==error::success);
    assert(core.ingest(owner,sample(4),payload)==error::success);
    assert(core.take(owner,id,[](auto const& frame){assert(frame.metadata.source.sequence==3&&frame.metadata.loss_count==1);return true;})==error::success);
    assert(core.take(owner,id,[](auto const& frame){return frame.metadata.source.sequence==4;})==error::success&&inspect(core).records==0);
}
static void malformed_generation_clock_and_schema()
{
    n::core core{clock_now};assert(core.configure(owner,plan(),source)==error::success);
    auto invalid=sample();invalid.source++;assert(core.ingest(owner,invalid,payload)==error::invalid);
    invalid=sample();invalid.generation++;assert(core.ingest(owner,invalid,payload)==error::invalid);
    invalid=sample();invalid.clock++;assert(core.ingest(owner,invalid,payload)==error::invalid);
    invalid=sample();invalid.epoch=0;assert(core.ingest(owner,invalid,payload)==error::invalid);
    auto bad=payload;bad[0]=0;assert(core.ingest(owner,sample(),bad)==error::invalid);
    assert(core.ingest(owner,sample(),payload)==error::success&&core.ingest(owner,sample(),payload)==error::conflict);
    invalid=sample(2);invalid.source_ns=0;assert(core.ingest(owner,invalid,payload)==error::conflict);
    invalid.epoch++;assert(core.ingest(owner,invalid,payload)==error::success);
    assert(core.ingest(owner,sample(3),payload)==error::conflict);
}
static void explicit_eviction_and_retained_close()
{
    auto policy=plan();policy.handles=1;policy.full=n::overflow::drop_oldest;n::core core{clock_now};assert(core.configure(owner,policy,source)==error::success);auto id=open(core,1);
    assert(core.ingest(owner,sample(1),payload)==error::success&&core.ingest(owner,sample(2),payload)==error::success&&core.ingest(owner,sample(3),payload)==error::success&&inspect(core).losses==1);
    assert(core.take(owner,id,[&](auto const& frame)
    {
        assert(frame.metadata.source.sequence==2&&frame.metadata.loss_count==1);assert(core.ingest(owner,sample(4),payload)==error::again);
        assert(core.close(owner,id)==error::success&&!inspect(core).handles&&inspect(core).retained_handles==1);
        assert(core.open(owner,policy.profile,1,[](auto){assert(false);return true;})==error::too_large);return true;
    })==error::cancelled);
    assert(inspect(core).records==2&&!inspect(core).retained_handles);id=open(core,1);
    try{core.take(owner,id,[](auto const&)->bool{throw 42;});assert(false);}catch(int value){assert(value==42);}
    assert(!inspect(core).delivering&&inspect(core).records==2);
    assert(core.take(owner,id,[](auto const& frame){return frame.metadata.source.sequence==2;})==error::success);
}
static void concurrent_retire_and_fork()
{
    n::core core{clock_now};assert(core.configure(owner,plan(),source)==error::success);auto id=open(core);assert(core.ingest(owner,sample(),payload)==error::success);
    std::barrier rendezvous{2};error status{};
    std::thread worker{[&]{status=core.take(owner,id,[&](auto const& frame){rendezvous.arrive_and_wait();rendezvous.arrive_and_wait();return std::equal(frame.body.begin(),frame.body.end(),payload.begin(),payload.end());});}};
    rendezvous.arrive_and_wait();assert(core.retire(owner)==error::success);rendezvous.arrive_and_wait();worker.join();assert(status==error::cancelled&&inspect(core).records==1);
    auto child=::fork();assert(child>=0);if(!child){::alarm(3);n::usage out;::_exit(core.inspect(owner,out)==error::denied&&core.close(owner,id)==error::denied?0:4);}
    int result{};assert(::waitpid(child,&result,0)==child&&WIFEXITED(result)&&!WEXITSTATUS(result));
}
static void recoverable_allocations_and_no_hot_allocation()
{
#if defined(UWVM_SENSOR_FAULT_INJECTION)
    unsigned failed{};
    for(std::size_t point=1;point<=8;++point)
    {
        n::core core{clock_now};auto policy=plan();fail_after=point;allocation_failed=false;error status{};
        try{status=core.configure(owner,policy,source);}catch(std::bad_alloc const&){status=error::no_memory;}fail_after=0;
        if(!allocation_failed){assert(status==error::success);break;}++failed;assert(status==error::no_memory&&core.configure(owner,policy,source)==error::success);
    }
    assert(failed==2);n::core core{clock_now};assert(core.configure(owner,plan(),source)==error::success);fail_after=1;allocation_failed=false;
    auto id=open(core);assert(core.ingest(owner,sample(),payload)==error::success&&core.take(owner,id,[](auto const&){return true;})==error::success&&core.close(owner,id)==error::success);
    assert(!allocation_failed&&fail_after==1);fail_after=0;
#endif
}
static thread_local n::core* retiring_validator{};
static void committed_capture_receipts_and_reentrant_retirement()
{
    n::core core{clock_now};assert(core.configure(owner,plan(),source)==error::success);auto id=open(core);n::frame_metadata captured,latest;
    latest.capture_sequence=99;assert(core.observation(owner,id,latest)==error::again&&latest.capture_sequence==99);
    assert(core.ingest(owner,sample(),payload,&captured)==error::success&&captured.capture_sequence==1&&captured.captured_steady_ns==1000);
    assert(core.observation(owner,id,latest)==error::success&&latest==captured);auto untouched=captured;
    assert(core.ingest(owner,sample(),payload,&untouched)==error::conflict&&untouched==captured);
    assert(core.take(owner,id,[](auto const&){return true;})==error::success&&core.observation(owner,id,latest)==error::success&&latest==captured);
    assert(core.close(owner,id)==error::success&&core.observation(owner,id,latest)==error::bad_handle);
    n::core reentrant{clock_now};auto policy=plan();policy.validate_body=[](n::bytes)noexcept{assert(retiring_validator->retire(owner)==error::success);return true;};
    assert(reentrant.configure(owner,policy,source)==error::success);id=open(reentrant);retiring_validator=&reentrant;untouched=captured;
    assert(reentrant.ingest(owner,sample(),payload,&untouched)==error::cancelled&&untouched==captured&&!inspect(reentrant).captures);
    assert(reentrant.observation(owner,id,latest)==error::denied);retiring_validator=nullptr;
}
int main()
{
    configuration_owner_zero_and_no_side_effects();complete_delivery_loss_and_failed_copy();malformed_generation_clock_and_schema();
    explicit_eviction_and_retained_close();concurrent_retire_and_fork();recoverable_allocations_and_no_hot_allocation();
    committed_capture_receipts_and_reentrant_retirement();std::cout<<"passed 7 sensor core safety groups\n";return 0;
}
