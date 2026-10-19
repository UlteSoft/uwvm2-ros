package roswasm

import (
	"encoding/binary"
	"testing"
)

// These native finite-source fixtures use the actual private production
// dispatcher/executor. They create no raw imports, capability or ROS resource.
type eventFixture struct {
	state         capabilityState
	binding       EventBinding
	wire          []byte
	polls, closes int
	idle          bool
}

func newEventDispatchFixture() *eventFixture {
	binding, _ := NewEventBinding(2, 0xf000000000000011, 0)
	return &eventFixture{state: capabilityState{live: true, callback: &callbackState{}}, binding: binding, wire: unifiedFixture(EventTopicMessage)}
}
func (f *eventFixture) observerState() *capabilityState { return &f.state }
func (f *eventFixture) poll(out []byte) (uint64, error) {
	f.polls++
	if f.idle {
		return 0, Error{Code: Again}
	}
	if len(out) < len(f.wire) {
		return 0, Error{Code: TooLarge}
	}
	copy(out, f.wire)
	return uint64(len(f.wire)), nil
}
func (f *eventFixture) SpinOnce(out []byte, h func(UnifiedEvent) error) (bool, error) {
	return dispatchUnified(f.state.callback, &f.binding, out, f.poll, h)
}
func (f *eventFixture) Close() error {
	return retireOwner(&f.state, func(uint64) error { f.closes++; return nil })
}
func TestEventExecutorDispatchMutationAndOwnerLease(t *testing.T) {
	x, err := NewEventExecutor(64)
	if err != nil {
		t.Fatal(err)
	}
	f := newEventDispatchFixture()
	calls := 0
	if err = x.addStream(f, func(event UnifiedEvent) error {
		calls++
		if event.Sequence != 1 {
			t.Fatal("complete event")
		}
		before := f.polls
		if _, e := x.SpinOnce(); e.(Error).Code != Conflict {
			t.Fatal("nested executor")
		}
		if _, e := f.SpinOnce(make([]byte, 64), func(UnifiedEvent) error { return nil }); e.(Error).Code != Conflict || f.polls != before {
			t.Fatal("nested source")
		}
		if e := f.Close(); e.(Error).Code != Conflict || f.closes != 0 {
			t.Fatal("owner retirement")
		}
		if e := x.addStream(f, func(UnifiedEvent) error { return nil }); e.(Error).Code != Conflict {
			t.Fatal("callback registration")
		}
		return nil
	}); err != nil {
		t.Fatal(err)
	}
	if n, e := x.SpinOnce(); e != nil || n != 1 || calls != 1 {
		t.Fatal("actual executor callback", n, e)
	}
	if _, e := x.removeStream(0); e != nil {
		t.Fatal(e)
	}
	if err = x.Close(); err != nil {
		t.Fatal(err)
	}
	if err = f.Close(); err != nil || f.closes != 1 {
		t.Fatal("checked observer close")
	}
}
func TestEventBindingRejectsForeignReplayAndLengthTransactionally(t *testing.T) {
	f := newEventDispatchFixture()
	var out [64]byte
	if ok, e := f.SpinOnce(out[:], func(UnifiedEvent) error { return nil }); !ok || e != nil {
		t.Fatal(e)
	}
	old := f.binding
	for _, change := range []struct {
		at    int
		value uint64
	}{{32, 1}, {16, 9}, {48, 99}, {56, 1}} {
		f.wire = unifiedFixture(EventTopicMessage)
		binary.LittleEndian.PutUint64(f.wire[32:], 2)
		binary.LittleEndian.PutUint64(f.wire[change.at:], change.value)
		if _, err := f.SpinOnce(out[:], func(UnifiedEvent) error { t.Fatal("invalid callback"); return nil }); err == nil || f.binding != old {
			t.Fatal("refusal changed binding")
		}
	}
	f.wire = unifiedFixture(EventTopicMessage)
	binary.LittleEndian.PutUint64(f.wire[32:], 2)
	if ok, e := f.SpinOnce(out[:], func(UnifiedEvent) error { return Error{Code: Denied} }); ok || e.(Error).Code != Denied || f.binding.sequence != 2 || f.state.callback.busy {
		t.Fatal("callback failure must release without replay")
	}
}
func TestEventExecutorIdleCapacityAndDuplicateRegistration(t *testing.T) {
	x, _ := NewEventExecutor(64)
	f := newEventDispatchFixture()
	f.idle = true
	if err := x.addStream(f, func(UnifiedEvent) error { return nil }); err != nil {
		t.Fatal(err)
	}
	if e := x.addStream(f, func(UnifiedEvent) error { return nil }); e.(Error).Code != Conflict {
		t.Fatal("duplicate observer")
	}
	if n, e := x.SpinOnce(); n != 0 || e != nil {
		t.Fatal("idle")
	}
	if err := x.Close(); err != nil || f.closes != 1 {
		t.Fatal("owned observer cleanup")
	}
	if _, e := NewEventExecutor(63); e == nil {
		t.Fatal("envelope capacity")
	}
	if _, e := NewEventExecutor(4194305); e == nil {
		t.Fatal("upper bound")
	}
}
