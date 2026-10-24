// Native tests of the actual shared retirement core. Fake notifications are
// local counters, not native ROS/Guest authority or interoperability evidence.
package roswasm

import "testing"

func TestSharedOwnerBusyRefusesCloseRevokeWithoutRetirement(t *testing.T) {
	guard := &callbackState{busy: true}
	state := &capabilityState{handle: 7, live: true, callback: guard}
	calls := 0
	notify := func(handle uint64) error {
		calls++
		if handle != 7 {
			t.Fatal("identity changed")
		}
		return Error{Code: Timeout}
	}
	if err := retireOwner(state, notify); !isCode(err, Conflict) || !state.live || calls != 0 {
		t.Fatal("busy close/revoke mutated owner", err)
	}
	guard.busy = false
	if err := retireOwner(state, notify); !isCode(err, Timeout) || state.live || calls != 1 {
		t.Fatal("retire-before-failed-notification", err)
	}
	if err := retireOwner(state, notify); !isCode(err, BadHandle) || calls != 1 {
		t.Fatal("replayed retirement", err)
	}
}

func TestGroupedCleanupBusyPreflightPreservesEarlierIdleOwners(t *testing.T) {
	guard := &callbackState{busy: true}
	first := &capabilityState{handle: 3, live: true}
	second := &capabilityState{handle: 7, live: true, callback: guard}
	retired := &capabilityState{handle: 9, live: false, callback: guard}
	owners := []*capabilityState{first, second, retired}
	calls := 0
	cleanup := func() error {
		if err := ownersIdle(owners); err != nil {
			return err
		}
		for _, state := range owners {
			if state.live {
				if err := retireOwner(state, func(uint64) error { calls++; return nil }); err != nil {
					return err
				}
			}
		}
		return nil
	}
	if err := cleanup(); !isCode(err, Conflict) || !first.live || !second.live || calls != 0 {
		t.Fatal("partial grouped retirement", err)
	}
	guard.busy = false
	if err := cleanup(); err != nil || first.live || second.live || calls != 2 {
		t.Fatal("post-callback cleanup", err)
	}
	if err := cleanup(); err != nil || calls != 2 {
		t.Fatal("retired group replay", err)
	}
}
