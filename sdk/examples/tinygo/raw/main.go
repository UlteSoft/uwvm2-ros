// Stage1 demonstrates exact imports. Build with TinyGo -target=wasi and exactly
// one of -tags=ros1/-tags=ros2. Explicit host alias pub must grant String publish.
package main

import (
	"encoding/binary"
	"runtime"
	"unsafe"
	ros "uwvm2.local/roswasm"
)

func main() {
	// uintptr is used only to obtain a wasm32 memory-zero address; the ABI
	// parameters remain full-width uint64 even on this32-bit target.
	if unsafe.Sizeof(uintptr(0)) != 4 {
		println("requires wasm32 memory zero")
		panic("unsupported pointer model")
	}
	if version := ros.RawAbiVersion(); version != 1 {
		println("unsupported ABI", version)
		panic("unsupported ROS ABI")
	}
	alias := []byte("pub")
	var slot [8]byte
	code := ros.RawCapabilityOpen(0, uint64(uintptr(unsafe.Pointer(&alias[0]))), uint64(len(alias)), uint64(uintptr(unsafe.Pointer(&slot[0]))))
	runtime.KeepAlive(alias)
	runtime.KeepAlive(&slot)
	if code != 0 {
		println("open error", code)
		panic("raw capability open failed")
	}
	handle := binary.LittleEndian.Uint64(slot[:])
	if handle == 0 { panic("malformed successful handle slot") }
	payload := []byte{5, 0, 0, 0, 'h', 'e', 'l', 'l', 'o'} // Canonical String, not CDR or a C string.
	code = ros.RawTopicPublish(handle, 0, uint64(uintptr(unsafe.Pointer(&payload[0]))), uint64(len(payload)))
	runtime.KeepAlive(payload)
	closeCode := ros.RawCapabilityClose(handle) // Always retire ownership, even after refusal.
	if code != 0 {
		println("publish error", code)
	}
	if closeCode != 0 {
		println("close notification failed after retirement", closeCode)
	}
	if code != 0 || closeCode != 0 { panic("raw publish/close failed") }
}
