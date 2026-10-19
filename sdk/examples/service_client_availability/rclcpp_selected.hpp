/* SPDX-License-Identifier: Apache-2.0
 * Illustrative explicit source binding, not a Host policy or native identity.
 * Use with the unchanged acquired official minimal_client/main.cpp or actual
 * original minimal_service/main.cpp; their source bytes are not modified here.
 */
#pragma once
#include <roswasm/ros2.h>
#include <roswasm/service_client_ros2.h>
#include <fast_io.h>
namespace roswasm_rclcpp {
inline std::uint32_t available_stdio_step(void*){
 char value{};auto end=::fast_io::operations::read_some(::fast_io::posix_stdin(),std::addressof(value),std::addressof(value)+1);
 if(end==std::addressof(value))return 110;return value=='R'?0:value=='Q'?125:22;
}
inline Selection const* selected_configuration()noexcept{
 static NodeBinding const nodes[]{{"minimal_service","","service-info-log"},{"minimal_client","","client-info-log"}};
 static ServiceBinding const rows[]{
  {"minimal_service","add_two_ints","example_interfaces/srv/AddTwoInts","official-default-service-profile",ServiceRole::server,"","","service-advertise","service-respond",4},
  {"minimal_client","add_two_ints","example_interfaces/srv/AddTwoInts","official-default-service-profile",ServiceRole::client,"service-call","service-observe","","",4,"service-available"}};
 static ServiceProfile const services{roswasmc_ros2_event_backend(),rows,roswasmc_ros2_service_client_backend()};
 static Selection const selected{roswasmc_ros2_backend(),{},nodes,available_stdio_step,nullptr,64,&services};return &selected;
}
}
