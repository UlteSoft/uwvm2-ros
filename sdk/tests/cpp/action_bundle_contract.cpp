/** @file
 * @brief Pure C++ fake-import model for explicit Action aggregation/goal binding.
 * Source-prepared; root alone compiles/runs serially with outputs in /tmp.
 * Success proves typed local contracts, never native Action/QoS/ROS delivery.
 */
#include <roswasm/roswasmcpp.hpp>
#include "../c/ownership_backend.h"
#include <cstdio>
static unsigned checks;
#define CHECK(expression) do {++checks;if(!(expression)) {std::printf("FAIL C++ Action bundle line=%u\n",(unsigned)__LINE__);return 1;}} while(0)
static void payload(roswasmcpp::wire::bytes const& bytes) {fixture_payload_size=bytes.size();std::copy(bytes.begin(),bytes.end(),fixture_payload);}
int main() {
    using namespace roswasmcpp;auto* backend=fixture_backend();Node node{*backend,"Action-bundle-model"};
    ActionClientAliases aliases{"send","own","feedback","result"};unsigned before=fixture_opens;
    auto invalid=node.create_action_client({"same","same","feedback","result"},1,64);
    CHECK(invalid.error()==22&&fixture_opens==before&&!invalid.is_open());
    auto oversized=node.create_action_client(aliases,1025,64);CHECK(oversized.error()==22&&fixture_opens==before);
    auto too_large=node.create_action_client(aliases,1,4193787);CHECK(too_large.error()==90&&fixture_opens==before);
    auto saved=backend->request[ROSWASMC_ACTION_RECEIVE_RESULT];backend->request[ROSWASMC_ACTION_RECEIVE_RESULT]=nullptr;
    auto unsupported=node.create_action_client(aliases,1,64);CHECK(unsupported.error()==95&&fixture_opens==before);
    backend->request[ROSWASMC_ACTION_RECEIVE_RESULT]=saved;fixture_denied="feedback";
    fixture_close_errors[fixture_next]=107;fixture_close_errors[fixture_next+1]=110;
    auto denied=node.create_action_client(aliases,1,64);auto rollback=denied.rollback_status();auto codes=denied.rollback_codes();
    CHECK(denied.error()==13&&!denied.is_open()&&rollback.primary_error==13&&rollback.cleanup_error==110&&rollback.attempted==2&&rollback.failed==2);
    std::array<uint64_t,5> empty_handles{};CHECK(codes[0]==107&&codes[1]==110&&denied.handles()==empty_handles);
    fixture_denied="denied";auto client=node.create_action_client(aliases,1,128);CHECK(client.error()==0&&client.is_open()&&!client.has_cancel_any());
    before=fixture_requests;bool accepted=true;CHECK(client.cancel_all(accepted)==13&&accepted&&fixture_requests==before);
    CHECK(client.cancel_any(999,accepted)==13&&accepted&&fixture_requests==before);
    wire::action_sequence sequence{999,2,{99}};CHECK(client.receive_result(999,sequence)==9&&sequence.goal==999&&fixture_requests==before);
    uint64_t goal=777;fixture_request_error=110;CHECK(client.send_goal(4,goal)==110&&goal==777&&client.goal_count()==0&&fixture_requests==before+1);
    fixture_request_error=0;fixture_payload_size=8;fixture_store64(uwvm_ros_address(fixture_payload),123);
    CHECK(client.send_goal(4,goal)==0&&goal==123&&client.owns_goal(goal)&&client.goal_count()==1);
    before=fixture_requests;uint64_t untouched=888;CHECK(client.send_goal(4,untouched)==90&&untouched==888&&fixture_requests==before&&!client.last_result().has_length);
    CHECK(client.cancel_own(999,accepted)==9&&accepted&&fixture_requests==before);
    fixture_payload_size=1;fixture_payload[0]=1;CHECK(client.cancel_own(goal,accepted)==0&&accepted&&client.owns_goal(goal));
    payload(wire::encode(wire::action_sequence{124,2,{0,1}},true));CHECK(client.receive_result(goal,sequence)==22&&sequence.goal==999&&client.owns_goal(goal));
    payload(wire::encode(wire::action_sequence{goal,0,{0,1}},false));CHECK(client.receive_feedback(goal,sequence)==0&&sequence.goal==goal&&sequence.values==std::vector<int32_t>({0,1})&&client.owns_goal(goal));
    fixture_request_error=11;CHECK(client.receive_result(goal,sequence)==11&&client.owns_goal(goal));fixture_request_error=0;
    payload(wire::encode(wire::action_sequence{goal,2,{0,1,1,2}},true));CHECK(client.receive_result(goal,sequence)==0&&!client.owns_goal(goal)&&client.goal_count()==0&&sequence.kind==2);
    before=fixture_requests;CHECK(client.receive_result(goal,sequence)==9&&fixture_requests==before);
    auto handles=client.handles();fixture_close_errors[handles[0]]=107;fixture_close_errors[handles[3]]=110;
    uint32_t statuses[5]={55,55,55,55,55};before=fixture_closes;auto cleanup=client.close_checked({statuses,3},13);
    CHECK(cleanup.cleanup_error==22&&cleanup.attempted==0&&fixture_closes==before&&client.is_open()&&statuses[0]==55);
    cleanup=client.close_checked(statuses,13);CHECK(cleanup.primary_error==13&&cleanup.cleanup_error==110&&cleanup.attempted==4&&cleanup.failed==2);
    CHECK(statuses[0]==107&&statuses[3]==110&&statuses[4]==55&&!client.is_open()&&client.error()==9);
    before=fixture_closes;CHECK(client.close()==9&&fixture_closes==before);goal=777;CHECK(client.send_goal(4,goal)==9&&goal==777&&client.last_result().error==9);
    aliases.cancel_any="any";auto stronger=node.create_action_client(aliases,2,64);CHECK(stronger.error()==0&&stronger.has_cancel_any()&&stronger.handles()[4]!=0);
    fixture_payload_size=1;fixture_payload[0]=1;accepted=false;CHECK(stronger.cancel_all(accepted)==0&&accepted);
    before=fixture_requests;accepted=false;CHECK(stronger.cancel_any(0,accepted)==22&&!accepted&&fixture_requests==before);
    CHECK(stronger.cancel_any(999,accepted)==0&&accepted&&stronger.goal_count()==0);
    handles=stronger.handles();fixture_close_errors[handles[4]]=110;fixture_close_errors[handles[0]]=107;
    cleanup=stronger.revoke_checked(statuses,22);CHECK(cleanup.primary_error==22&&cleanup.cleanup_error==110&&cleanup.attempted==5&&cleanup.failed==2&&stronger.error()==9);
    CHECK(statuses[4]==110&&statuses[0]==107&&stronger.goal_count()==0);
    auto deferred=node.create_action_client({"send2","own2","feedback2","result2"},1,64);Executor executor;
    fixture_payload_size=8;fixture_store64(uwvm_ros_address(fixture_payload),456);before=fixture_requests;
    auto future=deferred.async_send_goal(executor,4);CHECK(fixture_requests==before&&future.state()==FutureState::pending);size_t delivered=99;
    CHECK(executor.spin_some(0,delivered)==0&&fixture_requests==before+1&&delivered==0);goal=0;
    CHECK(future.take(goal)==0&&goal==456&&deferred.owns_goal(goal));CHECK(deferred.close()==0&&deferred.goal_count()==0);
    std::printf("PASS C++ Action bundle models checks=%u\n",checks);return 0;
}
