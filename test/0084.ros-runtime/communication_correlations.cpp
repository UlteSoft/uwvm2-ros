/** @file @brief Closed service request translation without native handle disclosure. */
#include <uwvm2/imported/ros/scene/communication_correlations.h>
#include <cassert>
#include <cstdio>
#include <thread>
#include <barrier>
#include <sys/wait.h>
namespace s=uwvm2::imported::ros::scene;
namespace c=s::communication;
using table=s::communication_worker::correlations;
using error=c::error;
using reference=s::communication_worker::receive_wire::reference;
struct fixture
{
    table records;int instance{};c::delivery_consumer who{&instance,1,41};reference ref{51,71,3,9};
    explicit fixture(table::role role,std::uint64_t capacity=2)
    {assert(records.configure({{1,2,3},51,61,71,role,capacity})==error::ok);}
    c::receipt receipt(std::uint64_t key,c::error status=error::ok,c::effect effect=c::effect::locally_accepted)
    {return {uwvm2::imported::ros::resources::allocate_identity(),71,{51,key},status,effect};}
    c::handle reserve_server(std::uint64_t native=81)
    {c::handle result{};assert(records.reserve_received(who,ref,61,native,result)==error::ok&&result);return result;}
    c::handle client(std::uint64_t native=81)
    {
        auto request=uwvm2::imported::ros::resources::allocate_identity();assert(records.reserve_client(who,request)==error::ok);
        assert(records.submitted(who,request,{error::ok,c::effect::locally_accepted,request,71,false},native)==error::ok);return request;
    }
};
static void server_translation()
{
    fixture f{table::role::server,1};auto id=f.reserve_server();c::handle duplicate{},native{};
    assert(f.records.reserve_received(f.who,f.ref,61,81,duplicate)==error::ok&&duplicate==id);
    assert(f.records.response_target(f.who,id,native)==error::denied&&f.records.release(f.who,id)==error::busy);
    auto release=f.receipt(1);assert(f.records.settled(f.who,id,f.ref,false,release)==error::ok);
    assert(f.records.settled(f.who,id,f.ref,false,release)==error::ok&&f.records.response_target(f.who,id,native)==error::denied);
    ++f.ref.nonce;auto next=f.reserve_server();assert(next!=id);auto ack=f.receipt(2);
    assert(f.records.settled(f.who,next,f.ref,true,ack)==error::ok&&f.records.settled(f.who,next,f.ref,true,ack)==error::ok);
    assert(f.records.response_target(f.who,next,native)==error::ok&&native==81);
    auto request=uwvm2::imported::ros::resources::allocate_identity();assert(f.records.response_pending(f.who,next,request)==error::ok);
    assert(f.records.response_target(f.who,next,native)==error::denied);
    assert(f.records.responded(f.who,next,{error::denied,c::effect::none,request,71,false})==error::ok);
    assert(f.records.response_target(f.who,next,native)==error::ok);
    ++request;assert(f.records.response_pending(f.who,next,request)==error::ok);
    assert(f.records.responded(f.who,next,{error::ok,c::effect::locally_accepted,request,71,false})==error::ok);
    assert(f.records.release(f.who,next)==error::ok);
    assert(f.records.response_target(f.who,next,native)==error::denied);
}
static void client_translation()
{
    fixture f{table::role::client,1};auto id=f.client();c::handle translated{};
    assert(f.records.reserve_received(f.who,f.ref,61,82,translated)==error::denied);
    assert(f.records.reserve_received(f.who,f.ref,61,81,translated)==error::ok&&translated==id);
    auto refused=f.receipt(1,error::timeout,c::effect::none);assert(f.records.settled(f.who,id,f.ref,true,refused)==error::ok);
    table::view view;assert(f.records.inspect(f.who,id,view)==error::ok&&view.state==table::phase::borrowed);
    auto release=f.receipt(2);assert(f.records.settled(f.who,id,f.ref,false,release)==error::ok);
    ++f.ref.nonce;assert(f.records.reserve_received(f.who,f.ref,61,81,translated)==error::ok&&translated==id);
    auto ack=f.receipt(3);assert(f.records.settled(f.who,id,f.ref,true,ack)==error::ok);
    auto future=uwvm2::imported::ros::resources::allocate_identity();assert(f.records.reserve_client(f.who,future)==error::again);
    assert(f.records.release(f.who,id)==error::ok&&f.records.reserve_client(f.who,id)==error::stale);
    assert(f.records.reserve_client(f.who,future)==error::ok&&f.records.abandon_client(f.who,future)==error::ok);
    assert(f.records.reserve_client(f.who,future)==error::stale);
}
static void hostile_identities_and_uncertainty()
{
    fixture f{table::role::server};auto id=f.reserve_server();auto other=f.who;++other.capability;c::handle translated{};
    assert(f.records.reserve_received(other,f.ref,61,81,translated)==error::busy);
    auto forged=f.ref;++forged.provider_generation;assert(f.records.reserve_received(f.who,forged,61,81,translated)==error::denied);
    assert(f.records.reserve_received(f.who,f.ref,62,81,translated)==error::denied);
    assert(f.records.reserve_received(f.who,f.ref,61,82,translated)==error::busy);
    auto ack=f.receipt(1);assert(f.records.settled(other,id,f.ref,true,ack)==error::denied);
    auto wrong=ack;++wrong.provider_generation;assert(f.records.settled(f.who,id,f.ref,true,wrong)==error::denied);
    wrong=ack;++wrong.key.sequence;assert(f.records.settled(f.who,id,f.ref,true,wrong)==error::stale);
    ack.observed=c::effect::unknown;ack.status=error::io;
    assert(f.records.settled(f.who,id,f.ref,true,ack)==error::ok&&f.records.release(f.who,id)==error::busy&&f.records.response_target(f.who,id,translated)==error::denied);
    assert(f.records.settled(f.who,id,f.ref,true,ack)==error::ok);
    table::view view;assert(f.records.inspect(f.who,id,view)==error::ok&&view.state==table::phase::uncertain);
    for(unsigned fault{};fault<4;++fault)
    {
        fixture invalid{table::role::client,1};auto request=uwvm2::imported::ros::resources::allocate_identity();assert(invalid.records.reserve_client(invalid.who,request)==error::ok);
        c::native_result result{error::ok,c::effect::locally_accepted,request,71,false};
        if(fault==0)result.request_identity=0;
        if(fault==1)++result.provider_generation;
        if(fault==2)result.deferred=true;
        if(fault==3)result.observed=c::effect::none;
        assert(invalid.records.submitted(invalid.who,request,result,81)==error::io);
        assert(invalid.records.release(invalid.who,request)==error::busy);
        auto next=uwvm2::imported::ros::resources::allocate_identity();assert(invalid.records.reserve_client(invalid.who,next)==error::again);
    }
}
static void concurrency_and_fork()
{
    fixture f{table::role::server,1};std::array<c::handle,8> ids{};std::array<std::thread,8> threads;std::barrier start{8};
    for(std::size_t i{};i<threads.size();++i)threads[i]=std::thread([&,i]{start.arrive_and_wait();assert(f.records.reserve_received(f.who,f.ref,61,81,ids[i])==error::ok);});
    for(auto& thread:threads)thread.join();
    for(auto id:ids)assert(id&&id==ids[0]);
    auto pid=::fork();assert(pid>=0);if(!pid){c::handle id{};assert(f.records.reserve_received(f.who,f.ref,61,81,id)==error::denied);f.records.retire();::_exit(0);}
    int status{};assert(::waitpid(pid,&status,0)==pid&&WIFEXITED(status)&&!WEXITSTATUS(status));
    f.records.retire();c::handle id{};assert(f.records.reserve_received(f.who,f.ref,61,81,id)==error::denied);
    auto release=f.receipt(1);assert(f.records.settled(f.who,ids[0],f.ref,false,release)==error::ok);
}
static void local_interest_cancellation()
{
    assert(table::reservation_bytes(1)>0&&!table::reservation_bytes(0)&&!table::reservation_bytes(65537));
    fixture f{table::role::client,1};assert(!f.records.occupied());auto id=f.client();assert(f.records.occupied());
    auto request=uwvm2::imported::ros::resources::allocate_identity();c::handle native{};
    auto foreign=f.who;++foreign.capability;
    assert(f.records.abandon_pending(foreign,id,request,native)==error::denied);
    assert(f.records.abandon_pending(f.who,id,request,native)==error::ok&&native==81);
    assert(f.records.reserve_received(f.who,f.ref,61,81,native)==error::denied&&f.records.release(f.who,id)==error::busy);
    assert(f.records.abandoned(f.who,id,{error::again,c::effect::none,request,71,false})==error::ok);
    assert(f.records.reserve_received(f.who,f.ref,61,81,native)==error::ok);
    assert(f.records.abandon_pending(f.who,id,request,native)==error::busy);
    auto release=f.receipt(1);assert(f.records.settled(f.who,id,f.ref,false,release)==error::ok);
    assert(f.records.abandon_pending(f.who,id,request,native)==error::ok);
    assert(f.records.abandoned(f.who,id,{error::ok,c::effect::locally_accepted,request,71,false})==error::ok&&f.records.release(f.who,id)==error::ok&&!f.records.occupied());
    for(unsigned fault{};fault!=5;++fault)
    {
        fixture invalid{table::role::client,1};auto next=invalid.client();request=uwvm2::imported::ros::resources::allocate_identity();
        assert(invalid.records.abandon_pending(invalid.who,next,request,native)==error::ok);
        c::native_result result{error::ok,c::effect::locally_accepted,request,71,false};
        if(!fault)result.observed=c::effect::observed_remote;
        if(fault==1)result.observed=c::effect::unknown;
        if(fault==2)result.request_identity=0;
        if(fault==3)++result.provider_generation;
        if(fault==4)result.deferred=true;
        assert(invalid.records.abandoned(invalid.who,next,result)==error::io&&invalid.records.occupied()&&invalid.records.release(invalid.who,next)==error::busy);
    }
}
int main()
{
    server_translation();client_translation();hostile_identities_and_uncertainty();concurrency_and_fork();local_interest_cancellation();
    std::puts("PASS bounded service correlations: host/native identity separation, server ACK before response, client original submission, release/reborrow, finite capacity, exact settlements, unknown retention, concurrent reservation and fork");
}
