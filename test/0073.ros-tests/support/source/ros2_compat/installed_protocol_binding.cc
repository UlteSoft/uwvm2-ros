/** Production Service/Action metadata admission against the actual bound
 * provider. These manually constructed capability DATA create no entity or
 * Runtime grant; only an independent authorized endpoint test can do that. */
#include "src/ros/gateway/ros2/custom_services.h"
#include "src/ros/gateway/ros2/custom_actions.h"
#include <cassert>

namespace gateway=::uwvm2::ros_gateway;
namespace ros=::uwvm2::imported::ros;
#ifdef UWVM_EXPECT_SOURCE_REFUSAL
constexpr bool expected=false;
#else
constexpr bool expected=true;
#endif

template<class Native,class Request,class Response>
void service(std::string_view name,std::string_view identity,std::string_view codec)
{
    using binding=gateway::ros2_custom_services::binding<Native,Request,Response>;
    ros::capability_spec cap;cap.type=std::string{name};cap.type_identity=std::string{identity};
    cap.operation="service.call";cap.limits["adapter-codec-identity"]=std::string{codec};
    assert(binding::known(cap,name,identity,codec)==expected);
    auto changed=cap;changed.type_identity.back()=changed.type_identity.back()=='0'?'1':'0';
    assert(!binding::known(changed,name,identity,codec));
    changed=cap;changed.operation="update";assert(!binding::known(changed,name,identity,codec));
    changed=cap;changed.limits.clear();assert(!binding::known(changed,name,identity,codec));
}
template<class Native,class Goal,class Result,class Feedback>
void action(std::string_view name,std::string_view identity,std::string_view codec)
{
    ros::capability_spec cap;cap.type=std::string{name};cap.type_identity=std::string{identity};
    cap.operation="action.send-goal";cap.limits["adapter-codec-identity"]=std::string{codec};
    assert((gateway::ros2_custom_actions::qualified<Native,Goal,Result,Feedback>(cap)==expected));
    auto changed=cap;changed.type_identity.back()=changed.type_identity.back()=='0'?'1':'0';
    assert((!gateway::ros2_custom_actions::qualified<Native,Goal,Result,Feedback>(changed)));
    changed=cap;changed.operation="debug";
    assert((!gateway::ros2_custom_actions::qualified<Native,Goal,Result,Feedback>(changed)));
    changed=cap;changed.operation="action.debug";
    assert((!gateway::ros2_custom_actions::qualified<Native,Goal,Result,Feedback>(changed)));
    changed=cap;changed.operation="action.update";
    assert((!gateway::ros2_custom_actions::qualified<Native,Goal,Result,Feedback>(changed)));
    changed=cap;changed.limits.clear();
    assert((!gateway::ros2_custom_actions::qualified<Native,Goal,Result,Feedback>(changed)));
    // This metadata-only project has not run the concrete Action lifecycle
    // availability probes. A successful schema proof must not enable it.
    static_assert(!UWVM_ROS2_HAVE_CUSTOM_ACTION,"use the separate actual lifecycle build probes");
    assert(!gateway::ros2_custom_actions::known(cap));
}
int main()
{
    unsigned services{},actions{};
#define UWVM_CHECK_SERVICE(Native,Name,Identity,Codec,Q,S) service<Native,Q,S>(Name,Identity,Codec);++services;
    UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(UWVM_CHECK_SERVICE)
#undef UWVM_CHECK_SERVICE
#define UWVM_CHECK_ACTION(Native,Name,Identity,Codec,G,R,F) action<Native,G,R,F>(Name,Identity,Codec);++actions;
    UWVM_ROS_CUSTOM_ROS2_ACTION_TYPES(UWVM_CHECK_ACTION)
#undef UWVM_CHECK_ACTION
    // A complete registered catalog may contain only one protocol family. KUKA,
    // for example, registers FollowJointTrajectory and vendor Messages while
    // retaining lifecycle Services as dependency-only definitions. Exercise all
    // registered bindings without requiring an unrelated public Service grant.
    assert(services||actions);
}
