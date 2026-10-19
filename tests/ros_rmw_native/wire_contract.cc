/** Authored protocol contracts. These use no ROS mocks or fabricated SDK
 * headers. They validate byte association, refusal and output atomicity only;
 * a passing local wire model cannot establish a native QoS/GID or ROS delivery.
 * Compile/run only in the resource owner's admitted /tmp test directory. */
#include <uwvm2/imported/ros/ipc/rmw_metadata.h>
#include <cassert>
#ifdef NDEBUG
#error "Wire contracts require active assertions; a release-elided test is not a passing result"
#endif
namespace ipc=uwvm2::imported::ros::ipc;
namespace wire=ipc::rmw_endpoint;
namespace info=ipc::rmw_metadata;
static wire::binding owner(){return {2,UINT64_C(0xf1234567a1234567),UINT64_C(0xff11223344556677),UINT64_C(0xaa11223344556677),UINT64_C(0xbb11223344556677)};}
static wire::qos qos(){return {1,1,2,1,7,{9223372036,854775807},{100,42},{0,42},false};}
static wire::packet publisher()
{wire::packet p{};p.owner=owner();p.incarnation=UINT64_C(0xfd11223344556677);p.profiles=1;p.actual[0]=qos();p.gid_size=24;for(unsigned i{};i<24;++i){p.gid[i]=static_cast<std::uint8_t>(i+1);}return p;}
static info::header message()
{
    info::header h{};h.owner=owner();h.incarnation=UINT64_C(0xfd11223344556677);h.sequence=UINT64_C(0xee11223344556677);
    h.flags=info::source_time|info::received_time|info::publisher_gid|info::publication_sequence;
    h.source_timestamp=-17;h.received_timestamp=INT64_MAX;h.publication_number=UINT64_C(0xf123456789abcdef);
    h.gid_size=24;h.gid[0]=4;h.gid[23]=7;h.payload_bytes=0;return h;
}
static void endpoint_known_bytes_and_association()
{
    auto p{publisher()};std::array<std::uint8_t,wire::packet_bytes>b{};assert(wire::encode(p,true,b));
    assert(b[0]=='U'&&b[1]=='M'&&b[2]=='R'&&b[3]=='E'&&b[6]==1&&b[7]==2&&b[8]==0&&b[9]==1);
    assert(b[16]==0x67&&b[23]==0xf1&&b[48]==0x77&&b[55]==0xfd&&b[64]==1&&b[80]==7&&b[224]==1&&b[247]==24);
    wire::packet decoded{};assert(wire::decode(b.data(),b.size(),true,decoded));
    assert(wire::same(decoded.owner,p.owner)&&decoded.incarnation==p.incarnation&&wire::same(decoded.actual[0],p.actual[0])&&decoded.gid==p.gid);
    wire::packet request{};request.owner=owner();assert(wire::associated_response(b.data(),b.size(),request,decoded));
    request.owner.generation^=1;auto unchanged{decoded.incarnation};assert(!wire::associated_response(b.data(),b.size(),request,decoded)&&decoded.incarnation==unchanged);
}
static void endpoint_refusal_and_unused_bytes()
{
    auto p{publisher()};std::array<std::uint8_t,wire::packet_bytes>b{};assert(wire::encode(p,true,b));
    wire::packet sentinel{};sentinel.incarnation=73;
    for(unsigned offset:{15u,56u,63u,137u,143u,144u,223u,248u,255u})
    {auto changed{b};changed[offset]=1;assert(!wire::decode(changed.data(),changed.size(),true,sentinel)&&sentinel.incarnation==73);}
    assert(!wire::decode(nullptr,b.size(),true,sentinel));assert(!wire::decode(b.data(),b.size()-1,true,sentinel));
    auto changed{b};changed[13]=3;assert(!wire::decode(changed.data(),changed.size(),true,sentinel));
    for(unsigned offset:{16u,24u,32u,40u,48u})
    {auto bad{b};std::fill(bad.begin()+offset,bad.begin()+offset+8,0);assert(!wire::decode(bad.data(),bad.size(),true,sentinel));}
    auto bad{b};std::fill(bad.begin()+20,bad.begin()+24,0);assert(!wire::decode(bad.data(),bad.size(),true,sentinel));
}
static void qos_must_be_concrete()
{
    auto p{publisher()};std::array<std::uint8_t,wire::packet_bytes> output{};output.fill(0x5a);
    auto original{output};p.actual[0].reliability=0;assert(!wire::encode(p,true,output)&&output==original);
    p=publisher();p.actual[0].history=3;assert(!wire::encode(p,true,output));
    p=publisher();p.actual[0].depth=0;assert(!wire::encode(p,true,output));
    p=publisher();p.actual[0].deadline.nanoseconds=1000000000;assert(!wire::encode(p,true,output));
    p=publisher();p.actual[0].avoid_ros_namespace_conventions=true;assert(wire::encode(p,true,output));
    wire::packet decoded{};assert(wire::decode(output.data(),output.size(),true,decoded)&&decoded.actual[0].avoid_ros_namespace_conventions);
}
static void two_independent_actual_profiles()
{
    auto p{publisher()};p.kind=wire::role::client;p.gid_size=0;p.gid.fill(0);p.profiles=2;
    p.actual[1]=qos();p.actual[1].reliability=2;p.actual[1].depth=11;
    std::array<std::uint8_t,wire::packet_bytes>b{};assert(wire::encode(p,true,b));wire::packet d{};
    assert(wire::decode(b.data(),b.size(),true,d)&&d.actual[0].depth==7&&d.actual[1].depth==11&&d.actual[1].reliability==2);
    p.gid_size=16;p.gid[0]=0x42;assert(wire::encode(p,true,b));
    assert(wire::decode(b.data(),b.size(),true,d)&&d.gid_size==16&&d.gid[0]==0x42);
    p.gid[17]=1;assert(!wire::encode(p,true,b));p.gid[17]=0;
    p.profiles=1;assert(!wire::encode(p,true,b));p.profiles=2;p.actual[1].liveliness=0;assert(!wire::encode(p,true,b));
}
static void activation_inspect_and_retire()
{
    wire::packet p{};p.owner=owner();std::array<std::uint8_t,wire::packet_bytes>b{};
    assert(wire::encode(p,false,b));wire::packet d{};assert(wire::decode(b.data(),b.size(),false,d));
    p.operation=wire::command::inspect;assert(!wire::encode(p,false,b));p.incarnation=77;assert(wire::encode(p,false,b));
    p.operation=wire::command::retire;assert(wire::encode(p,false,b)&&wire::encode(p,true,b));
    p.operation=wire::command::activate;assert(!wire::encode(p,false,b));
}
static void metadata_known_bytes_signed_fields_and_genuine_unknown_sequence()
{
    auto h{message()};std::array<std::uint8_t,info::header_bytes>b{};assert(info::complete(h)&&info::encode(h,b));
    assert(b[0]=='U'&&b[1]=='R'&&b[2]=='M'&&b[3]=='I'&&b[8]==192&&b[72]==0xef&&b[79]==0xff&&b[87]==0x7f);
    assert(b[88]==0xef&&b[95]==0xf1&&b[96]==0xff&&b[103]==0xff&&b[120]==24&&b[128]==4&&b[151]==7);
    info::header d{};assert(info::decode(b.data(),b.size(),0,d));
    assert(d.source_timestamp==-17&&d.received_timestamp==INT64_MAX&&d.publication_number==h.publication_number&&d.reception_number==UINT64_MAX);
}
static void metadata_refuses_extra_and_missing_native_facts()
{
    auto h{message()};std::array<std::uint8_t,info::header_bytes>b{};assert(info::encode(h,b));info::header sentinel{};sentinel.sequence=99;
    for(unsigned offset:{123u,152u,159u,160u,175u,176u,191u})
    {auto bad{b};bad[offset]=1;assert(!info::decode(bad.data(),bad.size(),0,sentinel)&&sentinel.sequence==99);}
    assert(!info::decode(b.data(),b.size(),1,sentinel));assert(!info::decode(b.data(),b.size()-1,0,sentinel));
    h.flags&=~info::source_time;assert(!info::valid(h));h.source_timestamp=0;assert(info::valid(h)&&!info::complete(h));
    h=message();h.flags|=info::reception_sequence;assert(!info::valid(h));
    h=message();h.flags=64;assert(!info::valid(h));
    h=message();h.control_flags=1;assert(!info::valid(h));
}
static void native_service_request_is_distinct_from_gateway_ticket()
{
    auto h{message()};h.kind=ipc::event_kind::service_request;h.correlation=UINT64_MAX;h.payload_bytes=16;
    h.flags=info::source_time|info::received_time|info::native_request_id;
    h.gid_size=0;h.gid.fill(0);h.publication_number=h.reception_number=UINT64_MAX;
    h.request_sequence=-31;h.writer_size=16;h.writer[0]=4;h.writer[15]=7;
    assert(info::complete(h));std::array<std::uint8_t,info::header_bytes>b{};assert(info::encode(h,b));info::header d{};
    assert(info::decode(b.data(),b.size(),16,d)&&d.request_sequence==-31&&d.correlation==UINT64_MAX&&d.writer[15]==7);
    h.correlation=0;assert(!info::valid(h));h.correlation=UINT64_MAX;h.flags|=info::publisher_gid;assert(!info::valid(h));
}
static void control_error_retirement_and_ambiguity_remain_exact()
{
    auto h{message()};h.kind=ipc::event_kind::resource_error;h.flags=0;h.source_timestamp=h.received_timestamp=0;
    h.publication_number=h.reception_number=UINT64_MAX;h.gid_size=0;h.gid.fill(0);h.error=110;
    h.correlation=UINT64_C(0xf123456789abcdef);h.control_flags=ipc::event_external_effects_may_remain;
    std::array<std::uint8_t,info::header_bytes>b{};assert(info::encode(h,b));info::header d{};
    assert(info::decode(b.data(),b.size(),0,d)&&d.error==110&&d.correlation==h.correlation&&d.control_flags==2&&!info::complete(d));
    h.kind=ipc::event_kind::resource_closed;h.error=125;assert(!info::valid(h));h.control_flags=ipc::event_source_retired;
    assert(info::encode(h,b)&&info::decode(b.data(),b.size(),0,d)&&d.error==125&&d.control_flags==1);
    h.control_flags=4;assert(!info::valid(h));
}
int main()
{
    endpoint_known_bytes_and_association();endpoint_refusal_and_unused_bytes();qos_must_be_concrete();two_independent_actual_profiles();
    activation_inspect_and_retire();metadata_known_bytes_signed_fields_and_genuine_unknown_sequence();metadata_refuses_extra_and_missing_native_facts();
    native_service_request_is_distinct_from_gateway_ticket();control_error_retirement_and_ambiguity_remain_exact();
}
