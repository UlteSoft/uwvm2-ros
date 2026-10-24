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
#include <aimdk_msgs/msg/agent_properties.hpp>
#include <aimdk_msgs/msg/agent_properties_value.hpp>
#include <aimdk_msgs/msg/agent_property_id_type.hpp>
#include <aimdk_msgs/msg/alert_code.hpp>
#include <aimdk_msgs/msg/alert_code_array.hpp>
#include <aimdk_msgs/msg/audio_capture.hpp>
#include <aimdk_msgs/msg/audio_data.hpp>
#include <aimdk_msgs/msg/audio_file.hpp>
#include <aimdk_msgs/msg/audio_info.hpp>
#include <aimdk_msgs/msg/audio_playback.hpp>
#include <aimdk_msgs/msg/audio_vad_state_type.hpp>
#include <aimdk_msgs/msg/common_request.hpp>
#include <aimdk_msgs/msg/common_response.hpp>
#include <aimdk_msgs/msg/common_state.hpp>
#include <aimdk_msgs/msg/common_task_request.hpp>
#include <aimdk_msgs/msg/common_task_response.hpp>
#include <aimdk_msgs/msg/current_version.hpp>
#include <aimdk_msgs/msg/diagnostic_info.hpp>
#include <aimdk_msgs/msg/diagnostic_info_array.hpp>
#include <aimdk_msgs/msg/diagnostic_type.hpp>
#include <aimdk_msgs/msg/domain_error_state.hpp>
#include <aimdk_msgs/msg/face_emoji_status.hpp>
#include <aimdk_msgs/msg/fix_line_intro_state.hpp>
#include <aimdk_msgs/msg/focus_requester.hpp>
#include <aimdk_msgs/msg/focus_response.hpp>
#include <aimdk_msgs/msg/hand_command.hpp>
#include <aimdk_msgs/msg/hand_command_array.hpp>
#include <aimdk_msgs/msg/hand_state.hpp>
#include <aimdk_msgs/msg/hand_state_array.hpp>
#include <aimdk_msgs/msg/hand_touch_sensor_data.hpp>
#include <aimdk_msgs/msg/hand_type.hpp>
#include <aimdk_msgs/msg/hds_event_query.hpp>
#include <aimdk_msgs/msg/hds_event_record.hpp>
#include <aimdk_msgs/msg/hr_point.hpp>
#include <aimdk_msgs/msg/joint_command.hpp>
#include <aimdk_msgs/msg/joint_command_array.hpp>
#include <aimdk_msgs/msg/joint_state.hpp>
#include <aimdk_msgs/msg/joint_state_array.hpp>
#include <aimdk_msgs/msg/mc_action.hpp>
#include <aimdk_msgs/msg/mc_action_command.hpp>
#include <aimdk_msgs/msg/mc_action_info.hpp>
#include <aimdk_msgs/msg/mc_action_status.hpp>
#include <aimdk_msgs/msg/mc_body_pose_status.hpp>
#include <aimdk_msgs/msg/mc_common_state.hpp>
#include <aimdk_msgs/msg/mc_control_area.hpp>
#include <aimdk_msgs/msg/mc_fsm_state.hpp>
#include <aimdk_msgs/msg/mc_hand_status.hpp>
#include <aimdk_msgs/msg/mc_input_action.hpp>
#include <aimdk_msgs/msg/mc_input_source.hpp>
#include <aimdk_msgs/msg/mc_locomotion_speed_bounds.hpp>
#include <aimdk_msgs/msg/mc_locomotion_speed_mode.hpp>
#include <aimdk_msgs/msg/mc_locomotion_speed_status.hpp>
#include <aimdk_msgs/msg/mc_locomotion_velocity.hpp>
#include <aimdk_msgs/msg/mc_motion_status.hpp>
#include <aimdk_msgs/msg/mc_motion_type.hpp>
#include <aimdk_msgs/msg/mc_player_state.hpp>
#include <aimdk_msgs/msg/mc_preset_motion.hpp>
#include <aimdk_msgs/msg/mc_runtime_model.hpp>
#include <aimdk_msgs/msg/mc_servo_status.hpp>
#include <aimdk_msgs/msg/message_header.hpp>
#include <aimdk_msgs/msg/navi_point.hpp>
#include <aimdk_msgs/msg/path.hpp>
#include <aimdk_msgs/msg/pilot_state.hpp>
#include <aimdk_msgs/msg/play_state_change.hpp>
#include <aimdk_msgs/msg/play_state_type.hpp>
#include <aimdk_msgs/msg/play_tts_request.hpp>
#include <aimdk_msgs/msg/play_tts_response.hpp>
#include <aimdk_msgs/msg/pmu_led_state.hpp>
#include <aimdk_msgs/msg/pmu_state.hpp>
#include <aimdk_msgs/msg/pnc_task_request.hpp>
#include <aimdk_msgs/msg/pnc_task_response.hpp>
#include <aimdk_msgs/msg/post_proc_sig.hpp>
#include <aimdk_msgs/msg/post_proc_sig_array.hpp>
#include <aimdk_msgs/msg/processed_audio_output.hpp>
#include <aimdk_msgs/msg/qr_code.hpp>
#include <aimdk_msgs/msg/qr_point.hpp>
#include <aimdk_msgs/msg/region.hpp>
#include <aimdk_msgs/msg/request_header.hpp>
#include <aimdk_msgs/msg/response_header.hpp>
#include <aimdk_msgs/msg/robot_resource.hpp>
#include <aimdk_msgs/msg/sig_set.hpp>
#include <aimdk_msgs/msg/slave_device.hpp>
#include <aimdk_msgs/msg/sm_system_state.hpp>
#include <aimdk_msgs/msg/system_status.hpp>
#include <aimdk_msgs/msg/task_master_state.hpp>
#include <aimdk_msgs/msg/touch_state.hpp>
#include <aimdk_msgs/msg/tts_priority_level.hpp>
#include <aimdk_msgs/msg/upper_body_command_array.hpp>
#include <aimdk_msgs/srv/abandon_audio_focus.hpp>
#include <aimdk_msgs/srv/execute_action_resource.hpp>
#include <aimdk_msgs/srv/get_all_joint_state.hpp>
#include <aimdk_msgs/srv/get_current_input_source.hpp>
#include <aimdk_msgs/srv/get_hand_type.hpp>
#include <aimdk_msgs/srv/get_hds_event_history.hpp>
#include <aimdk_msgs/srv/get_mc_action.hpp>
#include <aimdk_msgs/srv/get_mc_locomotion_speed_status.hpp>
#include <aimdk_msgs/srv/get_mc_preset_motion_state.hpp>
#include <aimdk_msgs/srv/get_mic_source_request.hpp>
#include <aimdk_msgs/srv/get_mute.hpp>
#include <aimdk_msgs/srv/get_robot_resources.hpp>
#include <aimdk_msgs/srv/get_stored_map_by_name.hpp>
#include <aimdk_msgs/srv/get_system_state.hpp>
#include <aimdk_msgs/srv/get_volume.hpp>
#include <aimdk_msgs/srv/group_control_time_sync.hpp>
#include <aimdk_msgs/srv/migrate_system_state.hpp>
#include <aimdk_msgs/srv/play_audio_file.hpp>
#include <aimdk_msgs/srv/play_emoji.hpp>
#include <aimdk_msgs/srv/play_emoji_group.hpp>
#include <aimdk_msgs/srv/play_tts.hpp>
#include <aimdk_msgs/srv/play_video.hpp>
#include <aimdk_msgs/srv/play_video_group.hpp>
#include <aimdk_msgs/srv/request_audio_focus.hpp>
#include <aimdk_msgs/srv/set_agent_properties_request.hpp>
#include <aimdk_msgs/srv/set_mc_action.hpp>
#include <aimdk_msgs/srv/set_mc_input_source.hpp>
#include <aimdk_msgs/srv/set_mc_preset_motion.hpp>
#include <aimdk_msgs/srv/set_mic_source_request.hpp>
#include <aimdk_msgs/srv/set_mute.hpp>
#include <aimdk_msgs/srv/set_pmu_led.hpp>
#include <aimdk_msgs/srv/set_volume.hpp>
#include <aimdk_msgs/srv/stop_audio_play.hpp>
#include <builtin_interfaces/msg/time.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <geometry_msgs/msg/point32.hpp>
#include <geometry_msgs/msg/polygon.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/pose2_d.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/quaternion.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/vector3.hpp>
#include <nav_msgs/msg/map_meta_data.hpp>
#include <std_msgs/msg/header.hpp>
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
if(!::uwvm_industrial_source_cb586b29fafdf87e7bddca01cc4d20b26aed50daf9f748bbe8caeb5f47e8462f::validate())throw std::runtime_error{"native source constants differ"};
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AgentProperties>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AgentProperties>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AgentPropertiesValue>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AgentPropertiesValue>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AgentPropertyIdType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AgentPropertyIdType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AlertCode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AlertCode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AlertCodeArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AlertCodeArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioCapture>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioCapture>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioFile>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioFile>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioPlayback>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioPlayback>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioVadStateType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioVadStateType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CommonRequest>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CommonRequest>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CommonResponse>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CommonResponse>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CommonState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CommonState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CommonTaskRequest>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CommonTaskRequest>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CommonTaskResponse>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CommonTaskResponse>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CurrentVersion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CurrentVersion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::DiagnosticInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::DiagnosticInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::DiagnosticInfoArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::DiagnosticInfoArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::DiagnosticType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::DiagnosticType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::DomainErrorState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::DomainErrorState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::FaceEmojiStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::FaceEmojiStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::FixLineIntroState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::FixLineIntroState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::FocusRequester>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::FocusRequester>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::FocusResponse>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::FocusResponse>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HRPoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HRPoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandCommand>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandCommand>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandCommandArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandCommandArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandStateArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandStateArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandTouchSensorData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandTouchSensorData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HdsEventQuery>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HdsEventQuery>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HdsEventRecord>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HdsEventRecord>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::JointCommand>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::JointCommand>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::JointCommandArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::JointCommandArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::JointState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::JointState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::JointStateArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::JointStateArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McAction>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McAction>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McActionCommand>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McActionCommand>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McActionInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McActionInfo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McActionStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McActionStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McBodyPoseStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McBodyPoseStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McCommonState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McCommonState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McControlArea>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McControlArea>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McFsmState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McFsmState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McHandStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McHandStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McInputAction>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McInputAction>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McInputSource>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McInputSource>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McLocomotionSpeedBounds>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McLocomotionSpeedBounds>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McLocomotionSpeedMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McLocomotionSpeedMode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McLocomotionSpeedStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McLocomotionSpeedStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McLocomotionVelocity>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McLocomotionVelocity>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McMotionStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McMotionStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McMotionType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McMotionType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McPlayerState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McPlayerState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McPresetMotion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McPresetMotion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McRuntimeModel>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McRuntimeModel>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McServoStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McServoStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::MessageHeader>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::MessageHeader>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::NaviPoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::NaviPoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::Path>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::Path>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PilotState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PilotState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PlayStateChange>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PlayStateChange>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PlayStateType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PlayStateType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PlayTtsRequest>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PlayTtsRequest>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PlayTtsResponse>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PlayTtsResponse>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PmuLedState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PmuLedState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PmuState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PmuState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PncTaskRequest>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PncTaskRequest>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PncTaskResponse>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PncTaskResponse>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PostProcSig>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PostProcSig>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PostProcSigArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PostProcSigArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::ProcessedAudioOutput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::ProcessedAudioOutput>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::QRCode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::QRCode>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::QRPoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::QRPoint>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::Region>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::Region>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::RequestHeader>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::RequestHeader>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::ResponseHeader>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::ResponseHeader>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::RobotResource>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::RobotResource>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::SigSet>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::SigSet>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::SlaveDevice>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::SlaveDevice>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::SmSystemState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::SmSystemState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::SystemStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::SystemStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::TaskMasterState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::TaskMasterState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::TouchState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::TouchState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::TtsPriorityLevel>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::TtsPriorityLevel>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::UpperBodyCommandArray>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::UpperBodyCommandArray>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::AbandonAudioFocus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::AbandonAudioFocus::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::AbandonAudioFocus::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::ExecuteActionResource>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::ExecuteActionResource::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::ExecuteActionResource::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetAllJointState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetAllJointState::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetAllJointState::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetCurrentInputSource>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetCurrentInputSource::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetCurrentInputSource::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetHandType>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetHandType::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetHandType::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetHdsEventHistory>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetHdsEventHistory::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetHdsEventHistory::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetMcAction>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetMcAction::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetMcAction::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetMcLocomotionSpeedStatus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetMcLocomotionSpeedStatus::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetMcLocomotionSpeedStatus::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetMcPresetMotionState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetMcPresetMotionState::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetMcPresetMotionState::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetMicSourceRequest>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetMicSourceRequest::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetMicSourceRequest::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetMute>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetMute::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetMute::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetRobotResources>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetRobotResources::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetRobotResources::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetStoredMapByName>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetStoredMapByName::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetStoredMapByName::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetSystemState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetSystemState::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetSystemState::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetVolume>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetVolume::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GetVolume::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GroupControlTimeSync>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GroupControlTimeSync::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::GroupControlTimeSync::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::MigrateSystemState>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::MigrateSystemState::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::MigrateSystemState::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::PlayAudioFile>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::PlayAudioFile::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::PlayAudioFile::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::PlayEmoji>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::PlayEmoji::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::PlayEmoji::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::PlayEmojiGroup>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::PlayEmojiGroup::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::PlayEmojiGroup::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::PlayTts>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::PlayTts::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::PlayTts::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::PlayVideo>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::PlayVideo::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::PlayVideo::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::PlayVideoGroup>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::PlayVideoGroup::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::PlayVideoGroup::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::RequestAudioFocus>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::RequestAudioFocus::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::RequestAudioFocus::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::SetAgentPropertiesRequest>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::SetAgentPropertiesRequest::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::SetAgentPropertiesRequest::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::SetMcAction>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::SetMcAction::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::SetMcAction::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::SetMcInputSource>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::SetMcInputSource::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::SetMcInputSource::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::SetMcPresetMotion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::SetMcPresetMotion::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::SetMcPresetMotion::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::SetMicSourceRequest>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::SetMicSourceRequest::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::SetMicSourceRequest::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::SetMute>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::SetMute::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::SetMute::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::SetPmuLed>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::SetPmuLed::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::SetPmuLed::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::SetVolume>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::SetVolume::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::SetVolume::Response>());
industrial_warm(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::StopAudioPlay>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::StopAudioPlay::Request>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::srv::StopAudioPlay::Response>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Point>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Point>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Point32>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Point32>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Polygon>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Polygon>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Pose>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Pose>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Pose2D>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Pose2D>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::PoseStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::PoseStamped>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Twist>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Twist>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::nav_msgs::msg::MapMetaData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::nav_msgs::msg::MapMetaData>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
industrial_warm(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};
bounded_native_io::gate('I');
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"cb586b29fafdf87e7bddca01cc4d20b26aed50daf9f748bbe8caeb5f47e8462f\",\"types\":{");
quoted("aimdk_msgs/msg/AgentProperties");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AgentProperties>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::AgentProperties>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/AgentPropertiesValue");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AgentPropertiesValue>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::AgentPropertiesValue>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/AgentPropertyIdType");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AgentPropertyIdType>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::AgentPropertyIdType>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/AlertCode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AlertCode>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::AlertCode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/AlertCodeArray");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AlertCodeArray>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::AlertCodeArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/AudioCapture");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioCapture>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::AudioCapture>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/AudioData");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioData>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::AudioData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/AudioFile");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioFile>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::AudioFile>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/AudioInfo");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioInfo>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::AudioInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/AudioPlayback");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioPlayback>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::AudioPlayback>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/AudioVadStateType");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::AudioVadStateType>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::AudioVadStateType>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/CommonRequest");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CommonRequest>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::CommonRequest>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/CommonResponse");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CommonResponse>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::CommonResponse>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/CommonState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CommonState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::CommonState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/CommonTaskRequest");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CommonTaskRequest>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::CommonTaskRequest>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/CommonTaskResponse");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CommonTaskResponse>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::CommonTaskResponse>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/CurrentVersion");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::CurrentVersion>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::CurrentVersion>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/DiagnosticInfo");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::DiagnosticInfo>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::DiagnosticInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/DiagnosticInfoArray");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::DiagnosticInfoArray>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::DiagnosticInfoArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/DiagnosticType");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::DiagnosticType>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::DiagnosticType>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/DomainErrorState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::DomainErrorState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::DomainErrorState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/FaceEmojiStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::FaceEmojiStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::FaceEmojiStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/FixLineIntroState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::FixLineIntroState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::FixLineIntroState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/FocusRequester");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::FocusRequester>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::FocusRequester>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/FocusResponse");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::FocusResponse>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::FocusResponse>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/HRPoint");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HRPoint>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::HRPoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/HandCommand");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandCommand>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::HandCommand>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/HandCommandArray");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandCommandArray>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::HandCommandArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/HandState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::HandState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/HandStateArray");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandStateArray>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::HandStateArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/HandTouchSensorData");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandTouchSensorData>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::HandTouchSensorData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/HandType");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HandType>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::HandType>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/HdsEventQuery");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HdsEventQuery>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::HdsEventQuery>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/HdsEventRecord");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::HdsEventRecord>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::HdsEventRecord>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/JointCommand");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::JointCommand>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::JointCommand>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/JointCommandArray");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::JointCommandArray>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::JointCommandArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/JointState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::JointState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::JointState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/JointStateArray");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::JointStateArray>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::JointStateArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McAction");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McAction>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McAction>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McActionCommand");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McActionCommand>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McActionCommand>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McActionInfo");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McActionInfo>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McActionInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McActionStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McActionStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McActionStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McBodyPoseStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McBodyPoseStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McBodyPoseStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McCommonState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McCommonState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McCommonState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McControlArea");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McControlArea>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McControlArea>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McFsmState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McFsmState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McFsmState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McHandStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McHandStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McHandStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McInputAction");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McInputAction>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McInputAction>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McInputSource");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McInputSource>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McInputSource>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McLocomotionSpeedBounds");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McLocomotionSpeedBounds>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McLocomotionSpeedBounds>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McLocomotionSpeedMode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McLocomotionSpeedMode>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McLocomotionSpeedMode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McLocomotionSpeedStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McLocomotionSpeedStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McLocomotionSpeedStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McLocomotionVelocity");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McLocomotionVelocity>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McLocomotionVelocity>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McMotionStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McMotionStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McMotionStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McMotionType");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McMotionType>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McMotionType>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McPlayerState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McPlayerState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McPlayerState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McPresetMotion");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McPresetMotion>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McPresetMotion>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McRuntimeModel");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McRuntimeModel>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McRuntimeModel>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/McServoStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::McServoStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::McServoStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/MessageHeader");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::MessageHeader>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::MessageHeader>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/NaviPoint");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::NaviPoint>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::NaviPoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/Path");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::Path>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::Path>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/PilotState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PilotState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::PilotState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/PlayStateChange");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PlayStateChange>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::PlayStateChange>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/PlayStateType");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PlayStateType>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::PlayStateType>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/PlayTtsRequest");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PlayTtsRequest>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::PlayTtsRequest>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/PlayTtsResponse");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PlayTtsResponse>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::PlayTtsResponse>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/PmuLedState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PmuLedState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::PmuLedState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/PmuState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PmuState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::PmuState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/PncTaskRequest");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PncTaskRequest>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::PncTaskRequest>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/PncTaskResponse");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PncTaskResponse>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::PncTaskResponse>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/PostProcSig");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PostProcSig>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::PostProcSig>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/PostProcSigArray");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::PostProcSigArray>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::PostProcSigArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/ProcessedAudioOutput");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::ProcessedAudioOutput>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::ProcessedAudioOutput>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/QRCode");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::QRCode>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::QRCode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/QRPoint");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::QRPoint>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::QRPoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/Region");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::Region>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::Region>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/RequestHeader");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::RequestHeader>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::RequestHeader>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/ResponseHeader");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::ResponseHeader>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::ResponseHeader>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/RobotResource");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::RobotResource>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::RobotResource>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/SigSet");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::SigSet>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::SigSet>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/SlaveDevice");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::SlaveDevice>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::SlaveDevice>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/SmSystemState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::SmSystemState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::SmSystemState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/SystemStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::SystemStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::SystemStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/TaskMasterState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::TaskMasterState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::TaskMasterState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/TouchState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::TouchState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::TouchState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/TtsPriorityLevel");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::TtsPriorityLevel>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::TtsPriorityLevel>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/msg/UpperBodyCommandArray");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::aimdk_msgs::msg::UpperBodyCommandArray>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::msg::UpperBodyCommandArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/AbandonAudioFocus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::AbandonAudioFocus>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::AbandonAudioFocus::Request>("request",false);
component<::aimdk_msgs::srv::AbandonAudioFocus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/ExecuteActionResource");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::ExecuteActionResource>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::ExecuteActionResource::Request>("request",false);
component<::aimdk_msgs::srv::ExecuteActionResource::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/GetAllJointState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetAllJointState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::GetAllJointState::Request>("request",false);
component<::aimdk_msgs::srv::GetAllJointState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/GetCurrentInputSource");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetCurrentInputSource>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::GetCurrentInputSource::Request>("request",false);
component<::aimdk_msgs::srv::GetCurrentInputSource::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/GetHandType");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetHandType>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::GetHandType::Request>("request",false);
component<::aimdk_msgs::srv::GetHandType::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/GetHdsEventHistory");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetHdsEventHistory>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::GetHdsEventHistory::Request>("request",false);
component<::aimdk_msgs::srv::GetHdsEventHistory::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/GetMcAction");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetMcAction>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::GetMcAction::Request>("request",false);
component<::aimdk_msgs::srv::GetMcAction::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/GetMcLocomotionSpeedStatus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetMcLocomotionSpeedStatus>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::GetMcLocomotionSpeedStatus::Request>("request",false);
component<::aimdk_msgs::srv::GetMcLocomotionSpeedStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/GetMcPresetMotionState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetMcPresetMotionState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::GetMcPresetMotionState::Request>("request",false);
component<::aimdk_msgs::srv::GetMcPresetMotionState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/GetMicSourceRequest");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetMicSourceRequest>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::GetMicSourceRequest::Request>("request",false);
component<::aimdk_msgs::srv::GetMicSourceRequest::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/GetMute");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetMute>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::GetMute::Request>("request",false);
component<::aimdk_msgs::srv::GetMute::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/GetRobotResources");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetRobotResources>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::GetRobotResources::Request>("request",false);
component<::aimdk_msgs::srv::GetRobotResources::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/GetStoredMapByName");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetStoredMapByName>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::GetStoredMapByName::Request>("request",false);
component<::aimdk_msgs::srv::GetStoredMapByName::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/GetSystemState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetSystemState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::GetSystemState::Request>("request",false);
component<::aimdk_msgs::srv::GetSystemState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/GetVolume");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GetVolume>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::GetVolume::Request>("request",false);
component<::aimdk_msgs::srv::GetVolume::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/GroupControlTimeSync");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::GroupControlTimeSync>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::GroupControlTimeSync::Request>("request",false);
component<::aimdk_msgs::srv::GroupControlTimeSync::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/MigrateSystemState");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::MigrateSystemState>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::MigrateSystemState::Request>("request",false);
component<::aimdk_msgs::srv::MigrateSystemState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/PlayAudioFile");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::PlayAudioFile>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::PlayAudioFile::Request>("request",false);
component<::aimdk_msgs::srv::PlayAudioFile::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/PlayEmoji");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::PlayEmoji>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::PlayEmoji::Request>("request",false);
component<::aimdk_msgs::srv::PlayEmoji::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/PlayEmojiGroup");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::PlayEmojiGroup>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::PlayEmojiGroup::Request>("request",false);
component<::aimdk_msgs::srv::PlayEmojiGroup::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/PlayTts");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::PlayTts>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::PlayTts::Request>("request",false);
component<::aimdk_msgs::srv::PlayTts::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/PlayVideo");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::PlayVideo>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::PlayVideo::Request>("request",false);
component<::aimdk_msgs::srv::PlayVideo::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/PlayVideoGroup");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::PlayVideoGroup>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::PlayVideoGroup::Request>("request",false);
component<::aimdk_msgs::srv::PlayVideoGroup::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/RequestAudioFocus");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::RequestAudioFocus>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::RequestAudioFocus::Request>("request",false);
component<::aimdk_msgs::srv::RequestAudioFocus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/SetAgentPropertiesRequest");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::SetAgentPropertiesRequest>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::SetAgentPropertiesRequest::Request>("request",false);
component<::aimdk_msgs::srv::SetAgentPropertiesRequest::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/SetMcAction");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::SetMcAction>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::SetMcAction::Request>("request",false);
component<::aimdk_msgs::srv::SetMcAction::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/SetMcInputSource");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::SetMcInputSource>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::SetMcInputSource::Request>("request",false);
component<::aimdk_msgs::srv::SetMcInputSource::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/SetMcPresetMotion");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::SetMcPresetMotion>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::SetMcPresetMotion::Request>("request",false);
component<::aimdk_msgs::srv::SetMcPresetMotion::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/SetMicSourceRequest");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::SetMicSourceRequest>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::SetMicSourceRequest::Request>("request",false);
component<::aimdk_msgs::srv::SetMicSourceRequest::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/SetMute");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::SetMute>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::SetMute::Request>("request",false);
component<::aimdk_msgs::srv::SetMute::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/SetPmuLed");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::SetPmuLed>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::SetPmuLed::Request>("request",false);
component<::aimdk_msgs::srv::SetPmuLed::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/SetVolume");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::SetVolume>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::SetVolume::Request>("request",false);
component<::aimdk_msgs::srv::SetVolume::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aimdk_msgs/srv/StopAudioPlay");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_service_type_support_handle<::aimdk_msgs::srv::StopAudioPlay>());
bounded_native_io::print(output(),",\"components\":{");
component<::aimdk_msgs::srv::StopAudioPlay::Request>("request",false);
component<::aimdk_msgs::srv::StopAudioPlay::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("builtin_interfaces/msg/Time");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::builtin_interfaces::msg::Time>());
bounded_native_io::print(output(),",\"components\":{");
component<::builtin_interfaces::msg::Time>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Point");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Point>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Point>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Point32");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Point32>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Point32>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Polygon");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Polygon>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Polygon>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Pose");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Pose>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Pose>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Pose2D");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Pose2D>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Pose2D>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/PoseStamped");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::PoseStamped>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::PoseStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Quaternion");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Quaternion>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Quaternion>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Twist");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Twist>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Twist>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/msg/Vector3");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::geometry_msgs::msg::Vector3>());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::msg::Vector3>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nav_msgs/msg/MapMetaData");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::nav_msgs::msg::MapMetaData>());
bounded_native_io::print(output(),",\"components\":{");
component<::nav_msgs::msg::MapMetaData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/msg/Header");
bounded_native_io::print(output(),":{\"official\":");
metadata(rosidl_typesupport_cpp::get_message_type_support_handle<::std_msgs::msg::Header>());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::msg::Header>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),"}}\n");return 0;
}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\n");return 1;}}
