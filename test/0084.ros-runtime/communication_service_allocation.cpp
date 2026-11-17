/** @file @brief Exhaust actual recoverable fast_io allocations in typed endpoint preparation. */
#define UWVM_COMMUNICATION_SERVICE_FIXTURE_ONLY
#include "communication_service_endpoint.cpp"
#include <atomic>
static std::atomic<std::size_t> fail_after{};
static std::atomic<bool> failed{};
static bool reject()noexcept
{
    auto current=fail_after.load();
    while(current)if(fail_after.compare_exchange_weak(current,current-1)){if(current==1){failed=true;return true;}break;}
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
    for(bool server:{false,true})
    {
        unsigned failures{};bool complete{};
        for(std::size_t point=1;point<256;++point)
        {
            service_environment f(server,{},true);c::node_lease parent;
            assert(f.registry.attach_node(f.node_owner,f.node_id,f.node,parent)==c::error::ok);
            auto bytes=owned::service_endpoint::reservation_bytes(f.policy,128,512);
            s::communication_workspace capacity;s::communication_workspace::lease reservation,second;
            assert(capacity.configure(bytes,1)==s::error::success&&capacity.reserve(bytes,reservation)==s::error::success);
            fail_after=point;failed=false;
            auto status=f.configure(std::move(parent),&reservation);auto injected=failed.load();fail_after=0;
            assert(!f.charges&&!f.pipes.drain()&&f.backend->creations==2);
            if(injected)
            {
                ++failures;assert(status==c::error::no_memory&&!f.endpoint->resource()&&reservation);
                assert(f.registry.attach_node(f.node_owner,f.node_id,f.node,parent)==c::error::ok);
                assert(f.configure(std::move(parent),&reservation)==c::error::ok);
            }
            else{assert(status==c::error::ok);complete=true;}
            assert(f.endpoint->resource()==f.service_id&&!f.endpoint->occupied()&&!reservation);
            assert(capacity.reserve(bytes,second)==s::error::again);
            assert(f.registry.abandon_adoption(f.slot,true)==c::error::ok);
            f.endpoint.reset();assert(capacity.reserve(bytes,second)==s::error::success);
            if(complete)break;
        }
        assert(complete&&failures>=15);
        std::printf("recoverable allocation points %s: %u\n",server?"service server":"service client",failures);
    }
    std::puts("PASS typed service recoverable allocation: actual fast_io failure sweep, no native effects, no authority charge, no leaked node observer and successful retry");
}
