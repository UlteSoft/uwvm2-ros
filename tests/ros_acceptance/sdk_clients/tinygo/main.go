// Package main exercises the real TinyGo SDK typed Node pub/sub, not fake imports.
// Build with scheduler=none and exactly one ros1/ros2 tag. The final binary's
// startup imports must be inspected: if this selected TinyGo runtime requires
// entropy, the supervisor must explicitly review/grant WASIp1 random in both
// rows. It must never install a fallback, infer authority from a failure, or
// inherit environment/socket/thread rights. ROS permissions remain empty by
// default even when a separate explicit language-startup WASI right is needed.
package main

import (
    "runtime"
    "unsafe"
    ros "uwvm2.local/roswasm"
)

type iovec struct { address uint32; length uint32 }
// WASIp1 uses four core i32 arguments, including integer offsets to this
// genuine eight-byte iovec and the four-byte count slot. A Go *iovec is not a
// supported wasmimport parameter for this compiler: an ordinary Go struct has
// no structs.HostLayout marker. Keep the raw signature scalar and convert the
// live local addresses at each call; do not change the WASI layout or permissions.
//go:wasmimport wasi_snapshot_preview1 fd_read
func wasiRead(uint32,uint32,uint32,uint32) uint32
//go:wasmimport wasi_snapshot_preview1 fd_write
func wasiWrite(uint32,uint32,uint32,uint32) uint32
//go:wasmimport wasi_snapshot_preview1 proc_exit
func wasiExit(uint32)
func address(pointer unsafe.Pointer) uint64 { return uint64(uintptr(pointer)) }
func check(ok bool) error { if !ok { return ros.Error{Code:ros.Invalid} };return nil }
func emit(text string) error {
    bytes:=[]byte(text); vector:=iovec{uint32(uintptr(unsafe.Pointer(&bytes[0]))),uint32(len(bytes))};count:=uint32(0xffffffff)
    code:=wasiWrite(1,uint32(uintptr(unsafe.Pointer(&vector))),1,uint32(uintptr(unsafe.Pointer(&count))))
    // Integer ABI operands carry no Go ownership information. Retain every
    // referenced object through the synchronous import before inspecting errno
    // or its count slot; this adds no buffering, retry or alternate I/O path.
    runtime.KeepAlive(bytes);runtime.KeepAlive(&vector);runtime.KeepAlive(&count)
    if code!=0 {return ros.Error{Code:code}};return check(count==uint32(len(bytes)))
}
func gate(expected byte) error {
    var byteValue byte;vector:=iovec{uint32(uintptr(unsafe.Pointer(&byteValue))),1};count:=uint32(0xffffffff)
    code:=wasiRead(0,uint32(uintptr(unsafe.Pointer(&vector))),1,uint32(uintptr(unsafe.Pointer(&count))))
    runtime.KeepAlive(&byteValue);runtime.KeepAlive(&vector);runtime.KeepAlive(&count)
    if code!=0 {return ros.Error{Code:code}};return check(count==1&&byteValue==expected)
}
func denied(alias string) error {
    bytes:=[]byte(alias);slot:=uint64(0x8877665544332211)
    code:=ros.RawCapabilityOpen(0,address(unsafe.Pointer(&bytes[0])),uint64(len(bytes)),address(unsafe.Pointer(&slot)))
    return check(code==ros.Denied&&slot==0x8877665544332211)
}
func refusal(handle uint64,expected uint32) error {
    var bytes [64]byte;for i:=range bytes {bytes[i]=0xa5};length:=uint64(0xffffffffffffffff)
    code:=ros.RawEventPoll(handle,0,address(unsafe.Pointer(&bytes[0])),64,address(unsafe.Pointer(&length)))
    if err:=check(code==expected&&length==0xffffffffffffffff);err!=nil{return err};for _,byteValue:=range bytes {if byteValue!=0xa5{return ros.Error{Code:ros.Invalid}}};return nil
}
func run() (first error) {
    if ros.RawAbiVersion()!=1{return ros.Error{Code:ros.Unsupported}}
    if deniedMode {for _,alias:=range []string{"pub","sub","ungranted"} {if err:=denied(alias);err!=nil{return err}};return emit("PASS_SDK_DEFAULT_DENIED\n")}
    if selectedBackend!=1&&selectedBackend!=2{return ros.Error{Code:ros.Invalid}}
    // NewSession's argument is the local owner budget, not a backend selector.
    // The ros1/ros2 build tag selects the actual raw import module separately.
    session,err:=ros.NewSession(2);if err!=nil{return err}
    defer func(){for _,result:=range session.Close(){if result!=nil&&first==nil{first=result}}}()
    node:=session.NewNode("sdk-delivery");publisher,err:=node.CreatePublisher("pub");if err!=nil{return err}
    subscription,err:=node.CreateSubscription("sub");if err!=nil{return err}
    oldPub,err:=publisher.RawHandle();if err!=nil{return err};oldSub,err:=subscription.RawHandle();if err!=nil{return err}
    if err=denied("ungranted");err!=nil{return err};if err=refusal(oldPub,ros.Denied);err!=nil{return err}
    var scratch [64]byte;if err=publisher.PublishString("warmup-js-relay",scratch[:]);err!=nil{return err}
    if err=emit("SDK_ENDPOINTS_CREATED\n");err!=nil{return err};if err=gate('P');err!=nil{return err}
    if err=publisher.PublishString("roswasm-js-relay-out",scratch[:]);err!=nil{return err};if err=emit("SDK_FINAL_SENT\n");err!=nil{return err}
    received:=false
    for i:=0;i<200;i++ {
        if err=gate('R');err!=nil{return err};var output [64]byte;for j:=range output {output[j]=0xa5}
        count,result:=subscription.Poll(output[:]);if result!=nil {status,ok:=result.(ros.Error);if !ok||status.Code!=ros.Again{return result};for _,v:=range output{if v!=0xa5{return ros.Error{Code:ros.Invalid}}};if err=emit("SDK_POLL_AGAIN\n");err!=nil{return err};continue}
        if count!=23||string(output[:23])!="\x13\x00\x00\x00roswasm-js-relay-in"{return ros.Error{Code:ros.Invalid}}
        for _,v:=range output[23:]{if v!=0xa5{return ros.Error{Code:ros.Invalid}}};text,result:=ros.DecodeString(output[:23]);if result!=nil{return result};if text!="roswasm-js-relay-in"{return ros.Error{Code:ros.Invalid}};received=true;break
    }
    if !received{return ros.Error{Code:ros.Invalid}};if err=emit("SDK_DELIVERED\n");err!=nil{return err}
    if err=subscription.Close();err!=nil{return err};if err=publisher.Close();err!=nil{return err}
    if err=refusal(oldPub,ros.BadHandle);err!=nil{return err};if err=refusal(oldSub,ros.BadHandle);err!=nil{return err}
    return emit("PASS_SDK_DELIVERY\n")
}
// main is a real application entry. No print/panic shim grants host authority.
func main(){if err:=run();err!=nil{if status,ok:=err.(ros.Error);ok{wasiExit(status.Code)}else{wasiExit(ros.Invalid)};return};wasiExit(0)}
