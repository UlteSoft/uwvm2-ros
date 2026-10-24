/* SPDX-License-Identifier: Apache-2.0 */
#ifdef NDEBUG
#error Contract assertions must remain active
#endif
#include "document.hpp"
#include "google/protobuf/struct.pb.h"
#include "bosdyn/api/signals.pb.h"
#include <cassert>
#include <string>
int main() {
    /* Independent original C++ protobuf classes parse the same fixed keyword
     * field wires as the C decoder. This does not use the C identifier policy. */
    uwvm::spot::protobuf::Document<::bosdyn::api::SignalData_Data> signal;
    const unsigned char signal_double[]={0x09,0,0,0,0,0,0x40,0x48,0x40};
    assert(signal.decode(signal_double,sizeof signal_double)==0&&signal.get()->double_()==48.5);
    const unsigned char signal_bool[]={0x20,1};
    assert(signal.decode(signal_bool,sizeof signal_bool)==0&&signal.get()->bool_());
    const unsigned char signal_int[]={0x10,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,1};
    assert(signal.decode(signal_int,sizeof signal_int)==0&&signal.get()->int_()==-1);
    ::bosdyn::api::RobotState original;
    auto *battery=original.add_battery_states();
    battery->set_identifier(std::string("a\0b",3));
    battery->mutable_voltage()->set_value(48.5);
    battery->add_temperatures(30.25);
    original.add_foot_state()->mutable_terrain()->set_ground_mu_est(0.65);
    auto *edge=&(*original.mutable_kinematic_state()->mutable_transforms_snapshot()->
                  mutable_child_to_parent_edge_map())["body"];
    edge->set_parent_frame_name("vision");
    edge->mutable_parent_tform_child()->mutable_position()->set_z(0.8);
    original.mutable_manipulator_state()->set_gripper_open_percentage(75);
    original.mutable_system_fault_state()->add_faults()->set_error_message("example");
    std::string wire;assert(original.SerializeToString(&wire));
    uwvm::spot::protobuf::RobotState parsed;
    assert(parsed.decode(wire.data(),wire.size())==0);
    auto const *value=parsed.get();
    assert(value->battery_states(0).identifier()==std::string("a\0b",3));
    assert(value->battery_states(0).voltage().value()==48.5);
    assert(value->foot_state(0).terrain().ground_mu_est()==0.65);
    assert(value->kinematic_state().transforms_snapshot().child_to_parent_edge_map().at("body").parent_frame_name()=="vision");
    assert(value->manipulator_state().gripper_open_percentage()==75);
    auto *prior=parsed.get();
    unsigned char bad[]={0x12,4,0x12,2,0xc0,0x80};
    assert(parsed.decode(bad,sizeof bad)==22&&parsed.get()==prior);
    assert(parsed.original_size()==wire.size()&&!std::memcmp(parsed.original_wire(),wire.data(),wire.size()));
    const unsigned char unknown[]={0xa0,6,7};
    assert(parsed.decode(unknown,sizeof unknown)==0);
    std::string encoded;assert(parsed.get()->SerializeToString(&encoded));
    assert(encoded==std::string(reinterpret_cast<char const *>(unknown),sizeof unknown));
    assert(parsed.original_size()==sizeof unknown);

    const unsigned char merged_power[]={0x0a,2,0x10,2,0x0a,2,0x18,2};
    assert(parsed.decode(merged_power,sizeof merged_power)==0);
    assert(parsed.get()->power_state().motor_power_state()==2&&parsed.get()->power_state().shore_power_state()==2);
    uwvm::spot::protobuf::Document<::google::protobuf::Value> dynamic;
    const unsigned char merged_oneof[]={0x32,4,0x0a,2,0x20,1,0x32,4,0x0a,2,0x20,0};
    assert(dynamic.decode(merged_oneof,sizeof merged_oneof)==0);
    assert(dynamic.get()->list_value().values_size()==2);
    assert(dynamic.get()->list_value().values(0).bool_value());
    assert(!dynamic.get()->list_value().values(1).bool_value());
    const unsigned char changed_oneof[]={0x32,4,0x0a,2,0x20,1,0x20,1};
    assert(dynamic.decode(changed_oneof,sizeof changed_oneof)==0);
    assert(dynamic.get()->kind_case()==::google::protobuf::Value::kBoolValue&&dynamic.get()->bool_value());
    /* C++ shares the original-descriptor whole-wire preflight. Failed depth
     * and wire-budget admission must preserve both object and captured wire. */
    auto *kept=dynamic.get();
    const auto old_size=dynamic.original_size();
    unsigned char deep[128]={0};std::size_t count=0;
    for(unsigned level=0;level<33;++level) {
        std::memmove(deep+2,deep,count);
        deep[0]=(level%2)?0x0a:0x32;
        deep[1]=static_cast<unsigned char>(count);count+=2;
    }
    assert(dynamic.decode(deep,count)==90&&dynamic.get()==kept);
    assert(dynamic.original_size()==old_size&&
           !std::memcmp(dynamic.original_wire(),changed_oneof,sizeof changed_oneof));
    std::array<unsigned char,UWVM_SPOT_PROTO_WIRE_MAX+1> too_large{};
    assert(dynamic.decode(too_large.data(),too_large.size())==90&&dynamic.get()==kept);
    assert(dynamic.original_size()==old_size);
    const unsigned char unknown_enum[]={0x12,11,0x40,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,1};
    assert(parsed.decode(unknown_enum,sizeof unknown_enum)==0);
    assert(static_cast<int>(parsed.get()->battery_states(0).status())==-1);
    return 0;
}
