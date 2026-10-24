/** @file
 * @brief Pure C++20 typed-owner close, transactional factories and Session model.
 * Root compiles one translation unit serially into /tmp. Refusal/unsupported
 * results are negative contracts; these tests cannot establish native ROS effects.
 */
#include <platform/ros/cpp/roswasm/roswasmcpp.hpp>
#include "../c/ownership_backend.h"
#include <cassert>
#include <cstdio>
static unsigned checks;
#define CHECK(value) do {++checks;assert(value);} while(0)
/** @brief Test partial acquisition failure, no replay and preserved typed output. */
int main() {
    using namespace roswasmcpp;auto* backend=fixture_backend();Node node{*backend,"pure-C++"};
    CHECK(node.label()=="pure-C++"&&fixture_opens==0);
    Session session{*backend,2};CHECK(session.check_abi()==0&&fixture_opens==0);
    auto first=session.open("allowed");auto second=session.open("allowed");
    CHECK(first.error==0&&second.error==0&&first.owner!=second.owner&&first.owner->is_open());
    auto before=fixture_opens;CHECK(session.open("allowed").error==90&&fixture_opens==before);
    fixture_close_errors[first.owner->handle()]=107;fixture_close_errors[second.owner->handle()]=110;
    uint32_t statuses[2];auto report=session.close(statuses,13);
    CHECK(report.primary_error==13&&report.cleanup_error==110&&report.attempted==2&&report.failed==2);
    CHECK(statuses[0]==107&&statuses[1]==110&&session.size()==0);
    before=fixture_opens;auto wrong=node.graph("allowed",ROSWASMC_PARAMETER_GET,64);
    CHECK(wrong.error()==22&&!wrong.is_open()&&fixture_opens==before);
    auto too_large=node.create_subscription<msg::String>("allowed",4193787);
    CHECK(too_large.error()==90&&fixture_opens==before);
    backend->subscribe_parameter_events=0;auto events=node.create_parameter_events("allowed",64);
    CHECK(events.error()==95&&fixture_opens==before);
    backend->subscribe_parameter_events=fixture_activate;
    fixture_activate_error=110;fixture_close_errors[fixture_next]=107;
    auto rejected=node.create_subscription<msg::String>("allowed",64);
    CHECK(rejected.error()==110&&rejected.rollback_error()==107&&!rejected.is_open());
    fixture_activate_error=0;before=fixture_opens;
    auto invalid_pair=node.create_service<srv::Trigger>("same","same",64);
    CHECK(invalid_pair.error()==22&&fixture_opens==before);
    fixture_denied="respond";fixture_close_errors[fixture_next]=107;before=fixture_activations;
    auto denied_pair=node.create_service<srv::Trigger>("advertise","respond",64);
    CHECK(denied_pair.error()==13&&fixture_activations==before);
    CHECK(denied_pair.rollback_status().primary_error==13&&denied_pair.rollback_status().cleanup_error==107);
    CHECK(denied_pair.advertise_handle()==0&&denied_pair.respond_handle()==0);
    fixture_denied="denied";auto server=node.create_service<srv::Trigger>("advertise","respond",64);
    CHECK(server.error()==0);before=fixture_polls;CHECK(server.poll_once()==22&&fixture_polls==before);
    fixture_close_errors[server.advertise_handle()]=107;fixture_close_errors[server.respond_handle()]=110;
    report=server.close_checked(statuses,22);
    CHECK(report.primary_error==22&&report.cleanup_error==110&&report.attempted==2&&report.failed==2);
    CHECK(statuses[0]==107&&statuses[1]==110&&server.close()==9);
    fixture_activate_error=110;auto action_server=node.create_action_server("advertise","respond",12);
    CHECK(action_server.error()==110&&action_server.rollback_status().attempted==2);
    CHECK(action_server.advertise_handle()==0&&action_server.respond_handle()==0);fixture_activate_error=0;
    auto client=node.create_client<srv::Trigger>("allowed",64);srv::Trigger::Response response{true,"keep"};
    fixture_request_error=110;before=fixture_requests;
    CHECK(client.call({},response)==110&&fixture_requests==before+1&&response.success&&response.message=="keep");
    fixture_request_error=0;fixture_payload_size=1;fixture_payload[0]=2;
    CHECK(client.call({},response)==22&&response.success&&response.message=="keep");
    auto publisher=node.create_publisher<msg::String>("allowed");
    auto subscriber=node.create_subscription<msg::String>("allowed",64);
    auto action=node.create_action_client("allowed",ROSWASMC_ACTION_SEND_GOAL,64);
    auto parameter=node.parameters("allowed",ROSWASMC_PARAMETER_GET,64);
    auto graph=node.graph("allowed",ROSWASMC_GRAPH_LIST_TOPICS,64);auto clock=node.clock("allowed",ROSWASMC_TIME_READ_ROS);
    auto logger=node.logger("allowed");auto tf=node.tf("allowed",ROSWASMC_TF_LOOKUP,64);
    auto lifecycle=node.lifecycle("allowed",ROSWASMC_LIFECYCLE_CHANGE_STATE,64);
    CHECK(publisher.error()==0&&subscriber.error()==0&&action.error()==0&&parameter.error()==0);
    CHECK(graph.error()==0&&clock.error()==0&&logger.error()==0&&tf.error()==0&&lifecycle.error()==0);
    before=fixture_polls;CHECK(subscriber.poll_once()==22&&fixture_polls==before);
    int64_t prior=123;fixture_request_error=110;CHECK(parameter.get(prior)==110&&prior==123);fixture_request_error=0;
    auto close_one=[&](auto& owner) {
        auto handle=owner.handle();CHECK(handle!=0&&owner.is_open());
        fixture_close_errors[handle]=107;CHECK(owner.close()==107&&owner.handle()==0&&!owner.is_open());
        auto count=fixture_closes;CHECK(owner.close()==9&&fixture_closes==count);
    };
    close_one(publisher);close_one(subscriber);close_one(client);close_one(action);close_one(parameter);
    close_one(graph);close_one(clock);close_one(logger);close_one(tf);close_one(lifecycle);
    std::printf("PASS C++ resource lifetime models checks=%u\n",checks);return 0;
}
