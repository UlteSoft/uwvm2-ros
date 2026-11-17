/** @file
 * @brief Exhaust each real recoverable fast_io allocation in dispatcher creation.
 * @details Linker malloc/calloc/realloc wrapping exercises actual containers and
 * shared ownership control blocks. Each failure must return without publishing a
 * handle or leaking retained workspace, then an uninjected retry must succeed.
 */
#if defined(UWVM_COMMUNICATION_TEST_GRAPH)
# define UWVM_COMMUNICATION_GRAPH_FIXTURE_ONLY
# include "communication_graph_dispatch.cpp"
#elif defined(UWVM_COMMUNICATION_TEST_EVENT)
# define UWVM_COMMUNICATION_EVENT_FIXTURE_ONLY
# include "communication_event_dispatch.cpp"
#elif defined(UWVM_COMMUNICATION_TEST_MUTATIONS) || defined(UWVM_COMMUNICATION_TEST_QUERIES)
# define UWVM_COMMUNICATION_DISPATCH_FIXTURE_ONLY
# include "communication_dispatch.cpp"
# include <uwvm2/imported/ros/scene/communication_queries.h>
#else
# define UWVM_COMMUNICATION_ACTION_FIXTURE_ONLY
# include "communication_action_dispatch.cpp"
#endif
static std::atomic<std::size_t> fail_after{};
static std::atomic<bool> failed{};
static bool reject() noexcept
{
    auto current=fail_after.load(std::memory_order_relaxed);
    while(current)
    {if(fail_after.compare_exchange_weak(current,current-1,std::memory_order_relaxed)){if(current==1){failed=true;return true;}break;}}
    return false;
}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t bytes){return reject()?nullptr:__real_malloc(bytes);}
extern "C" void* __wrap_calloc(std::size_t count,std::size_t bytes){return reject()?nullptr:__real_calloc(count,bytes);}
extern "C" void* __wrap_realloc(void* value,std::size_t bytes){return reject()?nullptr:__real_realloc(value,bytes);}
int main()
{
    std::size_t failures{};bool complete{};
    for(std::size_t point=1;point<128;++point)
    {
#if defined(UWVM_COMMUNICATION_TEST_GRAPH)
        graph_fixture f;auto input=f.request(gc::operation::watch);auto action=gc::operation::watch;
#elif defined(UWVM_COMMUNICATION_TEST_EVENT)
        event_fixture f;auto input=f.open();auto action=ec::operation::open;
#elif defined(UWVM_COMMUNICATION_TEST_MUTATIONS) || defined(UWVM_COMMUNICATION_TEST_QUERIES)
#if defined(UWVM_COMMUNICATION_TEST_QUERIES)
        s::communication_worker::queries worker;
        s::communication_jobs::plan capacity{{17,1,1},c::operation_family::topic,44,1,71,1,128,256,1000000000,768};
#else
        s::communication_worker::mutations worker;
        s::communication_jobs::plan capacity{{17,1,1},c::operation_family::topic,44,1,71,2,128,256,1000000000,896};
#endif
        auto validate=[](void*,c::operation_token const&,s::group_lease&)noexcept{return c::error::denied;};
        auto charge=[](void*,c::operation_token const&,std::uint64_t,s::group_lease&)noexcept{return c::error::denied;};
        fail_after=point;failed=false;auto status=worker.configure(capacity,validate,charge,nullptr);auto injected=failed.load();fail_after=0;
        if(injected){++failures;assert(status==c::error::no_memory&&!worker.occupied());assert(worker.configure(capacity,validate,charge,nullptr)==c::error::ok);}
        else{assert(status==c::error::ok&&!worker.occupied());complete=true;break;}
        continue;
#else
        action_fixture f;auto input=f.open();auto action=ac::operation::open;
#endif
#if !defined(UWVM_COMMUNICATION_TEST_MUTATIONS) && !defined(UWVM_COMMUNICATION_TEST_QUERIES)
        std::size_t required{};s::handle opened{};auto copy=[&](w::bytes bytes){w::reader reader{bytes};return reader.u64(opened)&&reader.done();};
        fail_after=point;failed=false;auto status=f.dispatch.invoke(f.origin.who,action,f.cap,input,4096,required,copy);
        auto injected=failed.load();fail_after=0;
        if(injected)
        {
            ++failures;assert(status==error::no_memory&&!opened);
#if defined(UWVM_COMMUNICATION_TEST_EVENT)
            assert(!f.reservations());
#endif
            assert(f.dispatch.invoke(f.origin.who,action,f.cap,input,4096,required,copy)==error::success&&opened);
        }
        else{assert(status==error::success&&opened);complete=true;break;}
#endif
    }
    assert(complete&&failures>=4);std::cout<<"passed communication dispatch allocation sweep: "<<failures<<" failures\n";
}
