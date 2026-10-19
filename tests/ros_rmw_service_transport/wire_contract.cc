/** Production UMRC value contracts. All native-looking records are explicit
 * logical fixtures, NOT actually received middleware metadata. No fake native
 * headers, Host, transport, entity factory or synthesized RMW types are used.
 */
#include <uwvm2/imported/ros/ipc/rmw_service.h>
#include <cstdlib>
namespace service=uwvm2::imported::ros::ipc::rmw_service;
namespace endpoint=uwvm2::imported::ros::ipc::rmw_endpoint;
namespace metadata=uwvm2::imported::ros::ipc::rmw_metadata;
namespace events=uwvm2::imported::ros::ipc;
static void require(bool value){if(!value){std::abort();}}
static service::control base(service::command operation=service::command::inspect)
{
    service::control p{};p.operation=operation;p.owner={2,UINT64_C(0xff123456a1234567),37,41,43};
    p.peer=UINT64_C(0xfe123456a1234567);p.incarnation=UINT64_C(0xfd123456a1234567);return p;
}
static service::control peek()
{auto p=base(service::command::peek);p.capacity=service::minimum_delivery_bytes+16;p.writer_bytes=16;return p;}
static metadata::header logical_info(service::control const& p)
{
    metadata::header h{};h.kind=p.kind==endpoint::role::client?events::event_kind::service_response:events::event_kind::service_request;
    h.owner=p.owner;h.incarnation=p.incarnation;h.sequence=17;h.correlation=UINT64_MAX-2;h.flags=metadata::source_time|metadata::received_time|metadata::native_request_id;
    h.writer_size=16;h.request_sequence=INT64_MIN;h.source_timestamp=0;h.received_timestamp=-1;return h;
}
static std::vector<std::uint8_t> delivery(service::control p=peek(),metadata::header h=logical_info(peek()))
{
    std::array<std::uint8_t,metadata::header_bytes> m{};require(metadata::encode(h,m));
    p.sequence=h.sequence;p.reservation=71;p.ticket=h.correlation;p.record_bytes=metadata::header_bytes;
    std::vector<std::uint8_t> bytes;require(service::pack_delivery(p,m.data(),m.size(),bytes)==0);return bytes;
}
static void exact_known_header_bytes()
{
    auto p=base();std::array<std::uint8_t,service::header_bytes>b{};require(service::encode(p,false,b));
    require(b[0]=='U'&&b[1]=='M'&&b[2]=='R'&&b[3]=='C'&&b[4]==1&&b[6]==2&&b[7]==2&&b[8]==160);
    require(b[12]==3&&b[16]==0x67&&b[23]==0xff&&b[55]==0xfd&&b[63]==0xfe);
    service::control q{};require(service::request(b.data(),b.size(),q)&&endpoint::same(q.owner,p.owner)&&q.peer==p.peer);
}
static void precreation_identity_and_fresh_incarnation()
{
    auto p=base(service::command::activate);std::array<std::uint8_t,service::header_bytes>b{};require(!service::encode(p,false,b));
    p.incarnation=0;require(service::encode(p,false,b)&&!service::encode(p,true,b));
    p.incarnation=UINT64_MAX;require(service::encode(p,true,b));
}
static void independently_selected_peer_is_full_and_distinct()
{
    auto p=base();require(service::valid(p,false));
    for(auto peer:{std::uint64_t{0},std::uint64_t{1},UINT64_C(0x100000000),p.owner.source}){p.peer=peer;require(!service::valid(p,false));}
}
static void foreign_binding_cannot_associate()
{
    auto p=base(service::command::available),q=p;std::array<std::uint8_t,service::header_bytes>b{};
    for(unsigned field{};field!=7;++field)
    {
        q=p;if(field==0)++q.owner.source;else if(field==1)++q.owner.instance;else if(field==2)++q.owner.generation;
        else if(field==3)++q.owner.session;else if(field==4)++q.incarnation;else if(field==5)++q.peer;else q.kind=endpoint::role::service;
        if(service::encode(q,true,b)){service::control out{};out.ticket=91;require(!service::associated_ack(b.data(),b.size(),p,out)&&out.ticket==91);}
    }
}
static void reserved_bytes_and_version_refused()
{
    auto p=base();std::array<std::uint8_t,service::header_bytes>b{};require(service::encode(p,false,b));
    for(auto offset:{4u,5u,7u,8u,12u,14u,15u,128u,159u})
    {auto changed=b;changed[offset]=0xff;service::control out{};out.value=77;require(!service::decode(changed.data(),changed.size(),false,out)&&out.value==77);}
}
static void input_has_exact_whole_extent()
{
    auto p=base(service::command::submit);p.body_bytes=16;std::array<std::uint8_t,service::header_bytes>b{};require(service::encode(p,false,b));
    std::vector<std::uint8_t> packet(b.begin(),b.end());packet.resize(b.size()+16);service::control out{};
    require(service::request(packet.data(),packet.size(),out)&&out.body_bytes==16);
    require(!service::request(packet.data(),packet.size()-1,out));packet.push_back(0);require(!service::request(packet.data(),packet.size(),out));
}
static void empty_input_stays_zero()
{
    auto p=base(service::command::submit);std::array<std::uint8_t,service::header_bytes>b{};require(service::encode(p,false,b));
    service::control out{};require(service::request(b.data(),b.size(),out)&&out.body_bytes==0);
    auto before=b;p.body_bytes=service::maximum_input_bytes+1;require(!service::encode(p,false,b)&&b==before);
}
static void submit_ack_owns_ticket_without_native_id_forgery()
{
    auto p=base(service::command::submit),reply=p;reply.ticket=UINT64_MAX;
    std::array<std::uint8_t,service::header_bytes>b{};require(service::encode(reply,true,b));service::control out{};
    require(service::associated_ack(b.data(),b.size(),p,out)&&out.ticket==UINT64_MAX&&!out.sequence&&!out.reservation);
    reply.ticket=0;require(!service::encode(reply,true,b));reply=p;reply.kind=endpoint::role::service;require(!service::valid(reply,false));
}
static void response_is_separate_native_effect()
{
    auto p=base(service::command::respond);p.kind=endpoint::role::service;p.ticket=73;p.body_bytes=8;
    require(service::valid(p,false));auto reply=p;reply.body_bytes=0;
    std::array<std::uint8_t,service::header_bytes>b{};require(service::encode(reply,true,b));service::control out{};
    require(service::associated_ack(b.data(),b.size(),p,out));reply.ticket=74;require(service::encode(reply,true,b)&&!service::associated_ack(b.data(),b.size(),p,out));
}
static void bounded_availability_has_independent_peer_and_boolean()
{
    auto p=base(service::command::available);p.wait_ns=service::maximum_wait_ns;auto reply=p;reply.value=1;
    std::array<std::uint8_t,service::header_bytes>b{};require(service::encode(reply,true,b));service::control out{};
    require(service::associated_ack(b.data(),b.size(),p,out)&&out.value==1);
    ++p.wait_ns;require(!service::valid(p,false));reply.value=2;require(!service::encode(reply,true,b));
}
static void complete_delivery_preserves_zero_writer_and_signed_values()
{
    auto p=peek();auto packet=delivery();service::control out{};metadata::header info{};
    require(service::associated_delivery(packet.data(),packet.size(),p,out,info));
    require(out.ticket==UINT64_MAX-2&&out.sequence==17&&out.reservation==71&&info.request_sequence==INT64_MIN&&
        info.source_timestamp==0&&info.received_timestamp==-1&&info.writer_size==16&&endpoint::zero(info.writer.data(),16));
}
static void delivery_wrong_side_and_missing_metadata_preserve_outputs()
{
    auto p=peek();auto packet=delivery();auto q=p;q.kind=endpoint::role::service;service::control out{};out.ticket=97;
    metadata::header info{};info.sequence=101;
    require(!service::associated_delivery(packet.data(),packet.size(),q,out,info)&&out.ticket==97&&info.sequence==101);
    // Remove the actual source-time presence flag. A zero value does not make
    // an absent metadata field complete; wire format remains otherwise valid.
    packet[service::header_bytes+12]&=static_cast<std::uint8_t>(~metadata::source_time);
    require(!service::associated_delivery(packet.data(),packet.size(),p,out,info)&&out.ticket==97&&info.sequence==101);
}
static void control_record_preserves_error_without_service_info()
{
    auto p=peek();metadata::header h{};h.owner=p.owner;h.incarnation=p.incarnation;h.sequence=19;
    h.kind=events::event_kind::resource_error;h.error=110;h.correlation=37;
    auto packet=delivery(p,h);service::control out{};metadata::header info{};
    require(service::associated_delivery(packet.data(),packet.size(),p,out,info)&&info.error==110&&info.correlation==37&&!info.flags&&!info.writer_size);
}
static void service_request_complete_kind_and_extent()
{
    auto p=peek();p.kind=endpoint::role::service;auto h=logical_info(p);auto packet=delivery(p,h);service::control out{};metadata::header info{};
    require(service::associated_delivery(packet.data(),packet.size(),p,out,info)&&info.kind==events::event_kind::service_request);
    packet.push_back(0);require(!service::associated_delivery(packet.data(),packet.size(),p,out,info));
}
static void borrow_extent_and_original_writer_width()
{
    auto p=peek();require(service::valid(p,false));p.capacity=service::minimum_delivery_bytes-1;require(!service::valid(p,false));
    p=peek();p.capacity=events::max_payload_bytes;require(service::valid(p,false));++p.capacity;require(!service::valid(p,false));
    p=peek();p.writer_bytes=17;require(!service::valid(p,false));p.writer_bytes=0;require(!service::valid(p,false));
}
static void exact_nonce_ack_and_no_role_swap()
{
    auto p=base(service::command::commit);p.sequence=17;p.reservation=71;auto reply=p;
    std::array<std::uint8_t,service::header_bytes>b{};require(service::encode(reply,true,b));service::control out{};
    require(service::associated_ack(b.data(),b.size(),p,out));++reply.reservation;require(service::encode(reply,true,b)&&!service::associated_ack(b.data(),b.size(),p,out));
    reply=p;reply.operation=service::command::cancel;require(service::encode(reply,true,b)&&!service::associated_ack(b.data(),b.size(),p,out));
}
static void endpoint_receipt_contains_actual_two_profiles()
{
    auto p=base(),reply=p;endpoint::packet actual{};actual.operation=endpoint::command::inspect;actual.kind=p.kind;actual.owner=p.owner;
    actual.incarnation=p.incarnation;actual.profiles=2;actual.gid_size=16;
    for(auto& q:actual.actual){q={1,1,2,1,4,{9223372036,854775807},{0,0},{9223372036,854775807},false};}
    std::array<std::uint8_t,service::header_bytes> head{};std::array<std::uint8_t,endpoint::packet_bytes> receipt{};
    require(service::encode(reply,true,head)&&endpoint::encode(actual,true,receipt));
    std::vector<std::uint8_t> packet(head.begin(),head.end());packet.insert(packet.end(),receipt.begin(),receipt.end());
    service::control out{};endpoint::packet observed{};require(service::associated_endpoint(packet.data(),packet.size(),p,out,observed)&&observed.profiles==2&&observed.gid_size==16);
    packet[service::header_bytes+48]^=1;require(!service::associated_endpoint(packet.data(),packet.size(),p,out,observed));
}
static void failed_pack_preserves_owned_output()
{
    auto p=peek();auto h=logical_info(p);std::array<std::uint8_t,metadata::header_bytes> raw{};require(metadata::encode(h,raw));
    p.sequence=h.sequence;p.reservation=71;p.ticket=h.correlation;p.record_bytes=raw.size();
    std::vector<std::uint8_t> output{7,9};auto before=output;require(service::pack_delivery(p,raw.data(),raw.size()-1,output)==22&&output==before);
}
int main()
{
    exact_known_header_bytes();precreation_identity_and_fresh_incarnation();independently_selected_peer_is_full_and_distinct();
    foreign_binding_cannot_associate();reserved_bytes_and_version_refused();input_has_exact_whole_extent();empty_input_stays_zero();
    submit_ack_owns_ticket_without_native_id_forgery();response_is_separate_native_effect();bounded_availability_has_independent_peer_and_boolean();
    complete_delivery_preserves_zero_writer_and_signed_values();delivery_wrong_side_and_missing_metadata_preserve_outputs();
    control_record_preserves_error_without_service_info();service_request_complete_kind_and_extent();borrow_extent_and_original_writer_width();
    exact_nonce_ack_and_no_role_swap();endpoint_receipt_contains_actual_two_profiles();failed_pack_preserves_owned_output();
}
