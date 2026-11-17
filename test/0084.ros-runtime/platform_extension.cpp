/** @file @brief Real nested CLI callbacks and default-zero platform child policy. */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/cmdline/params/scene.h>
#include <cassert>
#include <cstdio>
namespace s=uwvm2::imported::ros::scene;
namespace p=uwvm2::uwvm::cmdline::params;
namespace cli=uwvm2::utils::cmdline;
using op=s::robot_contract::operation;
constexpr auto child="uwvm_ros_v2.platform_extension.fixture.v1";
constexpr std::array routes{s::platform_extensions::route{"session.open",op::device_open},s::platform_extensions::route{"cycle.submit",op::control_submit}};
static s::grant device(std::string_view alias="drive")
{s::grant value;value.alias.assign(alias);value.resource="test-only-device";value.kind=s::purpose::device;value.binding="framed-scalar";value.actions.emplace("device.open");return value;}
static cli::parameter_return_type call(std::initializer_list<std::string_view> arguments,bool occupied=false)
{
    std::array<cli::parameter_parsing_results,8> parsed{};assert(arguments.size()+1<parsed.size());
    auto& parameter=p::ros_scene_child;parsed[0].para=&parameter;parsed[0].type=cli::parameter_parsing_results_type::parameter;
    std::size_t index{1};for(auto text:arguments)
    {
        parsed[index].str=uwvm2::utils::container::u8cstring_view{fast_io::containers::null_terminated,reinterpret_cast<char8_t const*>(text.data()),text.size()};
        parsed[index++].type=occupied?cli::parameter_parsing_results_type::occupied_arg:cli::parameter_parsing_results_type::arg;
    }
    auto result=parameter.handle(parsed.data(),parsed.data(),parsed.data()+index);
    if(!occupied)for(std::size_t i{1};i<index;++i)
        assert(parsed[i].type==(result==cli::parameter_return_type::def?cli::parameter_parsing_results_type::occupied_arg:cli::parameter_parsing_results_type::arg));
    return result;
}
static void closed_policy()
{
    for(auto invalid:{"uwvm_platform_v1","uwvm_ros_v2.fixture.v1","uwvm_ros_v2.platform_extension.*.v1", "uwvm_ros_v2.platform_extension.fixture.v0",
        "uwvm_ros_v2.platform_extension.fixture.v01","uwvm_ros_v2.platform_extension.fixture.v4294967296","uwvm_ros_v2.platform_extension.fixture.v1.tail",
        "uwvm_ros_v2.platform_extension.UPPER.v1","uwvm_ros_v2.platform_extension.fixture.more.v1"})assert(!s::platform_extensions::module_name(invalid));
    assert(s::platform_extensions::module_name("uwvm_ros_v2.platform_extension.nvidia-groot.v4294967295"));
    s::platform_extensions::policy policy;op selected=op::control_stop;
    assert(policy.resolve(child,"session.open","drive",selected)==s::error::denied&&selected==op::control_stop);
    assert(policy.select(child,true)==s::error::unsupported&&policy.bind(child,device())==s::error::unsupported);
    assert(policy.register_module({child,routes})==s::error::success&&policy.register_module({child,routes})==s::error::conflict);
    assert(!policy.enabled(child)&&policy.bind(child,device())==s::error::success&&!policy.allows("drive")&&policy.allows("other"));
    auto denied=policy;assert(denied.seal()==s::error::success&&denied.resolve(child,"session.open","drive",selected)==s::error::denied);
    assert(policy.select(child,true)==s::error::success&&policy.select(child,false)==s::error::conflict);
    assert(policy.register_module({"uwvm_ros_v2.platform_extension.second.v1",routes})==s::error::success);
    assert(policy.bind("uwvm_ros_v2.platform_extension.second.v1",device())==s::error::conflict);
    assert(policy.seal()==s::error::success&&policy.allows("drive"));
    assert(policy.resolve(child,"session.open","drive",selected)==s::error::success&&selected==op::device_open);
    assert(policy.resolve(child,"cycle.submit","drive",selected)==s::error::success&&selected==op::control_submit);
    assert(policy.resolve(child,"session.open","other",selected)==s::error::denied&&selected==op::control_submit);
    assert(policy.bind(child,device("late"))==s::error::denied&&policy.select(child,false)==s::error::denied);
    s::platform_extensions::policy many;
    for(unsigned i{};i<129;++i)
    {
        std::array<char,128> name{};auto length=std::snprintf(name.data(),name.size(),"uwvm_ros_v2.platform_extension.fixture%u.v1",i);
        assert(length>0&&many.register_module({{name.data(),static_cast<std::size_t>(length)},routes})==s::error::success);
    }
    assert(many.select("uwvm_ros_v2.platform_extension.fixture128.v1",true)==s::error::success);
    assert(many.seal()==s::error::invalid&&many.bind("uwvm_ros_v2.platform_extension.fixture128.v1",device())==s::error::success);
    assert(many.seal()==s::error::success&&many.resolve("uwvm_ros_v2.platform_extension.fixture128.v1","session.open","drive",selected)==s::error::success);
    s::platform_extensions::policy invalid;std::array bad{s::platform_extensions::route{"bad",static_cast<op>(255)}};
    assert(invalid.register_module({child,bad})==s::error::invalid);
    std::array duplicates{routes[0],routes[0]};assert(invalid.register_module({child,duplicates})==s::error::conflict);
    assert(invalid.register_module({child,routes})==s::error::success);
    auto wrong=device();wrong.kind=s::purpose::control;wrong.binding="framed-control";assert(invalid.bind(child,wrong)==s::error::denied);
}
int main()
{
    closed_policy();auto& d=uwvm2::uwvm::imported::scene::configuration;
    constexpr auto ok=cli::parameter_return_type::def,fail=cli::parameter_return_type::return_m1_imme;
    assert(call({"absent","platform_extension",child,"enable"})==fail);
    assert(d.create("robot")==s::error::success&&d.create("other")==s::error::success);
    assert(call({"robot","platform_extension",child,"enable"})==fail); // No CLI registration escape.
    assert(d.register_platform("robot",{child,routes})==s::error::success);
    assert(call({"other","platform_extension",child,"enable"})==fail); // Exact parent ownership.
    for(auto invalid:{"on","true","unlimited","*"})assert(call({"robot","platform_extension",child,invalid})==fail);
    assert(call({"robot","platform_extension",child})==fail&&call({"robot","platform_extension",child,"enable"},true)==fail);
    assert(call({"robot","platform_extension.bind",child,"drive"})==fail);
    assert(call({"robot","platform_extension.register",child,"drive"})==fail);
    assert(d.allow("robot",device())==s::error::success);
    assert(call({"robot","platform_extension.bind",child,"drive"})==ok);
    assert(call({"robot","platform_extension.bind",child,"drive"})==fail);
    assert(call({"robot","platform_extension",child,"enable"})==ok);
    assert(call({"robot","platform_extension",child,"disable"})==fail);
    // An enabled child is still missing its common driver plan. No policy or
    // provider may be activated merely because the child switch is present.
    assert(d.validate()!=s::error::success);
    s::deployment empty;assert(empty.create("robot")==s::error::success&&empty.register_platform("robot",{child,routes})==s::error::success);
    assert(empty.platform_setting("robot","platform_extension",child,"disable")==s::error::success&&empty.validate()==s::error::success);
    assert(empty.platform_setting("robot","platform_extension",child,"enable")!=s::error::success);
    empty.retire();assert(empty.platform_setting("robot","platform_extension",child,"enable")!=s::error::success);
    std::puts("PASS platform child ownership, nested CLI, 129 registrations, zero authority and immutable configuration");
}
