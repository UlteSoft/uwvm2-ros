//go:build tinygo && (wasm || tinygo.wasm) && ((ros1 && !ros2) || (ros2 && !ros1))

package roswasm

import("encoding/binary";"runtime";"unsafe")

// WatchEvents binds this explicitly granted observer to a source once. Zero
// correlation selects topics/services/incoming servers and metadata-only watches.
// Selected outgoing Action data requires a nonzero already owned goalID. A data
// watch must precede legacy activation; metadata can coexist without consuming it.
func(c *Capability) WatchEvents(source *Capability,correlation uint64)error {
	if c==nil||source==nil{return Error{Code:BadHandle}}
	if err:=ownersIdle([]*capabilityState{c.state,source.state});err!=nil{return err}
	h,err:=c.RawHandle();if err!=nil{return err};s,err:=source.RawHandle();if err!=nil{return err}
	return status(RawEventWatch(h,s,correlation))
}
// PollEventV1 copies one complete event into caller-bounded memory0 storage.
// Unlike legacy Poll, TooLarge retains the record. Read Required only when the
// output sentinel changed, and decode initialized bytes before using the body.
func(c *Capability) PollEventV1(output []byte)(uint64,error) {
	if c==nil{return 0,Error{Code:BadHandle}};if err:=ownersIdle([]*capabilityState{c.state});err!=nil{return 0,err}
	h,err:=c.RawHandle();if err!=nil{return 0,err};out,err:=bytesAddress(output);if err!=nil{return 0,err}
	slot:=[8]byte{255,255,255,255,255,255,255,255};z,err:=address(unsafe.Pointer(&slot[0]));if err!=nil{return 0,err}
	code:=RawEventPollV1(h,0,out,uint64(len(output)),z);runtime.KeepAlive(output);runtime.KeepAlive(&slot);return finish(code,slot,uint64(len(output)))
}
// EventReadiness observes current bounded credit without reserving it.
func(c *Capability) EventReadiness()(EventReadiness,error) {
	if c==nil{return EventReadiness{},Error{Code:BadHandle}};if err:=ownersIdle([]*capabilityState{c.state});err!=nil{return EventReadiness{},err}
	h,err:=c.RawHandle();if err!=nil{return EventReadiness{},err};var bytes [32]byte;out,err:=address(unsafe.Pointer(&bytes[0]));if err!=nil{return EventReadiness{},err}
	code:=RawEventReady(h,0,out);runtime.KeepAlive(&bytes);if err:=status(code);err!=nil{return EventReadiness{},err};return DecodeEventReadiness(bytes[:])
}
// SubmitService admits one canonical request through this service source and its
// separately granted observer. Success returns an owned requestID, not completion.
// Response/error masks must both be explicit. The native task owns a copied input.
// No goroutine/clock/thread is started by the SDK. Never replay107/110 implicitly.
func(c *Capability) SubmitService(observer *Capability,input []byte)(uint64,error) {
	if c==nil||observer==nil{return 0,Error{Code:BadHandle}};if err:=ownersIdle([]*capabilityState{c.state,observer.state});err!=nil{return 0,err}
	h,err:=c.RawHandle();if err!=nil{return 0,err};o,err:=observer.RawHandle();if err!=nil{return 0,err};a,err:=bytesAddress(input);if err!=nil{return 0,err}
	var slot [8]byte;z,err:=address(unsafe.Pointer(&slot[0]));if err!=nil{return 0,err}
	code:=RawServiceSubmit(h,o,0,a,uint64(len(input)),z);runtime.KeepAlive(input);runtime.KeepAlive(&slot);if err:=status(code);err!=nil{return 0,err};id:=binary.LittleEndian.Uint64(slot[:]);if id==0{return 0,Error{Code:Invalid}};return id,nil
}
