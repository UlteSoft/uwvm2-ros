/** @file
 * @brief Real fake-import regression for intrinsic C++ polling borrow guards.
 * Invoke resources directly, outside any Executor, and record each import count.
 * Nested polling/close must return17 before side effects; callback replacement
 * keeps the current functor alive. Exception-enabled host builds also check
 * lease reset after throws. No native ROS/Wasm behavior follows from this model.
 */
#include <roswasm/roswasmcpp.hpp>
#include "../c/ownership_backend.h"
#include <cstdio>
static unsigned checks;
#define CHECK(expression) do {++checks;if(!(expression)) {std::printf("FAIL intrinsic poll guard line=%u\n",(unsigned)__LINE__);return 1;}} while(0)
/** @brief Separate response import, with a canonically empty response result.
 * The event remains unchanged; a reply cannot reuse/overwrite advertised bytes.
 */
static uint32_t reply(uint64_t handle,uint32_t memory,uint64_t input,uint64_t input_size,uint64_t output,uint64_t capacity,uint64_t slot) {
    (void)memory;(void)input;(void)input_size;(void)output;(void)capacity;
    if(!handle||handle>=128||!fixture_live[handle]) return UWVM_ROS_BAD_HANDLE;
    ++fixture_requests;fixture_store64(slot,0);return 0;
}
int main() {
    using namespace roswasmcpp;auto* backend=fixture_backend();
    backend->request[ROSWASMC_SERVICE_RESPOND]=reply;backend->request[ROSWASMC_ACTION_RESPOND]=reply;
    Node node{*backend,"intrinsic-guard"};auto sub=node.create_subscription<msg::String>("sub",64);
    auto encoded=wire::string("one");std::copy(encoded.begin(),encoded.end(),fixture_payload);fixture_payload_size=encoded.size();
    uint32_t nested=0,closed=0,revoked=0;unsigned next_callbacks=0;bool snapshot_alive=false;
    auto token=std::make_shared<int>(1);std::weak_ptr<int> weak=token;
    sub.on_message([token,&sub,&weak,&snapshot_alive,&nested,&closed,&revoked,&next_callbacks](msg::String const& value){
        nested=sub.poll_once();closed=sub.close();revoked=sub.revoke();
        sub.on_message([&next_callbacks](auto const&){++next_callbacks;});
        snapshot_alive=!weak.expired()&&value.data=="one";
    });token.reset();unsigned polls=fixture_polls,closes=fixture_closes;
    CHECK(sub.poll_once()==0);
    CHECK(nested==17&&closed==17&&revoked==17);
    CHECK(fixture_polls==polls+1&&fixture_closes==closes&&sub.is_open());
    CHECK(snapshot_alive&&weak.expired()&&next_callbacks==0);
    CHECK(sub.poll_once()==0&&next_callbacks==1);
    unsigned stateful_sub=0;sub.on_message([count=0u,&stateful_sub](auto const&) mutable {stateful_sub=++count;});
    CHECK(sub.poll_once()==0&&sub.poll_once()==0&&stateful_sub==2);
#if defined(__cpp_exceptions)
    sub.on_message([](auto const&){throw 1;});bool thrown=false;
    try {sub.poll_once();}catch(int value){thrown=value==1;}
    CHECK(thrown);
#endif
    CHECK(sub.close()==0&&!sub.is_open());
    polls=fixture_polls;CHECK(sub.error()==9&&sub.poll_once()==9&&fixture_polls==polls);
    auto service=node.create_service<srv::Trigger>("advertise","respond",64);CHECK(service.error()==0);
    fixture_payload_size=8;fixture_store64(uwvm_ros_address(fixture_payload),42);
    uint32_t statuses[2]={55,66};roswasmc_cleanup_report refused{},revoke_refused{};unsigned requests=fixture_requests;
    token=std::make_shared<int>(2);weak=token;snapshot_alive=false;
    service.on_request([token,&service,&weak,&snapshot_alive,&nested,&closed,&statuses,&refused,&revoke_refused,&revoked,&next_callbacks](srv::Trigger::Request const&){
        nested=service.poll_once();closed=service.close();refused=service.close_checked(statuses,110);
        revoked=service.revoke();revoke_refused=service.revoke_checked(statuses,77);
        service.on_request([&next_callbacks](auto const&){++next_callbacks;return srv::Trigger::Response{true,"next"};});
        snapshot_alive=!weak.expired();return srv::Trigger::Response{true,"first"};
    });token.reset();polls=fixture_polls;closes=fixture_closes;
    CHECK(service.poll_once()==0);
    CHECK(nested==17&&closed==17&&refused.cleanup_error==17&&refused.primary_error==110&&refused.attempted==0);
    CHECK(revoked==17&&revoke_refused.cleanup_error==17&&revoke_refused.primary_error==77&&revoke_refused.attempted==0);
    CHECK(statuses[0]==55&&statuses[1]==66&&fixture_polls==polls+1&&fixture_closes==closes&&fixture_requests==requests+1);
    CHECK(snapshot_alive&&weak.expired()&&service.advertise_handle()!=0&&service.respond_handle()!=0);
    CHECK(service.poll_once()==0&&next_callbacks==2);
    unsigned stateful_service=0;service.on_request([count=0u,&stateful_service](auto const&) mutable {stateful_service=++count;return srv::Trigger::Response{true,"stateful"};});
    CHECK(service.poll_once()==0&&service.poll_once()==0&&stateful_service==2);
#if defined(__cpp_exceptions)
    service.on_request([](auto const&)->srv::Trigger::Response {throw 2;});thrown=false;requests=fixture_requests;
    try {service.poll_once();}catch(int value){thrown=value==2;}
    CHECK(thrown&&fixture_requests==requests);
#endif
    auto cleanup=service.close_checked();CHECK(cleanup.attempted==2&&cleanup.cleanup_error==0);
    polls=fixture_polls;CHECK(service.error()==9&&service.poll_once()==9&&fixture_polls==polls);
    auto action=node.create_action_server("action-advertise","action-respond",12);CHECK(action.error()==0);
    fixture_payload_size=12;fixture_store64(uwvm_ros_address(fixture_payload),43);uwvm_ros_store_u32(fixture_payload+8,4);
    uint32_t responded=99;token=std::make_shared<int>(3);weak=token;snapshot_alive=false;statuses[0]=55;statuses[1]=66;
    action.on_goal([token,&action,&weak,&snapshot_alive,&nested,&closed,&refused,&revoke_refused,&revoked,&statuses,&responded,&next_callbacks](FibonacciGoal const& goal){
        nested=action.poll_once();closed=action.close();refused=action.close_checked(statuses);
        revoked=action.revoke();revoke_refused=action.revoke_checked(statuses,77);
        action.on_goal([&next_callbacks](auto const&){++next_callbacks;});
        snapshot_alive=!weak.expired()&&goal.id==43&&goal.order==4;
        responded=action.respond({goal.id,2,{0,1,1,2}});
    });token.reset();polls=fixture_polls;closes=fixture_closes;requests=fixture_requests;
    CHECK(action.poll_once()==0);
    CHECK(nested==17&&closed==17&&refused.cleanup_error==17&&refused.attempted==0);
    CHECK(revoked==17&&revoke_refused.cleanup_error==17&&revoke_refused.primary_error==77&&revoke_refused.attempted==0);
    CHECK(statuses[0]==55&&statuses[1]==66&&fixture_polls==polls+1&&fixture_closes==closes&&fixture_requests==requests+1&&responded==0);
    CHECK(snapshot_alive&&weak.expired()&&action.advertise_handle()!=0&&action.respond_handle()!=0);
    CHECK(action.poll_once()==0&&next_callbacks==3);
    unsigned stateful_action=0;action.on_goal([count=0u,&stateful_action](auto const&) mutable {stateful_action=++count;});
    CHECK(action.poll_once()==0&&action.poll_once()==0&&stateful_action==2);
#if defined(__cpp_exceptions)
    action.on_goal([](auto const&){throw 3;});thrown=false;
    try {action.poll_once();}catch(int value){thrown=value==3;}
    CHECK(thrown);
#endif
    cleanup=action.close_checked();CHECK(cleanup.attempted==2&&cleanup.cleanup_error==0);
    polls=fixture_polls;CHECK(action.error()==9&&action.poll_once()==9&&action.respond({43,2,{0}})==9&&fixture_polls==polls);
    auto revoke_service=node.create_service<srv::Trigger>("revoke-advertise","revoke-respond",64);
    auto response_handle=revoke_service.respond_handle();auto advertise_handle=revoke_service.advertise_handle();
    fixture_close_errors[response_handle]=110;fixture_close_errors[advertise_handle]=107;cleanup=revoke_service.revoke_checked(statuses,13);
    CHECK(cleanup.primary_error==13&&cleanup.cleanup_error==110&&cleanup.attempted==2&&cleanup.failed==2&&statuses[0]==107&&statuses[1]==110&&revoke_service.error()==9);
    auto revoke_action=node.create_action_server("revoke-action-advertise","revoke-action-respond",12);
    response_handle=revoke_action.respond_handle();advertise_handle=revoke_action.advertise_handle();
    fixture_close_errors[response_handle]=110;fixture_close_errors[advertise_handle]=107;cleanup=revoke_action.revoke_checked(statuses,22);
    CHECK(cleanup.primary_error==22&&cleanup.cleanup_error==110&&cleanup.attempted==2&&cleanup.failed==2&&statuses[0]==107&&statuses[1]==110&&revoke_action.error()==9);
    std::printf("PASS intrinsic poll guard models checks=%u\n",checks);return 0;
}
