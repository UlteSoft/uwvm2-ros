/** @file @brief Actual registry-node evidence, independent creation identity and fenced reuse.
 * @details The in-process backend models a verified incarnation. This fixture
 * proves host state/admission invariants, not native worker startup or VM wiring.
 */
#include <uwvm2/imported/ros/scene/communication_creation.h>
#include <cassert>
#include <cstdio>
#include <thread>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;
namespace c=s::communication;
namespace creation=s::communication_creation;
namespace opens=uwvm2::imported::ros::communication_open_contract;
namespace w=s::wire;
static c::profile profile(std::string_view name)
{c::profile result;result.identity=name;result.digest[0]=1;return result;}
struct native_context final:c::native_context
{bool active{true};bool live()const noexcept override{return active;}c::error shutdown()noexcept override{active=false;return c::error::ok;}};
struct native_node final:c::native_node
{c::handle generation{};explicit native_node(c::handle selected)noexcept:generation(selected){}c::handle provider_generation()const noexcept override{return generation;}};
struct backend final:c::backend
{
    c::handle generation{71};std::weak_ptr<c::native_node> last;
    unsigned stage_calls{},node_creations{};bool ready{true};
    void* stage_context{};
    c::error(*stage_hook)(void*,c::node_plan const&,c::node_stage_request const&)noexcept{};
    c::error(*create_hook)(void*,c::node_plan const&)noexcept{};
    c::error create_context(c::context_plan const&,std::shared_ptr<c::native_context>& out)noexcept override
    {try{out=uwvm2::utils::container::make_shared<native_context>();return c::error::ok;}catch(...){return c::error::no_memory;}}
    c::error create_node(c::node_plan const& plan,std::shared_ptr<c::native_context> const&,std::shared_ptr<c::native_node>& out)noexcept override
    {if(create_hook){auto status=create_hook(stage_context,plan);if(status!=c::error::ok)return status;}
        try{out=uwvm2::utils::container::make_shared<native_node>(generation);last=out;++node_creations;return c::error::ok;}catch(...){return c::error::no_memory;}}
    c::error prepare_node(c::node_plan const& plan,std::shared_ptr<c::native_context> const&,c::node_stage_request const& request)noexcept override
    {++stage_calls;if(stage_hook)return stage_hook(stage_context,plan,request);return ready?c::error::ok:c::error::again;}
    c::error create_topic(c::topic_plan const&,c::type_plan const&,std::shared_ptr<c::native_node> const&,c::handle,
                         std::shared_ptr<c::receive_queue> const&,std::shared_ptr<c::native_topic>&)noexcept override{return c::error::unsupported;}
};
struct fixture
{
    std::shared_ptr<backend> native=uwvm2::utils::container::make_shared<backend>();
    c::registry registry{native,{1,1,0,0,0,64}};
    c::owner context_owner{17,1,3},node_owner{17,2,3};
    c::profile context_profile{profile("context")},node_profile{profile("node")};
    s::authority authority;int instance{};s::caller who{&instance,1};s::handle context_cap{},node_cap{};
    c::handle context{},node{};c::node_attachment evidence;
    creation::ledger ledger;creation::identity original;creation::view receipt;
    std::array<std::uint8_t,24> carrier{};
    explicit fixture(c::handle generation=71)
    {
        native->generation=generation;
        assert(registry.configure(c::context_plan{context_owner,context_profile,0,{},c::transport::unauthenticated_loopback})==c::error::ok);
        assert(registry.configure(c::node_plan{node_owner,node_profile,context_profile,"robot","/scene",1})==c::error::ok);
        assert(registry.open_context(context_owner,context_profile,context)==c::error::ok);
        assert(registry.open_node(node_owner,context_owner,context,node_profile,node)==c::error::ok);
        assert(registry.node_evidence(node_owner,node,evidence)==(generation?c::error::ok:c::error::unsupported));
        for(auto name:{"capability.open","context.open","node.open"})assert(authority.expose(name,s::exposure::allow)==s::error::success);
        for(bool is_node:{false,true})
        {
            s::grant grant;grant.alias=is_node?"node":"context";grant.resource=grant.alias;grant.kind=is_node?s::purpose::node:s::purpose::context;grant.binding="native-communication";
            grant.actions.emplace(is_node?"node.open":"context.open");assert(authority.configure(std::move(grant))==s::error::success);
        }
        assert(authority.enroll(who)==s::error::success&&authority.open(who,"node",node_cap)==s::error::success&&authority.open(who,"context",context_cap)==s::error::success);
        creation::plan plan{node_owner,node_profile,opens::endpoint_kind::node,1,71,64,10000000000};assert(ledger.configure({&plan,1},128)==c::error::ok);
        original.who=who;original.authority=node_owner;original.key=1;original.deadline=c::now()+5000000000;original.count=2;
        original.origins[0]={creation::dependency::operation,node_owner,node_cap,0};original.origins[1]={creation::dependency::context,context_owner,context_cap,context};
        w::writer out{carrier};assert(out.u64(42)&&out.u64(1)&&out.u64(original.deadline));
    }
    s::group_lease admitted(std::uint64_t bytes=0,bool charge=false)
    {s::group_lease held;creation::claim_set claims;assert(authority.admit_group(who,creation::admission_claims(original,opens::endpoint_kind::node,claims,bytes),held,charge)==s::error::success);return held;}
    void begin()
    {
        creation::view reserved;assert(ledger.reserve(original,carrier,reserved)==c::error::ok);
        auto held=admitted(carrier.size()+48,true);assert(ledger.commit(who,reserved.receipt.request_id,held,receipt)==c::error::ok);
        creation::work work;assert(ledger.begin(receipt.receipt.request_id,held,work)==c::error::ok);
    }
};
static void typed_publication()
{
    fixture f,foreign;f.begin();auto request=f.receipt.receipt.request_id;assert(request!=f.node&&f.evidence.live());
    assert(f.evidence.authority()==f.node_owner&&f.evidence.context_authority()==f.context_owner&&f.evidence.provider_generation()==71&&f.evidence.provider_generation()!=f.node_owner.generation);
    assert(f.ledger.complete(request,c::error::ok,c::effect::locally_accepted,9001)==c::error::unsupported);
    assert(f.ledger.complete_node(request,{})==c::error::denied&&f.ledger.complete_node(request,foreign.evidence)==c::error::denied);
    assert(f.ledger.complete_node(request,f.evidence)==c::error::ok);
    creation::work work;c::handle native{};assert(f.ledger.provider_result(request,work,native)==c::error::busy&&!native);
    auto held=f.admitted();opens::published published;
    assert(f.ledger.publish(f.who,request,held,request,published)==c::error::denied);
    assert(f.ledger.publish(f.who,request,held,f.node,published)==c::error::ok&&published.kind==opens::endpoint_kind::node);
    assert(f.ledger.publish(f.who,request,held,f.node,published)==c::error::ok);
    assert(f.ledger.cleanup(request,true)==c::error::busy&&f.ledger.release(f.who,request)==c::error::busy);
    held.release();assert(f.registry.close(f.node_owner,f.node)==c::error::ok&&!f.evidence.live());
    assert(f.ledger.cleanup(request,false)==c::error::busy&&f.ledger.release(f.who,request)==c::error::busy);
    assert(f.ledger.cleanup(request,true)==c::error::ok&&f.ledger.release(f.who,request)==c::error::ok);
}
static void incarnation_and_reuse()
{
    fixture absent(0);assert(!absent.evidence.live());
    fixture wrong(72);wrong.begin();assert(wrong.ledger.complete_node(wrong.receipt.receipt.request_id,wrong.evidence)==c::error::denied);
    fixture f;auto old=f.evidence;auto retained=f.native->last.lock();assert(retained);
    assert(f.registry.close(f.node_owner,f.node)==c::error::ok&&!old.live());auto previous=f.node;
    assert(f.registry.open_node(f.node_owner,f.context_owner,f.context,f.node_profile,f.node)==c::error::ok&&f.node!=previous);
    assert(!old.live()&&retained&&f.registry.node_evidence(f.node_owner,f.node,f.evidence)==c::error::ok&&f.evidence.live());
    assert(f.registry.node_evidence(f.context_owner,f.node,f.evidence)==c::error::denied&&f.evidence.live());
    f.registry.revoke(f.context_owner);assert(!f.evidence.live()&&!old.live());
}
static void private_close_and_fork()
{
    fixture f;f.begin();assert(f.ledger.complete_node(f.receipt.receipt.request_id,f.evidence)==c::error::ok);
    assert(f.registry.close(f.node_owner,f.node)==c::error::ok);auto held=f.admitted();opens::published published;
    assert(f.ledger.publish(f.who,f.receipt.receipt.request_id,held,f.node,published)==c::error::denied);held.release();
    fixture live;auto child=::fork();assert(child>=0);
    if(!child)_exit(!live.evidence.live()?0:1);
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status)&&live.evidence.live());
    std::atomic<bool> done{};std::thread observer{[&]{while(!done.load())(void)live.evidence.live();}};
    assert(live.registry.close(live.node_owner,live.node)==c::error::ok);done=true;observer.join();assert(!live.evidence.live());
}
int main(){typed_publication();incarnation_and_reuse();private_close_and_fork();
    std::puts("PASS typed node creation evidence: exact actual owner/context/incarnation, distinct request/resource, no fabricated native token, weak ownership, fenced reuse, cleanup debt, fork and concurrency");return 0;}
