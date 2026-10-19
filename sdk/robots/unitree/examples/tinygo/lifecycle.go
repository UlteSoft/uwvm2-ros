//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1

// Typed Unitree example source contract (comments only; executable code below).
// Source-stage contract: copy the selected complete generated family module and
// the actual Guest SDK into fresh owned work as specified by
// sdk/robots/unitree/recipes/closedtyped-example-recipes.json. Never substitute
// a projected schema, fake native identity, mock bridge or generated source stub.
// For exact build/staging and independent native oracles, read TYPED_EXAMPLES.md.
// Helper ownership: life owns the actual ROS2 Session/Node, Publisher,
// Subscription and bounded Executor. A registered subscription transfers to
// Executor; a failed registration remains owned here. close attempts Executor,
// unregistered subscription, Publisher, Node and Session in that order while
// keeping the first error. The selected role file supplies initialize/poll.
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

import ros "uwvm2.local/roswasm"

// life is local single-use state. It owns every acquired typed resource; the
// bounded Executor never starts a goroutine or reads a clock. Native authority
// is fixed by the independent Runtime alias policy, not these labels.
type life struct {
	started, closed, denied, registered bool
	status, polls, received uint32
	session *ros.Session
	node *ros.Node
	pub *ros.Publisher
	sub *ros.Subscription
	executor *ros.Executor
}
var control life
func status(err error) uint32 { if err==nil{return 0};if e,ok:=err.(ros.Error);ok{return e.Code};return 22 }
func (x *life) close(first uint32) uint32 {
	if x.closed {if first!=0{return first};return 9};x.closed=true
	// Executor owns registered subscriptions. A failed registration leaves the
	// subscription with us. Node/Session then close any other still-live owner.
	if x.executor!=nil {if code:=status(x.executor.Close());first==0{first=code}}
	if x.sub!=nil&&!x.registered {if code:=status(x.sub.Close());first==0{first=code}}
	if x.pub!=nil {if code:=status(x.pub.Close());first==0{first=code}}
	if x.node!=nil {for _,err:=range x.node.Close(){if first==0{first=status(err)}}}
	if x.session!=nil {for _,err:=range x.session.Close(){if first==0{first=status(err)}}}
	x.status=first;return first
}
func (x *life) begin(mode uint32) uint32 {
	if x.started||x.closed{return 9};if mode>1{return x.close(22)};x.started=true
	var err error;x.session,err=ros.NewSession(4);if err!=nil{return x.close(status(err))}
	x.node=x.session.NewNode("unitree-typed-example")
	x.executor,err=ros.NewCheckedExecutor(32768);if err!=nil{return x.close(status(err))};return 0
}
func (x *life) deny(err error) uint32 {code:=status(err);x.denied=code==13;if x.denied{return x.close(0)};return x.close(code)}
func (x *life) poll() uint32 {
	if !x.started||x.closed{return 9};if x.polls==4096{return x.close(110)};x.polls++
	n,err:=x.executor.SpinOnce();if err!=nil{return x.close(status(err))}
	if x.status!=0{return x.close(x.status)};if n==0{return 11};return 0
}
// ABI case_status()->u32 observes the preserved operation/cleanup status.
// Read it after any ambiguous scalar getter or callback pass. Zero is the
// initial/success value; it cannot replace the delivery count/native witness.
// This getter neither authorizes, polls nor consumes a native queue.
//export case_status
func caseStatus() uint32 {return control.status}
// ABI denied_verified()->u32 is1 ONLY after mode0 observed actual denial13.
// It is0 before that observation or in a positive case. The independent native
// observer must additionally prove zero new endpoints/payload; this flag alone
// cannot establish absence of effects. It grants no permission.
//export denied_verified
func deniedVerified() uint32 {if control.denied{return 1};return 0}
// ABI received_count()->u32 observes the number of validated complete values.
// Require successful initialization and a poll result0 first. Zero is a valid
// initial count and never proves delivery; this getter performs no native call.
//export received_count
func receivedCount() uint32 {return control.received}
// ABI close_case()->u32 retires every ordinarily acquired owner after the
// current poll/callback has unwound, attempting remaining closes after errors.
// Return0 means those closes succeeded; later close/poll/submit returns9.
// A denial case was already closed by initialize_case and therefore returns9.
// External ownership remains necessary for an unrecoverable trap/process kill.
//export close_case
func closeCase() uint32 {return control.close(0)}
