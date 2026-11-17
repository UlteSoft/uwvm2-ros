/** @file @brief Opaque asynchronous action drafts and exact typed dispatcher adoption.
 * @details Native creation evidence is modelled by the production ledger; this
 * fixture verifies the public action dispatcher, not a middleware transport.
 */
#define UWVM_COMMUNICATION_ACTION_FIXTURE_ONLY
#include "communication_action_dispatch.cpp"
namespace creation=s::communication_creation;
namespace opens=uwvm2::imported::ros::communication_open_contract;
struct async_action
{
    action_fixture f{actions::role::client,true,false};
    std::shared_ptr<s::communication_action_dispatch::opening> pending;
    creation::ledger ledger;creation::plan policy;creation::view receipt;bytes carrier;
    unsigned enqueues{},inspections{};
    static c::error enqueue(void* context,s::communication_action_dispatch::opening const& pending,w::bytes input,creation::view& output)noexcept
    {
        auto& self=*static_cast<async_action*>(context);++self.enqueues;
        auto charge=[](void* context,creation::identity const& identity,std::uint64_t bytes,s::group_lease& held)noexcept
        {return static_cast<c::error>(static_cast<async_action*>(context)->f.dispatch.admit_open(identity,bytes,held,true));};
        auto status=self.ledger.enqueue(pending.identity(),input,charge,&self,output);if(status==c::error::ok)self.receipt=output;return status;
    }
    static c::error inspect(void* context,s::communication_action_dispatch::opening const& pending,creation::view& output)noexcept
    {auto& self=*static_cast<async_action*>(context);++self.inspections;return self.ledger.inspect(pending.identity().who,pending.request(),output);}
    async_action()
    {
        assert(f.dispatch.prepare_async()==error::success);auto semantic=f.open();
        auto until=c::now()+1000000000;
        carrier=encode([&](auto& out){return out.encoded(semantic)&&out.u64(1)&&out.u64(until);});
        auto before=f.origin.admissions("action");
        assert(f.dispatch.stage_open(f.origin.who,f.cap,carrier,47,pending)==error::too_large&&!pending);
        assert(f.dispatch.stage_open(f.origin.who,f.cap,carrier,48,pending)==error::success&&pending&&f.origin.admissions("action")==before);
        auto expected=pending;assert(f.dispatch.stage_open(f.origin.who,f.cap,carrier,48,pending)==error::success&&pending==expected);
        auto changed=carrier;changed[changed.size()-16]=2;
        assert(f.dispatch.stage_open(f.origin.who,f.cap,changed,48,pending)==error::conflict);
        s::communication_action_dispatch::plan frozen;assert(f.dispatch.describe_plan("action",frozen)==error::success);
        policy={pending->identity().authority,frozen.policy.selected,opens::endpoint_kind::action,1,71,4096,10000000000};
        assert(ledger.configure({&policy,1},8192)==c::error::ok);
        std::size_t required{};bytes first,second;
        assert(f.dispatch.invoke_open(f.origin.who,f.cap,carrier,48,required,[&](w::bytes value){first.assign(value.begin(),value.end());return false;},{this,enqueue,inspect})==error::io);
        assert(f.origin.admissions("action")==before+1&&enqueues==1&&inspections==0);
        assert(f.dispatch.invoke_open(f.origin.who,f.cap,carrier,48,required,[&](w::bytes value){second.assign(value.begin(),value.end());return true;},{this,enqueue,inspect})==error::success);
        assert(first==second&&enqueues==1&&inspections==1&&f.origin.admissions("action")==before+1);
        auto altered=pending->identity();++altered.origins[4].authority.grant;s::group_lease held;
        assert(f.dispatch.admit_open(altered,0,held)==error::denied);
        assert(f.dispatch.admit_open(pending->identity(),1,held,true)==error::invalid);
        s::communication_action_dispatch::usage usage;assert(f.dispatch.inspect({},usage)==error::success&&!usage.handles);
        bytes out;assert(f.call(ac::operation::open,semantic,out)==error::unsupported);
        assert(f.call(ac::operation::describe,encode([&](auto& out){return out.u64(pending->resource())&&out.boolean(false);}),out)==error::bad_handle);
    }
    void adopt()
    {
        s::group_lease held;assert(f.dispatch.admit_open(pending->identity(),0,held)==error::success);
        creation::work work;assert(ledger.begin(receipt.receipt.request_id,held,work)==c::error::ok);
        assert(ledger.complete(receipt.receipt.request_id,c::error::ok,c::effect::locally_accepted,9001)==c::error::ok);
        c::node_lease node;assert(f.dispatch.adoption_node(pending,node)==error::success&&node.live());
        f.native=uwvm2::utils::container::make_shared<action_native>(std::move(node));f.endpoint=uwvm2::utils::container::make_shared<actions::endpoint>();
        f.authority=pending->identity().authority;f.id=pending->resource();
        assert(f.endpoint->configure(pending->specification(),f.id,f.native)==c::error::ok);
        assert(f.dispatch.adopt_open(pending,f.endpoint)==error::success);
        opens::published published;assert(ledger.publish(f.origin.who,receipt.receipt.request_id,held,f.id,published)==c::error::ok);
        auto wrong=published;++wrong.handle;assert(f.dispatch.publish_open(pending,wrong,held)==error::denied);
        assert(f.dispatch.publish_open(pending,published,held)==error::success&&f.dispatch.publish_open(pending,published,held)==error::success);
    }
};
static void typed_adoption()
{
    async_action a;a.adopt();bytes out;
    std::size_t required{};assert(a.f.dispatch.invoke_open(a.f.origin.who,a.f.cap,a.carrier,48,required,[](w::bytes){return true;},{&a,async_action::enqueue,async_action::inspect})==error::success&&a.enqueues==1);
    auto describe=encode([&](auto& out){return out.u64(a.f.id)&&out.boolean(false);});
    assert(a.f.call(ac::operation::describe,describe,out)==error::success);
    s::events::binding source;assert(a.f.dispatch.event_source(a.f.origin.who,a.f.cap,a.f.id,source)==error::success);
    assert(a.f.dispatch.release_open(a.pending,a.receipt)==error::again);
    assert(a.f.dispatch.close(a.f.origin.who,a.f.cap,a.f.id)==error::success);
    assert(a.ledger.cleanup(a.receipt.receipt.request_id,true)==c::error::ok&&a.ledger.release(a.f.origin.who,a.receipt.receipt.request_id)==c::error::ok);
    assert(a.ledger.inspect(a.f.origin.who,a.receipt.receipt.request_id,a.receipt)==c::error::ok);
    assert(a.f.dispatch.release_open(a.pending,a.receipt)==error::success);
    assert(a.f.dispatch.release_open(a.pending,a.receipt)==error::success);
    auto wrong_release=a.receipt;++wrong_release.receipt.request_id;
    assert(a.f.dispatch.release_open(a.pending,wrong_release)==error::again);
    auto previous=a.pending;a.carrier[a.carrier.size()-16]=2;
    assert(a.f.dispatch.stage_open(a.f.origin.who,a.f.cap,a.carrier,48,a.pending)==error::success&&a.pending!=previous&&a.pending->resource()!=previous->resource());
}
static void pending_retirement()
{
    async_action a;a.f.dispatch.revoke("action");s::group_lease held;
    assert(a.f.dispatch.admit_open(a.pending->identity(),0,held)==error::cancelled);
    c::node_lease node;assert(a.f.dispatch.adoption_node(a.pending,node)==error::cancelled&&!node);
    async_action parent;assert(parent.f.origin.authority->close(parent.f.origin.who,parent.f.origin.node_cap)==error::success);
    assert(parent.f.dispatch.admit_open(parent.pending->identity(),0,held)==error::bad_handle);
    async_action cancelled;creation::view result;
    assert(cancelled.ledger.cancel(cancelled.f.origin.who,cancelled.receipt.receipt.request_id,result)==c::error::ok&&
        cancelled.ledger.release(cancelled.f.origin.who,cancelled.receipt.receipt.request_id)==c::error::ok&&
        cancelled.ledger.inspect(cancelled.f.origin.who,cancelled.receipt.receipt.request_id,result)==c::error::ok);
    assert(cancelled.f.dispatch.release_open(cancelled.pending,result)==error::success);
}
int main(){typed_adoption();pending_retirement();std::puts("PASS asynchronous action adoption: exact private drafts, full origin admission, independent creation/resource identities, verified publication, closed parent fencing and explicit cleanup");}
