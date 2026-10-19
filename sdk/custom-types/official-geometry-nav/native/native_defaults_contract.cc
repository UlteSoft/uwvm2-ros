/** Actual installed modern ROS2 metadata/default/value contract.
 * Build with the real exported-and-bound complete family; no synthetic native
 * provider, hash, TypeDescription or introspection implementation is supplied.
 * No Node, endpoint, grant, ROS graph or network operation is created here.
 */
#include <fast_io.h> // This real Gateway-header contract selects C++20 explicitly.
#include "gateways/ros2/custom_topics.h"
#include "tests/ros_custom_types/bounded-v2/native_protocol.h"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <array>
#include <vector>
#if !UWVM_ROS2_HAVE_TYPE_DESCRIPTION
#error "Declared defaults require actual modern complete TypeDescription; nullptr introspection is UNKNOWN"
#endif
namespace ipc=::uwvm2::imported::ros::ipc;
namespace gateway=::uwvm2::ros_gateway::ros2_custom_topics;
namespace compat=::uwvm2::ros_gateway::ros2_compat;
namespace native=::uwvm_custom_bounded_native;
namespace {
unsigned checks{},failures{};
void check(bool ok,char const* why){++checks;if(!ok){++failures;bounded_native_io::print(STDERR_FILENO,"FAIL_NATIVE_DEFAULT ",why,'\n');}}
template<class Native,class Codec> void full_value(Native const& value) {
    check(gateway::binding<Native,Codec>::native_contract(),"actual baked identity AND full TypeDescription/default closure");
    std::vector<std::uint8_t> bytes;check(Codec::encode(value,bytes),"full native value encoded");
    Native decoded{};check(Codec::decode(bytes.data(),bytes.size(),decoded)&&decoded==value,"all fields roundtrip");
    auto before=decoded;auto truncated=bytes;truncated.pop_back();
    check(!Codec::decode(truncated.data(),truncated.size(),decoded)&&decoded==before,"missing data never filled from declared defaults");
    bytes.push_back(0);check(!Codec::decode(bytes.data(),bytes.size(),decoded)&&decoded==before,"trailing bytes preserve destination");
}
}
int main(){
    using Q=::geometry_msgs::msg::Quaternion;using QC=native::Ros2Geometry_msgsMsgQuaternionCodec;
    Q all(::rosidl_runtime_cpp::MessageInitialization::ALL),defaults(::rosidl_runtime_cpp::MessageInitialization::DEFAULTS_ONLY),zero(::rosidl_runtime_cpp::MessageInitialization::ZERO);
    check(all.x==0&&all.y==0&&all.z==0&&all.w==1,"actual ALL constructor preserves original declared variant");
    check(defaults.x==0&&defaults.y==0&&defaults.z==0&&defaults.w==1,"actual DEFAULTS_ONLY constructor preserves declared defaults");
    check(zero.x==0&&zero.y==0&&zero.z==0&&zero.w==0,"ZERO constructor remains distinct from declared initialization");
    roswasmc_bounded_2_geometry_msgs_msg_Quaternion guest{};
    check(!roswasmc_bounded_2_geometry_msgs_msg_Quaternion_default(&guest)&&guest.x==all.x&&guest.y==all.y&&guest.z==all.z&&guest.w==all.w,"real C value initializer equals actual native defaults");
    full_value<Q,QC>(all);Q arbitrary;arbitrary.x=.125;arbitrary.y=-.25;arbitrary.z=.5;arbitrary.w=.75;
    full_value<Q,QC>(arbitrary); // Defaults initialize; they never constrain arbitrary full payload values.
    auto support=::rosidl_typesupport_cpp::get_message_type_support_handle<Q>();
    auto description=support&&support->get_type_description_func?support->get_type_description_func(support):nullptr;
    check(description!=nullptr,"real modern description is mandatory");
    auto schema=QC::native_schema();
    check(!compat::matches_schema(compat::message_members_for<Q>(),schema,QC::type),"legacy default pointer absence cannot attest declared defaults");
    if(description&&schema.record_count==1&&schema.records[0].field_count==4){
        std::array<ipc::custom_native_field,4> fields{};
        for(std::size_t i{};i!=4;++i)fields[i]=schema.records[0].fields[i];
        auto record=schema.records[0];record.fields=fields.data();ipc::custom_native_schema altered{&record,1};
        fields[3].native_default="0.0";
        check(!gateway::matches_native_schema(*description,altered,QC::type),"same baked RIHS cannot hide wrong normalized default");
        fields[3].native_default="1";
        check(!gateway::matches_native_schema(*description,altered,QC::type),"raw source spelling cannot substitute normalized TypeDescription literal");
        fields[3].native_default={};
        check(!gateway::matches_native_schema(*description,altered,QC::type),"partial default erasure refused");
        fields[3].native_default="1.0";
        for(auto& field:fields)field.native_default={};
        check(!gateway::matches_native_schema(*description,altered,QC::type),"whole default erasure refused independently of native hash");
    }else check(false,"whole actual Quaternion schema required");
    full_value<::geometry_msgs::msg::Pose,native::Ros2Geometry_msgsMsgPoseCodec>({});
    full_value<::geometry_msgs::msg::PoseArray,native::Ros2Geometry_msgsMsgPoseArrayCodec>({});
    full_value<::geometry_msgs::msg::PoseStamped,native::Ros2Geometry_msgsMsgPoseStampedCodec>({});
    full_value<::geometry_msgs::msg::PoseWithCovariance,native::Ros2Geometry_msgsMsgPoseWithCovarianceCodec>({});
    full_value<::geometry_msgs::msg::PoseWithCovarianceStamped,native::Ros2Geometry_msgsMsgPoseWithCovarianceStampedCodec>({});
    full_value<::geometry_msgs::msg::QuaternionStamped,native::Ros2Geometry_msgsMsgQuaternionStampedCodec>({});
    full_value<::geometry_msgs::msg::Transform,native::Ros2Geometry_msgsMsgTransformCodec>({});
    full_value<::geometry_msgs::msg::TransformStamped,native::Ros2Geometry_msgsMsgTransformStampedCodec>({});
    ::nav_msgs::msg::Odometry odom;odom.child_frame_id="base";odom.pose.pose.orientation=arbitrary;
    for(unsigned i{};i!=36;++i){odom.pose.covariance[i]=(static_cast<int>(i)-18)*.125;odom.twist.covariance[i]=i*.0625;}
    full_value<::nav_msgs::msg::Odometry,native::Ros2Nav_msgsMsgOdometryCodec>(odom);
    ::nav_msgs::msg::Path path;path.header.frame_id="map";path.poses.resize(2);path.poses[0].pose.orientation=arbitrary;
    path.poses[1].pose.orientation.w=1;
    full_value<::nav_msgs::msg::Path,native::Ros2Nav_msgsMsgPathCodec>(path);
    bounded_native_io::print(STDOUT_FILENO,"NATIVE_QUATERNION_DEFAULT_CONTRACT checks=",checks," failures=",failures,'\n');return failures?1:0;
}
