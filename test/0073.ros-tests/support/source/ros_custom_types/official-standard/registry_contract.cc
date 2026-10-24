/** Compile/run only with the actual freshly generated standard-mode manifest.
 * This native component checks the real immutable registry lookup boundary.
 * It is not a native type identity, Gateway, Runtime/Guest or ROS-delivery test.
 * No fake manifest or synthetic accepted hash is supplied by this source.
 */
#ifndef UWVM_ROS_CUSTOM_TOPIC_MANIFEST
#error "Supply the actual freshly generated manifest after native identity export"
#endif
#include <uwvm2/imported/ros/ipc/custom_type.h>
#include "test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/native_protocol.h"
namespace ipc=uwvm2::imported::ros::ipc;
namespace {
unsigned checks{},failures{};
void check(bool ok,char const* text) {++checks;if(!ok){++failures;bounded_native_io::print(STDERR_FILENO,"FAIL_STANDARD_REGISTRY ",text,'\n');}}
}
int main() {
    check(ipc::custom_topic_registry_valid,"actual generated registry is structurally well formed");
    auto entries=ipc::custom_topic_entries();unsigned targets{};
    for(std::size_t i{};i!=entries.size;++i){auto const& d=entries.data[i];
        if(ipc::official_standard_topic_name(d.ros_version,d.type)){
            ++targets;check(d.official_standard&&d.bounded_layout,"explicit trusted standard marker with full bounded schema");
            check(ipc::find_custom_topic(d.ros_version,d.type)==&d,"actual same descriptor lookup");
        }}
    check(targets>0,"actual selected standard JointState entry exists");
    for(unsigned version:{1u,2u}){
        check(!ipc::custom_topic_name(version,version==1?"sensor_msgs/JointState":"sensor_msgs/msg/JointState"),"old custom grammar remains reserved");
        check(!ipc::find_custom_topic(version,version==1?"std_msgs/Header":"std_msgs/msg/Header"),"Header is a dependency, never a topic registration");
        check(!ipc::official_standard_topic_name(version,version==1?"std_msgs/String":"std_msgs/msg/String"),"existing String builtin cannot opt in");
        check(!ipc::official_standard_topic_name(version,version==1?"geometry_msgs/Twist":"geometry_msgs/msg/Twist"),"existing Twist builtin cannot opt in");
        check(!ipc::official_standard_topic_name(version,version==1?"sensor_msgs/Image":"sensor_msgs/msg/Image"),"existing Image builtin cannot opt in");
    }
    check(!ipc::find_custom_topic(2,"builtin_interfaces/msg/Time"),"Time is a dependency only");
    check(!ipc::find_custom_topic(2,"builtin_interfaces/msg/Duration"),"Duration is a dependency only");
    bounded_native_io::print(STDOUT_FILENO,"PASS_OFFICIAL_STANDARD_REGISTRY checks=",checks," failures=",failures,'\n');
    return failures?1:0;
}
