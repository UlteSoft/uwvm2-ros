/** @file @brief Exercise actual finite recording/replay CLI callbacks without activating native I/O. */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/cmdline/params/scene.h>
#include "recording_deployment_fixture.h"
#include <cstdio>
namespace p=uwvm2::uwvm::cmdline::params;namespace cli=uwvm2::utils::cmdline;namespace f=recording_fixture;
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
int main()
{
    constexpr auto ok=cli::parameter_return_type::def,fail=cli::parameter_return_type::return_m1_imme;
    auto profile=f::recording_profile(),replay=f::replay_profile();auto digest=f::hex(profile.sha256),replay_digest=f::hex(replay.sha256),buffer_digest=f::hex(f::buffer_profile().sha256),canonical=f::hex(f::definition());
    assert(call(p::ros_scene_create,{"robot"})==ok&&call(p::ros_scene_bind,{"robot","main"})==ok);
    assert(call(p::ros_scene_recording_profile,{"robot","recording",profile.identity.view(),digest})==fail);
    assert(call(p::ros_scene_recording_definition,{"robot","recording","/must-not-be-read","1",digest})==fail);
    assert(call(p::ros_scene_grant,{"robot","recording","recording","bool.records","private-recording","recording.open,recording.append,recording.checkpoint,recording.stop,resource.close,resource.observe,data.export"})==ok);
    assert(call(p::ros_scene_grant,{"robot","replay","replay","bool.records","private-replay","replay.open,replay.read,replay.seek,resource.close"})==ok);
    assert(call(p::ros_scene_grant,{"robot","input","buffer","bool.bytes","sealed-buffer","buffer.create,buffer.write,buffer.read,buffer.seal,resource.close,data.export"})==ok);
    assert(call(p::ros_scene_check,{})==fail);
    auto uppercase=digest;for(auto& c:uppercase)if(c>='a'&&c<='f')c-=32;
    assert(call(p::ros_scene_recording_profile,{"robot","recording",profile.identity.view(),uppercase})==fail);
    assert(call(p::ros_scene_recording_profile,{"robot","recording",profile.identity.view(),digest})==ok);
    assert(call(p::ros_scene_recording_profile,{"robot","recording",profile.identity.view(),digest})==fail);
    assert(call(p::ros_scene_recording_definition_hex,{"robot","recording",canonical})==fail);
    for(auto invalid:{"unlimited","-1","18446744073709551616","1.0"})
    {
        assert(call(p::ros_scene_recording_budget,{"robot","recordings",invalid})==fail);
        assert(call(p::ros_scene_recording_limit,{"robot","recording","segments",invalid})==fail);
        assert(call(p::ros_scene_replay,{"robot","replay","recording",replay.identity.view(),replay_digest,invalid})==fail);
    }
    assert(call(p::ros_scene_recording_limit,{"robot","recording","unknown","1"})==fail);
    assert(call(p::ros_scene_recording_limit,{"robot","recording","segments"})==fail);
    assert(call(p::ros_scene_recording_limit,{"robot","recording","segments","2"},true)==fail);
    for(std::size_t i{};i<f::limits.size();++i)
    {
        std::array<char,32> number{};auto converted=std::to_chars(number.data(),number.data()+number.size()-1,f::limits[i]);assert(converted.ec==std::errc{});
        assert(call(p::ros_scene_recording_limit,{"robot","recording",f::s::recording::configuration::limit_names[i],{number.data(),static_cast<std::size_t>(converted.ptr-number.data())}})==ok);
    }
    for(auto invalid:{"0","GG","AA",""})assert(call(p::ros_scene_recording_definition_hex,{"robot","recording",invalid})==fail);
    assert(call(p::ros_scene_recording_definition,{"robot","recording","/does-not-exist","unlimited",digest})==fail);
    assert(call(p::ros_scene_recording_definition,{"robot","recording","/does-not-exist","1",uppercase})==fail);
    assert(call(p::ros_scene_recording_definition,{"robot","recording","/does-not-exist","1",digest})==fail);
    assert(call(p::ros_scene_recording_definition_hex,{"robot","recording",canonical})==ok);
    assert(call(p::ros_scene_recording_definition_hex,{"robot","recording",canonical})==fail);
    assert(call(p::ros_scene_replay,{"robot","replay","recording",replay.identity.view(),replay_digest,"2"})==ok);
    assert(call(p::ros_scene_replay,{"robot","replay","recording",replay.identity.view(),replay_digest,"2"})==fail);
    for(std::size_t i{};i<f::budgets.size();++i)
    {
        std::array<char,32> number{};auto converted=std::to_chars(number.data(),number.data()+number.size()-1,f::budgets[i]);assert(converted.ec==std::errc{});
        assert(call(p::ros_scene_recording_budget,{"robot",f::s::recording_budget_configuration::names[i],{number.data(),static_cast<std::size_t>(converted.ptr-number.data())}})==ok);
    }
    assert(call(p::ros_scene_buffer_budget,{"robot","16","8","32","128"})==ok);
    assert(call(p::ros_scene_buffer,{"robot","input","serialized.bool",buffer_digest,"16","8","4","4","32","128","1","4","1","1","1"})==ok);
    assert(call(p::ros_scene_check,{})==cli::parameter_return_type::return_imme);
    assert(call(p::ros_scene_recording_budget,{"robot","recordings","1"})==fail);
    assert(call(p::ros_scene_recording_limit,{"robot","recording","segments","1"})==fail);
    int identity{};std::shared_ptr<f::s::broker> broker;
    assert(uwvm2::uwvm::imported::scene::configuration.select("main",{&identity,1},broker)==f::error::success&&broker->recording_store);
    f::s::handle cap{};assert(broker->permissions.open({&identity,1},"recording",cap)==f::error::denied);
    std::puts("PASS actual recording CLI callbacks, mandatory capacities, closed codecs, logical digest, zero exposure and late-setting refusal");
}
