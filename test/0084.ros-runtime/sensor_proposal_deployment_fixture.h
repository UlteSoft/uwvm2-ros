/** @file @brief Exact sensor/proposal deployment policy shared by native and actual CLI fixtures. */
#pragma once
#include "model_deployment_fixture.h"
#include <cassert>
namespace sensor_proposal_fixture
{
    namespace s=uwvm2::imported::ros::scene;namespace p=s::proposals;
    using s::error;using bytes=uwvm2::utils::container::std_vector<std::uint8_t>;
    template<typename F>bytes encode(F&& fn)
    {std::array<std::uint8_t,16384> storage;s::wire::writer out{storage};fn(out);return {out.written().begin(),out.written().end()};}
    inline s::buffers::profile profile(std::string_view name)
    {s::buffers::profile value;assert(value.identity.assign(name));value.sha256[0]=7;return value;}
    inline uwvm2::utils::container::std_string hex(s::bytes input)
    {constexpr char digits[]="0123456789abcdef";uwvm2::utils::container::std_string out;for(auto x:input){out.push_back(digits[x>>4]);out.push_back(digits[x&15]);}return out;}
    inline s::grant grant(std::string_view alias,s::purpose kind,std::string_view binding,std::initializer_list<char const*> actions)
    {s::grant value;value.alias=alias;value.resource=alias;value.kind=kind;value.binding=binding;value.max_handles=value.max_resources=8;for(auto action:actions)value.actions.emplace(action);return value;}
    inline auto buffer_profile(){return profile("sensor.buffer");}
    inline s::buffers::plan buffer_plan()
    {s::buffers::plan value;value.schema=buffer_profile();value.objects=8;value.bytes=32;value.object_bytes=value.transfer_bytes=value.elements=4;value.workspace_bytes=64;value.leases=64;value.rank=1;value.element_mask=value.order_mask=value.color_mask=1;return value;}
    inline s::managed_lifecycle::plan lifecycle_plan(){return {4,60000000000,254,3};}
    inline auto sensor_definition()
    {
        return encode([](auto& out)
        {
            auto registered=s::sensor_octets_registration();assert(out.text(s::sensors::definition_magic)&&out.text("sensor.actual")&&p::write_profile(out,registered.schema)&&
                out.text(registered.type.view())&&out.text(registered.encoding.view())&&out.text("logical")&&out.text("count")&&out.text("frames")&&p::write_profile(out,buffer_profile())&&
                out.u64(101)&&out.u64(1)&&out.u64(102)&&out.u64(1)&&out.u64(0)&&out.u64(0)&&out.boolean(true)&&out.boolean(false)&&out.text("reject"));
        });
    }
    inline auto sensor_profile(){auto value=profile("sensor.actual");value.sha256=s::buffers::hash(sensor_definition());return value;}
    inline auto sensor_limits()
    {
        s::sensors::plan source;auto registered=s::sensor_octets_registration();
        source.profile=sensor_profile();source.schema=registered.schema;source.type=registered.type;source.encoding=registered.encoding;
        source.frame.assign("logical");source.units.assign("count");source.validate_body=registered.validate;source.source_sequence=true;
        source.handles=2;source.records=2;source.body_bytes=4;std::uint64_t core{};assert(s::sensors::core::required_storage(source,core));
        s::sensors::ingress::plan transport{4,4096,0};assert(s::sensors::ingress::required_storage(transport,transport.retained_bytes));
        return std::array<std::uint64_t,11>{2,2,4,core,2,8,4,4096,transport.retained_bytes,65536,8};
    }
    inline auto target_profile()
    {auto value=profile("local.node");value.sha256=s::proposal_lifecycle_target::profile_digest(value.identity.view(),"node",lifecycle_plan());return value;}
    inline auto proposal_definition(bool physical=false,bool wrong_source=false)
    {
        return encode([&](auto& out)
        {
            assert(out.text(p::definition_magic)&&out.text("proposal.policy")&&out.text("brain")&&p::write_profile(out,profile("local.echo"))&&p::write_profile(out,profile("proposal.task"))&&out.u64(1)&&
                out.text(wrong_source?"unknown":"sensor")&&p::write_profile(out,sensor_profile())&&out.text("frames")&&p::write_profile(out,buffer_profile())&&out.text("logical")&&out.u64(1)&&
                p::write_profile(out,profile("lifecycle.configure"))&&p::write_profile(out,s::proposal_lifecycle_target::argument_schema())&&p::write_profile(out,target_profile())&&out.text("node")&&
                out.text("lifecycle.submit")&&out.text(physical?"physical":"non_physical")&&out.text("latest_required")&&out.text("ros.lifecycle.submit")&&out.text("le64")&&out.text("logical")&&out.u64(60000000000)&&out.u64(1));
        });
    }
    inline auto proposal_profile(bool physical=false,bool wrong_source=false)
    {auto value=profile("proposal.policy");value.sha256=s::buffers::hash(proposal_definition(physical,wrong_source));return value;}
    inline constexpr std::array<std::uint64_t,11> proposal_limits{4,4096,1,4,1,1,60000000000,60000000000,4,65536,16};
    inline auto guest_limits(){p::guest_limits value{2,4,0};assert(s::proposal_guest_dispatch::required_storage(value,value.retained_bytes));return value;}
    inline void environment(s::deployment& d,bool expose=true)
    {
        assert(d.create("robot")==error::success&&d.bind("robot","main")==error::success);
        if(!expose)return;
        for(auto action:{"capability.open","capability.close","resource.close","buffer.read","lifecycle.open","lifecycle.state","lifecycle.submit"})
            assert(d.expose("robot",action,s::exposure::allow)==error::success);
        for(auto const& action:uwvm2::imported::ros::sensor_contract::operations)assert(d.expose("robot",uwvm2::utils::container::std_string(action.name),s::exposure::allow)==error::success);
        for(auto const& action:uwvm2::imported::ros::proposal_contract::operations)assert(d.expose("robot",uwvm2::utils::container::std_string(action.name),s::exposure::allow)==error::success);
        for(auto const& action:uwvm2::imported::ros::model_contract::operations)assert(d.expose("robot",uwvm2::utils::container::std_string(action.name),s::exposure::allow)==error::success);
    }
    inline void sensor(s::deployment& d,int descriptor,std::size_t skip=11,bool export_data=true,bool budget=true)
    {
        auto cap=grant("sensor",s::purpose::sensor,"bounded-sensor",{"sensor.open","sensor.take","sensor.describe","resource.close","resource.observe"});if(export_data)cap.actions.emplace("data.export");
        assert(d.allow("robot",cap)==error::success&&d.allow("robot",grant("frames",s::purpose::buffer,"sealed-buffer",{"buffer.read","resource.close"}))==error::success);
        assert(d.buffer_budget("robot",32,8,64,64)==error::success&&d.buffer_plan("robot","frames",buffer_plan())==error::success);
        assert(d.sensor_profile("robot","sensor",sensor_profile(),"octets")==error::success&&d.sensor_definition("robot","sensor",sensor_definition())==error::success);
        auto limits=sensor_limits();for(std::size_t i{};i<limits.size();++i)if(i!=skip)assert(d.sensor_limit("robot","sensor",s::sensors::configuration::names[i],limits[i])==error::success);
        assert(d.sensor_descriptor("robot","sensor",static_cast<std::uint64_t>(descriptor))==error::success);
        if(budget)assert(d.sensor_budget("robot",{1,1024*1024})==error::success);
    }
    inline void model(s::deployment& d)
    {
        assert(d.allow("robot",grant("brain",s::purpose::language_model,"local-model",{"model.open","model.submit","model.take","model.cancel","model.describe","request.describe","request.release","resource.close"}))==error::success);
        s::models::plan plan;plan.model=profile("local.echo");plan.task=profile("proposal.task");plan.parameter_type.assign("proposal.envelope");plan.output_type.assign(p::model_output_type);assert(s::models::select_codec("bytes",plan));
        assert(d.model_plan("robot","brain",plan)==error::success&&d.model_limits("robot","brain",{1,4,16,1,1024,8192,4,65536,60000000000})==error::success&&
            d.model_budget("robot",{1,4,16,8192,4,65536})==error::success);
        configure_inert_model_worker(d,"robot","brain");
    }
    inline void proposal(s::deployment& d,std::size_t skip=11,bool physical=false,bool wrong_source=false)
    {
        model(d);
        assert(d.allow("robot",grant("node",s::purpose::lifecycle,"local-lifecycle",{"lifecycle.open","lifecycle.state","lifecycle.submit","resource.close"}))==error::success&&d.managed_lifecycle_plan("robot","node",lifecycle_plan())==error::success);
        assert(d.allow("robot",grant("suggestions",s::purpose::proposal,"model-proposals",{"proposal.capture","proposal.publish","proposal.read","resource.close"}))==error::success&&
            d.allow("robot",grant("judge",s::purpose::arbiter,"proposal-arbiter",{"proposal.accept","request.describe","request.release"}))==error::success);
        assert(d.proposal_profile("robot","suggestions",proposal_profile(physical,wrong_source),"brain")==error::success&&d.proposal_definition("robot","suggestions",proposal_definition(physical,wrong_source))==error::success);
        for(std::size_t i{};i<proposal_limits.size();++i)if(i!=skip)assert(d.proposal_limit("robot","suggestions",p::configuration::limit_names[i],proposal_limits[i])==error::success);
        assert(d.proposal_budget("robot",{1,1},guest_limits())==error::success&&d.proposal_arbiter("robot","judge","suggestions","node",1)==error::success);
    }
    inline auto no_input=[](auto const&,auto const&,auto,auto&){assert(false);return error::denied;};
}
