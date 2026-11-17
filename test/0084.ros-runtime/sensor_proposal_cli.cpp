/** @file @brief Actual receive-only sensor and model-proposal CLI parsing without native activation. */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/cmdline/params/scene.h>
#include "sensor_proposal_deployment_fixture.h"
#include <cstdio>
namespace p=uwvm2::uwvm::cmdline::params;namespace cli=uwvm2::utils::cmdline;namespace f=sensor_proposal_fixture;
static cli::parameter_return_type call(cli::parameter const& parameter,std::initializer_list<std::string_view> arguments,bool occupied=false)
{
    std::array<cli::parameter_parsing_results,32> parsed{};assert(arguments.size()<parsed.size());
    parsed[0].para=&parameter;parsed[0].type=cli::parameter_parsing_results_type::parameter;std::size_t index=1;
    for(auto text:arguments)
    {
        parsed[index].str=uwvm2::utils::container::u8cstring_view{fast_io::containers::null_terminated,reinterpret_cast<char8_t const*>(text.data()),text.size()};
        parsed[index++].type=occupied?cli::parameter_parsing_results_type::occupied_arg:cli::parameter_parsing_results_type::arg;
    }
    auto result=parameter.handle(parsed.data(),parsed.data(),parsed.data()+index);
    if(!occupied)for(std::size_t i=1;i<index;++i)assert(parsed[i].type==
        (result==cli::parameter_return_type::def||result==cli::parameter_return_type::return_imme?cli::parameter_parsing_results_type::occupied_arg:cli::parameter_parsing_results_type::arg));
    return result;
}
static auto number(std::uint64_t value)
{
    std::array<char,32> output{};auto result=std::to_chars(output.data(),output.data()+output.size()-1,value);assert(result.ec==std::errc{});
    return uwvm2::utils::container::std_string(output.data(),result.ptr);
}
int main()
{
    constexpr auto ok=cli::parameter_return_type::def,fail=cli::parameter_return_type::return_m1_imme;
    auto& d=uwvm2::uwvm::imported::scene::configuration;f::environment(d,false);
    auto sensor=f::sensor_profile(),proposal=f::proposal_profile();auto sensor_hash=f::hex(sensor.sha256),proposal_hash=f::hex(proposal.sha256);
    auto sensor_wire=f::hex(f::sensor_definition()),proposal_wire=f::hex(f::proposal_definition());
    assert(call(p::ros_scene_sensor_profile,{"robot","sensor",sensor.identity.view(),sensor_hash,"octets"})==fail);
    assert(call(p::ros_scene_sensor_definition,{"robot","sensor","/must-not-open","1",sensor_hash})==fail);
    assert(call(p::ros_scene_sensor_definition_hex,{"robot","sensor",sensor_wire})==fail);
    assert(call(p::ros_scene_proposal_definition_hex,{"robot","suggestions",proposal_wire})==fail);
    assert(d.allow("robot",f::grant("sensor",f::s::purpose::sensor,"bounded-sensor",{"sensor.open","sensor.take","sensor.describe","data.export","resource.observe","resource.close"}))==f::error::success);
    assert(d.allow("robot",f::grant("frames",f::s::purpose::buffer,"sealed-buffer",{"buffer.read","resource.close"}))==f::error::success&&
           d.buffer_budget("robot",32,8,64,64)==f::error::success&&d.buffer_plan("robot","frames",f::buffer_plan())==f::error::success);
    for(auto invalid:{"unlimited","-1","18446744073709551616","1.0"})
    {
        assert(call(p::ros_scene_sensor_budget,{"robot",invalid,"1048576"})==fail);
        assert(call(p::ros_scene_sensor_descriptor,{"robot","sensor",invalid})==fail);
        assert(call(p::ros_scene_sensor_limit,{"robot","sensor","source.handles",invalid})==fail);
        assert(call(p::ros_scene_proposal_budget,{"robot",invalid,"1","2","4","1048576"})==fail);
        assert(call(p::ros_scene_proposal_arbiter,{"robot","judge","suggestions","node",invalid})==fail);
    }
    assert(call(p::ros_scene_sensor_budget,{"robot","33","1048576"})==fail);
    assert(call(p::ros_scene_sensor_budget,{"robot","1","1048576"})==ok);
    assert(call(p::ros_scene_sensor_budget,{"robot","1","1048576"})==fail);
    assert(call(p::ros_scene_sensor_profile,{"robot","sensor",sensor.identity.view(),sensor_hash,"arbitrary"})==fail);
    assert(call(p::ros_scene_sensor_profile,{"robot","sensor",sensor.identity.view(),sensor_hash,"octets"})==ok);
    assert(call(p::ros_scene_sensor_profile,{"robot","sensor",sensor.identity.view(),sensor_hash,"octets"})==fail);
    assert(call(p::ros_scene_sensor_limit,{"robot","sensor","source.handles","2"})==fail);
    for(auto invalid:{"","0","GG","AA"})assert(call(p::ros_scene_sensor_definition_hex,{"robot","sensor",invalid})==fail);
    assert(call(p::ros_scene_sensor_definition,{"robot","sensor","/does-not-exist","1",sensor_hash})==fail);
    assert(call(p::ros_scene_sensor_definition_hex,{"robot","sensor",sensor_wire})==ok);
    assert(call(p::ros_scene_sensor_definition_hex,{"robot","sensor",sensor_wire})==fail);
    assert(call(p::ros_scene_sensor_descriptor,{"robot","sensor","2147483648"})==fail);
    assert(call(p::ros_scene_sensor_descriptor,{"robot","sensor","2147483647"})==ok);
    assert(call(p::ros_scene_sensor_descriptor,{"robot","sensor","0"})==fail);
    assert(call(p::ros_scene_sensor_limit,{"robot","sensor","unknown","1"})==fail);
    assert(call(p::ros_scene_sensor_limit,{"robot","sensor","source.handles"})==fail);
    assert(call(p::ros_scene_sensor_limit,{"robot","sensor","source.handles","2"},true)==fail);
    auto limits=f::sensor_limits();for(std::size_t i{};i<limits.size();++i)
    {auto value=number(limits[i]);assert(call(p::ros_scene_sensor_limit,{"robot","sensor",f::s::sensors::configuration::names[i],value})==ok);}
    assert(call(p::ros_scene_sensor_limit,{"robot","sensor","source.handles","2"})==fail);
    f::model(d);
    assert(d.allow("robot",f::grant("node",f::s::purpose::lifecycle,"local-lifecycle",{"lifecycle.open","lifecycle.state","lifecycle.submit","resource.close"}))==f::error::success&&
           d.managed_lifecycle_plan("robot","node",f::lifecycle_plan())==f::error::success);
    assert(d.allow("robot",f::grant("suggestions",f::s::purpose::proposal,"model-proposals",{"proposal.capture","proposal.publish","proposal.read","resource.close"}))==f::error::success&&
           d.allow("robot",f::grant("judge",f::s::purpose::arbiter,"proposal-arbiter",{"proposal.accept","request.describe","request.release"}))==f::error::success);
    assert(call(p::ros_scene_proposal_profile,{"robot","suggestions",proposal.identity.view(),proposal_hash,"missing"})==fail);
    assert(call(p::ros_scene_proposal_profile,{"robot","suggestions",proposal.identity.view(),proposal_hash,"brain"})==ok);
    assert(call(p::ros_scene_proposal_profile,{"robot","suggestions",proposal.identity.view(),proposal_hash,"brain"})==fail);
    for(auto invalid:{"","0","GG","AA"})assert(call(p::ros_scene_proposal_definition_hex,{"robot","suggestions",invalid})==fail);
    assert(call(p::ros_scene_proposal_definition,{"robot","suggestions","/does-not-exist","1",proposal_hash})==fail);
    assert(call(p::ros_scene_proposal_definition_hex,{"robot","suggestions",proposal_wire})==ok);
    assert(call(p::ros_scene_proposal_definition_hex,{"robot","suggestions",proposal_wire})==fail);
    assert(call(p::ros_scene_proposal_limit,{"robot","suggestions","unknown","1"})==fail);
    for(std::size_t i{};i<f::proposal_limits.size();++i)
    {auto value=number(f::proposal_limits[i]);assert(call(p::ros_scene_proposal_limit,{"robot","suggestions",f::s::proposals::configuration::limit_names[i],value})==ok);}
    assert(call(p::ros_scene_proposal_limit,{"robot","suggestions","proposals","4"})==fail);
    auto retained=number(f::guest_limits().retained_bytes);
    assert(call(p::ros_scene_proposal_budget,{"robot","1","1","2","4","0"})==fail);
    assert(call(p::ros_scene_proposal_budget,{"robot","1","1","2","4",retained})==ok);
    assert(call(p::ros_scene_proposal_budget,{"robot","1","1","2","4",retained})==fail);
    for(auto invalid:{"0","256"})assert(call(p::ros_scene_proposal_arbiter,{"robot","judge","suggestions","node",invalid})==fail);
    assert(call(p::ros_scene_proposal_arbiter,{"robot","judge","suggestions","node","1"})==ok);
    assert(call(p::ros_scene_proposal_arbiter,{"robot","judge","suggestions","node","1"})==fail);
    assert(call(p::ros_scene_check,{})==cli::parameter_return_type::return_imme);
    assert(call(p::ros_scene_sensor_descriptor,{"robot","sensor","0"})==fail&&call(p::ros_scene_proposal_definition_hex,{"robot","suggestions",proposal_wire})==fail);
    int identity{};std::shared_ptr<f::s::broker> broker;
    assert(d.select("main",{&identity,1},broker)==f::error::denied&&!broker);
    std::puts("PASS actual sensor/proposal CLI callbacks, finite capacities, authenticated definitions, no static I/O, default-zero authority and late-setting refusal");
}
