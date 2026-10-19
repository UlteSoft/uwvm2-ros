/** @file
 * @brief Pure generated C++20 custom owner/callback/state regression.
 * Compile this source against the exact fresh SourceB bounded generation and
 * current SDK includes. Its fake backend records real generated facade calls;
 * it neither starts ROS/Wasm nor establishes native interoperability. Root owns
 * serial compilation/execution; source preparation does not assert model PASS.
 */
#include "../deployment-final/generated/bounded/bounded_interfaces.hpp"
#include "../../../sdk/tests/c/ownership_backend.h"
#include <cstdio>
static unsigned checks;
#define CHECK(value) do {++checks;if(!(value)){std::printf("FAIL bounded facade line=%u\n",unsigned(__LINE__));return 1;}} while(0)
using Echo=uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEchoServer;
using Empty=uwvm_custom::Ros2Uwvm_ros_bounded_v2SrvEmptyClient;
using Digest=uwvm_custom::Ros2Uwvm_ros_bounded_v2ActionDigestServer;
using Action=uwvm_custom::Ros2Uwvm_ros_bounded_v2ActionDigestClient;
static uint32_t empty_reply(uint64_t handle,uint32_t memory,uint64_t input,uint64_t n,uint64_t output,uint64_t capacity,uint64_t slot)
{(void)memory;(void)input;(void)n;(void)output;(void)capacity;if(handle>=128||!fixture_live[handle])return 9;++fixture_requests;fixture_store64(slot,0);return 0;}
/** Only pure test storage is mutated; payload64-bit IDs remain exact. */
static void event(uint64_t id,unsigned char const* bytes,size_t size)
{fixture_store64(uwvm_ros_address(fixture_payload),id);if(size)std::memcpy(fixture_payload+8,bytes,size);fixture_payload_size=8+size;}
int main()
{
    auto backend=fixture_backend();backend->request[ROSWASMC_SERVICE_RESPOND]=empty_reply;backend->request[ROSWASMC_ACTION_RESPOND]=empty_reply;
    roswasmc_context context{backend};Echo service{context,"advertise","respond"};CHECK(service.error()==0&&service.is_open());
    unsigned char wire[128];size_t length=0;Echo::Request request{};
    CHECK(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_encode(&request,wire,sizeof wire,&length)==0);event(42,wire,length);
    uint32_t nested=0,close=0,revoke=0;std::array<uint32_t,2> statuses{55,66};unsigned stateful=0;
    service.on_request([count=0u,&service,&nested,&close,&revoke,&statuses,&stateful](auto const& value)mutable{
        stateful=++count;nested=service.poll_once();close=service.close_checked(110,statuses).cleanup_error;revoke=service.revoke_checked(77,statuses).cleanup_error;
        Echo::Response out{};out.accepted=1;out.echoed=value.sample;return out;
    });auto polls=fixture_polls,closes=fixture_closes,requests=fixture_requests;
    CHECK(service.poll_once()==0&&service.poll_once()==0&&stateful==2);
    CHECK(nested==17&&close==17&&revoke==17&&statuses[0]==55&&statuses[1]==66);
    CHECK(fixture_polls==polls+2&&fixture_closes==closes&&fixture_requests==requests+2);
    unsigned replaced=0;auto token=std::make_shared<int>(1);std::weak_ptr<int> weak=token;bool alive=false;
    service.on_request([token,&service,&weak,&alive,&replaced](auto const&){service.on_request([&replaced](auto const&){++replaced;return Echo::Response{};});alive=!weak.expired();return Echo::Response{};});token.reset();
    CHECK(service.poll_once()==0&&alive&&weak.expired()&&replaced==0);CHECK(service.poll_once()==0&&replaced==1);
#if defined(__cpp_exceptions)
    service.on_request([](auto const&)->Echo::Response{throw 7;});bool caught=false;requests=fixture_requests;
    try{service.poll_once();}catch(int code){caught=code==7;}CHECK(caught&&fixture_requests==requests);
#endif
    auto cleanup=service.close_checked(0,statuses);CHECK(cleanup.attempted==2&&!cleanup.cleanup_error&&service.error()==9);
    CHECK(service.poll_once()==9&&service.respond(42,Echo::Response{}).error==9);
    Digest action{context,"action-events","action-replies"};CHECK(action.error()==0);
    Digest::Goal goal{};CHECK(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_encode(&goal,wire,sizeof wire,&length)==0);event(43,wire,length);
    stateful=0;action.on_goal([count=0u,&action,&nested,&statuses,&close,&stateful](uint64_t id,auto const&)mutable{stateful=++count;nested=action.poll_once();close=action.close_checked(0,statuses).cleanup_error;Digest::Replies reply{};reply.result.accepted=1;reply.result.sum=16;if(id!=43)reply.kind=0;return reply;});requests=fixture_requests;
    CHECK(action.poll_once()==0&&action.poll_once()==0&&stateful==2);CHECK(nested==17&&close==17&&fixture_requests==requests+4);
    action.on_goal([](auto,auto const&){Digest::Replies out{};out.result.accepted=2;return out;});requests=fixture_requests;
    CHECK(action.poll_once()==22&&fixture_requests==requests);cleanup=action.revoke_checked(13,statuses);CHECK(cleanup.primary_error==13&&cleanup.attempted==2&&action.error()==9);
    CHECK(action.respond_result(43,2,Digest::Result{}).error==9);
    backend->request[ROSWASMC_SERVICE_CALL]=empty_reply;Empty empty{context,"empty"};CHECK(empty.error()==0);Empty::Request rq{};Empty::Response rp{};CHECK(empty.call(rq,rp).error==0);
    CHECK(empty.close()==0&&empty.error()==9&&empty.call(rq,rp).error==9);
    roswasmc_action_aliases aliases{{"send",4},{"own",3},{"feedback",8},{"result",6},{nullptr,0}};
    Action client{context,aliases,1};CHECK(client.error()==0&&client.is_open());Action::Goal outgoing{};uint64_t id=99;fixture_payload_size=8;fixture_store64(uwvm_ros_address(fixture_payload),42);
    CHECK(client.send_goal(outgoing,id).error==0&&id==42&&client.owns(42));requests=fixture_requests;CHECK(client.send_goal(outgoing,id).error==90&&fixture_requests==requests);
    Action::Feedback feedback{};CHECK(client.feedback(999,feedback).error==9&&fixture_requests==requests);int accepted=55;CHECK(client.cancel_any(999,accepted).error==13&&accepted==55&&fixture_requests==requests);
    std::array<uint32_t,5> action_statuses{};cleanup=client.close_checked(0,action_statuses);CHECK(cleanup.attempted==4&&client.error()==9&&client.send_goal(outgoing,id).error==9);
    Echo denied{context,"denied","respond"};CHECK(denied.error()==13&&!denied.is_open()&&denied.construction_cleanup().primary_error==13);
    std::printf("PASS bounded typed facade checks=%u\n",checks);return 0;
}
