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

/** @file C++20 real typed Publisher/Subscription topic RPC reactor.
 * Complete generated API8 codecs, explicit caller-driven scheduling, no ROS
 * service abstraction, timer, network setup or inferred robot permissions.
 * The test-only native peer must use actual unitree_api message types and the
 * exact bounded identity/ack protocol documented in TYPED_EXAMPLES.md.
 */
#include <roswasm/ros2.h>
#include "bounded_records.hpp"
#include <memory>
#define EXPORT(name) extern "C" __attribute__((export_name(name)))
using Request=uwvm_custom::Ros2Unitree_apiMsgRequest;
using Response=uwvm_custom::Ros2Unitree_apiMsgResponse;
namespace {
roswasmc_context context{};
// Zero-initialized Guest-local owners avoid global C++ constructors/destructors.
// These pointers never cross an import/export or represent native addresses.
roswasmcpp::Publisher<Request>* publisher{};
roswasmcpp::Subscription<Response>* subscription{};
bool started{},closed{},denied{},submitted{};
uint32_t last_status{},polls{},received{},reply_code{};
int64_t request_id{},api_id{};
uint32_t close_all(uint32_t first=0) {
    if(closed)return first?first:9;closed=true;
    if(subscription&&subscription->handle()){auto code=subscription->close();if(!first)first=code;}
    if(publisher&&publisher->handle()){auto code=publisher->close();if(!first)first=code;}
    delete subscription;subscription=nullptr;delete publisher;publisher=nullptr;last_status=first;return first;
}
}
// ABI initialize_case(mode,arg0,arg1)->u32 is single-use. Mode0 proves actual
// default denial; mode1 requires the explicit positive aliases stated above.
// LowState args are (family1=GO or2=HG,reserved0); API args are (request_id,
// api_id), each1..0x7fffffff. Return0 is setup/denial success, never delivery.
// An error attempts cleanup; a fresh instance is required for another case.
EXPORT("initialize_case") uint32_t initialize_case(uint32_t mode,uint32_t id,uint32_t api) {
    if(started||closed)return 9;if(mode>1||!id||id>0x7fffffff||!api||api>0x7fffffff)return close_all(22);
    started=true;request_id=id;api_id=api;if(uwvm_ros2_abi_version()!=1)return close_all(95);
    context.backend=roswasmc_ros2_backend();
    publisher=new roswasmcpp::Publisher<Request>(context,"unitree-api-request");
    auto error=publisher->error();
    if(mode==0){denied=error==13&&!publisher->handle();return close_all(denied?0:error?error:22);}
    if(error)return close_all(error);
    subscription=new roswasmcpp::Subscription<Response>(context,"unitree-api-response",roswasmcpp::Codec<Response>::max_wire_bytes);
    error=subscription->error();if(error)return close_all(error);
    subscription->on_message([](Response const& value){
        constexpr char ack[]="typed-example-ack";
        if(!submitted||value.header.identity.id!=request_id||value.header.identity.api_id!=api_id||value.header.status.code||value.data.size!=sizeof ack-1||value.binary.size){last_status=22;return;}
        for(size_t i=0;i<sizeof ack-1;++i)if(value.data.data[i]!=static_cast<unsigned char>(ack[i])){last_status=22;return;}
        reply_code=static_cast<uint32_t>(value.header.status.code);++received;
    });return 0;
}
// ABI submit_once()->u32 sends one complete typed Request after successful
// positive initialization. It uses the configured ids, empty binary and "{}".
// Return0 means the SDK accepted publication; only the independent native
// peer and later matching Response prove transport. A repeated call returns9.
EXPORT("submit_once") uint32_t submit_once() {
    if(!started||closed||submitted)return 9;Request value{};value.header.identity.id=request_id;value.header.identity.api_id=api_id;
    value.parameter.size=2;value.parameter.data[0]='{';value.parameter.data[1]='}';
    auto error=publisher->publish(value);if(error)return close_all(error);submitted=true;return 0;
}
// ABI poll_once()->u32 performs one explicit copied SDK poll pass. API must
// have submitted first. Return11 means no new value;0 means a newly validated
// value, and nonzero errors retain their status and attempt cleanup. No hidden
// retry occurs; the4097th attempt closes with110. Inspect count before getters.
EXPORT("poll_once") uint32_t poll_once() {
    if(!started||closed||!submitted)return 9;if(polls++>=4096)return close_all(110);
    auto error=subscription->poll_once();if(error==11)return 11;if(error)return close_all(error);if(last_status)return close_all(last_status);return 0;
}
// ABI received_count()->u32 observes the number of validated complete values.
// Require successful initialization and a poll result0 first. Zero is a valid
// initial count and never proves delivery; this getter performs no native call.
EXPORT("received_count") uint32_t received_count(){return received;}
// ABI response_code()->u32 observes the accepted Response status after a
// successful poll and received_count>0. This fixture accepts status0 only;
// an initial zero is not an acknowledgement and invokes no native operation.
EXPORT("response_code") uint32_t response_code(){return reply_code;}
// ABI denied_verified()->u32 is1 ONLY after mode0 observed actual denial13.
// It is0 before that observation or in a positive case. The independent native
// observer must additionally prove zero new endpoints/payload; this flag alone
// cannot establish absence of effects. It grants no permission.
EXPORT("denied_verified") uint32_t denied_verified(){return denied;}
// ABI case_status()->u32 observes the preserved operation/cleanup status.
// Read it after any ambiguous scalar getter or callback pass. Zero is the
// initial/success value; it cannot replace the delivery count/native witness.
// This getter neither authorizes, polls nor consumes a native queue.
EXPORT("case_status") uint32_t case_status(){return last_status;}
// ABI close_case()->u32 retires every ordinarily acquired owner after the
// current poll/callback has unwound, attempting remaining closes after errors.
// Return0 means those closes succeeded; later close/poll/submit returns9.
// A denial case was already closed by initialize_case and therefore returns9.
// External ownership remains necessary for an unrecoverable trap/process kill.
EXPORT("close_case") uint32_t close_case(){return close_all();}
