/** @file @brief Bounded action identity translation and complete untrusted event decoding. */
#define main action_base_fixture_entry
#include "communication_actions.cpp"
#undef main
#include <uwvm2/imported/ros/scene/communication_action_correlations.h>
#include <barrier>
#include <thread>
#include <sys/wait.h>
namespace worker=uwvm2::imported::ros::scene::communication_worker;
namespace wire=uwvm2::imported::ros::scene::wire;
using mapping=worker::action_correlations;
static c::handle fresh(){auto id=uwvm2::imported::ros::resources::allocate_identity();assert(id);return id;}
static worker::action_record record(a::event_kind kind,c::handle goal,c::handle request,a::uuid uuid)
{worker::action_record value;value.information.source=9001;value.information.generation=71;value.information.sequence=1;value.native_goal=goal;value.native_request=request;value.kind=kind;value.identity=uuid;return value;}
static c::native_result accepted(c::handle request){return {c::error::ok,c::effect::locally_accepted,request,71,false};}
static void clients()
{
    mapping rows;assert(rows.configure({{1,1,1},81,9001,71,a::role::client,1,1})==c::error::ok);
    int instance{},foreign{};c::delivery_consumer who{&instance,1,11},other{&foreign,1,11};a::uuid uuid{1};auto goal=fresh();
    assert(rows.begin_submit(who,goal,uuid)==c::error::ok&&rows.begin_submit(who,goal,uuid)==c::error::ok&&rows.occupied());
    assert(rows.begin_submit(other,goal,uuid)==c::error::stale&&rows.release_terminal(goal)==c::error::busy);
    assert(rows.submitted(who,goal,accepted(goal),7001)==c::error::ok&&!rows.occupied());
    auto input=record(a::event_kind::acceptance,7001,7001,uuid);mapping::translated actual;
    assert(rows.event(who,input,actual)==c::error::ok&&actual.goal==goal&&actual.request==goal&&actual.native_goal==7001);
    ++input.native_goal;assert(rows.event(who,input,actual)==c::error::denied);--input.native_goal;
    ++input.native_request;assert(rows.event(who,input,actual)==c::error::denied);--input.native_request;
    ++input.identity[1];assert(rows.event(who,input,actual)==c::error::denied);--input.identity[1];
    assert(rows.event(other,input,actual)==c::error::denied);
    auto cancelled=fresh();assert(rows.begin_cancel(who,goal,cancelled,actual)==c::error::ok&&actual.native_goal==7001&&actual.request==cancelled);
    assert(rows.cancelled(who,goal,{c::error::denied,c::effect::none,cancelled,71,false},0)==c::error::ok&&!rows.occupied());
    auto retry=fresh();assert(rows.begin_cancel(who,goal,retry,actual)==c::error::ok);
    assert(rows.cancelled(who,goal,accepted(retry),7002)==c::error::ok&&!rows.occupied());
    input.kind=a::event_kind::cancel_response;input.native_request=7002;
    assert(rows.event(who,input,actual)==c::error::ok&&actual.request==retry&&actual.request!=goal);
    assert(rows.begin_cancel(who,goal,fresh(),actual)==c::error::again);
    assert(rows.release_terminal(goal)==c::error::ok);
    assert(rows.event(who,input,actual)==c::error::denied);
    auto newgoal=fresh();assert(rows.begin_submit(who,newgoal,uuid)==c::error::ok);
    assert(rows.submitted(who,newgoal,{c::error::ok,c::effect::locally_accepted,newgoal,72,false},7003)==c::error::io&&rows.occupied());
    assert(rows.release_terminal(newgoal)==c::error::busy&&rows.abandon_submit(who,newgoal)==c::error::denied);
}
static void servers()
{
    mapping rows;assert(rows.configure({{1,1,1},81,9001,71,a::role::server,2,1})==c::error::ok);
    int instance{};c::delivery_consumer who{&instance,1,11};a::uuid uuid{2};mapping::translated actual,retry;
    auto input=record(a::event_kind::proposal,7101,7102,uuid);
    assert(rows.proposal(who,input,actual)==c::error::ok&&actual.goal!=actual.request&&actual.goal!=7101&&actual.request!=7102);
    assert(rows.proposal(who,input,retry)==c::error::ok&&retry.goal==actual.goal&&retry.request==actual.request);
    auto goal=actual.goal,proposal=actual.request;
    auto operation=fresh();assert(rows.begin_response(who,goal,goal,operation,false,retry)==c::error::denied);
    assert(rows.begin_response(who,goal,proposal,operation,false,retry)==c::error::ok&&retry.native_goal==7101&&retry.native_request==7102);
    assert(rows.release_terminal(goal)==c::error::busy);
    assert(rows.responded(who,goal,{c::error::denied,c::effect::none,operation,71,false})==c::error::ok);
    input.kind=a::event_kind::cancel_request;input.native_request=7103;
    assert(rows.cancellation(who,input,actual)==c::error::ok&&actual.request!=proposal&&actual.goal==goal);
    auto cancellation=actual.request;
    assert(rows.cancellation(who,input,retry)==c::error::ok&&retry.request==cancellation);
    ++input.native_request;assert(rows.cancellation(who,input,retry)==c::error::again);--input.native_request;
    operation=fresh();assert(rows.begin_response(who,goal,cancellation,operation,true,retry)==c::error::ok&&retry.native_request==7103);
    assert(rows.responded(who,goal,accepted(operation))==c::error::ok);
    input=record(a::event_kind::proposal,7201,7103,a::uuid{3});
    assert(rows.proposal(who,input,retry)==c::error::stale);input.native_request=7202;
    assert(rows.proposal(who,input,retry)==c::error::ok);
    assert(rows.release_terminal(goal)==c::error::ok);
    auto child=::fork();assert(child>=0);
    if(!child){assert(rows.occupied()&&rows.release_terminal(retry.goal)==c::error::denied);::_exit(0);}
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
}
static void framing()
{
    auto policy=plan(a::role::server);std::array<std::uint8_t,2048> bytes;std::array<std::uint8_t,1> body{7};a::delivery delivery;
    delivery.information.source=9001;delivery.information.generation=71;delivery.information.sequence=3;delivery.goal=7101;delivery.request=7102;
    delivery.kind=a::event_kind::proposal;delivery.observed=a::state::pending;delivery.body=body;delivery.goal_uuid[0]=7;
    auto encode=[&]{wire::writer out{bytes};assert(uwvm2::imported::ros::action_contract::encode_delivery(out,delivery,policy));return out.size();};
    auto length=encode();worker::action_record output;
    for(std::size_t size{};size<length;++size)assert(!worker::decode_action_record({bytes.data(),size},policy,9001,71,output));
    assert(worker::decode_action_record({bytes.data(),length},policy,9001,71,output)&&output.native_goal==7101&&output.native_request==7102&&output.payload[0]==7);
    assert(!worker::decode_action_record({bytes.data(),length},policy,9002,71,output));
    assert(!worker::decode_action_record({bytes.data(),length},policy,9001,72,output));
    policy.direction=a::role::client;assert(!worker::decode_action_record({bytes.data(),length},policy,9001,71,output));policy.direction=a::role::server;
    policy.goal.type_identity="test/Foreign";assert(!worker::decode_action_record({bytes.data(),length},policy,9001,71,output));policy=plan(a::role::server);
    delivery.observed=a::state::accepted;length=encode();assert(!worker::decode_action_record({bytes.data(),length},policy,9001,71,output));
    delivery.observed=a::state::pending;delivery.goal_uuid={};length=encode();assert(!worker::decode_action_record({bytes.data(),length},policy,9001,71,output));
}
static void concurrency()
{
    mapping rows;assert(!mapping::reservation_bytes(0,1)&&!mapping::reservation_bytes(65536,65536));
    assert(rows.configure({{1,1,1},81,9001,71,a::role::client,1,0})==c::error::ok);
    int instance{};c::delivery_consumer who{&instance,1,11};auto goal=fresh();std::barrier start{3};std::array<c::error,2> outcomes;
    std::thread first{[&]{start.arrive_and_wait();outcomes[0]=rows.begin_submit(who,goal,a::uuid{1});}};
    std::thread second{[&]{start.arrive_and_wait();outcomes[1]=rows.begin_submit(who,goal,a::uuid{1});}};
    start.arrive_and_wait();first.join();second.join();assert(outcomes[0]==c::error::ok&&outcomes[1]==c::error::ok);
    assert(rows.submitted(who,goal,accepted(goal),7001)==c::error::ok);mapping::translated translated;
    assert(rows.begin_cancel(who,goal,fresh(),translated)==c::error::again);
    assert(rows.release_terminal(goal)==c::error::ok&&!rows.occupied());
}
int main()
{
    clients();servers();framing();concurrency();
    std::puts("PASS bounded action correlations: distinct host/native goal and cancellation IDs, exact consumer and UUID, fixed capacity, failed/unknown mutations, strict full framing, retained retries, concurrency and fork");
}
