/** @file @brief Exercise real legacy-enable and warning-policy callbacks before mutation. */
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
#include <uwvm2/uwvm/cmdline/params/ros1.h>
#include <uwvm2/uwvm/cmdline/params/ros2.h>
#include <uwvm2/uwvm/cmdline/callback/log_disable_warning.h>
#include <uwvm2/uwvm/cmdline/callback/log_enable_warning.h>
#include <uwvm2/uwvm/cmdline/callback/log_convert_warn_to_fatal.h>
#include <cassert>
#include <cstdio>

namespace p=uwvm2::uwvm::cmdline::params;
namespace cli=uwvm2::utils::cmdline;
namespace ros=uwvm2::imported::ros;
namespace storage=uwvm2::uwvm::imported::ros;
namespace io=uwvm2::uwvm::io;
static constexpr auto ok=cli::parameter_return_type::def,fail=cli::parameter_return_type::return_m1_imme;
static auto call(cli::parameter const& parameter,std::initializer_list<std::string_view> arguments)
{
    std::array<cli::parameter_parsing_results,4> parsed{};assert(arguments.size()<parsed.size());
    parsed[0].para=&parameter;parsed[0].type=cli::parameter_parsing_results_type::parameter;
    std::size_t index{1};for(auto text:arguments)
    {
        parsed[index].str=uwvm2::utils::container::u8cstring_view{fast_io::containers::null_terminated,reinterpret_cast<char8_t const*>(text.data()),text.size()};
        parsed[index++].type=cli::parameter_parsing_results_type::arg;
    }
    return parameter.handle(parsed.data(),parsed.data(),parsed.data()+index);
}
int main(int argc,char** argv)
{
    assert(argc==4);std::string_view version{argv[1]},scope{argv[2]},mode{argv[3]};
    assert(version=="ros1"||version=="ros2");assert(scope=="global"||scope=="single"||scope=="group");
    auto v=version=="ros1"?ros::version::ros1:ros::version::ros2;
    auto s=scope=="global"?ros::scope::global:scope=="single"?ros::scope::single:ros::scope::group;
    auto const* enable=version=="ros1"?
        (scope=="global"?&p::ros1_global_enable:scope=="single"?&p::ros1_single_enable:&p::ros1_group_enable):
        (scope=="global"?&p::ros2_global_enable:scope=="single"?&p::ros2_single_enable:&p::ros2_group_enable);
    auto const* disable=version=="ros1"?
        (scope=="global"?&p::ros1_global_disable:scope=="single"?&p::ros1_single_disable:&p::ros1_group_disable):
        (scope=="global"?&p::ros2_global_disable:scope=="single"?&p::ros2_single_disable:&p::ros2_group_disable);
    if(scope!="global")
    {
        auto const* create=version=="ros1"?(scope=="single"?&p::ros1_single_create:&p::ros1_group_create):
            (scope=="single"?&p::ros2_single_create:&p::ros2_group_create);
        assert(call(*create,{"legacy"})==ok);
    }
    auto scoped=[&](cli::parameter const& parameter){return scope=="global"?call(parameter,{}):call(parameter,{"legacy"});};
    assert(scoped(*disable)==ok); // Disablement never emits a deprecation warning.
    assert(io::show_deprecated_ros_warning&&!io::deprecated_ros_warning_fatal);
    bool fatal=mode.find("fatal")!=mode.npos;
    bool suppressed=mode=="suppressed"||mode=="suppressed-fatal";
    bool all=mode.starts_with("all-");
    auto category=all?"all":"deprecated-ros";
    if(mode!="default")
    {
        assert(call(p::log_disable_warning,{category})==ok&&!io::show_deprecated_ros_warning);
        if(all)assert(!io::show_untrusted_dl_warning);
        if(!suppressed)
        {
            assert(call(p::log_enable_warning,{category})==ok&&io::show_deprecated_ros_warning);
            if(all)assert(io::show_untrusted_dl_warning);
        }
    }
    if(fatal)assert(call(p::log_convert_warn_to_fatal,{category})==ok&&io::deprecated_ros_warning_fatal);
    bool const configured_before=storage::policy_configured;
    bool const enabled_before=storage::policy_any_enabled;
    auto result=scoped(*enable);bool const rejected=fatal&&!suppressed;
    assert(result==(rejected?fail:ok));
    auto target=storage::runtime_policy.find_target(v,s,scope=="global"?"":"legacy");
    assert(target&&target->enabled==!rejected);
    if(rejected)assert(storage::policy_configured==configured_before&&storage::policy_any_enabled==enabled_before);
    else assert(storage::policy_any_enabled&&storage::policy_configured);
    std::puts("PASS legacy enable warning and mutation boundary");
}
