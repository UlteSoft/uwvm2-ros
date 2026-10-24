/** @file Real installed-SDK Action metadata boundary contract.
 * No mock ActionSupport, native Message, generated SDK, hash or ROS endpoint.
 * The external actual process owner must prove source/SDK/compiler/fullMD/link/
 * ELF/mappings and original pidfds before releasing this program's SIGSTOP+I.
 * Actual Dashing example_interfaces and Humble action_tutorials_interfaces have
 * DIFFERENT Feedback field names. Selection preserves the original schema.
 */
#include "src/ros/gateway/ros2/compat/metadata_export.h"
#include "test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/native_protocol.h"
#ifndef UWVM_LEGACY_ACTION_EXAMPLE_INTERFACES
#error "Explicit actual native Fibonacci package selector required"
#endif
#if UWVM_LEGACY_ACTION_EXAMPLE_INTERFACES == 1
#include <example_interfaces/action/fibonacci.hpp>
using Native=::example_interfaces::action::Fibonacci;
static constexpr char root_name[]{"example_interfaces/action/Fibonacci"};
static constexpr char feedback_field[]{"sequence"};
#elif UWVM_LEGACY_ACTION_EXAMPLE_INTERFACES == 0
#include <action_tutorials_interfaces/action/fibonacci.hpp>
using Native=::action_tutorials_interfaces::action::Fibonacci;
static constexpr char root_name[]{"action_tutorials_interfaces/action/Fibonacci"};
static constexpr char feedback_field[]{"partial_sequence"};
#else
#error "Select exactly one real installed official Fibonacci package"
#endif
#include <array>
#include <cassert>
#include <cstdio>
#include <csignal>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#if !UWVM_ROS2_HAVE_OPENSSL || !UWVM_ROS2_HAVE_LEGACY_ACTION_TYPES || !UWVM_ROS2_HAVE_INTROSPECTION_CPP
#error "Actual Action/Message/Service introspection and linked OpenSSL required"
#endif
#ifdef NDEBUG
#error "Actual Action contract assertions must remain enabled"
#endif
namespace compat=::uwvm2::ros_gateway::ros2_compat;
namespace legacy=compat::legacy_metadata_export;
namespace ipc=::uwvm2::imported::ros::ipc;
using F=ipc::custom_native_field;using R=ipc::custom_native_record;
unsigned checks{};
void check(bool value) {++checks;assert(value);}

/** Owning strings and field arrays outlive every borrowed schema view. No
 * record is a replacement for native support: the helper compares the real
 * installed typed metadata independently, including its original capacities. */
