//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1

// Code derived from the runtime ABI registry. All arguments use explicit widths.
package roswasm

// RawAbiVersion calls abi_version in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 abi_version
func RawAbiVersion() uint32

// RawCapabilityOpen calls capability_open in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 capability_open
func RawCapabilityOpen(memory uint32, alias_offset uint64, alias_length uint64, handle_out_offset uint64) uint32

// RawCapabilityClose calls capability_close in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 capability_close
func RawCapabilityClose(handle uint64) uint32

// RawCapabilityRevoke calls capability_revoke in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 capability_revoke
func RawCapabilityRevoke(handle uint64) uint32

// RawTopicPublish calls topic_publish in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 topic_publish
func RawTopicPublish(handle uint64, memory uint32, payload_offset uint64, payload_length uint64) uint32

// RawTopicSubscribe calls topic_subscribe in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 topic_subscribe
func RawTopicSubscribe(handle uint64) uint32

// RawEventPoll calls event_poll in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 event_poll
func RawEventPoll(handle uint64, memory uint32, output_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawServiceCall calls service_call in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 service_call
func RawServiceCall(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawServiceAdvertise calls service_advertise in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 service_advertise
func RawServiceAdvertise(handle uint64) uint32

// RawServiceRespond calls service_respond in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 service_respond
func RawServiceRespond(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawActionSendGoal calls action_send_goal in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 action_send_goal
func RawActionSendGoal(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawActionCancelOwnGoal calls action_cancel_own_goal in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 action_cancel_own_goal
func RawActionCancelOwnGoal(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawActionCancelAnyGoal calls action_cancel_any_goal in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 action_cancel_any_goal
func RawActionCancelAnyGoal(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawActionReceiveFeedback calls action_receive_feedback in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 action_receive_feedback
func RawActionReceiveFeedback(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawActionReceiveResult calls action_receive_result in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 action_receive_result
func RawActionReceiveResult(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawActionAdvertise calls action_advertise in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 action_advertise
func RawActionAdvertise(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawActionRespond calls action_respond in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 action_respond
func RawActionRespond(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawTfLookup calls tf_lookup in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 tf_lookup
func RawTfLookup(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawTfBroadcast calls tf_broadcast in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 tf_broadcast
func RawTfBroadcast(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawParameterGet calls parameter_get in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 parameter_get
func RawParameterGet(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawParameterSet calls parameter_set in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 parameter_set
func RawParameterSet(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawParameterList calls parameter_list in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 parameter_list
func RawParameterList(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawParameterDescribe calls parameter_describe in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 parameter_describe
func RawParameterDescribe(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawParameterDeclare calls parameter_declare in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 parameter_declare
func RawParameterDeclare(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawParameterUndeclare calls parameter_undeclare in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 parameter_undeclare
func RawParameterUndeclare(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawGraphInspectTopic calls graph_inspect_topic in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 graph_inspect_topic
func RawGraphInspectTopic(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawGraphInspectNode calls graph_inspect_node in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 graph_inspect_node
func RawGraphInspectNode(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawGraphInspectService calls graph_inspect_service in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 graph_inspect_service
func RawGraphInspectService(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawGraphInspectAction calls graph_inspect_action in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 graph_inspect_action
func RawGraphInspectAction(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawParameterSubscribeEvents calls parameter_subscribe_events in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 parameter_subscribe_events
func RawParameterSubscribeEvents(handle uint64) uint32

// RawGraphListTopics calls graph_list_topics in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 graph_list_topics
func RawGraphListTopics(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawGraphListNodes calls graph_list_nodes in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 graph_list_nodes
func RawGraphListNodes(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawGraphListServices calls graph_list_services in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 graph_list_services
func RawGraphListServices(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawLifecycleGetState calls lifecycle_get_state in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 lifecycle_get_state
func RawLifecycleGetState(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawLifecycleChangeState calls lifecycle_change_state in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 lifecycle_change_state
func RawLifecycleChangeState(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawTimeReadRos calls time_read_ros in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 time_read_ros
func RawTimeReadRos(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawTimeReadSystem calls time_read_system in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 time_read_system
func RawTimeReadSystem(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawTimeReadSteady calls time_read_steady in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 time_read_steady
func RawTimeReadSteady(handle uint64, memory uint32, request_offset uint64, request_length uint64, response_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawLogWrite calls log_write in uwvm_ros2_v1.
// Pass guest-memory offsets, not host addresses; all output slots are eight LE bytes.
// This low-level function does not infer authority or preserve output on errors.
//
//go:wasmimport uwvm_ros2_v1 log_write
func RawLogWrite(handle uint64, memory uint32, payload_offset uint64, payload_length uint64) uint32

func invoke(op Operation, h uint64, m uint32, a, n, b, c, z uint64) uint32 {
	switch op {
	case OpServiceCall:
		return RawServiceCall(h, m, a, n, b, c, z)
	case OpServiceRespond:
		return RawServiceRespond(h, m, a, n, b, c, z)
	case OpActionSendGoal:
		return RawActionSendGoal(h, m, a, n, b, c, z)
	case OpActionCancelOwnGoal:
		return RawActionCancelOwnGoal(h, m, a, n, b, c, z)
	case OpActionCancelAnyGoal:
		return RawActionCancelAnyGoal(h, m, a, n, b, c, z)
	case OpActionReceiveFeedback:
		return RawActionReceiveFeedback(h, m, a, n, b, c, z)
	case OpActionReceiveResult:
		return RawActionReceiveResult(h, m, a, n, b, c, z)
	case OpActionAdvertise:
		return RawActionAdvertise(h, m, a, n, b, c, z)
	case OpActionRespond:
		return RawActionRespond(h, m, a, n, b, c, z)
	case OpTfLookup:
		return RawTfLookup(h, m, a, n, b, c, z)
	case OpTfBroadcast:
		return RawTfBroadcast(h, m, a, n, b, c, z)
	case OpParameterGet:
		return RawParameterGet(h, m, a, n, b, c, z)
	case OpParameterSet:
		return RawParameterSet(h, m, a, n, b, c, z)
	case OpParameterList:
		return RawParameterList(h, m, a, n, b, c, z)
	case OpParameterDescribe:
		return RawParameterDescribe(h, m, a, n, b, c, z)
	case OpParameterDeclare:
		return RawParameterDeclare(h, m, a, n, b, c, z)
	case OpParameterUndeclare:
		return RawParameterUndeclare(h, m, a, n, b, c, z)
	case OpGraphInspectTopic:
		return RawGraphInspectTopic(h, m, a, n, b, c, z)
	case OpGraphInspectNode:
		return RawGraphInspectNode(h, m, a, n, b, c, z)
	case OpGraphInspectService:
		return RawGraphInspectService(h, m, a, n, b, c, z)
	case OpGraphInspectAction:
		return RawGraphInspectAction(h, m, a, n, b, c, z)
	case OpGraphListTopics:
		return RawGraphListTopics(h, m, a, n, b, c, z)
	case OpGraphListNodes:
		return RawGraphListNodes(h, m, a, n, b, c, z)
	case OpGraphListServices:
		return RawGraphListServices(h, m, a, n, b, c, z)
	case OpLifecycleGetState:
		return RawLifecycleGetState(h, m, a, n, b, c, z)
	case OpLifecycleChangeState:
		return RawLifecycleChangeState(h, m, a, n, b, c, z)
	case OpTimeReadRos:
		return RawTimeReadRos(h, m, a, n, b, c, z)
	case OpTimeReadSystem:
		return RawTimeReadSystem(h, m, a, n, b, c, z)
	case OpTimeReadSteady:
		return RawTimeReadSteady(h, m, a, n, b, c, z)
	default:
		return Unsupported
	}
}

func subscribeParameterEvents(h uint64) uint32 { return RawParameterSubscribeEvents(h) }

// RawEventWatch calls event_watch in uwvm_ros2_v1.
// Bind separately granted event.observe and source owners. Correlation is nonzero only when an owned outgoing Action data kind is selected; metadata-only watches use zero and activate/consume no native payload. No aliases or rights are created.
// Offsets are full-width selected-memory offsets; imports grant no authority.
//go:wasmimport uwvm_ros2_v1 event_watch
func RawEventWatch(observer_handle uint64, source_handle uint64, correlation uint64) uint32

// RawEventPollV1 calls event_poll_v1 in uwvm_ros2_v1.
// Read one complete versioned64-byte-header event. Error90 writes required size and retains the event; error11 is idle. No automatic replay.
// Offsets are full-width selected-memory offsets; imports grant no authority.
//go:wasmimport uwvm_ros2_v1 event_poll_v1
func RawEventPollV1(handle uint64, memory uint32, output_offset uint64, capacity uint64, length_out_offset uint64) uint32

// RawEventReady calls event_ready in uwvm_ros2_v1.
// Write32 little-endian bytes: ready count,next record bytes,admission slots,admission bytes. A readiness snapshot is not a reservation.
// Offsets are full-width selected-memory offsets; imports grant no authority.
//go:wasmimport uwvm_ros2_v1 event_ready
func RawEventReady(observer_handle uint64, memory uint32, output_offset uint64) uint32

// RawServiceSubmit calls service_submit in uwvm_ros2_v1.
// Admit one bounded asynchronous service request. A successful acknowledgement writes requestID; completion requires explicit service-response and resource-error observation. Never replay a timeout.
// Offsets are full-width selected-memory offsets; imports grant no authority.
//go:wasmimport uwvm_ros2_v1 service_submit
func RawServiceSubmit(source_handle uint64, observer_handle uint64, memory uint32, request_offset uint64, request_length uint64, request_id_out_offset uint64) uint32

