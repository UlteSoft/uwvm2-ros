/** @file
 * @brief Executable custom C++20 server factories with exact typed components.
 * Generated Server classes privately own independently granted advertise/respond
 * aliases. Intrinsic guards span decoding/callback/response, refuse nested poll
 * and close/revoke17, and reset on a C++ exception. Callback replacement retains
 * the same mutable functor via shared ownership. Raw numeric ABI escape is not
 * exposed. This entry drives <=200 passes using checked basic stdin/stdout only.
 */
#include <cstdio>
#include <cstring>
#if UWVM_BOUNDED_ROS_VERSION == 1
#include <roswasm/ros1.h>
#define BACKEND roswasmc_ros1_backend
#define ABI uwvm_ros1_abi_version
#elif UWVM_BOUNDED_ROS_VERSION == 2
#include <roswasm/ros2.h>
#define BACKEND roswasmc_ros2_backend
#define ABI uwvm_ros2_abi_version
#else
#error "Select exactly one custom backend"
#endif
#include "../generated-bounded-v2/bounded_interfaces.hpp"
#if UWVM_BOUNDED_ROS_VERSION == 1
using Echo=uwvm_custom::Ros1Uwvm_ros_bounded_v2EchoServer;
using Digest=uwvm_custom::Ros1Uwvm_ros_bounded_v2DigestServer;
using Envelope=uwvm_custom::Ros1Uwvm_ros_bounded_v2Envelope;
#else
using Echo=uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEchoServer;
using Digest=uwvm_custom::Ros2Uwvm_ros_bounded_v2ActionDigestServer;
using Envelope=uwvm_custom::Ros2Uwvm_ros_bounded_v2MsgEnvelope;
#endif
namespace {bool same(Envelope const& v){return v.label.size==14&&!std::memcmp(v.label.data,"bounded-native",14)&&v.samples.size==3&&v.samples.data[0]==3&&v.samples.data[1]==5&&v.samples.data[2]==8&&v.points.size==2&&v.points.data[0].value==-7&&v.points.data[0].enabled==1&&v.points.data[1].value==11&&v.points.data[1].enabled==0&&v.labels[0].size==5&&!std::memcmp(v.labels[0].data,"first",5)&&v.labels[1].size==6&&!std::memcmp(v.labels[1].data,"second",6);}uint32_t marker(char const* text){return std::fputs(text,stdout)<0||std::fflush(stdout)?107:0;}}
int main(){
    if(ABI()!=1)return 95;roswasmc_context context{BACKEND()};
    Echo echo{context,"bounded-service-events","bounded-service-replies"};uint32_t error{echo.error()};
    Digest action{context,"bounded-action-events","bounded-action-replies"};if(!error)error=action.error();
    unsigned service_calls{},goals{};bool valid{true};uint64_t pending_id{};Digest::Result pending_result{};
    echo.on_request([&](Echo::Request const& request){++service_calls;Echo::Response response{};valid=valid&&request.limit==32&&same(request.sample);response.accepted=valid?1:2;response.echoed=request.sample;return response;});
    action.on_goal([&](uint64_t id,Digest::Goal const& goal){++goals;valid=valid&&id&&same(goal.sample);Digest::Replies response{};if(!valid){response.kind=0;return response;}response.feedback.processed=3;response.feedback.phase.size=8;std::memcpy(response.feedback.phase.data,"accepted",8);response.result.accepted=1;response.result.sum=16;response.result.note.size=21;std::memcpy(response.result.note.data,"bounded-native-result",21);pending_id=id;pending_result=response.result;response.send_result=false;return response;});
    if(!error)error=marker("SDK_BOUNDED_SERVERS_READY\n");
    for(unsigned pass{};!error&&(service_calls!=1||goals!=1)&&pass!=200;++pass){
        if(std::getchar()!='R'){error=22;break;}
        if(!service_calls){auto code=echo.process_once();if(code!=11)error=code;}
        if(!error&&!goals){auto code=action.process_once();if(code!=11)error=code;if(!error&&goals){
            // The typed facade sends only feedback in this pass. Retain the
            // exact Goal/result and both role owners until native delivery E.
            error=marker("SDK_BOUNDED_FEEDBACK_SENT\n");if(!error&&std::getchar()!='E')error=22;
            if(!error)error=action.respond_result(pending_id,2,pending_result).error;
        }}
        if(!error&&!valid)error=22;
        if(!error&&(service_calls!=1||goals!=1))error=marker("SDK_BOUNDED_AGAIN\n");
    }
    if(!error&&(!valid||service_calls!=1||goals!=1))error=110;
    if(!error)error=marker("SDK_BOUNDED_REPLIES_SENT\n");
    // D follows independent native DELIVERED, before retiring either endpoint.
    if(!error&&std::getchar()!='D')error=22;
    std::array<uint32_t,2> statuses{};
    if(action.is_open()){auto close=action.close_checked(error,statuses);if(!error)error=close.cleanup_error;}
    if(echo.is_open()){auto close=echo.close_checked(error,statuses);if(!error)error=close.cleanup_error;}
    if(!error)error=marker("PASS_BOUNDED_CUSTOM_SERVER\n");return static_cast<int>(error);
}
