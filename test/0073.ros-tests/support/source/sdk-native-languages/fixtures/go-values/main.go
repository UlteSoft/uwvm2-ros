// This application runs real official-Go Wasm codecs over independent ABI bytes.
// Its embedded JSON is copied from the frozen ABI input by the finite-owner driver.
package main

import (
	"bytes"
	_ "embed"
	"encoding/binary"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"math"
	"strconv"
	ros "uwvm2.local/roswasmgo"
)

//go:embed wire-vectors-v1.json
var specification []byte

type vector struct {
	Name, Codec, Hex string
	Value            json.RawMessage
}
type suite struct{ Positive, Negative []vector }
type decoder func([]byte) (ros.Message, error)
type encoded []byte

func (b encoded) Encode() ([]byte, error) { return append([]byte(nil), b...), nil }
func unsigned(raw json.RawMessage) uint64 {
	value, err := strconv.ParseUint(parse[string](raw), 10, 64)
	if err != nil {
		panic(err)
	}
	return value
}
func goal(raw json.RawMessage) ros.GoalID {
	value := unsigned(raw)
	var b [8]byte
	binary.LittleEndian.PutUint64(b[:], value)
	id, err := ros.DecodeGoalID(b[:])
	if err != nil || id.Raw() != value {
		panic("exact goal ID")
	}
	return id
}
func goalBytes(id ros.GoalID) encoded { b := id.Bytes(); return encoded(b[:]) }
func fibonacci(order int32) (encoded, error) {
	var b [4]byte
	n, e := ros.EncodeFibonacciGoal(order, b[:])
	return encoded(b[:n]), e
}
func sequence(value ros.FibonacciSequence, feedback bool) (encoded, error) {
	var b [256]byte
	var n uint64
	var e error
	if feedback {
		n, e = ros.EncodeFibonacciFeedback(value.Goal, value.Values, b[:])
	} else {
		n, e = ros.EncodeFibonacciReply(value.Goal, value.Kind, value.Values, b[:])
	}
	return encoded(b[:n]), e
}
func parse[T any](raw json.RawMessage) T {
	var v T
	if err := json.Unmarshal(raw, &v); err != nil {
		panic(err)
	}
	return v
}
func real(raw json.RawMessage) float64 {
	if string(raw) == `"negative-zero"` {
		return math.Copysign(0, -1)
	}
	return parse[float64](raw)
}
func signed(raw json.RawMessage, bits int) int64 {
	v, err := strconv.ParseInt(parse[string](raw), 10, bits)
	if err != nil {
		panic(err)
	}
	return v
}
func boxed[T ros.Message](f func([]byte) (T, error)) decoder {
	return func(b []byte) (ros.Message, error) { v, e := f(b); return v, e }
}
func parameter(kind ros.ParameterKind, raw json.RawMessage) ros.ParameterValue {
	v := ros.ParameterValue{Kind: kind}
	switch kind {
	case ros.ParameterBool:
		v.Bool = parse[bool](raw)
	case ros.ParameterInt32:
		v.Int32 = int32(signed(raw, 32))
	case ros.ParameterInt64:
		v.Int64 = signed(raw, 64)
	case ros.ParameterFloat64:
		v.Float64 = real(raw)
	case ros.ParameterString:
		v.Text = parse[string](raw)
	case ros.ParameterBoolArray:
		v.Bools = parse[[]bool](raw)
	case ros.ParameterByteArray:
		v.Bytes = parse[[]byte](raw)
	case ros.ParameterStringArray:
		v.Strings = parse[[]string](raw)
	case ros.ParameterInt32Array:
		for _, e := range parse[[]json.RawMessage](raw) {
			v.Int32s = append(v.Int32s, int32(signed(e, 32)))
		}
	case ros.ParameterInt64Array:
		for _, e := range parse[[]json.RawMessage](raw) {
			v.Int64s = append(v.Int64s, signed(e, 64))
		}
	case ros.ParameterFloat64Array:
		for _, e := range parse[[]json.RawMessage](raw) {
			v.Float64s = append(v.Float64s, real(e))
		}
	default:
		panic("unsupported parameter constructor")
	}
	return v
}

