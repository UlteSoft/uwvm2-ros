package roswasm

// CallbackGroup serializes guest-thread callbacks with a shared local guard.
// It imports no ROS resource and is not a native executor callback group.
type CallbackGroup struct { busy bool }

// Run invokes once after checking reentry. No result-slot or queue borrow is
// held across the callback. Deferred reset covers native Go recoverable panic;
// a TinyGo Wasm trap may abort without cleanup and is not converted to a status.
func (g *CallbackGroup) Run(callback func() error) error {
	if g == nil || callback == nil { return Error{Code: Invalid} }
	if g.busy { return Error{Code: Conflict} }
	g.busy = true
	defer func() { g.busy = false }()
	return callback()
}

// Future owns one local deferred result. It never blocks or drives the Runtime.
// Cancel only suppresses a not-started closure; it does not revoke a host alias,
// interrupt a synchronous Client call or undo admitted native side effects.
type Future[T any] struct { stage uint8; value T; err error }

// IsReady reports whether one result can be taken without scheduling more work.
func (f *Future[T]) IsReady() bool { return f != nil && f.stage == 2 }

// Take returns pending11 or consumed9, otherwise transfers the result once.
func (f *Future[T]) Take() (T, error) {
	var zero T
	if f == nil || f.stage == 3 { return zero, Error{Code: BadHandle} }
	if f.stage != 2 { return zero, Error{Code: Again} }
	f.stage = 3
	value, err := f.value, f.err
	f.value = zero; f.err = nil
	return value, err
}

// Cancel makes not-started work complete with local125. Started/consumed work
// returns9 and is never retried. This125 is local cancellation, not host revoke.
func (f *Future[T]) Cancel() error {
	if f == nil || f.stage != 0 { return Error{Code: BadHandle} }
	f.stage = 2; f.err = Error{Code: Revoked}
	return nil
}

type localJob struct { run, cancel func() }
type localTimerEntry struct { timer *Timer; group *CallbackGroup; callback func() error }

// Timer stores a caller-domain deadline and period in exact uint64 nanoseconds.
// No clock is implicitly opened, read, advanced or converted through float64.
type Timer struct { live bool; deadline, period uint64 }

// Cancel disables a registration once without importing a native close.
func (t *Timer) Cancel() error {
	if t == nil || !t.live { return Error{Code: BadHandle} }
	t.live = false; return nil
}

// IsActive reports local registration state, not native connection readiness.
func (t *Timer) IsActive() bool { return t != nil && t.live }

// Context is a bounded local deferred queue/timer scheduler. A native ROS
// Context/Session is not created. Call SpinOnce explicitly on the Guest thread;
// a deferred ROS Client still performs its synchronous call in that later step.
// The application supplies monotonic time, optionally from an explicit Clock
// capability. No goroutine, implicit clock, busy loop or retry is introduced.
type Context struct {
	maximum int
	jobs []localJob
	timers []localTimerEntry
	closed, spinning, hasTime bool
	lastTime uint64
}

// NewContext checks a combined1..1024 queued-job/active-timer budget locally.
func NewContext(maximum int) (*Context, error) {
	if maximum < 1 || maximum > 1024 { return nil, Error{Code: Invalid} }
	return &Context{maximum: maximum}, nil
}
func (c *Context) room() error {
	if c == nil || c.closed { return Error{Code: BadHandle} }
	if c.spinning { return Error{Code: Conflict} }
	live := c.timers[:0]
	for _, entry := range c.timers { if entry.timer.live { live = append(live,entry) } }
	for i:=len(live); i<len(c.timers); i++ { c.timers[i] = localTimerEntry{} }
	c.timers = live
	if len(c.jobs)+len(c.timers) >= c.maximum { return Error{Code: TooLarge} }
	return nil
}

// Defer queues one closure and returns a typed one-shot Future. Go does not
// allow type parameters on methods, so this is a package function with explicit
// Context. Copy borrowed event data before capturing it; no native pointers are
// retained. The callback never runs inline and no permissions are inferred.
func Defer[T any](c *Context, callback func() (T,error)) (*Future[T],error) {
	if callback == nil { return nil, Error{Code: Invalid} }
	if err := c.room(); err != nil { return nil,err }
	future := &Future[T]{}
	c.jobs = append(c.jobs,localJob{
		run: func() { if future.stage == 0 { future.stage = 1; future.value,future.err = callback(); future.stage = 2 } },
		cancel: func() { if future.stage == 0 { _ = future.Cancel() } },
	})
	return future,nil
}

// NewTimer registers an explicit absolute first deadline and positive period.
// An overdue pass emits at most one callback; it never replays missed ticks.
func (c *Context) NewTimer(first, period uint64, group *CallbackGroup, callback func() error) (*Timer,error) {
	if period == 0 || group == nil || callback == nil { return nil,Error{Code: Invalid} }
	if err := c.room(); err != nil { return nil,err }
	timer := &Timer{live:true,deadline:first,period:period}
	c.timers = append(c.timers,localTimerEntry{timer,group,callback})
	return timer,nil
}

// SpinOnce consumes the jobs present at entry and each due timer at most once.
// Backwards time is rejected before side effects; overflow retires that timer
// without dispatch. Nested spin/mutation is17. The first timer error is retained
// while other due callbacks are checked. Returned count includes consumed jobs.
func (c *Context) SpinOnce(now uint64) (uint32,error) {
	if c == nil || c.closed { return 0,Error{Code: BadHandle} }
	if c.spinning { return 0,Error{Code: Conflict} }
	if c.hasTime && now < c.lastTime { return 0,Error{Code: Invalid} }
	c.hasTime = true; c.lastTime = now; c.spinning = true
	defer func() { c.spinning = false }()
	jobs := c.jobs; c.jobs = nil
	var count uint32
	for _,job := range jobs { job.run(); count++ }
	var first error
	for _,entry := range c.timers {
		timer := entry.timer
		if !timer.live || now < timer.deadline { continue }
		if now > ^uint64(0)-timer.period { timer.live = false; if first == nil { first=Error{Code: Invalid} }; continue }
		timer.deadline = now+timer.period
		if err:=entry.group.Run(entry.callback); err!=nil && first==nil { first=err }
		count++
	}
	return count,first
}

// Shutdown cancels queued local results and timers. Close Session separately to
// retire native capabilities. Started synchronous RPC cannot be interrupted by
// this local state. Repeated/closed operations return9; active mutation is17.
func (c *Context) Shutdown() error {
	if c == nil || c.closed { return Error{Code: BadHandle} }
	if c.spinning { return Error{Code: Conflict} }
	c.closed = true
	for _,job := range c.jobs { job.cancel() }; c.jobs=nil
	for _,entry:=range c.timers { entry.timer.live=false }; c.timers=nil
	return nil
}