struct Expected
{
    std::string root{root_name},goal{root+"_Goal"},result{root+"_Result"},feedback{root+"_Feedback"};
    std::string send{root+"_SendGoal"},get{root+"_GetResult"},wrapped{root+"_FeedbackMessage"};
    std::string srq{send+"_Request"},srp{send+"_Response"},sev{send+"_Event"};
    std::string grq{get+"_Request"},grp{get+"_Response"},gev{get+"_Event"};
    std::array<F,6> root_fields{{{"goal",goal},{"result",result},{"feedback",feedback},{"send_goal_service",send},{"get_result_service",get},{"feedback_message",wrapped}}};
    std::array<F,1> goal_fields{{{"order","int32"}}};
    std::array<F,1> result_fields{{{"sequence","int32",0,false,0,true,0}}};
    std::array<F,1> feedback_fields{{{feedback_field,"int32",0,false,0,true,0}}};
    std::array<F,3> send_fields{{{"request_message",srq},{"response_message",srp},{"event_message",sev}}};
    std::array<F,3> get_fields{{{"request_message",grq},{"response_message",grp},{"event_message",gev}}};
    std::array<F,2> send_request{{{"goal_id","unique_identifier_msgs/msg/UUID"},{"goal",goal}}};
    std::array<F,2> send_response{{{"accepted","bool"},{"stamp","builtin_interfaces/msg/Time"}}};
    std::array<F,1> get_request{{{"goal_id","unique_identifier_msgs/msg/UUID"}}};
    std::array<F,2> get_response{{{"status","int8"},{"result",result}}};
    std::array<F,2> feedback_message{{{"goal_id","unique_identifier_msgs/msg/UUID"},{"feedback",feedback}}};
    std::array<F,1> uuid{{{"uuid","uint8",16,true}}};
    std::array<F,2> time{{{"sec","int32"},{"nanosec","uint32"}}};
    std::array<F,4> event_info{{{"event_type","uint8"},{"stamp","builtin_interfaces/msg/Time"},{"client_gid","uint8",16,true},{"sequence_number","int64"}}};
    std::array<F,3> send_event{{{"info","service_msgs/msg/ServiceEventInfo"},{"request",srq,0,false,0,true,1},{"response",srp,0,false,0,true,1}}};
    std::array<F,3> get_event{{{"info","service_msgs/msg/ServiceEventInfo"},{"request",grq,0,false,0,true,1},{"response",grp,0,false,0,true,1}}};
    std::array<R,16> rows{{{root,root_fields.data(),6},{goal,goal_fields.data(),1},{result,result_fields.data(),1},{feedback,feedback_fields.data(),1},
        {send,send_fields.data(),3},{srq,send_request.data(),2},{srp,send_response.data(),2},{sev,send_event.data(),3},
        {get,get_fields.data(),3},{grq,get_request.data(),1},{grp,get_response.data(),2},{gev,get_event.data(),3},
        {wrapped,feedback_message.data(),2},{"unique_identifier_msgs/msg/UUID",uuid.data(),1},{"builtin_interfaces/msg/Time",time.data(),2},
        {"service_msgs/msg/ServiceEventInfo",event_info.data(),4}}};
    Expected()=default;Expected(Expected const&)=delete;Expected& operator=(Expected const&)=delete;
    ipc::custom_native_schema schema() const {return {rows.data(),rows.size()};}
};

bool refused(ipc::custom_native_schema graph,std::string_view root,ipc::custom_native_schema runtime)
{try {(void)legacy::action<Native>(graph,root,runtime);return false;}catch(std::runtime_error const&) {return true;}}

template<class Field> void declared_default_case(Expected& e,ipc::custom_native_schema runtime)
{
    if constexpr(legacy::has_declared_native_default<Field>::value)
    {
        auto rows=e.rows;auto fields=e.root_fields;Field value=fields[0];value.native_default="0.0";fields[0]=value;
        rows[0].fields=fields.data();check(refused({rows.data(),rows.size()},root_name,runtime));
    }
}

/** Warm every native support path before the mapping checkpoint. Dispatching
 * these statically selected handles loads metadata only, never a ROS Node. */
