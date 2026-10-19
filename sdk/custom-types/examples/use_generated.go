// TinyGo: generated fixed records implement the existing roswasm.Message API.
// Copy generated fixed_records.go into your guest module's customtypes package.
// Replace example.local/guest below with that module's name. Build with the
// existing SDK ROS1 backend selection. The host must explicitly grant packet-pub
// with the official native identity, codec identity and every Topic quota.
// Build the temporary guest module serially with -target=wasi -scheduler=none
// -tags=ros1 and a separate /tmp output; select the SDK dependency explicitly.
// Inspect the emitted WASI imports, including any independent startup entropy
// requirement, instead of supplying automatic permissions. Basic stderr failure
// diagnostics do not request a native logger. Publication0 does not prove native
// delivery; a matching independent peer and qualified host adapter are required.
// Compilation/delivery remain separate acceptance scopes.
package main

import (
	ros "uwvm2.local/roswasm"
	fixed "example.local/guest/customtypes"
)

// publishPacket makes exactly one attempt and closes on all post-open paths.
func publishPacket()(primary error) {
	packet:=fixed.Ros1Example_interfaces_customPacket{Sequence:7,Active:true,Measurement:1.25,Tag:[4]uint8{0x41,0,0,0},Samples:[3]int16{-3,0,0}}
	bytes,err:=packet.Encode()
	if err!=nil{return err}
	publisher,err:=ros.Open("packet-pub")
	if err!=nil{return err}
	defer func(){
		cleanup:=publisher.Close(); cleanupCode=errorCode(cleanup)
		if cleanup!=nil && primary==nil { primary=cleanup }
	}()
	return publisher.Publish(bytes)
}

// Non-SDK fixed-record validation errors map to Invalid22; native SDK errors
// retain their unsigned exact code. This never selects another backend or retries.
func errorCode(err error)uint32 {
	if err==nil { return 0 }; if value,ok:=err.(ros.Error);ok { return value.Code }; return ros.Invalid
}

// main preserves the primary outcome and checked cleanup diagnostic, then fails
// visibly on any nonzero status. Panic/abort cannot promise further cleanup.
func main(){ primary:=publishPacket(); lastCode=errorCode(primary); if primary!=nil { println("checked Packet status",lastCode,"cleanup",cleanupCode); panic("generated Packet example failed") } }

// LastStatus exposes a primitive final status for a retained-instance harness.
// A command harness must also require no trap/nonzero exit; no payload is printed.
//go:wasmexport lastStatus
func LastStatus()uint32 { return lastCode }

// CleanupStatus reports the attempted close's exact code independently of the
// operation's primary result. Zero also applies when no handle was ever opened.
//go:wasmexport cleanupStatus
func CleanupStatus()uint32 { return cleanupCode }

// Local scalar diagnostics carry neither native resources nor hidden retries.
var lastCode,cleanupCode uint32
