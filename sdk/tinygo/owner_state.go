package roswasm

// Shared local ownership state used by actual Wasm capabilities and native
// source-independent retirement tests. No import, permission or finalizer is
// introduced. A server's two role states point to its same callbackState.
type capabilityState struct {
	handle uint64
	live bool
	callback *callbackState
}

// Preflight every live owner before grouped cleanup mutates even its first
// state. Busy17 leaves Node/Session factories and all owners available for
// checked cleanup after dispatch. Retired entries need no callback lease.
func ownersIdle(states []*capabilityState) error {
	for _,state:=range states {
		if state!=nil && state.live && state.callback!=nil {
			if err:=callbackIdle(state.callback); err!=nil { return err }
		}
	}
	return nil
}

// Retire exactly once before notifying the backend, but never while a shared
// callback lease is busy. Notification failure cannot restore local ownership.
// The test-owned notify function exercises this exact production state machine.
func retireOwner(state *capabilityState,notify func(uint64)error)error {
	if state==nil || !state.live { return Error{Code:BadHandle} }
	if state.callback!=nil { if err:=callbackIdle(state.callback); err!=nil { return err } }
	if notify==nil { return Error{Code:Invalid} }
	handle:=state.handle; state.live=false; return notify(handle)
}
