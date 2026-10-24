#include <stdexcept>
#include <chrono>
#include <thread>
#include <uwvm2/imported/ros/ipc/parameter_event.h>
#include "../../../../../src/ros/gateway/common/config.h"
#if defined(__cpp_concepts)
#include <fast_io.h>
#include <uwvm2/imported/ros/policy/schema.h>
#include "../../../../../sdk/origin/common/codec.hpp"
#endif
namespace core=uwvm2::imported::ros;
namespace ipc=core::ipc;
namespace gateway=uwvm2::ros_gateway;
static core::capability_spec cap()
{
 core::capability_spec c;c.alias="deleted";c.operation="param.subscribe-events";c.resource="/delete/native/value";
 c.type="builtin/param-event-deletion-v1";c.type_identity="builtin:param-event-deletion-v1";
 c.limits={{"parameter-node","/delete/native"},{"parameter-name","value"},{"max-parameter-bytes","4096"},{"timeout-ms","2000"},{"max-inflight","2"},{"max-rate-hz","100"},{"burst","8"},{"max-bytes-per-second","1048576"},{"validator","none"},{"audit","allow-and-deny"},{"audit-max-rate-hz","100"},{"audit-burst","100"},{"audit-max-records","10000"},{"audit-max-bytes","1048576"},{"max-subscribers","1"},{"max-event-fields","8"},{"max-event-queue-depth","8"},{"max-event-queue-bytes","32768"},{"overflow-policy","reject"},{"qos.history","keep-last"},{"qos.depth","32"},{"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}};return c;
}
int main()
{
 // Keep this standalone test alive long enough for the external executor to
 // capture its real ELF/PID/cgroup before its small oracles finish.
 std::this_thread::sleep_for(std::chrono::milliseconds(150));
 unsigned checks{};auto check=[&](bool result){if(!result) { throw std::runtime_error("deletion-only unit oracle failed"); }++checks;};
 std::vector<std::uint8_t> const golden{123,0,0,0,200,1,0,0,3,2,0,14,0,0,0,'/','d','e','l','e','t','e','/','n','a','t','i','v','e',5,0,0,0,'v','a','l','u','e',0,0,0,0};
 ipc::typed_parameter_event event;event.seconds=123;event.nanoseconds=456;event.kind=3;event.node="/delete/native";event.name="value";event.value.type=ipc::parameter_kind::deletion;
 std::vector<std::uint8_t> bytes{42};check(!ipc::encode_typed_parameter_event(event,bytes)&&bytes==golden);
 ipc::typed_parameter_event out;check(!ipc::decode_typed_parameter_event(golden,out)&&out.kind==3&&out.value.type==ipc::parameter_kind::deletion&&out.value.string_value.empty());
 auto c=cap();check(!ipc::validate_parameter_contract(core::version::ros2,c));gateway::validate(core::version::ros2,c);
 check(!ipc::validate_typed_parameter_event(core::version::ros2,c,golden));
 check(ipc::parameter_type(core::version::ros2,c.type)==ipc::parameter_kind::unsupported);
 check(ipc::parameter_event_type(core::version::ros1,c.type)==ipc::parameter_kind::unsupported);
 auto before=out;
 for(unsigned kind:{1u,2u}) { auto wire=golden;wire[8]=kind;check(ipc::decode_typed_parameter_event(wire,out)==22&&out.kind==before.kind&&out.node==before.node);event.kind=kind;bytes={42};check(ipc::encode_typed_parameter_event(event,bytes)==22&&bytes==std::vector<std::uint8_t>{42}); }
 event.kind=3;
 for(unsigned tag:{10u,255u}) { auto wire=golden;wire[10]=tag;check(ipc::decode_typed_parameter_event(wire,out)==22&&out.value.type==before.value.type); }
 auto bad=golden;bad.back()=1;bad.push_back(0);check(ipc::decode_typed_parameter_event(bad,out)==22);
 bad=golden;bad.push_back(0);check(ipc::decode_typed_parameter_event(bad,out)==22);
 for(auto const& field: {"max-parameter-elements","parameter-min-value","parameter-max-value"}) { auto invalid=c;invalid.limits[field]="1";check(ipc::validate_parameter_contract(core::version::ros2,invalid)==22); }
 auto invalid=c;invalid.type_identity="builtin:param-event-bool-v1";check(ipc::validate_parameter_contract(core::version::ros2,invalid)==13);
 invalid=c;invalid.limits["parameter-name"]="other";check(ipc::validate_typed_parameter_event(core::version::ros2,invalid,golden)==13);
 invalid=c;invalid.limits["max-parameter-bytes"]="41";check(ipc::validate_typed_parameter_event(core::version::ros2,invalid,golden)==90);
 invalid=c;invalid.type="builtin/param-event-bool-v1";invalid.type_identity="builtin:param-event-bool-v1";check(ipc::validate_typed_parameter_event(core::version::ros2,invalid,golden)==13);
 for(auto const& operation:{"param.get","param.set","param.declare","param.undeclare","param.describe"}) { invalid=c;invalid.operation=operation;check(ipc::validate_parameter_contract(core::version::ros2,invalid)==95); }
#if defined(__cpp_concepts)
 check(static_cast<bool>(core::validate_capability(core::version::ros2,c)));
 for(auto const& operation:{"param.get","param.set","param.declare","param.undeclare","topic.publish"}) { check(!core::backend_supported(core::version::ros2,operation,c.type)); }
 namespace sdk=uwvm_ros;sdk::typed_parameter_event sdk_event;sdk_event.seconds=123;sdk_event.nanoseconds=456;sdk_event.kind=3;sdk_event.node="/delete/native";sdk_event.name="value";sdk_event.tag=sdk::parameter_event_value_tag::deletion;
 sdk::bytes sdk_bytes{42};check(sdk::encode_typed_parameter_event(sdk_event,sdk_bytes)&&sdk_bytes==golden);
 sdk::typed_parameter_event sdk_out;check(sdk::decode_typed_parameter_event(golden,sdk_out)&&sdk_out.tag==sdk::parameter_event_value_tag::deletion&&sdk_out.kind==3);
 for(unsigned kind:{1u,2u}) { auto wire=golden;wire[8]=kind;check(!sdk::decode(wire,sdk_out)&&sdk_out.kind==3);sdk_event.kind=kind;sdk_bytes={42};check(!sdk::encode_typed_parameter_event(sdk_event,sdk_bytes)&&sdk_bytes==sdk::bytes{42}); }
 sdk_event.kind=3;
 for(unsigned tag:{10u,255u}) { auto wire=golden;wire[10]=tag;check(!sdk::decode(wire,sdk_out)&&sdk_out.tag==sdk::parameter_event_value_tag::deletion); }
 bad=golden;bad.back()=1;bad.push_back(0);check(!sdk::decode(bad,sdk_out));
 // A deletion notification reports no prior metadata. Unselected value fields
 // in the SDK object have no wire representation and are not attested.
 sdk_event.string_value="unknown prior";sdk_event.double_value=std::numeric_limits<double>::quiet_NaN();check(sdk::encode(sdk_event)==golden);
 check(core::builtin_type_identity(core::version::ros2,c.type)==c.type_identity);
#endif
#if defined(__cpp_concepts)
 fast_io::io::println("PASS deletion-only CPP20+ IPC/SDK/Worker-schema ",checks," checks: exact tag0/kind3 empty payload, transactional rejection, dedicated permission, unknown prior metadata");
#else
 gateway::native_io::fixed_text<256> line;
 check(line.append("PASS deletion-only CPP17 IPC/Gateway-contract ")&&line.number(checks)&&line.append(" checks: exact tag0/kind3 empty payload, transactional rejection, dedicated permission, unknown prior metadata\n"));
 if(!gateway::native_io::write_all(STDOUT_FILENO,line.view())) { throw std::runtime_error("unit output failed"); }
#endif
}
