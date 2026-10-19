package roswasm

import (
	"encoding/binary"
	"math"
	"strings"
	"unicode/utf8"
)

// Message encodes one complete builtin canonical value. It is a closed ROS guest
// wire convenience, not an arbitrary native ROS/CDR serializer or an authority grant.
type Message interface{ Encode() ([]byte, error) }

type writer struct {
	b   []byte
	err error
}

func (w *writer) add(b []byte) {
	if w.err != nil {
		return
	}
	if uint64(len(b)) > MaxCanonicalBytes-uint64(len(w.b)) {
		w.err = Error{Code: TooLarge}
		return
	}
	w.b = append(w.b, b...)
}
func (w *writer) u8(v uint8)   { w.add([]byte{v}) }
func (w *writer) u32(v uint32) { var b [4]byte; binary.LittleEndian.PutUint32(b[:], v); w.add(b[:]) }
func (w *writer) u64(v uint64) { var b [8]byte; binary.LittleEndian.PutUint64(b[:], v); w.add(b[:]) }
func (w *writer) real(v float64) {
	if math.IsNaN(v) || math.IsInf(v, 0) {
		w.err = invalid()
		return
	}
	w.u64(math.Float64bits(v))
}
func (w *writer) blob(b []byte) {
	if uint64(len(b)) > uint64(^uint32(0)) {
		w.err = Error{Code: TooLarge}
		return
	}
	w.u32(uint32(len(b)))
	w.add(b)
}
func (w *writer) text(s string) {
	if !utf8.ValidString(s) {
		w.err = invalid()
		return
	}
	w.blob([]byte(s))
}
func (w *writer) finish() ([]byte, error) {
	if w.err != nil {
		return nil, w.err
	}
	return w.b, nil
}

type reader struct {
	b   []byte
	at  uint64
	err error
}

func newReader(b []byte) *reader {
	r := &reader{b: b}
	if uint64(len(b)) > MaxCanonicalBytes {
		r.err = Error{Code: TooLarge}
	}
	return r
}
func (r *reader) bytes(n uint64) []byte {
	if r.err != nil {
		return nil
	}
	if n > uint64(len(r.b))-r.at {
		r.err = invalid()
		return nil
	}
	b := r.b[r.at : r.at+n]
	r.at += n
	return b
}
func (r *reader) u8() uint8 {
	b := r.bytes(1)
	if r.err != nil {
		return 0
	}
	return b[0]
}
func (r *reader) u32() uint32 {
	b := r.bytes(4)
	if r.err != nil {
		return 0
	}
	return binary.LittleEndian.Uint32(b)
}
func (r *reader) u64() uint64 {
	b := r.bytes(8)
	if r.err != nil {
		return 0
	}
	return binary.LittleEndian.Uint64(b)
}
func (r *reader) real() float64 {
	v := math.Float64frombits(r.u64())
	if math.IsNaN(v) || math.IsInf(v, 0) {
		r.err = invalid()
	}
	return v
}
func (r *reader) blob() []byte { n := r.u32(); return r.bytes(uint64(n)) }
func (r *reader) text() string {
	b := r.blob()
	if !utf8.Valid(b) {
		r.err = invalid()
	}
	return string(b)
}
func (r *reader) done() error {
	if r.err != nil {
		return r.err
	}
	if r.at != uint64(len(r.b)) {
		return invalid()
	}
	return nil
}
func (r *reader) count(width uint64) uint32 {
	n := r.u32()
	if r.err != nil {
		return 0
	}
	if n > MaxParameterElements {
		r.err = Error{Code: TooLarge}
		return 0
	}
	if uint64(n) > (uint64(len(r.b))-r.at)/width {
		r.err = invalid()
		return 0
	}
	return n
}
func validStamp(seconds, nanos uint32) bool { return seconds <= 2147483647 && nanos < 1000000000 }
func eventName(s string, node bool) bool {
	maximum := 1024
	separator := byte('/')
	start := 0
	if !node {
		maximum = 255
		separator = '.'
	}
	if len(s) == 0 || len(s) > maximum {
		return false
	}
	if node {
		if len(s) < 2 || s[0] != '/' {
			return false
		}
		start = 1
	}
	first := true
	for i := start; i < len(s); i++ {
		c := s[i]
		if c == separator {
			if first {
				return false
			}
			first = true
			continue
		}
		alpha := c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_'
		if !alpha && !(c >= '0' && c <= '9' && !first) {
			return false
		}
		first = false
	}
	return !first
}

// String is a canonical UTF-8 ROS String, with a u32 byte-length prefix.
type String string

