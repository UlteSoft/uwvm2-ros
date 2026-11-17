/** @file @brief Recoverable fast_io allocation exhaustion before action creation admission. */
#define UWVM_COMMUNICATION_ACTION_FIXTURE_ONLY
#include "communication_action_dispatch.cpp"
#include <atomic>
static std::atomic<std::size_t> fail_after{};
static std::atomic<bool> failed{};
static bool reject()noexcept
{
    auto current=fail_after.load();
    while(current)if(fail_after.compare_exchange_weak(current,current-1))
    {if(current==1){failed=true;return true;}break;}
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
    for(auto role:{actions::role::client,actions::role::server})
    {
        bool complete{};unsigned failures{};
        for(std::size_t point{1};point<256;++point)
        {
            action_fixture f{role,true,false};assert(f.dispatch.prepare_async()==error::success);
            auto semantic=f.open();auto carrier=encode([&](auto& out){return out.encoded(semantic)&&out.u64(1)&&out.u64(c::now()+1000000000);});
            std::shared_ptr<s::communication_action_dispatch::opening> pending;auto before=f.origin.admissions("action");
            fail_after=point;failed=false;auto status=f.dispatch.stage_open(f.origin.who,f.cap,carrier,48,pending);
            bool injected=failed.load();fail_after=0;
            assert(f.origin.admissions("action")==before&&!f.native&&!f.endpoint);
            if(injected)
            {
                ++failures;assert(status==error::no_memory&&!pending);
                assert(f.dispatch.stage_open(f.origin.who,f.cap,carrier,48,pending)==error::success);
            }
            else{assert(status==error::success);complete=true;}
            assert(pending&&pending->node().live()&&!pending->request());
            if(complete)break;
        }
        assert(complete&&failures);std::printf("recoverable action draft allocations %s: %u\n",role==actions::role::client?"client":"server",failures);
    }
    std::puts("PASS typed action recoverable allocation: every draft allocation fails safely before debit or native effect, with bounded node ownership and successful retry");
}
