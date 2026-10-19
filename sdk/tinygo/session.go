//go:build tinygo && (wasm || tinygo.wasm) && ((ros1 && !ros2) || (ros2 && !ros1))

package roswasm

import "unsafe"

// Session is a bounded local ownership group over the explicitly selected
// build-tag backend. It never creates a native Context, node, connection or
// grant. Each factory still opens an operator-configured alias. The Wasm
// Instance, not a Session or Node label, remains the Runtime caller identity.
type Session struct {
	closed bool
	maximum int
	states []*capabilityState
}

// NewSession checks wasm32 pointers, ABI version1 and a local1..1024 live-handle
// budget before allocating the registry. This cannot increase native quotas.
func NewSession(maximumResources int) (*Session, error) {
	if maximumResources < 1 || maximumResources > 1024 { return nil, Error{Code: Invalid} }
	if unsafe.Sizeof(uintptr(0)) != 4 || RawAbiVersion() != 1 { return nil, Error{Code: Unsupported} }
	return &Session{maximum: maximumResources}, nil
}

// NewNode creates a local label attached to this group, without opening anything.
// Retained Nodes reject future factories locally after the Session is closed.
func (s *Session) NewNode(label string) *Node { return &Node{label: label, session: s, closed: s == nil} }

// IsOpen is local state only; it makes no native connection or permission claim.
func (s *Session) IsOpen() bool { return s != nil && !s.closed }

func liveStates(states []*capabilityState) []*capabilityState {
	live := states[:0]
	for _, state := range states { if state != nil && state.live { live = append(live, state) } }
	for i := len(live); i < len(states); i++ { states[i] = nil }
	return live
}
func closeStates(states []*capabilityState) []error {
	if err:=ownersIdle(states); err!=nil { return []error{err} }
	results := make([]error, 0, len(states))
	for _, state := range states { if state != nil && state.live { results = append(results, (&Capability{state: state}).Close()) } }
	return results
}

// Close marks the group closed before attempting every live handle once. Inspect
// every returned error; preserve earlier operation errors while still checking
// cleanup. Notification failure can follow native retirement, so no retry is
// issued. Repeated Close returns BadHandle9. There is no finalizer, thread or
// guaranteed cleanup after a Guest trap/panic; use explicit checked shutdown.
// A live server callback returns17 before any owner retires; the group remains
// open and must be explicitly closed after dispatch instead of inside a handler.
func (s *Session) Close() []error {
	if s == nil || s.closed { return []error{Error{Code: BadHandle}} }
	if err:=ownersIdle(s.states); err!=nil { return []error{err} }
	s.closed = true
	results := closeStates(s.states)
	s.states = nil
	return results
}
