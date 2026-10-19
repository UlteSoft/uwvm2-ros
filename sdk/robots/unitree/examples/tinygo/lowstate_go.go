//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1 && unitree_lowstate && unitree_go && !unitree_hg

// Typed Unitree example source contract (comments only; executable code below).
// Source-stage contract: copy the selected complete generated family module and
// the actual Guest SDK into fresh owned work as specified by
// sdk/robots/unitree/recipes/closedtyped-example-recipes.json. Never substitute
// a projected schema, fake native identity, mock bridge or generated source stub.
// For exact build/staging and independent native oracles, read TYPED_EXAMPLES.md.
// Role: read actual ROS2 unitree_go/msg/LowState (20 carrier slots) or
// unitree_hg/msg/LowState (35), as explicitly selected by this entry/build.
// Mode0 checks the first unitree-lowstate alias open is denied without activating
// a subscription. Mode1 requires ONLY that alias as topic.subscribe; no publisher,
// LowCmd, service, robot address or CRC packer is introduced. Complete generated
// decode precedes observation of finite q/dq/tau_est, tick and the native CRC
// field. Every observed word must match an independent actual native witness.
// Ordered caller protocol: validate the real binary and explicitly initialize
// its selected reactor runtime first; use one fresh instance for mode0 denial
// and another for mode1 positive behavior. Then initialize_case once, explicitly
// submit for topic RPC, poll_once at most4096 times, inspect a received value,
// and close_case after the polling/callback stack has returned. No retry loop,
// clock, thread, goroutine or native endpoint is created by this source itself.
// A successful factory or codec operation does not prove native transport.
// The trusted external Runtime policy supplies rights; this code grants none.
// Status convention:0 means the documented operation succeeded;11 is an idle
// poll;9 rejects unused/closed/repeated lifecycle operations;22 rejects invalid
// configuration or message data;90 is a capacity bound;95 rejects the selected
// ABI when checked;110 exhausts the finite poll budget. Original SDK errors are
// preserved. Rust can additionally return17 for a conflicting state borrow.
// Expected mode0 alias denial13 is translated to0 ONLY with denied_verified=1.
// All acquired owners are explicitly retired on ordinary completion/failure,
// attempting later closes after an earlier close error. Traps/panics/kills still
// require the external process owner to finish actual Runtime/Gateway cleanup.
// TinyGo build note: explicitly run the actual exported _initialize once before
// any example call. Use opt1/scheduler-none/c-shared with the bounded pure target;
// actual zero-import/ROS-only binary and initializer behavior need separate proof.


package main
import vendor "example.local/uwvm-unitree-typed/customtypes"
const selectedFamily uint32=1
const selectedMotors=20
// The entire generated GO24 module is an external pinned build input.
func decodeState(bytes []byte)(vendor.Ros2Unitree_goMsgLowState,error){return vendor.DecodeRos2Unitree_goMsgLowState(bytes)}