template<class Service> void warm_service()
{
    auto h=::rosidl_typesupport_cpp::get_service_type_support_handle<Service>();
    check(h&&h->typesupport_identifier);
    constexpr char id[]{"rosidl_typesupport_introspection_cpp"};
    auto actual=std::strcmp(h->typesupport_identifier,id)==0?h:(h->func?h->func(h,id):nullptr);
    check(actual&&actual->data);
    check(compat::message_members_for<typename Service::Request>()!=nullptr);
    check(compat::message_members_for<typename Service::Response>()!=nullptr);
}
int main()
{
    Expected expected;auto graph=expected.schema();auto runtime=legacy::action_runtime_schema();
    using Send=Native::Impl::SendGoalService;using Get=Native::Impl::GetResultService;
    using Cancel=Native::Impl::CancelGoalService;using Feedback=Native::Impl::FeedbackMessage;using Status=Native::Impl::GoalStatusMessage;
    auto support=::rosidl_typesupport_cpp::get_action_type_support_handle<Native>();check(support!=nullptr);
    check(support->goal_service_type_support==::rosidl_typesupport_cpp::get_service_type_support_handle<Send>());
    check(support->result_service_type_support==::rosidl_typesupport_cpp::get_service_type_support_handle<Get>());
    check(support->cancel_service_type_support==::rosidl_typesupport_cpp::get_service_type_support_handle<Cancel>());
    check(support->feedback_message_type_support==::rosidl_typesupport_cpp::get_message_type_support_handle<Feedback>());
    check(support->status_message_type_support==::rosidl_typesupport_cpp::get_message_type_support_handle<Status>());
    warm_service<Send>();warm_service<Get>();warm_service<Cancel>();
    check(compat::message_members_for<Native::Goal>()!=nullptr);check(compat::message_members_for<Native::Result>()!=nullptr);
    check(compat::message_members_for<Native::Feedback>()!=nullptr);check(compat::message_members_for<Feedback>()!=nullptr);
    check(compat::message_members_for<Status>()!=nullptr);check(compat::message_members_for<::unique_identifier_msgs::msg::UUID>()!=nullptr);
    check(compat::message_members_for<::builtin_interfaces::msg::Time>()!=nullptr);
    constexpr bool modern=compat::has_baked_hash<std::remove_pointer_t<decltype(support)>>::value;
    std::string metadata;
    if constexpr(modern) {check(refused(graph,root_name,runtime));}
    else {metadata=legacy::action<Native>(graph,root_name,runtime);check(!metadata.empty());}
    if(::raise(SIGSTOP)!=0) {throw std::runtime_error("native Action mapping stop failed");}
    bounded_native_io::gate('I');
    // This is a metadata/representation contract, not native RPC delivery.
    Native::Goal goal{};goal.order=19;check(goal.order==19);
    Native::Result result{};result.sequence={0,1,1,2,3};check(result.sequence.size()==5&&result.sequence[4]==3);
    Native::Feedback feedback{};
#if UWVM_LEGACY_ACTION_EXAMPLE_INTERFACES == 1
    feedback.sequence={0,1,1,2};check(feedback.sequence.size()==4&&feedback.sequence[3]==2);
#else
    feedback.partial_sequence={0,1,1,2};check(feedback.partial_sequence.size()==4&&feedback.partial_sequence[3]==2);
#endif
    if constexpr(!modern)
    {
        check(metadata.find("LEGACY_NATIVE_ACTION_WITH_STANDARD_PROTOCOL_DERIVED_RIHS01")!=std::string::npos);
        check(metadata.find("\"action_type_support_associations_verified\": true")!=std::string::npos);
        check(metadata.find("\"standard_protocol\": \"ROS2_ACTION_EVENT_V1\"")!=std::string::npos);
        for(auto role:{"send_goal","get_result","feedback_message","cancel_goal","status","uuid","time"})
        {check(metadata.find(std::string{"\""}+role+"\": {")!=std::string::npos);}
        check(metadata.find(compat::rihs_identity(graph,root_name))!=std::string::npos);
    }
    check(refused({},root_name,runtime));check(refused(graph,"different/action/Fibonacci",runtime));check(refused(graph,root_name,{}));
    for(std::size_t i{};i!=graph.record_count;++i)
    {
        auto rows=expected.rows;std::vector<F> fields(rows[i].fields,rows[i].fields+rows[i].field_count);
        fields[0].name="wrong_field";rows[i].fields=fields.data();check(refused({rows.data(),rows.size()},root_name,runtime));
    }
    std::array<R,17> duplicate{};for(std::size_t i{};i!=expected.rows.size();++i) {duplicate[i]=expected.rows[i];}
    duplicate[16]=expected.rows[0];check(refused({duplicate.data(),duplicate.size()},root_name,runtime));
    check(refused({graph.records,graph.record_count-1},root_name,runtime));
    for(std::size_t i{};i!=runtime.record_count;++i)
    {
        std::vector<R> rows(runtime.records,runtime.records+runtime.record_count);
        std::vector<F> fields(rows[i].fields,rows[i].fields+rows[i].field_count);fields[0].name="wrong_protocol";
        rows[i].fields=fields.data();check(refused(graph,root_name,{rows.data(),rows.size()}));
    }
    declared_default_case<F>(expected,runtime);
    std::printf("PASS_LEGACY_ACTION_NATIVE_CONTRACT sdk=%s roots=1 checks=%u ROS_entities=0 delivery_qualified=0\n",modern?"modern-refusal":"legacy-native",checks);
    return 0;
}
