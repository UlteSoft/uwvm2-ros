// SPDX-License-Identifier: Apache-2.0
// An actual EXECUTABLE compile AND link; never executed at configure time.
#ifndef __cpp_exceptions
#error "Original C++ exceptions required"
#endif
#include <uwvm_rmw/codec.h>
#include <rcl_interfaces/msg/log.h>
#include <rcl_interfaces/msg/parameter_event.hpp>
#include <rcl_interfaces/srv/describe_parameters.hpp>
#include <rcl_interfaces/srv/get_parameters.hpp>
#include <rcl_interfaces/srv/get_parameter_types.hpp>
#include <rcl_interfaces/srv/list_parameters.hpp>
#include <rcl_interfaces/srv/set_parameters.hpp>
#include <rcl_interfaces/srv/set_parameters_atomically.hpp>
#include <type_description_interfaces/srv/get_type_description.h>
#include <rosidl_typesupport_cpp/message_type_support.hpp>
#include <rosidl_typesupport_cpp/service_type_support.hpp>
#include <rosidl_typesupport_introspection_c/message_introspection.h>
#include <rosidl_typesupport_introspection_c/service_introspection.h>
#include <rosidl_typesupport_introspection_cpp/message_introspection.hpp>
#include <rosidl_typesupport_introspection_cpp/service_introspection.hpp>
#include <rosidl_runtime_c/type_description/type_description__struct.h>
#include <type_traits>
static rosidl_message_type_support_t const*volatile messages[2];
static rosidl_service_type_support_t const*volatile services[7];
static_assert(std::is_same_v<decltype(rcl_interfaces__msg__Log{}.level),uint8_t>);
static_assert(std::is_same_v<decltype(type_description_interfaces__srv__GetTypeDescription_Request{}.include_type_sources),bool>);
int main(){
 messages[0]=ROSIDL_GET_MSG_TYPE_SUPPORT(rcl_interfaces,msg,Log);
 messages[1]=rosidl_typesupport_cpp::get_message_type_support_handle<rcl_interfaces::msg::ParameterEvent>();
 services[0]=ROSIDL_GET_SRV_TYPE_SUPPORT(type_description_interfaces,srv,GetTypeDescription);
 services[1]=rosidl_typesupport_cpp::get_service_type_support_handle<rcl_interfaces::srv::DescribeParameters>();
 services[2]=rosidl_typesupport_cpp::get_service_type_support_handle<rcl_interfaces::srv::GetParameters>();
 services[3]=rosidl_typesupport_cpp::get_service_type_support_handle<rcl_interfaces::srv::GetParameterTypes>();
 services[4]=rosidl_typesupport_cpp::get_service_type_support_handle<rcl_interfaces::srv::ListParameters>();
 services[5]=rosidl_typesupport_cpp::get_service_type_support_handle<rcl_interfaces::srv::SetParameters>();
 services[6]=rosidl_typesupport_cpp::get_service_type_support_handle<rcl_interfaces::srv::SetParametersAtomically>();
 auto m=messages[0];auto s=services[0];
 (void)m->get_type_hash_func(m)->version;(void)m->get_type_description_func(m);(void)m->get_type_description_sources_func(m);
 (void)s->get_type_hash_func(s);(void)s->get_type_description_func(s);(void)s->get_type_description_sources_func(s);
 (void)s->request_typesupport;(void)s->response_typesupport;(void)s->event_typesupport;
 (void)s->event_message_create_handle_function;(void)s->event_message_destroy_handle_function;
 rosidl_typesupport_introspection_c__MessageMembers cm{};rosidl_typesupport_introspection_c__MessageMember cf{};
 rosidl_typesupport_introspection_c__ServiceMembers cs{};
 rosidl_typesupport_introspection_cpp::MessageMembers pm{};rosidl_typesupport_introspection_cpp::MessageMember pf{};
 rosidl_typesupport_introspection_cpp::ServiceMembers ps{};
 (void)cm.has_any_key_member_;(void)cm.init_function;(void)cm.fini_function;(void)cf.is_key_;
 (void)cf.fetch_function;(void)cf.assign_function;(void)cf.resize_function;(void)cf.default_value_;(void)cs.event_members_;
 (void)pm.has_any_key_member_;(void)pm.init_function;(void)pm.fini_function;(void)pf.is_key_;
 (void)pf.fetch_function;(void)pf.assign_function;(void)pf.resize_function;(void)pf.default_value_;(void)ps.event_members_;
 type_description_interfaces__srv__GetTypeDescription_Request request{};
 if(!type_description_interfaces__srv__GetTypeDescription_Request__init(&request))return 12;
 bool actual_default=request.include_type_sources;
 type_description_interfaces__srv__GetTypeDescription_Request__fini(&request);
 return actual_default?0:22;
}