// Encode creates a fresh bounded canonical String. Native fields may be stricter about NUL.
func (s String) Encode() ([]byte, error) { w := &writer{}; w.text(string(s)); return w.finish() }

// Bool is a canonical one-byte Bool; no integer coercion is inferred.
type Bool bool

// Encode creates the one-byte canonical0/1 representation.
func (v Bool) Encode() ([]byte, error) {
	if v {
		return []byte{1}, nil
	}
	return []byte{0}, nil
}

// Twist contains finite linear xyz followed by angular xyz.
type Twist [6]float64

// Encode creates the exact48-byte canonical Twist payload.
func (v Twist) Encode() ([]byte, error) {
	b := make([]byte, 48)
	_, err := EncodeTwist([6]float64(v), b)
	if err != nil {
		return nil, err
	}
	return b, nil
}

// Header carries canonical Image/Transform sequence, stamp and frame. Supply a
// zero Sequence for ROS2; ROS1 native transports may generate their own sequence.
type Header struct {
	// Sequence is the canonical Header sequence.
	Sequence uint32
	// Seconds spans full uint32 for ROS1 Image. ROS2 Image and both Transform
	// adapters additionally require at most2147483647 through host/typed validation.
	Seconds uint32
	// Nanoseconds is below one billion.
	Nanoseconds uint32
	// Frame is the exact configured frame name, checked independently by the host.
	Frame string
}

func (h Header) write(w *writer) {
	if h.Nanoseconds >= 1000000000 {
		w.err = invalid()
		return
	}
	w.u32(h.Sequence)
	w.u32(h.Seconds)
	w.u32(h.Nanoseconds)
	w.text(h.Frame)
}
func readHeader(r *reader) Header {
	h := Header{Sequence: r.u32(), Seconds: r.u32(), Nanoseconds: r.u32(), Frame: r.text()}
	if h.Nanoseconds >= 1000000000 {
		r.err = invalid()
	}
	return h
}

// Image is the canonical builtin Image. The host separately checks supported
// native encoding, dimensions, step, payload quota and publication authority.
type Image struct {
	// Header identifies the timestamp and image frame.
	Header Header
	// Height is the pixel row count.
	Height uint32
	// Width is the pixel column count.
	Width uint32
	// Encoding is an explicit supported native name such as rgb8.
	Encoding string
	// BigEndian is encoded as a canonical one-byte Bool.
	BigEndian bool
	// Step is the bytes per row including explicit padding.
	Step uint32
	// Data contains the raw bounded image bytes.
	Data []byte
}

// Encode creates a fresh canonical Image, never native ROS serialized bytes.
func (v Image) Encode() ([]byte, error) {
	w := &writer{}
	v.Header.write(w)
	w.u32(v.Height)
	w.u32(v.Width)
	w.text(v.Encoding)
	if v.BigEndian {
		w.u8(1)
	} else {
		w.u8(0)
	}
	w.u32(v.Step)
	w.blob(v.Data)
	return w.finish()
}

// DecodeImage validates complete canonical structure and owns the returned data.
func DecodeImage(b []byte) (Image, error) {
	r := newReader(b)
	v := Image{Header: readHeader(r), Height: r.u32(), Width: r.u32(), Encoding: r.text()}
	flag := r.u8()
	if flag > 1 {
		r.err = invalid()
	}
	v.BigEndian = flag != 0
	v.Step = r.u32()
	v.Data = append([]byte(nil), r.blob()...)
	if err := r.done(); err != nil {
		return Image{}, err
	}
	return v, nil
}

// Transform is canonical TransformStamped with finite translation/quaternion.
// There is no implicit normalization or choice of unauthorized frame pairs.
type Transform struct {
	// Header provides the parent frame and timestamp.
	Header Header
	// ChildFrame is the exact configured child/source frame.
	ChildFrame string
	// Values contains translation xyz followed by quaternion xyzw.
	Values [7]float64
}

// Encode creates one bounded canonical TransformStamped.
func (v Transform) Encode() ([]byte, error) {
	if !validStamp(v.Header.Seconds, v.Header.Nanoseconds) {
		return nil, invalid()
	}
	w := &writer{}
	v.Header.write(w)
	w.text(v.ChildFrame)
	for _, x := range v.Values {
		w.real(x)
	}
	return w.finish()
}

// DecodeTransform consumes the entire finite canonical TransformStamped.
func DecodeTransform(b []byte) (Transform, error) {
	r := newReader(b)
	v := Transform{Header: readHeader(r), ChildFrame: r.text()}
	if !validStamp(v.Header.Seconds, v.Header.Nanoseconds) {
		r.err = invalid()
	}
	for i := range v.Values {
		v.Values[i] = r.real()
	}
	if err := r.done(); err != nil {
		return Transform{}, err
	}
	return v, nil
}

