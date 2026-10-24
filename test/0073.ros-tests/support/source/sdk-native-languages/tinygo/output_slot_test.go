// Output slot tests exercise the common wrapper result interpretation without
// invoking a native ROS import. Executing them is a separate compilation verdict.
package roswasm

import (
	"encoding/binary"
	"testing"
)

func TestOutputSlotContract(t *testing.T) {
	slot := func(n uint64) [8]byte { var b [8]byte; binary.LittleEndian.PutUint64(b[:], n); return b }
	cases := []struct {
		code     uint32
		n        uint64
		capacity uint64
		want     uint32
		observed bool
	}{
		{TooLarge, ^uint64(0), 16, TooLarge, false},
		{TooLarge, 32, 16, TooLarge, true},
		{Denied, 32, 16, Denied, false},
		{Success, 16, 16, Success, false},
		{Success, 17, 16, Invalid, false},
		{Success, ^uint64(0), 16, Invalid, false},
		{^uint32(0), 0, 16, ^uint32(0), false},
	}
	for _, tc := range cases {
		n, err := finish(tc.code, slot(tc.n), tc.capacity)
		if tc.want == Success {
			if err != nil || n != tc.n {
				t.Fatal("success length", err)
			}
			continue
		}
		e, ok := err.(Error)
		if !ok || e.Code != tc.want || e.HasRequired != tc.observed {
			t.Fatalf("slot contract %+v got %#v", tc, err)
		}
		if e.HasRequired && e.Required != tc.n {
			t.Fatal("required length precision")
		}
	}
}
