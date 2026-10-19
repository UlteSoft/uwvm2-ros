/** @file
 * @brief Synthetic canonical grammar/envelope regression models, never native ROS proof.
 * Compile/run later through the root's serial /tmp model lane. No native ROS
 * package or library is loaded; placeholder identities below are intentionally
 * synthetic syntax fixtures, not generated registrations or interoperability.
 */
#include <uwvm2/imported/ros/ipc/custom_type.h>
#include <cstdio>
#include <vector>
#include <array>
namespace ipc=uwvm2::imported::ros::ipc;
namespace {unsigned checks{};bool good=true;void check(bool value){++checks;good=good&&value;}
constexpr ipc::custom_wire_node scalar_bool{ipc::custom_wire_kind::boolean,0,nullptr,0};
constexpr ipc::custom_wire_node scalar_u16{ipc::custom_wire_kind::u16,0,nullptr,0};
constexpr ipc::custom_wire_node inner_fields[]={{ipc::custom_wire_kind::f32,0,nullptr,0},scalar_bool};
constexpr ipc::custom_wire_node inner{ipc::custom_wire_kind::record,0,inner_fields,2};
constexpr ipc::custom_wire_node fields[]={scalar_bool,{ipc::custom_wire_kind::array,2,&scalar_u16,1},{ipc::custom_wire_kind::utf8,5,nullptr,0},{ipc::custom_wire_kind::sequence,2,&inner,1}};
constexpr ipc::custom_wire_node schema{ipc::custom_wire_kind::record,0,fields,4};
constexpr ipc::custom_wire_node empty{ipc::custom_wire_kind::record,0,nullptr,0};
constexpr char native_id[]{"RIHS01_0000000000000000000000000000000000000000000000000000000000000000"};
constexpr char codec_id[]{"UWVMROS1_0000000000000000000000000000000000000000000000000000000000000000"};
}
int main(){
    ipc::custom_wire_extent extent{};check(ipc::custom_wire_extent_of(schema,extent));check(extent.minimum==13);check(extent.maximum==28);check(extent.cells==13);
    // Bool1, two u16s, UTF-8 micro sign, then two {finite f32,Bool} records.
    std::vector<std::uint8_t> golden{1,0x34,0x12,0xff,0xff,2,0,0,0,0xc2,0xb5,2,0,0,0,0,0,0xc0,0x3f,1,0,0,0,0xc0,0};
    check(golden.size()==25);check(ipc::validate_custom_wire(schema,golden.data(),golden.size()));
    for(size_t n{};n<golden.size();++n)check(!ipc::validate_custom_wire(schema,golden.data(),n));
    auto mutation=golden;mutation.push_back(0);check(!ipc::validate_custom_wire(schema,mutation.data(),mutation.size()));
    for(auto bad:{2u,255u}){mutation=golden;mutation[0]=static_cast<std::uint8_t>(bad);check(!ipc::validate_custom_wire(schema,mutation.data(),mutation.size()));}
    mutation=golden;mutation[9]=0;check(!ipc::validate_custom_wire(schema,mutation.data(),mutation.size()));
    mutation=golden;mutation[9]=0xc0;check(!ipc::validate_custom_wire(schema,mutation.data(),mutation.size()));
    mutation=golden;mutation[10]=0xff;check(!ipc::validate_custom_wire(schema,mutation.data(),mutation.size()));
    mutation=golden;mutation[5]=6;check(!ipc::validate_custom_wire(schema,mutation.data(),mutation.size()));
    mutation=golden;mutation[11]=3;check(!ipc::validate_custom_wire(schema,mutation.data(),mutation.size()));
    mutation=golden;mutation[11]=255;mutation[12]=255;mutation[13]=255;mutation[14]=255;check(!ipc::validate_custom_wire(schema,mutation.data(),mutation.size()));
    mutation=golden;mutation[17]=0x80;mutation[18]=0x7f;check(!ipc::validate_custom_wire(schema,mutation.data(),mutation.size()));
    mutation=golden;mutation[17]=0xc0;mutation[18]=0x7f;check(!ipc::validate_custom_wire(schema,mutation.data(),mutation.size()));
    mutation=golden;mutation[19]=2;check(!ipc::validate_custom_wire(schema,mutation.data(),mutation.size()));
    check(ipc::validate_custom_wire(empty,nullptr,0));check(!ipc::validate_custom_wire(empty,golden.data(),1));
    ipc::custom_topic_descriptor part{2,"fixture/srv/Empty_Request",native_id,codec_id,0,nullptr,0,&empty,0};
    auto response=part;response.type="fixture/srv/Empty_Response";
    ipc::custom_service_descriptor service{2,"fixture/srv/Empty",native_id,codec_id,&part,&response};
    check(ipc::validate_custom_service_request(service,"service.call",nullptr,0));
    check(ipc::validate_custom_service_output(service,"service.call",nullptr,0));
    std::array<std::uint8_t,9> ticket{7,0,0,0,0,0,0,0,2};
    check(ipc::validate_custom_service_event(service,ticket.data(),8));check(ipc::validate_custom_service_request(service,"service.respond",ticket.data(),8));
    ticket[0]=0;check(!ipc::validate_custom_service_event(service,ticket.data(),8));ticket[0]=7;
    check(!ipc::validate_custom_service_request(service,"service.respond",ticket.data(),9));
    auto goal=part;goal.type="fixture/action/Empty_Goal";auto terminal=part;terminal.type="fixture/action/Empty_Result";auto feedback=part;feedback.type="fixture/action/Empty_Feedback";
    ipc::custom_action_descriptor action{2,"fixture/action/Empty",native_id,codec_id,&goal,&terminal,&feedback};
    for(std::uint8_t kind=1;kind<=4;++kind){ticket[8]=kind;check(ipc::validate_custom_action_request(action,"action.respond",ticket.data(),9));check(ipc::validate_custom_action_output(action,"action.receive-result",ticket.data(),9)==(kind>=2));}
    ticket[8]=0;check(!ipc::validate_custom_action_request(action,"action.respond",ticket.data(),9));ticket[8]=5;check(!ipc::validate_custom_action_request(action,"action.respond",ticket.data(),9));
    check(ipc::validate_custom_action_event(action,ticket.data(),8));check(ipc::validate_custom_action_output(action,"action.receive-feedback",ticket.data(),8));
    check(ipc::custom_component_owner(service.type,&part,"Request"));check(!ipc::custom_component_owner(action.type,&part,"Goal"));
    check(!ipc::find_custom_service(2,service.type));check(!ipc::find_custom_action(2,action.type));
    std::printf("%s bounded-wire checks=%u synthetic_only=1\n",good?"PASS":"FAIL",checks);return good?0:1;
}
