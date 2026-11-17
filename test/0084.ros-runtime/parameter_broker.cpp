/** @file
 * @brief Exercise actual authority admission around parameter/node registries.
 * @details The native fixture provides only deterministic local node lifetimes;
 * these tests do not claim native ROS parameter-service interoperability.
 */
#define main parameter_store_fixture_entry
#include "parameter_store.cpp"
#undef main
#include <uwvm2/imported/ros/scene/parameter_broker.h>

namespace s=uwvm2::imported::ros::scene;
namespace containers=uwvm2::utils::container;
static thread_local std::size_t fault_after{},allocation_count{};
static thread_local bool allocation_failed{};
static bool deny_allocation() noexcept
{++allocation_count;if(fault_after&&!--fault_after){allocation_failed=true;return true;}return false;}
extern "C" void* __real_malloc(std::size_t);
extern "C" void* __real_calloc(std::size_t,std::size_t);
extern "C" void* __real_realloc(void*,std::size_t);
extern "C" void* __wrap_malloc(std::size_t n){return deny_allocation()?nullptr:__real_malloc(n);}
extern "C" void* __wrap_calloc(std::size_t n,std::size_t s){return deny_allocation()?nullptr:__real_calloc(n,s);}
extern "C" void* __wrap_realloc(void* p,std::size_t n){return deny_allocation()?nullptr:__real_realloc(p,n);}

