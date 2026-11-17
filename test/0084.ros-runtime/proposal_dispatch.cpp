/** @file @brief Real model completion, sealed sources and independent inert proposal admission. */
#pragma push_macro("main")
#undef main
#define main prior_model_dispatch_main
#include "model_dispatch.cpp"
#pragma pop_macro("main")
#include <uwvm2/imported/ros/scene/proposal_dispatch.h>
#include <functional>
namespace p=s::proposals;
namespace a=s::proposal_admission;
namespace pc=uwvm2::imported::ros::proposal_contract;
static std::atomic<std::uint64_t> proposal_now{1000};
static std::uint64_t proposal_clock() noexcept{return proposal_now.load();}
static constexpr std::array<std::uint8_t,1> argument{9};
static p::plan proposal_plan()
{
    p::plan value;value.selected=profile("proposal.policy");value.model=profile("local.identity");value.task=profile("tensor.increment");
    assert(value.model_alias.assign("brain"));value.storage={4,4096,1,8,1,1,1000,1000};value.captures=4;value.workspace_bytes=65536;value.workspace_calls=16;
    value.source_count=value.tool_count=1;auto& source=value.sources[0];
    assert(source.alias.assign("sensor")&&source.buffer_alias.assign("data")&&source.frame.assign("base"));source.profile=profile("sensor.profile");source.buffer_profile=profile("tensor");
    auto& tool=value.tools[0];tool.profile=profile("motion.tool");tool.schema=profile("motion.schema");tool.target_profile=profile("motor.profile");
    assert(tool.target_alias.assign("motor")&&tool.frame.assign("base")&&tool.argument_type.assign("bounded.motion")&&tool.encoding.assign("bytes"));
    tool.action=pc::target_action::device_submit;tool.effect=a::effect_class::physical;tool.observations=a::observation_policy::latest_required;
    tool.max_observation_age_ns=500;tool.minimum_observations=1;assert(p::valid(value));return value;
}
static s::grant extra_grant(std::string_view alias,s::purpose kind,std::string_view binding,std::initializer_list<char const*> actions)
{
    s::grant value;value.alias=alias;value.resource=alias;value.kind=kind;value.binding=binding;value.max_handles=4;value.max_resources=4;
    for(auto name:actions){value.actions.emplace(name);}
    return value;
}
struct proposal_fixture
{
    std::shared_ptr<s::authority> permissions=uwvm2::utils::container::make_shared<s::authority>();
    s::buffers::registry buffers;s::model_dispatch model{permissions,buffers};
    s::proposal_dispatch dispatch{permissions,model,buffers,71,81,{1,2},proposal_clock};
    int instance{};s::caller who{&instance,1};s::handle model_cap{},data_cap{},source_cap{},read_cap{},accept_cap{},second_cap{},target_cap{},input{},session{};
    s::proposal_dispatch::source_sample sample;s::proposal_dispatch::target_evidence target;
    std::function<void()> source_callback,target_callback;unsigned validations{},executions{};
    p::plan policy;
    explicit proposal_fixture(p::plan plan=proposal_plan(),s::grant arbiter_grant=extra_grant("judge",s::purpose::arbiter,"proposal-arbiter",{"proposal.accept","request.describe","request.release"})) :policy(plan)
    {
        proposal_now=1000;
        assert(permissions->expose("capability.open",s::exposure::allow)==error::success&&permissions->expose("capability.close",s::exposure::allow)==error::success);
        for(auto const& operation:c::operations)assert(permissions->expose(uwvm2::utils::container::std_string(operation.name),s::exposure::allow)==error::success);
        for(auto name:{"buffer.read","proposal.read","proposal.accept","device.submit","resource.close"})assert(permissions->expose(name,s::exposure::allow)==error::success);
        assert(permissions->configure(model_grant())==error::success&&permissions->configure(data_grant())==error::success);
        assert(permissions->configure(extra_grant("sensor",s::purpose::device,"sensor",{"resource.observe"}))==error::success);
        assert(permissions->configure(extra_grant("suggestions",s::purpose::proposal,"model-proposals",{"proposal.read","resource.close"}))==error::success);
        assert(permissions->configure(std::move(arbiter_grant))==error::success);
        assert(permissions->configure(extra_grant("judge.two",s::purpose::arbiter,"proposal-arbiter",{"proposal.accept"}))==error::success);
        assert(permissions->configure(extra_grant("motor",s::purpose::device,"motor",{"device.submit"}))==error::success);
        assert(permissions->enroll(who)==error::success);
        for(auto entry:{std::pair{"brain",&model_cap},{"data",&data_cap},{"sensor",&source_cap},{"suggestions",&read_cap},{"judge",&accept_cap},{"judge.two",&second_cap},{"motor",&target_cap}})
            assert(permissions->open(who,entry.first,*entry.second)==error::success);
        auto bp=buffer_plan();bp.leases=16;assert(buffers.bind("data",bp)==error::success);
        auto mp=model_plan();assert(mp.output_type.assign(p::model_output_type));mp.output_bytes=8192;mp.output_items=2;mp.workspace_bytes=32768;
        mp.validate_output=[](auto bytes,bool terminal)noexcept{p::envelope value;return terminal&&p::decode_envelope(bytes,value);};
        assert(model.bind("brain",mp)==error::success&&model.attach("brain",8)==error::success);
        assert(dispatch.bind("suggestions",policy)==error::success);p::arbiter_plan arbitration;assert(arbitration.proposal_alias.assign("suggestions")&&arbitration.target_alias.assign("motor"));arbitration.tool_mask=1;
        assert(dispatch.bind_arbiter("judge",arbitration)==error::success&&dispatch.bind_arbiter("judge.two",arbitration)==error::success);
        assert(dispatch.prepare({this,source_capture,source_validate},{this,target_resolve,target_validate})==error::success);
        s::buffers::layout layout;layout.schema=profile("tensor");layout.rank=1;layout.shape[0]=layout.bytes=4;layout.strides[0]=1;
        assert(buffers.create("data",layout,2,[&](auto id){input=id;return true;})==error::success);
        constexpr std::array<std::uint8_t,4> data{1,2,3,4};std::uint64_t accepted{};
        assert(buffers.write("data",input,0,data,accepted)==error::success&&buffers.seal("data",input,s::buffers::hash(data))==error::success);
        sample.environment=71;sample.owner=91;sample.owner_generation=81;sample.profile=profile("sensor.profile");assert(sample.frame.assign("base"));
        sample.observation={data_cap,input,101,1,102,1,103,1000,1000,1,1,1,1};
        target={71,92,81,201,1,1,202,1,profile("motor.profile"),a::effect_class::physical,validate_arguments};
        std::vector<std::uint8_t> out;assert(model_call(c::operation::open,opening(),out)==error::success);s::wire::reader reader{out};assert(reader.u64(session)&&reader.done());
    }
    static bool validate_arguments(s::wire::bytes bytes)noexcept{return bytes.size()==1&&bytes[0]==9;}
    static error source_capture(void* context,s::caller,s::handle,s::handle resource,p::source_rule const&,s::proposal_dispatch::source_sample& out)
    {auto& self=*static_cast<proposal_fixture*>(context);if(resource!=self.sample.observation.source)return error::bad_handle;out=self.sample;return error::success;}
    static error source_validate(void* context,s::caller,s::handle cap,p::source_rule const&,s::proposal_dispatch::source_sample const& current,a::observation const& captured,a::observation_policy policy)
    {
        auto& self=*static_cast<proposal_fixture*>(context);if(self.source_callback)self.source_callback();
        if(cap!=self.source_cap||current.owner!=self.sample.owner||current.profile!=self.sample.profile||current.frame!=self.sample.frame)return error::denied;
        auto actual=self.sample.observation;actual.frame=captured.frame;actual.capability=captured.capability;
        if(actual.capability!=captured.capability||actual.source!=captured.source||actual.source_generation!=captured.source_generation||actual.clock!=captured.clock||actual.epoch!=captured.epoch||
           actual.calibration_generation!=captured.calibration_generation||actual.transform_generation!=captured.transform_generation||actual.source_authority_epoch!=captured.source_authority_epoch)return error::conflict;
        if(policy==a::observation_policy::latest_required&&actual!=captured)return error::conflict;
        return error::success;
    }
    static error target_resolve(void* context,p::tool_rule const&,s::handle resource,s::proposal_dispatch::target_evidence& out)
    {auto& self=*static_cast<proposal_fixture*>(context);if(resource!=self.target.resource)return error::bad_handle;out=self.target;return error::success;}
    static error target_validate(void* context,s::caller,s::handle,p::tool_rule const&,s::proposal_dispatch::target_evidence const&,s::wire::bytes,std::span<a::observation const>)
    {auto& self=*static_cast<proposal_fixture*>(context);++self.validations;if(self.target_callback)self.target_callback();return error::success;}
    error model_call(c::operation action,s::wire::bytes input,std::vector<std::uint8_t>& out)
    {std::size_t required{};auto copy=[&](auto bytes){out.assign(bytes.begin(),bytes.end());return true;};return model.invoke(who,action,model_cap,input,16384,required,copy);}
    std::vector<std::uint8_t> envelope()
    {
        auto const& tool=policy.tools[0];p::envelope value{tool.profile,tool.schema,tool.target_profile,tool.target_alias.view(),tool.frame.view(),tool.argument_type.view(),tool.encoding.view(),tool.action,target.resource,argument};
        std::array<std::uint8_t,8192> buffer{};s::wire::writer writer{buffer};assert(p::encode_envelope(writer,value));return {writer.written().begin(),writer.written().end()};
    }
    error capture(s::proposal_dispatch::captured_inputs& result)
    {s::proposal_dispatch::source_reference reference{0,source_cap,sample.observation.source,data_cap};return dispatch.capture(who,"suggestions",{&reference,1},result);}
    s::handle finish(std::vector<std::uint8_t> const& final)
    {
        std::array<std::uint8_t,512> buffer{};s::wire::writer out{buffer};constexpr std::array<std::uint8_t,1> params{1};
        assert(out.u64(session)&&out.u64(1)&&out.u64(session)&&c::write_profile(out,profile("tensor.increment"))&&out.text("increment")&&out.text("bytes")&&out.blob(params));
        assert(out.u64(1)&&out.u64(data_cap)&&out.u64(input)&&out.u64(0)&&out.u64(0)&&out.boolean(false)&&out.boolean(false));
        assert(out.u64(8192)&&out.u64(1)&&out.u64(8)&&out.u64(s::models::now()+1000000000));
        std::vector<std::uint8_t> result;assert(model_call(c::operation::submit,out.written(),result)==error::success);s::wire::reader reader{result};s::handle request{};assert(reader.u64(request));
        s::model_dispatch::work work;assert(model.claim("brain",8,work)==error::success&&work.finish(s::models::state::succeeded,final)==error::success);return request;
    }
    s::handle published(s::proposal_dispatch::captured_inputs* retained=nullptr)
    {
        s::proposal_dispatch::captured_inputs captured;assert(capture(captured)==error::success);auto final=envelope();auto request=finish(final);s::handle proposal{};
        auto result=dispatch.publish(captured,request,final,1400,proposal);if(result!=error::success)std::cerr<<"publish status="<<int(result)<<'\n';assert(result==error::success&&proposal);
        if(retained){*retained=captured;}
        return proposal;
    }
    std::vector<std::uint8_t> acceptance(s::handle proposal,std::uint64_t sequence=1,s::handle cap=0)
    {return words({cap?cap:accept_cap,sequence,read_cap,proposal,target_cap,target.resource,1300});}
    error call(pc::operation action,s::wire::bytes input,std::vector<std::uint8_t>& out,std::size_t capacity=8192,s::proposal_dispatch::sink custom={},s::handle cap=0,s::proposal_dispatch::cancellation cancel={})
    {std::size_t required{};auto copy=[&](auto bytes){out.assign(bytes.begin(),bytes.end());return true;};return dispatch.invoke(who,action,cap?cap:(action==pc::operation::read?read_cap:accept_cap),input,capacity,required,custom?custom:s::proposal_dispatch::sink(copy),cancel);}
    std::uint64_t admissions(std::string_view name)
    {uwvm2::utils::container::std_vector<s::authority_snapshot> out;assert(permissions->inspect(out,16));for(auto const& row:out)if(row.alias==name)return row.admissions;assert(false);return 0;}
};
static void full_pipeline_and_recovery()
{
    proposal_fixture value;auto id=value.published();std::vector<std::uint8_t> out;auto read=words({id});auto before=value.admissions("suggestions");
    assert(value.call(pc::operation::read,read,out,1)==error::too_large&&before==value.admissions("suggestions"));
    assert(value.call(pc::operation::read,read,out)==error::success);auto saved=out;
    auto failed=[](auto){return false;};assert(value.call(pc::operation::read,read,out,8192,failed)==error::io);
    assert(value.call(pc::operation::read,read,out)==error::success&&out==saved);
    auto accept=value.acceptance(id);before=value.admissions("judge");assert(value.call(pc::operation::accept,accept,out,1)==error::too_large&&before==value.admissions("judge"));
    assert(value.call(pc::operation::accept,accept,out,8192,failed)==error::io&&value.validations==1&&!value.executions);
    assert(value.call(pc::operation::accept,accept,out)==error::success&&value.validations==1);saved=out;
    s::wire::reader receipt{out};std::uint64_t request{},generation{};std::string_view state,effect,detail;
    assert(receipt.u64(request)&&request&&receipt.text(state)&&state=="admitted"&&receipt.text(effect)&&effect=="locally_accepted"&&receipt.u64(generation)&&generation==1&&receipt.text(detail)&&detail.empty()&&receipt.done());
    assert(value.dispatch.close(value.who,value.read_cap,id)==error::success);
    assert(value.permissions->revoke("motor",[](auto){})==error::success&&value.permissions->revoke("sensor",[](auto){})==error::success);
    assert(value.call(pc::operation::accept,accept,out)==error::success&&out==saved&&value.validations==1);
    auto changed=accept;changed.back()^=1;assert(value.call(pc::operation::accept,changed,out)==error::conflict);
}
static void authenticated_complete_envelope()
{
    proposal_fixture value;s::proposal_dispatch::captured_inputs captured;assert(value.capture(captured)==error::success);auto final=value.envelope();auto request=value.finish(final);s::handle id{};
    auto changed=final;changed.back()^=1;assert(value.dispatch.publish(captured,request,changed,1400,id)==error::conflict&&!id);
    changed=final;changed[50]^=1;assert(value.dispatch.publish(captured,request,changed,1400,id)==error::conflict&&!id);
    assert(value.dispatch.publish(captured,request,final,1400,id)==error::success);
    proposal_fixture wrong;assert(wrong.capture(captured)==error::conflict);s::proposal_dispatch::captured_inputs other;assert(wrong.capture(other)==error::success);
    s::handle absent{};assert(value.dispatch.publish(other,request,final,1400,absent)!=error::success&&!absent);
    proposal_fixture mismatch;auto body=mismatch.envelope();p::envelope decoded;assert(p::decode_envelope(body,decoded));decoded.frame="other";
    std::array<std::uint8_t,8192> storage{};s::wire::writer writer{storage};assert(p::encode_envelope(writer,decoded));body.assign(writer.written().begin(),writer.written().end());
    s::proposal_dispatch::captured_inputs good;assert(mismatch.capture(good)==error::success);request=mismatch.finish(body);
    assert(mismatch.dispatch.publish(good,request,body,1400,absent)==error::denied&&!absent);
}
static void revoked_sources_targets_and_reentrancy()
{
    for(auto alias:{"sensor","data","brain"})
    {
        proposal_fixture value;s::proposal_dispatch::captured_inputs captured;assert(value.capture(captured)==error::success);auto body=value.envelope();auto request=value.finish(body);
        assert(value.permissions->revoke(alias,[](auto){})==error::success);s::handle id{};assert(value.dispatch.publish(captured,request,body,1400,id)!=error::success&&!id);
    }
    for(auto alias:{"sensor","data","motor","suggestions","judge"})
    {
        proposal_fixture value;auto id=value.published();std::vector<std::uint8_t> out;assert(value.permissions->revoke(alias,[](auto){})==error::success);
        assert(value.call(pc::operation::accept,value.acceptance(id),out)!=error::success&&!value.executions);
    }
    proposal_fixture reentrant;auto id=reentrant.published();std::vector<std::uint8_t> out;
    reentrant.target_callback=[&]{assert(reentrant.permissions->revoke("sensor",[](auto){})==error::success);};
    assert(reentrant.call(pc::operation::accept,reentrant.acceptance(id),out)==error::denied&&!reentrant.executions);
    proposal_fixture nested;id=nested.published();auto accept=nested.acceptance(id);bool busy{};
    nested.target_callback=[&]{std::vector<std::uint8_t> ignored;busy=nested.call(pc::operation::accept,accept,ignored)==error::again;};
    assert(nested.call(pc::operation::accept,accept,out)==error::success&&busy&&nested.validations==1);
    proposal_fixture independent;id=independent.published();assert(independent.call(pc::operation::accept,independent.acceptance(id,1,independent.second_cap),out,8192,{},independent.second_cap)==error::success);
    independent.dispatch.revoke("judge");assert(independent.call(pc::operation::accept,independent.acceptance(id,1,independent.second_cap),out,8192,{},independent.second_cap)==error::success);
}
static void fences_freshness_and_budgets()
{
    for(int mode=0;mode<5;++mode)
    {
        proposal_fixture value;auto id=value.published();std::vector<std::uint8_t> out;
        if(mode==0)++value.target.controller_epoch;
        if(mode==1)++value.sample.observation.sample_sequence;
        if(mode==2)++value.sample.observation.epoch;
        if(mode==3)proposal_now=1401;
        if(mode==4)value.target.owner_generation++;
        assert(value.call(pc::operation::accept,value.acceptance(id),out)!=error::success&&!value.executions);
    }
    auto plan=proposal_plan();plan.captures=1;proposal_fixture finite{plan};s::proposal_dispatch::captured_inputs first,second;
    assert(finite.capture(first)==error::success&&finite.capture(second)==error::too_large);auto copy=first;first.reset();assert(finite.capture(second)==error::too_large);copy.reset();assert(finite.capture(second)==error::success);
    s::proposal_dispatch::usage usage;assert(finite.dispatch.inspect("suggestions",usage)==error::success&&usage.captures==1&&usage.workspace_calls==1);
    auto grant=extra_grant("judge",s::purpose::arbiter,"proposal-arbiter",{"proposal.accept"});grant.limits.uncertain=true;grant.limits.calls=0;grant.limits.window=std::chrono::hours(1);
    proposal_fixture zero{proposal_plan(),grant};auto id=zero.published();std::vector<std::uint8_t> out;assert(zero.call(pc::operation::accept,zero.acceptance(id),out)==error::again&&!zero.validations&&!zero.executions);
}
static void publication_reentry_and_fork()
{
    proposal_fixture value;s::proposal_dispatch::captured_inputs capture;assert(value.capture(capture)==error::success);auto body=value.envelope();auto request=value.finish(body);s::handle id{};
    unsigned observed{};value.source_callback=[&]{if(++observed==2)value.dispatch.revoke("suggestions");};
    assert(value.dispatch.publish(capture,request,body,1400,id)!=error::success&&!id);
    proposal_fixture parent;id=parent.published();auto pid=::fork();assert(pid>=0);
    if(!pid){::alarm(3);std::vector<std::uint8_t> out;::_exit(parent.call(pc::operation::read,words({id}),out)==error::denied?0:3);}
    int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&WEXITSTATUS(status)==0);
}
int main()
{
    full_pipeline_and_recovery();authenticated_complete_envelope();revoked_sources_targets_and_reentrancy();fences_freshness_and_budgets();publication_reentry_and_fork();
    std::puts("passed 5 proposal dispatch security groups");return 0;
}
