/* Additive Guest SOURCE service profile. This header is included by compat.h.
 * Native public type/signature spellings retain Apache-2.0 ROS attributions in
 * services/README.md and the exact official source copies. uwvm fields are
 * private storage, not native rcl/rmw ABI. No native identity is manufactured. */
#ifndef UWVM_RCLC_SERVICES_H
#define UWVM_RCLC_SERVICES_H
#include <roswasm/events.h>
#include <roswasm/codec.h>
#define UWVM_RCLC_MAX_SERVICE_REQUESTS 64u

typedef struct example_interfaces__srv__AddTwoInts_Request {int64_t a,b;} example_interfaces__srv__AddTwoInts_Request;
typedef struct example_interfaces__srv__AddTwoInts_Response {int64_t sum;} example_interfaces__srv__AddTwoInts_Response;
/* The empty ROS request has no canonical wire member. The actual rosidl IDL
 * adapter inserts this uint8 C storage placeholder; it is never serialized. */
typedef struct std_srvs__srv__Trigger_Request {uint8_t structure_needs_at_least_one_member;} std_srvs__srv__Trigger_Request;
typedef struct std_srvs__srv__Trigger_Response {bool success;rosidl_runtime_c__String message;} std_srvs__srv__Trigger_Response;
typedef struct std_srvs__srv__SetBool_Request {bool data;} std_srvs__srv__SetBool_Request;
typedef struct std_srvs__srv__SetBool_Response {bool success;rosidl_runtime_c__String message;} std_srvs__srv__SetBool_Response;
#define UWVM_RCLC_DECLARE_RECORD(T) \
 typedef struct T##__Sequence {T*data;size_t size,capacity;} T##__Sequence; \
 bool T##__init(T*);void T##__fini(T*);T*T##__create(void);void T##__destroy(T*); \
 bool T##__copy(T const*,T*);bool T##__are_equal(T const*,T const*); \
 bool T##__Sequence__init(T##__Sequence*,size_t);void T##__Sequence__fini(T##__Sequence*); \
 T##__Sequence*T##__Sequence__create(size_t);void T##__Sequence__destroy(T##__Sequence*); \
 bool T##__Sequence__copy(T##__Sequence const*,T##__Sequence*); \
 bool T##__Sequence__are_equal(T##__Sequence const*,T##__Sequence const*)
UWVM_RCLC_DECLARE_RECORD(example_interfaces__srv__AddTwoInts_Request);
UWVM_RCLC_DECLARE_RECORD(example_interfaces__srv__AddTwoInts_Response);
UWVM_RCLC_DECLARE_RECORD(std_srvs__srv__Trigger_Request);
UWVM_RCLC_DECLARE_RECORD(std_srvs__srv__Trigger_Response);
UWVM_RCLC_DECLARE_RECORD(std_srvs__srv__SetBool_Request);
UWVM_RCLC_DECLARE_RECORD(std_srvs__srv__SetBool_Response);
#undef UWVM_RCLC_DECLARE_RECORD

typedef struct rosidl_service_type_support_t rosidl_service_type_support_t;
typedef rosidl_service_type_support_t const*(*rosidl_service_typesupport_handle_function)(rosidl_service_type_support_t const*,char const*);
typedef struct rosidl_service_introspection_info_s {uint8_t event_type;int32_t stamp_sec;uint32_t stamp_nanosec;uint8_t client_gid[16];int64_t sequence_number;} rosidl_service_introspection_info_t;
typedef void*(*rosidl_event_message_create_handle_function_function)(rosidl_service_introspection_info_t const*,rcutils_allocator_t*,void const*,void const*);
typedef bool(*rosidl_event_message_destroy_handle_function_function)(void*,rcutils_allocator_t*);
typedef rosidl_type_hash_t const*(*rosidl_service_get_type_hash_function)(rosidl_service_type_support_t const*);
typedef rosidl_runtime_c__type_description__TypeDescription const*(*rosidl_service_get_type_description_function)(rosidl_service_type_support_t const*);
typedef rosidl_runtime_c__type_description__TypeSource__Sequence const*(*rosidl_service_get_type_description_sources_function)(rosidl_service_type_support_t const*);
struct rosidl_service_type_support_t {
 char const*typesupport_identifier;void const*data;rosidl_service_typesupport_handle_function func;
 rosidl_message_type_support_t const*request_typesupport,*response_typesupport,*event_typesupport;
 rosidl_event_message_create_handle_function_function event_message_create_handle_function;
 rosidl_event_message_destroy_handle_function_function event_message_destroy_handle_function;
 rosidl_service_get_type_hash_function get_type_hash_func;
 rosidl_service_get_type_description_function get_type_description_func;
 rosidl_service_get_type_description_sources_function get_type_description_sources_func;
};
#define UWVM_RCLC_DECLARE_SUPPORT(P,N) \
 rosidl_service_type_support_t const*rosidl_typesupport_c__get_service_type_support_handle__##P##__srv__##N(void); \
 rosidl_message_type_support_t const*rosidl_typesupport_c__get_message_type_support_handle__##P##__srv__##N##_Request(void); \
 rosidl_message_type_support_t const*rosidl_typesupport_c__get_message_type_support_handle__##P##__srv__##N##_Response(void)
