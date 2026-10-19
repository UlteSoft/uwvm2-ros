package roswasm

import "unsafe"

// Private actual dispatcher core. The Wasm Server delegates its real imports;
// native tests supply local sources/counters. This creates no public mock backend,
// native resource or permission, and does not change any registered ABI call.
type callbackState struct { busy bool }
func callbackIdle(state *callbackState) error {
	if state == nil { return Error{Code:BadHandle} }
	if state.busy { return Error{Code:Conflict} }
	return nil
}
type serviceSource interface {
	pollService([]byte)(uint64,error)
	respondService([]byte)error
}
type goalSource interface { pollGoal()(GoalID,int32,error) }
func serviceOnce(state *callbackState, source serviceSource, input,output []byte, handler func(RequestID,[]byte,[]byte)(uint64,error))(bool,error){
	if err:=callbackIdle(state); err!=nil { return false,err }
	if handler==nil || source==nil { return false,Error{Code:Invalid} }
	// Ordinary local buffer overlap checking works in native tests and wasm32.
	// ABI pointer-width validation remains inside Capability before its import.
	if len(input)>0 && len(output)>0 {
		a:=uintptr(unsafe.Pointer(&input[0])); b:=uintptr(unsafe.Pointer(&output[0]))
		if a<b+uintptr(len(output)) && b<a+uintptr(len(input)){ return false,Error{Code:Invalid} }
	}
	state.busy=true; defer func(){ state.busy=false }()
	n,err:=source.pollService(input)
	if err!=nil { if e,ok:=err.(Error);ok && e.Code==Again { return false,nil }; return false,err }
	if n>uint64(len(input)) || (n!=8 && n!=9){ return false,Error{Code:Invalid} }
	id,err:=DecodeTriggerEvent(input[:8]); if err!=nil { return false,err }
	if n==9 { if _,err=DecodeBool(input[8:9]);err!=nil { return false,err } }
	used,err:=handler(id,input[8:n],output); if err!=nil { return false,err }
	if used<13 || used>uint64(len(output)){ return false,Error{Code:Invalid} }
	replyID,err:=DecodeTriggerEvent(output[:8]); if err!=nil || replyID.Raw()!=id.Raw(){ return false,Error{Code:Invalid} }
	if _,err=DecodeServiceResponse(output[8:used]);err!=nil { return false,err }
	return true,source.respondService(output[:used])
}
func goalOnce(state *callbackState, source goalSource, handler func(GoalID,int32)error)(bool,error){
	if err:=callbackIdle(state);err!=nil { return false,err }
	if source==nil || handler==nil { return false,Error{Code:Invalid} }
	state.busy=true; defer func(){ state.busy=false }()
	id,order,err:=source.pollGoal()
	if err!=nil { if e,ok:=err.(Error);ok && e.Code==Again { return false,nil }; return false,err }
	return true,handler(id,order)
}
