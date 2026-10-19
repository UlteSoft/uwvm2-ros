package roswasm

import (
	"encoding/binary"
	"math"
	"unicode/utf8"
)

// MaxCanonicalBytes is the conservative canonical request-data ceiling. It
// excludes IPC framing and does not replace the host's smaller explicit quota.
const MaxCanonicalBytes uint64 = 4193786

// MaxParameterElements is the canonical array ceiling; grants need an explicit
// positive element limit and numeric arrays need explicit inclusive bounds.
const MaxParameterElements uint32 = 1048576

func invalid() error { return Error{Code: Invalid} }
func need(out []byte, n uint64) error {
	if n > uint64(len(out)) {
		return Error{Code: TooLarge, Required: n, HasRequired: true}
	}
	return nil
}

// EncodeString writes a u32 UTF-8 byte length and exact bytes, with no terminator.
// Only the returned prefix is initialized. It checks size before modifying out.
func EncodeString(value string, out []byte) (uint64, error) {
	if !utf8.ValidString(value) || uint64(len(value)) > uint64(^uint32(0)) {
		return 0, invalid()
	}
	n := uint64(len(value)) + 4
	if n > MaxCanonicalBytes {
		return 0, Error{Code: TooLarge}
	}
	if err := need(out, n); err != nil {
		return 0, err
	}
	binary.LittleEndian.PutUint32(out[:4], uint32(len(value)))
	copy(out[4:n], value)
	return n, nil
}

// DecodeString consumes exactly one canonical UTF-8 String. Embedded NUL is
// permitted by this wire format; individual native adapters may be stricter.
func DecodeString(b []byte) (string, error) {
	if len(b) < 4 || uint64(len(b)) > MaxCanonicalBytes {
		return "", invalid()
	}
	n := binary.LittleEndian.Uint32(b[:4])
	if uint64(n) != uint64(len(b)-4) || !utf8.Valid(b[4:]) {
		return "", invalid()
	}
	return string(b[4:]), nil
}

// EncodeBool writes exactly one byte0/1; it also serves SetBool requests.
func EncodeBool(v bool, out []byte) (uint64, error) {
	if err := need(out, 1); err != nil {
		return 0, err
	}
	out[0] = 0
	if v {
		out[0] = 1
	}
	return 1, nil
}

// DecodeBool rejects trailing bytes and any byte other than0 or1.
func DecodeBool(b []byte) (bool, error) {
	if len(b) != 1 || b[0] > 1 {
		return false, invalid()
	}
	return b[0] != 0, nil
}

// EncodeTwist writes finite linear xyz then angular xyz as six LE Float64s.
// Native host grants separately enforce magnitude and rate bounds.
func EncodeTwist(v [6]float64, out []byte) (uint64, error) {
	for _, x := range v {
		if math.IsNaN(x) || math.IsInf(x, 0) {
			return 0, invalid()
		}
	}
	if err := need(out, 48); err != nil {
		return 0, err
	}
	for i, x := range v {
		binary.LittleEndian.PutUint64(out[i*8:(i+1)*8], math.Float64bits(x))
	}
	return 48, nil
}

// DecodeTwist consumes exactly six finite LE Float64 values.
func DecodeTwist(b []byte) ([6]float64, error) {
	var v [6]float64
	if len(b) != 48 {
		return v, invalid()
	}
	for i := range v {
		v[i] = math.Float64frombits(binary.LittleEndian.Uint64(b[i*8 : (i+1)*8]))
		if math.IsNaN(v[i]) || math.IsInf(v[i], 0) {
			return [6]float64{}, invalid()
		}
	}
	return v, nil
}

// ServiceResponse is the native Trigger/SetBool Bool+String response. Success
// is a native service field, independent of an import's numeric error result.
type ServiceResponse struct {
	// Success is the native operation's success flag.
	Success bool
	// Message is the exact native UTF-8 response text.
	Message string
}

// EncodeServiceResponse writes a canonical Bool followed by canonical String.
func EncodeServiceResponse(v ServiceResponse, out []byte) (uint64, error) {
	if !utf8.ValidString(v.Message) || uint64(len(v.Message))+5 > MaxCanonicalBytes {
		return 0, invalid()
	}
	n := uint64(len(v.Message)) + 5
	if err := need(out, n); err != nil {
		return 0, err
	}
	out[0] = 0
	if v.Success {
		out[0] = 1
	}
	_, err := EncodeString(v.Message, out[1:n])
	return n, err
}

// DecodeServiceResponse consumes the complete canonical service response.
func DecodeServiceResponse(b []byte) (ServiceResponse, error) {
	if len(b) < 5 {
		return ServiceResponse{}, invalid()
	}
	flag, err := DecodeBool(b[:1])
	if err != nil {
		return ServiceResponse{}, err
	}
	text, err := DecodeString(b[1:])
	if err != nil {
		return ServiceResponse{}, err
	}
	return ServiceResponse{Success: flag, Message: text}, nil
}

