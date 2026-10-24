/* SPDX-License-Identifier: Apache-2.0
 * Real C++20 generated Codec ownership and boundary checks. No mock transport.
 */
#ifdef NDEBUG
#error "Contract assertions must remain enabled"
#endif
#include <cassert>
#include <cstring>
#include "scout.hpp"
int main() {
    using namespace roswasm_agilex::scout;
    Status value{},decoded{};value.linear_velocity=0.25;value.angular_velocity=-0.5;
    value.header.frame_id.size=5;std::memcpy(value.header.frame_id.data,"base0",5);
#if UWVM_SCOUT_ROS_VERSION == 1
    value.lateral_velocity=0.125;value.motor_states[3].motor_pose=-7.25;
#else
    value.actuator_states[3].motor_temperature=-19;
#endif
    roswasmcpp::wire::bytes bytes;
    assert(roswasmcpp::Codec<Status>::encode(value,bytes));
    assert(roswasmcpp::Codec<Status>::decode(bytes,decoded));
    assert(decoded.linear_velocity==0.25&&decoded.angular_velocity==-0.5);
    assert(decoded.header.frame_id.size==5&&!std::memcmp(decoded.header.frame_id.data,"base0",5));
#if UWVM_SCOUT_ROS_VERSION == 1
    assert(decoded.lateral_velocity==0.125&&decoded.motor_states[3].motor_pose==-7.25);
#else
    assert(decoded.actuator_states[3].motor_temperature==-19);
#endif
    Status before=decoded;bytes.pop_back();
    assert(!roswasmcpp::Codec<Status>::decode(bytes,decoded));
    assert(!std::memcmp(&before,&decoded,sizeof decoded));
    auto output=bytes;value.header.frame_id.size=129;
    assert(!roswasmcpp::Codec<Status>::encode(value,output)&&output==bytes);
#if UWVM_SCOUT_ROS_VERSION == 2
    auto odom=roswasmcpp::Codec<Odometry>::default_value();assert(odom.pose.pose.orientation.w==1);
#endif
    return 0;
}
