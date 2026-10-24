/** Industrial native metadata source; no Node, endpoint or grant. */
#include "test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/native_protocol.h"
#include <array>
#include <string_view>
#include <stdexcept>
#include <csignal>
#include <cstdint>
#if UWVM_CUSTOM_NATIVE_ROS_VERSION != 1
#error "Select the exact source-reviewed ROS major"
#endif
#include <uwvm_fourier_bridge/ExecuteMove.h>
#include <uwvm_fourier_bridge/GetBaseData.h>
#include <uwvm_fourier_bridge/GetCartesianState.h>
#include <uwvm_fourier_bridge/GetContactData.h>
#include <uwvm_fourier_bridge/GetFsmName.h>
#include <uwvm_fourier_bridge/GetFsmState.h>
#include <uwvm_fourier_bridge/GetGroupMotionState.h>
#include <uwvm_fourier_bridge/GetGroupMotorCfg.h>
#include <uwvm_fourier_bridge/GetGroupState.h>
#include <uwvm_fourier_bridge/GetSnapshot.h>
#include <uwvm_fourier_bridge/GetStandPose.h>
#include <uwvm_fourier_bridge/GetUpperFsmName.h>
#include <uwvm_fourier_bridge/GetUpperFsmState.h>
#include <uwvm_fourier_bridge/GetVelocitySource.h>
#include <uwvm_fourier_bridge/GetVelocitySourceName.h>
#include <uwvm_fourier_bridge/GroupState.h>
#include <uwvm_fourier_bridge/GroupValues.h>
#include <uwvm_fourier_bridge/MotionRecipe.h>
#include <uwvm_fourier_bridge/SetFsmState.h>
#include <uwvm_fourier_bridge/SetGroupCommand.h>
#include <uwvm_fourier_bridge/SetMotorCfgPd.h>
#include <uwvm_fourier_bridge/SetStandPose.h>
#include <uwvm_fourier_bridge/SetUpperFsmState.h>
#include <uwvm_fourier_bridge/SetVelocity.h>
#include <uwvm_fourier_bridge/SetVelocitySource.h>
#include <uwvm_fourier_bridge/Status.h>
#include <uwvm_fourier_bridge/VelocityCommand.h>
#include <uwvm_fourier_bridge/WaitGroupsMotionComplete.h>
#include <ros/message_traits.h>
#include <ros/service_traits.h>
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
int main() {try {
if(!::uwvm_industrial_source_ca3d163bab055381827226140568f3bef7eaac187cebd76878e0b63e9e442356::validate())throw std::runtime_error{"native source constants differ"};
if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};
bounded_native_io::gate('I');
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"ca3d163bab055381827226140568f3bef7eaac187cebd76878e0b63e9e442356\",\"types\":{");
quoted("uwvm_fourier_bridge/ExecuteMove");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::ExecuteMove>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::ExecuteMove::Request>("request",false);
component<::uwvm_fourier_bridge::ExecuteMove::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/GetBaseData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::GetBaseData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::GetBaseData::Request>("request",false);
component<::uwvm_fourier_bridge::GetBaseData::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/GetCartesianState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::GetCartesianState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::GetCartesianState::Request>("request",false);
component<::uwvm_fourier_bridge::GetCartesianState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/GetContactData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::GetContactData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::GetContactData::Request>("request",false);
component<::uwvm_fourier_bridge::GetContactData::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/GetFsmName");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::GetFsmName>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::GetFsmName::Request>("request",false);
component<::uwvm_fourier_bridge::GetFsmName::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/GetFsmState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::GetFsmState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::GetFsmState::Request>("request",false);
component<::uwvm_fourier_bridge::GetFsmState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/GetGroupMotionState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::GetGroupMotionState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::GetGroupMotionState::Request>("request",false);
component<::uwvm_fourier_bridge::GetGroupMotionState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/GetGroupMotorCfg");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::GetGroupMotorCfg>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::GetGroupMotorCfg::Request>("request",false);
component<::uwvm_fourier_bridge::GetGroupMotorCfg::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/GetGroupState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::GetGroupState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::GetGroupState::Request>("request",false);
component<::uwvm_fourier_bridge::GetGroupState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/GetSnapshot");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::GetSnapshot>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::GetSnapshot::Request>("request",false);
component<::uwvm_fourier_bridge::GetSnapshot::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/GetStandPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::GetStandPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::GetStandPose::Request>("request",false);
component<::uwvm_fourier_bridge::GetStandPose::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/GetUpperFsmName");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::GetUpperFsmName>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::GetUpperFsmName::Request>("request",false);
component<::uwvm_fourier_bridge::GetUpperFsmName::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/GetUpperFsmState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::GetUpperFsmState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::GetUpperFsmState::Request>("request",false);
component<::uwvm_fourier_bridge::GetUpperFsmState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/GetVelocitySource");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::GetVelocitySource>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::GetVelocitySource::Request>("request",false);
component<::uwvm_fourier_bridge::GetVelocitySource::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/GetVelocitySourceName");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::GetVelocitySourceName>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::GetVelocitySourceName::Request>("request",false);
component<::uwvm_fourier_bridge::GetVelocitySourceName::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/GroupState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::uwvm_fourier_bridge::GroupState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::GroupState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/GroupValues");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::uwvm_fourier_bridge::GroupValues>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::GroupValues>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/MotionRecipe");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::uwvm_fourier_bridge::MotionRecipe>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::MotionRecipe>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/SetFsmState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::SetFsmState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::SetFsmState::Request>("request",false);
component<::uwvm_fourier_bridge::SetFsmState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/SetGroupCommand");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::SetGroupCommand>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::SetGroupCommand::Request>("request",false);
component<::uwvm_fourier_bridge::SetGroupCommand::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/SetMotorCfgPd");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::SetMotorCfgPd>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::SetMotorCfgPd::Request>("request",false);
component<::uwvm_fourier_bridge::SetMotorCfgPd::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/SetStandPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::SetStandPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::SetStandPose::Request>("request",false);
component<::uwvm_fourier_bridge::SetStandPose::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/SetUpperFsmState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::SetUpperFsmState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::SetUpperFsmState::Request>("request",false);
component<::uwvm_fourier_bridge::SetUpperFsmState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/SetVelocity");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::SetVelocity>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::SetVelocity::Request>("request",false);
component<::uwvm_fourier_bridge::SetVelocity::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/SetVelocitySource");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::SetVelocitySource>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::SetVelocitySource::Request>("request",false);
component<::uwvm_fourier_bridge::SetVelocitySource::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/Status");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::uwvm_fourier_bridge::Status>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::Status>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/VelocityCommand");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::uwvm_fourier_bridge::VelocityCommand>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::VelocityCommand>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("uwvm_fourier_bridge/WaitGroupsMotionComplete");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::uwvm_fourier_bridge::WaitGroupsMotionComplete>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::uwvm_fourier_bridge::WaitGroupsMotionComplete::Request>("request",false);
component<::uwvm_fourier_bridge::WaitGroupsMotionComplete::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),"}}\n");return 0;
}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\n");return 1;}}
