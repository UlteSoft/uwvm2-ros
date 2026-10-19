//go:build tinygo && (wasm || tinygo.wasm) && ros2 && !ros1 && unitree_topicrpc && !unitree_lowstate

// Typed Unitree example source contract (comments only; executable code below).
// Source-stage contract: copy the selected complete generated family module and
// the actual Guest SDK into fresh owned work as specified by
// sdk/robots/unitree/recipes/closedtyped-example-recipes.json. Never substitute
// a projected schema, fake native identity, mock bridge or generated source stub.
// For exact build/staging and independent native oracles, read TYPED_EXAMPLES.md.
// Role: complete unitree_api Request/Response topic RPC, NOT a ROS service or
// firmware API call. Mode0 opens only unitree-api-request and expects denial.
// Mode1 requires that alias as topic.publish and unitree-api-response as
// topic.subscribe, each with the actual corresponding native message identity.
// After both endpoints match, submit_once publishes exactly one full Request
// with explicit positive int64-compatible ids, parameter="{}" and empty binary.
// The independent SIM fixture must reply with those same ids, status0, exact
// data="typed-example-ack" and empty binary. No physical endpoint is selected.
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

// Test-only topic RPC. The independent native peer supplies a real typed
// Response; no service or robot firmware endpoint is called by this example.
var requestID,apiID int64
var submitted bool
var responseCode uint32

// ABI initialize_case(mode,arg0,arg1)->u32 is single-use. Mode0 proves actual
// default denial; mode1 requires the explicit positive aliases stated above.
// LowState args are (family1=GO or2=HG,reserved0); API args are (request_id,
// api_id), each1..0x7fffffff. Return0 is setup/denial success, never delivery.
// An error attempts cleanup; a fresh instance is required for another case.
//export initialize_case
func initializeCase(mode,id,api uint32) uint32 {
	if id==0||id>0x7fffffff||api==0||api>0x7fffffff{return control.close(22)}
	if code:=control.begin(mode);code!=0{return code};requestID=int64(id);apiID=int64(api)
	var err error;control.pub,err=control.node.CreatePublisher("unitree-api-request")
	if err!=nil{if mode==0{return control.deny(err)};return control.close(status(err))};if mode==0{return control.close(22)}
	control.sub,err=control.node.CreateSubscription("unitree-api-response");if err!=nil{return control.close(status(err))}
	err=control.executor.TryAddSubscription(control.sub,func(bytes []byte){
		value,err:=vendor.DecodeRos2Unitree_apiMsgResponse(bytes)
		if err!=nil||!submitted||value.Header.Identity.Id!=requestID||value.Header.Identity.Api_id!=apiID||value.Header.Status.Code!=0||value.Data!="typed-example-ack"||len(value.Binary)!=0{control.status=22;return}
		responseCode=uint32(value.Header.Status.Code);control.received++
	});if err!=nil{return control.close(status(err))};control.registered=true;return 0
}
// ABI submit_once()->u32 sends one complete typed Request after successful
// positive initialization. It uses the configured ids, empty binary and "{}".
// Return0 means the SDK accepted publication; only the independent native
// peer and later matching Response prove transport. A repeated call returns9.
//export submit_once
func submitOnce() uint32 {
	if !control.started||control.closed||submitted{return 9}
	var value vendor.Ros2Unitree_apiMsgRequest;value.Header.Identity.Id=requestID;value.Header.Identity.Api_id=apiID;value.Parameter="{}"
	if err:=control.pub.PublishMessage(value);err!=nil{return control.close(status(err))};submitted=true;return 0
}
// ABI poll_once()->u32 performs one explicit copied SDK poll pass. API must
// have submitted first. Return11 means no new value;0 means a newly validated
// value, and nonzero errors retain their status and attempt cleanup. No hidden
// retry occurs; the4097th attempt closes with110. Inspect count before getters.
//export poll_once
func pollOnce() uint32 {if !submitted{return 9};return control.poll()}
// ABI response_code()->u32 observes the accepted Response status after a
// successful poll and received_count>0. This fixture accepts status0 only;
// an initial zero is not an acknowledgement and invokes no native operation.
//export response_code
func responseCodeValue() uint32 {return responseCode}
func main() {}
