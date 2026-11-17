/** @file @brief Exercise actual transform CLI callbacks and complete finite configuration. */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/cmdline/params/scene.h>
#include <cassert>
#include <cstdio>
#include "transform_deployment_fixture.h"
namespace p=uwvm2::uwvm::cmdline::params;
namespace cli=uwvm2::utils::cmdline;
namespace s=uwvm2::imported::ros::scene;

static cli::parameter_return_type call(cli::parameter const& parameter,std::initializer_list<std::string_view> arguments,bool occupied=false)
{
    std::array<cli::parameter_parsing_results,32> parsed{};assert(arguments.size()<parsed.size());
    parsed[0].para=&parameter;parsed[0].type=cli::parameter_parsing_results_type::parameter;
    std::size_t index{1};for(auto text:arguments)
    {
        parsed[index].str=uwvm2::utils::container::u8cstring_view{fast_io::containers::null_terminated,reinterpret_cast<char8_t const*>(text.data()),text.size()};
        parsed[index++].type=occupied?cli::parameter_parsing_results_type::occupied_arg:cli::parameter_parsing_results_type::arg;
    }
    auto result=parameter.handle(parsed.data(),parsed.data(),parsed.data()+index);
    if(!occupied)for(std::size_t i{1};i<index;++i)
        assert(parsed[i].type==(result==cli::parameter_return_type::def||result==cli::parameter_return_type::return_imme?
            cli::parameter_parsing_results_type::occupied_arg:cli::parameter_parsing_results_type::arg));
    return result;
}
static uwvm2::utils::container::std_string hex(s::bytes bytes)
{
    constexpr char digits[]="0123456789abcdef";uwvm2::utils::container::std_string out;out.reserve(bytes.size()*2);
    for(auto value:bytes){out.push_back(digits[value>>4]);out.push_back(digits[value&15]);}return out;
}
int main()
{
    namespace f=transform_fixture;constexpr auto ok=cli::parameter_return_type::def,fail=cli::parameter_return_type::return_m1_imme;
    auto digest=hex(f::profile().sha256),canonical=hex(f::definition());
    assert(call(p::ros_scene_create,{"robot"})==ok&&call(p::ros_scene_bind,{"robot","main"})==ok);
    assert(call(p::ros_scene_transform_profile,{"robot","frames","frames",digest})==fail);
    assert(call(p::ros_scene_grant,{"robot","frames","transform","robot.frames","local-transform","transform.open,transform.publish,transform.lookup,transform.available,transform.velocity,transform.submit,transform.take,transform.cancel,transform.clear,transform.static.replace,request.describe,request.release,resource.close"})==ok);
    assert(call(p::ros_scene_check,{})==fail);
    auto uppercase=digest;for(auto& ch:uppercase)if(ch>='a'&&ch<='f')ch-=32;
    assert(call(p::ros_scene_transform_profile,{"robot","frames","frames",uppercase})==fail);
    assert(call(p::ros_scene_transform_profile,{"robot","frames","frames",digest})==ok);
    assert(call(p::ros_scene_transform_profile,{"robot","frames","frames",digest})==fail);
    for(auto invalid:{"unlimited","-1","18446744073709551616","1.0"})
    {
        assert(call(p::ros_scene_transform_limit,{"robot","frames","cache.samples",invalid})==fail);
        assert(call(p::ros_scene_transform_budget,{"robot",invalid,"8","1000000","1000000","1000000","65536"})==fail);
    }
    assert(call(p::ros_scene_transform_limit,{"robot","frames","unknown","1"})==fail);
    assert(call(p::ros_scene_transform_limit,{"robot","frames","cache.samples"})==fail);
    assert(call(p::ros_scene_transform_limit,{"robot","frames","cache.samples","16"},true)==fail);
    assert(call(p::ros_scene_transform_definition_hex,{"robot","frames","0"})==fail);
    assert(call(p::ros_scene_transform_definition_hex,{"robot","frames","GG"})==fail);
    assert(call(p::ros_scene_transform_definition_hex,{"robot","frames","AA"})==fail);
    assert(call(p::ros_scene_transform_definition_hex,{"robot","frames","00"})==fail);
    assert(call(p::ros_scene_transform_definition,{"robot","frames","/does-not-exist","unlimited",digest})==fail);
    assert(call(p::ros_scene_transform_definition,{"robot","frames","/does-not-exist","10",uppercase})==fail);
    assert(call(p::ros_scene_transform_definition_hex,{"robot","frames",canonical})==ok);
    assert(call(p::ros_scene_transform_definition_hex,{"robot","frames",canonical})==fail);
    assert(call(p::ros_scene_transform_definition,{"robot","frames","/does-not-exist","10",digest})==fail);
    assert(call(p::ros_scene_transform_budget,{"robot","2","8","1000000","1000000","1000000","65536"})==ok);
    assert(call(p::ros_scene_transform_budget,{"robot","2","8","1000000","1000000","1000000","65536"})==fail);
    for(std::size_t i{};i<f::limits.size();++i)
    {
        std::array<char,32> buffer{};auto converted=std::to_chars(buffer.data(),buffer.data()+buffer.size()-1,f::limits[i]);assert(converted.ec==std::errc{});
        assert(call(p::ros_scene_transform_limit,{"robot","frames",s::transforms::limit_names[i],{buffer.data(),static_cast<std::size_t>(converted.ptr-buffer.data())}})==ok);
    }
    assert(call(p::ros_scene_transform_limit,{"robot","frames","cache.samples","16"})==fail);
    assert(call(p::ros_scene_check,{})==cli::parameter_return_type::return_imme);
    assert(call(p::ros_scene_transform_definition_hex,{"robot","frames",canonical})==fail);
    assert(call(p::ros_scene_transform_limit,{"robot","frames","sessions","1"})==fail);
    int identity{};std::shared_ptr<s::broker> state;auto& config=uwvm2::uwvm::imported::scene::configuration;
    assert(config.select("main",{&identity,1},state)==s::error::success&&state);
    s::handle cap{};assert(state->permissions.open({&identity,1},"frames",cap)==s::error::denied); // Configuring is not exposure.
    std::puts("PASS actual transform CLI callbacks, mandatory bounds, strict canonical bytes, digest, zero exposure and late-setting refusal");
}
