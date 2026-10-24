//go:build tinygo && (wasm || tinygo.wasm) && ros1 && !ros2

// Same-main legacy test framing only. The actual example remains in package
// main, compiled together with this file and one selected command_main file.
// No Cgo, replacement SDK, wire offsets, goroutine, timer or host-native ROS
// handle is used. Only actual basic WASIp1 pipe imports are introduced.
package main

import "unsafe"

//go:wasmimport wasi_snapshot_preview1 fd_read
func legacyRead(fd uint32, vectors unsafe.Pointer, count uint32, transferred unsafe.Pointer) uint32
//go:wasmimport wasi_snapshot_preview1 fd_write
func legacyWrite(fd uint32, vectors unsafe.Pointer, count uint32, transferred unsafe.Pointer) uint32
//go:wasmimport wasi_snapshot_preview1 proc_exit
func legacyExit(code uint32)

type legacyVector struct { address,length uint32 }
var legacyInput [16]byte
var legacyOutput [4096]byte
var legacyUsed uint32
var legacyOpened bool

// A failed operation preserves its first status and attempts all ordinary
// example-owned closes. The external process owner still handles traps/kills.
func legacyFail(code uint32) {
	if legacyOpened { _ = closeCase(0);legacyOpened=false }
	if code==0 {code=22};legacyExit(code);panic("proc_exit unexpectedly returned")
}
func legacyRequire(ok bool) {if !ok {legacyFail(22)}}
func legacyInputBytes(length uint32) {
	legacyRequire(length>0&&length<=16);at,attempts:=uint32(0),uint32(0)
	for at<length&&attempts<32 {
		attempts++;part:=legacyVector{uint32(uintptr(unsafe.Pointer(&legacyInput[at]))),length-at};done:=uint32(0)
		code:=legacyRead(0,unsafe.Pointer(&part),1,unsafe.Pointer(&done))
		if code==27 {continue};if code!=0 {legacyFail(code)}
		legacyRequire(done>0&&done<=length-at);at+=done
	}
	if at!=length {legacyFail(110)}
}
func legacyAppend(value string) {
	for i:=0;i<len(value);i++ {legacyRequire(value[i]<128&&legacyUsed<4096);legacyOutput[legacyUsed]=value[i];legacyUsed++}
}
func legacyNumber(value uint32) {
	var digits [10]byte;n:=uint32(0)
	for {digits[n]=byte('0'+value%10);n++;value/=10;if value==0 {break}}
	for n>0 {n--;legacyRequire(legacyUsed<4096);legacyOutput[legacyUsed]=digits[n];legacyUsed++}
}
func legacyFlush() {
	at,attempts:=uint32(0),uint32(0)
	for at<legacyUsed&&attempts<4096 {
		attempts++;part:=legacyVector{uint32(uintptr(unsafe.Pointer(&legacyOutput[at]))),legacyUsed-at};done:=uint32(0)
		code:=legacyWrite(1,unsafe.Pointer(&part),1,unsafe.Pointer(&done))
		if code==27 {continue};if code!=0 {legacyFail(code)}
		legacyRequire(done>0&&done<=legacyUsed-at);at+=done
	}
	if at!=legacyUsed {legacyFail(110)};legacyUsed=0
}
func legacyLine(value string) {legacyAppend(value);legacyFlush()}
func legacyLE32(offset uint32) uint32 {return uint32(legacyInput[offset])|uint32(legacyInput[offset+1])<<8|uint32(legacyInput[offset+2])<<16|uint32(legacyInput[offset+3])<<24}
func legacyControl() byte {legacyInputBytes(1);return legacyInput[0]}
// Actual default-command startup initializes the TinyGo runtime/package data
// before calling main. READY therefore precedes legacy SDK case initialization
// but follows real language initialization. Parent sends ULG1/family/mode/zero.
func legacySetup(family uint32) uint32 {
	legacyLine("READY_LEGACY_MAIN\n");legacyInputBytes(16)
	legacyRequire(legacyInput[0]=='U'&&legacyInput[1]=='L'&&legacyInput[2]=='G'&&legacyInput[3]=='1')
	legacyRequire(legacyLE32(4)==family&&legacyLE32(8)<=1&&legacyLE32(12)==0);return legacyLE32(8)
}
func legacyCheck(code uint32) {if code!=0 {legacyFail(code)}}
func legacyComplete() {legacyOpened=false;legacyLine("PASS_LEGACY_CASE\n");legacyExit(0);panic("proc_exit unexpectedly returned")}
