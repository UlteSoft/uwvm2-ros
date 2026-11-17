/** @file @brief Exhaust actual recoverable fast_io allocations in typed endpoint preparation. */
#define UWVM_COMMUNICATION_TOPIC_FIXTURE_ONLY
#include "communication_topic_endpoint.cpp"
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
    for(bool subscription:{false,true})
    {
        unsigned failures{};bool complete{};
        for(std::size_t point=1;point<256;++point)
        {
            topic_environment f(subscription,{},true);auto endpoint=uwvm2::utils::container::make_unique<owned::topic_endpoint>();c::node_lease parent;
            assert(f.registry.attach_node(f.node_owner,f.node_id,f.node,parent)==c::error::ok);
            c::type_plan schema{f.type,"test/Bytes","canonical",{3},&bytes_valid,f.schema_owner};
            c::topic_plan policy{f.owner,f.topic,f.node,f.type,"/scene/data",subscription?c::role::subscription:c::role::publisher,f.quality,64,
                subscription?2u:0u,subscription?128u:0u,c::overflow::reject,1000000000};
            auto reserved=(subscription?3776u:2816u)+owned::endpoint_closure::reserved_bytes;
            jobs::plan queue{f.owner,c::operation_family::topic,f.topic_id,1,71,1,128,512,1000000000,reserved};
            s::communication_workspace capacity;s::communication_workspace::lease reservation,second;
            assert(capacity.configure(reserved,1)==s::error::success&&capacity.reserve(reserved,reservation)==s::error::success);
            fail_after=point;failed=false;
            auto status=endpoint->configure(policy,schema,std::move(parent),queue,9001,{128,512,reserved},topic_environment::admit,topic_environment::charge,&f,&reservation);
            auto injected=failed.load();fail_after=0;
            assert(!f.charges&&!f.pipes.drain()&&f.backend->creations==2);
            if(injected)
            {
                ++failures;assert(status==c::error::no_memory&&!endpoint->resource()&&reservation);
                assert(f.registry.attach_node(f.node_owner,f.node_id,f.node,parent)==c::error::ok);
                assert(endpoint->configure(policy,schema,std::move(parent),queue,9001,{128,512,reserved},topic_environment::admit,topic_environment::charge,&f,&reservation)==c::error::ok);
            }
            else{assert(status==c::error::ok);complete=true;}
            assert(endpoint->resource()==f.topic_id&&!endpoint->occupied()&&!reservation);
            assert(capacity.reserve(reserved,second)==s::error::again);
            assert(f.registry.abandon_adoption(f.slot,true)==c::error::ok);
            endpoint.reset();assert(capacity.reserve(reserved,second)==s::error::success);
            if(complete)break;
        }
        assert(complete&&failures>=10);
        std::printf("recoverable allocation points %s: %u\n",subscription?"subscription":"publisher",failures);
    }
    std::puts("PASS typed topic recoverable allocation: actual fast_io failure sweep, no native effects, no authority charge, no leaked node observer and successful retry");
}
