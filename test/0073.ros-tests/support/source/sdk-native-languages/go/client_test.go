//go:build wasip1 && wasm

package roswasm

import (
	"bytes"
	"encoding/binary"
	"strings"
	"testing"
)

// Synthetic owners are confined to this test binary. They are not acquired
// Host resources, native ROS witnesses, or a production handle-adoption API.
func synthetic() *Owner {
	o := &Owner{state: &ownerState{}}
	o.state.handle.Store(0xfedcba9876543210)
	return o
}
func refused(code uint32) bool { return code == BadHandle || code == Denied }

func TestActualABIAndDefaultDenial(t *testing.T) {
	if AbiVersion() != 1 {
		t.Fatal("actual backend ABI version")
	}
	for _, alias := range []string{"ungranted-telemetry", "pub"} {
		o, code := Open(alias)
		if o != nil || code != Denied {
			t.Fatalf("real zero-grant open %q: owner=%v errno=%d", alias, o, code)
		}
	}
}

func TestAliasValidation(t *testing.T) {
	for _, s := range []string{"", strings.Repeat("a", 256), "a\x00b", string([]byte{0xff})} {
		if o, c := Open(s); o != nil || c != Invalid {
			t.Fatalf("invalid alias %q: %v/%d", s, o, c)
		}
	}
}

func TestCanonicalStringAndService(t *testing.T) {
	for _, s := range []string{"", "robot", "\ufeff\x00机器人"} {
		b, c := EncodeString(s)
		if c != OK || binary.LittleEndian.Uint32(b) != uint32(len(s)) || !bytes.Equal(b[4:], []byte(s)) {
			t.Fatal("complete String golden")
		}
		back, c := DecodeString(b)
		if c != OK || back != s {
			t.Fatal("String roundtrip")
		}
		for _, flag := range []byte{0, 1} {
			response := append([]byte{flag}, b...)
			got, c := DecodeServiceResponse(response)
			if c != OK || got.Success != (flag == 1) || got.Message != s {
				t.Fatal("complete service response")
			}
		}
		if _, c := DecodeString(append(b, 0)); c != Invalid {
			t.Fatal("trailing String")
		}
	}
	for _, b := range [][]byte{{}, {0}, {1, 0, 0, 0}, {1, 0, 0, 0, 0xff}} {
		if _, c := DecodeString(b); c != Invalid {
			t.Fatalf("malformed String %v", b)
		}
	}
	if b, c := EncodeString(string([]byte{0xff})); c != Invalid || b != nil {
		t.Fatal("malformed UTF-8 encode")
	}
	if _, c := DecodeServiceResponse([]byte{2, 0, 0, 0, 0}); c != Invalid {
		t.Fatal("noncanonical Bool")
	}
	if _, c := EncodeString(strings.Repeat("x", int(MaxRequestBytes)-3)); c != TooLarge {
		t.Fatal("bounded String allocation")
	}
}

func TestFullWidthRequestIDs(t *testing.T) {
	for _, id := range []uint64{1, 1 << 63, ^uint64(0)} {
		var b [8]byte
		binary.LittleEndian.PutUint64(b[:], id)
		got, c := DecodeTriggerRequest(b[:])
		if c != OK || got != id {
			t.Fatal("request ID narrowed")
		}
	}
	for _, b := range [][]byte{nil, make([]byte, 7), make([]byte, 8), make([]byte, 9)} {
		if _, c := DecodeTriggerRequest(b); c != Invalid {
			t.Fatal("invalid request ID")
		}
	}
}

