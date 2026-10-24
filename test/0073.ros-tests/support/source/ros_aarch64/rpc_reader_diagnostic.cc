#include "/work/test/0073.ros-tests/support/source/ros_resources/common.h"
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <uwvm2/imported/ros/ipc/parameter_event.h>
using namespace ros_resources_probe;
// Prepared from the exact frozen original header. The only semantic addition
// is a graph dump before the unchanged rejected-GID assertion.
#include "/tmp/addon/typed_event_witness_diagnostic.h"

int main(int argc,char** argv)
{
    try
    {
        require(argc>=2&&std::string{argv[1]}=="--typed-events","diagnostic supports original typed event witness only");
        return ros2_typed_event_witness(argc,argv);
    }
    catch(std::exception const& error)
    { std::cerr<<"FAIL "<<error.what()<<'\n'<<std::flush;return 1; }
}
