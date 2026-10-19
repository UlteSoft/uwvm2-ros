/** @file
 * @brief C++20 typed custom Service/Action owner execution.
 * Prepare the fresh generated-bounded-v2 products as documented in README.md.
 * Compile once per UWVM_BOUNDED_ROS_VERSION=1/2; std-WASI recipes use -fno-rtti
 * and -fno-exceptions where the installed libc++ requires them. Basic stdin R
 * releases at most200 caller-driven receives; checked E after actual feedback
 * delivery orders the independent terminal reply. The controller owns real time.
 * There is no automatic send/cancel retry and no implicit native capability.
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
#error "Select exactly one bounded custom backend"
#endif
#include "../generated-bounded-v2/bounded_interfaces.hpp"
#if UWVM_BOUNDED_ROS_VERSION == 1
using Echo=uwvm_custom::Ros1Uwvm_ros_bounded_v2EchoClient;
using Empty=uwvm_custom::Ros1Uwvm_ros_bounded_v2EmptyClient;
using Digest=uwvm_custom::Ros1Uwvm_ros_bounded_v2DigestClient;
using Envelope=uwvm_custom::Ros1Uwvm_ros_bounded_v2Envelope;
#else
using Echo=uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEchoClient;
using Empty=uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEmptyClient;
using Digest=uwvm_custom::Ros2Uwvm_ros_bounded_v2ActionDigestClient;
using Envelope=uwvm_custom::Ros2Uwvm_ros_bounded_v2MsgEnvelope;
#endif
namespace {
Envelope sample(){Envelope v{};v.label.size=14;std::memcpy(v.label.data,"bounded-native",14);v.samples.size=3;v.samples.data[0]=3;v.samples.data[1]=5;v.samples.data[2]=8;v.points.size=2;v.points.data[0].value=-7;v.points.data[0].enabled=1;v.points.data[1].value=11;v.labels[0].size=5;std::memcpy(v.labels[0].data,"first",5);v.labels[1].size=6;std::memcpy(v.labels[1].data,"second",6);return v;}
uint32_t marker(char const* text){return std::fputs(text,stdout)<0||std::fflush(stdout)?107:0;}
}
int main(){
    if(ABI()!=1)return 95;roswasmc_context context{BACKEND()};
    Echo echo{context,"bounded-echo"};if(echo.error())return static_cast<int>(echo.error());
    Empty empty{context,"bounded-empty"};if(empty.error()){auto first{empty.error()};auto cleanup{echo.close()};(void)cleanup;return static_cast<int>(first);}
    roswasmc_action_aliases aliases{uwvm_custom::alias("bounded-send"),uwvm_custom::alias("bounded-cancel"),uwvm_custom::alias("bounded-feedback"),uwvm_custom::alias("bounded-result"),{nullptr,0}};
    Digest action{context,aliases,8};uint32_t error{action.error()};
    Echo::Request request{};request.sample=sample();request.limit=32;Echo::Response response{};
    Empty::Request empty_request{};Empty::Response empty_response{};Digest::Goal goal{};goal.sample=sample();uint64_t id{};uint8_t kind{};Digest::Feedback feedback{};Digest::Result result{};bool got_feedback{},got_result{};
    if(!error)error=echo.call(request,response).error;
    if(!error){std::vector<unsigned char> expected,actual;auto oracle{sample()};if(!response.accepted||!roswasmcpp::Codec<Envelope>::encode(oracle,expected)||!roswasmcpp::Codec<Envelope>::encode(response.echoed,actual)||actual!=expected)error=22;}
    if(!error)error=empty.call(empty_request,empty_response).error;
    if(!error)error=action.send_goal(goal,id).error;
    if(!error)error=marker("SDK_BOUNDED_GOAL_SENT\n");
    for(unsigned pass{};!error&&!got_result&&pass!=200;++pass){
        if(std::getchar()!='R'){error=22;break;}
        if(!got_feedback){auto r=action.feedback(id,feedback);if(r.error==11){error=marker("SDK_BOUNDED_AGAIN\n");continue;}error=r.error;if(error)break;if(feedback.processed!=3||feedback.phase.size!=8||std::memcmp(feedback.phase.data,"accepted",8)){error=22;break;}got_feedback=true;error=marker("SDK_BOUNDED_FEEDBACK_DELIVERED\n");if(error)break;if(std::getchar()!='E'){error=22;break;}}
        auto r=action.result(id,kind,result);if(r.error==11){error=marker("SDK_BOUNDED_AGAIN\n");continue;}error=r.error;if(error)break;if(kind!=2||!result.accepted||result.sum!=16||result.note.size!=21||std::memcmp(result.note.data,"bounded-native-result",21)){error=22;break;}got_result=true;
    }
    if(!error&&(!got_feedback||!got_result))error=110;
    if(action.is_open()){std::array<uint32_t,5> statuses{};auto close=action.close_checked(error,statuses);if(!error)error=close.cleanup_error;}
    if(empty.is_open()){auto close=empty.close();if(!error)error=close;}
    if(echo.is_open()){auto close=echo.close();if(!error)error=close;}
    if(!error)error=marker("PASS_BOUNDED_CUSTOM_CLIENT\n");return static_cast<int>(error);
}