// RequestID is obtained only by decoding an advertised service event. Its
// numeric value is private; the Gateway independently verifies pending ownership.
type RequestID struct{ value uint64 }

// Raw returns the exact Gateway-issued request ID for a reviewed low-level call.
func (id RequestID) Raw() uint64 { return id.value }

// DecodeTriggerEvent reads exactly one nonzero eight-byte owned request ID.
func DecodeTriggerEvent(b []byte) (RequestID, error) {
	if len(b) != 8 {
		return RequestID{}, invalid()
	}
	v := binary.LittleEndian.Uint64(b)
	if v == 0 {
		return RequestID{}, invalid()
	}
	return RequestID{v}, nil
}

// DecodeSetBoolEvent reads an owned request ID and one canonical Bool request.
func DecodeSetBoolEvent(b []byte) (RequestID, bool, error) {
	if len(b) != 9 {
		return RequestID{}, false, invalid()
	}
	id, err := DecodeTriggerEvent(b[:8])
	if err != nil {
		return RequestID{}, false, err
	}
	v, err := DecodeBool(b[8:])
	return id, v, err
}

// EncodeServiceReply prepends a decoded request ID to the selected response.
// The server's respond grant must match the advertise resource/type contract.
func EncodeServiceReply(id RequestID, v ServiceResponse, out []byte) (uint64, error) {
	if id.value == 0 || !utf8.ValidString(v.Message) {
		return 0, invalid()
	}
	n := uint64(len(v.Message)) + 13
	if n > MaxCanonicalBytes {
		return 0, Error{Code: TooLarge}
	}
	if err := need(out, n); err != nil {
		return 0, err
	}
	binary.LittleEndian.PutUint64(out[:8], id.value)
	_, err := EncodeServiceResponse(v, out[8:n])
	return n, err
}

// GoalID is a Gateway-issued Fibonacci ID. Its Go zero value is invalid and
// typed client/reply methods reject it. Copying a decoded ID never transfers
// authority, and its generation/session bits must never be synthesized.
type GoalID struct{ value uint64 }

// Raw returns the exact goal ID for a reviewed raw operation.
func (id GoalID) Raw() uint64 { return id.value }

// Bytes returns the canonical u64 request used by feedback/result/cancel calls.
// A zero-value GoalID has no authority and must not be used as a raw request.
func (id GoalID) Bytes() [8]byte {
	var b [8]byte
	binary.LittleEndian.PutUint64(b[:], id.value)
	return b
}

// EncodeFibonacciGoal writes order0..46; the host can require a smaller maximum.
func EncodeFibonacciGoal(order int32, out []byte) (uint64, error) {
	if order < 0 || order > 46 {
		return 0, invalid()
	}
	if err := need(out, 4); err != nil {
		return 0, err
	}
	binary.LittleEndian.PutUint32(out[:4], uint32(order))
	return 4, nil
}

// DecodeGoalID consumes a successful send-goal response without float conversion.
func DecodeGoalID(b []byte) (GoalID, error) {
	if len(b) != 8 {
		return GoalID{}, invalid()
	}
	v := binary.LittleEndian.Uint64(b)
	if v == 0 {
		return GoalID{}, invalid()
	}
	return GoalID{v}, nil
}

// DecodeFibonacciGoalEvent decodes an advertised owned ID followed by i32 order.
func DecodeFibonacciGoalEvent(b []byte) (GoalID, int32, error) {
	if len(b) != 12 {
		return GoalID{}, 0, invalid()
	}
	id, err := DecodeGoalID(b[:8])
	if err != nil {
		return GoalID{}, 0, err
	}
	order := int32(binary.LittleEndian.Uint32(b[8:]))
	if order < 0 || order > 46 {
		return GoalID{}, 0, invalid()
	}
	return id, order, nil
}

// FibonacciSequence is a decoded feedback/result envelope with an exact owned ID.
type FibonacciSequence struct {
	// Goal is the Gateway-issued goal ID; never construct a zero GoalID.
	Goal GoalID
	// Kind is feedback1, success2, abort3 or cancellation4.
	Kind uint8
	// Values contains at most48 nonnegative signed i32 values.
	Values []int32
}