func TestOutputQuarantineAndLengthSentinel(t *testing.T) {
	out := []byte("private")
	var slot [8]byte
	for i := range slot {
		slot[i] = 255
	}
	for _, c := range []uint32{Again, Denied, TooLarge, 110} {
		r, got := finish(c, slot, out)
		if got != c || r.Data != nil || r.HasRequired {
			t.Fatal("failed/unwritten output exposed")
		}
	}
	binary.LittleEndian.PutUint64(slot[:], 1<<63)
	r, c := finish(TooLarge, slot, out)
	if c != TooLarge || r.Data != nil || !r.HasRequired || r.Required != 1<<63 {
		t.Fatal("full-width diagnostic length")
	}
	if r, c := finish(OK, slot, out); c != Invalid || r.Data != nil {
		t.Fatal("oversized success")
	}
	binary.LittleEndian.PutUint64(slot[:], 3)
	r, c = finish(OK, slot, out)
	if c != OK || string(r.Data) != "pri" || cap(r.Data) != 3 {
		t.Fatal("bounded complete prefix")
	}
	binary.LittleEndian.PutUint64(slot[:], 0)
	if r, c := finish(TooLarge, slot, out); c != TooLarge || !r.HasRequired || r.Required != 0 {
		t.Fatal("written zero is known")
	}
}

func TestZeroAndBusyOwners(t *testing.T) {
	for _, o := range []*Owner{nil, {}} {
		for _, c := range []uint32{o.Close(), o.Revoke(), o.Publish(nil), o.Subscribe(), o.AdvertiseService()} {
			if c != BadHandle {
				t.Fatalf("zero owner %d", c)
			}
		}
		if _, c := o.Poll(8); c != BadHandle {
			t.Fatal("zero poll")
		}
		if _, c := o.Request(OpServiceCall, nil, 8); c != BadHandle {
			t.Fatal("zero request")
		}
	}
	o := synthetic()
	o.state.busy.Store(true)
	if o.Close() != Busy || o.Revoke() != Busy || o.Publish(nil) != Busy {
		t.Fatal("nonblocking operation lease")
	}
	if _, c := o.Poll(8); c != Busy {
		t.Fatal("busy poll")
	}
	if _, c := o.Request(OpServiceCall, nil, 8); c != Busy {
		t.Fatal("busy request")
	}
	if o.state.handle.Load() != 0xfedcba9876543210 {
		t.Fatal("busy operation retired owner")
	}
	o.state.busy.Store(false)
}

func TestActualHostRejectsSyntheticOwners(t *testing.T) {
	o := synthetic()
	for _, c := range []uint32{o.PublishString("checked"), o.Subscribe(), o.AdvertiseService(), o.LogString("checked")} {
		if !refused(c) {
			t.Fatalf("synthetic owner reached unexpected result %d", c)
		}
	}
	for _, poll := range []func(uint32) (Result, uint32){o.Poll, o.PollEvent} {
		r, c := poll(64)
		if !refused(c) || r.Data != nil || r.HasRequired {
			t.Fatal("failed real Host poll exposed output")
		}
	}
	if r, c := o.Ready(); !refused(c) || r != (Readiness{}) {
		t.Fatal("failed readiness")
	}
	other := synthetic()
	if c := o.Watch(other, 0); !refused(c) {
		t.Fatalf("watch: %d", c)
	}
	if id, c := o.Submit(other, []byte{1}); !refused(c) || id != 0 {
		t.Fatalf("submit: %d/%d", id, c)
	}
	copyOwner := *o
	if c := o.Close(); !refused(c) {
		t.Fatalf("close failure classification %d", c)
	}
	if copyOwner.Close() != BadHandle || o.state.handle.Load() != 0 {
		t.Fatal("copies did not share retirement")
	}
}

