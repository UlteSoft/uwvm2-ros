// SPDX-License-Identifier: Apache-2.0
#include "profile_internal.hpp"
#include "../common/config.h"
#include "selection.hpp"
#include "registry.hpp"
#include "default_control_gateway_features.h"
#include <array>
#include <atomic>
#include <cstring>
#include <new>
#if UWVM_DEFAULT_CONTROL_GATEWAY_NATIVE_API
#include <rosidl_typesupport_introspection_c/message_introspection.h>
#include <rosidl_typesupport_introspection_c/service_introspection.h>
#include <rosidl_typesupport_introspection_c/identifier.h>
#include <rosidl_typesupport_introspection_cpp/message_introspection.hpp>
#include <rosidl_typesupport_introspection_cpp/service_introspection.hpp>
#include <rosidl_typesupport_introspection_cpp/identifier.hpp>
#include <rosidl_runtime_c/type_description/type_description__struct.h>
#endif
using namespace uwvm_default_control_gateway_detail;
namespace uwvm2::ros_gateway::default_control_gateway {
class profile {public:
 profile()=default;profile(profile const&)=delete;profile&operator=(profile const&)=delete;profile(profile&&)=delete;profile&operator=(profile&&)=delete;
 profile const*self{this};bool ready{};
 Graph const*graph{};std::size_t count{};
 std::array<std::array<std::uint8_t,32>,maximum_identities>hashes{};
 std::array<rosidl_message_type_support_t const*,maximum_nodes>message{},native_message{};
 std::array<rosidl_service_type_support_t const*,maximum_nodes>service{},native_service{};
 std::array<uwvm_rmw_codec,maximum_codecs>codecs{};
};
}
using uwvm2::ros_gateway::default_control_gateway::profile;
namespace {
// Each actual server/Profile has a distinct nonmoving owned state. Endpoint
// callbacks strongly retain it; final shared destruction reclaims this state.
#if UWVM_DEFAULT_CONTROL_GATEWAY_NATIVE_API
namespace ipp=rosidl_typesupport_introspection_cpp;
using Message=rosidl_message_type_support_t;using Service=rosidl_service_type_support_t;
static bool text(rosidl_runtime_c__String const&actual,std::string_view expected)noexcept {
 // Real generated metadata may use immutable capacity==size views. Do not read
 // an unpromised data[size] terminator as if these were owning object strings.
 return actual.size==expected.size()&&actual.capacity>=actual.size&&
 (actual.data?(!actual.size||!std::memcmp(actual.data,expected.data(),actual.size)):!actual.size&&!actual.capacity);
}
static bool individual(rosidl_runtime_c__type_description__IndividualTypeDescription const&v,Node const&n)noexcept {
 if(!text(v.type_name,n.name)||v.fields.size!=n.fields.size()||v.fields.capacity<v.fields.size||(!v.fields.data&&v.fields.size))return false;
 for(std::size_t i{};i<n.fields.size();++i){auto const&a=v.fields.data[i];auto const&f=n.fields[i];
 if(!text(a.name,f.name)||a.type.type_id!=f.description_type||a.type.capacity!=f.native_capacity||a.type.string_capacity!=f.native_string_capacity||!text(a.type.nested_type_name,f.nested_type)||!text(a.default_value,f.native_default))return false;}
 return true;
}
static bool description(rosidl_runtime_c__type_description__TypeDescription const*p,Graph const&g,std::size_t at)noexcept {
 if(!p||!individual(p->type_description,g.nodes[at]))return false;
 auto const&refs=p->referenced_type_descriptions;auto wanted=g.nodes[at].references;
 if(refs.size!=wanted.size()||refs.capacity<refs.size||(!refs.data&&refs.size))return false;std::array<bool,maximum_nodes>seen{};
 for(std::size_t i{};i<refs.size;++i){std::size_t j{};for(;j<wanted.size();++j)if(text(refs.data[i].type_name,g.nodes[wanted[j]].name))break;
 if(j==wanted.size()||seen[j]||!individual(refs.data[i],g.nodes[wanted[j]]))return false;seen[j]=true;}
 return true;
}
static bool sources(rosidl_runtime_c__type_description__TypeSource__Sequence const*p,Graph const&g,std::size_t at)noexcept {
 auto refs=g.nodes[at].references;if(!p||p->size!=refs.size()+1||p->capacity<p->size||!p->data)return false;std::array<bool,maximum_nodes>seen{};
 for(std::size_t i{};i<p->size;++i){auto const&a=p->data[i];std::size_t selected=maximum_nodes,index=maximum_nodes;
 if(text(a.type_name,g.nodes[at].name)){selected=at;index=0;}else for(std::size_t j{};j<refs.size();++j)if(text(a.type_name,g.nodes[refs[j]].name)){selected=refs[j];index=j+1;break;}
 if(selected==maximum_nodes||index>=seen.size()||seen[index])return false;seen[index]=true;auto const&n=g.nodes[selected];
 if(!text(a.encoding,n.source_encoding)||!text(a.raw_file_contents,n.source_text))return false;}
 return true;
}
template<class Support>static bool metadata(Support const*p,profile const&s,std::size_t at){
 if(!p||!p->get_type_hash_func||!p->get_type_description_func||!p->get_type_description_sources_func)return false;
 auto h=p->get_type_hash_func(p);auto const&n=s.graph->nodes[at];return h&&h->version==1&&!std::memcmp(h->value,s.hashes[n.identity].data(),32)&&
 description(p->get_type_description_func(p),*s.graph,at)&&sources(p->get_type_description_sources_func(p),*s.graph,at);
}
template<class Members>static bool members(Members const*m,profile const&s,std::size_t at){
 auto const&g=*s.graph;auto const&n=g.nodes[at];auto l=n.layout();
 if(!m||!m->message_namespace_||n.native_namespace!=m->message_namespace_||!m->message_name_||n.native_local!=m->message_name_||m->member_count_!=n.fields.size()||m->size_of_!=l.size||m->has_any_key_member_||!m->init_function||!m->fini_function||l.offsets.size()!=n.fields.size()||(!m->members_&&m->member_count_))return false;
 for(std::size_t i{};i<n.fields.size();++i){auto const&a=m->members_[i];auto const&f=n.fields[i];
 if(!a.name_||f.name!=a.name_||a.type_id_!=f.introspection_type||a.string_upper_bound_!=f.native_string_capacity||a.is_key_||a.is_array_!=f.array||a.array_size_!=f.native_capacity||a.is_upper_bound_!=f.upper||a.offset_!=l.offsets[i])return false;
 // default_value_ is UNKNOWN, regardless of nullness. Complete generic AND
 // introspection modern metadata/source/hash/default proof precedes this pass.
 if(f.nested_type.empty()){if(a.members_)return false;}else {std::size_t child{};for(;child<g.nodes.size();++child)if(g.nodes[child].language==n.language&&g.nodes[child].kind==Kind::message&&g.nodes[child].name==f.nested_type)break;
 if(child==g.nodes.size()||a.members_!=s.native_message[child])return false;}
 if(f.array){if(!a.size_function||!a.fetch_function||!a.assign_function)return false;bool sequence=f.upper||!f.native_capacity;
 if(bool(a.resize_function)!=sequence)return false;if(f.vector_bool){if(a.get_const_function||a.get_function)return false;}else if(!a.get_const_function||!a.get_function)return false;}
 else if(a.size_function||a.get_const_function||a.get_function||a.fetch_function||a.assign_function||a.resize_function)return false;
 }
 return true;
}
static bool whole(profile const&s){
 auto const&g=*s.graph;
 for(std::size_t i{};i<g.nodes.size();++i){auto const&n=g.nodes[i];if(n.kind==Kind::message){if(!metadata(s.message[i],s,i)||!metadata(s.native_message[i],s,i))return false;}
 else if(!metadata(s.service[i],s,i)||!metadata(s.native_service[i],s,i))return false;}
 for(std::size_t i{};i<g.nodes.size();++i){auto const&n=g.nodes[i];auto id=n.language==Language::c?rosidl_typesupport_introspection_c__identifier:ipp::typesupport_identifier;
 if(n.kind==Kind::message){auto p=s.native_message[i];if(!p||!p->typesupport_identifier||std::strcmp(p->typesupport_identifier,id)||!p->data||!p->func||!n.layout)return false;
 if(n.language==Language::c){if(!members(static_cast<rosidl_typesupport_introspection_c__MessageMembers const*>(p->data),s,i))return false;}
 else if(!members(static_cast<ipp::MessageMembers const*>(p->data),s,i))return false;}
 else {auto generic=s.service[i];auto p=s.native_service[i];auto q=n.service_components;
 if(!generic||!p||!p->typesupport_identifier||std::strcmp(p->typesupport_identifier,id)||!p->data||!p->func)return false;
 for(auto j:q)if(j>=g.nodes.size()||!s.native_message[j]||!s.native_message[j]->data)return false;
 auto check=[&](auto const*m){return m&&m->service_namespace_&&n.native_namespace==m->service_namespace_&&m->service_name_&&n.native_local==m->service_name_&&m->request_members_==s.native_message[q[0]]->data&&m->response_members_==s.native_message[q[1]]->data&&m->event_members_==s.native_message[q[2]]->data;};
 if(n.language==Language::c){if(!check(static_cast<rosidl_typesupport_introspection_c__ServiceMembers const*>(p->data)))return false;}
 else if(!check(static_cast<ipp::ServiceMembers const*>(p->data)))return false;
 auto direct=[&](Service const*x,bool exact){auto same=[&](Message const*y,std::size_t j){return y==s.native_message[j]||(!exact&&y==s.message[j]);};
 return same(x->request_typesupport,q[0])&&same(x->response_typesupport,q[1])&&same(x->event_typesupport,q[2])&&x->event_message_create_handle_function&&x->event_message_destroy_handle_function;};
 if(!direct(p,true)||!direct(generic,false))return false;}
 }
 return true;
}
static std::uint32_t validate(uwvm_rmw_codec const*c,void const*support)noexcept {
 try {if(!c||!c->validation_state)return 22;auto const&s=*static_cast<profile const*>(c->validation_state);
 if(s.self!=&s||!s.ready||!s.graph)return 22;std::size_t slot{};for(;slot<s.count;++slot)if(c==&s.codecs[slot])break;if(slot==s.count)return 22;
 auto const&spec=s.graph->codecs[slot];auto const&n=s.graph->nodes[spec.node];bool service=n.kind==Kind::service;
 void const*generic=service?static_cast<void const*>(s.service[spec.node]):s.message[spec.node];void const*native=service?static_cast<void const*>(s.native_service[spec.node]):s.native_message[spec.node];
 if(c->version!=1||c->kind!=(service?UWVM_RMW_SERVICE_CODEC:UWVM_RMW_MESSAGE_CODEC)||!c->native_type||n.name!=c->native_type||c->generated_support!=generic||(support!=generic&&support!=native)||std::memcmp(c->native_rihs01,s.hashes[n.identity].data(),32)||c->maximum_wire_bytes!=spec.maximum||c->encode!=spec.encode||c->decode!=spec.decode||c->validate!=&validate)return 22;
 if(service){if(spec.request>=s.count||spec.response>=s.count||c->request!=&s.codecs[spec.request]||c->response!=&s.codecs[spec.response]||c->encode||c->decode)return 22;}
 else if(c->request||c->response||!c->encode||!c->decode)return 22;
 return whole(s)?0u:22u;
 }catch(std::bad_alloc const&){return 12;}catch(...){return 22;}
}
static bool valid_graph(Graph const&g)noexcept {
 if(g.identity_names.empty()||g.identity_names.size()>maximum_identities||g.nodes.empty()||g.nodes.size()>maximum_nodes||g.codecs.empty()||g.codecs.size()>maximum_codecs)return false;
 for(auto const&n:g.nodes){if(n.identity>=g.identity_names.size()||n.name!=g.identity_names[n.identity]||n.name.empty()||n.references.size()+1>maximum_nodes)return false;
 for(auto j:n.references)if(j>=g.nodes.size()||g.nodes[j].language!=n.language)return false;}
 for(auto const&c:g.codecs){if(c.node>=g.nodes.size()||c.maximum>32768||c.minimum>c.maximum||c.canonical_identity.empty())return false;
 if(g.nodes[c.node].kind==Kind::message&&(!c.body||!c.encode||!c.decode))return false;}
 return true;
}
#endif
}
namespace uwvm2::ros_gateway::default_control_gateway {
std::uint32_t prepare(identity const*ids,std::size_t count,std::shared_ptr<profile const>&output)noexcept {
 if(!ids)return 22;
#if !UWVM_DEFAULT_CONTROL_GATEWAY_NATIVE_API
 (void)count;(void)output;return 95;
#else
 try {auto const&g=generated_graph();if(!valid_graph(g)||count!=g.identity_names.size())return 22;
 for(std::size_t i{};i<count;++i){if(!ids[i].native_type||g.identity_names[i]!=ids[i].native_type)return 22;bool nonzero{};for(auto b:ids[i].rihs01)nonzero|=b!=0;if(!nonzero)return 22;}
 auto owned=std::make_shared<profile>();auto*s=owned.get();s->graph=&g;s->count=g.codecs.size();
 for(std::size_t i{};i<count;++i)std::memcpy(s->hashes[i].data(),ids[i].rihs01,32);
 for(std::size_t i{};i<g.nodes.size();++i){auto const&n=g.nodes[i];if(n.language!=Language::cpp)return 22;
 if(n.kind==Kind::message){if(!n.message_generic||!n.message_introspection)return 22;s->message[i]=n.message_generic();s->native_message[i]=n.message_introspection();if(!s->message[i]||!s->native_message[i])return 95;}
 else {if(!n.service_generic||!n.service_introspection)return 22;s->service[i]=n.service_generic();s->native_service[i]=n.service_introspection();if(!s->service[i]||!s->native_service[i])return 95;}}
 if(!whole(*s))return 22;
 for(auto const&n:g.nodes)if(n.kind==Kind::message){if(!n.default_contract)return 22;auto error=n.default_contract();if(error)return error;}
 for(std::size_t i{};i<g.codecs.size();++i){auto const&spec=g.codecs[i];auto const&n=g.nodes[spec.node];auto&c=s->codecs[i];bool service=n.kind==Kind::service;
 c.version=1;c.kind=service?UWVM_RMW_SERVICE_CODEC:UWVM_RMW_MESSAGE_CODEC;c.native_type=n.name.data();c.generated_support=service?static_cast<void const*>(s->service[spec.node]):s->message[spec.node];
 std::memcpy(c.native_rihs01,s->hashes[n.identity].data(),32);c.maximum_wire_bytes=spec.maximum;c.validate=&validate;c.encode=spec.encode;c.decode=spec.decode;c.validation_state=s;
 if(service){if(spec.request>=g.codecs.size()||spec.response>=g.codecs.size())return 22;c.request=&s->codecs[spec.request];c.response=&s->codecs[spec.response];}}
 s->ready=true;for(std::size_t i{};i<s->count;++i)if(auto e=validate(&s->codecs[i],s->codecs[i].generated_support))return e;
 output=std::move(owned);return 0;
 }catch(std::bad_alloc const&){return 12;}catch(...){return 22;}
#endif
}
}
namespace uwvm_default_control_gateway_detail {
using namespace ::uwvm2::ros_gateway;
using ::uwvm2::ros_gateway::default_control_gateway::profile;
uwvm_rmw_codec const* codec(profile const&p,std::string_view name)noexcept {
 if(p.self!=&p||!p.ready||!p.graph)return nullptr;
 for(std::size_t i{};i<p.count;++i)if(p.graph->nodes[p.graph->codecs[i].node].name==name)return &p.codecs[i];return nullptr;
}
static CodecSpec const* spec(profile const&p,std::string_view name)noexcept {
 auto c=codec(p,name);return c?&p.graph->codecs[static_cast<std::size_t>(c-p.codecs.data())]:nullptr;
}
std::uint32_t body(profile const&p,std::string_view name,bool response,void const*bytes,std::size_t size)noexcept {
 auto s=spec(p,name);if(!s)return 95;
 if(p.graph->nodes[s->node].kind==Kind::service){auto i=response?s->response:s->request;if(i>=p.count)return 13;s=&p.graph->codecs[i];}
 if(!s->body)return 95;if(!bytes&&size)return 22;if(size>s->maximum)return 90;return s->body(bytes,size);
}
std::uint32_t select(profile const&p,ros::capability_spec const&cap,uwvm_rmw_codec const*&root)noexcept {
 try {auto c=codec(p,cap.type);auto s=spec(p,cap.type);if(!c||!s)return 95;
 bool service=c->kind==UWVM_RMW_SERVICE_CODEC;
 if((service&&(cap.operation!="service.call"&&cap.operation!="service.advertise"&&cap.operation!="service.respond"))||
 (!service&&(cap.operation!="topic.publish"&&cap.operation!="topic.subscribe")))return 13;
 auto at=cap.limits.find("adapter-codec-identity");
 if(at==cap.limits.end()||at->second!=s->canonical_identity||cap.type_identity!=identity_text(c->native_rihs01)||
 cap.alias.empty()||cap.resource.empty()||cap.resource.front()!='/'||field(cap,"validator")!="none"||!explicit_qos(cap))return 13;
 if(service){if(!c->request||!c->response||limit(cap,"max-request-bytes")<c->request->maximum_wire_bytes||
 limit(cap,"max-response-bytes")<c->response->maximum_wire_bytes||!limit(cap,"max-inflight")||limit(cap,"max-inflight")>256||
 !limit(cap,"timeout-ms")||limit(cap,"timeout-ms")>3600000)return 90;}
 else if(limit(cap,"max-message-bytes")<s->minimum)return 90;
 root=c;return 0;
 }catch(std::bad_alloc const&){return 12;}catch(...){return 22;}
}
}
namespace uwvm2::ros_gateway::default_control_gateway {
bool known(std::shared_ptr<profile const>const&p,ros::capability_spec const&cap)noexcept {
 uwvm_rmw_codec const*root{};return p&&!uwvm_default_control_gateway_detail::select(*p,cap,root);
}
std::uint32_t canonical_bounds(std::shared_ptr<profile const>const&p,ros::capability_spec const&cap,canonical_limits&out)noexcept {
 if(!p)return 95;uwvm_rmw_codec const*r{};auto e=uwvm_default_control_gateway_detail::select(*p,cap,r);if(e)return e;
 if(r->kind!=UWVM_RMW_SERVICE_CODEC)return 13;canonical_limits n{r->request->maximum_wire_bytes,r->response->maximum_wire_bytes,1000};out=n;return 0;
}
std::uint32_t validate_body(std::shared_ptr<profile const>const&p,ros::capability_spec const&cap,bool response,std::uint8_t const*b,std::size_t n)noexcept {
 if(!p)return 95;uwvm_rmw_codec const*r{};auto e=uwvm_default_control_gateway_detail::select(*p,cap,r);if(e)return e;
 return uwvm_default_control_gateway_detail::body(*p,cap.type,response,b,n);
}
}