// ParameterKind fixes the exact canonical adapter. Raw values carry no type tag;
// decoding must use the host-selected kind, without numeric/string coercion.
type ParameterKind uint8

const (
	// ParameterBool is one canonical Bool.
	ParameterBool ParameterKind = iota + 1
	// ParameterInt32 is a signed four-byte ROS1 scalar.
	ParameterInt32
	// ParameterInt64 is a signed eight-byte ROS2 scalar.
	ParameterInt64
	// ParameterFloat64 is a finite eight-byte IEEE754 scalar.
	ParameterFloat64
	// ParameterString is a canonical UTF-8 scalar String.
	ParameterString
	// ParameterBoolArray is count+canonical Bool values.
	ParameterBoolArray
	// ParameterInt32Array is count+signed ROS1 i32 values.
	ParameterInt32Array
	// ParameterInt64Array is count+signed ROS2 i64 values.
	ParameterInt64Array
	// ParameterFloat64Array is count+finite Float64 values.
	ParameterFloat64Array
	// ParameterStringArray is count+UTF-8 NUL-free Strings.
	ParameterStringArray
	// ParameterByteArray is count+unsigned ROS2 bytes.
	ParameterByteArray
)

// ParameterValue stores the member selected by Kind. Other members are ignored;
// they never infer another adapter. Host scalar/array grants remain independent.
type ParameterValue struct {
	// Kind selects exactly one field and its explicit configured adapter.
	Kind ParameterKind
	// Bool is used only for ParameterBool.
	Bool bool
	// Int32 is used only for the ROS1 ParameterInt32 adapter.
	Int32 int32
	// Int64 is used only for the ROS2 ParameterInt64 adapter, with exact precision.
	Int64 int64
	// Float64 must be finite and is used only for ParameterFloat64.
	Float64 float64
	// Text holds scalar UTF-8 ParameterString.
	Text string
	// Bools holds ParameterBoolArray.
	Bools []bool
	// Int32s holds ROS1 ParameterInt32Array.
	Int32s []int32
	// Int64s holds ROS2 ParameterInt64Array without Float64 conversion.
	Int64s []int64
	// Float64s holds finite ParameterFloat64Array.
	Float64s []float64
	// Strings holds NUL-free UTF-8 ParameterStringArray.
	Strings []string
	// Bytes holds unsigned ParameterByteArray, distinct from Bools.
	Bytes []byte
}

func arrayCount(w *writer, n uint64) {
	if n > uint64(MaxParameterElements) {
		w.err = Error{Code: TooLarge}
		return
	}
	w.u32(uint32(n))
}

// Encode produces a fresh canonical selected value with count prefixes for
// arrays. Numeric bounds and explicit native element/byte limits remain host-side.
func (v ParameterValue) Encode() ([]byte, error) {
	w := &writer{}
	switch v.Kind {
	case ParameterBool:
		if v.Bool {
			w.u8(1)
		} else {
			w.u8(0)
		}
	case ParameterInt32:
		w.u32(uint32(v.Int32))
	case ParameterInt64:
		w.u64(uint64(v.Int64))
	case ParameterFloat64:
		w.real(v.Float64)
	case ParameterString:
		w.text(v.Text)
	case ParameterBoolArray:
		arrayCount(w, uint64(len(v.Bools)))
		for _, x := range v.Bools {
			if x {
				w.u8(1)
			} else {
				w.u8(0)
			}
		}
	case ParameterInt32Array:
		arrayCount(w, uint64(len(v.Int32s)))
		for _, x := range v.Int32s {
			w.u32(uint32(x))
		}
	case ParameterInt64Array:
		arrayCount(w, uint64(len(v.Int64s)))
		for _, x := range v.Int64s {
			w.u64(uint64(x))
		}
	case ParameterFloat64Array:
		arrayCount(w, uint64(len(v.Float64s)))
		for _, x := range v.Float64s {
			w.real(x)
		}
	case ParameterStringArray:
		arrayCount(w, uint64(len(v.Strings)))
		for _, x := range v.Strings {
			if strings.ContainsRune(x, 0) {
				w.err = invalid()
				break
			}
			w.text(x)
		}
	case ParameterByteArray:
		arrayCount(w, uint64(len(v.Bytes)))
		w.add(v.Bytes)
	default:
		w.err = invalid()
	}
	return w.finish()
}

