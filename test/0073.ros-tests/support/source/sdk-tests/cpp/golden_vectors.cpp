/** @file
 * @brief Prepared C++20 golden-wire fixture covering all57 positive and11 negative cases.
 * Expected bytes come independently from sdk/origin/abi/wire-vectors-v1.json, SHA256 e4b1070d1a579019950b34aaf558abc7888449a46db0d5d4bb430515973a2d75.
 * This fixture is not executed here and does not establish native ROS or ARM behavior.
 */
#include <cassert>
#include <platform/ros/cpp/roswasm/roswasmcpp.hpp>
/** @brief Encode trusted builtin values and compare exact independently specified bytes. */
int main() {
using namespace roswasmcpp;
{ // string-empty
wire::bytes encoded;
encoded=wire::string(std::string{(char const*)"",0});
wire::bytes expected{0,0,0,0};assert(encoded==expected);
}
{ // string-ascii
wire::bytes encoded;
encoded=wire::string(std::string{(char const*)"\x68\x65\x6c\x6c\x6f",5});
wire::bytes expected{5,0,0,0,104,101,108,108,111};assert(encoded==expected);
}
{ // string-utf8
wire::bytes encoded;
encoded=wire::string(std::string{(char const*)"\x72\x6f\x62\x6f\x74\x20\xf0\x9f\x8c\x8d",10});
wire::bytes expected{10,0,0,0,114,111,98,111,116,32,240,159,140,141};assert(encoded==expected);
}
{ // bool-false
wire::bytes encoded;
encoded=wire::boolean(false);
wire::bytes expected{0};assert(encoded==expected);
}
{ // bool-true
wire::bytes encoded;
encoded=wire::boolean(true);
wire::bytes expected{1};assert(encoded==expected);
}
{ // int32--2147483648
wire::bytes encoded;
encoded=wire::parameter_int32(INT32_MIN);
wire::bytes expected{0,0,0,128};assert(encoded==expected);
}
{ // int32--1
wire::bytes encoded;
encoded=wire::parameter_int32(-INT64_C(1));
wire::bytes expected{255,255,255,255};assert(encoded==expected);
}
{ // int32-2147483647
wire::bytes encoded;
encoded=wire::parameter_int32(INT64_C(2147483647));
wire::bytes expected{255,255,255,127};assert(encoded==expected);
}
{ // int64--9223372036854775808
wire::bytes encoded;
encoded=wire::parameter_int64(INT64_MIN);
wire::bytes expected{0,0,0,0,0,0,0,128};assert(encoded==expected);
}
{ // int64--1
wire::bytes encoded;
encoded=wire::parameter_int64(-INT64_C(1));
wire::bytes expected{255,255,255,255,255,255,255,255};assert(encoded==expected);
}
{ // int64-9223372036854775807
wire::bytes encoded;
encoded=wire::parameter_int64(INT64_C(9223372036854775807));
wire::bytes expected{255,255,255,255,255,255,255,127};assert(encoded==expected);
}
{ // float64-zero
wire::bytes encoded;
encoded=wire::parameter_float64(0.0);
wire::bytes expected{0,0,0,0,0,0,0,0};assert(encoded==expected);
}
{ // float64-negative-zero
wire::bytes encoded;
encoded=wire::parameter_float64(-0.0);
wire::bytes expected{0,0,0,0,0,0,0,128};assert(encoded==expected);
}
{ // float64-fraction
wire::bytes encoded;
encoded=wire::parameter_float64(1.25);
wire::bytes expected{0,0,0,0,0,0,244,63};assert(encoded==expected);
}
{ // twist
wire::bytes encoded;
encoded=wire::twist(std::array<double,6>{1.0,-2.0,3.0,0.0,-0.0,0.5});
wire::bytes expected{0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,192,0,0,0,0,0,0,8,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,0,0,0,0,0,0,224,63};assert(encoded==expected);
}
{ // array-bool
wire::bytes encoded;
std::vector<uint8_t> values{0,1,0};encoded=wire::parameter_bool_array(values);
wire::bytes expected{3,0,0,0,0,1,0};assert(encoded==expected);
}
{ // array-byte
wire::bytes encoded;
std::vector<uint8_t> values{0,127,255};encoded=wire::parameter_byte_array(values);
wire::bytes expected{3,0,0,0,0,127,255};assert(encoded==expected);
}
{ // array-int32
wire::bytes encoded;
std::vector<int32_t> values{-INT64_C(1),INT64_C(2147483647)};encoded=wire::parameter_int32_array(values);
wire::bytes expected{2,0,0,0,255,255,255,255,255,255,255,127};assert(encoded==expected);
}
{ // array-int64
wire::bytes encoded;
std::vector<int64_t> values{INT64_MIN,INT64_C(9223372036854775807)};encoded=wire::parameter_int64_array(values);
wire::bytes expected{2,0,0,0,0,0,0,0,0,0,0,128,255,255,255,255,255,255,255,127};assert(encoded==expected);
}
{ // array-float64
wire::bytes encoded;
std::vector<double> values{1.25,-0.0};encoded=wire::parameter_float64_array(values);
wire::bytes expected{2,0,0,0,0,0,0,0,0,0,244,63,0,0,0,0,0,0,0,128};assert(encoded==expected);
}
{ // array-string
wire::bytes encoded;
std::vector<std::string> values{std::string{(char const*)"",0},std::string{(char const*)"\x68\x65\x6c\x6c\x6f",5},std::string{(char const*)"\xce\xbb",2}};encoded=wire::parameter_string_array(values);
wire::bytes expected{3,0,0,0,0,0,0,0,5,0,0,0,104,101,108,108,111,2,0,0,0,206,187};assert(encoded==expected);
}
{ // array-empty
wire::bytes encoded;
std::vector<std::string> values{};encoded=wire::parameter_string_array(values);
wire::bytes expected{0,0,0,0};assert(encoded==expected);
}
{ // image-rgb8
wire::bytes encoded;
wire::image value;
value.sequence=0;
value.seconds=1;
value.nanoseconds=2;
value.frame_id=std::string{(char const*)"\x63\x61\x6d\x65\x72\x61",6};
value.height=1;
value.width=1;
value.encoding=std::string{(char const*)"\x72\x67\x62\x38",4};
value.bigendian=false;
value.step=3;
value.data={255,0,7};
encoded=wire::encode(value);
wire::bytes expected{0,0,0,0,1,0,0,0,2,0,0,0,6,0,0,0,99,97,109,101,114,97,1,0,0,0,1,0,0,0,4,0,0,0,114,103,98,56,0,3,0,0,0,3,0,0,0,255,0,7};assert(encoded==expected);
}
{ // transform
wire::bytes encoded;
wire::transform value;
value.sequence=0;
value.seconds=1;
value.nanoseconds=2;
value.parent_frame=std::string{(char const*)"\x6d\x61\x70",3};
value.child_frame=std::string{(char const*)"\x62\x61\x73\x65",4};
value.translation={1.0,2.0,3.0};
value.quaternion={0.0,0.0,0.0,1.0};
encoded=wire::encode(value);
wire::bytes expected{0,0,0,0,1,0,0,0,2,0,0,0,3,0,0,0,109,97,112,4,0,0,0,98,97,115,101,0,0,0,0,0,0,240,63,0,0,0,0,0,0,0,64,0,0,0,0,0,0,8,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,240,63};assert(encoded==expected);
}
{ // trigger-request
wire::bytes encoded;
encoded={};
wire::bytes expected{};assert(encoded==expected);
}
{ // set-bool-request
wire::bytes encoded;
encoded=wire::boolean(true);
wire::bytes expected{1};assert(encoded==expected);
}
{ // service-response
wire::bytes encoded;
encoded=wire::encode(wire::service_response{true,std::string{(char const*)"\x6f\x6b",2}});
wire::bytes expected{1,2,0,0,0,111,107};assert(encoded==expected);
}
{ // service-request-event
wire::bytes encoded;
encoded=wire::encode(wire::service_envelope{UINT64_C(9223372036854775813),wire::bytes{1}});
wire::bytes expected{5,0,0,0,0,0,0,128,1};assert(encoded==expected);
}
{ // service-response-envelope
wire::bytes encoded;
encoded=wire::encode(wire::service_envelope{UINT64_C(9223372036854775813),wire::bytes{1,2,0,0,0,111,107}});
wire::bytes expected{5,0,0,0,0,0,0,128,1,2,0,0,0,111,107};assert(encoded==expected);
}
{ // fibonacci-order
wire::bytes encoded;
encoded=wire::fibonacci_goal(5);
wire::bytes expected{5,0,0,0};assert(encoded==expected);
}
{ // goal-id
wire::bytes encoded;
encoded=wire::goal_id(UINT64_C(9223372036854775813));
wire::bytes expected{5,0,0,0,0,0,0,128};assert(encoded==expected);
}
{ // incoming-goal
wire::bytes encoded;
encoded=wire::encode(wire::incoming_goal{UINT64_C(9223372036854775813),5});
wire::bytes expected{5,0,0,0,0,0,0,128,5,0,0,0};assert(encoded==expected);
}
{ // feedback
wire::bytes encoded;
encoded=wire::encode(wire::action_sequence{UINT64_C(9223372036854775813),0,std::vector<int32_t>{0,1,1,2,3,5}},false);
wire::bytes expected{5,0,0,0,0,0,0,128,6,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,2,0,0,0,3,0,0,0,5,0,0,0};assert(encoded==expected);
}
{ // success
wire::bytes encoded;
encoded=wire::encode(wire::action_sequence{UINT64_C(9223372036854775813),2,std::vector<int32_t>{0,1,1,2,3,5}},true);
wire::bytes expected{5,0,0,0,0,0,0,128,2,6,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,2,0,0,0,3,0,0,0,5,0,0,0};assert(encoded==expected);
}
{ // server-feedback
wire::bytes encoded;
encoded=wire::encode(wire::action_sequence{UINT64_C(9223372036854775813),1,std::vector<int32_t>{0,1,1,2,3,5}},true);
wire::bytes expected{5,0,0,0,0,0,0,128,1,6,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,2,0,0,0,3,0,0,0,5,0,0,0};assert(encoded==expected);
}
{ // lifecycle-state
wire::bytes encoded;
encoded=wire::encode(wire::lifecycle_state{3,std::string{(char const*)"\x61\x63\x74\x69\x76\x65",6}});
wire::bytes expected{3,6,0,0,0,97,99,116,105,118,101};assert(encoded==expected);
}
{ // time-nanoseconds
wire::bytes encoded;
encoded=wire::nanoseconds(UINT64_C(9223372036854775813));
wire::bytes expected{5,0,0,0,0,0,0,128};assert(encoded==expected);
}
{ // legacy-parameter-event
wire::bytes encoded;
encoded=wire::encode(wire::parameter_event{1,2,2,std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5},std::string{(char const*)"\x76\x61\x6c\x75\x65",5},std::string{(char const*)"\x74\x72\x75\x65",4}});
wire::bytes expected{1,0,0,0,2,0,0,0,2,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,4,0,0,0,116,114,117,101};assert(encoded==expected);
}
{ // typed-event-bool
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=2;value.tag=static_cast<wire::parameter_event_value_tag>(1);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
value.bool_value=true;
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,2,2,1,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,1,0,0,0,1};assert(encoded==expected);
}
{ // typed-event-int64
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=2;value.tag=static_cast<wire::parameter_event_value_tag>(2);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
value.integer_value=INT64_C(9223372036854775807);
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,2,2,2,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,8,0,0,0,255,255,255,255,255,255,255,127};assert(encoded==expected);
}
{ // typed-event-float64
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=2;value.tag=static_cast<wire::parameter_event_value_tag>(3);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
value.double_value=1.25;
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,2,2,3,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,8,0,0,0,0,0,0,0,0,0,244,63};assert(encoded==expected);
}
{ // typed-event-string
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=2;value.tag=static_cast<wire::parameter_event_value_tag>(4);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
value.string_value=std::string{(char const*)"\x68\x65\x6c\x6c\x6f",5};
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,2,2,4,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,9,0,0,0,5,0,0,0,104,101,108,108,111};assert(encoded==expected);
}
{ // typed-event-byte-array
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=2;value.tag=static_cast<wire::parameter_event_value_tag>(5);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
value.byte_array_value={0,255};
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,2,2,5,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,6,0,0,0,2,0,0,0,0,255};assert(encoded==expected);
}
{ // typed-event-bool-array
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=2;value.tag=static_cast<wire::parameter_event_value_tag>(6);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
value.bool_array_value={0,1};
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,2,2,6,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,6,0,0,0,2,0,0,0,0,1};assert(encoded==expected);
}
{ // typed-event-int64-array
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=2;value.tag=static_cast<wire::parameter_event_value_tag>(7);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
value.integer_array_value={-INT64_C(1),INT64_C(9223372036854775807)};
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,2,2,7,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,20,0,0,0,2,0,0,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,127};assert(encoded==expected);
}
{ // typed-event-float64-array
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=2;value.tag=static_cast<wire::parameter_event_value_tag>(8);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
value.double_array_value={1.25};
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,2,2,8,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,12,0,0,0,1,0,0,0,0,0,0,0,0,0,244,63};assert(encoded==expected);
}
{ // typed-event-string-array
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=2;value.tag=static_cast<wire::parameter_event_value_tag>(9);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
value.string_array_value={std::string{(char const*)"\x68\x65\x6c\x6c\x6f",5},std::string{(char const*)"",0}};
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,2,2,9,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,17,0,0,0,2,0,0,0,5,0,0,0,104,101,108,108,111,0,0,0,0};assert(encoded==expected);
}
{ // typed-deletion-tag-0
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=3;value.tag=static_cast<wire::parameter_event_value_tag>(0);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,3,2,0,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(encoded==expected);
}
{ // typed-deletion-tag-1
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=3;value.tag=static_cast<wire::parameter_event_value_tag>(1);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,3,2,1,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(encoded==expected);
}
{ // typed-deletion-tag-2
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=3;value.tag=static_cast<wire::parameter_event_value_tag>(2);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,3,2,2,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(encoded==expected);
}
{ // typed-deletion-tag-3
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=3;value.tag=static_cast<wire::parameter_event_value_tag>(3);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,3,2,3,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(encoded==expected);
}
{ // typed-deletion-tag-4
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=3;value.tag=static_cast<wire::parameter_event_value_tag>(4);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,3,2,4,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(encoded==expected);
}
{ // typed-deletion-tag-5
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=3;value.tag=static_cast<wire::parameter_event_value_tag>(5);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,3,2,5,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(encoded==expected);
}
{ // typed-deletion-tag-6
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=3;value.tag=static_cast<wire::parameter_event_value_tag>(6);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,3,2,6,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(encoded==expected);
}
{ // typed-deletion-tag-7
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=3;value.tag=static_cast<wire::parameter_event_value_tag>(7);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,3,2,7,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(encoded==expected);
}
{ // typed-deletion-tag-8
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=3;value.tag=static_cast<wire::parameter_event_value_tag>(8);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,3,2,8,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(encoded==expected);
}
{ // typed-deletion-tag-9
wire::bytes encoded;
wire::typed_parameter_event value;value.seconds=1;value.nanoseconds=2;value.kind=3;value.tag=static_cast<wire::parameter_event_value_tag>(9);value.node=std::string{(char const*)"\x2f\x64\x65\x6d\x6f",5};value.name=std::string{(char const*)"\x76\x61\x6c\x75\x65",5};
assert(wire::encode_typed_parameter_event(value,encoded));
wire::bytes expected{1,0,0,0,2,0,0,0,3,2,9,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};assert(encoded==expected);
}
{ // Negative: Bool must be exactly 0 or 1.
wire::bytes bad{2};
bool value=true;assert(!wire::decode_boolean(bad,value)&&value);
}
{ // Negative: Reject trailing bytes.
wire::bytes bad{1,0};
bool value=true;assert(!wire::decode_boolean(bad,value)&&value);
}
{ // Negative: Declared UTF-8 byte length exceeds the remaining buffer.
wire::bytes bad{5,0,0,0,104,105};
msg::String value{"keep"};assert(!Codec<msg::String>::decode(bad,value)&&value.data=="keep");
}
{ // Negative: Overlong UTF-8 is invalid.
wire::bytes bad{2,0,0,0,192,175};
msg::String value{"keep"};assert(!Codec<msg::String>::decode(bad,value)&&value.data=="keep");
}
{ // Negative: Float64 must be finite.
wire::bytes bad{0,0,0,0,0,0,240,127};
wire::bytes out{1,2};
double value{};assert(!wire::decode_parameter_float64(bad,value));
}
{ // Negative: Every Float64 array element must be finite.
wire::bytes bad{1,0,0,0,0,0,0,0,0,0,248,127};
wire::bytes out{1,2};
std::vector<double> value{};assert(!wire::decode_parameter_float64_array(bad,value));
}
{ // Negative: Every Bool array element must be 0 or 1.
wire::bytes bad{1,0,0,0,2};
wire::bytes out{1,2};
std::vector<uint8_t> value{};assert(!wire::decode_parameter_bool_array(bad,value));
}
{ // Negative: String-array elements are NUL-free.
wire::bytes bad{1,0,0,0,3,0,0,0,97,0,98};
wire::bytes out{1,2};
std::vector<std::string> value{};assert(!wire::decode_parameter_string_array(bad,value));
}
{ // Negative: An owned Goal ID is nonzero.
wire::bytes bad{0,0,0,0,0,0,0,0,2,0,0,0,0};
wire::action_sequence value;value.goal=77;assert(!wire::decode(bad,value,true)&&value.goal==77);
}
{ // Negative: Deletion-only tag 0 requires deleted kind 3.
wire::bytes bad{1,0,0,0,2,0,0,0,1,2,0,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,0,0,0,0};
wire::typed_parameter_event value;value.seconds=77;assert(!wire::decode_typed_parameter_event(bad,value)&&value.seconds==77);
}
{ // Negative: Deleted events have an empty selected payload.
wire::bytes bad{1,0,0,0,2,0,0,0,3,2,1,5,0,0,0,47,100,101,109,111,5,0,0,0,118,97,108,117,101,1,0,0,0,1};
wire::typed_parameter_event value;value.seconds=77;assert(!wire::decode_typed_parameter_event(bad,value)&&value.seconds==77);
}
return 0;
}