UWVM_RCLC_DECLARE_SUPPORT(example_interfaces,AddTwoInts);
UWVM_RCLC_DECLARE_SUPPORT(std_srvs,Trigger);
UWVM_RCLC_DECLARE_SUPPORT(std_srvs,SetBool);
#undef UWVM_RCLC_DECLARE_SUPPORT
#define ROSIDL_GET_SRV_TYPE_SUPPORT(P,S,N) rosidl_typesupport_c__get_service_type_support_handle__##P##__##S##__##N()

/* These are the real public field spellings, never populated with invented
 * native GUIDs/timestamps. All header-dependent APIs below refuse with95. */
#ifndef RMW_GID_STORAGE_SIZE
#define RMW_GID_STORAGE_SIZE 16u
#endif
typedef struct rmw_request_id_s {uint8_t writer_guid[RMW_GID_STORAGE_SIZE];int64_t sequence_number;} rmw_request_id_t;
typedef int64_t rmw_time_point_value_t;
typedef struct rmw_service_info_s {rmw_time_point_value_t source_timestamp,received_timestamp;rmw_request_id_t request_id;} rmw_service_info_t;

typedef enum uwvm_rclc_service_role {UWVM_RCLC_SERVICE_SERVER,UWVM_RCLC_SERVICE_CLIENT} uwvm_rclc_service_role;
typedef struct uwvm_rclc_service_binding {
 char const*node;char const*namespace_;char const*service;char const*type;
 uwvm_rclc_service_role role;
 /* Server: advertise/respond. Client: service.call/event.observe. Independent
  * exact aliases; a source's grant does not confer any observer rights. */
 char const*source_alias;char const*auxiliary_alias;rmw_qos_profile_t qos;
 size_t maximum_wire_bytes,maximum_requests;
} uwvm_rclc_service_binding;
struct uwvm_rclc_service_profile {
 roswasmc_event_backend const*events;
 uwvm_rclc_service_binding const*bindings;size_t binding_count;
 /* Physical sleep is opt-in, uses the independently granted steady clock and
  * selected driver. Zero/absent limits refuse nonzero sleep before sending. */
 uint64_t maximum_sleep_ns;size_t maximum_sleep_steps;
};
typedef struct uwvm_rclc_service_state uwvm_rclc_service_state;
typedef struct rcl_client_s {rcl_node_t const*uwvm_node;uint64_t uwvm_generation;uwvm_rclc_service_state*uwvm_state;} rcl_client_t;
typedef struct rcl_service_s {rcl_node_t const*uwvm_node;uint64_t uwvm_generation;uwvm_rclc_service_state*uwvm_state;} rcl_service_t;
typedef struct rcl_client_options_s {rmw_qos_profile_t qos;rcl_allocator_t allocator;} rcl_client_options_t;
typedef struct rcl_service_options_s {rmw_qos_profile_t qos;rcl_allocator_t allocator;} rcl_service_options_t;
extern rmw_qos_profile_t const rmw_qos_profile_services_default;
rcl_client_t rcl_get_zero_initialized_client(void);
rcl_service_t rcl_get_zero_initialized_service(void);
rcl_client_options_t rcl_client_get_default_options(void);
rcl_service_options_t rcl_service_get_default_options(void);
rcl_ret_t rcl_client_init(rcl_client_t*,rcl_node_t const*,rosidl_service_type_support_t const*,char const*,rcl_client_options_t const*);
rcl_ret_t rcl_service_init(rcl_service_t*,rcl_node_t const*,rosidl_service_type_support_t const*,char const*,rcl_service_options_t const*);
rcl_ret_t rclc_client_init(rcl_client_t*,rcl_node_t const*,rosidl_service_type_support_t const*,char const*,rmw_qos_profile_t const*);
rcl_ret_t rclc_client_init_default(rcl_client_t*,rcl_node_t const*,rosidl_service_type_support_t const*,char const*);
rcl_ret_t rclc_client_init_best_effort(rcl_client_t*,rcl_node_t const*,rosidl_service_type_support_t const*,char const*);
rcl_ret_t rclc_service_init(rcl_service_t*,rcl_node_t const*,rosidl_service_type_support_t const*,char const*,rmw_qos_profile_t const*);
rcl_ret_t rclc_service_init_default(rcl_service_t*,rcl_node_t const*,rosidl_service_type_support_t const*,char const*);
rcl_ret_t rclc_service_init_best_effort(rcl_service_t*,rcl_node_t const*,rosidl_service_type_support_t const*,char const*);
rcl_ret_t rcl_client_fini(rcl_client_t*,rcl_node_t*);
rcl_ret_t rcl_service_fini(rcl_service_t*,rcl_node_t*);
bool rcl_client_is_valid(rcl_client_t const*);
bool rcl_service_is_valid(rcl_service_t const*);
char const*rcl_client_get_service_name(rcl_client_t const*);
char const*rcl_service_get_service_name(rcl_service_t const*);
rcl_client_options_t const*rcl_client_get_options(rcl_client_t const*);
rcl_service_options_t const*rcl_service_get_options(rcl_service_t const*);
/* The output preserves ALL64 opaque Gateway-ticket bits via memcpy, even if
 * its signed C representation is negative. It is NOT an RMW sequence number. */