struct trusted_nodes
{
    fixture& source;std::shared_ptr<s::authority> permissions;s::handle node_cap{},context_cap{};
    E origins(s::caller who,s::handle cap,s::handle node,c::profile const& profile,c::owner expected)
    {
        if(cap!=node_cap||node!=source.node||profile!=source.node_profile||expected!=source.node_owner)return E::denied;
        std::array claims{s::admission_claim{node_cap,"node.open",0,false,true},s::admission_claim{context_cap,"context.open",0,false,true}};
        s::group_lease admitted;auto status=permissions->admit_group(who,claims,admitted,false);if(status!=E::success)return status;
        if(admitted.specification(0)->kind!=s::purpose::node||admitted.specification(1)->kind!=s::purpose::context)return E::denied;
        c::node_info observed;auto result=source.native.describe(expected,node,observed);
        return s::parameter_dispatch::node_access::convert(result);
    }
    E attach_node(s::caller who,s::handle cap,s::handle node,c::profile const& profile,c::node_lease& out)
    {
        auto status=origins(who,cap,node,profile,source.node_owner);if(status!=E::success)return status;
        return s::parameter_dispatch::node_access::convert(source.native.attach_node(source.node_owner,node,profile,out));
    }
    E validate_node(s::caller who,s::handle cap,s::handle node,c::profile const& profile,c::owner expected)
    {return origins(who,cap,node,profile,expected);}
};
struct broker_fixture
{
    fixture native;
    std::shared_ptr<s::authority> permissions=containers::make_shared<s::authority>();
    trusted_nodes nodes{native,permissions};
    s::parameter_dispatch dispatch{permissions,s::parameter_dispatch::node_access::from(nodes),clock_now};
    int instance{};s::caller who{&instance,1};s::handle cap{},other{};
    explicit broker_fixture(std::optional<std::uint64_t> calls={},bool node_exposed=true,std::uint64_t scopes=4,bool deferred=false,
                            p::plan const* policy=nullptr,std::string_view observation={})
    {
        for(auto name:{"capability.open","capability.close","context.open","resource.close","request.describe","request.release"})assert(permissions->expose(name,s::exposure::allow)==E::success);
        if(node_exposed)assert(permissions->expose("node.open",s::exposure::allow)==E::success);
        for(auto const& action:pc::operations)assert(permissions->expose(containers::std_string{action.name},s::exposure::allow)==E::success);
        s::grant context;context.alias=context.resource="context";context.binding="test";context.kind=s::purpose::context;context.actions={"context.open"};
        context.limits.calls=1;context.limits.uncertain=true;context.limits.window=std::chrono::hours{1};
        s::grant node=context;node.alias=node.resource="node";node.kind=s::purpose::node;node.actions={"node.open"};
        s::grant parameter;parameter.alias=parameter.resource="params";parameter.binding="local-parameters";parameter.kind=s::purpose::parameter;
        parameter.actions={"resource.close","request.describe","request.release"};
        for(auto const& action:pc::operations)
            if(observation.empty()||action.id==pc::operation::open||action.id==pc::operation::declare||action.name==observation)parameter.actions.emplace(action.name);
        parameter.max_resources=scopes;parameter.limits.calls=calls;parameter.limits.uncertain=calls.has_value();parameter.limits.window=std::chrono::hours{1};
        assert(permissions->configure(context)==E::success&&permissions->configure(node)==E::success&&permissions->configure(parameter)==E::success);
        parameter.alias=parameter.resource="other";assert(permissions->configure(parameter)==E::success);
        assert(permissions->enroll(who)==E::success&&permissions->open(who,"context",nodes.context_cap)==E::success&&
            permissions->open(who,"node",nodes.node_cap)==E::success&&permissions->open(who,"params",cap)==E::success&&permissions->open(who,"other",other)==E::success);
        s::lease admitted;assert(permissions->admit(who,nodes.context_cap,"context.open",8,admitted)==E::success);admitted.release();
        if(node_exposed){assert(permissions->admit(who,nodes.node_cap,"node.open",8,admitted)==E::success);admitted.release();}
        assert(dispatch.bind("params",policy?*policy:plan(deferred))==E::success&&dispatch.bind("other",policy?*policy:plan(deferred))==E::success);
    }
    record opening() const
    {
        record out;assert(out.output.u64(nodes.node_cap)&&out.output.u64(native.node)&&out.output.text("parameters")&&out.output.blob(plan().selected.sha256));return out;
    }
    result call(pc::operation operation,record const& input,std::uint64_t capacity=16384,s::handle selected=0,s::parameter_dispatch::sink custom={})
    {
        result value;auto copy=[&](wire::bytes bytes){value.bytes.assign(bytes.begin(),bytes.end());return true;};
        value.status=dispatch.invoke(who,operation,selected?selected:cap,input.bytes(),capacity,value.required,custom?custom:s::parameter_dispatch::sink{copy});return value;
    }
    std::uint64_t open()
    {auto request=opening();auto value=call(pc::operation::open,request);assert(value.status==E::success);wire::reader input{value.bytes};return number(input);}
    std::uint64_t admitted(std::string_view alias)
    {containers::std_vector<s::authority_snapshot> rows;assert(permissions->inspect(rows,4));for(auto const& row:rows)if(row.alias==alias)return row.admissions;assert(false);return 0;}
    p::usage usage(){p::usage value;assert(dispatch.try_inspect("params",value));return value;}
    result declare(std::uint64_t scope,std::uint64_t sequence,std::int64_t value=1)
    {record input;key(input.output,scope,sequence);assert(input.output.text("a")&&input.output.text("integer")&&input.output.text("")&&input.output.text("")&&input.output.boolean(false)&&input.output.boolean(false)&&input.output.boolean(false));integer(input.output,value);return call(pc::operation::declare,input);}
    result get(std::uint64_t scope)
    {record input;assert(input.output.u64(scope)&&input.output.u64(1)&&input.output.text("a"));return call(pc::operation::get,input);}
    std::int64_t value(std::uint64_t scope)
    {auto got=get(scope);assert(got.status==E::success);wire::reader in{got.bytes};assert(number(in)==1&&text(in)=="a"&&number(in)==2);std::int64_t out{};assert(in.i64(out));return out;}
    result request(p::request_operation action,std::uint64_t id,std::uint64_t capacity=4096,s::parameter_dispatch::sink custom={})
    {
        record input;assert(input.output.u64(id));result value;auto copy=[&](wire::bytes bytes){value.bytes.assign(bytes.begin(),bytes.end());return true;};
        value.status=dispatch.invoke_request(who,action,cap,input.bytes(),capacity,value.required,custom?custom:s::parameter_dispatch::sink{copy});return value;
    }
};
static void complete_admission_and_parent_fencing()
{
    broker_fixture f;auto scope=f.open();assert(f.declare(scope,1).status==E::success&&f.value(scope)==1);
    assert(f.admitted("node")==1&&f.admitted("context")==1&&f.admitted("params")==3);
    record read;assert(read.output.u64(scope)&&read.output.u64(1)&&read.output.text("a"));
    assert(f.call(pc::operation::get,read,16384,f.other).status==E::bad_handle);
    auto before=f.admitted("params");
    assert(f.permissions->revoke("context",[](auto){})==E::success);
    assert(f.get(scope).status==E::denied&&f.admitted("params")==before);
    assert(f.dispatch.close(f.who,f.cap,scope)==E::success&&f.usage().scopes==0&&f.usage().retained_scopes==0);
    broker_fixture hidden{{},false};auto input=hidden.opening();assert(hidden.call(pc::operation::open,input).status==E::denied&&!hidden.usage().scopes);
    broker_fixture limited{{},true,1};limited.open();auto extra=limited.opening();assert(limited.call(pc::operation::open,extra).status==E::again&&limited.usage().retained_scopes==1);
}
static void late_charge_and_zero_authority()
{
    broker_fixture zero{0};auto input=zero.opening();assert(zero.call(pc::operation::open,input).status==E::again&&zero.admitted("params")==0&&!zero.usage().scopes);
    broker_fixture f;auto scope=f.open();f.declare(scope,1);record set;set_record(set,scope,2,{{"a",2}});
    auto before=f.admitted("params");assert(f.call(pc::operation::set,set,0).status==E::too_large&&f.admitted("params")==before&&f.value(scope)==1);
    before=f.admitted("params");auto lost=[](wire::bytes){return false;};
    assert(f.call(pc::operation::set,set,16384,0,lost).status==E::io&&f.admitted("params")==before+1&&f.value(scope)==2);
    assert(receipt{f.call(pc::operation::set,set)}.state=="succeeded"&&f.usage().workspace==0);
    p::workspace_lease reserve;assert(f.dispatch.reserve_workspace(f.who,pc::operation::set,f.cap,128,reserve)==E::success&&f.usage().workspace==128);reserve.reset();
    assert(f.usage().workspace==0);
    int stranger{};std::uint64_t required{};assert(f.dispatch.invoke({&stranger,1},pc::operation::set,f.cap,set.bytes(),4096,required,lost)==E::denied);
}
static void callback_revocation_and_close_lifetime()
{
    broker_fixture f;auto scope=f.open();f.declare(scope,1);record take;assert(take.output.u64(scope));
    auto revoke=[&](wire::bytes){assert(f.permissions->revoke("node",[](auto){})==E::success);return true;};
    assert(f.call(pc::operation::take,take,16384,0,revoke).status==E::io&&f.usage().events==1);
    assert(f.dispatch.close(f.who,f.cap,scope)==E::success&&f.usage().events==0&&f.usage().retained_scopes==0);
    broker_fixture closing{{},true,1};auto open=closing.opening();
    auto fail=[&](wire::bytes){return false;};assert(closing.call(pc::operation::open,open,16384,0,fail).status==E::io&&!closing.usage().retained_scopes);
    auto owned=closing.open();closing.declare(owned,1);record next;assert(next.output.u64(owned));
    auto close=[&](wire::bytes)
    {
        assert(closing.dispatch.close(closing.who,closing.cap,owned)==E::success);
        assert(closing.usage().scopes==0&&closing.usage().retained_scopes==1);
        assert(closing.call(pc::operation::open,open).status==E::again);return true;
    };
    assert(closing.call(pc::operation::take,next,16384,0,close).status==E::cancelled&&!closing.usage().retained_scopes);
    assert(closing.open());
}
static void allocation_failure_transactions()
{
    // Each trial owns fresh state: every real allocator failure before admission
    // must leave values, key and finite reservations unchanged. The fixture's
    // returned vector is outside the VM allocation fault domain.
    unsigned failures{};
    for(std::size_t point=1;point<=48;++point)
    {
        broker_fixture f;auto scope=f.open();f.declare(scope,1);record set;set_record(set,scope,2,{{"a",2}});
        auto before=f.usage();auto admissions=f.admitted("params");fault_after=point;allocation_failed=false;
        auto changed=f.call(pc::operation::set,set);fault_after=0;
        if(!allocation_failed){assert(changed.status==E::success);break;}
        ++failures;assert(changed.status==E::no_memory&&f.admitted("params")==admissions&&f.value(scope)==1);
        auto after=f.usage();assert(after.bytes==before.bytes&&after.events==before.events&&after.event_bytes==before.event_bytes&&after.workspace==0&&after.pending_bytes==0&&after.receipt_bytes==before.receipt_bytes);
        assert(f.call(pc::operation::set,set).status==E::success&&f.value(scope)==2);
    }
    assert(failures>=5);
    for(std::size_t point=1;point<=24;++point)
    {
        broker_fixture f;auto input=f.opening();fault_after=point;allocation_failed=false;auto opened=f.call(pc::operation::open,input);fault_after=0;
        if(!allocation_failed){assert(opened.status==E::success);break;}
        assert(opened.status==E::no_memory&&!f.usage().scopes&&!f.usage().retained_scopes);
        assert(f.open());
    }
}
static void fork_and_workspace_bounds()
{
    broker_fixture f;auto scope=f.open();auto input=f.opening();p::workspace_lease lease;
    assert(f.dispatch.reserve_workspace(f.who,pc::operation::open,f.cap,4u*1024u*1024u+1u,lease)==E::too_large&&!lease);
    auto child=::fork();assert(child>=0);
    if(!child)
    {
        auto sink=[](wire::bytes){return true;};std::uint64_t required{};p::usage use;
        assert(f.dispatch.invoke(f.who,pc::operation::open,f.cap,input.bytes(),8,required,sink)==E::denied);
        assert(f.dispatch.preflight(f.who,pc::operation::open,f.cap,8)==E::denied&&f.dispatch.close(f.who,f.cap,scope)==E::denied);
        assert(f.dispatch.reserve_workspace(f.who,pc::operation::open,f.cap,8,lease)==E::denied&&!f.dispatch.try_inspect("params",use));
        f.dispatch.retire();std::destroy_at(&f.dispatch);::_exit(0);
    }
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&WEXITSTATUS(status)==0&&f.usage().scopes==1);
}
static void terminal_request_retirement()
{
    broker_fixture f;auto scope=f.open();auto first=receipt{f.declare(scope,1)};
    assert(receipt{f.request(p::request_operation::describe,first.request)}.state=="succeeded");
    auto ledger=f.usage().receipt_bytes;auto lost=[](wire::bytes){return false;};
    assert(f.request(p::request_operation::release,first.request,8,lost).status==E::io);
    assert(f.request(p::request_operation::describe,first.request).status==E::bad_handle&&f.usage().receipt_bytes==ledger);
    auto repeated=f.request(p::request_operation::release,first.request);wire::reader boolean_reply{repeated.bytes};
    assert(repeated.status==E::success&&!boolean(boolean_reply)&&boolean_reply.done());
    assert(receipt{f.declare(scope,1)}.state=="succeeded"&&f.usage().receipt_bytes==ledger); // detached key remains exact
    broker_fixture revoked;auto owned=revoked.open();auto receipt_id=receipt{revoked.declare(owned,1)}.request;
    assert(revoked.permissions->revoke("context",[](auto){})==E::success&&revoked.get(owned).status==E::denied);
    assert(receipt{revoked.request(p::request_operation::describe,receipt_id)}.state=="succeeded");
    assert(revoked.request(p::request_operation::release,receipt_id).status==E::success);
    broker_fixture pending{{},true,4,true};auto waiting=pending.open();pending.declare(waiting,1);record set;set_record(set,waiting,2,{{"a",2}});
    auto proposal=receipt{pending.call(pc::operation::set,set)};
    assert(pending.request(p::request_operation::release,proposal.request).status==E::again);
    auto before=pending.usage();current_time=1200;auto charges=pending.admitted("params");
    assert(pending.request(p::request_operation::release,proposal.request,0).status==E::too_large&&pending.admitted("params")==charges&&pending.usage().pending_bytes==before.pending_bytes);
    assert(receipt{pending.request(p::request_operation::describe,proposal.request)}.reason=="validation_timeout"&&pending.usage().pending_bytes==0);
    assert(pending.request(p::request_operation::release,proposal.request).status==E::success&&pending.value(waiting)==1);
}
static void deferred_and_cursor_allocation_recovery()
{
    unsigned staging_failures{},response_failures{},cursor_failures{};
    for(bool respond_phase:{false,true})for(std::size_t point=1;point<=64;++point)
    {
        broker_fixture f{{},true,4,true};auto scope=f.open();f.declare(scope,1);record take;assert(take.output.u64(scope));
        assert(f.call(pc::operation::take,take).status==E::success);
        record set;set_record(set,scope,2,{{"a",2}});record response;
        if(respond_phase)
        {
            auto waiting=receipt{f.call(pc::operation::set,set)};assert(waiting.state=="queued");
            assert(f.call(pc::operation::take,take).status==E::success);respond_record(response,scope,waiting.request,true,{{"a",3}});
        }
        auto before=f.usage();auto admissions=f.admitted("params");fault_after=point;allocation_failed=false;
        auto result=f.call(respond_phase?pc::operation::respond:pc::operation::set,respond_phase?response:set);fault_after=0;
        if(!allocation_failed){assert(result.status==E::success);break;}
        if(respond_phase)++response_failures;else ++staging_failures;
        assert(result.status==E::no_memory&&f.admitted("params")==admissions&&f.value(scope)==1);
        auto after=f.usage();assert(after.events==before.events&&after.event_bytes==before.event_bytes&&after.pending_bytes==before.pending_bytes&&
            after.receipt_bytes==before.receipt_bytes&&after.workspace==0);
        auto recovered=receipt{f.call(respond_phase?pc::operation::respond:pc::operation::set,respond_phase?response:set)};
        assert(recovered.state==(respond_phase?"succeeded":"queued"));
    }
    for(std::size_t point=1;point<=32;++point)
    {
        broker_fixture f;auto scope=f.open();unsigned sequence{};
        for(auto name:{"axis.a","axis.b"})
        {
            record input;key(input.output,scope,++sequence);assert(input.output.text(name)&&input.output.text("integer")&&input.output.text("")&&input.output.text("")&&input.output.boolean(false)&&input.output.boolean(false)&&input.output.boolean(false));integer(input.output,1);
            assert(f.call(pc::operation::declare,input).status==E::success);
        }
        record list;list_record(list,scope,1);auto before=f.usage();auto admissions=f.admitted("params");fault_after=point;allocation_failed=false;
        auto result=f.call(pc::operation::list,list);fault_after=0;
        if(!allocation_failed){assert(result.status==E::success);break;}
        ++cursor_failures;auto after=f.usage();assert(result.status==E::no_memory&&f.admitted("params")==admissions&&after.cursors==before.cursors&&
            after.cursor_bytes==before.cursor_bytes&&after.workspace==0);
        assert(f.call(pc::operation::list,list).status==E::success);
    }
    assert(staging_failures>=10&&response_failures>=4&&cursor_failures>=3);
}
static void independent_bounded_observation()
{
    for(bool search:{false,true})
    {
        auto selected=plan();
        if(search)
        {
            selected.values.selected=pv::algebra::ros1;selected.keys.clear();selected.search_root.assign("/plant");selected.max_search_depth=2;
            for(auto name:{"/plant/a/x","/plant/x"}){p::key_policy entry;entry.name.assign(name);entry.types=1ull<<2;selected.keys.push_back(entry);}
        }
        auto action=search?pc::operation::search:pc::operation::exists;
        broker_fixture f{{},true,4,false,&selected,search?"parameter.search":"parameter.exists"};auto scope=f.open();
        record query;assert(query.output.u64(scope)&&query.output.text(search?"x":"a"));if(search)assert(query.output.text("/plant/a"));
        assert(f.call(action,query).status==E::success&&f.get(scope).status==E::denied);
        auto denied=search?pc::operation::exists:pc::operation::search;
        assert(f.call(denied,query).status==E::denied);
        auto admissions=f.admitted("params");auto required=search?16u:24u;
        assert(f.call(action,query,required-1).status==E::too_large&&f.admitted("params")==admissions);
        unsigned deliveries{};auto copy=[&](wire::bytes output){++deliveries;wire::reader fields{output};assert(!boolean(fields));if(!search)assert(!boolean(fields));number(fields);assert(fields.done());return true;};
        fault_after=1;allocation_failed=false;auto failed=f.call(action,query,16384,0,copy);fault_after=0;
        assert(allocation_failed&&failed.status==E::no_memory&&!deliveries&&f.admitted("params")==admissions&&f.usage().workspace==0);
        assert(f.call(action,query,16384,0,copy).status==E::success&&deliveries==1&&f.usage().workspace==0);
        assert(f.admitted("node")==1&&f.admitted("context")==1);
        assert(f.call(action,query,16384,f.other).status==E::bad_handle);
        auto revoke=[&](wire::bytes){assert(f.permissions->revoke("context",[](auto){})==E::success);return true;};
        assert(f.call(action,query,16384,0,revoke).status==E::io&&f.usage().workspace==0);
        admissions=f.admitted("params");assert(f.call(action,query).status==E::denied&&f.admitted("params")==admissions);
        assert(f.dispatch.close(f.who,f.cap,scope)==E::success);
    }
}
int run_parameter_broker_tests()
{
    assert(parameter_store_fixture_entry()==0);
    complete_admission_and_parent_fencing();late_charge_and_zero_authority();callback_revocation_and_close_lifetime();
    allocation_failure_transactions();fork_and_workspace_bounds();terminal_request_retirement();deferred_and_cursor_allocation_recovery();
    independent_bounded_observation();
    std::puts("passed 8 parameter broker security groups");
    return 0;
}
#ifndef UWVM_PARAMETER_BROKER_EMBEDDED
int main(){return run_parameter_broker_tests();}
#endif
