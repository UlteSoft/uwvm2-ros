// Native execution of the actual common dispatch core with local fake sources.
// These counters/bytes are not native ROS behavior, authentication or policy.
package roswasm

import "testing"

type callbackFixture struct {
	polls, replies int
	code           uint32
	bad            bool
}

func (f *callbackFixture) pollService(out []byte) (uint64, error) {
	f.polls++
	if f.code != 0 {
		return 0, Error{Code: f.code}
	}
	copy(out, []byte{7, 0, 0, 0, 0, 0, 0, 0})
	if f.bad {
		out[8] = 2
		return 9, nil
	}
	return 8, nil
}
func (f *callbackFixture) respondService(wire []byte) error {
	f.replies++
	id, err := DecodeTriggerEvent(wire[:8])
	if err != nil || id.Raw() != 7 {
		return Error{Code: Invalid}
	}
	return nil
}
func (f *callbackFixture) pollGoal() (GoalID, int32, error) {
	f.polls++
	id, err := DecodeGoalID([]byte{7, 0, 0, 0, 0, 0, 0, 0})
	return id, 5, err
}
func callbackReply(id RequestID, _ []byte, out []byte) (uint64, error) {
	return EncodeServiceReply(id, ServiceResponse{Success: true, Message: "ok"}, out)
}
func TestActualServiceDispatchGuardAndIdentity(t *testing.T) {
	state := &callbackState{}
	source := &callbackFixture{}
	var input [16]byte
	var output [32]byte
	delivered, err := serviceOnce(state, source, input[:], output[:], func(id RequestID, request, out []byte) (uint64, error) {
		if !isCode(callbackIdle(state), Conflict) {
			t.Fatal("close guard")
		}
		before := source.polls
		var other [48]byte
		if _, err := serviceOnce(state, source, other[:16], other[16:], callbackReply); !isCode(err, Conflict) || source.polls != before {
			t.Fatal("nested dequeue", err)
		}
		if id.Raw() != 7 || len(request) != 0 {
			t.Fatal("identity")
		}
		return callbackReply(id, request, out)
	})
	if err != nil || !delivered || source.polls != 1 || source.replies != 1 || callbackIdle(state) != nil {
		t.Fatal("actual dispatch", err)
	}
	if _, err = serviceOnce(state, source, input[:], input[:], callbackReply); !isCode(err, Invalid) || source.polls != 1 {
		t.Fatal("overlap consumed", err)
	}
}
func TestActualServiceRefusalNeverResponds(t *testing.T) {
	state := &callbackState{}
	source := &callbackFixture{}
	var input [16]byte
	var output [32]byte
	_, err := serviceOnce(state, source, input[:], output[:], func(id RequestID, request, out []byte) (uint64, error) {
		n, err := callbackReply(id, request, out)
		out[0] = 8
		return n, err
	})
	if !isCode(err, Invalid) || source.replies != 0 {
		t.Fatal("wrong reply ID imported", err)
	}
	source.bad = true
	_, err = serviceOnce(state, source, input[:], output[:], func(RequestID, []byte, []byte) (uint64, error) { t.Fatal("malformed callback"); return 0, nil })
	if !isCode(err, Invalid) || source.replies != 0 || callbackIdle(state) != nil {
		t.Fatal("malformed", err)
	}
	source.code = Again
	if delivered, err := serviceOnce(state, source, input[:], output[:], callbackReply); delivered || err != nil || source.replies != 0 {
		t.Fatal("idle callback", err)
	}
}
func TestActualGoalDispatchRefusesNestedPoll(t *testing.T) {
	state := &callbackState{}
	source := &callbackFixture{}
	delivered, err := goalOnce(state, source, func(id GoalID, order int32) error {
		if id.Raw() != 7 || order != 5 {
			t.Fatal("goal identity")
		}
		before := source.polls
		_, err := goalOnce(state, source, func(GoalID, int32) error { t.Fatal("nested handler"); return nil })
		if !isCode(err, Conflict) || source.polls != before || !isCode(callbackIdle(state), Conflict) {
			t.Fatal("nested poll/close", err)
		}
		return nil
	})
	if err != nil || !delivered || source.polls != 1 || callbackIdle(state) != nil {
		t.Fatal("goal dispatch", err)
	}
}
