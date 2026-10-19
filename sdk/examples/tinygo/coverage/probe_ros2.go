//go:build ros2 && !ros1

// Backend-specific probe calls retain exactly the runtime ABI inventory.
package main

import ros "uwvm2.local/roswasm"

func invokeRaw(index uint32) uint32 {
	switch index {
	case 0:
		return ros.RawAbiVersion()
	case 1:
		return ros.RawCapabilityOpen(uint32(0), uint64(0), uint64(0), uint64(0))
	case 2:
		return ros.RawCapabilityClose(uint64(0))
	case 3:
		return ros.RawCapabilityRevoke(uint64(0))
	case 4:
		return ros.RawTopicPublish(uint64(0), uint32(0), uint64(0), uint64(0))
	case 5:
		return ros.RawTopicSubscribe(uint64(0))
	case 6:
		return ros.RawEventPoll(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0))
	case 7:
		return ros.RawServiceCall(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 8:
		return ros.RawServiceAdvertise(uint64(0))
	case 9:
		return ros.RawServiceRespond(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 10:
		return ros.RawActionSendGoal(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 11:
		return ros.RawActionCancelOwnGoal(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 12:
		return ros.RawActionCancelAnyGoal(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 13:
		return ros.RawActionReceiveFeedback(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 14:
		return ros.RawActionReceiveResult(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 15:
		return ros.RawActionAdvertise(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 16:
		return ros.RawActionRespond(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 17:
		return ros.RawTfLookup(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 18:
		return ros.RawTfBroadcast(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 19:
		return ros.RawParameterGet(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 20:
		return ros.RawParameterSet(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 21:
		return ros.RawParameterList(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 22:
		return ros.RawParameterDescribe(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 23:
		return ros.RawParameterDeclare(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 24:
		return ros.RawParameterUndeclare(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 25:
		return ros.RawGraphInspectTopic(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 26:
		return ros.RawGraphInspectNode(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 27:
		return ros.RawGraphInspectService(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 28:
		return ros.RawGraphInspectAction(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 29:
		return ros.RawParameterSubscribeEvents(uint64(0))
	case 30:
		return ros.RawGraphListTopics(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 31:
		return ros.RawGraphListNodes(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 32:
		return ros.RawGraphListServices(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 33:
		return ros.RawLifecycleGetState(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 34:
		return ros.RawLifecycleChangeState(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 35:
		return ros.RawTimeReadRos(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 36:
		return ros.RawTimeReadSystem(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 37:
		return ros.RawTimeReadSteady(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0), uint64(0), uint64(0))
	case 38:
		return ros.RawLogWrite(uint64(0), uint32(0), uint64(0), uint64(0))
	case 39:
		return ros.RawEventWatch(uint64(0), uint64(0), uint64(0))
	case 40:
		return ros.RawEventPollV1(uint64(0), uint32(0), uint64(0), uint64(0), uint64(0))
	case 41:
		return ros.RawEventReady(uint64(0), uint32(0), uint64(0))
	case 42:
		return ros.RawServiceSubmit(uint64(0), uint64(0), uint32(0), uint64(0), uint64(0), uint64(0))
	default:
		return ros.Unsupported
	}
}
