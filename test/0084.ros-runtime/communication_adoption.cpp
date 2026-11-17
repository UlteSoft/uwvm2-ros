// Exercise the real registry with unpublished reservations and typed worker
// facades. No native endpoint identity is ever accepted as a host resource.
#define main communication_core_fixture_entry
#include "communication_core.cpp"
#undef main
#include <atomic>

struct adoption_fixture
{
    std::shared_ptr<test_backend> backend{uwvm2::utils::container::make_shared<test_backend>()};
    c::registry registry{backend,{1,1,1,2,64,8,1}};
    c::owner context_owner{1,1,1},node_owner{1,2,1},topic_owner{1,3,1},service_owner{1,4,1},schema_owner{1,5,1};
    c::profile context{profile("context",1)},node{profile("node",2)},topic{profile("topic",3)},service{profile("service",4)},
        request{profile("request",5)},response{profile("response",6)};
    c::qos quality{1,1,2,1,2,{},{},{}};
    c::handle context_id{},node_id{};
    c::node_lease parent;
    adoption_fixture()
    {
        assert(registry.configure(c::context_plan{context_owner,context,4,{},c::transport::unauthenticated_loopback})==c::error::ok);
        assert(registry.configure(c::node_plan{node_owner,node,context,"worker","/scene",2})==c::error::ok);
        assert(registry.configure(c::type_plan{request,"test/Request","canonical",{1},&bytes_valid,schema_owner})==c::error::ok);
        assert(registry.configure(c::type_plan{response,"test/Response","canonical",{2},&bytes_valid,schema_owner})==c::error::ok);
        assert(registry.configure(c::topic_plan{topic_owner,topic,node,request,"/scene/in",c::role::subscription,quality,8,2,16,
            c::overflow::reject,10000000000ULL})==c::error::ok);
        assert(registry.configure(c::service_plan{service_owner,service,node,request,response,"/scene/service","test/Service",
            c::service_role::client,quality,2,8,8,16,10000000000ULL})==c::error::ok);
        assert(registry.open_context(context_owner,context,context_id)==c::error::ok);
        assert(registry.open_node(node_owner,context_owner,context_id,node,node_id)==c::error::ok);
        assert(registry.attach_node(node_owner,node_id,node,parent)==c::error::ok);
    }
};

static void topic_adoption()
{
    adoption_fixture f,foreign;
    c::endpoint_slot token,other;
    assert(f.registry.prepare_adoption({2,3,1},f.parent,f.topic,c::operation_family::topic,token)==c::error::denied);
    assert(f.registry.prepare_adoption(f.topic_owner,foreign.parent,f.topic,c::operation_family::topic,token)==c::error::denied);
    assert(f.registry.prepare_adoption(f.topic_owner,f.parent,f.topic,c::operation_family::service,token)==c::error::denied);
    assert(f.registry.prepare_adoption(f.topic_owner,f.parent,f.topic,c::operation_family::topic,token)==c::error::ok);
    auto id=token.resource();assert(id&&id!=f.node_id&&id!=f.context_id&&f.backend->creations==2);
    c::topic_info info;
    assert(f.registry.describe(f.topic_owner,id,info)==c::error::denied);
    c::handle untouched{999};
    assert(f.registry.open_topic(f.topic_owner,f.node_owner,f.node_id,f.topic,"/scene/in","test/Request",c::role::subscription,untouched)==c::error::busy);
    assert(untouched==999&&f.backend->creations==2);
    assert(f.registry.prepare_adoption(f.topic_owner,f.parent,f.topic,c::operation_family::topic,other)==c::error::busy&&!other);
    f.parent.reset();assert(f.registry.close(f.node_owner,f.node_id)==c::error::busy);
    auto value=uwvm2::utils::container::make_shared<test_topic>();
    assert(f.registry.adopt_topic(token,{},71)==c::error::denied&&token);
    assert(f.registry.adopt_topic(token,value,71)==c::error::denied&&token);
    value->evidence.provider_generation=71;
    assert(foreign.registry.adopt_topic(token,value,71)==c::error::denied&&token);
    assert(f.registry.adopt_topic(token,value,72)==c::error::denied&&token);
    value->evidence.size=33;assert(f.registry.adopt_topic(token,value,71)==c::error::denied&&token);
    value->evidence.size=0;
    c::endpoint_slot moved{std::move(token)};assert(!token&&moved.resource()==id);
    assert(f.registry.adopt_topic(moved,value,71)==c::error::ok&&!moved);
    assert(f.registry.adopt_topic(moved,value,71)==c::error::denied);
    assert(f.registry.describe(f.topic_owner,id,info)==c::error::ok);
    assert(f.registry.close(f.node_owner,f.node_id)==c::error::busy);
    assert(f.registry.close(f.topic_owner,id)==c::error::ok);
    assert(f.registry.close(f.node_owner,f.node_id)==c::error::ok);
    assert(f.backend->creations==2);
}

