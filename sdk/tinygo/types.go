// Package roswasm provides the uwvm2 capability-based ROS guest SDK.
// The raw layer imports one backend selected with tinygo, wasm and ros1/ros2 tags.
// Memory-zero pointer helpers reject widths other than wasm32; raw imports keep
// explicit uint32 memory indices and uint64 offsets, lengths and handles.
package roswasm

import "encoding/binary"

// Operation selects an exact request/response import. Backend-specific operations
// return Unsupported; the SDK never forwards a call to the other ROS backend.
type Operation uint32

const (
	// OpServiceCall selects service_call with its own explicit capability grant.
	OpServiceCall Operation = 1
	// OpServiceRespond selects service_respond with its own explicit capability grant.
	OpServiceRespond Operation = 2
	// OpActionSendGoal selects action_send_goal with its own explicit capability grant.
	OpActionSendGoal Operation = 3
	// OpActionCancelOwnGoal selects action_cancel_own_goal with its own explicit capability grant.
	OpActionCancelOwnGoal Operation = 4
	// OpActionCancelAnyGoal selects action_cancel_any_goal with its own explicit capability grant.
	OpActionCancelAnyGoal Operation = 5
	// OpActionReceiveFeedback selects action_receive_feedback with its own explicit capability grant.
	OpActionReceiveFeedback Operation = 6
	// OpActionReceiveResult selects action_receive_result with its own explicit capability grant.
	OpActionReceiveResult Operation = 7
	// OpActionAdvertise selects action_advertise with its own explicit capability grant.
	OpActionAdvertise Operation = 8
	// OpActionRespond selects action_respond with its own explicit capability grant.
	OpActionRespond Operation = 9
	// OpTfLookup selects tf_lookup with its own explicit capability grant.
	OpTfLookup Operation = 10
	// OpTfBroadcast selects tf_broadcast with its own explicit capability grant.
	OpTfBroadcast Operation = 11
	// OpParameterGet selects parameter_get with its own explicit capability grant.
	OpParameterGet Operation = 12
	// OpParameterSet selects parameter_set with its own explicit capability grant.
	OpParameterSet Operation = 13
	// OpParameterDelete selects parameter_delete with its own explicit capability grant.
	OpParameterDelete Operation = 14
	// OpParameterExists selects parameter_exists with its own explicit capability grant.
	OpParameterExists Operation = 15
	// OpParameterList selects parameter_list with its own explicit capability grant.
	OpParameterList Operation = 16
	// OpParameterSearch selects parameter_search with its own explicit capability grant.
	OpParameterSearch Operation = 17
	// OpGraphInspectTopic selects graph_inspect_topic with its own explicit capability grant.
	OpGraphInspectTopic Operation = 18
	// OpGraphInspectNode selects graph_inspect_node with its own explicit capability grant.
	OpGraphInspectNode Operation = 19
	// OpGraphInspectService selects graph_inspect_service with its own explicit capability grant.
	OpGraphInspectService Operation = 20
	// OpGraphListTopics selects graph_list_topics with its own explicit capability grant.
	OpGraphListTopics Operation = 21
	// OpGraphListNodes selects graph_list_nodes with its own explicit capability grant.
	OpGraphListNodes Operation = 22
	// OpGraphListServices selects graph_list_services with its own explicit capability grant.
	OpGraphListServices Operation = 23
	// OpTimeReadRos selects time_read_ros with its own explicit capability grant.
	OpTimeReadRos Operation = 24
	// OpTimeReadWall selects time_read_wall with its own explicit capability grant.
	OpTimeReadWall Operation = 25
	// OpParameterDescribe selects parameter_describe with its own explicit capability grant.
	OpParameterDescribe Operation = 26
	// OpParameterDeclare selects parameter_declare with its own explicit capability grant.
	OpParameterDeclare Operation = 27
	// OpParameterUndeclare selects parameter_undeclare with its own explicit capability grant.
	OpParameterUndeclare Operation = 28
	// OpGraphInspectAction selects graph_inspect_action with its own explicit capability grant.
	OpGraphInspectAction Operation = 29
	// OpLifecycleGetState selects lifecycle_get_state with its own explicit capability grant.
	OpLifecycleGetState Operation = 30
	// OpLifecycleChangeState selects lifecycle_change_state with its own explicit capability grant.
	OpLifecycleChangeState Operation = 31
	// OpTimeReadSystem selects time_read_system with its own explicit capability grant.
	OpTimeReadSystem Operation = 32
	// OpTimeReadSteady selects time_read_steady with its own explicit capability grant.
	OpTimeReadSteady Operation = 33
)

// Success means: The operation completed successfully.
const Success uint32 = 0

// NotFound means: The selected object or parameter is missing.
const NotFound uint32 = 2

// BadHandle means: The handle is stale, forged, foreign or already retired.
const BadHandle uint32 = 9

// Again means: The event queue is empty or a quota is temporarily exhausted.
const Again uint32 = 11

// Denied means: The explicit grant refuses this operation or payload.
const Denied uint32 = 13

// Conflict means: Configured contracts conflict.
const Conflict uint32 = 17

// Invalid means: The argument, memory range or canonical payload is invalid.
const Invalid uint32 = 22

// TooLarge means: A payload, quota or output capacity was exceeded.
const TooLarge uint32 = 90

// Unsupported means: This backend, adapter or pointer model does not support the call.
const Unsupported uint32 = 95

// Disconnected means: The gateway transport is disconnected.
const Disconnected uint32 = 107

// Timeout means: A deadline elapsed; already admitted external effects may remain.
const Timeout uint32 = 110

// Revoked means: The alias cannot be reopened after revocation.
const Revoked uint32 = 125

// Error preserves the unsigned host code and an optional observed length.
// An error90 length slot is not always written: HasRequired disambiguates it.
// Do not automatically retry; polling may already have consumed the event.
type Error struct {
	// Code is the unsigned ABI result, including future unknown codes.
	Code uint32
	// Required is valid only when HasRequired is true.
	Required uint64
	// HasRequired means the host overwrote the eight-byte sentinel slot on error90.
	HasRequired bool
}

// Error returns a stable message without converting 64-bit fields through float64.
func (e Error) Error() string {
	return "roswasm: capability call failed (inspect Code and HasRequired)"
}

func status(code uint32) error {
	if code == Success {
		return nil
	}
	return Error{Code: code}
}

func finish(code uint32, slot [8]byte, capacity uint64) (uint64, error) {
	n := binary.LittleEndian.Uint64(slot[:])
	if code != Success {
		return 0, Error{Code: code, Required: n, HasRequired: code == TooLarge && n != ^uint64(0)}
	}
	if n > capacity {
		return 0, Error{Code: Invalid}
	}
	return n, nil
}
