// Compile and link only: the std::string-bearing public symbols detect a
// selected libstdc++/roscpp string ABI mismatch without initializing ROS.
#include <ros/ros.h>
#include <string>
int main(int argc,char** argv)
{
    ::ros::init(argc,argv,std::string{"uwvm_ros1_sdk_link_probe"},
        ::ros::init_options::NoSigintHandler|::ros::init_options::NoRosout);
    ::ros::NodeHandle node{std::string{"/"}};
    return node.getNamespace().empty()?1:0;
}
