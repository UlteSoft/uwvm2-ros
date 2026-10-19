//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1

// Same-main typed test framing only. The actual example remains in package
// main, compiled together with this file and one selected command_main file.
// No Cgo, replacement SDK, wire offsets, goroutine, timer or host-native ROS
// handle is used. Only actual basic WASIp1 pipe imports are introduced.
package main

import "unsafe"

//go:wasmimport wasi_snapshot_preview1 fd_read
func typedRead(fd uint32, vectors unsafe.Pointer, count uint32, transferred unsafe.Pointer) uint32
//go:wasmimport wasi_snapshot_preview1 fd_write
func typedWrite(fd uint32, vectors unsafe.Pointer, count uint32, transferred unsafe.Pointer) uint32
//go:wasmimport wasi_snapshot_preview1 proc_exit
func typedExit(code uint32)

type typedVector struct { address,length uint32 }
var typedInput [16]byte
var typedOutput [4096]byte
var typedUsed uint32
var typedOpened bool

// A failed operation preserves its first status and attempts all ordinary
// example-owned closes. The external process owner still handles traps/kills.
func typedFail(code uint32) {
	if typedOpened { _ = closeCase();typedOpened=false }
	if code==0 {code=22};typedExit(code);panic("proc_exit unexpectedly returned")
}
func typedRequire(ok bool) {if !ok {typedFail(22)}}
func typedInputBytes(length uint32) {
	typedRequire(length>0&&length<=16);at,attempts:=uint32(0),uint32(0)
	for at<length&&attempts<32 {
		attempts++;part:=typedVector{uint32(uintptr(unsafe.Pointer(&typedInput[at]))),length-at};done:=uint32(0)
		code:=typedRead(0,unsafe.Pointer(&part),1,unsafe.Pointer(&done))
		if code==27 {continue};if code!=0 {typedFail(code)}
		typedRequire(done>0&&done<=length-at);at+=done
	}
	if at!=length {typedFail(110)}
}
func typedAppend(value string) {
	for i:=0;i<len(value);i++ {typedRequire(value[i]<128&&typedUsed<4096);typedOutput[typedUsed]=value[i];typedUsed++}
}
func typedNumber(value uint32) {
	var digits [10]byte;n:=uint32(0)
	for {digits[n]=byte('0'+value%10);n++;value/=10;if value==0 {break}}
	for n>0 {n--;typedRequire(typedUsed<4096);typedOutput[typedUsed]=digits[n];typedUsed++}
}
func typedFlush() {
	at,attempts:=uint32(0),uint32(0)
	for at<typedUsed&&attempts<4096 {
		attempts++;part:=typedVector{uint32(uintptr(unsafe.Pointer(&typedOutput[at]))),typedUsed-at};done:=uint32(0)
		code:=typedWrite(1,unsafe.Pointer(&part),1,unsafe.Pointer(&done))
		if code==27 {continue};if code!=0 {typedFail(code)}
		typedRequire(done>0&&done<=typedUsed-at);at+=done
	}
	if at!=typedUsed {typedFail(110)};typedUsed=0
}
func typedLine(value string) {typedAppend(value);typedFlush()}
func typedLE32(offset uint32) uint32 {return uint32(typedInput[offset])|uint32(typedInput[offset+1])<<8|uint32(typedInput[offset+2])<<16|uint32(typedInput[offset+3])<<24}
func typedControl() byte {typedInputBytes(1);return typedInput[0]}
// Actual default-command startup initializes the TinyGo runtime/package data
// before calling main. READY therefore precedes typed SDK case initialization
// but follows real language initialization. Parent sends UTM1/family/mode/zero.
func typedSetup(family uint32) uint32 {
	typedLine("READY_TYPED_MAIN\n");typedInputBytes(16)
	typedRequire(typedInput[0]=='U'&&typedInput[1]=='T'&&typedInput[2]=='M'&&typedInput[3]=='1')
	typedRequire(typedLE32(4)==family&&typedLE32(8)<=1&&typedLE32(12)==0);return typedLE32(8)
}
func typedCheck(code uint32) {if code!=0 {typedFail(code)}}
func typedComplete() {typedOpened=false;typedLine("PASS_TYPED_CASE\n");typedExit(0);panic("proc_exit unexpectedly returned")}