func decodeSequence(b []byte, kindByte bool) (FibonacciSequence, error) {
	prefix := uint64(12)
	at := uint64(8)
	kind := uint8(1)
	if kindByte {
		prefix = 13
		at = 9
	}
	if uint64(len(b)) < prefix {
		return FibonacciSequence{}, invalid()
	}
	id, err := DecodeGoalID(b[:8])
	if err != nil {
		return FibonacciSequence{}, err
	}
	if kindByte {
		kind = b[8]
		if kind < 2 || kind > 4 {
			return FibonacciSequence{}, invalid()
		}
	}
	count := binary.LittleEndian.Uint32(b[at : at+4])
	if count > 48 || uint64(len(b))-prefix != uint64(count)*4 {
		return FibonacciSequence{}, invalid()
	}
	out := FibonacciSequence{Goal: id, Kind: kind, Values: make([]int32, count)}
	for i := uint32(0); i < count; i++ {
		start := prefix + uint64(i)*4
		v := int32(binary.LittleEndian.Uint32(b[start : start+4]))
		if v < 0 {
			return FibonacciSequence{}, invalid()
		}
		out.Values[i] = v
	}
	return out, nil
}

// DecodeFibonacciFeedback consumes the complete ID+count+values feedback envelope.
func DecodeFibonacciFeedback(b []byte) (FibonacciSequence, error) { return decodeSequence(b, false) }

// DecodeFibonacciResult consumes a terminal2/3/4 result envelope.
func DecodeFibonacciResult(b []byte) (FibonacciSequence, error) { return decodeSequence(b, true) }

// EncodeFibonacciReply writes server feedback1/result2/3/4. Cancellation4 remains
// subject to the native state machine; codecs do not grant cancel-any authority.
func EncodeFibonacciReply(id GoalID, kind uint8, values []int32, out []byte) (uint64, error) {
	if id.value == 0 || kind < 1 || kind > 4 || len(values) > 48 {
		return 0, invalid()
	}
	for _, v := range values {
		if v < 0 {
			return 0, invalid()
		}
	}
	n := uint64(13) + uint64(len(values))*4
	if err := need(out, n); err != nil {
		return 0, err
	}
	binary.LittleEndian.PutUint64(out[:8], id.value)
	out[8] = kind
	binary.LittleEndian.PutUint32(out[9:13], uint32(len(values)))
	for i, v := range values {
		binary.LittleEndian.PutUint32(out[13+i*4:17+i*4], uint32(v))
	}
	return n, nil
}

// DecodeTime reads exact unsigned nanoseconds in a separately granted clock domain.
func DecodeTime(b []byte) (uint64, error) {
	if len(b) != 8 {
		return 0, invalid()
	}
	return binary.LittleEndian.Uint64(b), nil
}

func putTime(out []byte, v uint64) { binary.LittleEndian.PutUint64(out, v) }

// EncodeTriggerRequest returns the canonical empty Trigger request.
func EncodeTriggerRequest() []byte { return []byte{} }

// DecodeTriggerRequest rejects every nonempty request for the Trigger adapter.
func DecodeTriggerRequest(b []byte) error {
	if len(b) != 0 {
		return invalid()
	}
	return nil
}

// EncodeFibonacciFeedback writes the incoming ID+count+values shape, without a
// kind byte. This codec helper does not grant native feedback publication rights.
func EncodeFibonacciFeedback(id GoalID, values []int32, out []byte) (uint64, error) {
	if id.value == 0 || len(values) > 48 {
		return 0, invalid()
	}
	for _, v := range values {
		if v < 0 {
			return 0, invalid()
		}
	}
	n := uint64(12) + uint64(len(values))*4
	if err := need(out, n); err != nil {
		return 0, err
	}
	binary.LittleEndian.PutUint64(out[:8], id.value)
	binary.LittleEndian.PutUint32(out[8:12], uint32(len(values)))
	for i, v := range values {
		binary.LittleEndian.PutUint32(out[12+i*4:16+i*4], uint32(v))
	}
	return n, nil
}

// DecodeFibonacciReply accepts canonical server feedback1 or result2/3/4. It
// checks shape only; it cannot attest to native cancellation/pending-goal state.
func DecodeFibonacciReply(b []byte) (FibonacciSequence, error) {
	if len(b) < 13 || b[8] < 1 || b[8] > 4 {
		return FibonacciSequence{}, invalid()
	}
	if b[8] > 1 {
		return DecodeFibonacciResult(b)
	}
	id, err := DecodeGoalID(b[:8])
	if err != nil {
		return FibonacciSequence{}, err
	}
	n := binary.LittleEndian.Uint32(b[9:13])
	if n > 48 || uint64(len(b)-13) != uint64(n)*4 {
		return FibonacciSequence{}, invalid()
	}
	v := FibonacciSequence{Goal: id, Kind: 1, Values: make([]int32, n)}
	for i := range v.Values {
		x := int32(binary.LittleEndian.Uint32(b[13+i*4 : 17+i*4]))
		if x < 0 {
			return FibonacciSequence{}, invalid()
		}
		v.Values[i] = x
	}
	return v, nil
}