func TestEveryRPCAndBackendAbsence(t *testing.T) {
	o := synthetic()
	supported := 0
	for op := OpServiceCall; op <= lastOperation; op++ {
		r, c := o.Request(op, nil, 16)
		if supports(op) {
			supported++
			if !refused(c) {
				t.Fatalf("supported RPC %d errno%d", op, c)
			}
		} else if c != Unsupported {
			t.Fatalf("backend-absent RPC %d errno%d", op, c)
		}
		if r.Data != nil || r.HasRequired {
			t.Fatal("failed RPC output")
		}
	}
	want := 25
	if Backend == 2 {
		want = 29
	}
	if supported != want {
		t.Fatalf("RPC inventory %d/%d", supported, want)
	}
	for _, op := range []Operation{0, 255} {
		if _, c := o.Request(op, nil, 0); c != Invalid {
			t.Fatal("unknown operation")
		}
	}
	if _, c := o.Poll(MaxOutputBytes + 1); c != TooLarge {
		t.Fatal("bounded receive")
	}
	if _, c := o.Request(OpServiceCall, nil, MaxOutputBytes+1); c != TooLarge {
		t.Fatal("bounded RPC output")
	}
	if Backend == 1 {
		if o.SubscribeParameterEvents() != Unsupported {
			t.Fatal("ROS1 parameter events")
		}
	} else if c := o.SubscribeParameterEvents(); !refused(c) {
		t.Fatal("actual ROS2 parameter event denial")
	}
}

func TestPairLeaseRollback(t *testing.T) {
	a, b := synthetic(), synthetic()
	copyA := *a
	if a.Watch(&copyA, 0) != Invalid {
		t.Fatal("same copied owner pair")
	}
	if _, c := a.Submit(nil, nil); c != BadHandle {
		t.Fatal("missing observer")
	}
	b.state.busy.Store(true)
	if a.Watch(b, 0) != Busy || a.state.busy.Load() {
		t.Fatal("first owner lease not rolled back")
	}
	b.state.busy.Store(false)
}

func TestConcurrentLeaseExclusion(t *testing.T) {
	o := synthetic()
	start := make(chan struct{})
	release := make(chan struct{})
	results := make(chan uint32, 32)
	done := make(chan struct{}, 32)
	for i := 0; i < 32; i++ {
		go func() {
			<-start
			s, _, c := o.acquire()
			results <- c
			if c == OK {
				<-release
				s.busy.Store(false)
			}
			done <- struct{}{}
		}()
	}
	close(start)
	winners := 0
	for i := 0; i < 32; i++ {
		c := <-results
		if c == OK {
			winners++
		} else if c != Busy {
			t.Fatal("lease classification")
		}
	}
	close(release)
	for i := 0; i < 32; i++ {
		<-done
	}
	if winners != 1 || o.state.busy.Load() {
		t.Fatal("exclusive shared lease")
	}
}

func TestSessionReservationAndRetirement(t *testing.T) {
	for _, n := range []uint32{0, 1025} {
		if s, c := NewSession(n); c != Invalid || s != nil {
			t.Fatal("session bound")
		}
	}
	s, c := NewSession(2)
	if c != OK {
		t.Fatal("session")
	}
	copySession := *s
	if o, c := s.Open("ungranted-telemetry"); c != Denied || o != nil || s.state.owners[0] != nil {
		t.Fatal("failed open used slot")
	}
	a, b := synthetic(), synthetic()
	s.state.owners[0] = a.state
	s.state.owners[1] = b.state
	if o, c := s.Open("pub"); c != TooLarge || o != nil {
		t.Fatal("full session")
	}
	b.state.busy.Store(true)
	if r, c := s.Close(); c != Busy || r != nil || s.state.closed || a.state.busy.Load() {
		t.Fatal("partial session cleanup before preflight")
	}
	b.state.busy.Store(false)
	r, c := copySession.Close()
	if c != OK || len(r) != 2 {
		t.Fatal("complete cleanup records")
	}
	for i, v := range r {
		if v.Index != uint32(i) || !v.Attempted || !refused(v.Code) {
			t.Fatalf("cleanup slot %d: %+v", i, v)
		}
	}
	if a.Close() != BadHandle || b.Close() != BadHandle {
		t.Fatal("session owner copies")
	}
	if _, c := s.Open("pub"); c != BadHandle {
		t.Fatal("copied session closed state")
	}
}
