/** Real generated native bundle + actual installed SDK complete-value oracle.
 * No generated identity is supplied here. The required macro comes ONLY from
 * a freshly native-bound emitted native_ros2.hpp. Compile with actual Root SDK
 * feature proofs, full MD/link/maps and this whole source generation.
 */
// The common manifest is loaded by custom_type.h first. The Gateway then
// includes the ONE selected native provider; do not predefine its row macros.
#if !defined(UWVM_NATIVE_BOUND_CONTRACT_SELECTION) || !defined(UWVM_ROS_CUSTOM_TOPIC_MANIFEST) || !defined(UWVM_ROS_CUSTOM_ROS2_HEADER)
#error "Use the actual paired selected-bundle CMake contract; no direct native include"
#endif
#include "tests/ros_custom_types/bounded-v2/native_protocol.h"
#include <csignal>
#include <cstdlib>
#include "gateways/ros2/custom_topics.h"
#include <example_interfaces/srv/add_two_ints.hpp>
#include <rosidl_typesupport_cpp/service_type_support.hpp>
#include <cassert>
#include <cstdio>
#include <limits>
#include <type_traits>
#if !UWVM_ROS2_HAVE_TYPE_DESCRIPTION
#error "Actual modern native TypeDescription API is required; no downgrade"
#endif
#ifndef UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES
#error "Actual bound generated complete Service inventory is required"
#endif
#ifdef NDEBUG
#error "Compile this native contract with assertions enabled"
#endif
namespace n=uwvm2::ros_gateway::ros2_custom_topics;
namespace ipc=uwvm2::imported::ros::ipc;
unsigned checks{};void check(bool value){++checks;assert(value);if(!value)std::abort();}
#define CONTRACT(Native,Type,Identity,Canonical,Request,Response) \
void family(){ \
 static_assert(std::is_same_v<Native,example_interfaces::srv::AddTwoInts>); \
 check(std::string_view{Type}=="example_interfaces/srv/AddTwoInts"); \
 auto common=ipc::find_custom_service(2,Type); \
 check(common&&ipc::custom_service_shape_valid(*common)); \
 check(common->native_identity==Identity&&common->codec_identity==Canonical); \
 check(common->request->type==Request::type&&common->response->type==Response::type); \
 check(common->request->native_identity==Request::native_identity&&common->response->native_identity==Response::native_identity); \
 check(common->request->codec_identity==Request::codec_identity&&common->response->codec_identity==Response::codec_identity); \
 check(common->request->wire_bytes==16&&common->response->wire_bytes==8); \
 auto support=rosidl_typesupport_cpp::get_service_type_support_handle<Native>(); \
 check(n::native_interface_contract<Native>(support,Type,Identity)); \
 check(support&&support->get_type_description_func); \
 auto description=support->get_type_description_func(support); \
 check(description&&n::matches_native_schema(*description,ipc::custom_native_interface_schema<Native>::schema(),Type)); \
 check((n::binding<typename Native::Request,Request>::native_contract())); \
 check((n::binding<typename Native::Response,Response>::native_contract())); \
 typename Native::Request q{};q.a=41;q.b=1;std::vector<std::uint8_t> bytes; \
 check(Request::encode(q,bytes)&&bytes.size()==16); \
 for(unsigned i{};i!=16;++i)check(bytes[i]==(i==0?41:i==8?1:0)); \
 typename Native::Request q2{};check(Request::decode(bytes.data(),bytes.size(),q2)&&q2.a==41&&q2.b==1); \
 q.a=std::numeric_limits<std::int64_t>::min();q.b=std::numeric_limits<std::int64_t>::max(); \
 check(Request::encode(q,bytes)&&Request::decode(bytes.data(),bytes.size(),q2)&&q2.a==q.a&&q2.b==q.b); \
 auto before=q2;bytes.push_back(0);check(!Request::decode(bytes.data(),bytes.size(),q2)&&q2.a==before.a&&q2.b==before.b); \
 bytes.pop_back();bytes.pop_back();check(!Request::decode(bytes.data(),bytes.size(),q2)&&q2.a==before.a&&q2.b==before.b); \
 typename Native::Response r{};r.sum=42;check(Response::encode(r,bytes)&&bytes.size()==8); \
 for(unsigned i{};i!=8;++i)check(bytes[i]==(i?0:42)); \
 typename Native::Response r2{};check(Response::decode(bytes.data(),bytes.size(),r2)&&r2.sum==42); \
 r2.sum=99;bytes.push_back(0);check(!Response::decode(bytes.data(),bytes.size(),r2)&&r2.sum==99); \
 check(std::string_view{Canonical}.size()>0); \
}
UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(CONTRACT)
#undef CONTRACT
int main(){
using S=example_interfaces::srv::AddTwoInts;
auto warm=[](auto support){return support&&support->get_type_hash_func&&support->get_type_description_func&&support->get_type_hash_func(support)&&support->get_type_description_func(support);};
check(warm(rosidl_typesupport_cpp::get_service_type_support_handle<S>()));
check(warm(rosidl_typesupport_cpp::get_message_type_support_handle<S::Request>()));
check(warm(rosidl_typesupport_cpp::get_message_type_support_handle<S::Response>()));
if(::raise(SIGSTOP)!=0)return 22;bounded_native_io::gate('I');family();std::printf("PASS_ADD_TWO_INTS_NATIVE_FULL_PROTOCOL_CODEC checks=%u DELIVERY_QUALIFIED=0\n",checks);return 0;}
