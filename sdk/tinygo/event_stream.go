//go:build tinygo && (wasm || tinygo.wasm) && ((ros1 && !ros2) || (ros2 && !ros1))

package roswasm

// EventStream borrows explicit source/observer owners and performs one watch.
// Neither source activation through a legacy queue nor alias opening is implicit.
// Owners must remain alive; Capability copies share the same retirement state.
// Retain/register the returned Stream pointer; do not copy a live Stream value,
// because each watch has exactly one contiguous sequence/session audit state.
// The observer's shared callback lease prevents direct/Node/Session retirement
// during dispatch. Source lifetime stays independent, including closed metadata.
type EventStream struct {
	observer, source *Capability
	binding          EventBinding
}

// NewEventStream binds already-open independent grants. Failure leaves both
// owners with the caller for checked cleanup and creates no default authority.
func NewEventStream(observer, source *Capability, correlation uint64) (*EventStream, error) {
	if observer == nil || source == nil {
		return nil, Error{Code: BadHandle}
	}
	o, err := observer.RawHandle()
	if err != nil {
		return nil, err
	}
	s, err := source.RawHandle()
	if err != nil {
		return nil, err
	}
	if o == s {
		return nil, Error{Code: Invalid}
	}
	binding, err := NewEventBinding(eventBackend, s, correlation)
	if err != nil {
		return nil, err
	}
	if err = observer.WatchEvents(source, correlation); err != nil {
		return nil, err
	}
	if observer.state.callback == nil {
		observer.state.callback = &callbackState{}
	}
	return &EventStream{observer: observer, source: source, binding: binding}, nil
}

// Poll copies and validates one bound event. TooLarge retains the native record;
// there is no resize/retry. Body borrows output until that buffer is overwritten.
func (s *EventStream) Poll(output []byte) (UnifiedEvent, error) {
	if s == nil {
		return UnifiedEvent{}, Error{Code: BadHandle}
	}
	n, err := s.observer.PollEventV1(output)
	if err != nil {
		return UnifiedEvent{}, err
	}
	return s.binding.Decode(output[:n])
}

// SpinOnce invokes one Guest-thread callback, or returns false on idle11.
// Binding commits before user code, so a handler failure never replays an event.
// The observer lease spans the handler and unwinds on normal Go panic cleanup;
// a fatal Wasm trap cannot promise cleanup. Handler bytes are borrowed, not native.
func (s *EventStream) SpinOnce(output []byte, handler func(UnifiedEvent) error) (bool, error) {
	if s == nil || s.observerState() == nil {
		return false, Error{Code: BadHandle}
	}
	if handler == nil {
		return false, Error{Code: Invalid}
	}
	return dispatchUnified(s.observer.state.callback, &s.binding, output, s.observer.PollEventV1, handler)
}

// Readiness is finite current credit only, never a reservation for Submit.
func (s *EventStream) Readiness() (EventReadiness, error) {
	if s == nil {
		return EventReadiness{}, Error{Code: BadHandle}
	}
	return s.observer.EventReadiness()
}

// Submit performs actual native asynchronous service admission once. The returned
// ID is not completion; match a later service-response/resource-error event.
func (s *EventStream) Submit(input []byte) (uint64, error) {
	if s == nil {
		return 0, Error{Code: BadHandle}
	}
	return s.source.SubmitService(s.observer, input)
}

// Close retires only the observer. The caller still owns the independent source.
func (s *EventStream) Close() error {
	if s == nil {
		return Error{Code: BadHandle}
	}
	return s.observer.Close()
}

// Revoke retires only the exact observer alias, without cancel-any authority.
func (s *EventStream) Revoke() error {
	if s == nil {
		return Error{Code: BadHandle}
	}
	return s.observer.Revoke()
}

// observerState is private identity/preflight, never a public fake ROS backend.
func (s *EventStream) observerState() *capabilityState {
	if s == nil || s.observer == nil {
		return nil
	}
	return s.observer.state
}

// Add registers only actual EventStream resources through the public Wasm API.
// Native policy fixtures exercise the same private executor, without raw imports.
func (x *EventExecutor) Add(stream *EventStream, handler func(UnifiedEvent) error) error {
	if stream == nil || stream.observer == nil || stream.source == nil {
		return Error{Code: Invalid}
	}
	return x.addStream(stream, handler)
}

// Remove returns the borrowed actual stream without closing either owner.
func (x *EventExecutor) Remove(index int) (*EventStream, error) {
	if x == nil || x.closed {
		return nil, Error{Code: BadHandle}
	}
	if x.spinning {
		return nil, Error{Code: Conflict}
	}
	if index < 0 || index >= len(x.entries) {
		return nil, Error{Code: Invalid}
	}
	stream, ok := x.entries[index].stream.(*EventStream)
	if !ok {
		return nil, Error{Code: Invalid}
	}
	_, err := x.removeStream(index)
	return stream, err
}
