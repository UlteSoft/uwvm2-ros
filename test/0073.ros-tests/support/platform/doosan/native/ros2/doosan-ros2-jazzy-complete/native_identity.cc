/** Industrial native metadata source; no Node, endpoint or grant. */
#include "test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/native_protocol.h"
#include <array>
#include <string_view>
#include <stdexcept>
#include <csignal>
#include <cstdint>
#if UWVM_CUSTOM_NATIVE_ROS_VERSION != 2
#error "Select the exact source-reviewed ROS major"
#endif
#include <dsr_msgs2/action/jog_h2r.hpp>
#include <dsr_msgs2/action/movej_h2r.hpp>
#include <dsr_msgs2/action/movel_h2r.hpp>
#include <dsr_msgs2/msg/alter_motion_stream.hpp>
#include <dsr_msgs2/msg/jog_multi_axis.hpp>
#include <dsr_msgs2/msg/log_alarm.hpp>
#include <dsr_msgs2/msg/modbus_state.hpp>
#include <dsr_msgs2/msg/robot_disconnection.hpp>
#include <dsr_msgs2/msg/robot_error.hpp>
#include <dsr_msgs2/msg/robot_state.hpp>
#include <dsr_msgs2/msg/robot_state_rt.hpp>
#include <dsr_msgs2/msg/robot_stop.hpp>
#include <dsr_msgs2/msg/servoj_rt_stream.hpp>
#include <dsr_msgs2/msg/servoj_stream.hpp>
#include <dsr_msgs2/msg/servol_rt_stream.hpp>
#include <dsr_msgs2/msg/servol_stream.hpp>
#include <dsr_msgs2/msg/speedj_rt_stream.hpp>
#include <dsr_msgs2/msg/speedj_stream.hpp>
#include <dsr_msgs2/msg/speedl_rt_stream.hpp>
#include <dsr_msgs2/msg/speedl_stream.hpp>
#include <dsr_msgs2/msg/torque_rt_stream.hpp>
#include <dsr_msgs2/srv/align_axis1.hpp>
#include <dsr_msgs2/srv/align_axis2.hpp>
#include <dsr_msgs2/srv/alter_motion.hpp>
#include <dsr_msgs2/srv/calc_coord.hpp>
#include <dsr_msgs2/srv/change_collision_sensitivity.hpp>
#include <dsr_msgs2/srv/change_operation_speed.hpp>
#include <dsr_msgs2/srv/check_force_condition.hpp>
#include <dsr_msgs2/srv/check_motion.hpp>
#include <dsr_msgs2/srv/check_orientation_condition1.hpp>
#include <dsr_msgs2/srv/check_orientation_condition2.hpp>
#include <dsr_msgs2/srv/check_position_condition.hpp>
#include <dsr_msgs2/srv/config_create_modbus.hpp>
#include <dsr_msgs2/srv/config_create_tcp.hpp>
#include <dsr_msgs2/srv/config_create_tool.hpp>
#include <dsr_msgs2/srv/config_delete_modbus.hpp>
#include <dsr_msgs2/srv/config_delete_tcp.hpp>
#include <dsr_msgs2/srv/config_delete_tool.hpp>
#include <dsr_msgs2/srv/connect_rt_control.hpp>
#include <dsr_msgs2/srv/coord_transform.hpp>
#include <dsr_msgs2/srv/disable_alter_motion.hpp>
#include <dsr_msgs2/srv/disconnect_rt_control.hpp>
#include <dsr_msgs2/srv/drl_pause.hpp>
#include <dsr_msgs2/srv/drl_resume.hpp>
#include <dsr_msgs2/srv/drl_start.hpp>
#include <dsr_msgs2/srv/drl_stop.hpp>
#include <dsr_msgs2/srv/enable_alter_motion.hpp>
#include <dsr_msgs2/srv/fkin.hpp>
#include <dsr_msgs2/srv/get_control_mode.hpp>
#include <dsr_msgs2/srv/get_control_space.hpp>
#include <dsr_msgs2/srv/get_ctrl_box_analog_input.hpp>
#include <dsr_msgs2/srv/get_ctrl_box_digital_input.hpp>
#include <dsr_msgs2/srv/get_ctrl_box_digital_output.hpp>
#include <dsr_msgs2/srv/get_current_pose.hpp>
#include <dsr_msgs2/srv/get_current_posj.hpp>
#include <dsr_msgs2/srv/get_current_posx.hpp>
#include <dsr_msgs2/srv/get_current_rotm.hpp>
#include <dsr_msgs2/srv/get_current_solution_space.hpp>
#include <dsr_msgs2/srv/get_current_tcp.hpp>
#include <dsr_msgs2/srv/get_current_tool.hpp>
#include <dsr_msgs2/srv/get_current_tool_flange_posx.hpp>
#include <dsr_msgs2/srv/get_current_velj.hpp>
#include <dsr_msgs2/srv/get_current_velx.hpp>
#include <dsr_msgs2/srv/get_desired_posj.hpp>
#include <dsr_msgs2/srv/get_desired_posx.hpp>
#include <dsr_msgs2/srv/get_desired_velj.hpp>
#include <dsr_msgs2/srv/get_desired_velx.hpp>
#include <dsr_msgs2/srv/get_drl_state.hpp>
#include <dsr_msgs2/srv/get_external_torque.hpp>
#include <dsr_msgs2/srv/get_input_register_bit.hpp>
#include <dsr_msgs2/srv/get_input_register_float.hpp>
#include <dsr_msgs2/srv/get_input_register_int.hpp>
#include <dsr_msgs2/srv/get_joint_torque.hpp>
#include <dsr_msgs2/srv/get_last_alarm.hpp>
#include <dsr_msgs2/srv/get_modbus_input.hpp>
#include <dsr_msgs2/srv/get_orientation_error.hpp>
#include <dsr_msgs2/srv/get_output_register_bit.hpp>
#include <dsr_msgs2/srv/get_output_register_float.hpp>
#include <dsr_msgs2/srv/get_output_register_int.hpp>
#include <dsr_msgs2/srv/get_robot_link_info.hpp>
#include <dsr_msgs2/srv/get_robot_mode.hpp>
#include <dsr_msgs2/srv/get_robot_speed_mode.hpp>
#include <dsr_msgs2/srv/get_robot_state.hpp>
#include <dsr_msgs2/srv/get_robot_system.hpp>
#include <dsr_msgs2/srv/get_rt_control_input_data_list.hpp>
#include <dsr_msgs2/srv/get_rt_control_input_version_list.hpp>
#include <dsr_msgs2/srv/get_rt_control_output_data_list.hpp>
#include <dsr_msgs2/srv/get_rt_control_output_version_list.hpp>
#include <dsr_msgs2/srv/get_solution_space.hpp>
#include <dsr_msgs2/srv/get_tool_digital_input.hpp>
#include <dsr_msgs2/srv/get_tool_digital_output.hpp>
#include <dsr_msgs2/srv/get_tool_force.hpp>
#include <dsr_msgs2/srv/get_user_cart_coord.hpp>
#include <dsr_msgs2/srv/get_workpiece_weight.hpp>
#include <dsr_msgs2/srv/ikin.hpp>
#include <dsr_msgs2/srv/is_done_bolt_tightening.hpp>
#include <dsr_msgs2/srv/jog.hpp>
#include <dsr_msgs2/srv/jog_multi.hpp>
#include <dsr_msgs2/srv/move_blending.hpp>
#include <dsr_msgs2/srv/move_circle.hpp>
#include <dsr_msgs2/srv/move_home.hpp>
#include <dsr_msgs2/srv/move_joint.hpp>
#include <dsr_msgs2/srv/move_jointx.hpp>
#include <dsr_msgs2/srv/move_line.hpp>
#include <dsr_msgs2/srv/move_pause.hpp>
#include <dsr_msgs2/srv/move_periodic.hpp>
#include <dsr_msgs2/srv/move_resume.hpp>
#include <dsr_msgs2/srv/move_spiral.hpp>
#include <dsr_msgs2/srv/move_spline_joint.hpp>
#include <dsr_msgs2/srv/move_spline_task.hpp>
#include <dsr_msgs2/srv/move_stop.hpp>
#include <dsr_msgs2/srv/move_wait.hpp>
#include <dsr_msgs2/srv/overwrite_user_cart_coord.hpp>
#include <dsr_msgs2/srv/parallel_axis1.hpp>
#include <dsr_msgs2/srv/parallel_axis2.hpp>
#include <dsr_msgs2/srv/read_data_rt.hpp>
#include <dsr_msgs2/srv/release_compliance_ctrl.hpp>
#include <dsr_msgs2/srv/release_force.hpp>
#include <dsr_msgs2/srv/reset_workpiece_weight.hpp>
#include <dsr_msgs2/srv/robotiq2_f_close.hpp>
#include <dsr_msgs2/srv/robotiq2_f_move.hpp>
#include <dsr_msgs2/srv/robotiq2_f_open.hpp>
#include <dsr_msgs2/srv/serial_send_data.hpp>
#include <dsr_msgs2/srv/servo_off.hpp>
#include <dsr_msgs2/srv/set_accj_rt.hpp>
#include <dsr_msgs2/srv/set_accx_rt.hpp>
#include <dsr_msgs2/srv/set_ctrl_box_analog_input_type.hpp>
#include <dsr_msgs2/srv/set_ctrl_box_analog_output.hpp>
#include <dsr_msgs2/srv/set_ctrl_box_analog_output_type.hpp>
#include <dsr_msgs2/srv/set_ctrl_box_digital_output.hpp>
#include <dsr_msgs2/srv/set_current_tcp.hpp>
#include <dsr_msgs2/srv/set_current_tool.hpp>
#include <dsr_msgs2/srv/set_desired_force.hpp>
#include <dsr_msgs2/srv/set_modbus_output.hpp>
#include <dsr_msgs2/srv/set_output_register_bit.hpp>
#include <dsr_msgs2/srv/set_output_register_float.hpp>
#include <dsr_msgs2/srv/set_output_register_int.hpp>
#include <dsr_msgs2/srv/set_ref_coord.hpp>
#include <dsr_msgs2/srv/set_robot_control.hpp>
#include <dsr_msgs2/srv/set_robot_mode.hpp>
#include <dsr_msgs2/srv/set_robot_speed_mode.hpp>
#include <dsr_msgs2/srv/set_robot_system.hpp>
#include <dsr_msgs2/srv/set_rt_control_input.hpp>
#include <dsr_msgs2/srv/set_rt_control_output.hpp>
#include <dsr_msgs2/srv/set_safe_stop_reset_type.hpp>
#include <dsr_msgs2/srv/set_safety_mode.hpp>
#include <dsr_msgs2/srv/set_singular_handling_force.hpp>
#include <dsr_msgs2/srv/set_singularity_handling.hpp>
#include <dsr_msgs2/srv/set_stiffnessx.hpp>
#include <dsr_msgs2/srv/set_tool_digital_output.hpp>
#include <dsr_msgs2/srv/set_tool_shape.hpp>
#include <dsr_msgs2/srv/set_user_cart_coord1.hpp>
#include <dsr_msgs2/srv/set_user_cart_coord2.hpp>
#include <dsr_msgs2/srv/set_user_cart_coord3.hpp>
#include <dsr_msgs2/srv/set_velj_rt.hpp>
#include <dsr_msgs2/srv/set_velx_rt.hpp>
#include <dsr_msgs2/srv/start_rt_control.hpp>
#include <dsr_msgs2/srv/stop_rt_control.hpp>
#include <dsr_msgs2/srv/task_compliance_ctrl.hpp>
#include <dsr_msgs2/srv/trans.hpp>
#include <dsr_msgs2/srv/write_data_rt.hpp>
#include <std_msgs/msg/float64_multi_array.hpp>
#include <std_msgs/msg/multi_array_dimension.hpp>
#include <std_msgs/msg/multi_array_layout.hpp>
#include <rosidl_typesupport_cpp/message_type_support.hpp>
#include <rosidl_typesupport_cpp/service_type_support.hpp>
#include <rosidl_typesupport_cpp/action_type_support.hpp>
#include "native_constants.hpp"
namespace
{
    int output() {return STDOUT_FILENO;}
    /** Emit bounded JSON text with checked Unicode metadata supplied by ROS.
     * Escape ASCII controls and quotes; no arbitrary filename or code is loaded. */
    void quoted(std::string_view value)
    {
        if(value.size()>1048576) {throw std::length_error("native metadata ceiling");}
        size_t length{2};for(unsigned char c:value) {length+=c<32?6:c=='"'||c=='\\'?2:1;}std::string escaped;escaped.reserve(length);escaped.push_back('"');constexpr char digits[]{"0123456789abcdef"};
        for(unsigned char c:value)
        {
            if(c=='"'||c=='\\') {escaped.push_back('\\');escaped.push_back(static_cast<char>(c));}
            else if(c<32) {for(char prefix:std::string_view{"\\u00"}) {escaped.push_back(prefix);}escaped.push_back(digits[c>>4]);escaped.push_back(digits[c&15]);}
            else {escaped.push_back(static_cast<char>(c));}
        }
        escaped.push_back('"');bounded_native_io::emit(output(),escaped);
    }
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    template<class Message> void component(char const* role,bool comma)
    {
        if(comma) {bounded_native_io::print(output(),',');}quoted(role);bounded_native_io::print(output(),":{");
        bounded_native_io::print(output(),"\"type\":");quoted(::ros::message_traits::DataType<Message>::value());
        bounded_native_io::print(output(),",\"native_identity\":");quoted(::ros::message_traits::MD5Sum<Message>::value());
        bounded_native_io::print(output(),",\"definition\":");quoted(::ros::message_traits::Definition<Message>::value());bounded_native_io::print(output(),'}');
    }
    template<class Message> void message(char const* name,bool comma)
    {if(comma) {bounded_native_io::print(output(),',');}quoted(name);bounded_native_io::print(output(),":{\"native_identity\":");quoted(::ros::message_traits::MD5Sum<Message>::value());bounded_native_io::print(output(),",\"components\":{");component<Message>("message",false);bounded_native_io::print(output(),"}}");}
    template<class Service> void service(char const* name)
    {bounded_native_io::print(output(),',');quoted(name);bounded_native_io::print(output(),":{\"native_identity\":");quoted(::ros::service_traits::MD5Sum<Service>::value());bounded_native_io::print(output(),",\"official_type\":");quoted(::ros::service_traits::DataType<Service>::value());bounded_native_io::print(output(),",\"components\":{");component<typename Service::Request>("request",false);component<typename Service::Response>("response",true);bounded_native_io::print(output(),"}}");}
#else
    /** Official static borrowed description strings may report capacity equal
     * to size, excluding the separately emitted NUL. Check that actual NUL,
     * pointer and finite length; this is not the Guest string admission rule. */
    template<class Text> std::string_view text(Text const& value)
    {if(value.size>1048576||value.size>value.capacity||!value.data||value.data[value.size]!=0) {throw std::runtime_error("invalid official metadata text");}return {value.data,value.size};}
    void record(rosidl_runtime_c__type_description__IndividualTypeDescription const& value)
    {
        if(value.fields.size>4096||value.fields.size>value.fields.capacity||(value.fields.size&&!value.fields.data)) {throw std::runtime_error("native field table ceiling");}
        bounded_native_io::print(output(),"{\"type\":");quoted(text(value.type_name));bounded_native_io::print(output(),",\"fields\":[");
        for(size_t i{};i!=value.fields.size;++i)
        {auto const& f{value.fields.data[i]};if(i) {bounded_native_io::print(output(),',');}bounded_native_io::print(output(),"{\"name\":");quoted(text(f.name));bounded_native_io::print(output(),",\"type_id\":",static_cast<unsigned>(f.type.type_id),",\"capacity\":",f.type.capacity,",\"string_capacity\":",f.type.string_capacity,",\"nested_type_name\":");
            if(!f.type.nested_type_name.data&&!f.type.nested_type_name.size&&!f.type.nested_type_name.capacity) {quoted("");}else {quoted(text(f.type.nested_type_name));}
            bounded_native_io::print(output(),",\"default_value\":");if(!f.default_value.data&&!f.default_value.size&&!f.default_value.capacity) {quoted("");}else {quoted(text(f.default_value));}bounded_native_io::print(output(),'}');}
        bounded_native_io::print(output(),"]}");
    }
    template<class Support> void metadata(Support const* support)
    {
        if(!support||!support->get_type_hash_func||!support->get_type_description_func) {throw std::runtime_error("official metadata unavailable");}
        auto hash{support->get_type_hash_func(support)};auto description{support->get_type_description_func(support)};
        if(!hash||hash->version!=1||!description||description->referenced_type_descriptions.size>512||description->referenced_type_descriptions.size>description->referenced_type_descriptions.capacity||(description->referenced_type_descriptions.size&&!description->referenced_type_descriptions.data)) {throw std::runtime_error("official type description ceiling");}
        std::array<char,71> identity{};constexpr char hex[]{"0123456789abcdef"};constexpr char prefix[]{"RIHS01_"};for(unsigned i{};i!=7;++i) {identity[i]=prefix[i];}for(unsigned i{};i!=32;++i) {identity[7+2*i]=hex[hash->value[i]>>4];identity[8+2*i]=hex[hash->value[i]&15];}
        bounded_native_io::print(output(),"{\"native_identity\":");quoted({identity.data(),identity.size()});bounded_native_io::print(output(),",\"root\":");record(description->type_description);bounded_native_io::print(output(),",\"references\":[");for(size_t i{};i!=description->referenced_type_descriptions.size;++i) {if(i) {bounded_native_io::print(output(),',');}record(description->referenced_type_descriptions.data[i]);}bounded_native_io::print(output(),"]}");
    }
    template<class Message> void component(char const* name,bool comma)
    {if(comma) {bounded_native_io::print(output(),',');}quoted(name);bounded_native_io::print(output(),':');metadata(rosidl_typesupport_cpp::get_message_type_support_handle<Message>());}
    template<class Message> void message(char const* name,bool comma)
    {if(comma) {bounded_native_io::print(output(),',');}quoted(name);bounded_native_io::print(output(),":{\"official\":");metadata(rosidl_typesupport_cpp::get_message_type_support_handle<Message>());bounded_native_io::print(output(),",\"components\":{");component<Message>("message",false);bounded_native_io::print(output(),"}}");}
    template<class Service> void service(char const* name)
    {bounded_native_io::print(output(),',');quoted(name);bounded_native_io::print(output(),":{\"official\":");metadata(rosidl_typesupport_cpp::get_service_type_support_handle<Service>());bounded_native_io::print(output(),",\"components\":{");component<typename Service::Request>("request",false);component<typename Service::Response>("response",true);bounded_native_io::print(output(),"}}");}
#endif
}
namespace {template<class S> void industrial_warm(S const* s){if(!s||!s->get_type_hash_func||!s->get_type_description_func)throw std::runtime_error{"modern native metadata unavailable"};auto h=s->get_type_hash_func(s);auto d=s->get_type_description_func(s);if(!h||h->version!=1||!d)throw std::runtime_error{"native metadata warm failed"};}}

