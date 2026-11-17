/** @file @brief Actual sealed sensor/model/lifecycle admission, with no simulated target. */
#define main prior_model_fixture_main
#include "model_dispatch.cpp"
#undef main
#include <uwvm2/imported/ros/scene/sensor_source.h>
#include <uwvm2/imported/ros/scene/proposal_lifecycle_target.h>
namespace p=s::proposals;namespace pc=uwvm2::imported::ros::proposal_contract;namespace ml=s::managed_lifecycle;
static std::uint64_t observed_now{};
static void(*clock_callback)()noexcept{};
static std::uint64_t target_clock()noexcept{if(clock_callback)clock_callback();return observed_now;}
static s::grant grant(std::string_view alias,s::purpose kind,std::string_view binding,std::initializer_list<char const*> actions)
{s::grant value;value.alias=alias;value.resource=alias;value.kind=kind;value.binding=binding;value.max_handles=value.max_resources=4;for(auto action:actions)value.actions.emplace(action);return value;}
struct actual_fixture
{
    std::shared_ptr<s::authority> permissions=uwvm2::utils::container::make_shared<s::authority>();
    s::buffers::registry buffers;s::model_dispatch model{permissions,buffers};ml::registry lifecycle;
    s::sensors::source sensor{permissions,buffers,target_clock};
    s::proposal_lifecycle_target target{permissions,lifecycle,71,81,target_clock};
    s::proposal_dispatch proposals{permissions,model,buffers,71,81,{1,1},target_clock};
    int instance{};s::caller who{&instance,1};
    s::handle model_cap{},buffer_cap{},sensor_cap{},target_cap{},read_cap{},accept_cap{},sensor_handle{},client{},server{},session{},input{};
    ml::plan bounds{4,UINT64_C(60000000000),254,3};p::plan policy;
    explicit actual_fixture(bool guest_publication=false, bool limited_budget=false,unsigned operation_mask=7,bool sensor_export=false)
    {
        observed_now=s::models::now();clock_callback=nullptr;
        for(auto name:{"capability.open","capability.close","buffer.read","sensor.open","sensor.take","sensor.describe","resource.close","lifecycle.submit","proposal.read","proposal.accept"})
            assert(permissions->expose(name,s::exposure::allow)==error::success);
        if(guest_publication)for(auto name:{"proposal.capture","proposal.publish"})assert(permissions->expose(name,s::exposure::allow)==error::success);
        for(auto const& operation:c::operations)assert(permissions->expose(uwvm2::utils::container::std_string(operation.name),s::exposure::allow)==error::success);
        assert(permissions->configure(model_grant())==error::success&&permissions->configure(data_grant())==error::success);
        auto sensor_grant=grant("sensor",s::purpose::sensor,"bounded-sensor",{"sensor.open","sensor.take","sensor.describe","resource.observe","resource.close"});
        if(sensor_export)sensor_grant.actions.emplace("data.export");
        assert(permissions->configure(std::move(sensor_grant))==error::success);
        assert(permissions->configure(grant("node",s::purpose::lifecycle,"local-lifecycle",{"lifecycle.submit"}))==error::success);
        auto suggestion_grant=grant("suggestions",s::purpose::proposal,"model-proposals",{"proposal.read","resource.close"});
        if(!(operation_mask&1))suggestion_grant.actions.erase("proposal.read");
        if(guest_publication&&bool(operation_mask&2))suggestion_grant.actions.emplace("proposal.capture");
        if(guest_publication&&bool(operation_mask&4))suggestion_grant.actions.emplace("proposal.publish");
        if(limited_budget){suggestion_grant.limits.uncertain=true;suggestion_grant.limits.bytes=0;}
        assert(permissions->configure(std::move(suggestion_grant))==error::success);
        assert(permissions->configure(grant("judge",s::purpose::arbiter,"proposal-arbiter",{"proposal.accept","request.describe","request.release"}))==error::success);
        assert(permissions->enroll(who)==error::success);
        for(auto item:{std::pair{"brain",&model_cap},{"data",&buffer_cap},{"sensor",&sensor_cap},{"node",&target_cap},{"suggestions",&read_cap},{"judge",&accept_cap}})
            assert(permissions->open(who,item.first,*item.second)==error::success);
        auto bp=buffer_plan();bp.objects=8;bp.bytes=32;bp.leases=32;assert(buffers.bind("data",bp)==error::success);
        auto mp=model_plan();mp.output_bytes=8192;mp.output_items=2;mp.workspace_bytes=32768;assert(mp.output_type.assign(p::model_output_type));
        mp.validate_output=[](auto bytes,bool terminal)noexcept{p::envelope decoded;return terminal&&p::decode_envelope(bytes,decoded);};
        assert(model.bind("brain",mp)==error::success&&model.attach("brain",8)==error::success);
        assert(lifecycle.bind("node",bounds)==error::success&&lifecycle.open("node",ml::client,4,client)==error::success&&lifecycle.open("node",ml::server,4,server)==error::success);
        s::proposal_lifecycle_target::configuration target_config;assert(target_config.alias.assign("node"));target_config.profile=profile("local.node");target_config.bounds=bounds;
        target_config.profile.sha256=s::proposal_lifecycle_target::profile_digest("local.node","node",bounds);assert(target.bind(target_config)==error::success);
        s::sensors::source::configuration sensor_config;auto& ingress=sensor_config.ingress;ingress.profile=profile("sensor.actual");ingress.schema=profile("bounded.sample");
        assert(ingress.type.assign("bounded.sample")&&ingress.frame.assign("logical")&&ingress.units.assign("count")&&ingress.encoding.assign("bytes"));
        ingress.handles=2;ingress.records=2;ingress.body_bytes=4;ingress.source_sequence=true;ingress.validate_body=[](auto bytes)noexcept{return bytes.size()==4;};
        assert(s::sensors::core::required_storage(ingress,ingress.storage_bytes));sensor_config.history=2;sensor_config.buffer_objects=8;sensor_config.buffer_profile=profile("tensor");
        assert(sensor_config.alias.assign("sensor")&&sensor_config.buffer_alias.assign("data"));
        assert(sensor.configure({71,91,81},sensor_config,{101,1,102,1,0,0})==error::success);
        assert(sensor.open(who,sensor_cap,ingress.profile,[&](auto id){sensor_handle=id;return true;})==error::success);
        constexpr std::array<std::uint8_t,4> body{1,2,3,4};assert(sensor.ingest({101,1,1,0,0,0,false},body)==error::success);
        assert(sensor.take(who,sensor_cap,sensor_handle,[&](auto const& value){input=value.buffer;return true;})==error::success);
        policy.selected=profile("proposal.policy");policy.model=profile("local.identity");policy.task=profile("tensor.increment");assert(policy.model_alias.assign("brain"));
        policy.storage={4,4096,1,4,1,1,UINT64_C(60000000000),UINT64_C(60000000000)};policy.captures=4;policy.workspace_bytes=65536;policy.workspace_calls=16;
        policy.source_count=policy.tool_count=1;auto& source=policy.sources[0];assert(source.alias.assign("sensor")&&source.buffer_alias.assign("data")&&source.frame.assign("logical"));
        source.profile=ingress.profile;source.buffer_profile=sensor_config.buffer_profile;
        auto& tool=policy.tools[0];tool.profile=profile("lifecycle.configure");tool.schema=s::proposal_lifecycle_target::argument_schema();tool.target_profile=target_config.profile;
        assert(tool.target_alias.assign("node")&&tool.argument_type.assign(s::proposal_lifecycle_target::argument_type)&&tool.encoding.assign("le64")&&tool.frame.assign("logical"));
        tool.action=pc::target_action::lifecycle_submit;tool.effect=p::admission::effect_class::non_physical;tool.observations=p::admission::observation_policy::latest_required;
        tool.minimum_observations=1;tool.max_observation_age_ns=UINT64_C(60000000000);assert(p::valid(policy));
        assert(proposals.bind("suggestions",policy)==error::success);p::arbiter_plan arbitration;assert(arbitration.proposal_alias.assign("suggestions")&&arbitration.target_alias.assign("node"));arbitration.tool_mask=1;
        assert(proposals.bind_arbiter("judge",arbitration)==error::success&&proposals.prepare(sensor.access(),target.access())==error::success);
        std::vector<std::uint8_t> out;assert(model_call(c::operation::open,opening(),out)==error::success);s::wire::reader reader{out};assert(reader.u64(session)&&reader.done());
    }
    error model_call(c::operation action,s::wire::bytes input,std::vector<std::uint8_t>& out)
    {std::size_t required{};auto copy=[&](auto bytes){out.assign(bytes.begin(),bytes.end());return true;};return model.invoke(who,action,model_cap,input,16384,required,copy);}
    std::vector<std::uint8_t> arguments(std::uint64_t key=1,std::uint64_t transition=1,std::uint64_t generation=1,std::uint64_t deadline=0)
    {return words({client,key,transition,generation,deadline?deadline:observed_now+UINT64_C(1000000000)});}
    std::vector<std::uint8_t> envelope()
    {
        auto arguments=this->arguments();auto const& tool=policy.tools[0];p::envelope value{tool.profile,tool.schema,tool.target_profile,tool.target_alias.view(),tool.frame.view(),
            tool.argument_type.view(),tool.encoding.view(),tool.action,client,arguments};
        std::array<std::uint8_t,8192> data;s::wire::writer writer{data};assert(p::encode_envelope(writer,value));return {writer.written().begin(),writer.written().end()};
    }
    s::proposal_dispatch::target_evidence target_evidence()
    {s::proposal_dispatch::target_evidence result;assert(target.resolve(policy.tools[0],client,result)==error::success);return result;}
    error validate(s::wire::bytes input,s::proposal_dispatch::target_evidence const& actual)
    {std::array<p::admission::observation,1> observations;return target.validate(who,target_cap,policy.tools[0],actual,input,observations);}
    ml::submission_observation observe()
    {ml::submission_observation result;assert(lifecycle.observe_submission("node",client,result)==error::success);return result;}
    s::handle publish()
    {
        s::proposal_dispatch::source_reference reference{0,sensor_cap,sensor_handle,buffer_cap};s::proposal_dispatch::captured_inputs captured;
        assert(proposals.capture(who,"suggestions",{&reference,1},captured)==error::success&&captured.buffer(0)==input);
        auto final=envelope();std::array<std::uint8_t,512> data;s::wire::writer out{data};constexpr std::array<std::uint8_t,1> params{1};
        assert(out.u64(session)&&out.u64(1)&&out.u64(session)&&c::write_profile(out,profile("tensor.increment"))&&out.text("increment")&&out.text("bytes")&&out.blob(params));
        assert(out.u64(1)&&out.u64(buffer_cap)&&out.u64(input)&&out.u64(0)&&out.u64(0)&&out.boolean(false)&&out.boolean(false));
        assert(out.u64(8192)&&out.u64(1)&&out.u64(8)&&out.u64(s::models::now()+UINT64_C(1000000000)));
        std::vector<std::uint8_t> response;assert(model_call(c::operation::submit,out.written(),response)==error::success);s::wire::reader reader{response};s::handle request{};assert(reader.u64(request));
        s::model_dispatch::work work;assert(model.claim("brain",8,work)==error::success);
        assert(work.with_input(0,[](auto bytes,auto const&,auto const&){return bytes.size()==4&&bytes[0]==1;})==error::success);
        assert(work.finish(s::models::state::succeeded,final)==error::success);s::handle id{};
        assert(proposals.publish(captured,request,final,observed_now+UINT64_C(2000000000),id)==error::success);return id;
    }
    error accept(s::handle id,std::vector<std::uint8_t>& response,s::proposal_dispatch::sink custom={})
    {
        auto input=words({accept_cap,1,read_cap,id,target_cap,client,observed_now+UINT64_C(1000000000)});std::size_t required{};
        auto copy=[&](auto bytes){response.assign(bytes.begin(),bytes.end());return true;};
        return proposals.invoke(who,pc::operation::accept,accept_cap,input,8192,required,custom?custom:s::proposal_dispatch::sink(copy));
    }
};
static void full_actual_chain_is_inert_and_recoverable()
{
    actual_fixture f;auto before=f.observe();auto id=f.publish();std::vector<std::uint8_t> out;
    assert(f.accept(id,out,[](auto){return false;})==error::io&&f.observe().current==before.current);
    assert(f.accept(id,out)==error::success);auto saved=out;s::wire::reader result{out};s::handle request{},provider{};std::string_view state,effect,reason;
    assert(result.u64(request)&&request&&result.text(state)&&state=="admitted"&&result.text(effect)&&effect=="locally_accepted"&&result.u64(provider)&&provider&&result.text(reason)&&reason.empty()&&result.done());
    assert(f.observe().current==before.current&&f.observe().next_key==1);
    assert(f.lifecycle.take("node",f.server,[&](auto const&){return true;})==error::again);
    assert(f.permissions->revoke("node",[&](auto alias){f.lifecycle.revoke(alias);})==error::success);
    assert(f.accept(id,out)==error::success&&out==saved); // Historical receipt creates no execution authority.
}
static void actual_target_and_source_changes_refuse()
{
    for(int mode=0;mode<5;++mode)
    {
        actual_fixture f;auto id=f.publish();std::vector<std::uint8_t> out;
        if(mode==0){ml::receipt receipt;assert(f.lifecycle.submit("node",f.client,1,1,1,s::models::now()+UINT64_C(1000000000),receipt)==error::success);}
        if(mode==1){constexpr std::array<std::uint8_t,4> bytes{1,2,3,4};assert(f.sensor.ingest({101,1,2,0,0,0,false},bytes)==error::success);}
        if(mode==2)f.sensor.retire();
        if(mode==3)f.target.revoke("node");
        if(mode==4)f.buffers.revoke("data");
        assert(f.accept(id,out)!=error::success&&out.empty());
    }
}
static void actual_schema_profiles_and_deadlines()
{
    actual_fixture f;auto evidence=f.target_evidence();assert(f.validate(f.arguments(),evidence)==error::success);
    assert(f.validate(f.arguments(2),evidence)==error::conflict&&f.validate(f.arguments(1,3),evidence)==error::denied);
    assert(f.validate(f.arguments(1,1,2),evidence)==error::conflict&&f.validate(f.arguments(1,1,1,observed_now),evidence)==error::timeout);
    assert(f.validate(f.arguments(1,1,1,observed_now+f.bounds.max_deadline_ns+1),evidence)==error::timeout);
    auto malformed=f.arguments();malformed.push_back(0);assert(!evidence.validate_arguments(malformed));
    auto wrong=f.policy.tools[0];wrong.effect=p::admission::effect_class::physical;s::proposal_dispatch::target_evidence untouched;
    assert(f.target.resolve(wrong,f.client,untouched)==error::denied);wrong=f.policy.tools[0];wrong.schema.sha256[0]^=1;
    assert(f.target.resolve(wrong,f.client,untouched)==error::denied&&f.target.resolve(f.policy.tools[0],f.server,untouched)==error::bad_handle);
    auto expected=evidence;++expected.authority_epoch;assert(f.validate(f.arguments(),expected)==error::conflict);
    assert(f.lifecycle.close("node",f.client)==error::success&&f.target.resolve(f.policy.tools[0],f.client,untouched)==error::bad_handle);
}
static void cached_observation_never_expires_or_executes()
{
    actual_fixture f;ml::receipt receipt;auto deadline=s::models::now()+UINT64_C(1000000000);
    assert(f.lifecycle.submit("node",f.client,1,1,1,deadline,receipt)==error::success);auto before=f.observe();auto evidence=f.target_evidence();
    observed_now=deadline+1;assert(f.validate(f.arguments(2,1,before.current.generation,observed_now+100),evidence)==error::timeout);
    auto after=f.observe();assert(after.current==before.current&&after.current.request&&after.current.state==10&&after.next_key==2);
}
static thread_local actual_fixture* reentrant{};
static void reentrant_clock_retire_and_revoke()
{
    for(int mode=0;mode<2;++mode)
    {
        actual_fixture f;auto evidence=f.target_evidence();auto args=f.arguments();reentrant=&f;
        clock_callback=mode==0?+[]()noexcept{reentrant->target.retire();}:
            +[]()noexcept{assert(reentrant->permissions->revoke("node",[](auto){})==error::success);};
        assert(f.validate(args,evidence)!=error::success);clock_callback=nullptr;assert(f.observe().current.state==1);
    }
    reentrant=nullptr;
}
static void zero_allocating_observation_and_fork()
{
    actual_fixture f;auto evidence=f.target_evidence();auto args=f.arguments();
#if defined(UWVM_MODEL_DISPATCH_FAULT_INJECTION)
    fail_after=1;failed_allocation=false;assert(f.validate(args,evidence)==error::success);assert(!failed_allocation&&fail_after==1);fail_after=0;
#endif
    auto pid=::fork();assert(pid>=0);if(!pid){::alarm(3);s::proposal_dispatch::target_evidence out;::_exit(f.target.resolve(f.policy.tools[0],f.client,out)==error::denied&&f.validate(args,evidence)==error::denied?0:4);}
    int result{};assert(::waitpid(pid,&result,0)==pid&&WIFEXITED(result)&&!WEXITSTATUS(result));
}
#if defined(UWVM_PROPOSAL_LIFECYCLE_FIXTURE_ONLY)
int prior_lifecycle_fixture_main()
#else
int main()
#endif
{
    full_actual_chain_is_inert_and_recoverable();actual_target_and_source_changes_refuse();actual_schema_profiles_and_deadlines();
    cached_observation_never_expires_or_executes();reentrant_clock_retire_and_revoke();zero_allocating_observation_and_fork();
    std::cout<<"passed 6 actual sensor model lifecycle proposal safety groups\n";return 0;
}