rcl_ret_t rcl_send_request(rcl_client_t const*,void const*,int64_t*);
rcl_ret_t rcl_take_response(rcl_client_t const*,rmw_request_id_t*,void*);
rcl_ret_t rcl_take_response_with_info(rcl_client_t const*,rmw_service_info_t*,void*);
rcl_ret_t rcl_take_request(rcl_service_t const*,rmw_request_id_t*,void*);
rcl_ret_t rcl_take_request_with_info(rcl_service_t const*,rmw_service_info_t*,void*);
rcl_ret_t rcl_send_response(rcl_service_t const*,rmw_request_id_t*,void*);
rcl_ret_t rcl_service_server_is_available(rcl_node_t const*,rcl_client_t const*,bool*);
void rclc_sleep_ms(unsigned int);
typedef void(*rclc_service_callback_t)(void const*,void*);
typedef void(*rclc_service_callback_with_context_t)(void const*,void*,void*);
typedef void(*rclc_service_callback_with_request_id_t)(void const*,rmw_request_id_t*,void*);
typedef void(*rclc_client_callback_t)(void const*);
typedef void(*rclc_client_callback_with_request_id_t)(void const*,rmw_request_id_t*);
rcl_ret_t rclc_executor_add_service(rclc_executor_t*,rcl_service_t*,void*,void*,rclc_service_callback_t);
rcl_ret_t rclc_executor_add_service_with_context(rclc_executor_t*,rcl_service_t*,void*,void*,rclc_service_callback_with_context_t,void*);
rcl_ret_t rclc_executor_add_service_with_request_id(rclc_executor_t*,rcl_service_t*,void*,void*,rclc_service_callback_with_request_id_t);
rcl_ret_t rclc_executor_add_client(rclc_executor_t*,rcl_client_t*,void*,rclc_client_callback_t);
rcl_ret_t rclc_executor_add_client_with_request_id(rclc_executor_t*,rcl_client_t*,void*,rclc_client_callback_with_request_id_t);
/* Explicit profile diagnostics: local removal does not cancel native work.
 * States:1 pending,2 local tombstone(error125),3 complete decoded response,4 terminal error.
 * Total submissions are bounded across the client's lifetime; slots/IDs are
 * never reused, so duplicate/late records cannot become a new request. */
/* Explicit local C callback-failure channel: no response is sent afterward. */
rcl_ret_t uwvm_rclc_service_callback_error(rcl_service_t*,uint32_t);
rcl_ret_t uwvm_rclc_remove_pending_request(rcl_client_t*,uint64_t);
rcl_ret_t uwvm_rclc_request_status(rcl_client_t const*,uint64_t,unsigned*,uint32_t*);
#endif
