/** @file @brief Graph dispatcher through owned worker queries and hostile reply validation. */
#define UWVM_COMMUNICATION_QUERY_FIXTURE_ONLY
#include "communication_queries.cpp"
#include <uwvm2/imported/ros/scene/communication_worker_graph.h>
struct worker_graph_fixture:graph_fixture
{
    std::shared_ptr<s::communication_worker::graph_source> isolated;
    query_pipes pipes{2048};
    explicit worker_graph_fixture(std::optional<std::uint64_t> calls={}):graph_fixture(false,calls)
    {
        auto factory=[](void* object,graph::plan const& policy,c::node_lease node,std::shared_ptr<graph::source>& result)noexcept
        {
            auto& self=*static_cast<worker_graph_fixture*>(object);
            try
            {
                auto value=uwvm2::utils::container::make_shared<s::communication_worker::graph_source>();
                s::communication_jobs::plan queue{policy.authority,c::operation_family::graph,policy.authority.grant,1,71,1,128,2048,10000000000,4352};
                auto validate=[](void* pointer,c::operation_token const& token,s::group_lease& held)noexcept
                {return static_cast<c::error>(static_cast<worker_graph_fixture*>(pointer)->dispatch.revalidate(token,held));};
                auto charge=[](void* pointer,c::operation_token const& token,std::uint64_t bytes,s::group_lease& held)noexcept
                {return static_cast<c::error>(static_cast<worker_graph_fixture*>(pointer)->dispatch.charge_operation(token,bytes,held));};
                auto status=value->configure(policy,std::move(node),queue,validate,charge,&self);if(status!=c::error::ok)return status;
                self.isolated=value;result=std::move(value);return c::error::ok;
            }
            catch(std::bad_alloc const&){return c::error::no_memory;}catch(...){return c::error::io;}
        };
        assert(dispatch.prepare({this,factory,true})==error::success);
    }
    bytes rows(bool reversed=false,unsigned fault=0)
    {
        return encode([&](auto& out)
        {
            auto node=[&]{return out.u64(1)&&out.text(fault==2?"/forbidden":"/scene/robot")&&out.text(fault==3?"/unexpected":"")&&out.text("");};
            auto topic=[&]{return out.u64(fault==4?64:2)&&out.text("/scene/robot")&&out.text(fault==5?"relative":"/scene/data")&&out.text("test/Request");};
            if(!out.u64(fault==1?3:2))return false;
            if(!(reversed?(topic()&&node()):(node()&&topic())))return false;
            return fault!=6||out.u64(0);
        });
    }
    void complete(std::uint64_t sequence,bytes const& body)
    {
        assert(isolated->pump(pipes.transport)==c::error::ok&&pipes.sent()==rpc::header_bytes);
        auto frame=encode([&](auto& out){return out.u64(rpc::reply_magic)&&out.u64(71)&&out.u64(sequence)&&out.u64(0)&&out.u64(0)&&out.u64(body.size())&&out.encoded(body);});
        assert(::write(pipes.outgoing[1],frame.data(),frame.size())==static_cast<ssize_t>(frame.size()));
        assert(isolated->pump(pipes.transport)==c::error::ok);
    }
    static s::handle revision(bytes const& body)
    {w::reader in{body};s::handle value{};assert(in.u64(value)&&value);return value;}
    static std::optional<s::handle> next(bytes const& body)
    {w::reader in{body};s::handle value{};bool present{};assert(in.u64(value)&&in.boolean(present));if(!present)return {};assert(in.u64(value)&&value);return value;}
};
static void real_dispatch_queue_and_retry()
{
    worker_graph_fixture f{1};bytes output,failed;
    assert(f.call(gc::operation::snapshot,output)==error::again&&f.isolated&&f.isolated->occupied()&&f.origin.admissions("graph")==1&&!f.pipes.sent());
    assert(f.call(gc::operation::snapshot,output)==error::again&&f.origin.admissions("graph")==1);
    f.complete(1,f.rows(true));auto reject=[&](w::bytes body){failed.assign(body.begin(),body.end());return false;};
    assert(f.call(gc::operation::snapshot,output,{},4096,reject)==error::io&&!f.isolated->occupied()&&f.origin.admissions("graph")==1);
    assert(f.call(gc::operation::snapshot,output)==error::success&&output==failed&&f.origin.admissions("graph")==1);
    auto cursor=worker_graph_fixture::next(output);assert(cursor);
    assert(f.call(gc::operation::snapshot,output,cursor)==error::success&&!worker_graph_fixture::next(output)&&f.origin.admissions("graph")==1);
    assert(f.call(gc::operation::snapshot,output)==error::again&&!f.isolated->occupied()&&!f.pipes.sent());
    worker_graph_fixture zero{0};assert(zero.call(gc::operation::snapshot,output)==error::again&&zero.isolated&&!zero.isolated->occupied()&&zero.origin.admissions("graph")==0&&!zero.pipes.sent());
}
static void graph_order_and_hostile_rows()
{
    worker_graph_fixture f;bytes output;
    assert(f.call(gc::operation::snapshot,output)==error::again);f.complete(1,f.rows());assert(f.call(gc::operation::snapshot,output)==error::success);
    auto revision=worker_graph_fixture::revision(output);assert(f.call(gc::operation::snapshot,output,worker_graph_fixture::next(output))==error::success);
    assert(f.call(gc::operation::snapshot,output)==error::again);f.complete(2,f.rows(true));assert(f.call(gc::operation::snapshot,output)==error::success&&worker_graph_fixture::revision(output)==revision);
    for(unsigned fault=1;fault<=6;++fault)
    {
        worker_graph_fixture invalid;bytes result;
        assert(invalid.call(gc::operation::snapshot,result)==error::again);invalid.complete(1,invalid.rows(false,fault));
        assert(invalid.call(gc::operation::snapshot,result)==error::invalid&&result.empty()&&invalid.isolated->occupied());
        assert(invalid.call(gc::operation::snapshot,result)==error::cancelled&&result.empty());
    }
}
static void graph_watch_queue_fencing()
{
    for(bool sent:{false,true})
    {
        worker_graph_fixture f;bytes output;assert(f.call(gc::operation::watch,output)==error::success);w::reader read{output};s::handle id{};assert(read.u64(id)&&read.done());
        s::events::binding binding;assert(f.dispatch.event_source(f.origin.who,f.cap,id,binding)==error::success);s::events::snapshot value;
        assert(f.dispatch.observe_source(f.origin.who,f.cap,binding,value)==c::error::again&&f.isolated->occupied());
        if(sent)assert(f.isolated->pump(f.pipes.transport)==c::error::ok&&f.pipes.sent()==rpc::header_bytes);
        assert(f.dispatch.close(f.origin.who,f.cap,id)==error::success);
        if(sent){s::communication_jobs::receipt result;assert(f.isolated->cancel(f.pipes.transport,result)==c::error::ok&&result.cleanup_debt&&result.observed==c::effect::unknown);}
        else assert(f.isolated->pump(f.pipes.transport)==c::error::ok&&!f.pipes.sent());
        assert(f.dispatch.observe_source(f.origin.who,f.cap,binding,value)==c::error::bad_handle);
    }
}
int main()
{
    real_dispatch_queue_and_retry();graph_order_and_hostile_rows();graph_watch_queue_fencing();
    std::puts("PASS isolated graph source: actual dispatcher, reserved query charge, exhausted-budget pagination, stable ordering, hostile rows and watch incarnation fencing");
}