// DecodeParameter consumes a whole payload using its explicit adapter kind.
// Refusal returns a zero result and does not replace any application variable.
func DecodeParameter(kind ParameterKind, b []byte) (ParameterValue, error) {
	r := newReader(b)
	v := ParameterValue{Kind: kind}
	switch kind {
	case ParameterBool:
		x := r.u8()
		if x > 1 {
			r.err = invalid()
		}
		v.Bool = x != 0
	case ParameterInt32:
		v.Int32 = int32(r.u32())
	case ParameterInt64:
		v.Int64 = int64(r.u64())
	case ParameterFloat64:
		v.Float64 = r.real()
	case ParameterString:
		v.Text = r.text()
	case ParameterBoolArray:
		n := r.count(1)
		v.Bools = make([]bool, n)
		for i := range v.Bools {
			x := r.u8()
			if x > 1 {
				r.err = invalid()
			}
			v.Bools[i] = x != 0
		}
	case ParameterInt32Array:
		n := r.count(4)
		v.Int32s = make([]int32, n)
		for i := range v.Int32s {
			v.Int32s[i] = int32(r.u32())
		}
	case ParameterInt64Array:
		n := r.count(8)
		v.Int64s = make([]int64, n)
		for i := range v.Int64s {
			v.Int64s[i] = int64(r.u64())
		}
	case ParameterFloat64Array:
		n := r.count(8)
		v.Float64s = make([]float64, n)
		for i := range v.Float64s {
			v.Float64s[i] = r.real()
		}
	case ParameterStringArray:
		n := r.count(4)
		v.Strings = make([]string, n)
		for i := range v.Strings {
			x := r.text()
			if strings.ContainsRune(x, 0) {
				r.err = invalid()
			}
			v.Strings[i] = x
		}
	case ParameterByteArray:
		n := r.count(1)
		v.Bytes = append([]byte(nil), r.bytes(uint64(n))...)
	default:
		r.err = invalid()
	}
	if err := r.done(); err != nil {
		return ParameterValue{}, err
	}
	return v, nil
}

// EventTag is the exact typed ROS2 parameter-event projection0..9. The tag does
// not authorize a subscription; each projection needs its own host grant.
type EventTag uint8

const (
	// EventDeletion is a separate NOT_SET deletion notification with no prior type/value attestation.
	EventDeletion EventTag = iota
	// EventBool selects canonical Bool payloads.
	EventBool
	// EventInt64 selects exact signed i64 payloads.
	EventInt64
	// EventFloat64 selects finite Float64 payloads.
	EventFloat64
	// EventString selects scalar UTF-8 String payloads.
	EventString
	// EventByteArray selects unsigned byte-array payloads.
	EventByteArray
	// EventBoolArray selects canonical Bool-array payloads.
	EventBoolArray
	// EventInt64Array selects signed i64-array payloads.
	EventInt64Array
	// EventFloat64Array selects finite Float64-array payloads.
	EventFloat64Array
	// EventStringArray selects UTF-8 NUL-free String-array payloads.
	EventStringArray
)

func eventParameter(tag EventTag) (ParameterKind, error) {
	switch tag {
	case EventBool:
		return ParameterBool, nil
	case EventInt64:
		return ParameterInt64, nil
	case EventFloat64:
		return ParameterFloat64, nil
	case EventString:
		return ParameterString, nil
	case EventByteArray:
		return ParameterByteArray, nil
	case EventBoolArray:
		return ParameterBoolArray, nil
	case EventInt64Array:
		return ParameterInt64Array, nil
	case EventFloat64Array:
		return ParameterFloat64Array, nil
	case EventStringArray:
		return ParameterStringArray, nil
	default:
		return 0, invalid()
	}
}

// TypedParameterEvent is a checked version2 envelope over ABI v1 imports.
// Deleted events have no Value; wire bytes cannot prove the native prior value.
type TypedParameterEvent struct {
	// Seconds is a nonnegative timestamp component.
	Seconds uint32
	// Nanoseconds is below one billion.
	Nanoseconds uint32
	// Kind is new1, changed2 or deleted3.
	Kind uint8
	// Tag is the independently configured projection.
	Tag EventTag
	// Node is the exact configured absolute ROS2 node.
	Node string
	// Name is the exact configured dotted parameter name.
	Name string
	// Value is present only for new/changed events, with the selected kind.
	Value *ParameterValue
}

