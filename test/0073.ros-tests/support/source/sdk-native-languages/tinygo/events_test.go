package roswasm

import (
	"encoding/binary"
	"testing"
)

// Native source-free codec contracts; these tests neither enable raw imports
// nor establish a ROS/Guest delivery result. Full-width IDs must never narrow.
func unifiedFixture(kind EventKind) []byte {
	length := 64
	if kind == EventServiceRequest || kind == EventActionFeedback || kind == EventActionResult {
		length = 72
	}
	bytes := make([]byte, length)
	binary.LittleEndian.PutUint32(bytes, 0x45525755)
	binary.LittleEndian.PutUint16(bytes[4:], 1)
	binary.LittleEndian.PutUint16(bytes[6:], uint16(kind))
	bytes[8] = 2
	binary.LittleEndian.PutUint64(bytes[16:], 0xf000000000000011)
	binary.LittleEndian.PutUint64(bytes[32:], 1)
	binary.LittleEndian.PutUint64(bytes[40:], 0x20000000000001)
	binary.LittleEndian.PutUint64(bytes[48:], 0xe000000000000021)
	if kind >= EventServiceRequest && kind <= EventActionResult {
		binary.LittleEndian.PutUint64(bytes[24:], 17)
	}
	if length == 72 {
		binary.LittleEndian.PutUint64(bytes[56:], 8)
		binary.LittleEndian.PutUint64(bytes[64:], 17)
	}
	if kind == EventResourceError {
		bytes[9] = 2
		binary.LittleEndian.PutUint32(bytes[12:], 5)
	}
	if kind == EventResourceClosed {
		bytes[9] = 1
	}
	return bytes
}
func TestUnifiedEventKindsAndBorrow(t *testing.T) {
	for kind := EventTopicMessage; kind <= EventResourceClosed; kind++ {
		bytes := unifiedFixture(kind)
		event, err := DecodeUnifiedEvent(bytes)
		if err != nil || event.Kind != kind || event.Backend != 2 || event.Source != 0xf000000000000011 || event.Instance != 0x20000000000001 || event.Session != 0xe000000000000021 {
			t.Fatal("complete kind/full-width metadata")
		}
		if len(event.Body) > 0 {
			bytes[64] = 18
			if event.Body[0] != 18 {
				t.Fatal("documented caller-buffer body borrow")
			}
		}
	}
}
func TestUnifiedEventMalformedRefused(t *testing.T) {
	cases := [][]byte{}
	a := unifiedFixture(EventResourceError)
	a[9] = 4
	cases = append(cases, a)
	a = unifiedFixture(EventServiceRequest)
	a[64] = 18
	cases = append(cases, a)
	for _, at := range []int{4, 6, 10, 56} {
		a = unifiedFixture(EventTopicMessage)
		a[at] = 2
		if at == 6 {
			a[at] = 8
		}
		cases = append(cases, a)
	}
	a = unifiedFixture(EventTopicMessage)
	binary.LittleEndian.PutUint64(a[16:], 0)
	cases = append(cases, a)
	a = unifiedFixture(EventTopicMessage)
	binary.LittleEndian.PutUint64(a[32:], 0)
	cases = append(cases, a)
	for _, bytes := range cases {
		event, err := DecodeUnifiedEvent(bytes)
		if e, ok := err.(Error); !ok || e.Code != Invalid || event.Source != 0 || event.Body != nil {
			t.Fatal("malformed input must publish no event")
		}
	}
}
func TestEventReadinessFullWidth(t *testing.T) {
	bytes := make([]byte, 32)
	for i, value := range []uint64{0x20000000000001, 69, 2, ^uint64(0)} {
		binary.LittleEndian.PutUint64(bytes[8*i:], value)
	}
	value, err := DecodeEventReadiness(bytes)
	if err != nil || value.ReadyEvents != 0x20000000000001 || value.NextEventBytes != 69 || value.AdmissionSlots != 2 || value.AdmissionBytes != ^uint64(0) {
		t.Fatal("full-width readiness")
	}
	if _, err = DecodeEventReadiness(bytes[:31]); err == nil {
		t.Fatal("exact readiness length")
	}
}
