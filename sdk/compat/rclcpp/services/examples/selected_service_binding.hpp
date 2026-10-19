/** Explicit source binding ONLY, not a Host grant or a discovery result.
 * Actual native identity/QoS/default service profile must be independently
 * admitted. Separate aliases authorize call, observe, advertise, respond, log.
 */
#pragma once
#include <roswasm/ros2.h>
#include <fast_io.h>
namespace roswasm_rclcpp {
inline std::uint32_t service_stdio_step(void*){
    char byte{};auto end=::fast_io::operations::read_some(::fast_io::posix_stdin(),std::addressof(byte),std::addressof(byte)+1);
    if(end==std::addressof(byte))return 110;return byte=='R'?0:byte=='Q'?125:22;
}
inline Selection const* selected_configuration()noexcept{
    static NodeBinding const nodes[]{{"minimal_service","","service-info-log"},{"minimal_client","","client-info-log"}};
    static ServiceBinding const rows[]{
        {"minimal_service","add_two_ints","example_interfaces/srv/AddTwoInts","official-default-service-profile",ServiceRole::server,"","","service-advertise","service-respond",4},
        {"minimal_client","add_two_ints","example_interfaces/srv/AddTwoInts","official-default-service-profile",ServiceRole::client,"service-call","service-observe","","",4}};
    static ServiceProfile const services{roswasmc_ros2_event_backend(),rows};
    static Selection const selected{roswasmc_ros2_backend(),{},nodes,service_stdio_step,nullptr,64,&services};return &selected;
}
}
