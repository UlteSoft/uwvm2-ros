package roswasm

import (
	"errors"
	"testing"
)

func TestConstructionPrimaryAndRollback(t *testing.T) {
	primary := Error{Code: Denied}
	if got := constructionFailure(primary, nil, nil); got != primary {
		t.Fatal("successful rollback changed primary", got)
	}
	var first, second capabilityState
	first.handle, first.live = 0xfedcba9876543210, true
	second.handle, second.live = 7, true
	var ids []uint64
	closeFirst := func() error {
		return retireOwner(&first, func(id uint64) error {
			ids = append(ids, id)
			return Error{Code: Timeout}
		})
	}
	closeSecond := func() error {
		return retireOwner(&second, func(id uint64) error {
			ids = append(ids, id)
			return Error{Code: Disconnected}
		})
	}
	got := constructionFailure(primary, closeFirst(), nil, closeSecond())
	joined, ok := got.(*ConstructionError)
	if !ok || len(joined.Cleanup) != 2 || joined.Primary != primary ||
		joined.Cleanup[0].(Error).Code != Timeout || joined.Cleanup[1].(Error).Code != Disconnected {
		t.Fatal("lost primary or ordered cleanup failures", got)
	}
	if joined.Unwrap() != primary || ErrorCode(got) != Denied {
		t.Fatal("authorization error no longer discoverable", got)
	}
	if first.live || second.live || len(ids) != 2 || ids[0] != first.handle || ids[1] != second.handle {
		t.Fatal("retirement order or full-width ID changed", ids)
	}
	if err := closeFirst(); err.(Error).Code != BadHandle || len(ids) != 2 {
		t.Fatal("rollback notification replayed", err, ids)
	}
	other := errors.New("caller primary")
	if constructionFailure(other) != other {
		t.Fatal("arbitrary primary identity changed")
	}
}

func TestErrorCodeWithoutReflection(t *testing.T) {
	value := Error{Code: Timeout}
	var pointer *Error
	var construction *ConstructionError
	cycle := &ConstructionError{}
	cycle.Primary = cycle
	foreign := errors.New("foreign error")
	rows := []struct {
		err  error
		want uint32
	}{
		{nil, Success}, {value, Timeout}, {&value, Timeout},
		{pointer, Invalid}, {construction, Invalid}, {foreign, Invalid},
		{&ConstructionError{Primary: Error{Code: Denied}, Cleanup: []error{value}}, Denied},
		{cycle, Invalid},
	}
	for index, row := range rows {
		if got := ErrorCode(row.err); got != row.want {
			t.Fatalf("row %d: got %d want %d", index, got, row.want)
		}
	}
	var deep error = Error{Code: Denied}
	for index := 0; index < 32; index++ {
		deep = &ConstructionError{Primary: deep}
	}
	if ErrorCode(deep) != Invalid {
		t.Fatal("deep primary chain was not bounded")
	}
}
