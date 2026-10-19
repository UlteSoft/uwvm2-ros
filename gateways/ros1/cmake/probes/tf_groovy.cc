#include <ros/ros.h>
#include <tf2_ros/buffer.h>
int main()
{
    ::tf2::Buffer buffer{::ros::Duration{1.},false};
    buffer.clear();
    geometry_msgs::TransformStamped transform;
    buffer.setTransform(transform,"compile_only",true);
    buffer.canTransform("parent","child",::ros::Time{});
    buffer.lookupTransform("parent","child",::ros::Time{});
    return 0;
}
