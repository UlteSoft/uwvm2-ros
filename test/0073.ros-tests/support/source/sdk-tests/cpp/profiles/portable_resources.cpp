/** @file Native pure-model coverage of complete old-standard resource methods.
 * This verifies the actual C++ owners and canonical codecs over a test-owned
 * dispatch table. It is never counted as Guest, Gateway or native ROS evidence.
 */
#include <platform/ros/cpp/roswasm/roswasm.hpp>
#include <platform/ros/cpp/roswasm/detail/industrial_portable.hpp>
#include "../../c/ownership_backend.h"
#include <cassert>
#include <cstring>
using namespace roswasmcpp;
static void payload(wire::bytes const &value) {
    assert(value.size() <= sizeof fixture_payload);
    fixture_payload_size = value.size();
    if (!value.empty())
        std::memcpy(fixture_payload, buffer_data(value), value.size());
}
static ServiceServer<srv::SetBool> *active_server;
static unsigned handled;
static srv::SetBool::Response response(srv::SetBool::Request const &request) {
    assert(request.data);
    assert(active_server->close() == 17);
    ++handled;
    fixture_payload_size = 0;
    srv::SetBool::Response value;
    value.success = true;
    value.message = "complete response";
    return value;
}
#if ROSWASM_CPP_LEGACY
static srv::SetBool::Response legacy_response(void *, srv::SetBool::Request const &request) {
    return response(request);
}
#endif
static uint64_t event_source, event_sequence = 1;
static uint32_t watch(uint64_t observer, uint64_t source, uint64_t correlation) {
    return observer && source == event_source && !correlation ? 0 : 13;
}
static uint32_t event_poll(uint64_t, uint32_t, uint64_t output, uint64_t capacity, uint64_t slot) {
    fixture_store64(slot, 68);
    if (capacity < 68)
        return 90;
    unsigned char *p = reinterpret_cast<unsigned char *>(static_cast<uintptr_t>(output));
    std::memset(p, 0, 68);
    p[0] = 'U';
    p[1] = 'W';
    p[2] = 'R';
    p[3] = 'E';
    p[4] = 1;
    p[6] = 1;
    p[8] = 2;
    fixture_store64(output + 16, event_source);
    fixture_store64(output + 32, event_sequence);
    fixture_store64(output + 40, 42);
    fixture_store64(output + 48, 43);
    fixture_store64(output + 56, 4);
    return 0;
}
static uint32_t readiness(uint64_t, uint32_t, uint64_t output) {
    std::memset(reinterpret_cast<void *>(static_cast<uintptr_t>(output)), 0, 32);
    return 0;
}
static uint32_t submit(uint64_t, uint64_t, uint32_t, uint64_t, uint64_t, uint64_t output) {
    fixture_store64(output, 77);
    return 0;
}
int main() {
    Node node(*fixture_backend(), "portable-resources");
    wire::service_response original;
    original.success = true;
    original.message = "all fields";
    payload(wire::encode(original));
    ServiceClient<srv::Trigger> client(node.context(), "service.call", 128);
    srv::Trigger::Request request;
    srv::Trigger::Response actual;
    assert(!client.error() && !client.call(request, actual) && actual.success &&
           actual.message == original.message);
    fixture_request_error = 110;
    unsigned before = fixture_requests;
    assert(client.call(request, actual) == 110 && fixture_requests == before + 1 &&
           actual.message == "all fields");
    fixture_request_error = 0;
    ServiceServer<srv::SetBool> server(node.context(), "service.advertise", "service.respond", 128);
    assert(!server.error());
    active_server = &server;
#if ROSWASM_CPP_LEGACY
    server.on_request(legacy_response, 0);
#else
    server.on_request(response);
#endif
    wire::service_envelope incoming;
    incoming.request_id = 51;
    incoming.payload = wire::boolean(true);
    payload(wire::encode(incoming));
    assert(!server.poll_once() && handled == 1);
    assert(!server.close());
    ActionClient send(node.context(), "goal.send", ROSWASMC_ACTION_SEND_GOAL, 32);
    payload(wire::goal_id(UINT64_C(0xfedcba9876543210)));
    uint64_t id = 0;
    assert(!send.send_goal(3, id) && id == UINT64_C(0xfedcba9876543210));
    before = fixture_requests;
    bool accepted = false;
    assert(send.cancel(id, accepted) == 13 && fixture_requests == before);
    ActionClientAliases aliases;
    aliases.send = "bundle.send";
    aliases.cancel_own = "bundle.own";
    aliases.feedback = "bundle.feedback";
    aliases.result = "bundle.result";
    ActionClientBundle bundle(node.context(), aliases, 3, 256);
    assert(!bundle.error());
    payload(wire::goal_id(53));
    assert(!bundle.send_goal(3, id) && id == 53 && bundle.owns_goal(id));
    payload(wire::boolean(true));
    assert(!bundle.cancel_own(id, accepted) && accepted && bundle.owns_goal(id));
    before = fixture_requests;
    assert(bundle.cancel_any(id, accepted) == 13 && fixture_requests == before);
    wire::action_sequence sequence;
    sequence.goal = id;
    sequence.kind = 2;
    sequence.values.push_back(INT32_MIN);
    sequence.values.push_back(-1);
    sequence.values.push_back(8);
    payload(wire::encode(sequence));
    wire::action_sequence result;
    assert(!bundle.receive_result(id, result) && result.values == sequence.values &&
           result.kind == 2 && !bundle.owns_goal(id));
    before = fixture_requests;
    assert(bundle.receive_result(id, result) == 9 && fixture_requests == before);
    assert(!bundle.close());
    Parameters get(node.context(), "parameter.get", ROSWASMC_PARAMETER_GET, 128);
    std::vector<int64_t> values;
    values.push_back(INT64_MIN);
    values.push_back(INT64_MAX);
    payload(wire::parameter_int64_array(values));
    std::vector<int64_t> read;
    assert(!get.get(read) && read == values);
    Parameters set(node.context(), "parameter.set", ROSWASMC_PARAMETER_SET, 0);
    fixture_payload_size = 0;
    assert(!set.set(values));
    before = fixture_requests;
    assert(set.get(read) == 13 && fixture_requests == before);
    Parameters exists(node.context(), "parameter.exists", ROSWASMC_PARAMETER_EXISTS, 8);
    payload(wire::boolean(true));
    bool present = false;
    assert(!exists.exists(present) && present);
    Parameters erase(node.context(), "parameter.erase", ROSWASMC_PARAMETER_DELETE, 0);
    fixture_payload_size = 0;
    assert(!erase.erase());
    Parameters metadata(node.context(), "parameter.describe", ROSWASMC_PARAMETER_DESCRIBE, 128);
    payload(wire::string("complete metadata"));
    std::string text;
    assert(!metadata.metadata(text) && text == "complete metadata");
    Graph graph(node.context(), "graph", ROSWASMC_GRAPH_LIST_TOPICS, 128);
    payload(wire::string("complete graph"));
    assert(!graph.inspect(text) && text == "complete graph");
    Clock clock(node.context(), "clock", ROSWASMC_TIME_READ_ROS);
    payload(wire::lookup_time(UINT64_MAX));
    uint64_t time = 0;
    assert(!clock.read(time) && time == UINT64_MAX);
    Logger logger(node.context(), "logger");
    before = fixture_publishes;
    assert(!logger.log("message") && fixture_publishes == before + 1);
    TF lookup(node.context(), "lookup", ROSWASMC_TF_LOOKUP, 256);
    msg::Transform transform;
    transform.parent_frame = "parent";
    transform.child_frame = "child";
    transform.translation[1] = -2.0;
    payload(wire::encode(transform));
    msg::Transform copied;
    assert(!lookup.lookup(0, copied) && copied.child_frame == "child" &&
           copied.translation[1] == -2.0 && copied.quaternion[3] == 1);
    TF broadcast(node.context(), "broadcast", ROSWASMC_TF_BROADCAST, 0);
    fixture_payload_size = 0;
    assert(!broadcast.broadcast(transform));
    Lifecycle state(node.context(), "state", ROSWASMC_LIFECYCLE_GET_STATE, 128);
    wire::lifecycle_state native;
    native.id = 3;
    native.label = "active";
    payload(wire::encode(native));
    LifecycleState observed;
    assert(!state.get_state(observed) && observed.id == 3 && observed.label == "active");
    Lifecycle change(node.context(), "change", ROSWASMC_LIFECYCLE_CHANGE_STATE, 1);
    payload(wire::boolean(true));
    assert(!change.change_state(accepted) && accepted);
    Capability source(node.context(), "event.source"), observer(node.context(), "event.observer");
    event_source = source.handle();
    roswasmc_event_backend api = {2, watch, event_poll, readiness, submit};
    EventStream events(observer, source, api);
    assert(!events.error());
    unsigned char event_bytes[68];
    EventPoll first = events.poll(event_bytes, sizeof event_bytes);
    assert(!first.result.error && first.header.source == event_source &&
           first.header.sequence == 1);
    assert(events.poll(event_bytes, sizeof event_bytes).result.error == 22);
    event_sequence = 2;
    assert(!events.poll(event_bytes, sizeof event_bytes).result.error);
    uint64_t ticket = 0;
    assert(!events.submit(ByteView(), ticket) && ticket == 77);
    assert(!events.close_checked());
    uwvm_industrial::Journal journal;
    assert(!journal.before_send() && journal.outcome().native_outcome_unknown);
    assert(journal.before_send() == 17);
    assert(!journal.accepted(64));
    assert(!journal.before_cancel(64) && !journal.cancelled(64, true) &&
           journal.outcome().native_outcome_unknown);
    assert(!journal.terminal(64, 2, 0) && !journal.outcome().native_outcome_unknown);
    assert(!journal.before_retire() && journal.before_retire() == 9);
    return 0;
}
