package roswasm

import "encoding/binary"

// EventKind is the stable seven-kind versioned event selector. It confers no
// native permission; the observer and actual source need independent grants.
type EventKind uint16
const (
	EventTopicMessage EventKind=1
	EventServiceRequest EventKind=2
	EventServiceResponse EventKind=3
	EventActionFeedback EventKind=4
	EventActionResult EventKind=5
	EventResourceError EventKind=6
	EventResourceClosed EventKind=7
)
// UnifiedEvent owns metadata and borrows Body from the caller's complete copy.
// Flags:1 source retired,2 admitted external effects may remain,4 incoming Action,
//8 parameter projection. Decode Body with the unchanged actual source codec.
// Instance is a trusted integer, never a native pointer. All IDs remain uint64.
type UnifiedEvent struct {
	Kind EventKind
	Backend,Flags uint8
	Error uint32
	Source,Correlation,Sequence,Instance,Session uint64
	Body []byte
}
// DecodeUnifiedEvent validates the entire64-byte LE envelope before publishing
// any result. It rejects unknown flags/version/kind, nonzero reserved fields,
// zero identities, malformed lengths and duplicated ticket mismatches. It does
// not invoke any import, allocate a body buffer or reinterpret native structs.
func DecodeUnifiedEvent(bytes []byte)(UnifiedEvent,error) {
	bad:=func()(UnifiedEvent,error){return UnifiedEvent{},Error{Code:Invalid}}
	if len(bytes)<64||len(bytes)>4194304||binary.LittleEndian.Uint32(bytes)!=0x45525755||binary.LittleEndian.Uint16(bytes[4:])!=1||binary.LittleEndian.Uint16(bytes[10:])!=0 {return bad()}
	e:=UnifiedEvent{Kind:EventKind(binary.LittleEndian.Uint16(bytes[6:])),Backend:bytes[8],Flags:bytes[9],Error:binary.LittleEndian.Uint32(bytes[12:]),Source:binary.LittleEndian.Uint64(bytes[16:]),Correlation:binary.LittleEndian.Uint64(bytes[24:]),Sequence:binary.LittleEndian.Uint64(bytes[32:]),Instance:binary.LittleEndian.Uint64(bytes[40:]),Session:binary.LittleEndian.Uint64(bytes[48:]),Body:bytes[64:]}
	if e.Kind<1||e.Kind>7||e.Backend<1||e.Backend>2||e.Source==0||e.Sequence==0||e.Instance==0||e.Session==0||binary.LittleEndian.Uint64(bytes[56:])!=uint64(len(e.Body)) {return bad()}
	if e.Kind>=EventResourceError {
		if len(e.Body)!=0||e.Flags & ^uint8(3)!=0||(e.Kind==EventResourceError&&e.Error==0)||(e.Kind==EventResourceClosed&&e.Flags&1==0) {return bad()}
	} else {
		if e.Error!=0 {return bad()}
		if e.Kind==EventTopicMessage {if e.Correlation!=0||(e.Flags!=0&&e.Flags!=8) {return bad()}} else if e.Correlation==0||(e.Kind==EventServiceRequest&&(e.Flags!=0&&e.Flags!=4))||(e.Kind!=EventServiceRequest&&e.Flags!=0) {return bad()}
		if (e.Kind==EventServiceRequest||e.Kind==EventActionFeedback||e.Kind==EventActionResult)&&(len(e.Body)<8||binary.LittleEndian.Uint64(e.Body)!=e.Correlation) {return bad()}
	}
	return e,nil
}
// EventReadiness is a finite snapshot, not a reservation or permission grant.
// ServiceSubmit can still return11 even after a positive snapshot.
type EventReadiness struct {ReadyEvents,NextEventBytes,AdmissionSlots,AdmissionBytes uint64}
// DecodeEventReadiness requires exactly32 full-width little-endian bytes.
func DecodeEventReadiness(bytes []byte)(EventReadiness,error) {
	if len(bytes)!=32 {return EventReadiness{},Error{Code:Invalid}}
	return EventReadiness{binary.LittleEndian.Uint64(bytes),binary.LittleEndian.Uint64(bytes[8:]),binary.LittleEndian.Uint64(bytes[16:]),binary.LittleEndian.Uint64(bytes[24:])},nil
}

// EventBinding audits one independently authorized stream without allocating or
// creating a watch. Failed decoding leaves its source/session/sequence unchanged.
// Zero correlation accepts distinct service requests; a nonzero owned Action ID
// restricts correlated data while permitting uncorrelated numeric error metadata.
type EventBinding struct { backend uint8;source,correlation,sequence,instance,session uint64 }
// NewEventBinding selects an exact backend/source. Initial delivery is sequence1.
func NewEventBinding(backend uint8,source,correlation uint64)(EventBinding,error){
	if backend<1||backend>2||source==0{return EventBinding{},Error{Code:Invalid}}
	return EventBinding{backend:backend,source:source,correlation:correlation},nil
}
// Decode validates a complete event and commits its contiguous delivery sequence.
// Body remains borrowed from bytes. Copy it before the caller reuses scratch.
func(b *EventBinding) Decode(bytes []byte)(UnifiedEvent,error){
	if b==nil{return UnifiedEvent{},Error{Code:Invalid}};e,err:=DecodeUnifiedEvent(bytes);if err!=nil{return UnifiedEvent{},err}
	if e.Backend!=b.backend||e.Source!=b.source||b.sequence==^uint64(0)||e.Sequence!=b.sequence+1||
		(b.instance!=0&&(e.Instance!=b.instance||e.Session!=b.session))||
		(b.correlation!=0&&e.Correlation!=0&&e.Correlation!=b.correlation){return UnifiedEvent{},Error{Code:Invalid}}
	b.sequence=e.Sequence;b.instance=e.Instance;b.session=e.Session;return e,nil
}
