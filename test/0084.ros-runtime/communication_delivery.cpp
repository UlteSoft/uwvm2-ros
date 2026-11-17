/** @file @brief Exact two-phase receive with concurrent producer and failed guest sinks. */
#define main existing_communication_core_tests
#include "communication_core.cpp"
#undef main
#include <uwvm2/imported/ros/scene/communication_receive_contract.h>
#include <uwvm2/imported/ros/scene/communication_receive_reply.h>
#include <uwvm2/imported/ros/scene/authority.h>
#include <cassert>
#include <cstdio>
#include <thread>
#include <barrier>
#include <sys/wait.h>
namespace c=uwvm2::imported::ros::scene::communication;
using error=c::error;
static void receive_carriers()
{
    namespace contract=uwvm2::imported::ros::communication_receive_contract;
    namespace wire=uwvm2::imported::ros::scene::wire;
    namespace schema=uwvm2::imported::ros::scene::schema_contract;
    std::array<std::uint8_t,1024> storage;
    for(auto const& descriptor:contract::operations)
    {
        wire::writer out{storage};
        if(contract::reserves(descriptor.id)){assert(out.u64(51)&&out.u64(200));}
        else{for(auto word:{51u,1u,51u,71u,1u,9u})assert(out.u64(word));}
        auto shape=schema::find(descriptor.name);assert(shape&&schema::validate(out.written(),shape->request,{1024,100,20,10}));
        namespace scene=uwvm2::imported::ros::scene;
        auto purpose=descriptor.name.starts_with("topic.")?scene::purpose::topic:scene::purpose::service;
        assert(scene::permits(purpose,descriptor.name)&&!scene::permits(scene::purpose::device,descriptor.name)&&
            !scene::permits(purpose==scene::purpose::topic?scene::purpose::service:scene::purpose::topic,descriptor.name));
        contract::request request;assert(contract::decode(descriptor.id,out.written(),request)&&request.reservation.endpoint==51);
        for(std::size_t i{};i<out.size();++i)assert(!contract::decode(descriptor.id,out.written().first(i),request));
        storage[out.size()]=0;assert(!contract::decode(descriptor.id,{storage.data(),out.size()+1},request));
        c::metadata info;info.source=51;info.generation=91;info.sequence=1;
        std::array<std::uint8_t,1> payload{7};wire::writer reply{storage};
        bool service=descriptor.name.starts_with("service.");
        assert(contract::encode(reply,{51,71,1,9},200,info,service?42:0,service?"service.request":"topic.message",{"test/Bytes","canonical",payload}));
        auto reservation=schema::find(service?"service.reserve":"topic.reserve");
        assert(reservation&&schema::validate(reply.written(),reservation->result,{1024,100,20,10}));
        // The wire's real worker generation is independent from the local
        // queue generation; no fallback silently substitutes the latter.
        wire::reader input{reply.written()};std::uint64_t endpoint{},generation{};
        assert(input.u64(endpoint)&&endpoint==51&&input.u64(generation)&&generation==71);
        wire::writer invalid{storage};assert(!contract::encode(invalid,{51,0,1,9},200,info,0,"topic.message",{"test/Bytes","canonical",payload}));
    }
}
static void hostile_worker_replies()
{
    namespace worker=uwvm2::imported::ros::scene::communication_worker;
    namespace contract=uwvm2::imported::ros::communication_receive_contract;
    namespace wire=uwvm2::imported::ros::scene::wire;
    for(auto direction:{worker::receive_direction::topic,worker::receive_direction::service_request,worker::receive_direction::service_response})
    {
        worker::receive_schema schema{51,71,8,direction,"test/Bytes","canonical",bytes_valid};
        c::metadata metadata;metadata.source=51;metadata.generation=71;metadata.sequence=3;
        bool topic=direction==worker::receive_direction::topic;
        auto kind=topic?"topic.message":direction==worker::receive_direction::service_request?"service.request":"service.response";
        std::array<std::uint8_t,2> payload{7,8};std::array<std::uint8_t,512> data;
        wire::writer encoded{data};assert(contract::encode(encoded,{51,71,3,9},200,metadata,topic?0:42,kind,{schema.type,schema.encoding,payload}));
        auto body=encoded.written();worker::received_record result;
        assert(worker::decode_received(body,schema,200,result)&&result.native_reference.nonce==9&&result.payload.size()==2&&result.payload[1]==8);
        for(std::size_t i{};i<body.size();++i)assert(!worker::decode_received(body.first(i),schema,200,result));
        for(std::size_t at:{0,8,16,24,32,40,48,56})
        {
            auto changed=data;std::fill_n(changed.begin()+at,8,0);
            assert(!worker::decode_received({changed.data(),body.size()},schema,200,result));
        }
        for(std::size_t at:{64,72})
        {auto changed=data;changed[at]=1;assert(!worker::decode_received({changed.data(),body.size()},schema,200,result));}
        auto changed=data;changed[96]=topic?1:0;assert(!worker::decode_received({changed.data(),body.size()},schema,200,result));
        changed=data;changed[body.size()-2]=255;assert(!worker::decode_received({changed.data(),body.size()},schema,200,result));
        auto wrong=schema;++wrong.provider_generation;assert(!worker::decode_received(body,wrong,200,result));
        wrong=schema;wrong.direction=topic?worker::receive_direction::service_request:worker::receive_direction::topic;
        assert(!worker::decode_received(body,wrong,200,result));
        wrong=schema;wrong.type="test/Other";assert(!worker::decode_received(body,wrong,200,result));
        wrong=schema;wrong.payload_bytes=1;assert(!worker::decode_received(body,wrong,200,result));
        assert(!worker::decode_received(body,schema,201,result));
        data[body.size()]=0;assert(!worker::decode_received({data.data(),body.size()+1},schema,200,result));
        // Every refusal leaves the previously validated view unchanged.
        assert(result.native_reference.nonce==9&&result.native_metadata.source==51&&result.payload.size()==2);
    }
    for(auto status:{error::ok,error::io,error::timeout})for(auto observed:{c::effect::none,c::effect::locally_accepted,c::effect::unknown})
    {
        std::array<std::uint8_t,160> data;wire::writer out{data};
        assert(uwvm2::imported::ros::communication_contract::encode_receipt(out,{42,71,{},status,observed}));
        std::uint64_t native{};assert(worker::decode_receive_settlement(out.written(),71,status,observed,native)&&native==42);
        assert(!worker::decode_receive_settlement(out.written(),72,status,observed,native));
        assert(!worker::decode_receive_settlement(out.written(),71,static_cast<error>(999),observed,native));
        for(std::size_t i{};i<out.size();++i)assert(!worker::decode_receive_settlement(out.written().first(i),71,status,observed,native));
    }
}
struct fixture
{
    c::receive_queue queue{2,8,c::overflow::drop_oldest};int instance{};
    c::delivery_consumer consumer{&instance,1,91};c::delivery_reference reference;
    c::delivery_receipt receipt;std::array<std::uint8_t,8> first{1},second{2},third{3};unsigned copies{};
    fixture(){assert(queue.bind(51,71)==error::ok);push(first);push(second);}
    void push(std::span<std::uint8_t const> body){c::metadata info;info.source=51;info.generation=71;assert(queue.push(info,body)==error::ok);}
    error reserve(bool accept=true)
    {return queue.reserve(consumer,200,100,[&](auto selected,auto deadline,c::delivery const& value)
        {++copies;reference=selected;assert(deadline==200&&value.body[0]==1&&value.information.sequence==selected.sequence);return accept;});}
};
static void lost_output_and_exact_settlement()
{
    fixture f;assert(f.reserve(false)==error::io&&f.queue.has_reservation());auto first=f.reference;
    auto reject=[](auto,auto,auto const&)->bool{throw 1;};assert(f.queue.reserve(f.consumer,200,100,reject)==error::io);
    assert(f.reserve()==error::ok&&f.reference==first&&f.copies==2);
    assert(f.queue.take([](auto const&){assert(false);return true;})==error::again);
    auto other=f.consumer;++other.generation;
    assert(f.queue.reserve(other,200,100,[](auto,auto,auto const&){assert(false);return true;})==error::busy);
    assert(f.queue.reserve(f.consumer,201,100,[](auto,auto,auto const&){assert(false);return true;})==error::stale);
    std::uint64_t revision{};bool ready{},terminal{};f.queue.observe(revision,ready,terminal);
    assert(revision&&!ready&&!terminal); // An already borrowed head must not keep the waitset spinning.
    auto forged=first;++forged.nonce;
    assert(f.queue.settle(other,first,{51,1},true,100,f.receipt)==error::denied);
    assert(f.queue.settle(f.consumer,forged,{51,1},true,100,f.receipt)==error::denied);
    assert(f.queue.settle(f.consumer,first,{51,2},true,100,f.receipt)==error::stale);
    assert(f.queue.settle(f.consumer,first,{51,1},true,100,f.receipt)==error::ok&&!f.queue.has_reservation());auto id=f.receipt.request;
    assert(id&&f.receipt.source_generation==71&&f.receipt.observed==c::effect::locally_accepted);
    assert(f.queue.settle(f.consumer,first,{51,1},true,100,f.receipt)==error::ok&&f.receipt.request==id);
    assert(f.queue.settle(f.consumer,first,{51,1},false,100,f.receipt)==error::stale);
    assert(f.queue.take([&](auto const& value){return value.body[0]==2;})==error::ok);
    assert(f.queue.take([](auto const&){assert(false);return true;})==error::again);
}
static void release_deadline_and_overflow()
{
    fixture f;assert(f.reserve()==error::ok);auto first=f.reference;
    c::metadata info;info.source=51;info.generation=71;
    assert(f.queue.push(info,f.third)==error::again&&f.reserve()==error::ok&&f.reference==first);
    assert(f.queue.settle(f.consumer,first,{51,1},true,200,f.receipt)==error::timeout&&f.queue.has_reservation());
    assert(f.queue.reserve(f.consumer,200,201,[](auto,auto,auto const&){assert(false);return true;})==error::timeout);
    // Cleanup does not require an unexpired deadline and never consumes data.
    assert(f.queue.settle(f.consumer,first,{51,1},false,201,f.receipt)==error::ok&&!f.queue.has_reservation());
    std::uint64_t revision{};bool ready{},terminal{};f.queue.observe(revision,ready,terminal);assert(ready&&!terminal);
    c::delivery_reference next;
    assert(f.queue.reserve(f.consumer,400,300,[&](auto ref,auto,c::delivery const& value){next=ref;return value.body[0]==1;})==error::ok);
    assert(next.nonce!=first.nonce&&next.sequence==first.sequence);
    assert(f.queue.settle(f.consumer,first,{51,2},true,300,f.receipt)==error::denied);
    assert(f.queue.settle(f.consumer,next,{51,2},true,300,f.receipt)==error::ok);
    assert(f.queue.push(info,f.third)==error::ok);
    assert(f.queue.take([](auto const& value){return value.body[0]==2;})==error::ok);
    assert(f.queue.take([](auto const& value){return value.body[0]==3&&value.information.loss_before==1;})==error::ok);
}
static void concurrent_sink_and_retirement()
{
    fixture f;std::barrier entered{2},leave{2};error result{};c::delivery_reference ref;
    std::thread reader{[&]{result=f.queue.reserve(f.consumer,200,100,[&](auto reference,auto,c::delivery const& value)
        {ref=reference;entered.arrive_and_wait();leave.arrive_and_wait();return value.body[0]==1;});}};
    entered.arrive_and_wait();c::metadata info;info.source=51;info.generation=71;
    assert(f.queue.push(info,f.third)==error::again);
    assert(f.queue.settle(f.consumer,ref,{51,1},false,100,f.receipt)==error::again);
    assert(f.queue.bind(52,72)==error::busy);f.queue.retire();leave.arrive_and_wait();reader.join();
    assert(result==error::cancelled&&f.queue.has_reservation());
    assert(f.queue.settle(f.consumer,ref,{51,1},true,100,f.receipt)==error::cancelled);
    assert(f.queue.settle(f.consumer,ref,{51,1},false,100,f.receipt)==error::ok);
    assert(f.queue.bind(52,72)==error::ok);
    assert(f.queue.settle(f.consumer,ref,{51,1},false,100,f.receipt)==error::denied);
    fixture reentrant;
    assert(reentrant.queue.reserve(reentrant.consumer,200,100,[&](auto current,auto,auto const&)
        {reentrant.reference=current;reentrant.queue.retire();return true;})==error::cancelled);
    assert(reentrant.queue.settle(reentrant.consumer,reentrant.reference,{51,1},false,100,reentrant.receipt)==error::ok);
}
static void concurrent_consumers_and_fork()
{
    fixture f;std::array<c::delivery_reference,8> references;std::array<error,8> results;std::array<std::thread,8> threads;std::barrier start{8};
    for(std::size_t i{};i<threads.size();++i)threads[i]=std::thread{[&,i]
        {start.arrive_and_wait();results[i]=f.queue.reserve(f.consumer,200,100,[&](auto ref,auto,auto const&){references[i]=ref;return true;});}};
    for(auto& thread:threads)thread.join();
    c::delivery_reference ref;
    for(std::size_t i{};i<results.size();++i)
    {assert(results[i]==error::ok||results[i]==error::again);if(results[i]==error::ok){if(ref.nonce)assert(ref==references[i]);else ref=references[i];}}
    assert(ref.nonce);auto child=::fork();assert(child>=0);
    if(!child)
    {
        assert(f.reserve()==error::denied&&f.queue.settle(f.consumer,ref,{51,1},false,100,f.receipt)==error::denied);
        f.queue.retire();::_exit(0);
    }
    int status{};assert(::waitpid(child,&status,0)==child&&WIFEXITED(status)&&!WEXITSTATUS(status));
    assert(f.queue.settle(f.consumer,ref,{51,1},true,100,f.receipt)==error::ok);
}
int main()
{
    assert(existing_communication_core_tests()==0);
    receive_carriers();
    hostile_worker_replies();
    lost_output_and_exact_settlement();release_deadline_and_overflow();concurrent_sink_and_retirement();concurrent_consumers_and_fork();
    std::puts("PASS two-phase communication delivery: pinned original, failed-copy retry, exact caller/nonce/key, expiry cleanup, retained ordering, concurrent producer/consumers, reentrant retirement and fork fencing");
}
