// Golden codec vectors are independent expected bytes from the common ABI fixtures.
// Generating this test is not a compilation/runtime verdict.
package roswasm

import (
	"encoding/hex"
	"math"
	"testing"
)

func fixture(s string) []byte {
	b, err := hex.DecodeString(s)
	if err != nil {
		panic(err)
	}
	return b
}
func assertBytes(t *testing.T, b, want []byte, err error) {
	t.Helper()
	if err != nil {
		t.Fatal(err)
	}
	if hex.EncodeToString(b) != hex.EncodeToString(want) {
		t.Fatalf("got %x want %x", b, want)
	}
}
func TestSharedPositiveVectors(t *testing.T) {
	t.Run("string-empty", func(t *testing.T) {
		want := fixture("00000000")
		b, err := String("").Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeString(want)
		if err != nil || decoded != "" {
			t.Fatal("string decode", err)
		}
	})
	t.Run("string-ascii", func(t *testing.T) {
		want := fixture("0500000068656c6c6f")
		b, err := String("hello").Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeString(want)
		if err != nil || decoded != "hello" {
			t.Fatal("string decode", err)
		}
	})
	t.Run("string-utf8", func(t *testing.T) {
		want := fixture("0a000000726f626f7420f09f8c8d")
		b, err := String("robot 🌍").Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeString(want)
		if err != nil || decoded != "robot 🌍" {
			t.Fatal("string decode", err)
		}
	})
	t.Run("bool-false", func(t *testing.T) {
		want := fixture("00")
		b := make([]byte, 1)
		_, err := EncodeBool(false, b)
		assertBytes(t, b, want, err)
		decoded, err := DecodeBool(want)
		if err != nil || decoded != false {
			t.Fatal("bool decode", err)
		}
	})
	t.Run("bool-true", func(t *testing.T) {
		want := fixture("01")
		b := make([]byte, 1)
		_, err := EncodeBool(true, b)
		assertBytes(t, b, want, err)
		decoded, err := DecodeBool(want)
		if err != nil || decoded != true {
			t.Fatal("bool decode", err)
		}
	})
	t.Run("int32--2147483648", func(t *testing.T) {
		want := fixture("00000080")
		v := ParameterValue{Kind: ParameterInt32, Int32: -2147483648}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeParameter(ParameterInt32, want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("int32--1", func(t *testing.T) {
		want := fixture("ffffffff")
		v := ParameterValue{Kind: ParameterInt32, Int32: -1}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeParameter(ParameterInt32, want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("int32-2147483647", func(t *testing.T) {
		want := fixture("ffffff7f")
		v := ParameterValue{Kind: ParameterInt32, Int32: 2147483647}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeParameter(ParameterInt32, want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("int64--9223372036854775808", func(t *testing.T) {
		want := fixture("0000000000000080")
		v := ParameterValue{Kind: ParameterInt64, Int64: -9223372036854775808}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeParameter(ParameterInt64, want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("int64--1", func(t *testing.T) {
		want := fixture("ffffffffffffffff")
		v := ParameterValue{Kind: ParameterInt64, Int64: -1}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeParameter(ParameterInt64, want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("int64-9223372036854775807", func(t *testing.T) {
		want := fixture("ffffffffffffff7f")
		v := ParameterValue{Kind: ParameterInt64, Int64: 9223372036854775807}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeParameter(ParameterInt64, want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("float64-zero", func(t *testing.T) {
		want := fixture("0000000000000000")
		v := ParameterValue{Kind: ParameterFloat64, Float64: 0.0}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeParameter(ParameterFloat64, want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("float64-negative-zero", func(t *testing.T) {
		want := fixture("0000000000000080")
		v := ParameterValue{Kind: ParameterFloat64, Float64: math.Copysign(0, -1)}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeParameter(ParameterFloat64, want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("float64-fraction", func(t *testing.T) {
		want := fixture("000000000000f43f")
		v := ParameterValue{Kind: ParameterFloat64, Float64: 1.25}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeParameter(ParameterFloat64, want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("twist", func(t *testing.T) {
		want := fixture("000000000000f03f00000000000000c0000000000000084000000000000000000000000000000080000000000000e03f")
		v := Twist{1.0, -2.0, 3.0, 0.0, math.Copysign(0, -1), 0.5}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTwist(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = Twist(decoded).Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("array-bool", func(t *testing.T) {
		want := fixture("03000000000100")
		v := ParameterValue{Kind: ParameterBoolArray, Bools: []bool{false, true, false}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeParameter(ParameterBoolArray, want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("array-byte", func(t *testing.T) {
		want := fixture("03000000007fff")
		v := ParameterValue{Kind: ParameterByteArray, Bytes: []byte{0, 127, 255}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeParameter(ParameterByteArray, want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("array-int32", func(t *testing.T) {
		want := fixture("02000000ffffffffffffff7f")
		v := ParameterValue{Kind: ParameterInt32Array, Int32s: []int32{-1, 2147483647}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeParameter(ParameterInt32Array, want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("array-int64", func(t *testing.T) {
		want := fixture("020000000000000000000080ffffffffffffff7f")
		v := ParameterValue{Kind: ParameterInt64Array, Int64s: []int64{-9223372036854775808, 9223372036854775807}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeParameter(ParameterInt64Array, want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("array-float64", func(t *testing.T) {
		want := fixture("02000000000000000000f43f0000000000000080")
		v := ParameterValue{Kind: ParameterFloat64Array, Float64s: []float64{1.25, math.Copysign(0, -1)}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeParameter(ParameterFloat64Array, want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("array-string", func(t *testing.T) {
		want := fixture("03000000000000000500000068656c6c6f02000000cebb")
		v := ParameterValue{Kind: ParameterStringArray, Strings: []string{"", "hello", "λ"}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeParameter(ParameterStringArray, want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("array-empty", func(t *testing.T) {
		want := fixture("00000000")
		v := ParameterValue{Kind: ParameterStringArray, Strings: []string{}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeParameter(ParameterStringArray, want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("image-rgb8", func(t *testing.T) {
		want := fixture("0000000001000000020000000600000063616d65726101000000010000000400000072676238000300000003000000ff0007")
		v := Image{Header: Header{Sequence: 0, Seconds: 1, Nanoseconds: 2, Frame: "camera"}, Height: 1, Width: 1, Encoding: "rgb8", BigEndian: false, Step: 3, Data: []byte{255, 0, 7}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeImage(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("transform", func(t *testing.T) {
		want := fixture("000000000100000002000000030000006d61700400000062617365000000000000f03f00000000000000400000000000000840000000000000000000000000000000000000000000000000000000000000f03f")
		v := Transform{Header: Header{Sequence: 0, Seconds: 1, Nanoseconds: 2, Frame: "map"}, ChildFrame: "base", Values: [7]float64{1.0, 2.0, 3.0, 0.0, 0.0, 0.0, 1.0}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTransform(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("trigger-request", func(t *testing.T) {
		want := fixture("")
		assertBytes(t, EncodeTriggerRequest(), want, nil)
		if err := DecodeTriggerRequest(want); err != nil {
			t.Fatal(err)
		}
	})
	t.Run("set-bool-request", func(t *testing.T) {
		want := fixture("01")
		b := make([]byte, 1)
		_, err := EncodeBool(true, b)
		assertBytes(t, b, want, err)
		decoded, err := DecodeBool(want)
		if err != nil || decoded != true {
			t.Fatal("bool decode", err)
		}
	})
	t.Run("service-response", func(t *testing.T) {
		want := fixture("01020000006f6b")
		v := ServiceResponse{Success: true, Message: "ok"}
		b := make([]byte, 64)
		n, err := EncodeServiceResponse(v, b)
		assertBytes(t, b[:n], want, err)
		decoded, err := DecodeServiceResponse(want)
		if err != nil || decoded != v {
			t.Fatal("response decode", err)
		}
	})
	t.Run("service-request-event", func(t *testing.T) {
		want := fixture("050000000000008001")
		id, v, err := DecodeSetBoolEvent(want)
		if err != nil || id.Raw() != 9223372036854775813 || !v {
			t.Fatal("event decode", err)
		}
	})
	t.Run("service-response-envelope", func(t *testing.T) {
		want := fixture("050000000000008001020000006f6b")
		id, err := DecodeTriggerEvent(want[:8])
		if err != nil {
			t.Fatal(err)
		}
		response, err := DecodeServiceResponse(want[8:])
		if err != nil {
			t.Fatal(err)
		}
		b := make([]byte, 64)
		n, err := EncodeServiceReply(id, response, b)
		assertBytes(t, b[:n], want, err)
		if id.Raw() != 9223372036854775813 {
			t.Fatal("ID precision")
		}
	})
	t.Run("fibonacci-order", func(t *testing.T) {
		want := fixture("05000000")
		b := make([]byte, 4)
		_, err := EncodeFibonacciGoal(5, b)
		assertBytes(t, b, want, err)
	})
	t.Run("goal-id", func(t *testing.T) {
		want := fixture("0500000000000080")
		id, err := DecodeGoalID(want)
		if err != nil || id.Raw() != 9223372036854775813 {
			t.Fatal("ID precision", err)
		}
		b := id.Bytes()
		assertBytes(t, b[:], want, nil)
	})
	t.Run("incoming-goal", func(t *testing.T) {
		want := fixture("050000000000008005000000")
		id, order, err := DecodeFibonacciGoalEvent(want)
		if err != nil || id.Raw() != 9223372036854775813 || order != 5 {
			t.Fatal("incoming goal", err)
		}
	})
	t.Run("feedback", func(t *testing.T) {
		want := fixture("050000000000008006000000000000000100000001000000020000000300000005000000")
		v, err := DecodeFibonacciFeedback(want)
		if err != nil || v.Goal.Raw() != 9223372036854775813 {
			t.Fatal("action decode", err)
		}
		b := make([]byte, 256)
		n, err := EncodeFibonacciFeedback(v.Goal, v.Values, b)
		assertBytes(t, b[:n], want, err)
	})
	t.Run("success", func(t *testing.T) {
		want := fixture("05000000000000800206000000000000000100000001000000020000000300000005000000")
		v, err := DecodeFibonacciResult(want)
		if err != nil || v.Goal.Raw() != 9223372036854775813 {
			t.Fatal("action decode", err)
		}
		b := make([]byte, 256)
		n, err := EncodeFibonacciReply(v.Goal, v.Kind, v.Values, b)
		assertBytes(t, b[:n], want, err)
	})
	t.Run("server-feedback", func(t *testing.T) {
		want := fixture("05000000000000800106000000000000000100000001000000020000000300000005000000")
		v, err := DecodeFibonacciReply(want)
		if err != nil || v.Goal.Raw() != 9223372036854775813 {
			t.Fatal("action decode", err)
		}
		b := make([]byte, 256)
		n, err := EncodeFibonacciReply(v.Goal, v.Kind, v.Values, b)
		assertBytes(t, b[:n], want, err)
	})
	t.Run("lifecycle-state", func(t *testing.T) {
		want := fixture("0306000000616374697665")
		v := LifecycleState{ID: 3, Label: "active"}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeLifecycleState(want)
		if err != nil || decoded != v {
			t.Fatal("lifecycle decode", err)
		}
	})
	t.Run("time-nanoseconds", func(t *testing.T) {
		want := fixture("0500000000000080")
		n, err := DecodeTime(want)
		if err != nil || n != 9223372036854775813 {
			t.Fatal("time precision", err)
		}
	})
	t.Run("legacy-parameter-event", func(t *testing.T) {
		want := fixture("010000000200000002050000002f64656d6f0500000076616c75650400000074727565")
		v := LegacyParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 2, Node: "/demo", Name: "value", Value: "true"}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeLegacyParameterEvent(want)
		if err != nil || decoded != v {
			t.Fatal("legacy decode", err)
		}
	})
	t.Run("typed-event-bool", func(t *testing.T) {
		want := fixture("0100000002000000020201050000002f64656d6f0500000076616c75650100000001")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 2, Tag: EventBool, Node: "/demo", Name: "value", Value: &ParameterValue{Kind: ParameterBool, Bool: true}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-event-int64", func(t *testing.T) {
		want := fixture("0100000002000000020202050000002f64656d6f0500000076616c756508000000ffffffffffffff7f")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 2, Tag: EventInt64, Node: "/demo", Name: "value", Value: &ParameterValue{Kind: ParameterInt64, Int64: 9223372036854775807}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-event-float64", func(t *testing.T) {
		want := fixture("0100000002000000020203050000002f64656d6f0500000076616c756508000000000000000000f43f")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 2, Tag: EventFloat64, Node: "/demo", Name: "value", Value: &ParameterValue{Kind: ParameterFloat64, Float64: 1.25}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-event-string", func(t *testing.T) {
		want := fixture("0100000002000000020204050000002f64656d6f0500000076616c7565090000000500000068656c6c6f")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 2, Tag: EventString, Node: "/demo", Name: "value", Value: &ParameterValue{Kind: ParameterString, Text: "hello"}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-event-byte-array", func(t *testing.T) {
		want := fixture("0100000002000000020205050000002f64656d6f0500000076616c7565060000000200000000ff")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 2, Tag: EventByteArray, Node: "/demo", Name: "value", Value: &ParameterValue{Kind: ParameterByteArray, Bytes: []byte{0, 255}}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-event-bool-array", func(t *testing.T) {
		want := fixture("0100000002000000020206050000002f64656d6f0500000076616c756506000000020000000001")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 2, Tag: EventBoolArray, Node: "/demo", Name: "value", Value: &ParameterValue{Kind: ParameterBoolArray, Bools: []bool{false, true}}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-event-int64-array", func(t *testing.T) {
		want := fixture("0100000002000000020207050000002f64656d6f0500000076616c75651400000002000000ffffffffffffffffffffffffffffff7f")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 2, Tag: EventInt64Array, Node: "/demo", Name: "value", Value: &ParameterValue{Kind: ParameterInt64Array, Int64s: []int64{-1, 9223372036854775807}}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-event-float64-array", func(t *testing.T) {
		want := fixture("0100000002000000020208050000002f64656d6f0500000076616c75650c00000001000000000000000000f43f")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 2, Tag: EventFloat64Array, Node: "/demo", Name: "value", Value: &ParameterValue{Kind: ParameterFloat64Array, Float64s: []float64{1.25}}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-event-string-array", func(t *testing.T) {
		want := fixture("0100000002000000020209050000002f64656d6f0500000076616c756511000000020000000500000068656c6c6f00000000")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 2, Tag: EventStringArray, Node: "/demo", Name: "value", Value: &ParameterValue{Kind: ParameterStringArray, Strings: []string{"hello", ""}}}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-deletion-tag-0", func(t *testing.T) {
		want := fixture("0100000002000000030200050000002f64656d6f0500000076616c756500000000")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 3, Tag: EventDeletion, Node: "/demo", Name: "value"}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-deletion-tag-1", func(t *testing.T) {
		want := fixture("0100000002000000030201050000002f64656d6f0500000076616c756500000000")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 3, Tag: EventBool, Node: "/demo", Name: "value"}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-deletion-tag-2", func(t *testing.T) {
		want := fixture("0100000002000000030202050000002f64656d6f0500000076616c756500000000")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 3, Tag: EventInt64, Node: "/demo", Name: "value"}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-deletion-tag-3", func(t *testing.T) {
		want := fixture("0100000002000000030203050000002f64656d6f0500000076616c756500000000")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 3, Tag: EventFloat64, Node: "/demo", Name: "value"}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-deletion-tag-4", func(t *testing.T) {
		want := fixture("0100000002000000030204050000002f64656d6f0500000076616c756500000000")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 3, Tag: EventString, Node: "/demo", Name: "value"}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-deletion-tag-5", func(t *testing.T) {
		want := fixture("0100000002000000030205050000002f64656d6f0500000076616c756500000000")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 3, Tag: EventByteArray, Node: "/demo", Name: "value"}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-deletion-tag-6", func(t *testing.T) {
		want := fixture("0100000002000000030206050000002f64656d6f0500000076616c756500000000")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 3, Tag: EventBoolArray, Node: "/demo", Name: "value"}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-deletion-tag-7", func(t *testing.T) {
		want := fixture("0100000002000000030207050000002f64656d6f0500000076616c756500000000")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 3, Tag: EventInt64Array, Node: "/demo", Name: "value"}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-deletion-tag-8", func(t *testing.T) {
		want := fixture("0100000002000000030208050000002f64656d6f0500000076616c756500000000")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 3, Tag: EventFloat64Array, Node: "/demo", Name: "value"}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
	t.Run("typed-deletion-tag-9", func(t *testing.T) {
		want := fixture("0100000002000000030209050000002f64656d6f0500000076616c756500000000")
		v := TypedParameterEvent{Seconds: 1, Nanoseconds: 2, Kind: 3, Tag: EventStringArray, Node: "/demo", Name: "value"}
		b, err := v.Encode()
		assertBytes(t, b, want, err)
		decoded, err := DecodeTypedParameterEvent(want)
		if err != nil {
			t.Fatal(err)
		}
		b, err = decoded.Encode()
		assertBytes(t, b, want, err)
	})
}
func TestSharedNegativeVectors(t *testing.T) {
	{
		b := fixture("02")
		_, err := DecodeBool(b)
		if err == nil {
			t.Fatal("expected refusal: Bool must be exactly 0 or 1.")
		}
	}
	{
		b := fixture("0100")
		_, err := DecodeBool(b)
		if err == nil {
			t.Fatal("expected refusal: Reject trailing bytes.")
		}
	}
	{
		b := fixture("050000006869")
		_, err := DecodeString(b)
		if err == nil {
			t.Fatal("expected refusal: Declared UTF-8 byte length exceeds the remaining buffer.")
		}
	}
	{
		b := fixture("02000000c0af")
		_, err := DecodeString(b)
		if err == nil {
			t.Fatal("expected refusal: Overlong UTF-8 is invalid.")
		}
	}
	{
		b := fixture("000000000000f07f")
		_, err := DecodeParameter(ParameterFloat64, b)
		if err == nil {
			t.Fatal("expected refusal: Float64 must be finite.")
		}
	}
	{
		b := fixture("01000000000000000000f87f")
		_, err := DecodeParameter(ParameterFloat64Array, b)
		if err == nil {
			t.Fatal("expected refusal: Every Float64 array element must be finite.")
		}
	}
	{
		b := fixture("0100000002")
		_, err := DecodeParameter(ParameterBoolArray, b)
		if err == nil {
			t.Fatal("expected refusal: Every Bool array element must be 0 or 1.")
		}
	}
	{
		b := fixture("0100000003000000610062")
		_, err := DecodeParameter(ParameterStringArray, b)
		if err == nil {
			t.Fatal("expected refusal: String-array elements are NUL-free.")
		}
	}
	{
		b := fixture("00000000000000000200000000")
		_, err := DecodeFibonacciResult(b)
		if err == nil {
			t.Fatal("expected refusal: An owned Goal ID is nonzero.")
		}
	}
	{
		b := fixture("0100000002000000010200050000002f64656d6f0500000076616c756500000000")
		_, err := DecodeTypedParameterEvent(b)
		if err == nil {
			t.Fatal("expected refusal: Deletion-only tag 0 requires deleted kind 3.")
		}
	}
	{
		b := fixture("0100000002000000030201050000002f64656d6f0500000076616c75650100000001")
		_, err := DecodeTypedParameterEvent(b)
		if err == nil {
			t.Fatal("expected refusal: Deleted events have an empty selected payload.")
		}
	}
}