static void service_adoption()
{
    adoption_fixture f;c::endpoint_slot slot;
    assert(f.registry.prepare_adoption(f.service_owner,f.parent,f.service,c::operation_family::service,slot)==c::error::ok);
    auto value=uwvm2::utils::container::make_shared<test_service>();value->evidence.provider_generation=81;
    auto wrong=uwvm2::utils::container::make_shared<test_topic>();wrong->evidence.provider_generation=81;
    assert(f.registry.adopt_topic(slot,wrong,81)==c::error::denied);
    value->actual_channels[0]=c::qos{};
    value->actual_channels[0]->reliability=99;
    assert(f.registry.adopt_service(slot,value,81)==c::error::denied&&slot);
    value->actual_channels[0]=f.quality;value->actual_channels[1]=f.quality;
    auto id=slot.resource();assert(f.registry.adopt_service(slot,value,81)==c::error::ok&&!slot);
    c::service_info info;assert(f.registry.describe(f.service_owner,id,info)==c::error::ok);
    assert(f.registry.close(f.service_owner,id)==c::error::ok&&f.backend->creations==2);
}

static void cancellation_and_revocation()
{
    for(unsigned variant{};variant!=5;++variant)
    {
        adoption_fixture f;c::endpoint_slot slot;
        assert(f.registry.prepare_adoption(f.topic_owner,f.parent,f.topic,c::operation_family::topic,slot)==c::error::ok);
        auto old=slot.resource();
        assert(f.registry.abandon_adoption(slot,false)==c::error::busy&&slot.resource()==old);
        auto value=uwvm2::utils::container::make_shared<test_topic>();value->evidence.provider_generation=71;
        if(variant==1)f.registry.revoke(f.schema_owner);
        if(variant==2)f.registry.revoke(f.node_owner);
        if(variant==3)f.registry.revoke(f.context_owner);
        if(variant==4)f.registry.retire();
        if(variant)assert(f.registry.adopt_topic(slot,value,71)==c::error::denied&&slot);
        assert(f.registry.abandon_adoption(slot,true)==c::error::ok&&!slot);
        assert(f.registry.abandon_adoption(slot,true)==c::error::denied);
        auto status=f.registry.prepare_adoption(f.topic_owner,f.parent,f.topic,c::operation_family::topic,slot);
        if(!variant)
        {assert(status==c::error::ok&&slot.resource()!=old);assert(f.registry.abandon_adoption(slot,true)==c::error::ok);}
        else assert(status==c::error::denied&&!slot);
    }
}

static void concurrent_and_fork()
{
    adoption_fixture f;
    std::array<c::endpoint_slot,2> slots;
    std::array<c::error,2> outcomes{};
    std::atomic<unsigned> ready{};
    auto run=[&](unsigned i){ready.fetch_add(1);while(ready.load()!=2)std::this_thread::yield();
        outcomes[i]=f.registry.prepare_adoption(f.topic_owner,f.parent,f.topic,c::operation_family::topic,slots[i]);};
    std::thread first{run,0},second{run,1};first.join();second.join();
    assert((outcomes[0]==c::error::ok&&outcomes[1]==c::error::busy)||(outcomes[1]==c::error::ok&&outcomes[0]==c::error::busy));
    auto& slot=slots[outcomes[0]==c::error::ok?0:1];
#if defined(__linux__)
    auto child=::fork();assert(child>=0);
    if(!child)
    {
        assert(f.registry.abandon_adoption(slot,true)==c::error::denied);
        auto value=uwvm2::utils::container::make_shared<test_topic>();value->evidence.provider_generation=71;
        assert(f.registry.adopt_topic(slot,value,71)==c::error::denied);
        ::_exit(0);
    }
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
#endif
    assert(f.registry.abandon_adoption(slot,true)==c::error::ok);
}

int main()
{
    communication_core_fixture_entry();
    topic_adoption();service_adoption();cancellation_and_revocation();concurrent_and_fork();
    std::cout<<"PASS typed asynchronous endpoint adoption: authority, identity, cleanup, revocation, concurrency and fork\n";
}
