package roswasm

// eventSource is a private polling/close contract. The public Wasm API accepts
// Subscription only. Codec tests can exercise this exact executor using a pure
// source fixture without importing, instantiating or executing a ROS function.
type eventSource interface {
	Poll([]byte) (uint64, error)
	Close() error
}
type executorEntry struct {
	source   eventSource
	callback func([]byte)
}

// Executor polls each initially registered queue once per SpinOnce, using one
// explicitly bounded scratch buffer. Callbacks run on the guest thread. Nested
// spins and Close during a spin return Invalid22 before changing scratch/state;
// callback-time registrations are ignored and must be submitted after the pass.
// No async native callback, background goroutine or automatic error90 retry exists.
type Executor struct {
	entries  []executorEntry
	scratch  []byte
	spinning bool
	closed bool
}

// NewExecutor allocates one application-chosen uint32 scratch capacity. This
// cannot increase any host message/queue/rate quota or open a resource.
func NewExecutor(capacity uint32) *Executor { return &Executor{scratch: make([]byte, capacity)} }

func (x *Executor) add(source eventSource, callback func([]byte)) {
	_ = x.tryAdd(source, callback)
}

func (x *Executor) tryAdd(source eventSource, callback func([]byte)) error {
	if x == nil || x.closed { return Error{Code: BadHandle} }
	if x.spinning || source == nil || callback == nil { return Error{Code: Invalid} }
	if len(x.entries) >= 1024 { return Error{Code: TooLarge} }
	x.entries = append(x.entries, executorEntry{source, callback})
	return nil
}

// NewCheckedExecutor validates the application scratch bound before allocation.
// The canonical wire bound cannot increase any host resource quota.
func NewCheckedExecutor(capacity uint32) (*Executor, error) {
	if uint64(capacity) > MaxCanonicalBytes { return nil, Error{Code: TooLarge} }
	return NewExecutor(capacity), nil
}

// SpinOnce polls each registered queue at most once. Again is idle; any other
// error stops the pass and preserves the number already delivered. Reentry is
// rejected before modifying scratch. The deferred guard resets on normal
// return and native Go recoverable panic. A TinyGo/WASI panic can terminate the
// module without deferred cleanup; no panic recovery or handle-close guarantee
// is made for that target. Callback panic is an application failure, not ROS status.
func (x *Executor) SpinOnce() (uint32, error) {
	if x == nil || x.closed { return 0, Error{Code: BadHandle} }
	if x.spinning {
		return 0, Error{Code: Invalid}
	}
	x.spinning = true
	defer func() { x.spinning = false }()
	var count uint32
	// A Go range evaluates this slice/length once; additions cannot extend this
	// pass. The explicit busy rule additionally rejects all such registrations.
	for _, entry := range x.entries {
		if entry.source == nil || entry.callback == nil {
			return count, Error{Code: Invalid}
		}
		n, err := entry.source.Poll(x.scratch)
		if err != nil {
			if e, ok := err.(Error); ok && e.Code == Again {
				continue
			}
			return count, err
		}
		if n > uint64(len(x.scratch)) {
			return count, Error{Code: Invalid}
		}
		entry.callback(x.scratch[:n])
		count++
	}
	return count, nil
}

// Close attempts every owned subscription close, preserving the first error.
// Closing during SpinOnce is refused with Invalid22 and changes no registrations
// or handle state; call Close only after the current pass returns.
func (x *Executor) Close() error {
	if x == nil || x.closed { return Error{Code: BadHandle} }
	if x.spinning {
		return Error{Code: Invalid}
	}
	x.closed = true
	var first error
	for _, entry := range x.entries {
		if entry.source == nil {
			if first == nil {
				first = Error{Code: Invalid}
			}
			continue
		}
		if err := entry.source.Close(); err != nil && first == nil {
			first = err
		}
	}
	x.entries = nil
	return first
}
