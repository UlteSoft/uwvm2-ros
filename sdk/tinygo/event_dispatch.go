package roswasm

// Shared actual dispatcher. Native tests supply a finite copied event source,
// while EventStream supplies the real event_poll_v1 import. No public mock
// backend exists. Lease/binding checks precede callback and lease resets on
// normal return/panic cleanup. A fatal Wasm trap has no cleanup guarantee.
func dispatchUnified(state *callbackState, binding *EventBinding, output []byte, poll func([]byte) (uint64, error), handler func(UnifiedEvent) error) (bool, error) {
	if err := callbackIdle(state); err != nil {
		return false, err
	}
	if binding == nil || poll == nil || handler == nil {
		return false, Error{Code: Invalid}
	}
	n, err := poll(output)
	if err != nil {
		if e, ok := err.(Error); ok && e.Code == Again {
			return false, nil
		}
		return false, err
	}
	if n > uint64(len(output)) {
		return false, Error{Code: Invalid}
	}
	event, err := binding.Decode(output[:n])
	if err != nil {
		return false, err
	}
	state.busy = true
	defer func() { state.busy = false }()
	if err = handler(event); err != nil {
		return false, err
	}
	return true, nil
}

// Private protocol shared with native policy fixtures. Public Wasm registration
// accepts only EventStream; no alternate ROS backend or capability is exposed.
type eventStreamSource interface {
	SpinOnce([]byte, func(UnifiedEvent) error) (bool, error)
	Close() error
	observerState() *capabilityState
}
type eventExecutorEntry struct {
	stream  eventStreamSource
	handler func(UnifiedEvent) error
}

// EventExecutor is an additive registering executor for unified streams. The
// legacy Executor keeps its original destructive-too-small queue contract.
// Calls are serial and explicit; no goroutine, clock, sleep or native callback
// is started. Streams/owners remain borrowed and must outlive registrations.
type EventExecutor struct {
	entries          []eventExecutorEntry
	scratch          []byte
	spinning, closed bool
}

// NewEventExecutor validates64..4194304 bytes before allocating fixed scratch.
func NewEventExecutor(capacity uint32) (*EventExecutor, error) {
	if capacity < 64 || capacity > 4194304 {
		return nil, Error{Code: Invalid}
	}
	return &EventExecutor{scratch: make([]byte, capacity)}, nil
}

// Add registers one stream up to1024 entries. Busy/duplicate registrations fail
// before mutation and leave all capability ownership with the application.
func (x *EventExecutor) addStream(stream eventStreamSource, handler func(UnifiedEvent) error) error {
	if x == nil || x.closed {
		return Error{Code: BadHandle}
	}
	if x.spinning {
		return Error{Code: Conflict}
	}
	if stream == nil || stream.observerState() == nil || handler == nil {
		return Error{Code: Invalid}
	}
	if len(x.entries) >= 1024 {
		return Error{Code: TooLarge}
	}
	for _, e := range x.entries {
		if e.stream.observerState() == stream.observerState() {
			return Error{Code: Conflict}
		}
	}
	x.entries = append(x.entries, eventExecutorEntry{stream, handler})
	return nil
}

// Remove unregisters without closing either owner. Mutation while spinning17
// changes no entries; perform deferred lifecycle changes after the pass returns.
func (x *EventExecutor) removeStream(index int) (eventStreamSource, error) {
	if x == nil || x.closed {
		return nil, Error{Code: BadHandle}
	}
	if x.spinning {
		return nil, Error{Code: Conflict}
	}
	if index < 0 || index >= len(x.entries) {
		return nil, Error{Code: Invalid}
	}
	s := x.entries[index].stream
	copy(x.entries[index:], x.entries[index+1:])
	x.entries[len(x.entries)-1] = eventExecutorEntry{}
	x.entries = x.entries[:len(x.entries)-1]
	return s, nil
}

// SpinOnce polls each registration at most once. Errors preserve the number of
// callbacks completed earlier in the pass; consumed records are never replayed.
func (x *EventExecutor) SpinOnce() (uint32, error) {
	if x == nil || x.closed {
		return 0, Error{Code: BadHandle}
	}
	if x.spinning {
		return 0, Error{Code: Conflict}
	}
	x.spinning = true
	defer func() { x.spinning = false }()
	var count uint32
	for _, e := range x.entries {
		done, err := e.stream.SpinOnce(x.scratch, e.handler)
		if err != nil {
			return count, err
		}
		if done {
			count++
		}
	}
	return count, nil
}

// Close preflights all observer leases, attempts each live observer once and
// reports the first error. Sources remain independently owned by the caller.
func (x *EventExecutor) Close() error {
	if x == nil || x.closed {
		return Error{Code: BadHandle}
	}
	if x.spinning {
		return Error{Code: Conflict}
	}
	states := make([]*capabilityState, 0, len(x.entries))
	for _, e := range x.entries {
		states = append(states, e.stream.observerState())
	}
	if err := ownersIdle(states); err != nil {
		return err
	}
	x.closed = true
	var first error
	for _, e := range x.entries {
		if state := e.stream.observerState(); state != nil && state.live {
			if err := e.stream.Close(); err != nil && first == nil {
				first = err
			}
		}
	}
	x.entries = nil
	return first
}
