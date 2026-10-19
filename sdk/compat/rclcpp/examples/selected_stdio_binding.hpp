/** Actual imported ROS2 backend + explicit finite source bindings.
 * This is not a Host policy. Matching per-module grants must separately admit
 * exact String topics, KeepLast10, complete bytes, severity and steady time.
 * Every output must retain this selected source/full7 as a compiler input.
 */
#pragma once
#include <roswasm/ros2.h>
#include <fast_io.h>
namespace roswasm_rclcpp {
inline std::uint32_t stdio_step(void*){
    char byte{};auto end=::fast_io::operations::read_some(::fast_io::posix_stdin(),std::addressof(byte),std::addressof(byte)+1);
    if(end==std::addressof(byte))return 110;return byte=='R'?0:byte=='Q'?125:22;
}
inline Selection const* selected_configuration()noexcept{
    static TopicBinding const topics[]{
        {"minimal_publisher","topic","compat-publish",Role::publish,10,1028},
        {"minimal_subscriber","topic","compat-subscribe",Role::subscribe,10,1028}};
    static NodeBinding const nodes[]{
        {"minimal_publisher","compat-steady","compat-pub-log"},
        {"minimal_subscriber","","compat-sub-log"}};
    static Selection const selection{roswasmc_ros2_backend(),topics,nodes,stdio_step,nullptr,4096};return &selection;
}
}
