#include "../../gateways/ros2/environment.h"
#include "../../gateways/common/native_io.h"
#include <string_view>

namespace env=uwvm2::ros_gateway::ros2_environment;
namespace
{
    unsigned checks{};
    bool equal(char const* name,std::string_view expected)
    {
        auto value{::getenv(name)};
        return value!=nullptr&&std::string_view{value}==expected;
    }
    bool require(bool condition)
    {
        ++checks;
        return condition;
    }
}

int main()
{
    // This process uses only synthetic environment values. It neither reads
    // nor prints inherited credentials, creates a ROS context or owns a socket.
    env::set("ROS_SECURITY_ENCLAVE_OVERRIDE","/wrong-test-identity");
    if(!require(equal("ROS_SECURITY_ENCLAVE_OVERRIDE","/wrong-test-identity"))) { return 1; }
    env::clear_enclave_override();
    if(!require(::getenv("ROS_SECURITY_ENCLAVE_OVERRIDE")==nullptr)) { return 2; }
    env::clear_enclave_override();
    if(!require(::getenv("ROS_SECURITY_ENCLAVE_OVERRIDE")==nullptr)) { return 3; }

    // Overwrite inherited/synthetic values, using the same checked functions
    // as both Gateway security branches. Actual rcl/DDS identity is a separate
    // native Enforce acceptance test, not established by getenv equality.
    env::set("RMW_IMPLEMENTATION","wrong-test-rmw");
    env::set("RMW_IMPLEMENTATION","rmw_fastrtps_cpp");
    if(!require(equal("RMW_IMPLEMENTATION","rmw_fastrtps_cpp"))) { return 4; }
    env::set("ROS_DOMAIN_ID","0");env::set("ROS_DOMAIN_ID","74");
    if(!require(equal("ROS_DOMAIN_ID","74"))) { return 5; }
    env::set("ROS_SECURITY_ENABLE","false");env::set("ROS_SECURITY_ENABLE","true");
    if(!require(equal("ROS_SECURITY_ENABLE","true"))) { return 6; }
    env::set("ROS_SECURITY_STRATEGY","Enforce");
    if(!require(equal("ROS_SECURITY_STRATEGY","Enforce"))) { return 7; }
    env::set("ROS_SECURITY_KEYSTORE","/synthetic-test-keystore");
    if(!require(equal("ROS_SECURITY_KEYSTORE","/synthetic-test-keystore"))) { return 8; }
    env::set("ROS_SECURITY_ENABLE","false");env::clear("ROS_SECURITY_STRATEGY");env::clear("ROS_SECURITY_KEYSTORE");
    if(!require(equal("ROS_SECURITY_ENABLE","false"))) { return 9; }
    if(!require(::getenv("ROS_SECURITY_STRATEGY")==nullptr)) { return 10; }
    if(!require(::getenv("ROS_SECURITY_KEYSTORE")==nullptr)) { return 11; }

    bool set_refused{},clear_refused{};
    try { env::set("","value"); }
    catch(std::runtime_error const&) { set_refused=true; }
    try { env::clear("invalid=name"); }
    catch(std::runtime_error const&) { clear_refused=true; }
    if(!require(set_refused)) { return 12; }
    if(!require(clear_refused)) { return 13; }
    if(checks!=13u) { return 14; }
    uwvm2::ros_gateway::native_io::fixed_text<128> line;
    if(!line.append("PASS C++17 ROS2 checked environment contract checks=")||
       !line.number(checks)||!line.append("\n")||
       !uwvm2::ros_gateway::native_io::write_all(STDOUT_FILENO,line.view())) { return 15; }
    return 0;
}
