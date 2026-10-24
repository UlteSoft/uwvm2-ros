// Pure executor fixtures use the actual common executor implementation and no
// native ROS import. These tests are prepared; compile/run in the authorized
// cgroup before recording an execution verdict. Passing them is not ROS proof.
// The recover/panic fixture concerns native Go behavior only; it is not a TinyGo
// WASI panic-recovery guarantee.
package roswasm

import "testing"

type fixtureSource struct {
	polls, closes uint32
	value         byte
	code          uint32
}

func (x *fixtureSource) Poll(out []byte) (uint64, error) {
	x.polls++
	if x.code != 0 {
		return 0, Error{Code: x.code}
	}
	if len(out) == 0 {
		return 0, nil
	}
	out[0] = x.value
	return 1, nil
}
func (x *fixtureSource) Close() error    { x.closes++; return nil }
func isCode(err error, code uint32) bool { e, ok := err.(Error); return ok && e.Code == code }

func TestExecutorRefusesCallbackMutationsAndNestedSpin(t *testing.T) {
	e := NewExecutor(8)
	first := &fixtureSource{value: 7}
	extra := &fixtureSource{value: 9}
	var callbacks uint32
	e.add(first, func(data []byte) {
		callbacks++
		before := data[0]
		n, err := e.SpinOnce()
		if n != 0 || !isCode(err, Invalid) {
			t.Fatal("nested spin must refuse before any poll", n, err)
		}
		if data[0] != before || first.polls != callbacks {
			t.Fatal("nested spin modified borrowed scratch or polled again")
		}
		e.add(extra, func([]byte) { t.Fatal("callback-time registration must remain unregistered") })
		if err := e.Close(); !isCode(err, Invalid) {
			t.Fatal("callback-time close must refuse", err)
		}
		if first.closes != 0 || extra.closes != 0 {
			t.Fatal("refused close retired resources")
		}
	})
	for pass := uint32(1); pass <= 2; pass++ {
		n, err := e.SpinOnce()
		if err != nil || n != 1 || first.polls != pass || extra.polls != 0 || len(e.entries) != 1 {
			t.Fatal("one bounded original registration per pass", n, err)
		}
	}
	if err := e.Close(); err != nil || first.closes != 1 || len(e.entries) != 0 {
		t.Fatal("explicit post-pass close", err)
	}
}

func TestExecutorRejectsNilBeforePoll(t *testing.T) {
	e := NewExecutor(8)
	source := &fixtureSource{}
	e.add(nil, func([]byte) {})
	e.add(source, nil)
	if len(e.entries) != 0 {
		t.Fatal("nil registration accepted")
	}
	if n, err := e.SpinOnce(); err != nil || n != 0 || source.polls != 0 {
		t.Fatal("nil registration caused consumption", n, err)
	}
	// Defense against an internally malformed entry still occurs before Poll.
	e.entries = []executorEntry{{source: source, callback: nil}}
	if n, err := e.SpinOnce(); n != 0 || !isCode(err, Invalid) || source.polls != 0 {
		t.Fatal("malformed callback consumed an event", n, err)
	}
}

func TestExecutorResetsGuardAfterCallbackPanic(t *testing.T) {
	e := NewExecutor(8)
	source := &fixtureSource{value: 5}
	e.add(source, func([]byte) { panic("application callback failure") })
	func() {
		defer func() {
			if recover() == nil {
				t.Fatal("expected application panic")
			}
		}()
		_, _ = e.SpinOnce()
	}()
	if e.spinning {
		t.Fatal("panic left executor permanently busy")
	}
	if err := e.Close(); err != nil || source.closes != 1 {
		t.Fatal("cleanup after application panic", err)
	}
}

func TestExecutorIdleAndErrorAccounting(t *testing.T) {
	e := NewExecutor(8)
	idle := &fixtureSource{code: Again}
	success := &fixtureSource{value: 3}
	failure := &fixtureSource{code: Denied}
	later := &fixtureSource{}
	e.add(idle, func([]byte) { t.Fatal("idle callback") })
	e.add(success, func([]byte) {})
	e.add(failure, func([]byte) { t.Fatal("denied callback") })
	e.add(later, func([]byte) { t.Fatal("polled after stopping error") })
	n, err := e.SpinOnce()
	if n != 1 || !isCode(err, Denied) || later.polls != 0 || e.spinning {
		t.Fatal("bounded delivery/error accounting", n, err)
	}
}
