// Pure local-state tests. They use no Capability, native import or clock.
// Root runs native Go tests serially; this is not TinyGo/ROS acceptance.
package roswasm

import "testing"

func TestLocalFutureOneShotAndCancellation(t *testing.T) {
	c, _ := NewContext(2)
	calls := 0
	f, err := Defer(c, func() (uint64, error) { calls++; return 42, nil })
	if err != nil || calls != 0 {
		t.Fatal("inline work", err, calls)
	}
	if _, err = f.Take(); !isCode(err, Again) {
		t.Fatal("pending", err)
	}
	if n, err := c.SpinOnce(0); n != 1 || err != nil || !f.IsReady() {
		t.Fatal("completion", n, err)
	}
	if value, err := f.Take(); value != 42 || err != nil {
		t.Fatal("owned value", value, err)
	}
	if _, err = f.Take(); !isCode(err, BadHandle) {
		t.Fatal("twice", err)
	}
	f, _ = Defer(c, func() (uint64, error) { calls++; return 99, nil })
	if err = f.Cancel(); err != nil {
		t.Fatal(err)
	}
	c.SpinOnce(1)
	if _, err = f.Take(); !isCode(err, Revoked) || calls != 1 {
		t.Fatal("cancel side effect", err, calls)
	}
}
func TestLocalTimerBudgetTimeAndShutdown(t *testing.T) {
	if _, err := NewContext(0); !isCode(err, Invalid) {
		t.Fatal(err)
	}
	c, _ := NewContext(1)
	calls := 0
	timer, err := c.NewTimer(10, 5, &CallbackGroup{}, func() error { calls++; return nil })
	if err != nil {
		t.Fatal(err)
	}
	if _, err := Defer(c, func() (int, error) { return 1, nil }); !isCode(err, TooLarge) {
		t.Fatal(err)
	}
	c.SpinOnce(9)
	c.SpinOnce(10)
	c.SpinOnce(100)
	if calls != 2 {
		t.Fatal("catch-up replay", calls)
	}
	if _, err = c.SpinOnce(99); !isCode(err, Invalid) || calls != 2 {
		t.Fatal("backwards", err, calls)
	}
	if err = c.Shutdown(); err != nil || timer.IsActive() {
		t.Fatal(err)
	}
	if _, err = c.SpinOnce(101); !isCode(err, BadHandle) {
		t.Fatal(err)
	}
	if err = c.Shutdown(); !isCode(err, BadHandle) {
		t.Fatal(err)
	}
}
func TestLocalGroupAndTimerOverflow(t *testing.T) {
	g := &CallbackGroup{}
	calls := 0
	if err := g.Run(func() error { return g.Run(func() error { calls++; return nil }) }); !isCode(err, Conflict) || calls != 0 {
		t.Fatal(err, calls)
	}
	if err := g.Run(func() error { return nil }); err != nil {
		t.Fatal(err)
	}
	c, _ := NewContext(1)
	timer, _ := c.NewTimer(^uint64(0), 1, g, func() error { t.Fatal("overflow dispatch"); return nil })
	if _, err := c.SpinOnce(^uint64(0)); !isCode(err, Invalid) || timer.IsActive() {
		t.Fatal(err)
	}
	f, _ := Defer(c, func() (int, error) { t.Fatal("shutdown dispatch"); return 0, nil })
	c.Shutdown()
	if _, err := f.Take(); !isCode(err, Revoked) {
		t.Fatal(err)
	}
}
func TestExecutorCheckedClosedRegistration(t *testing.T) {
	e := NewExecutor(8)
	source := &fixtureSource{}
	if err := e.tryAdd(source, func([]byte) {}); err != nil {
		t.Fatal(err)
	}
	if err := e.Close(); err != nil || source.closes != 1 {
		t.Fatal(err, source.closes)
	}
	if _, err := e.SpinOnce(); !isCode(err, BadHandle) || source.polls != 0 {
		t.Fatal(err)
	}
	if err := e.tryAdd(source, func([]byte) {}); !isCode(err, BadHandle) {
		t.Fatal(err)
	}
	if err := e.Close(); !isCode(err, BadHandle) || source.closes != 1 {
		t.Fatal("repeated close", err)
	}
}
