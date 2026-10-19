/* SPDX-License-Identifier: Apache-2.0
 * Original implementation over actual generated C++ types. Published schema/
 * introspection templates and buildfarm generated headers are retained as DATA
 * in the accompanying source package. No generated struct/hash is synthesized.
 */
#include "uwvm_rmw/cpp_codecs.hpp"
#include "uwvm_rmw/contracts.h"
#include <roswasm/codec.h>
#include <std_msgs/msg/string.hpp>
#include <std_srvs/srv/empty.hpp>
#include <example_interfaces/srv/add_two_ints.hpp>
#include <service_msgs/msg/service_event_info.hpp>
#include <builtin_interfaces/msg/time.hpp>
#include <rosidl_typesupport_cpp/message_type_support.hpp>
#include <rosidl_typesupport_cpp/service_type_support.hpp>
#include <rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp>
#include <rosidl_typesupport_introspection_cpp/service_type_support_decl.hpp>
#include <rosidl_typesupport_introspection_cpp/message_introspection.hpp>
#include <rosidl_typesupport_introspection_cpp/service_introspection.hpp>
#include <rosidl_typesupport_introspection_cpp/field_types.hpp>
#include <rosidl_typesupport_introspection_cpp/identifier.hpp>
#include <rosidl_runtime_c/type_description/type_description__struct.h>
#include <rosidl_runtime_c/type_description/field_type__struct.h>
#include <bit>
#include <cstddef>
#include <cstring>
#include <new>
#include <string>
#include <type_traits>
#include <utility>
namespace uwvm_rmw_cpp::detail {
namespace ic=::rosidl_typesupport_introspection_cpp;
using String=::std_msgs::msg::String;using Empty=::std_srvs::srv::Empty;
using Add=::example_interfaces::srv::AddTwoInts;
using Info=::service_msgs::msg::ServiceEventInfo;using Time=::builtin_interfaces::msg::Time;
using Message=::rosidl_message_type_support_t;using Service=::rosidl_service_type_support_t;
constexpr std::size_t messages=9,identities=11,descriptors=7,none=identities;
struct Field {
  char const* name;std::uint8_t description_type,native_type;std::size_t offset,capacity,nested;
  bool array,upper;
};
struct Spec {char const* name;char const* ns;char const* local;std::size_t size,count;
  std::array<Field,4> fields;std::uint16_t references;};
namespace td {
constexpr auto nested=rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE;
constexpr auto u8=rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8;
constexpr auto i32=rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32;
constexpr auto u32=rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT32;
constexpr auto i64=rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT64;
constexpr auto text=rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING;
constexpr auto bytes16=rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8_ARRAY;
constexpr auto bounded=rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_BOUNDED_SEQUENCE;
}
/* Native offsets/sizes come from the actual included generated C++ definitions,
 * exactly as the official introspection generator uses offsetof/sizeof. No
 * numeric std::string/vector layout or C-generated object cast is guessed. */
static std::array<Spec,identities> const specs{{
 {"std_msgs/msg/String","std_msgs::msg","String",sizeof(String),1,
  {{{"data",td::text,ic::ROS_TYPE_STRING,offsetof(String,data),0,none,false,false}}},0},
 {"builtin_interfaces/msg/Time","builtin_interfaces::msg","Time",sizeof(Time),2,
  {{{"sec",td::i32,ic::ROS_TYPE_INT32,offsetof(Time,sec),0,none,false,false},
    {"nanosec",td::u32,ic::ROS_TYPE_UINT32,offsetof(Time,nanosec),0,none,false,false}}},0},
 {"service_msgs/msg/ServiceEventInfo","service_msgs::msg","ServiceEventInfo",sizeof(Info),4,
  {{{"event_type",td::u8,ic::ROS_TYPE_UINT8,offsetof(Info,event_type),0,none,false,false},
    {"stamp",td::nested,ic::ROS_TYPE_MESSAGE,offsetof(Info,stamp),0,1,false,false},
    {"client_gid",td::bytes16,ic::ROS_TYPE_UINT8,offsetof(Info,client_gid),16,none,true,false},
    {"sequence_number",td::i64,ic::ROS_TYPE_INT64,offsetof(Info,sequence_number),0,none,false,false}}},1u<<1},
 {"std_srvs/srv/Empty_Request","std_srvs::srv","Empty_Request",sizeof(Empty::Request),1,
  {{{"structure_needs_at_least_one_member",td::u8,ic::ROS_TYPE_UINT8,offsetof(Empty::Request,structure_needs_at_least_one_member),0,none,false,false}}},0},
 {"std_srvs/srv/Empty_Response","std_srvs::srv","Empty_Response",sizeof(Empty::Response),1,
  {{{"structure_needs_at_least_one_member",td::u8,ic::ROS_TYPE_UINT8,offsetof(Empty::Response,structure_needs_at_least_one_member),0,none,false,false}}},0},
 {"std_srvs/srv/Empty_Event","std_srvs::srv","Empty_Event",sizeof(Empty::Event),3,
  {{{"info",td::nested,ic::ROS_TYPE_MESSAGE,offsetof(Empty::Event,info),0,2,false,false},
    {"request",td::bounded,ic::ROS_TYPE_MESSAGE,offsetof(Empty::Event,request),1,3,true,true},
    {"response",td::bounded,ic::ROS_TYPE_MESSAGE,offsetof(Empty::Event,response),1,4,true,true}}},(1u<<1)|(1u<<2)|(1u<<3)|(1u<<4)},
 {"example_interfaces/srv/AddTwoInts_Request","example_interfaces::srv","AddTwoInts_Request",sizeof(Add::Request),2,
  {{{"a",td::i64,ic::ROS_TYPE_INT64,offsetof(Add::Request,a),0,none,false,false},
    {"b",td::i64,ic::ROS_TYPE_INT64,offsetof(Add::Request,b),0,none,false,false}}},0},
 {"example_interfaces/srv/AddTwoInts_Response","example_interfaces::srv","AddTwoInts_Response",sizeof(Add::Response),1,
  {{{"sum",td::i64,ic::ROS_TYPE_INT64,offsetof(Add::Response,sum),0,none,false,false}}},0},
 {"example_interfaces/srv/AddTwoInts_Event","example_interfaces::srv","AddTwoInts_Event",sizeof(Add::Event),3,
  {{{"info",td::nested,ic::ROS_TYPE_MESSAGE,offsetof(Add::Event,info),0,2,false,false},
    {"request",td::bounded,ic::ROS_TYPE_MESSAGE,offsetof(Add::Event,request),1,6,true,true},
    {"response",td::bounded,ic::ROS_TYPE_MESSAGE,offsetof(Add::Event,response),1,7,true,true}}},(1u<<1)|(1u<<2)|(1u<<6)|(1u<<7)},
 {"std_srvs/srv/Empty","std_srvs::srv","Empty",0,3,
  {{{"request_message",td::nested,0,0,0,3,false,false},{"response_message",td::nested,0,0,0,4,false,false},
    {"event_message",td::nested,0,0,0,5,false,false}}},(1u<<1)|(1u<<2)|(1u<<3)|(1u<<4)|(1u<<5)},
 {"example_interfaces/srv/AddTwoInts","example_interfaces::srv","AddTwoInts",0,3,
  {{{"request_message",td::nested,0,0,0,6,false,false},{"response_message",td::nested,0,0,0,7,false,false},
    {"event_message",td::nested,0,0,0,8,false,false}}},(1u<<1)|(1u<<2)|(1u<<6)|(1u<<7)|(1u<<8)}
}};
static_assert(sizeof(std::int64_t)==8&&sizeof(std::uint32_t)==4);
static_assert(std::is_same_v<decltype(String::data),std::string>);
static_assert(std::is_same_v<decltype(Add::Request::a),std::int64_t>);
static_assert(Info::REQUEST_SENT==0&&Info::REQUEST_RECEIVED==1&&Info::RESPONSE_SENT==2&&Info::RESPONSE_RECEIVED==3);
struct RegistryState {
  NativeIdentities selected;std::size_t string_bound;bool ready{};
  std::array<Message const*,messages> generic{},native{};
  std::array<Service const*,2> services{},native_services{};
  std::array<uwvm_rmw_codec,descriptors> codecs{};
  explicit RegistryState(NativeIdentities const& ids,std::size_t bound):selected{ids},string_bound{bound}{}
};
static bool text(rosidl_runtime_c__String const& v,char const* expected) noexcept {
  auto size=std::strlen(expected);if(v.size!=size)return false;
  if(!v.data)return !size&&!v.capacity;
  /* Actual generated __description.c uses read-only static views whose capacity
   * equals size, not the owned runtime String allocation convention size+1.
   * Compare only the bounded meaningful bytes; never probe data[size]. */
  return v.capacity>=size&&(!size||!std::memcmp(v.data,expected,size));
}
static std::size_t index_name(rosidl_runtime_c__String const& name) noexcept {
  for(std::size_t i{};i!=identities;++i)if(text(name,specs[i].name))return i;return none;
}
static bool individual(rosidl_runtime_c__type_description__IndividualTypeDescription const& v,std::size_t index) noexcept {
  auto const& spec=specs[index];if(!text(v.type_name,spec.name)||v.fields.size!=spec.count||v.fields.capacity<v.fields.size||!v.fields.data)return false;
  for(std::size_t i{};i!=spec.count;++i){auto const& f=v.fields.data[i];auto const& wanted=spec.fields[i];
    if(!text(f.name,wanted.name)||f.type.type_id!=wanted.description_type||f.type.capacity!=wanted.capacity||f.type.string_capacity||
      !text(f.type.nested_type_name,wanted.nested==none?"":specs[wanted.nested].name)||!text(f.default_value,""))return false;}
  return true;
}
static bool description(rosidl_runtime_c__type_description__TypeDescription const* v,std::size_t index) noexcept {
  if(!v||!individual(v->type_description,index))return false;auto const& refs=v->referenced_type_descriptions;
  auto mask=specs[index].references;if(refs.size!=static_cast<std::size_t>(std::popcount(mask))||refs.capacity<refs.size||(!refs.data&&refs.size))return false;
  std::uint16_t seen{};for(std::size_t i{};i!=refs.size;++i){auto at=index_name(refs.data[i].type_name);
    if(at==none||!(mask&(1u<<at))||(seen&(1u<<at))||!individual(refs.data[i],at))return false;seen|=static_cast<std::uint16_t>(1u<<at);}
  return seen==mask;
}
/* Bound source metadata as well as the semantic graph. It is evidence, never a
 * source hash calculator or permission. Sources may use original .msg/.srv or
 * implicit generated IDL encodings; each name must belong to the proven graph. */
static bool sources(rosidl_runtime_c__type_description__TypeSource__Sequence const* v,std::size_t index) noexcept {
  if(!v||!v->size||v->size>identities||v->capacity<v->size||!v->data)return false;
  auto mask=static_cast<std::uint16_t>(specs[index].references|(1u<<index));std::uint16_t seen{};
  for(std::size_t i{};i!=v->size;++i){auto const& source=v->data[i];auto at=index_name(source.type_name);
    if(at==none||!(mask&(1u<<at))||(seen&(1u<<at)))return false;seen|=static_cast<std::uint16_t>(1u<<at);
    auto valid=[](rosidl_runtime_c__String const& s,std::size_t max){return s.size<=max&&s.data&&s.capacity>=s.size;};
    bool implicit=at>=3&&at<=8;
    auto expected_encoding=implicit?"implicit":at>=messages?"srv":"msg";
    if(!text(source.encoding,expected_encoding))return false;
    if(implicit){if(!text(source.raw_file_contents,""))return false;}
    else if(!valid(source.raw_file_contents,4194304)||!source.raw_file_contents.size)return false;
  }
  return seen==mask;
}
template<class Support>static bool metadata(Support const* support,RegistryState const& state,std::size_t index) {
  if(!support||!support->get_type_hash_func||!support->get_type_description_func||!support->get_type_description_sources_func)return false;
  auto hash=support->get_type_hash_func(support);
  return hash&&hash->version==1&&!std::memcmp(hash->value,state.selected.hashes[index].data(),32)&&
    description(support->get_type_description_func(support),index)&&sources(support->get_type_description_sources_func(support),index);
}
static bool members(RegistryState const& s,std::size_t index) noexcept {
  auto h=s.native[index];if(!h||!h->typesupport_identifier||std::strcmp(h->typesupport_identifier,ic::typesupport_identifier)||!h->data||!h->func)return false;
  auto m=static_cast<ic::MessageMembers const*>(h->data);auto const& spec=specs[index];
  if(!m->message_namespace_||std::strcmp(m->message_namespace_,spec.ns)||!m->message_name_||std::strcmp(m->message_name_,spec.local)||
    m->member_count_!=spec.count||m->size_of_!=spec.size||m->has_any_key_member_||!m->members_||!m->init_function||!m->fini_function)return false;
  for(std::size_t i{};i!=spec.count;++i){auto const& f=m->members_[i];auto const& wanted=spec.fields[i];
    if(!f.name_||std::strcmp(f.name_,wanted.name)||f.type_id_!=wanted.native_type||f.string_upper_bound_||f.is_key_||
      f.is_array_!=wanted.array||f.array_size_!=wanted.capacity||f.is_upper_bound_!=wanted.upper||f.offset_!=wanted.offset||f.default_value_||
      f.members_!=(wanted.nested==none?nullptr:s.native[wanted.nested]))return false;
    if(wanted.array){if(!f.size_function||!f.get_const_function||!f.get_function||!f.fetch_function||!f.assign_function||bool(f.resize_function)!=wanted.upper)return false;}
    else if(f.size_function||f.get_const_function||f.get_function||f.fetch_function||f.assign_function||f.resize_function)return false;
  }
  return true;
}
static bool whole_graph(RegistryState const& s) {
  for(std::size_t i{};i!=messages;++i)if(!members(s,i)||!metadata(s.native[i],s,i)||!metadata(s.generic[i],s,i))return false;
  for(std::size_t i{};i!=2;++i){auto h=s.native_services[i];auto const& spec=specs[9+i];
    if(!h||!h->typesupport_identifier||std::strcmp(h->typesupport_identifier,ic::typesupport_identifier)||!h->data||!h->func)return false;
    auto m=static_cast<ic::ServiceMembers const*>(h->data);auto req=i?6u:3u,resp=i?7u:4u,event=i?8u:5u;
    auto generic=s.services[i];if(!generic)return false;
    auto pair=[](Message const* value,Message const* a,Message const* b){return value==a||value==b;};
    if(!pair(generic->request_typesupport,s.generic[req],s.native[req])||
      !pair(generic->response_typesupport,s.generic[resp],s.native[resp])||
      !pair(generic->event_typesupport,s.generic[event],s.native[event])||
      !generic->event_message_create_handle_function||!generic->event_message_destroy_handle_function)return false;
    if(!m->service_namespace_||std::strcmp(m->service_namespace_,spec.ns)||!m->service_name_||std::strcmp(m->service_name_,spec.local)||
      m->request_members_!=s.native[req]->data||m->response_members_!=s.native[resp]->data||m->event_members_!=s.native[event]->data||
      h->request_typesupport!=s.native[req]||h->response_typesupport!=s.native[resp]||h->event_typesupport!=s.native[event]||
      !h->event_message_create_handle_function||!h->event_message_destroy_handle_function||
      !metadata(h,s,9+i)||!metadata(s.services[i],s,9+i))return false;
  }
  return true;
}
static std::uint32_t validate(uwvm_rmw_codec const* c,void const* supplied) noexcept {
#if defined(__cpp_exceptions)
  try {
#endif
  if(!c||!c->validation_state)return 22;auto const& s=*static_cast<RegistryState const*>(c->validation_state);
  if(!s.ready)return 22;std::size_t slot=descriptors;for(std::size_t i{};i!=descriptors;++i)if(&s.codecs[i]==c){slot=i;break;}
  if(slot==descriptors)return 22;static constexpr std::size_t map[]{0,3,4,9,6,7,10};auto id=map[slot];bool root=id>=messages;
  void const* expected=root?static_cast<void const*>(s.services[id-messages]):s.generic[id];
  void const* direct=root?static_cast<void const*>(s.native_services[id-messages]):s.native[id];
  auto bound=slot==0?s.string_bound:slot==1||slot==2||slot==3?0u:slot==5?8u:16u;
  if(c->version!=1||c->kind!=(root?UWVM_RMW_SERVICE_CODEC:UWVM_RMW_MESSAGE_CODEC)||!c->native_type||
    std::strcmp(c->native_type,specs[id].name)||c->generated_support!=expected||(supplied!=expected&&supplied!=direct)||
    std::memcmp(c->native_rihs01,s.selected.hashes[id].data(),32)||c->maximum_wire_bytes!=bound)return 22;
  if(root){auto req=slot==3?1u:4u,resp=slot==3?2u:5u;if(c->encode||c->decode||c->request!=&s.codecs[req]||c->response!=&s.codecs[resp])return 22;}
  else if(!c->encode||!c->decode||c->request||c->response)return 22;
  return whole_graph(s)?0u:22u;
#if defined(__cpp_exceptions)
  }catch(std::bad_alloc const&){return 12;}catch(...){return 22;}
#endif
}
static std::uint64_t get(unsigned char const* p) noexcept {std::uint64_t x{};for(unsigned i{};i!=8;++i)x|=std::uint64_t{p[i]}<<(8*i);return x;}
static void put(unsigned char* p,std::uint64_t v) noexcept {for(unsigned i{};i!=8;++i)p[i]=static_cast<unsigned char>(v>>(8*i));}
static std::uint32_t string_encode(void const* value,void* bytes,std::size_t cap,std::size_t* length) noexcept {
  if(!value||!length||(!bytes&&cap))return 22;auto const& v=static_cast<String const*>(value)->data;
  if(v.size()>UWVM_RMW_MAX_CANONICAL_BYTES-4||v.size()+4>cap)return 90;
  if(!roswasmc_utf8({v.data(),v.size()},1))return 22;auto out=static_cast<unsigned char*>(bytes);
  uwvm_ros_store_u32(out,static_cast<std::uint32_t>(v.size()));if(!v.empty())std::memcpy(out+4,v.data(),v.size());*length=v.size()+4;return 0;
}
static std::uint32_t string_decode(void const* bytes,std::size_t length,void* object) noexcept {
#if defined(__cpp_exceptions)
  try {
#endif
  if(!object||!bytes||length<4||length>UWVM_RMW_MAX_CANONICAL_BYTES)return 22;
  auto p=static_cast<unsigned char const*>(bytes);auto n=static_cast<std::uint32_t>(p[0])|(std::uint32_t{p[1]}<<8)|(std::uint32_t{p[2]}<<16)|(std::uint32_t{p[3]}<<24);
  if(n!=length-4||!roswasmc_utf8({reinterpret_cast<char const*>(p+4),n},1))return 22;
  std::string next{reinterpret_cast<char const*>(p+4),n};static_cast<String*>(object)->data.swap(next);return 0;
#if defined(__cpp_exceptions)
  }catch(std::bad_alloc const&){return 12;}catch(...){return 22;}
#endif
}
template<class T>static std::uint32_t empty_encode(void const* object,void*,std::size_t cap,std::size_t* length) noexcept {
  if(!object||!length||cap)return 22;
  /* The actual .srv has no fields; rosidl_adapter inserts this native IDL byte
   * only for an empty struct. Native metadata/hash still include and validate it.
   * Admit only the default zero placeholder: a caller-written nonzero byte is
   * refused unchanged, never silently discarded as a canonical empty value. */
  if(static_cast<T const*>(object)->structure_needs_at_least_one_member)return 22;
  *length=0;return 0;
}
template<class T>static std::uint32_t empty_decode(void const*,std::size_t length,void* object) noexcept {
  if(!object||length)return 22;static_cast<T*>(object)->structure_needs_at_least_one_member=0;return 0;
}
static std::uint32_t request_encode(void const* object,void* output,std::size_t cap,std::size_t* length) noexcept {
  if(!object||!output||!length)return 22;if(cap<16)return 90;auto const& v=*static_cast<Add::Request const*>(object);auto p=static_cast<unsigned char*>(output);
  put(p,std::bit_cast<std::uint64_t>(v.a));put(p+8,std::bit_cast<std::uint64_t>(v.b));*length=16;return 0;
}
static std::uint32_t request_decode(void const* input,std::size_t length,void* object) noexcept {
  if(!input||!object||length!=16)return 22;auto p=static_cast<unsigned char const*>(input);Add::Request next;
  next.a=std::bit_cast<std::int64_t>(get(p));next.b=std::bit_cast<std::int64_t>(get(p+8));*static_cast<Add::Request*>(object)=next;return 0;
}
static std::uint32_t response_encode(void const* object,void* output,std::size_t cap,std::size_t* length) noexcept {
  if(!object||!output||!length)return 22;if(cap<8)return 90;put(static_cast<unsigned char*>(output),std::bit_cast<std::uint64_t>(static_cast<Add::Response const*>(object)->sum));*length=8;return 0;
}
static std::uint32_t response_decode(void const* input,std::size_t length,void* object) noexcept {
  if(!input||!object||length!=8)return 22;auto n=std::bit_cast<std::int64_t>(get(static_cast<unsigned char const*>(input)));static_cast<Add::Response*>(object)->sum=n;return 0;
}
template<class T>static bool message_handles(RegistryState& state,std::size_t index) {
  auto generic=::rosidl_typesupport_cpp::get_message_type_support_handle<T>();auto native=ic::get_message_type_support_handle<T>();
  if(!generic||!native||!generic->func||generic->func(generic,ic::typesupport_identifier)!=native)return false;
  state.generic[index]=generic;state.native[index]=native;return true;
}
template<class T>static bool service_handles(RegistryState& state,std::size_t index) {
  auto generic=::rosidl_typesupport_cpp::get_service_type_support_handle<T>();auto native=ic::get_service_type_support_handle<T>();
  if(!generic||!native||!generic->func||generic->func(generic,ic::typesupport_identifier)!=native)return false;
  state.services[index]=generic;state.native_services[index]=native;return true;
}
static std::uint32_t initialize(RegistryState& s) {
  if(s.string_bound<4||s.string_bound>UWVM_RMW_MAX_CANONICAL_BYTES)return 22;
  for(auto const& h:s.selected.hashes){unsigned nonzero{};for(auto b:h)nonzero|=b;if(!nonzero)return 22;}
  if(!message_handles<String>(s,0)||!message_handles<Time>(s,1)||!message_handles<Info>(s,2)||
    !message_handles<Empty::Request>(s,3)||!message_handles<Empty::Response>(s,4)||!message_handles<Empty::Event>(s,5)||
    !message_handles<Add::Request>(s,6)||!message_handles<Add::Response>(s,7)||!message_handles<Add::Event>(s,8)||
    !service_handles<Empty>(s,0)||!service_handles<Add>(s,1)||!whole_graph(s))return 22;
  static constexpr std::size_t map[]{0,3,4,9,6,7,10};
  for(std::size_t i{};i!=descriptors;++i){auto id=map[i];auto& c=s.codecs[i];c.version=1;c.kind=id<messages?UWVM_RMW_MESSAGE_CODEC:UWVM_RMW_SERVICE_CODEC;
    c.native_type=specs[id].name;c.generated_support=id<messages?static_cast<void const*>(s.generic[id]):s.services[id-messages];
    std::memcpy(c.native_rihs01,s.selected.hashes[id].data(),32);c.maximum_wire_bytes=i==0?s.string_bound:i==1||i==2||i==3?0:i==5?8:16;
    c.validate=validate;c.validation_state=&s;}
  s.codecs[0].encode=string_encode;s.codecs[0].decode=string_decode;
  s.codecs[1].encode=empty_encode<Empty::Request>;s.codecs[1].decode=empty_decode<Empty::Request>;
  s.codecs[2].encode=empty_encode<Empty::Response>;s.codecs[2].decode=empty_decode<Empty::Response>;
  s.codecs[3].request=&s.codecs[1];s.codecs[3].response=&s.codecs[2];
  s.codecs[4].encode=request_encode;s.codecs[4].decode=request_decode;s.codecs[5].encode=response_encode;s.codecs[5].decode=response_decode;
  s.codecs[6].request=&s.codecs[4];s.codecs[6].response=&s.codecs[5];s.ready=true;
  for(auto const& c:s.codecs)if(validate(&c,c.generated_support)){s.ready=false;return 22;}return 0;
}
}
namespace uwvm_rmw_cpp {
Registry::Registry(NativeIdentities const& expected,std::size_t bound) noexcept {
#if defined(__cpp_exceptions)
  try {
#endif
  state_=std::make_unique<detail::RegistryState>(expected,bound);error_=detail::initialize(*state_);
  if(error_)state_->ready=false;
#if defined(__cpp_exceptions)
  }catch(std::bad_alloc const&){error_=12;if(state_)state_->ready=false;}catch(...){error_=22;if(state_)state_->ready=false;}
#endif
}
Registry::~Registry()=default;
std::span<uwvm_rmw_codec const> Registry::codecs()const noexcept {return error_||!state_||!state_->ready?std::span<uwvm_rmw_codec const>{}:std::span<uwvm_rmw_codec const>{state_->codecs};}
uwvm_rmw_codec const* Registry::codec(Codec kind)const noexcept {auto values=codecs();auto i=static_cast<std::size_t>(kind);return i<values.size()?&values[i]:nullptr;}
}
