/** @file @brief Canonical asynchronous topic/service drafts, ownership and recovery.
 * @details The production creation ledger models verified provider results.
 * This suite checks host decisions; actual middleware uses a separate fixture.
 */
#define UWVM_COMMUNICATION_DISPATCH_FIXTURE_ONLY
#include "communication_dispatch.cpp"
namespace creation=s::communication_creation;
namespace opens=uwvm2::imported::ros::communication_open_contract;
using draft=s::communication_dispatch::opening;
struct async_endpoint
{
    fixture f;
    bool is_service{},is_server{};
    s::handle cap{};
    text alias;
    contract::operation action{};
    std::shared_ptr<draft> pending;
    creation::ledger ledger;creation::plan policy;creation::view receipt;bytes carrier;
    unsigned enqueues{},inspections{};
    bool reject_enqueue{};
    static c::error enqueue(void* context,draft const& pending,w::bytes input,creation::view& output)noexcept
    {
        auto& self=*static_cast<async_endpoint*>(context);++self.enqueues;if(self.reject_enqueue)return c::error::again;
        auto charge=[](void* context,creation::identity const& identity,std::uint64_t bytes,s::group_lease& held)noexcept
        {return static_cast<c::error>(static_cast<async_endpoint*>(context)->f.dispatch.admit_open(identity,bytes,held,true));};
        auto status=self.ledger.enqueue(pending.identity(),input,charge,&self,output);if(status==c::error::ok)self.receipt=output;return status;
    }
    static c::error inspect(void* context,draft const& pending,creation::view& output)noexcept
    {auto& self=*static_cast<async_endpoint*>(context);++self.inspections;return self.ledger.inspect(pending.identity().who,pending.request(),output);}
    explicit async_endpoint(bool service=false,bool server=false,std::optional<std::uint64_t> calls={}):
        f(false,false,{},false,false,false,false,server?c::service_role::server:c::service_role::client,calls),is_service(service),is_server(server)
    {
        assert(f.dispatch.prepare_async(f.native)==error::success);
        f.context_id=f.opened(contract::operation::context_open,f.context_cap,open_profile("ctx"));
        auto node_request=encode([&](auto& out){return out.u64(f.context_cap)&&out.u64(f.context_id)&&contract::encode_profile(out,profile("node"));});
        f.node_id=f.opened(contract::operation::node_open,f.node_cap,node_request);
        cap=service?f.service_cap:server?f.sub_cap:f.pub_cap;alias=service?"service":server?"subscriber":"publisher";
        action=service?contract::operation::service_open:contract::operation::topic_open;
        auto semantic=f.endpoint(service?"srv":server?"sub":"pub",service?(server?"server":"client"):(server?"subscription":"publisher"));
        carrier=encode([&](auto& out){return out.encoded(semantic)&&out.u64(1)&&out.u64(c::now()+5000000000);});
        assert(f.dispatch.stage_open(f.who,action,cap,carrier,47,pending)==error::too_large&&!pending);
        assert(f.dispatch.stage_open(f.who,action,cap,carrier,48,pending)==error::success&&pending);
        assert(pending->identity().count==(service?5u:4u)&&pending->resource()!=f.node_id&&pending->node().live());
        assert(pending->schema(0)&&pending->schema(service?1:0)&&!pending->schema(service?2:1));
        policy={pending->identity().authority,profile(service?"srv":server?"sub":"pub"),service?opens::endpoint_kind::service:opens::endpoint_kind::topic,1,71,4096,10000000000};
        assert(ledger.configure({&policy,1},8192)==c::error::ok);
        bytes output;assert(f.call(action,cap,semantic,output)==error::unsupported&&f.native->creations==2);
        s::communication_dispatch::usage usage;assert(f.dispatch.inspect(alias,usage)==error::success&&!usage.handles&&!usage.orphaned);
        auto describe=service?contract::operation::service_describe:contract::operation::topic_describe;
        auto unavailable=f.call(describe,cap,resource(pending->resource()),output);
        assert(unavailable==error::bad_handle||(calls&&!*calls&&unavailable==error::again));
    }
    error invoke(s::communication_dispatch::sink sink={},uwvm2::utils::container::function_ref<bool()> cancelled={})
    {
        auto copy=[](w::bytes){return true;};std::size_t required{};
        return f.dispatch.invoke_open(f.who,action,cap,carrier,48,required,sink?sink:s::communication_dispatch::sink(copy),{this,enqueue,inspect},cancelled);
    }
    void commit()
    {
        auto before=f.admissions(alias);auto attempts=enqueues,observations=inspections;bytes lost,retry;
        assert(invoke([&](w::bytes value){lost.assign(value.begin(),value.end());return false;})==error::io);
        assert(enqueues==attempts+1&&inspections==observations&&f.admissions(alias)==before+1&&pending->request()==receipt.receipt.request_id);
        assert(invoke([&](w::bytes value){retry.assign(value.begin(),value.end());return true;})==error::success);
        assert(lost==retry&&enqueues==attempts+1&&inspections==observations+1&&f.admissions(alias)==before+1);
        auto wrong=pending->identity();++wrong.origins[2].resource;s::group_lease held;
        assert(f.dispatch.admit_open(wrong,0,held)==error::denied);
        assert(f.dispatch.admit_open(pending->identity(),1,held,true)==error::invalid);
    }
    void adopt()
    {
        s::group_lease held;assert(f.dispatch.admit_open(pending->identity(),0,held)==error::success);
        creation::work work;assert(ledger.begin(pending->request(),held,work)==c::error::ok);
        assert(ledger.complete(pending->request(),c::error::ok,c::effect::locally_accepted,9001)==c::error::ok);
        c::node_lease node;assert(f.dispatch.adoption_node(pending,node)==error::success&&node.live());
        if(is_service)
        {
            auto endpoint=uwvm2::utils::container::make_shared<service>();endpoint->endpoint_evidence.provider_generation=72;
            assert(f.dispatch.adopt_open(pending,endpoint)==error::denied);
            endpoint->endpoint_evidence.provider_generation=71;assert(f.dispatch.adopt_open(pending,endpoint)==error::success);
            assert(f.dispatch.adopt_open(pending,endpoint)==error::success);
            assert(f.dispatch.adopt_open(pending,uwvm2::utils::container::make_shared<service>())==error::conflict);
        }
        else
        {
            auto endpoint=uwvm2::utils::container::make_shared<topic>();endpoint->evidence.provider_generation=72;
            assert(f.dispatch.adopt_open(pending,endpoint)==error::denied);
            endpoint->evidence.provider_generation=71;assert(f.dispatch.adopt_open(pending,endpoint)==error::success);
            assert(f.dispatch.adopt_open(pending,endpoint)==error::success);
            assert(f.dispatch.adopt_open(pending,uwvm2::utils::container::make_shared<topic>())==error::conflict);
        }
        opens::published published;assert(ledger.publish(f.who,pending->request(),held,pending->resource(),published)==c::error::ok);
        auto wrong=published;++wrong.handle;assert(f.dispatch.publish_open(pending,wrong,held)==error::denied);
        assert(f.dispatch.publish_open(pending,published,held)==error::success&&f.dispatch.publish_open(pending,published,held)==error::success);
        assert(f.native->creations==2);
    }
};
static void all_roles()
{
    for(bool service:{false,true})for(bool server:{false,true})
    {
        async_endpoint a(service,server);a.commit();a.adopt();bytes out;
        assert(a.invoke()==error::success&&a.enqueues==1);
        assert(a.f.call(service?contract::operation::service_describe:contract::operation::topic_describe,a.cap,resource(a.pending->resource()),out)==error::success);
        assert(a.f.dispatch.release_open(a.pending,a.receipt)==error::again);
        assert(a.f.dispatch.close(a.f.who,a.cap,a.pending->resource())==error::success);
        assert(a.ledger.cleanup(a.pending->request(),true)==c::error::ok&&a.ledger.release(a.f.who,a.pending->request())==c::error::ok);
        assert(a.ledger.inspect(a.f.who,a.pending->request(),a.receipt)==c::error::ok);
        auto forged=a.receipt;++forged.provider_generation;assert(a.f.dispatch.release_open(a.pending,forged)==error::again);
        assert(a.f.dispatch.release_open(a.pending,a.receipt)==error::success);
        assert(a.f.dispatch.release_open(a.pending,a.receipt)==error::success);
        auto wrong_release=a.receipt;++wrong_release.receipt.request_id;
        assert(a.f.dispatch.release_open(a.pending,wrong_release)==error::again);
        auto previous=a.pending;a.carrier[a.carrier.size()-16]=2;
        assert(a.f.dispatch.stage_open(a.f.who,a.action,a.cap,a.carrier,48,a.pending)==error::success&&a.pending!=previous&&a.pending->resource()!=previous->resource());
    }
}
static void rollback_and_retry()
{
    async_endpoint a;a.reject_enqueue=true;auto old=a.pending;
    assert(a.invoke()==error::again&&a.f.admissions(a.alias)==0);
    assert(a.f.dispatch.stage_open(a.f.who,a.action,a.cap,a.carrier,48,a.pending)==error::success&&a.pending!=old);
    unsigned callbacks{};assert(a.invoke({},[&]()->bool{if(++callbacks==2)throw 1;return false;})==error::io);
    old=a.pending;assert(a.f.dispatch.stage_open(a.f.who,a.action,a.cap,a.carrier,48,a.pending)==error::success&&a.pending!=old);
    a.reject_enqueue=false;a.commit();
    auto changed=a.carrier;changed.back()^=1;
    assert(a.f.dispatch.stage_open(a.f.who,a.action,a.cap,changed,48,old)==error::conflict);
    auto foreign=a.f.who;++foreign.generation;
    assert(a.f.dispatch.stage_open(foreign,a.action,a.cap,a.carrier,48,old)!=error::success);
}
static void zero_and_final_budget()
{
    async_endpoint zero(false,false,0);assert(zero.invoke()==error::again&&zero.f.admissions(zero.alias)==0);
    async_endpoint last(false,false,1);last.commit();assert(last.invoke()==error::success&&last.f.admissions(last.alias)==1);
    assert(last.f.dispatch.preflight(last.f.who,last.action,last.cap,last.carrier.size())==error::success);
    assert(last.f.authority->close(last.f.who,last.f.node_cap)==error::success);
    assert(last.invoke()==error::success&&last.enqueues==1);
    s::group_lease held;assert(last.f.dispatch.admit_open(last.pending->identity(),0,held)!=error::success);
}
static void cancellation_and_retirement()
{
    for(unsigned kind{};kind<4;++kind)
    {
        async_endpoint a(bool(kind&1));a.commit();
        if(kind&2)a.f.dispatch.revoke(a.alias);
        else assert(a.f.authority->close(a.f.who,a.f.node_cap)==error::success);
        s::group_lease held;assert(a.f.dispatch.admit_open(a.pending->identity(),0,held)!=error::success);
        c::node_lease node;assert(a.f.dispatch.adoption_node(a.pending,node)!=error::success&&!node);
        assert(a.ledger.cancel(a.f.who,a.pending->request(),a.receipt)==c::error::ok);
        assert(a.ledger.release(a.f.who,a.pending->request())==c::error::ok);
        assert(a.ledger.inspect(a.f.who,a.pending->request(),a.receipt)==c::error::ok);
        assert(a.f.dispatch.release_open(a.pending,a.receipt)==error::success);
    }
}
static void concurrent_and_fork()
{
    async_endpoint a;assert(a.invoke([&](w::bytes){assert(a.invoke()==error::again);return true;})==error::success);
    std::array<std::thread,4> readers;
    for(auto& thread:readers)thread=std::thread{[&]{for(unsigned i{};i<100;++i){std::shared_ptr<draft> retained;
        assert(a.f.dispatch.stage_open(a.f.who,a.action,a.cap,a.carrier,48,retained)==error::success&&retained==a.pending);}}};
    for(auto& thread:readers)thread.join();
    auto child=::fork();assert(child>=0);if(!child){s::group_lease held;_exit(a.f.dispatch.admit_open(a.pending->identity(),0,held)==error::denied?0:1);}
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
}
#ifndef UWVM_COMMUNICATION_ENDPOINT_ADOPTION_FIXTURE_ONLY
int main(){all_roles();rollback_and_retry();zero_and_final_budget();cancellation_and_retirement();concurrent_and_fork();
    std::puts("PASS asynchronous endpoint adoption: four roles, exact origins, independent requests/resources, lost copy, zero quota, revoked parents, cleanup, reentrancy, concurrent retry and fork rejection");}
#endif
