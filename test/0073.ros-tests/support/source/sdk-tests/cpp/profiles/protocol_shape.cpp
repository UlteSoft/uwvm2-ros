/** @file
 * @brief Native model checks of lifecycle/action request framing in every C++ mode.
 * @details Strict test-owned dispatch callbacks check the same empty lifecycle
 * request and marked action response required by the gateway. No real import,
 * Guest authority, ROS node, robot or network endpoint is used.
 * @see src/ros/gateway/ros2/lifecycle.h
 * @see src/ros/gateway/ros2/action.h
 */
#include <platform/ros/cpp/roswasm/roswasmcpp.hpp>
#include "../../c/ownership_backend.h"
#include <cassert>
#include <cstring>

using namespace roswasmcpp;

static unsigned lifecycle_requests;
static unsigned response_requests;
static unsigned cancel_requests;
static unsigned char response_kind;

static void payload(wire::bytes const &value) {
    assert(value.size() <= sizeof fixture_payload);
    fixture_payload_size = value.size();
    if (!value.empty())
        std::memcpy(fixture_payload, buffer_data(value), value.size());
}

static uint32_t lifecycle_request(uint64_t handle, uint32_t memory, uint64_t input,
                                  uint64_t input_size, uint64_t output, uint64_t capacity,
                                  uint64_t slot) {
    assert(input_size == 0);
    ++lifecycle_requests;
    return fixture_request(handle, memory, input, input_size, output, capacity, slot);
}

static uint32_t action_response(uint64_t handle, uint32_t memory, uint64_t input,
                                uint64_t input_size, uint64_t output, uint64_t capacity,
                                uint64_t slot) {
    assert(input_size >= 13);
    roswasmc_action_sequence sequence;
    assert(!roswasmc_decode_action_sequence(
        reinterpret_cast<void const *>(static_cast<uintptr_t>(input)),
        static_cast<size_t>(input_size), 1, &sequence));
    assert(sequence.goal == 61 && sequence.count == 2);
    response_kind = sequence.kind;
    ++response_requests;
    return fixture_request(handle, memory, input, input_size, output, capacity, slot);
}

static uint32_t cancel_all_request(uint64_t handle, uint32_t memory, uint64_t input,
                                   uint64_t input_size, uint64_t output, uint64_t capacity,
                                   uint64_t slot) {
    assert(input_size == 8);
    assert(uwvm_ros_load_u64(
               reinterpret_cast<unsigned char const *>(static_cast<uintptr_t>(input))) == 0);
    ++cancel_requests;
    return fixture_request(handle, memory, input, input_size, output, capacity, slot);
}

int main() {
    roswasmc_backend backend = *fixture_backend();
    backend.request[ROSWASMC_LIFECYCLE_CHANGE_STATE] = lifecycle_request;
    backend.request[ROSWASMC_ACTION_RESPOND] = action_response;
    backend.request[ROSWASMC_ACTION_CANCEL_ANY_GOAL] = cancel_all_request;
    roswasmc_context context = {&backend};

    Lifecycle change(context, "change", ROSWASMC_LIFECYCLE_CHANGE_STATE, 1);
    payload(wire::boolean(true));
    bool accepted = false;
    assert(!change.error() && !change.change_state(accepted) && accepted);
    assert(lifecycle_requests == 1);
    payload(wire::boolean(false));
    assert(!change.change_state(accepted) && !accepted && lifecycle_requests == 2);
    fixture_payload[0] = 2;
    accepted = true;
    assert(change.change_state(accepted) == UWVM_ROS_INVALID && accepted);
    assert(lifecycle_requests == 3);
    fixture_request_error = 110;
    assert(change.change_state(accepted) == 110 && accepted && lifecycle_requests == 4);
    fixture_request_error = 0;
#if ROSWASM_CPP_LEGACY || ROSWASM_CPP_STANDARD < 20
    unsigned calls = fixture_requests;
    assert(change.change(1) == UWVM_ROS_INVALID && fixture_requests == calls);
#endif

    ActionClient cancel(context, "cancel-any", ROSWASMC_ACTION_CANCEL_ANY_GOAL, 1);
    payload(wire::boolean(true));
    accepted = false;
    assert(!cancel.error() && !cancel.cancel_all(accepted) && accepted && cancel_requests == 1);
    ActionClient own(context, "cancel-own", ROSWASMC_ACTION_CANCEL_OWN_GOAL, 1);
    unsigned before = fixture_requests;
    assert(own.cancel_all(accepted) == UWVM_ROS_DENIED && fixture_requests == before);

    ActionServer server(context, "action-advertise", "action-respond", 12);
    assert(!server.error());
    wire::action_sequence sequence;
    sequence.goal = 61;
    sequence.kind = 1;
    sequence.values.push_back(-1);
    sequence.values.push_back(8);
    fixture_payload_size = 0;
    assert(!server.respond(sequence) && response_requests == 1 && response_kind == 1);
#if ROSWASM_CPP_LEGACY || ROSWASM_CPP_STANDARD < 20
    assert(!server.respond(sequence, false) && response_requests == 2 && response_kind == 1);
    assert(!server.respond(sequence, true) && response_requests == 3 && response_kind == 1);
#endif
    sequence.kind = 2;
    assert(!server.respond(sequence) && response_kind == 2);
#if ROSWASM_CPP_LEGACY || ROSWASM_CPP_STANDARD < 20
    before = fixture_requests;
    assert(server.respond(sequence, false) == UWVM_ROS_INVALID && fixture_requests == before);
#endif

    ActionClient result(context, "result", ROSWASMC_ACTION_RECEIVE_RESULT, 64);
    wire::action_sequence decoded;
    decoded.goal = 99;
    decoded.kind = 4;
    decoded.values.push_back(777);
    sequence.kind = 1;
    payload(wire::encode(sequence, true));
    before = fixture_requests;
    assert(result.receive(61, decoded) == UWVM_ROS_INVALID && fixture_requests == before + 1);
    assert(decoded.goal == 99 && decoded.kind == 4 && decoded.values.size() == 1 &&
           decoded.values[0] == 777);
    sequence.kind = 2;
    payload(wire::encode(sequence, true));
    assert(!result.receive(61, decoded) && decoded.goal == 61 && decoded.kind == 2 &&
           decoded.values == sequence.values);
    return 0;
}