var kinds = map[string]ros.ParameterKind{
	"parameter-int32": ros.ParameterInt32, "parameter-int64": ros.ParameterInt64,
	"parameter-float64": ros.ParameterFloat64, "parameter-bool-array": ros.ParameterBoolArray,
	"parameter-byte-array": ros.ParameterByteArray, "parameter-int32-array": ros.ParameterInt32Array,
	"parameter-int64-array": ros.ParameterInt64Array, "parameter-float64-array": ros.ParameterFloat64Array,
	"parameter-string-array": ros.ParameterStringArray,
}

func codec(v vector, construct bool) (ros.Message, decoder, bool) {
	if kind, ok := kinds[v.Codec]; ok {
		d := func(b []byte) (ros.Message, error) { return ros.DecodeParameter(kind, b) }
		if !construct {
			return nil, d, true
		}
		return parameter(kind, v.Value), d, true
	}
	switch v.Codec {
	case "trigger-request":
		d := func(b []byte) (ros.Message, error) {
			if e := ros.DecodeTriggerBody(b); e != nil {
				return nil, e
			}
			return encoded(ros.EncodeTriggerRequest()), nil
		}
		return encoded(ros.EncodeTriggerRequest()), d, true
	case "fibonacci-goal":
		d := func(b []byte) (ros.Message, error) {
			n, e := ros.DecodeFibonacciGoal(b)
			if e != nil {
				return nil, e
			}
			return fibonacci(n)
		}
		if !construct {
			return nil, d, true
		}
		b, e := fibonacci(parse[int32](v.Value))
		if e != nil {
			panic(e)
		}
		return b, d, true
	case "goal-id":
		d := func(b []byte) (ros.Message, error) {
			id, e := ros.DecodeGoalID(b)
			if e != nil {
				return nil, e
			}
			return goalBytes(id), nil
		}
		if !construct {
			return nil, d, true
		}
		return goalBytes(goal(v.Value)), d, true
	case "time":
		d := func(b []byte) (ros.Message, error) {
			n, e := ros.DecodeTime(b)
			if e != nil {
				return nil, e
			}
			var out [8]byte
			_, e = ros.EncodeTime(n, out[:])
			return encoded(out[:]), e
		}
		if !construct {
			return nil, d, true
		}
		var b [8]byte
		_, e := ros.EncodeTime(unsigned(v.Value), b[:])
		if e != nil {
			panic(e)
		}
		return encoded(b[:]), d, true
	case "incoming-goal":
		d := func(b []byte) (ros.Message, error) {
			id, order, e := ros.DecodeFibonacciGoalEvent(b)
			if e != nil {
				return nil, e
			}
			body, e := fibonacci(order)
			if e != nil {
				return nil, e
			}
			return ros.Envelope[encoded]{ID: id.Raw(), Value: body}, nil
		}
		if !construct {
			return nil, d, true
		}
		x := parse[struct {
			ID    json.RawMessage `json:"goal_id"`
			Order int32
		}](v.Value)
		body, e := fibonacci(x.Order)
		if e != nil {
			panic(e)
		}
		return ros.Envelope[encoded]{ID: unsigned(x.ID), Value: body}, d, true
	case "action-feedback", "action-result", "action-response":
		d := func(b []byte) (ros.Message, error) {
			var value ros.FibonacciSequence
			var e error
			switch v.Codec {
			case "action-feedback":
				value, e = ros.DecodeFibonacciFeedback(b)
			case "action-result":
				value, e = ros.DecodeFibonacciResult(b)
			case "action-response":
				value, e = ros.DecodeFibonacciReply(b)
			}
			if e != nil {
				return nil, e
			}
			return sequence(value, v.Codec == "action-feedback")
		}
		if !construct {
			return nil, d, true
		}
		x := parse[struct {
			ID     json.RawMessage `json:"goal_id"`
			Kind   uint8
			Values []int32
		}](v.Value)
		b, e := sequence(ros.FibonacciSequence{Goal: goal(x.ID), Kind: x.Kind, Values: x.Values}, v.Codec == "action-feedback")
		if e != nil {
			panic(e)
		}
		return b, d, true
	case "service-request-event":
		decode := func(b []byte) (ros.Bool, error) { x, e := ros.DecodeBool(b); return ros.Bool(x), e }
		d := func(b []byte) (ros.Message, error) {
			x, e := ros.DecodeIncoming(b, decode)
			if e != nil {
				return nil, e
			}
			return ros.Envelope[ros.Bool]{ID: x.ID, Value: x.Value}, nil
		}
		if !construct {
			return nil, d, true
		}
		x := parse[struct {
			ID  json.RawMessage `json:"request_id"`
			Hex string          `json:"request_hex"`
		}](v.Value)
		bytes, e := hex.DecodeString(x.Hex)
		if e != nil {
			panic(e)
		}
		body, e := decode(bytes)
		if e != nil {
			panic(e)
		}
		return ros.Envelope[ros.Bool]{ID: unsigned(x.ID), Value: body}, d, true
	case "service-response-envelope":
		decode := func(b []byte) (ros.ServiceResponse, error) {
			x, c := ros.DecodeServiceResponse(b)
			if c != 0 {
				return x, ros.APIError{Code: c}
			}
			return x, nil
		}
		d := func(b []byte) (ros.Message, error) {
			x, e := ros.DecodeIncoming(b, decode)
			if e != nil {
				return nil, e
			}
			return ros.Envelope[ros.ServiceResponse]{ID: x.ID, Value: x.Value}, nil
		}
		if !construct {
			return nil, d, true
		}
		x := parse[struct {
			ID  json.RawMessage `json:"request_id"`
			Hex string          `json:"response_hex"`
		}](v.Value)
		bytes, e := hex.DecodeString(x.Hex)
		if e != nil {
			panic(e)
		}
		body, e := decode(bytes)
		if e != nil {
			panic(e)
		}
		return ros.Envelope[ros.ServiceResponse]{ID: unsigned(x.ID), Value: body}, d, true
	case "string":
		d := func(b []byte) (ros.Message, error) {
			s, c := ros.DecodeString(b)
			if c != 0 {
				return nil, ros.APIError{Code: c}
			}
			return ros.String(s), nil
		}
		if !construct {
			return nil, d, true
		}
		return ros.String(parse[string](v.Value)), d, true
	case "bool", "set-bool-request":
		d := func(b []byte) (ros.Message, error) { x, e := ros.DecodeBool(b); return ros.Bool(x), e }
		if !construct {
			return nil, d, true
		}
		return ros.Bool(parse[bool](v.Value)), d, true
	case "twist":
		d := func(b []byte) (ros.Message, error) { x, e := ros.DecodeTwist(b); return ros.Twist(x), e }
		if !construct {
			return nil, d, true
		}
		var x ros.Twist
		for i, e := range parse[[]json.RawMessage](v.Value) {
			x[i] = real(e)
		}
		return x, d, true
	case "image":
		d := boxed(ros.DecodeImage)
		if !construct {
			return nil, d, true
		}
		x := parse[struct {
			Sequence, Seconds, Nanoseconds, Height, Width, Step uint32
			Frame                                               string `json:"frame_id"`
			Encoding                                            string
			BigEndian                                           bool
			Data                                                []byte
		}](v.Value)
		return ros.Image{Header: ros.Header{Sequence: x.Sequence, Seconds: x.Seconds, Nanoseconds: x.Nanoseconds, Frame: x.Frame}, Height: x.Height, Width: x.Width, Step: x.Step, Encoding: x.Encoding, BigEndian: x.BigEndian, Data: x.Data}, d, true
	case "transform":
		d := boxed(ros.DecodeTransform)
		if !construct {
			return nil, d, true
		}
		x := parse[struct {
			Sequence, Seconds, Nanoseconds uint32
			Parent                         string `json:"parent_frame"`
			Child                          string `json:"child_frame"`
			Translation                    [3]float64
			Quaternion                     [4]float64
		}](v.Value)
		return ros.Transform{Header: ros.Header{Sequence: x.Sequence, Seconds: x.Seconds, Nanoseconds: x.Nanoseconds, Frame: x.Parent}, ChildFrame: x.Child, Values: [7]float64{x.Translation[0], x.Translation[1], x.Translation[2], x.Quaternion[0], x.Quaternion[1], x.Quaternion[2], x.Quaternion[3]}}, d, true
	case "lifecycle-state":
		d := boxed(ros.DecodeLifecycleState)
		if !construct {
			return nil, d, true
		}
		return parse[ros.LifecycleState](v.Value), d, true
	case "service-response":
		d := func(b []byte) (ros.Message, error) {
			x, c := ros.DecodeServiceResponse(b)
			if c != 0 {
				return nil, ros.APIError{Code: c}
			}
			return x, nil
		}
		if !construct {
			return nil, d, true
		}
		return parse[ros.ServiceResponse](v.Value), d, true
	case "parameter-event":
		d := boxed(ros.DecodeLegacyParameterEvent)
		if !construct {
			return nil, d, true
		}
		return parse[ros.LegacyParameterEvent](v.Value), d, true
	case "typed-parameter-event":
		d := boxed(ros.DecodeTypedParameterEvent)
		if !construct {
			return nil, d, true
		}
		x := parse[struct {
			Seconds, Nanoseconds uint32
			Kind, Tag            uint8
			Node, Name           string
			Value                json.RawMessage
		}](v.Value)
		event := ros.TypedParameterEvent{Seconds: x.Seconds, Nanoseconds: x.Nanoseconds, Kind: x.Kind, Tag: ros.EventTag(x.Tag), Node: x.Node, Name: x.Name}
		if x.Kind != 3 {
			tags := []ros.ParameterKind{0, ros.ParameterBool, ros.ParameterInt64, ros.ParameterFloat64, ros.ParameterString, ros.ParameterByteArray, ros.ParameterBoolArray, ros.ParameterInt64Array, ros.ParameterFloat64Array, ros.ParameterStringArray}
			if int(x.Tag) >= len(tags) {
				panic("tag outside fixture contract")
			}
			value := parameter(tags[x.Tag], x.Value)
			event.Value = &value
		}
		return event, d, true
	}
	return nil, nil, false
}
func main() {
	if ros.AbiVersion() != 1 {
		panic("actual ROS ABI version")
	}
	owner, status := ros.Open("lesson.ungranted")
	if status != ros.Denied {
		if status == 0 {
			owner.Close()
		}
		panic("actual ROS default denial")
	}
	s := parse[suite](specification)
	positive, negative, truncated := 0, 0, 0
	var omitted []string
	for _, v := range s.Positive {
		value, decode, ok := codec(v, true)
		if !ok {
			omitted = append(omitted, v.Name)
			continue
		}
		want, err := hex.DecodeString(v.Hex)
		if err != nil {
			panic(err)
		}
		actual, err := value.Encode()
		if err != nil || !bytes.Equal(actual, want) {
			panic("golden encode: " + v.Name)
		}
		back, err := decode(want)
		if err != nil {
			panic("golden decode: " + v.Name)
		}
		encoded, err := back.Encode()
		if err != nil || !bytes.Equal(encoded, want) {
			panic("golden roundtrip: " + v.Name)
		}
		for i := 0; i < len(want); i++ {
			if _, err := decode(want[:i]); err == nil {
				panic("accepted truncation: " + v.Name)
			}
			truncated++
		}
		if _, err := decode(append(append([]byte(nil), want...), 0)); err == nil {
			panic("accepted trailing: " + v.Name)
		}
		positive++
	}
	for _, v := range s.Negative {
		_, decode, ok := codec(v, false)
		if !ok {
			omitted = append(omitted, "negative:"+v.Codec)
			continue
		}
		b, err := hex.DecodeString(v.Hex)
		if err != nil {
			panic(err)
		}
		if _, err := decode(b); err == nil {
			panic("accepted negative: " + v.Codec)
		}
		negative++
	}
	if len(omitted) != 0 || positive != len(s.Positive) || negative != len(s.Negative) {
		panic(fmt.Sprintf("incomplete ABI fixture coverage: positive=%d/%d negative=%d/%d omitted=%q", positive, len(s.Positive), negative, len(s.Negative), omitted))
	}
	fmt.Printf("PASS_TASK03_GO_VALUES positive=%d negative=%d truncations=%d omitted=%q\n", positive, negative, truncated, omitted)
}
