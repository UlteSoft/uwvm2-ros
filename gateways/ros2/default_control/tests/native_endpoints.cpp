#include "../../compat/node.h"
// SPDX-License-Identifier: Apache-2.0
// Genuine selected Native objects and direct-RCL endpoints. Authored only.
// Each mode runs under a separate externally finite native process owner.
#include "default_control_gateway.h"
#include "default_control_gateway_identity_input.hpp"
#include "profile_internal.hpp"
#include "native_rcl_service_registry.h"
#include "native_rmw_subscription.h"
#include <rclcpp/rclcpp.hpp>
#include <rcl_interfaces/msg/log.hpp>
#include <rcl_interfaces/msg/parameter_event.hpp>
#include <rcl_interfaces/srv/describe_parameters.hpp>
#include <rcl_interfaces/srv/get_parameters.hpp>
#include <rcl_interfaces/srv/get_parameter_types.hpp>
#include <rcl_interfaces/srv/list_parameters.hpp>
#include <rcl_interfaces/srv/set_parameters.hpp>
#include <rcl_interfaces/srv/set_parameters_atomically.hpp>
#include <type_description_interfaces/srv/get_type_description.hpp>
#include <fast_io.h>
#include <cstring>
#include <stdexcept>
using namespace uwvm2::ros_gateway;
namespace control=default_control_gateway;
namespace detail=uwvm_default_control_gateway_detail;
namespace wire=::uwvm2::imported::ros::ipc::rmw_endpoint;
namespace info=::uwvm2::imported::ros::ipc::rmw_metadata;
using Clock=std::chrono::steady_clock;
#define REQUIRE(x) do{if(!(x)){fast_io::perrln("control Gateway contract line ",__LINE__);throw std::runtime_error("contract");}}while(false)
static ros::capability_spec cap(control::profile const&p,std::string name,std::string operation,std::string alias,std::string resource){
 auto root=detail::codec(p,name);REQUIRE(root);auto const&g=detail::generated_graph();auto at=std::find_if(g.codecs.begin(),g.codecs.end(),[&](auto const&s){return g.nodes[s.node].name==name;});REQUIRE(at!=g.codecs.end());
 ros::capability_spec c{};c.type=std::move(name);c.operation=std::move(operation);c.alias=std::move(alias);c.resource=std::move(resource);c.type_identity=native_rcl_service::identity_text(root->native_rihs01);
 c.limits={{"adapter-codec-identity",std::string{at->canonical_identity}},{"validator","none"},{"qos.history","keep-last"},{"qos.depth","1000"},{"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.liveliness","automatic"},{"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness-lease-duration","infinite"},
 {"max-inflight","4"},{"timeout-ms","2000"},{"max-request-bytes",std::to_string(root->request?root->request->maximum_wire_bytes:0)},{"max-response-bytes",std::to_string(root->response?root->response->maximum_wire_bytes:0)},
 {"max-message-bytes",std::to_string(root->maximum_wire_bytes)},{"max-queue-bytes","262144"},{"max-bytes-per-second","1048576"},{"burst","4"},{"overflow-policy","reject"}};
 return c;
}
static std::vector<std::uint8_t> encode(uwvm_rmw_codec const*c,void const*object){
 REQUIRE(c&&c->encode);std::vector<std::uint8_t> bytes(c->maximum_wire_bytes);std::size_t n=SIZE_MAX;
 REQUIRE(c->encode(object,bytes.data(),bytes.size(),&n)==0&&n<=bytes.size());bytes.resize(n);return bytes;
}
static void poll_until(std::shared_ptr<native_rcl_service_registry::endpoint>const&e,wire::binding owner,std::uint64_t id,rclcpp::executors::SingleThreadedExecutor&executor,Clock::time_point until){
 for(;;){REQUIRE(Clock::now()<until);auto error=e->poll(owner,id);if(!error)return;REQUIRE(error==11);executor.spin_once(std::chrono::milliseconds{1});}
}
static native_rcl_service::snapshot borrow(std::shared_ptr<native_rcl_service_registry::endpoint>const&e,wire::binding owner,std::uint64_t id){
 native_rcl_service::snapshot first{};REQUIRE(e->peek(owner,id,sizeof(rmw_request_id_t::writer_guid),first,32768+info::header_bytes)==0);
 auto sequence=first.sequence,reservation=first.reservation;REQUIRE(e->cancel(owner,id,sequence,reservation)==0);
 native_rcl_service::snapshot second{};REQUIRE(e->peek(owner,id,sizeof(rmw_request_id_t::writer_guid),second,32768+info::header_bytes)==0);
 REQUIRE(second.sequence==sequence&&second.reservation!=reservation);REQUIRE(e->commit(owner,id,sequence,reservation)==13);
 REQUIRE(second.size==first.size&&!std::memcmp(first.bytes.get(),second.bytes.get(),first.size));return second;
}
static info::header header(native_rcl_service::snapshot const&s){info::header h{};REQUIRE(s.bytes&&s.size>=info::header_bytes&&info::decode(s.bytes.get(),info::header_bytes,s.size-info::header_bytes,h)&&info::complete(h));return h;}
template<class Service>static void roundtrip(std::shared_ptr<control::profile const>const&p,::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&node,rclcpp::executors::SingleThreadedExecutor&executor,std::string name,typename Service::Request const&request,typename Service::Response const&response,unsigned ordinal){
 auto root=detail::codec(*p,name);REQUIRE(root&&root->generated_support==rosidl_typesupport_cpp::get_service_type_support_handle<Service>());
 auto text="/control_gateway_contract/manual_"+std::to_string(ordinal);auto client=cap(*p,name,"service.call","call-"+std::to_string(ordinal),text),server=cap(*p,name,"service.advertise","ad-"+std::to_string(ordinal),text),responder=cap(*p,name,"service.respond","response-"+std::to_string(ordinal),text);
 wire::binding c{2,0x100000001ULL+ordinal*3,11,12,13},s{2,c.source+1,11,12,13},r{2,c.source+2,11,12,13};
 auto cr=std::make_shared<native_rcl_service::report>(),sr=std::make_shared<native_rcl_service::report>();std::shared_ptr<native_rcl_service_registry::endpoint>ce,se;wire::packet cp{},sp{};
 // The actual default-control proof is independent of the optional old CPP10
 // Registry; the same public factory signature still accepts it when present.
 REQUIRE(control::create_service(p,node,{},client,c,100+ordinal,8,cr,ce,cp)==0);
 struct Guard{std::shared_ptr<native_rcl_service_registry::endpoint>*c,*s;wire::binding co,so;std::uint64_t id;~Guard(){if(*s)(void)(*s)->retire(so,id);if(*c)(void)(*c)->retire(co,id);}}guard{&ce,&se,c,s,100+ordinal};
 REQUIRE(control::create_service(p,node,{},server,s,100+ordinal,8,sr,se,sp)==0);REQUIRE(wire::valid_response(cp)&&wire::valid_response(sp));
 wire::packet observed{};REQUIRE(ce->inspect(c,100+ordinal,observed)==0&&wire::same(observed.owner,c));
 auto request_bytes=encode(root->request,&request);auto response_bytes=encode(root->response,&response);
 REQUIRE(control::validate_body(p,client,false,request_bytes.data(),request_bytes.size())==0);
 auto bad=request_bytes;bad.push_back(0);auto sends=cr->observe().native[static_cast<unsigned>(native_rcl_service::operation::send)].calls;std::uint64_t ticket=UINT64_MAX;
 REQUIRE(ce->submit(c,100+ordinal,bad.data(),bad.size(),ticket)==22&&ticket==UINT64_MAX);REQUIRE(cr->observe().native[static_cast<unsigned>(native_rcl_service::operation::send)].calls==sends);
 auto until=Clock::now()+std::chrono::seconds{5};REQUIRE(Clock::now()<until);
 // Explicit additional Native-only query grant, paired to this SAME client.
 // Real readiness is observed before the one send; graph names/counts do not
 // stand in for availability and no additional probe client is created.
 auto readiness=client;readiness.operation="service.available";readiness.alias="available-"+std::to_string(ordinal);
 readiness.limits.erase("validator");readiness.limits["service-source-alias"]=client.alias;
 readiness.limits["max-inflight"]="1";readiness.limits["max-request-bytes"]="64";readiness.limits["max-response-bytes"]="64";
 readiness.limits["max-wait-ns"]="1000000000";readiness.limits["max-native-clients"]="1";readiness.limits["max-binding-history"]="8";
 auto readiness_owner=c;readiness_owner.source=0x200000000ULL+ordinal;
 for(;;){REQUIRE(Clock::now()<until);bool ready{};REQUIRE(ce->available(c,100+ordinal,readiness_owner,readiness,ready)==0);if(ready)break;executor.spin_once(std::chrono::milliseconds{1});}
 REQUIRE(Clock::now()<until);
 REQUIRE(ce->submit(c,100+ordinal,request_bytes.data(),request_bytes.size(),ticket)==0&&ticket!=UINT64_MAX);
 poll_until(se,s,100+ordinal,executor,until);auto incoming=borrow(se,s,100+ordinal);auto ih=header(incoming);
 REQUIRE(ih.kind==ipc::event_kind::service_request&&wire::same(ih.owner,s)&&ih.payload_bytes==request_bytes.size());
 REQUIRE(!std::memcmp(incoming.bytes.get()+info::header_bytes,request_bytes.data(),request_bytes.size()));
 REQUIRE(se->commit(s,100+ordinal,incoming.sequence,incoming.reservation)==0);
 REQUIRE(se->respond(s,100+ordinal,r,responder,ih.correlation,response_bytes.data(),response_bytes.size())==0);
 REQUIRE(se->respond(s,100+ordinal,r,responder,ih.correlation,response_bytes.data(),response_bytes.size())==125);
 poll_until(ce,c,100+ordinal,executor,until);auto reply=borrow(ce,c,100+ordinal);auto rh=header(reply);
 REQUIRE(rh.kind==ipc::event_kind::service_response&&wire::same(rh.owner,c)&&rh.correlation==ticket&&rh.payload_bytes==response_bytes.size());
 REQUIRE(rh.writer_size==ih.writer_size&&rh.request_sequence==ih.request_sequence&&!std::memcmp(rh.writer.data(),ih.writer.data(),rh.writer_size));
 // No positivity assumption for signed native sequence/times, no nonzero-GID
 // assumption, and no substitution of local tickets for the actual request ID.
 REQUIRE(!std::memcmp(reply.bytes.get()+info::header_bytes,response_bytes.data(),response_bytes.size()));
 typename Service::Response decoded{};REQUIRE(root->response->decode(reply.bytes.get()+info::header_bytes,response_bytes.size(),&decoded)==0);REQUIRE(encode(root->response,&decoded)==response_bytes);
 REQUIRE(ce->commit(c,100+ordinal,reply.sequence,reply.reservation)==0);
 REQUIRE(se->retire(s,100+ordinal)==0);REQUIRE(ce->retire(c,100+ordinal)==0);REQUIRE(ce->retire(c,100+ordinal)==125);
 REQUIRE(cr->observe().native[static_cast<unsigned>(native_rcl_service::operation::send)].calls==sends+1);
 REQUIRE(sr->observe().native[static_cast<unsigned>(native_rcl_service::operation::respond)].calls==1);
 REQUIRE(cr->observe().native[static_cast<unsigned>(native_rcl_service::operation::fini)].calls==1&&sr->observe().native[static_cast<unsigned>(native_rcl_service::operation::fini)].calls==1);
 ce.reset();se.reset(); // Guards are now disarmed; once-fini counters stay real.
}
static rcl_interfaces::msg::ParameterValue parameter(){rcl_interfaces::msg::ParameterValue v{};v.type=4;v.bool_value=true;v.integer_value=-9;v.double_value=1.25;v.string_value="native-\xC2\xB5";v.byte_array_value={0,255};v.bool_array_value={false,true};v.integer_array_value={-2,3};v.double_array_value={-2.5,3.25};v.string_array_value={"left","right"};return v;}
static std::string counted(rosidl_runtime_c__String const&s){REQUIRE(s.capacity>=s.size&&(!s.size||s.data));return s.size?std::string{s.data,s.size}:std::string{};}
static type_description_interfaces::msg::IndividualTypeDescription individual(rosidl_runtime_c__type_description__IndividualTypeDescription const&v){
 type_description_interfaces::msg::IndividualTypeDescription out{};out.type_name=counted(v.type_name);REQUIRE(v.fields.capacity>=v.fields.size&&(!v.fields.size||v.fields.data));
 for(std::size_t i{};i<v.fields.size;++i){auto const&a=v.fields.data[i];type_description_interfaces::msg::Field f{};f.name=counted(a.name);f.type.type_id=a.type.type_id;f.type.capacity=a.type.capacity;f.type.string_capacity=a.type.string_capacity;f.type.nested_type_name=counted(a.type.nested_type_name);f.default_value=counted(a.default_value);out.fields.push_back(std::move(f));}return out;
}
static void services(std::shared_ptr<control::profile const>const&p,::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&node,rclcpp::executors::SingleThreadedExecutor&executor){
 using namespace rcl_interfaces::srv;
 DescribeParameters::Request dq{};dq.names={"source_value"};DescribeParameters::Response ds{};rcl_interfaces::msg::ParameterDescriptor descriptor{};descriptor.name="source_value";descriptor.type=4;descriptor.description="whole";descriptor.additional_constraints="retained";descriptor.read_only=false;descriptor.dynamic_typing=false;rcl_interfaces::msg::FloatingPointRange f{};f.from_value=-2.5;f.to_value=3.25;f.step=0.25;descriptor.floating_point_range={f};rcl_interfaces::msg::IntegerRange i{};i.from_value=-9;i.to_value=11;i.step=2;descriptor.integer_range={i};ds.descriptors={descriptor};roundtrip<DescribeParameters>(p,node,executor,"rcl_interfaces/srv/DescribeParameters",dq,ds,1);
 GetParameters::Request gq{};gq.names={"source_value"};GetParameters::Response gs{};gs.values={parameter()};roundtrip<GetParameters>(p,node,executor,"rcl_interfaces/srv/GetParameters",gq,gs,2);
 GetParameterTypes::Request tq{};tq.names={"source_value"};GetParameterTypes::Response ts{};ts.types={4};roundtrip<GetParameterTypes>(p,node,executor,"rcl_interfaces/srv/GetParameterTypes",tq,ts,3);
 ListParameters::Request lq{};lq.prefixes={"source"};lq.depth=2;ListParameters::Response ls{};ls.result.names={"source_value"};ls.result.prefixes={"source"};roundtrip<ListParameters>(p,node,executor,"rcl_interfaces/srv/ListParameters",lq,ls,4);
 rcl_interfaces::msg::Parameter value{};value.name="source_value";value.value=parameter();
 SetParameters::Request sq{};sq.parameters={value};SetParameters::Response ss{};rcl_interfaces::msg::SetParametersResult result{};result.successful=true;result.reason="whole";ss.results={result};roundtrip<SetParameters>(p,node,executor,"rcl_interfaces/srv/SetParameters",sq,ss,5);
 SetParametersAtomically::Request aq{};aq.parameters={value};SetParametersAtomically::Response as{};as.result=result;roundtrip<SetParametersAtomically>(p,node,executor,"rcl_interfaces/srv/SetParametersAtomically",aq,as,6);
 using GTD=type_description_interfaces::srv::GetTypeDescription;GTD::Request q{};REQUIRE(q.include_type_sources);q.type_name="type_description_interfaces/srv/GetTypeDescription";auto root=detail::codec(*p,q.type_name);REQUIRE(root);q.type_hash=native_rcl_service::identity_text(root->native_rihs01);REQUIRE(q.type_hash.size()==71);
 auto support=rosidl_typesupport_cpp::get_service_type_support_handle<GTD>();auto d=support->get_type_description_func(support);auto sources=support->get_type_description_sources_func(support);REQUIRE(d&&sources&&d->referenced_type_descriptions.size==11&&sources->size==12);
 GTD::Response response{};response.successful=true;response.type_description.type_description=individual(d->type_description);for(std::size_t n{};n<d->referenced_type_descriptions.size;++n)response.type_description.referenced_type_descriptions.push_back(individual(d->referenced_type_descriptions.data[n]));
 REQUIRE(sources->capacity>=sources->size&&sources->data);for(std::size_t n{};n<sources->size;++n){type_description_interfaces::msg::TypeSource source{};source.type_name=counted(sources->data[n].type_name);source.encoding=counted(sources->data[n].encoding);source.raw_file_contents=counted(sources->data[n].raw_file_contents);response.type_sources.push_back(std::move(source));}
 type_description_interfaces::msg::KeyValue extra{};extra.key="source";extra.value="whole-counted-bytes";response.extra_information={extra};roundtrip<GTD>(p,node,executor,q.type_name,q,response,7);
}
static void topics(std::shared_ptr<control::profile const>const&p,::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const&node,rclcpp::executors::SingleThreadedExecutor&executor){
 auto pubcap=cap(*p,"rcl_interfaces/msg/ParameterEvent","topic.publish","event-out","/control_gateway_contract/events"),subcap=cap(*p,pubcap.type,"topic.subscribe","event-in",pubcap.resource);
 wire::binding po{2,0x900000001,11,12,13},so{2,0x900000002,11,12,13};std::shared_ptr<native_rmw_publisher::endpoint>pub;std::shared_ptr<native_rmw_subscription::endpoint>sub;wire::packet a{},b{};
 REQUIRE(control::create_subscription(p,node,subcap,so,51,sub,b)==0);struct Guard{decltype(pub)*p;decltype(sub)*s;wire::binding po,so;~Guard(){if(*p)(void)(*p)->retire(po,50);if(*s)(void)(*s)->retire(so,51);}}guard{&pub,&sub,po,so};REQUIRE(control::create_publisher(p,node,pubcap,po,50,pub,a)==0);
 rcl_interfaces::msg::ParameterEvent event{};event.stamp.sec=-2;event.stamp.nanosec=300000000;event.node="/control_gateway_contract";rcl_interfaces::msg::Parameter v{};v.name="source_value";v.value=parameter();event.new_parameters={v};v.name="changed";event.changed_parameters={v};v.name="deleted";event.deleted_parameters={v};auto bytes=encode(detail::codec(*p,pubcap.type),&event);
 auto discovery=Clock::now()+std::chrono::seconds{3};while(!node->count_subscribers(pubcap.resource)||!node->count_publishers(pubcap.resource)){REQUIRE(Clock::now()<discovery);executor.spin_once(std::chrono::milliseconds{10});}
 // Graph visibility only schedules this Native test. It is not a transport
 // readiness/identity fact; the actual single publication must still arrive.
 REQUIRE(pub->publish(po,50,bytes.data(),bytes.size())==0);native_rmw_queue::snapshot peek{};auto until=Clock::now()+std::chrono::seconds{5};for(;;){REQUIRE(Clock::now()<until);auto error=sub->peek(so,51,sizeof(rmw_gid_t::data),peek,32768+info::header_bytes);if(!error)break;REQUIRE(error==11);executor.spin_once(std::chrono::milliseconds{1});}
 REQUIRE(peek.size==info::header_bytes+bytes.size()&&!std::memcmp(peek.bytes.get()+info::header_bytes,bytes.data(),bytes.size()));REQUIRE(sub->cancel(so,51,peek.sequence,peek.reservation)==0);
 native_rmw_queue::snapshot again{};REQUIRE(sub->peek(so,51,sizeof(rmw_gid_t::data),again,32768+info::header_bytes)==0&&again.sequence==peek.sequence&&again.reservation!=peek.reservation);REQUIRE(sub->commit(so,51,peek.sequence,peek.reservation)==13);REQUIRE(sub->commit(so,51,again.sequence,again.reservation)==0);
 REQUIRE(pub->retire(po,50)==0&&sub->retire(so,51)==0);pub.reset();sub.reset();
 auto logcap=cap(*p,"rcl_interfaces/msg/Log","topic.publish","rosout","/control_gateway_contract/log");std::shared_ptr<native_rmw_publisher::endpoint>log;REQUIRE(control::create_publisher(p,node,logcap,po,52,log,a)==0);rcl_interfaces::msg::Log record{};record.stamp.sec=-2;record.stamp.nanosec=7;record.level=20;record.name="default_control";record.msg="native-\xC2\xB5";record.file="source.cpp";record.function="whole";record.line=91;auto lb=encode(detail::codec(*p,logcap.type),&record);REQUIRE(log->publish(po,52,lb.data(),lb.size())==0);record.msg.assign("bad\0text",8);std::size_t sentinel=SIZE_MAX;std::array<std::uint8_t,32768>buffer{};REQUIRE(detail::codec(*p,logcap.type)->encode(&record,buffer.data(),buffer.size(),&sentinel)==22&&sentinel==SIZE_MAX);REQUIRE(log->retire(po,52)==0);log.reset();
}
int main(int argc,char**argv){try{
 REQUIRE(argc==2);std::string_view mode{argv[1]};auto const&ids=uwvm_default_control_gateway_input::identities;std::shared_ptr<control::profile const>p;
 if(mode=="bad-identity"){auto wrong=ids;wrong[0].rihs01[0]^=1;REQUIRE(control::prepare(wrong.data(),wrong.size(),p)==22&&!p);fast_io::println("PASS actual independent wrong-ID refusal");return 0;}
 REQUIRE(control::prepare(ids.data(),ids.size(),p)==0&&p);auto log=detail::codec(*p,"rcl_interfaces/msg/Log");REQUIRE(log&&log->validate(log,log->generated_support)==0);auto stolen=*log;REQUIRE(stolen.validate(&stolen,stolen.generated_support)==22);
 if(mode=="proof"){fast_io::println("PASS actual full C++ metadata/address proof");return 0;}
 REQUIRE(mode=="services"||mode=="topics");rclcpp::init(argc,argv);auto node=std::make_shared<::uwvm2::ros_gateway::ros2_compat::node_type>("control_gateway_contract");rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
 try{if(mode=="services")services(p,node,executor);else topics(p,node,executor);}catch(...){executor.remove_node(node);node.reset();rclcpp::shutdown();throw;}
 executor.remove_node(node);node.reset();rclcpp::shutdown();p.reset();fast_io::println("PASS actual full control endpoint contract");return 0;
 }catch(std::exception const&e){fast_io::perrln(e.what());return 1;}catch(...){return 1;}}