// Encode creates a bounded typed envelope with outer node/name/payload lengths.
// Tag0 is admitted only for kind3 and deletion payloads must be empty.
func (v TypedParameterEvent) Encode() ([]byte, error) {
	if !validStamp(v.Seconds, v.Nanoseconds) || v.Kind < 1 || v.Kind > 3 || v.Tag > 9 || !eventName(v.Node, true) || !eventName(v.Name, false) {
		return nil, invalid()
	}
	var payload []byte
	if v.Kind == 3 {
		if v.Value != nil {
			return nil, invalid()
		}
	} else {
		kind, err := eventParameter(v.Tag)
		if err != nil || v.Value == nil || v.Value.Kind != kind {
			return nil, invalid()
		}
		payload, err = v.Value.Encode()
		if err != nil {
			return nil, err
		}
	}
	w := &writer{}
	w.u32(v.Seconds)
	w.u32(v.Nanoseconds)
	w.u8(v.Kind)
	w.u8(2)
	w.u8(uint8(v.Tag))
	w.text(v.Node)
	w.text(v.Name)
	w.blob(payload)
	return w.finish()
}

// DecodeTypedParameterEvent validates complete version/kind/tag/name/timestamp,
// selected scalar/array payloads and trailing bytes. Deleted prior contents remain unknown.
func DecodeTypedParameterEvent(b []byte) (TypedParameterEvent, error) {
	r := newReader(b)
	v := TypedParameterEvent{Seconds: r.u32(), Nanoseconds: r.u32(), Kind: r.u8()}
	version := r.u8()
	v.Tag = EventTag(r.u8())
	v.Node = r.text()
	v.Name = r.text()
	payload := r.blob()
	if err := r.done(); err != nil {
		return TypedParameterEvent{}, err
	}
	if !validStamp(v.Seconds, v.Nanoseconds) || v.Kind < 1 || v.Kind > 3 || version != 2 || v.Tag > 9 || !eventName(v.Node, true) || !eventName(v.Name, false) {
		return TypedParameterEvent{}, invalid()
	}
	if v.Kind == 3 {
		if len(payload) != 0 {
			return TypedParameterEvent{}, invalid()
		}
	} else {
		kind, err := eventParameter(v.Tag)
		if err != nil {
			return TypedParameterEvent{}, err
		}
		value, err := DecodeParameter(kind, payload)
		if err != nil {
			return TypedParameterEvent{}, err
		}
		v.Value = &value
	}
	return v, nil
}

// LegacyParameterEvent is the independently granted legacy String projection.
type LegacyParameterEvent struct {
	// Seconds is the nonnegative native event timestamp.
	Seconds uint32
	// Nanoseconds is below one billion.
	Nanoseconds uint32
	// Kind is new1, changed2 or deleted3.
	Kind uint8
	// Node is the selected absolute node.
	Node string
	// Name is the selected parameter name.
	Name string
	// Value is the selected String, empty on deletion.
	Value string
}

// Encode creates a bounded canonical legacy event without inventing a typed tag.
func (v LegacyParameterEvent) Encode() ([]byte, error) {
	if !validStamp(v.Seconds, v.Nanoseconds) || v.Kind < 1 || v.Kind > 3 || v.Kind == 3 && v.Value != "" {
		return nil, invalid()
	}
	w := &writer{}
	w.u32(v.Seconds)
	w.u32(v.Nanoseconds)
	w.u8(v.Kind)
	w.text(v.Node)
	w.text(v.Name)
	w.text(v.Value)
	return w.finish()
}

// DecodeLegacyParameterEvent consumes a complete independently granted String event.
func DecodeLegacyParameterEvent(b []byte) (LegacyParameterEvent, error) {
	r := newReader(b)
	v := LegacyParameterEvent{Seconds: r.u32(), Nanoseconds: r.u32(), Kind: r.u8(), Node: r.text(), Name: r.text(), Value: r.text()}
	if err := r.done(); err != nil {
		return LegacyParameterEvent{}, err
	}
	if !validStamp(v.Seconds, v.Nanoseconds) || v.Kind < 1 || v.Kind > 3 || v.Kind == 3 && v.Value != "" {
		return LegacyParameterEvent{}, invalid()
	}
	return v, nil
}

// LifecycleState carries one native state ID and a canonical UTF-8 label.
type LifecycleState struct {
	// ID is the native lifecycle state identifier.
	ID uint8
	// Label is the native lifecycle state label.
	Label string
}

// Encode creates the one-byte ID plus canonical String lifecycle response.
func (v LifecycleState) Encode() ([]byte, error) {
	w := &writer{}
	w.u8(v.ID)
	w.text(v.Label)
	return w.finish()
}

// DecodeLifecycleState consumes a complete lifecycle response.
func DecodeLifecycleState(b []byte) (LifecycleState, error) {
	r := newReader(b)
	v := LifecycleState{ID: r.u8(), Label: r.text()}
	if err := r.done(); err != nil {
		return LifecycleState{}, err
	}
	return v, nil
}