int main() {try {
if(!::uwvm_industrial_source_914fbb8df370c81fa64abb00a596b8c0fa1a71711765bf0ab2c7bd79d074f03f::validate())throw std::runtime_error{"native source constants differ"};
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::dsr_msgs2::action::JogH2r>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::action::JogH2r::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::action::JogH2r::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::action::JogH2r::Feedback>());
auto a0=rosidl_typesupport_cpp::get_action_type_support_handle<::dsr_msgs2::action::JogH2r>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::JogH2r::Impl::SendGoalService>());if(a0->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::JogH2r::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::JogH2r::Impl::GetResultService>());if(a0->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::JogH2r::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::JogH2r::Impl::CancelGoalService>());if(a0->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::JogH2r::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::dsr_msgs2::action::JogH2r::Impl::FeedbackMessage>());if(a0->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::dsr_msgs2::action::JogH2r::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::dsr_msgs2::action::JogH2r::Impl::GoalStatusMessage>());if(a0->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::dsr_msgs2::action::JogH2r::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::dsr_msgs2::action::MovejH2r>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::action::MovejH2r::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::action::MovejH2r::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::action::MovejH2r::Feedback>());
auto a1=rosidl_typesupport_cpp::get_action_type_support_handle<::dsr_msgs2::action::MovejH2r>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::MovejH2r::Impl::SendGoalService>());if(a1->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::MovejH2r::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::MovejH2r::Impl::GetResultService>());if(a1->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::MovejH2r::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::MovejH2r::Impl::CancelGoalService>());if(a1->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::MovejH2r::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::dsr_msgs2::action::MovejH2r::Impl::FeedbackMessage>());if(a1->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::dsr_msgs2::action::MovejH2r::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::dsr_msgs2::action::MovejH2r::Impl::GoalStatusMessage>());if(a1->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::dsr_msgs2::action::MovejH2r::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_action_type_support_handle<::dsr_msgs2::action::MovelH2r>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::action::MovelH2r::Goal>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::action::MovelH2r::Result>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::action::MovelH2r::Feedback>());
auto a2=rosidl_typesupport_cpp::get_action_type_support_handle<::dsr_msgs2::action::MovelH2r>();
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::MovelH2r::Impl::SendGoalService>());if(a2->goal_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::MovelH2r::Impl::SendGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::MovelH2r::Impl::GetResultService>());if(a2->result_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::MovelH2r::Impl::GetResultService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::MovelH2r::Impl::CancelGoalService>());if(a2->cancel_service_type_support!=rosidl_typesupport_cpp::get_service_type_support_handle<typename ::dsr_msgs2::action::MovelH2r::Impl::CancelGoalService>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::dsr_msgs2::action::MovelH2r::Impl::FeedbackMessage>());if(a2->feedback_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::dsr_msgs2::action::MovelH2r::Impl::FeedbackMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<typename ::dsr_msgs2::action::MovelH2r::Impl::GoalStatusMessage>());if(a2->status_message_type_support!=rosidl_typesupport_cpp::get_message_type_support_handle<typename ::dsr_msgs2::action::MovelH2r::Impl::GoalStatusMessage>())throw std::runtime_error{"actual five-link Action association differs"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::AlterMotionStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::AlterMotionStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::JogMultiAxis>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::JogMultiAxis>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::LogAlarm>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::LogAlarm>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::ModbusState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::ModbusState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::RobotDisconnection>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::RobotDisconnection>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::RobotError>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::RobotError>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::RobotState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::RobotState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::RobotStateRt>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::RobotStateRt>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::RobotStop>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::RobotStop>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::ServojRtStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::ServojRtStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::ServojStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::ServojStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::ServolRtStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::ServolRtStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::ServolStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::ServolStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::SpeedjRtStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::SpeedjRtStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::SpeedjStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::SpeedjStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::SpeedlRtStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::SpeedlRtStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::SpeedlStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::SpeedlStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::TorqueRtStream>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::TorqueRtStream>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::AlignAxis1>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::AlignAxis1::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::AlignAxis1::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::AlignAxis2>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::AlignAxis2::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::AlignAxis2::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::AlterMotion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::AlterMotion::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::AlterMotion::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::CalcCoord>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::CalcCoord::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::CalcCoord::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ChangeCollisionSensitivity>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ChangeCollisionSensitivity::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ChangeCollisionSensitivity::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ChangeOperationSpeed>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ChangeOperationSpeed::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ChangeOperationSpeed::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::CheckForceCondition>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::CheckForceCondition::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::CheckForceCondition::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::CheckMotion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::CheckMotion::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::CheckMotion::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::CheckOrientationCondition1>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::CheckOrientationCondition1::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::CheckOrientationCondition1::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::CheckOrientationCondition2>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::CheckOrientationCondition2::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::CheckOrientationCondition2::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::CheckPositionCondition>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::CheckPositionCondition::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::CheckPositionCondition::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ConfigCreateModbus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ConfigCreateModbus::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ConfigCreateModbus::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ConfigCreateTcp>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ConfigCreateTcp::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ConfigCreateTcp::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ConfigCreateTool>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ConfigCreateTool::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ConfigCreateTool::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ConfigDeleteModbus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ConfigDeleteModbus::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ConfigDeleteModbus::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ConfigDeleteTcp>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ConfigDeleteTcp::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ConfigDeleteTcp::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ConfigDeleteTool>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ConfigDeleteTool::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ConfigDeleteTool::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ConnectRtControl>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ConnectRtControl::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ConnectRtControl::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::CoordTransform>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::CoordTransform::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::CoordTransform::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::DisableAlterMotion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::DisableAlterMotion::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::DisableAlterMotion::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::DisconnectRtControl>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::DisconnectRtControl::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::DisconnectRtControl::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::DrlPause>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::DrlPause::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::DrlPause::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::DrlResume>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::DrlResume::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::DrlResume::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::DrlStart>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::DrlStart::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::DrlStart::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::DrlStop>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::DrlStop::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::DrlStop::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::EnableAlterMotion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::EnableAlterMotion::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::EnableAlterMotion::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::Fkin>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::Fkin::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::Fkin::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetControlMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetControlMode::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetControlMode::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetControlSpace>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetControlSpace::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetControlSpace::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCtrlBoxAnalogInput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCtrlBoxAnalogInput::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCtrlBoxAnalogInput::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCtrlBoxDigitalInput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCtrlBoxDigitalInput::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCtrlBoxDigitalInput::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCtrlBoxDigitalOutput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCtrlBoxDigitalOutput::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCtrlBoxDigitalOutput::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentPose>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentPose::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentPose::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentPosj>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentPosj::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentPosj::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentPosx>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentPosx::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentPosx::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentRotm>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentRotm::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentRotm::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentSolutionSpace>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentSolutionSpace::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentSolutionSpace::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentTcp>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentTcp::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentTcp::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentTool>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentTool::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentTool::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentToolFlangePosx>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentToolFlangePosx::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentToolFlangePosx::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentVelj>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentVelj::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentVelj::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentVelx>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentVelx::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetCurrentVelx::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetDesiredPosj>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetDesiredPosj::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetDesiredPosj::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetDesiredPosx>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetDesiredPosx::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetDesiredPosx::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetDesiredVelj>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetDesiredVelj::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetDesiredVelj::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetDesiredVelx>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetDesiredVelx::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetDesiredVelx::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetDrlState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetDrlState::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetDrlState::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetExternalTorque>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetExternalTorque::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetExternalTorque::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetInputRegisterBit>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetInputRegisterBit::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetInputRegisterBit::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetInputRegisterFloat>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetInputRegisterFloat::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetInputRegisterFloat::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetInputRegisterInt>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetInputRegisterInt::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetInputRegisterInt::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetJointTorque>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetJointTorque::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetJointTorque::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetLastAlarm>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetLastAlarm::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetLastAlarm::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetModbusInput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetModbusInput::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetModbusInput::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetOrientationError>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetOrientationError::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetOrientationError::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetOutputRegisterBit>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetOutputRegisterBit::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetOutputRegisterBit::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetOutputRegisterFloat>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetOutputRegisterFloat::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetOutputRegisterFloat::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetOutputRegisterInt>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetOutputRegisterInt::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetOutputRegisterInt::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRobotLinkInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRobotLinkInfo::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRobotLinkInfo::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRobotMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRobotMode::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRobotMode::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRobotSpeedMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRobotSpeedMode::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRobotSpeedMode::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRobotState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRobotState::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRobotState::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRobotSystem>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRobotSystem::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRobotSystem::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRtControlInputDataList>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRtControlInputDataList::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRtControlInputDataList::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRtControlInputVersionList>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRtControlInputVersionList::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRtControlInputVersionList::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRtControlOutputDataList>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRtControlOutputDataList::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRtControlOutputDataList::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRtControlOutputVersionList>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRtControlOutputVersionList::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetRtControlOutputVersionList::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetSolutionSpace>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetSolutionSpace::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetSolutionSpace::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetToolDigitalInput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetToolDigitalInput::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetToolDigitalInput::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetToolDigitalOutput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetToolDigitalOutput::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetToolDigitalOutput::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetToolForce>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetToolForce::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetToolForce::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetUserCartCoord>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetUserCartCoord::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetUserCartCoord::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetWorkpieceWeight>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetWorkpieceWeight::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::GetWorkpieceWeight::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::Ikin>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::Ikin::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::Ikin::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::IsDoneBoltTightening>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::IsDoneBoltTightening::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::IsDoneBoltTightening::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::Jog>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::Jog::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::Jog::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::JogMulti>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::JogMulti::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::JogMulti::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveBlending>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveBlending::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveBlending::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveCircle>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveCircle::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveCircle::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveHome>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveHome::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveHome::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveJoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveJoint::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveJoint::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveJointx>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveJointx::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveJointx::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveLine>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveLine::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveLine::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MovePause>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MovePause::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MovePause::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MovePeriodic>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MovePeriodic::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MovePeriodic::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveResume>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveResume::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveResume::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveSpiral>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveSpiral::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveSpiral::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveSplineJoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveSplineJoint::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveSplineJoint::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveSplineTask>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveSplineTask::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveSplineTask::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveStop>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveStop::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveStop::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveWait>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveWait::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::MoveWait::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::OverwriteUserCartCoord>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::OverwriteUserCartCoord::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::OverwriteUserCartCoord::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ParallelAxis1>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ParallelAxis1::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ParallelAxis1::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ParallelAxis2>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ParallelAxis2::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ParallelAxis2::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ReadDataRt>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ReadDataRt::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ReadDataRt::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ReleaseComplianceCtrl>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ReleaseComplianceCtrl::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ReleaseComplianceCtrl::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ReleaseForce>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ReleaseForce::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ReleaseForce::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ResetWorkpieceWeight>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ResetWorkpieceWeight::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ResetWorkpieceWeight::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::Robotiq2FClose>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::Robotiq2FClose::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::Robotiq2FClose::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::Robotiq2FMove>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::Robotiq2FMove::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::Robotiq2FMove::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::Robotiq2FOpen>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::Robotiq2FOpen::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::Robotiq2FOpen::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SerialSendData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SerialSendData::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SerialSendData::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ServoOff>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ServoOff::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::ServoOff::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetAccjRt>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetAccjRt::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetAccjRt::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetAccxRt>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetAccxRt::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetAccxRt::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetCtrlBoxAnalogInputType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetCtrlBoxAnalogInputType::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetCtrlBoxAnalogInputType::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetCtrlBoxAnalogOutput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetCtrlBoxAnalogOutput::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetCtrlBoxAnalogOutput::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetCtrlBoxAnalogOutputType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetCtrlBoxAnalogOutputType::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetCtrlBoxAnalogOutputType::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetCtrlBoxDigitalOutput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetCtrlBoxDigitalOutput::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetCtrlBoxDigitalOutput::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetCurrentTcp>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetCurrentTcp::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetCurrentTcp::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetCurrentTool>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetCurrentTool::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetCurrentTool::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetDesiredForce>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetDesiredForce::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetDesiredForce::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetModbusOutput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetModbusOutput::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetModbusOutput::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetOutputRegisterBit>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetOutputRegisterBit::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetOutputRegisterBit::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetOutputRegisterFloat>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetOutputRegisterFloat::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetOutputRegisterFloat::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetOutputRegisterInt>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetOutputRegisterInt::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetOutputRegisterInt::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetRefCoord>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetRefCoord::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetRefCoord::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetRobotControl>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetRobotControl::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetRobotControl::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetRobotMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetRobotMode::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetRobotMode::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetRobotSpeedMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetRobotSpeedMode::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetRobotSpeedMode::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetRobotSystem>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetRobotSystem::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetRobotSystem::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetRtControlInput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetRtControlInput::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetRtControlInput::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetRtControlOutput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetRtControlOutput::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetRtControlOutput::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetSafeStopResetType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetSafeStopResetType::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetSafeStopResetType::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetSafetyMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetSafetyMode::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetSafetyMode::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetSingularHandlingForce>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetSingularHandlingForce::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetSingularHandlingForce::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetSingularityHandling>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetSingularityHandling::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetSingularityHandling::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetStiffnessx>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetStiffnessx::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetStiffnessx::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetToolDigitalOutput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetToolDigitalOutput::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetToolDigitalOutput::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetToolShape>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetToolShape::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetToolShape::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetUserCartCoord1>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetUserCartCoord1::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetUserCartCoord1::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetUserCartCoord2>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetUserCartCoord2::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetUserCartCoord2::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetUserCartCoord3>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetUserCartCoord3::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetUserCartCoord3::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetVeljRt>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetVeljRt::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetVeljRt::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetVelxRt>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetVelxRt::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::SetVelxRt::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::StartRtControl>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::StartRtControl::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::StartRtControl::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::StopRtControl>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::StopRtControl::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::StopRtControl::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::TaskComplianceCtrl>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::TaskComplianceCtrl::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::TaskComplianceCtrl::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::Trans>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::Trans::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::Trans::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::WriteDataRt>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::WriteDataRt::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::srv::WriteDataRt::Response>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Float64MultiArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Float64MultiArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::MultiArrayDimension>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::MultiArrayDimension>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::MultiArrayLayout>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::MultiArrayLayout>());
if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};
bounded_native_io::gate('I');
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"914fbb8df370c81fa64abb00a596b8c0fa1a71711765bf0ab2c7bd79d074f03f\",\"types\":{");
quoted("dsr_msgs2/action/JogH2r");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::dsr_msgs2::action::JogH2r>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::action::JogH2r::Goal>("goal",false);
component<::dsr_msgs2::action::JogH2r::Result>("result",true);
component<::dsr_msgs2::action::JogH2r::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/action/MovejH2r");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::dsr_msgs2::action::MovejH2r>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::action::MovejH2r::Goal>("goal",false);
component<::dsr_msgs2::action::MovejH2r::Result>("result",true);
component<::dsr_msgs2::action::MovejH2r::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/action/MovelH2r");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_action_type_support_handle<::dsr_msgs2::action::MovelH2r>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::action::MovelH2r::Goal>("goal",false);
component<::dsr_msgs2::action::MovelH2r::Result>("result",true);
component<::dsr_msgs2::action::MovelH2r::Feedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/AlterMotionStream");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::AlterMotionStream>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::AlterMotionStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/JogMultiAxis");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::JogMultiAxis>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::JogMultiAxis>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/LogAlarm");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::LogAlarm>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::LogAlarm>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/ModbusState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::ModbusState>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::ModbusState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/RobotDisconnection");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::RobotDisconnection>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::RobotDisconnection>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/RobotError");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::RobotError>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::RobotError>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/RobotState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::RobotState>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::RobotState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/RobotStateRt");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::RobotStateRt>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::RobotStateRt>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/RobotStop");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::RobotStop>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::RobotStop>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/ServojRtStream");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::ServojRtStream>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::ServojRtStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/ServojStream");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::ServojStream>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::ServojStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/ServolRtStream");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::ServolRtStream>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::ServolRtStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/ServolStream");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::ServolStream>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::ServolStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/SpeedjRtStream");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::SpeedjRtStream>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::SpeedjRtStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/SpeedjStream");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::SpeedjStream>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::SpeedjStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/SpeedlRtStream");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::SpeedlRtStream>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::SpeedlRtStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/SpeedlStream");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::SpeedlStream>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::SpeedlStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/msg/TorqueRtStream");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::dsr_msgs2::msg::TorqueRtStream>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::msg::TorqueRtStream>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/AlignAxis1");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::AlignAxis1>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::AlignAxis1::Request>("request",false);
component<::dsr_msgs2::srv::AlignAxis1::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/AlignAxis2");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::AlignAxis2>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::AlignAxis2::Request>("request",false);
component<::dsr_msgs2::srv::AlignAxis2::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/AlterMotion");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::AlterMotion>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::AlterMotion::Request>("request",false);
component<::dsr_msgs2::srv::AlterMotion::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/CalcCoord");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::CalcCoord>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::CalcCoord::Request>("request",false);
component<::dsr_msgs2::srv::CalcCoord::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/ChangeCollisionSensitivity");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ChangeCollisionSensitivity>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::ChangeCollisionSensitivity::Request>("request",false);
component<::dsr_msgs2::srv::ChangeCollisionSensitivity::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/ChangeOperationSpeed");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ChangeOperationSpeed>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::ChangeOperationSpeed::Request>("request",false);
component<::dsr_msgs2::srv::ChangeOperationSpeed::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/CheckForceCondition");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::CheckForceCondition>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::CheckForceCondition::Request>("request",false);
component<::dsr_msgs2::srv::CheckForceCondition::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/CheckMotion");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::CheckMotion>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::CheckMotion::Request>("request",false);
component<::dsr_msgs2::srv::CheckMotion::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/CheckOrientationCondition1");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::CheckOrientationCondition1>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::CheckOrientationCondition1::Request>("request",false);
component<::dsr_msgs2::srv::CheckOrientationCondition1::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/CheckOrientationCondition2");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::CheckOrientationCondition2>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::CheckOrientationCondition2::Request>("request",false);
component<::dsr_msgs2::srv::CheckOrientationCondition2::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/CheckPositionCondition");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::CheckPositionCondition>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::CheckPositionCondition::Request>("request",false);
component<::dsr_msgs2::srv::CheckPositionCondition::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/ConfigCreateModbus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ConfigCreateModbus>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::ConfigCreateModbus::Request>("request",false);
component<::dsr_msgs2::srv::ConfigCreateModbus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/ConfigCreateTcp");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ConfigCreateTcp>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::ConfigCreateTcp::Request>("request",false);
component<::dsr_msgs2::srv::ConfigCreateTcp::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/ConfigCreateTool");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ConfigCreateTool>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::ConfigCreateTool::Request>("request",false);
component<::dsr_msgs2::srv::ConfigCreateTool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/ConfigDeleteModbus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ConfigDeleteModbus>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::ConfigDeleteModbus::Request>("request",false);
component<::dsr_msgs2::srv::ConfigDeleteModbus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/ConfigDeleteTcp");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ConfigDeleteTcp>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::ConfigDeleteTcp::Request>("request",false);
component<::dsr_msgs2::srv::ConfigDeleteTcp::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/ConfigDeleteTool");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ConfigDeleteTool>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::ConfigDeleteTool::Request>("request",false);
component<::dsr_msgs2::srv::ConfigDeleteTool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/ConnectRtControl");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ConnectRtControl>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::ConnectRtControl::Request>("request",false);
component<::dsr_msgs2::srv::ConnectRtControl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/CoordTransform");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::CoordTransform>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::CoordTransform::Request>("request",false);
component<::dsr_msgs2::srv::CoordTransform::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/DisableAlterMotion");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::DisableAlterMotion>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::DisableAlterMotion::Request>("request",false);
component<::dsr_msgs2::srv::DisableAlterMotion::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/DisconnectRtControl");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::DisconnectRtControl>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::DisconnectRtControl::Request>("request",false);
component<::dsr_msgs2::srv::DisconnectRtControl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/DrlPause");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::DrlPause>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::DrlPause::Request>("request",false);
component<::dsr_msgs2::srv::DrlPause::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/DrlResume");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::DrlResume>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::DrlResume::Request>("request",false);
component<::dsr_msgs2::srv::DrlResume::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/DrlStart");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::DrlStart>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::DrlStart::Request>("request",false);
component<::dsr_msgs2::srv::DrlStart::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/DrlStop");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::DrlStop>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::DrlStop::Request>("request",false);
component<::dsr_msgs2::srv::DrlStop::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/EnableAlterMotion");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::EnableAlterMotion>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::EnableAlterMotion::Request>("request",false);
component<::dsr_msgs2::srv::EnableAlterMotion::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/Fkin");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::Fkin>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::Fkin::Request>("request",false);
component<::dsr_msgs2::srv::Fkin::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetControlMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetControlMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetControlMode::Request>("request",false);
component<::dsr_msgs2::srv::GetControlMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetControlSpace");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetControlSpace>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetControlSpace::Request>("request",false);
component<::dsr_msgs2::srv::GetControlSpace::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetCtrlBoxAnalogInput");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCtrlBoxAnalogInput>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetCtrlBoxAnalogInput::Request>("request",false);
component<::dsr_msgs2::srv::GetCtrlBoxAnalogInput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetCtrlBoxDigitalInput");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCtrlBoxDigitalInput>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetCtrlBoxDigitalInput::Request>("request",false);
component<::dsr_msgs2::srv::GetCtrlBoxDigitalInput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetCtrlBoxDigitalOutput");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCtrlBoxDigitalOutput>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetCtrlBoxDigitalOutput::Request>("request",false);
component<::dsr_msgs2::srv::GetCtrlBoxDigitalOutput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetCurrentPose");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentPose>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetCurrentPose::Request>("request",false);
component<::dsr_msgs2::srv::GetCurrentPose::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetCurrentPosj");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentPosj>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetCurrentPosj::Request>("request",false);
component<::dsr_msgs2::srv::GetCurrentPosj::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetCurrentPosx");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentPosx>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetCurrentPosx::Request>("request",false);
component<::dsr_msgs2::srv::GetCurrentPosx::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetCurrentRotm");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentRotm>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetCurrentRotm::Request>("request",false);
component<::dsr_msgs2::srv::GetCurrentRotm::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetCurrentSolutionSpace");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentSolutionSpace>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetCurrentSolutionSpace::Request>("request",false);
component<::dsr_msgs2::srv::GetCurrentSolutionSpace::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetCurrentTcp");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentTcp>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetCurrentTcp::Request>("request",false);
component<::dsr_msgs2::srv::GetCurrentTcp::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetCurrentTool");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentTool>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetCurrentTool::Request>("request",false);
component<::dsr_msgs2::srv::GetCurrentTool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetCurrentToolFlangePosx");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentToolFlangePosx>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetCurrentToolFlangePosx::Request>("request",false);
component<::dsr_msgs2::srv::GetCurrentToolFlangePosx::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetCurrentVelj");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentVelj>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetCurrentVelj::Request>("request",false);
component<::dsr_msgs2::srv::GetCurrentVelj::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetCurrentVelx");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetCurrentVelx>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetCurrentVelx::Request>("request",false);
component<::dsr_msgs2::srv::GetCurrentVelx::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetDesiredPosj");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetDesiredPosj>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetDesiredPosj::Request>("request",false);
component<::dsr_msgs2::srv::GetDesiredPosj::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetDesiredPosx");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetDesiredPosx>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetDesiredPosx::Request>("request",false);
component<::dsr_msgs2::srv::GetDesiredPosx::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetDesiredVelj");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetDesiredVelj>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetDesiredVelj::Request>("request",false);
component<::dsr_msgs2::srv::GetDesiredVelj::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetDesiredVelx");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetDesiredVelx>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetDesiredVelx::Request>("request",false);
component<::dsr_msgs2::srv::GetDesiredVelx::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetDrlState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetDrlState>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetDrlState::Request>("request",false);
component<::dsr_msgs2::srv::GetDrlState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetExternalTorque");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetExternalTorque>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetExternalTorque::Request>("request",false);
component<::dsr_msgs2::srv::GetExternalTorque::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetInputRegisterBit");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetInputRegisterBit>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetInputRegisterBit::Request>("request",false);
component<::dsr_msgs2::srv::GetInputRegisterBit::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetInputRegisterFloat");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetInputRegisterFloat>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetInputRegisterFloat::Request>("request",false);
component<::dsr_msgs2::srv::GetInputRegisterFloat::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetInputRegisterInt");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetInputRegisterInt>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetInputRegisterInt::Request>("request",false);
component<::dsr_msgs2::srv::GetInputRegisterInt::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetJointTorque");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetJointTorque>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetJointTorque::Request>("request",false);
component<::dsr_msgs2::srv::GetJointTorque::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetLastAlarm");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetLastAlarm>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetLastAlarm::Request>("request",false);
component<::dsr_msgs2::srv::GetLastAlarm::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetModbusInput");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetModbusInput>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetModbusInput::Request>("request",false);
component<::dsr_msgs2::srv::GetModbusInput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetOrientationError");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetOrientationError>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetOrientationError::Request>("request",false);
component<::dsr_msgs2::srv::GetOrientationError::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetOutputRegisterBit");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetOutputRegisterBit>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetOutputRegisterBit::Request>("request",false);
component<::dsr_msgs2::srv::GetOutputRegisterBit::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetOutputRegisterFloat");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetOutputRegisterFloat>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetOutputRegisterFloat::Request>("request",false);
component<::dsr_msgs2::srv::GetOutputRegisterFloat::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetOutputRegisterInt");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetOutputRegisterInt>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetOutputRegisterInt::Request>("request",false);
component<::dsr_msgs2::srv::GetOutputRegisterInt::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetRobotLinkInfo");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRobotLinkInfo>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetRobotLinkInfo::Request>("request",false);
component<::dsr_msgs2::srv::GetRobotLinkInfo::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetRobotMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRobotMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetRobotMode::Request>("request",false);
component<::dsr_msgs2::srv::GetRobotMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetRobotSpeedMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRobotSpeedMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetRobotSpeedMode::Request>("request",false);
component<::dsr_msgs2::srv::GetRobotSpeedMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetRobotState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRobotState>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetRobotState::Request>("request",false);
component<::dsr_msgs2::srv::GetRobotState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetRobotSystem");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRobotSystem>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetRobotSystem::Request>("request",false);
component<::dsr_msgs2::srv::GetRobotSystem::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetRtControlInputDataList");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRtControlInputDataList>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetRtControlInputDataList::Request>("request",false);
component<::dsr_msgs2::srv::GetRtControlInputDataList::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetRtControlInputVersionList");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRtControlInputVersionList>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetRtControlInputVersionList::Request>("request",false);
component<::dsr_msgs2::srv::GetRtControlInputVersionList::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetRtControlOutputDataList");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRtControlOutputDataList>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetRtControlOutputDataList::Request>("request",false);
component<::dsr_msgs2::srv::GetRtControlOutputDataList::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetRtControlOutputVersionList");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetRtControlOutputVersionList>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetRtControlOutputVersionList::Request>("request",false);
component<::dsr_msgs2::srv::GetRtControlOutputVersionList::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetSolutionSpace");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetSolutionSpace>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetSolutionSpace::Request>("request",false);
component<::dsr_msgs2::srv::GetSolutionSpace::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetToolDigitalInput");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetToolDigitalInput>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetToolDigitalInput::Request>("request",false);
component<::dsr_msgs2::srv::GetToolDigitalInput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetToolDigitalOutput");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetToolDigitalOutput>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetToolDigitalOutput::Request>("request",false);
component<::dsr_msgs2::srv::GetToolDigitalOutput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetToolForce");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetToolForce>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetToolForce::Request>("request",false);
component<::dsr_msgs2::srv::GetToolForce::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetUserCartCoord");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetUserCartCoord>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetUserCartCoord::Request>("request",false);
component<::dsr_msgs2::srv::GetUserCartCoord::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/GetWorkpieceWeight");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::GetWorkpieceWeight>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::GetWorkpieceWeight::Request>("request",false);
component<::dsr_msgs2::srv::GetWorkpieceWeight::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/Ikin");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::Ikin>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::Ikin::Request>("request",false);
component<::dsr_msgs2::srv::Ikin::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/IsDoneBoltTightening");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::IsDoneBoltTightening>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::IsDoneBoltTightening::Request>("request",false);
component<::dsr_msgs2::srv::IsDoneBoltTightening::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/Jog");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::Jog>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::Jog::Request>("request",false);
component<::dsr_msgs2::srv::Jog::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/JogMulti");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::JogMulti>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::JogMulti::Request>("request",false);
component<::dsr_msgs2::srv::JogMulti::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/MoveBlending");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveBlending>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::MoveBlending::Request>("request",false);
component<::dsr_msgs2::srv::MoveBlending::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/MoveCircle");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveCircle>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::MoveCircle::Request>("request",false);
component<::dsr_msgs2::srv::MoveCircle::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/MoveHome");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveHome>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::MoveHome::Request>("request",false);
component<::dsr_msgs2::srv::MoveHome::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/MoveJoint");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveJoint>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::MoveJoint::Request>("request",false);
component<::dsr_msgs2::srv::MoveJoint::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/MoveJointx");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveJointx>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::MoveJointx::Request>("request",false);
component<::dsr_msgs2::srv::MoveJointx::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/MoveLine");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveLine>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::MoveLine::Request>("request",false);
component<::dsr_msgs2::srv::MoveLine::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/MovePause");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MovePause>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::MovePause::Request>("request",false);
component<::dsr_msgs2::srv::MovePause::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/MovePeriodic");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MovePeriodic>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::MovePeriodic::Request>("request",false);
component<::dsr_msgs2::srv::MovePeriodic::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/MoveResume");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveResume>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::MoveResume::Request>("request",false);
component<::dsr_msgs2::srv::MoveResume::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/MoveSpiral");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveSpiral>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::MoveSpiral::Request>("request",false);
component<::dsr_msgs2::srv::MoveSpiral::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/MoveSplineJoint");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveSplineJoint>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::MoveSplineJoint::Request>("request",false);
component<::dsr_msgs2::srv::MoveSplineJoint::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/MoveSplineTask");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveSplineTask>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::MoveSplineTask::Request>("request",false);
component<::dsr_msgs2::srv::MoveSplineTask::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/MoveStop");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveStop>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::MoveStop::Request>("request",false);
component<::dsr_msgs2::srv::MoveStop::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/MoveWait");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::MoveWait>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::MoveWait::Request>("request",false);
component<::dsr_msgs2::srv::MoveWait::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/OverwriteUserCartCoord");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::OverwriteUserCartCoord>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::OverwriteUserCartCoord::Request>("request",false);
component<::dsr_msgs2::srv::OverwriteUserCartCoord::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/ParallelAxis1");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ParallelAxis1>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::ParallelAxis1::Request>("request",false);
component<::dsr_msgs2::srv::ParallelAxis1::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/ParallelAxis2");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ParallelAxis2>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::ParallelAxis2::Request>("request",false);
component<::dsr_msgs2::srv::ParallelAxis2::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/ReadDataRt");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ReadDataRt>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::ReadDataRt::Request>("request",false);
component<::dsr_msgs2::srv::ReadDataRt::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/ReleaseComplianceCtrl");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ReleaseComplianceCtrl>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::ReleaseComplianceCtrl::Request>("request",false);
component<::dsr_msgs2::srv::ReleaseComplianceCtrl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/ReleaseForce");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ReleaseForce>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::ReleaseForce::Request>("request",false);
component<::dsr_msgs2::srv::ReleaseForce::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/ResetWorkpieceWeight");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ResetWorkpieceWeight>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::ResetWorkpieceWeight::Request>("request",false);
component<::dsr_msgs2::srv::ResetWorkpieceWeight::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/Robotiq2FClose");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::Robotiq2FClose>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::Robotiq2FClose::Request>("request",false);
component<::dsr_msgs2::srv::Robotiq2FClose::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/Robotiq2FMove");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::Robotiq2FMove>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::Robotiq2FMove::Request>("request",false);
component<::dsr_msgs2::srv::Robotiq2FMove::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/Robotiq2FOpen");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::Robotiq2FOpen>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::Robotiq2FOpen::Request>("request",false);
component<::dsr_msgs2::srv::Robotiq2FOpen::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SerialSendData");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SerialSendData>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SerialSendData::Request>("request",false);
component<::dsr_msgs2::srv::SerialSendData::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/ServoOff");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::ServoOff>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::ServoOff::Request>("request",false);
component<::dsr_msgs2::srv::ServoOff::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetAccjRt");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetAccjRt>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetAccjRt::Request>("request",false);
component<::dsr_msgs2::srv::SetAccjRt::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetAccxRt");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetAccxRt>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetAccxRt::Request>("request",false);
component<::dsr_msgs2::srv::SetAccxRt::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetCtrlBoxAnalogInputType");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetCtrlBoxAnalogInputType>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetCtrlBoxAnalogInputType::Request>("request",false);
component<::dsr_msgs2::srv::SetCtrlBoxAnalogInputType::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetCtrlBoxAnalogOutput");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetCtrlBoxAnalogOutput>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetCtrlBoxAnalogOutput::Request>("request",false);
component<::dsr_msgs2::srv::SetCtrlBoxAnalogOutput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetCtrlBoxAnalogOutputType");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetCtrlBoxAnalogOutputType>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetCtrlBoxAnalogOutputType::Request>("request",false);
component<::dsr_msgs2::srv::SetCtrlBoxAnalogOutputType::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetCtrlBoxDigitalOutput");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetCtrlBoxDigitalOutput>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetCtrlBoxDigitalOutput::Request>("request",false);
component<::dsr_msgs2::srv::SetCtrlBoxDigitalOutput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetCurrentTcp");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetCurrentTcp>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetCurrentTcp::Request>("request",false);
component<::dsr_msgs2::srv::SetCurrentTcp::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetCurrentTool");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetCurrentTool>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetCurrentTool::Request>("request",false);
component<::dsr_msgs2::srv::SetCurrentTool::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetDesiredForce");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetDesiredForce>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetDesiredForce::Request>("request",false);
component<::dsr_msgs2::srv::SetDesiredForce::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetModbusOutput");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetModbusOutput>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetModbusOutput::Request>("request",false);
component<::dsr_msgs2::srv::SetModbusOutput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetOutputRegisterBit");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetOutputRegisterBit>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetOutputRegisterBit::Request>("request",false);
component<::dsr_msgs2::srv::SetOutputRegisterBit::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetOutputRegisterFloat");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetOutputRegisterFloat>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetOutputRegisterFloat::Request>("request",false);
component<::dsr_msgs2::srv::SetOutputRegisterFloat::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetOutputRegisterInt");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetOutputRegisterInt>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetOutputRegisterInt::Request>("request",false);
component<::dsr_msgs2::srv::SetOutputRegisterInt::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetRefCoord");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetRefCoord>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetRefCoord::Request>("request",false);
component<::dsr_msgs2::srv::SetRefCoord::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetRobotControl");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetRobotControl>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetRobotControl::Request>("request",false);
component<::dsr_msgs2::srv::SetRobotControl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetRobotMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetRobotMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetRobotMode::Request>("request",false);
component<::dsr_msgs2::srv::SetRobotMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetRobotSpeedMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetRobotSpeedMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetRobotSpeedMode::Request>("request",false);
component<::dsr_msgs2::srv::SetRobotSpeedMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetRobotSystem");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetRobotSystem>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetRobotSystem::Request>("request",false);
component<::dsr_msgs2::srv::SetRobotSystem::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetRtControlInput");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetRtControlInput>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetRtControlInput::Request>("request",false);
component<::dsr_msgs2::srv::SetRtControlInput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetRtControlOutput");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetRtControlOutput>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetRtControlOutput::Request>("request",false);
component<::dsr_msgs2::srv::SetRtControlOutput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetSafeStopResetType");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetSafeStopResetType>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetSafeStopResetType::Request>("request",false);
component<::dsr_msgs2::srv::SetSafeStopResetType::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetSafetyMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetSafetyMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetSafetyMode::Request>("request",false);
component<::dsr_msgs2::srv::SetSafetyMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetSingularHandlingForce");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetSingularHandlingForce>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetSingularHandlingForce::Request>("request",false);
component<::dsr_msgs2::srv::SetSingularHandlingForce::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetSingularityHandling");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetSingularityHandling>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetSingularityHandling::Request>("request",false);
component<::dsr_msgs2::srv::SetSingularityHandling::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetStiffnessx");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetStiffnessx>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetStiffnessx::Request>("request",false);
component<::dsr_msgs2::srv::SetStiffnessx::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetToolDigitalOutput");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetToolDigitalOutput>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetToolDigitalOutput::Request>("request",false);
component<::dsr_msgs2::srv::SetToolDigitalOutput::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetToolShape");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetToolShape>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetToolShape::Request>("request",false);
component<::dsr_msgs2::srv::SetToolShape::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetUserCartCoord1");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetUserCartCoord1>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetUserCartCoord1::Request>("request",false);
component<::dsr_msgs2::srv::SetUserCartCoord1::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetUserCartCoord2");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetUserCartCoord2>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetUserCartCoord2::Request>("request",false);
component<::dsr_msgs2::srv::SetUserCartCoord2::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetUserCartCoord3");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetUserCartCoord3>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetUserCartCoord3::Request>("request",false);
component<::dsr_msgs2::srv::SetUserCartCoord3::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetVeljRt");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetVeljRt>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetVeljRt::Request>("request",false);
component<::dsr_msgs2::srv::SetVeljRt::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/SetVelxRt");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::SetVelxRt>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::SetVelxRt::Request>("request",false);
component<::dsr_msgs2::srv::SetVelxRt::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/StartRtControl");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::StartRtControl>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::StartRtControl::Request>("request",false);
component<::dsr_msgs2::srv::StartRtControl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/StopRtControl");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::StopRtControl>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::StopRtControl::Request>("request",false);
component<::dsr_msgs2::srv::StopRtControl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/TaskComplianceCtrl");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::TaskComplianceCtrl>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::TaskComplianceCtrl::Request>("request",false);
component<::dsr_msgs2::srv::TaskComplianceCtrl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/Trans");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::Trans>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::Trans::Request>("request",false);
component<::dsr_msgs2::srv::Trans::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("dsr_msgs2/srv/WriteDataRt");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::dsr_msgs2::srv::WriteDataRt>());
bounded_native_io::print(output(),",\"components\":{");
component<::dsr_msgs2::srv::WriteDataRt::Request>("request",false);
component<::dsr_msgs2::srv::WriteDataRt::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/msg/Float64MultiArray");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Float64MultiArray>());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::msg::Float64MultiArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/msg/MultiArrayDimension");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::MultiArrayDimension>());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::msg::MultiArrayDimension>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/msg/MultiArrayLayout");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::MultiArrayLayout>());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::msg::MultiArrayLayout>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),"}}\n");return 0;
}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\n");return 1;}}
