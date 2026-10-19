/** Authored SDK-free wire cases. NOT EXECUTED. No simulated ROS entity. */
#ifdef NDEBUG
#error "Wire checks must not be disabled"
#endif
#include <uwvm2/imported/ros/ipc/rmw_subscription.h>
#include <cstdio>
#include <cstdlib>
namespace ipc=uwvm2::imported::ros::ipc;
namespace wire=ipc::rmw_subscription;
namespace meta=ipc::rmw_metadata;
static unsigned failures;
#define CHECK(v) do {if(!(v)){++failures;std::fprintf(stderr,"line %u: %s\n",__LINE__,#v);}} while(false)
static wire::control request()
{return {wire::command::peek,{2,UINT64_C(0xa123000180000001),7,11,13},17,0,0,1024,0,16};}
static std::vector<std::uint8_t> record(wire::control const& req,std::uint64_t sequence=1)
{
    meta::header h{};h.owner=req.owner;h.incarnation=req.incarnation;h.sequence=sequence;
    h.flags=meta::source_time|meta::received_time|meta::publisher_gid;h.gid_size=16;
    h.source_timestamp=-7;h.received_timestamp=9;h.payload_bytes=1;
    // Explicit all-zero GID bytes with actual extent is legal wire DATA. This
    // does not pretend that these were captured from a native publisher.
    std::array<std::uint8_t,192> raw{};CHECK(meta::encode(h,raw));
    std::vector<std::uint8_t> bytes(raw.begin(),raw.end());bytes.push_back(1);return bytes;
}
static wire::control response(wire::control const& req,std::size_t bytes)
{auto r=req;r.sequence=1;r.reservation=91;r.record_bytes=bytes;return r;}
static void full_width_roundtrip()
{
    auto r=request();r.owner.source=UINT64_C(0xf1234567fedcba98);r.owner.instance=UINT64_MAX;
    r.owner.generation=UINT64_MAX-1;r.owner.session=UINT64_MAX-2;r.incarnation=UINT64_MAX-3;
    std::array<std::uint8_t,96> bytes{};CHECK(wire::encode(r,false,bytes));wire::control out{};
    CHECK(wire::decode(bytes.data(),bytes.size(),false,out)&&ipc::rmw_endpoint::same(out.owner,r.owner)&&out.incarnation==r.incarnation);
}
static void fixed_header_and_reserved_refuse()
{
    auto r=request();std::array<std::uint8_t,96> bytes{};CHECK(wire::encode(r,false,bytes));
    for(auto at:{0,4,8,13,14,15,88,95})
    {auto bad=bytes;bad[at]^=0x80;wire::control out{};out.incarnation=777;CHECK(!wire::decode(bad.data(),bad.size(),false,out)&&out.incarnation==777);}
    wire::control out{};CHECK(!wire::decode(bytes.data(),95,false,out));CHECK(!wire::decode(nullptr,96,false,out));
}
static void request_semantics_refuse()
{
    auto r=request();CHECK(wire::valid(r,false));r.sequence=1;CHECK(!wire::valid(r,false));r.sequence=0;r.reservation=1;CHECK(!wire::valid(r,false));
    r=request();r.record_bytes=1;CHECK(!wire::valid(r,false));r=request();r.gid_bytes=0;CHECK(!wire::valid(r,false));
    r=request();r.gid_bytes=33;CHECK(!wire::valid(r,false));r=request();r.capacity=287;CHECK(!wire::valid(r,false));
    r=request();r.capacity=UINT64_MAX;CHECK(!wire::valid(r,false));r=request();r.operation=static_cast<wire::command>(4);CHECK(!wire::valid(r,false));
}
static void complete_same_record_packet()
{
    auto req=request();auto raw=record(req);auto reply=response(req,raw.size());std::vector<std::uint8_t> packet;
    CHECK(!wire::pack_delivery(reply,raw.data(),raw.size(),packet)&&packet.size()==96+192+1);
    wire::control got{};meta::header h{};CHECK(wire::associated_delivery(packet.data(),packet.size(),req,got,h));
    CHECK(got.sequence==1&&got.reservation==91&&h.sequence==got.sequence&&h.owner.source==req.owner.source&&h.gid_size==16&&h.source_timestamp==-7);
    CHECK(packet.back()==1);
}
static void wrong_source_generation_session_refuse()
{
    auto req=request();auto raw=record(req);auto reply=response(req,raw.size());std::vector<std::uint8_t> packet;CHECK(!wire::pack_delivery(reply,raw.data(),raw.size(),packet));
    for(unsigned field=0;field!=5;++field)
    {auto bad=req;switch(field){case 0:++bad.owner.source;break;case 1:++bad.owner.instance;break;case 2:++bad.owner.generation;break;case 3:++bad.owner.session;break;default:++bad.incarnation;}
     wire::control out{};out.reservation=777;meta::header h{};h.sequence=888;CHECK(!wire::associated_delivery(packet.data(),packet.size(),bad,out,h)&&out.reservation==777&&h.sequence==888);}
}
static void metadata_missing_does_not_project()
{
    auto req=request();auto raw=record(req);auto reply=response(req,raw.size());
    ipc::event_put(raw.data()+12,meta::publisher_gid,4);ipc::event_put(raw.data()+72,0,8);ipc::event_put(raw.data()+80,0,8);
    std::vector<std::uint8_t> untouched{6,7};CHECK(wire::pack_delivery(reply,raw.data(),raw.size(),untouched)==22&&untouched==std::vector<std::uint8_t>({6,7}));
}
static void extent_capacity_atomic_refusal()
{
    auto req=request();auto raw=record(req);auto reply=response(req,raw.size());reply.capacity=288;
    std::vector<std::uint8_t> untouched{4,5};CHECK(wire::pack_delivery(reply,raw.data(),raw.size(),untouched)==22&&untouched==std::vector<std::uint8_t>({4,5}));
    reply=response(req,raw.size()+1);CHECK(wire::pack_delivery(reply,raw.data(),raw.size(),untouched)==22&&untouched==std::vector<std::uint8_t>({4,5}));
}
static void metadata_gid_extent_and_sequence_refuse()
{
    auto req=request();auto raw=record(req);auto reply=response(req,raw.size());reply.gid_bytes=32;meta::header untouched{};untouched.sequence=777;
    CHECK(!wire::associated_record(reply,raw.data(),raw.size(),untouched)&&untouched.sequence==777);
    reply=response(req,raw.size());reply.sequence=2;CHECK(!wire::associated_record(reply,raw.data(),raw.size(),untouched)&&untouched.sequence==777);
}
static void trailing_or_short_packet_refuse()
{
    auto req=request();auto raw=record(req);auto reply=response(req,raw.size());std::vector<std::uint8_t> packet;CHECK(!wire::pack_delivery(reply,raw.data(),raw.size(),packet));
    wire::control out{};meta::header h{};CHECK(!wire::associated_delivery(packet.data(),packet.size()-1,req,out,h));
    packet.push_back(0);CHECK(!wire::associated_delivery(packet.data(),packet.size(),req,out,h));
}
static void nonce_distinct_from_stream_identity()
{
    auto req=request();auto raw=record(req);auto a=response(req,raw.size()),b=a;++b.reservation;
    CHECK(a.sequence==b.sequence&&a.reservation!=b.reservation);
    wire::control old_ack{wire::command::commit,req.owner,req.incarnation,a.sequence,a.reservation};
    auto new_ack=old_ack;new_ack.reservation=b.reservation;
    std::array<std::uint8_t,96> packet{};CHECK(wire::encode(old_ack,true,packet));wire::control out{};
    CHECK(!wire::associated_ack(packet.data(),packet.size(),new_ack,out));CHECK(wire::associated_ack(packet.data(),packet.size(),old_ack,out));
}
static void acknowledgment_direction_and_token_refuse()
{
    auto req=request();wire::control ack{wire::command::cancel,req.owner,req.incarnation,1,91};std::array<std::uint8_t,96> packet{};
    CHECK(wire::encode(ack,true,packet));auto other=ack;other.operation=wire::command::commit;wire::control out{};CHECK(!wire::associated_ack(packet.data(),packet.size(),other,out));
    CHECK(wire::associated_ack(packet.data(),packet.size(),ack,out));ack.reservation=0;CHECK(!wire::valid(ack,false));
    ack.reservation=91;ack.capacity=288;CHECK(!wire::valid(ack,false));
}
static void control_helpers_are_failure_atomic()
{
    auto bad=request();bad.owner.source=1;std::array<std::uint8_t,96> encoded;encoded.fill(0x66);
    CHECK(!wire::encode(bad,false,encoded));for(auto value:encoded){CHECK(value==0x66);}
    auto good=request();CHECK(wire::encode(good,false,encoded));wire::control out{};out.reservation=999;
    CHECK(!wire::decode(encoded.data(),encoded.size(),true,out)&&out.reservation==999);
}
int main()
{
    full_width_roundtrip();fixed_header_and_reserved_refuse();request_semantics_refuse();complete_same_record_packet();
    wrong_source_generation_session_refuse();metadata_missing_does_not_project();extent_capacity_atomic_refusal();
    metadata_gid_extent_and_sequence_refuse();trailing_or_short_packet_refuse();nonce_distinct_from_stream_identity();
    acknowledgment_direction_and_token_refuse();control_helpers_are_failure_atomic();
    if(!failures){std::fputs("subscription-registry-wire:12 authored groups PASS\n",stdout);}return failures?1:0;
}
