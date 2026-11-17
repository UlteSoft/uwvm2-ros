/** @file @brief Exhaust every recoverable fast_io endpoint-draft allocation before native admission. */
#define UWVM_COMMUNICATION_DISPATCH_FIXTURE_ONLY
#include "communication_dispatch.cpp"
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
    for(bool service:{false,true})for(bool server:{false,true})
    {
        bool complete{};unsigned failures{};
        for(std::size_t point{1};point<256;++point)
        {
            fixture f(false,false,{},false,false,false,false,server?c::service_role::server:c::service_role::client);
            assert(f.dispatch.prepare_async(f.native)==error::success);
            f.context_id=f.opened(contract::operation::context_open,f.context_cap,open_profile("ctx"));
            auto node=encode([&](auto& out){return out.u64(f.context_cap)&&out.u64(f.context_id)&&contract::encode_profile(out,profile("node"));});
            f.node_id=f.opened(contract::operation::node_open,f.node_cap,node);
            auto semantic=f.endpoint(service?"srv":server?"sub":"pub",service?(server?"server":"client"):(server?"subscription":"publisher"));
            auto carrier=encode([&](auto& out){return out.encoded(semantic)&&out.u64(1)&&out.u64(c::now()+5000000000);});
            auto action=service?contract::operation::service_open:contract::operation::topic_open;
            auto cap=service?f.service_cap:server?f.sub_cap:f.pub_cap;
            auto alias=service?"service":server?"subscriber":"publisher";
            std::shared_ptr<s::communication_dispatch::opening> pending;
            fail_after=point;failed=false;auto status=f.dispatch.stage_open(f.who,action,cap,carrier,48,pending);
            bool injected=failed.load();fail_after=0;
            assert(f.admissions(alias)==0&&f.native->creations==2);
            if(injected)
            {
                ++failures;assert(status==error::no_memory&&!pending);
                assert(f.dispatch.stage_open(f.who,action,cap,carrier,48,pending)==error::success);
            }
            else{assert(status==error::success);complete=true;}
            assert(pending&&pending->node().live()&&!pending->request());
            if(complete)break;
        }
        assert(complete&&failures);std::printf("recoverable endpoint draft allocations %s %s: %u\n",service?"service":"topic",server?"receive":"send",failures);
    }
    std::puts("PASS endpoint draft recoverable allocation: all four roles fail safely before debit/native effects, preserve bounded node ownership and permit exact retry");
}
