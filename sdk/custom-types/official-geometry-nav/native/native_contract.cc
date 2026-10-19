/** @file Actual complete geometry/native codec contract, not ROS delivery.
 * Compile only with the actual bound family bundle and the matching installed
 * SDK. Generated static traits check every field type/order/fixed capacity.
 * The complete introspection graph must agree before an old ROS2 RIHS identity
 * can match. A modern missing/bad native hash never selects the legacy path.
 * No ROS Node, clock, publisher, network operation or robot command is created.
 */
#include "tests/ros_custom_types/bounded-v2/native_protocol.h"
#include <cstdint>
#include <string_view>
#include <vector>
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
#include "native_ros1.hpp"
#elif UWVM_CUSTOM_NATIVE_ROS_VERSION == 2
#include "native_ros2.hpp"
#include "gateways/ros2/compat/identity.h"
#else
#error "Select exactly one real installed ROS SDK and matching generated bundle"
#endif
namespace {
unsigned checks{},failures{};
void check(bool value,char const* reason){++checks;if(!value){++failures;bounded_native_io::print(STDERR_FILENO,"FAIL_GEOMETRY_NATIVE ",reason,'\n');}}
template<class Native,class Codec> bool actual_identity() {
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    return std::string_view{Codec::type}==::ros::message_traits::DataType<Native>::value()&&
        std::string_view{Codec::native_identity}==::ros::message_traits::MD5Sum<Native>::value();
#else
    namespace compat=::uwvm2::ros_gateway::ros2_compat;
    auto support=::rosidl_typesupport_cpp::get_message_type_support_handle<Native>();
    if(!support||!compat::matches_schema(compat::message_members_for<Native>(),Codec::native_schema(),Codec::type))return false;
    if constexpr(compat::has_baked_hash<std::remove_pointer_t<decltype(support)>>::value)
        return compat::baked_hash(support)==Codec::native_identity;
    else return compat::matches_rihs(Codec::native_schema(),Codec::type,Codec::native_identity);
#endif
}
template<class Native,class Codec> void run(Native const& value) {
    check(actual_identity<Native,Codec>(),"actual installed full declaration and identity");
    std::vector<std::uint8_t> wire;check(Codec::encode(value,wire),"complete native encode");
    Native decoded{};check(Codec::decode(wire.data(),wire.size(),decoded),"complete native decode");
    check(value==decoded,"every actual native field preserved");
    auto before=decoded;auto original=wire;wire.push_back(0);
    check(!Codec::decode(wire.data(),wire.size(),decoded),"trailing byte refused");
    check(decoded==before,"trailing failure preserves complete destination");
    if(!original.empty()){
        original.pop_back();check(!Codec::decode(original.data(),original.size(),decoded),"truncated complete record refused");
        check(decoded==before,"truncation preserves complete destination");
    }
}
// Configure meaningful complete fixtures; remaining type instantiations also
// compile/validate all fields and exercise exact zero/default-value roundtrips.
template<class Header> void header(Header& v) {
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    v.seq=19;v.stamp.sec=42;v.stamp.nsec=300000000;
#else
    v.stamp.sec=-2;v.stamp.nanosec=300000000;
#endif
    v.frame_id="map";
}
template<class Vector> void xyz(Vector& v,double a,double b,double c){v.x=a;v.y=b;v.z=c;}
template<class Value> void twist(Value& v){xyz(v.linear,1.25,-2.5,3.75);xyz(v.angular,-0.125,0.25,-0.5);}
template<class Value> void covariance(Value& v){for(unsigned i{};i!=36;++i)v.covariance[i]=(static_cast<int>(i)-18)*0.125;}
}
int main() {
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    run<::geometry_msgs::Accel,::uwvm_custom_bounded_native::Ros1Geometry_msgsAccelCodec>(::geometry_msgs::Accel{});
    run<::geometry_msgs::AccelStamped,::uwvm_custom_bounded_native::Ros1Geometry_msgsAccelStampedCodec>(::geometry_msgs::AccelStamped{});
    run<::geometry_msgs::AccelWithCovariance,::uwvm_custom_bounded_native::Ros1Geometry_msgsAccelWithCovarianceCodec>(::geometry_msgs::AccelWithCovariance{});
    run<::geometry_msgs::AccelWithCovarianceStamped,::uwvm_custom_bounded_native::Ros1Geometry_msgsAccelWithCovarianceStampedCodec>(::geometry_msgs::AccelWithCovarianceStamped{});
    run<::geometry_msgs::Point,::uwvm_custom_bounded_native::Ros1Geometry_msgsPointCodec>(::geometry_msgs::Point{});
    run<::geometry_msgs::Point32,::uwvm_custom_bounded_native::Ros1Geometry_msgsPoint32Codec>(::geometry_msgs::Point32{});
    run<::geometry_msgs::PointStamped,::uwvm_custom_bounded_native::Ros1Geometry_msgsPointStampedCodec>(::geometry_msgs::PointStamped{});
    run<::geometry_msgs::Polygon,::uwvm_custom_bounded_native::Ros1Geometry_msgsPolygonCodec>(::geometry_msgs::Polygon{});
    run<::geometry_msgs::PolygonStamped,::uwvm_custom_bounded_native::Ros1Geometry_msgsPolygonStampedCodec>(::geometry_msgs::PolygonStamped{});
    run<::geometry_msgs::Pose,::uwvm_custom_bounded_native::Ros1Geometry_msgsPoseCodec>(::geometry_msgs::Pose{});
    run<::geometry_msgs::PoseArray,::uwvm_custom_bounded_native::Ros1Geometry_msgsPoseArrayCodec>(::geometry_msgs::PoseArray{});
    run<::geometry_msgs::PoseStamped,::uwvm_custom_bounded_native::Ros1Geometry_msgsPoseStampedCodec>(::geometry_msgs::PoseStamped{});
    run<::geometry_msgs::PoseWithCovariance,::uwvm_custom_bounded_native::Ros1Geometry_msgsPoseWithCovarianceCodec>(::geometry_msgs::PoseWithCovariance{});
    run<::geometry_msgs::PoseWithCovarianceStamped,::uwvm_custom_bounded_native::Ros1Geometry_msgsPoseWithCovarianceStampedCodec>(::geometry_msgs::PoseWithCovarianceStamped{});
    run<::geometry_msgs::Quaternion,::uwvm_custom_bounded_native::Ros1Geometry_msgsQuaternionCodec>(::geometry_msgs::Quaternion{});
    run<::geometry_msgs::QuaternionStamped,::uwvm_custom_bounded_native::Ros1Geometry_msgsQuaternionStampedCodec>(::geometry_msgs::QuaternionStamped{});
    run<::geometry_msgs::Transform,::uwvm_custom_bounded_native::Ros1Geometry_msgsTransformCodec>(::geometry_msgs::Transform{});
    run<::geometry_msgs::TransformStamped,::uwvm_custom_bounded_native::Ros1Geometry_msgsTransformStampedCodec>(::geometry_msgs::TransformStamped{});
    run<::geometry_msgs::Twist,::uwvm_custom_bounded_native::Ros1Geometry_msgsTwistCodec>(::geometry_msgs::Twist{});
    run<::geometry_msgs::TwistStamped,::uwvm_custom_bounded_native::Ros1Geometry_msgsTwistStampedCodec>(::geometry_msgs::TwistStamped{});
    run<::geometry_msgs::TwistWithCovariance,::uwvm_custom_bounded_native::Ros1Geometry_msgsTwistWithCovarianceCodec>(::geometry_msgs::TwistWithCovariance{});
    run<::geometry_msgs::TwistWithCovarianceStamped,::uwvm_custom_bounded_native::Ros1Geometry_msgsTwistWithCovarianceStampedCodec>(::geometry_msgs::TwistWithCovarianceStamped{});
    run<::geometry_msgs::Vector3,::uwvm_custom_bounded_native::Ros1Geometry_msgsVector3Codec>(::geometry_msgs::Vector3{});
    run<::geometry_msgs::Vector3Stamped,::uwvm_custom_bounded_native::Ros1Geometry_msgsVector3StampedCodec>(::geometry_msgs::Vector3Stamped{});
    run<::geometry_msgs::Wrench,::uwvm_custom_bounded_native::Ros1Geometry_msgsWrenchCodec>(::geometry_msgs::Wrench{});
    run<::geometry_msgs::WrenchStamped,::uwvm_custom_bounded_native::Ros1Geometry_msgsWrenchStampedCodec>(::geometry_msgs::WrenchStamped{});
    run<::nav_msgs::Odometry,::uwvm_custom_bounded_native::Ros1Nav_msgsOdometryCodec>(::nav_msgs::Odometry{});
    run<::nav_msgs::Path,::uwvm_custom_bounded_native::Ros1Nav_msgsPathCodec>(::nav_msgs::Path{});
#endif
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 2
    run<::geometry_msgs::msg::Accel,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgAccelCodec>(::geometry_msgs::msg::Accel{});
    run<::geometry_msgs::msg::AccelStamped,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgAccelStampedCodec>(::geometry_msgs::msg::AccelStamped{});
    run<::geometry_msgs::msg::AccelWithCovariance,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgAccelWithCovarianceCodec>(::geometry_msgs::msg::AccelWithCovariance{});
    run<::geometry_msgs::msg::AccelWithCovarianceStamped,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgAccelWithCovarianceStampedCodec>(::geometry_msgs::msg::AccelWithCovarianceStamped{});
    run<::geometry_msgs::msg::Point,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgPointCodec>(::geometry_msgs::msg::Point{});
    run<::geometry_msgs::msg::Point32,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgPoint32Codec>(::geometry_msgs::msg::Point32{});
    run<::geometry_msgs::msg::PointStamped,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgPointStampedCodec>(::geometry_msgs::msg::PointStamped{});
    run<::geometry_msgs::msg::Polygon,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgPolygonCodec>(::geometry_msgs::msg::Polygon{});
    run<::geometry_msgs::msg::PolygonStamped,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgPolygonStampedCodec>(::geometry_msgs::msg::PolygonStamped{});
    run<::geometry_msgs::msg::Twist,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgTwistCodec>(::geometry_msgs::msg::Twist{});
    run<::geometry_msgs::msg::TwistStamped,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgTwistStampedCodec>(::geometry_msgs::msg::TwistStamped{});
    run<::geometry_msgs::msg::TwistWithCovariance,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgTwistWithCovarianceCodec>(::geometry_msgs::msg::TwistWithCovariance{});
    run<::geometry_msgs::msg::TwistWithCovarianceStamped,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgTwistWithCovarianceStampedCodec>(::geometry_msgs::msg::TwistWithCovarianceStamped{});
    run<::geometry_msgs::msg::Vector3,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgVector3Codec>(::geometry_msgs::msg::Vector3{});
    run<::geometry_msgs::msg::Vector3Stamped,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgVector3StampedCodec>(::geometry_msgs::msg::Vector3Stamped{});
    run<::geometry_msgs::msg::Wrench,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgWrenchCodec>(::geometry_msgs::msg::Wrench{});
    run<::geometry_msgs::msg::WrenchStamped,::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgWrenchStampedCodec>(::geometry_msgs::msg::WrenchStamped{});
#endif
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    using Stamped=::geometry_msgs::TwistWithCovarianceStamped;
    using StampedCodec=::uwvm_custom_bounded_native::Ros1Geometry_msgsTwistWithCovarianceStampedCodec;
    using Polygon=::geometry_msgs::PolygonStamped;
    using PolygonCodec=::uwvm_custom_bounded_native::Ros1Geometry_msgsPolygonStampedCodec;
#else
    using Stamped=::geometry_msgs::msg::TwistWithCovarianceStamped;
    using StampedCodec=::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgTwistWithCovarianceStampedCodec;
    using Polygon=::geometry_msgs::msg::PolygonStamped;
    using PolygonCodec=::uwvm_custom_bounded_native::Ros2Geometry_msgsMsgPolygonStampedCodec;
#endif
    Stamped stamped{};header(stamped.header);twist(stamped.twist.twist);covariance(stamped.twist);
    run<Stamped,StampedCodec>(stamped);
    Polygon polygon{};header(polygon.header);polygon.polygon.points.resize(3);
    xyz(polygon.polygon.points[0],1.25,-2.5,3.75);xyz(polygon.polygon.points[1],4.5,5.25,-6.125);xyz(polygon.polygon.points[2],-7.75,8.5,9.25);
    run<Polygon,PolygonCodec>(polygon);
    auto too_large=polygon;too_large.polygon.points.resize(65);std::size_t extent=777;
    check(!PolygonCodec::encoded_size(too_large,extent)&&extent==777,"nested point sequence bound checked before allocation");
    std::vector<std::uint8_t> original{1,2,3},out=original;
    check(!PolygonCodec::encode(too_large,out)&&out==original,"invalid nested full record preserves output");
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    using Odom=::nav_msgs::Odometry;using OdomCodec=::uwvm_custom_bounded_native::Ros1Nav_msgsOdometryCodec;
    Odom odom{};header(odom.header);odom.child_frame_id="base";
    xyz(odom.pose.pose.position,10.25,-20.5,30.75);odom.pose.pose.orientation.x=0.125;
    odom.pose.pose.orientation.y=-0.25;odom.pose.pose.orientation.z=0.5;odom.pose.pose.orientation.w=0.75;
    covariance(odom.pose);twist(odom.twist.twist);covariance(odom.twist);
    run<Odom,OdomCodec>(odom);
    auto invalid=odom;invalid.child_frame_id.assign(129,'x');extent=777;
    check(!OdomCodec::encoded_size(invalid,extent)&&extent==777,"complete child frame bound checked without truncation");
#endif
    bounded_native_io::print(STDOUT_FILENO,"PASS_GEOMETRY_NATIVE_FULL_VALUES checks=",checks," failures=",failures,'\n');
    return failures?1:0;
}
