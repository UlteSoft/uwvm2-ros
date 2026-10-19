/* SPDX-License-Identifier: Apache-2.0
 * Genuine native SDK component source, UNEXECUTED. No fake rcl/rmw function or
 * generated object is supplied. This must run only in Root's separately owned
 * isolated native DDS/domain/process envelope after an admitted complete SDK
 * export. Published identity fixture bytes alone are not that qualification.
 * No Host/IPC routing, Guest ABI or current RMW provider acceptance is claimed.
 */
#include "gateways/ros2/native_rcl_service.h"
#include "published_cpp_identity_fixture.hpp"
#include <thread>
#include <cstdio>
using namespace uwvm2::ros_gateway;
namespace direct=native_rcl_service;
static unsigned checks,failures;
#define CHECK(x) do{++checks;if(!(x)){++failures;std::fprintf(stderr,"FAIL_DIRECT_RCL_NATIVE line=%u\n",unsigned(__LINE__));}}while(0)
static ros::capability_spec capability(std::string alias,std::string operation,std::string type,std::string hash){
 ros::capability_spec c{};c.alias=std::move(alias);c.operation=std::move(operation);c.resource="/uwvm_direct_rcl_component";c.type=std::move(type);c.type_identity=std::move(hash);
 for(auto const& p:std::initializer_list<std::pair<std::string,std::string>>{{"validator","none"},{"qos.history","keep-last"},{"max-request-bytes","16"},{"max-response-bytes","8"},{"max-inflight","4"},{"timeout-ms","5000"},{"qos.depth","4"},{"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}})c.limits.emplace(p);
 return c;
}
template<class Service>static bool route(rclcpp::Node::SharedPtr node,std::shared_ptr<uwvm_rmw_cpp::Registry const> registry){
 auto root=registry->codec(direct::family_traits<Service>::codec);if(!root)return false;
 auto type=std::string{root->native_type};auto hash=direct::identity_text(root->native_rihs01);
 auto call=capability("direct-call","service.call",type,hash),advertise=capability("direct-advertise","service.advertise",type,hash),respond=capability("direct-respond","service.respond",type,hash);
 direct::wire::binding a{2,UINT64_C(0x100000001),2,3,4},b{2,UINT64_C(0x100000002),2,3,4},c{2,UINT64_C(0x100000003),2,3,4},d{2,UINT64_C(0x100000004),2,3,4};
 auto cr=std::make_shared<direct::report>(),sr=std::make_shared<direct::report>();std::shared_ptr<direct::client<Service>> client;std::shared_ptr<direct::server<Service>> server;direct::wire::packet cp{},sp{};
 auto code=direct::server<Service>::create(node,registry,advertise,b,20,8,sr,server,sp);CHECK(code==0);if(code)return false;
 code=direct::client<Service>::create(node,registry,call,a,21,8,cr,client,cp);CHECK(code==0);if(code){CHECK(server->retire(b,20)==0);return false;}
 CHECK(cp.profiles==2&&sp.profiles==2&&cp.gid_size==16);
 // This component's capability records are explicit TEST DATA. The future Host
 // must authenticate the separate service.available owner; call alone is denied.
 auto availability=capability("direct-availability","service.available",type,hash);availability.limits.erase("validator");
 availability.limits["service-source-alias"]=call.alias;availability.limits["max-inflight"]="1";
 availability.limits["max-request-bytes"]="64";availability.limits["max-response-bytes"]="64";
 availability.limits["max-wait-ns"]="1000000000";availability.limits["timeout-ms"]="1002";
 availability.limits["max-native-clients"]="1";availability.limits["max-binding-history"]="8";
 bool ready=true;auto denied=availability;denied.limits["service-source-alias"]="different-source";
 auto before=cr->observe().native[static_cast<unsigned>(direct::operation::available)].calls;
 CHECK(client->available(a,21,d,denied,ready)==13&&ready);
 CHECK(client->available(a,21,a,availability,ready)==13&&ready);
 auto stale=d;++stale.session;CHECK(client->available(a,21,stale,availability,ready)==13&&ready);
 CHECK(cr->observe().native[static_cast<unsigned>(direct::operation::available)].calls==before);
 auto discovery_end=std::chrono::steady_clock::now()+std::chrono::seconds(5);
 for(;;){code=client->available(a,21,d,availability,ready);if(code||ready)break;if(std::chrono::steady_clock::now()>=discovery_end){code=110;break;}std::this_thread::sleep_for(std::chrono::milliseconds(1));}
 CHECK(code==0&&ready);
 if(code||!ready){CHECK(client->retire(a,21)==0);CHECK(server->retire(b,20)==0);return false;}
 CHECK(cr->observe().native[static_cast<unsigned>(direct::operation::send)].calls==0);
 typename Service::Request request{};typename Service::Response response{};
 if constexpr(std::is_same_v<Service,example_interfaces::srv::AddTwoInts>){request.a=-7;request.b=49;response.sum=42;}
 std::array<std::uint8_t,16> body{};std::size_t size=SIZE_MAX;CHECK(root->request->encode(&request,body.data(),root->request->maximum_wire_bytes,&size)==0);
 std::uint64_t ticket{};CHECK(client->submit(a,21,body.data(),size,ticket)==0&&ticket);
 direct::snapshot request_record;auto end=std::chrono::steady_clock::now()+std::chrono::seconds(5);
 for(;;){code=server->poll(b,20);if(code!=0&&code!=11)break;code=server->peek(b,20,16,request_record);if(code!=11)break;if(std::chrono::steady_clock::now()>=end){code=110;break;}std::this_thread::sleep_for(std::chrono::milliseconds(1));}
 CHECK(code==0);bool passed=code==0;
 if(passed){direct::info::header request_info{};CHECK(direct::info::decode(request_record.bytes.get(),192,request_record.size-192,request_info)&&direct::info::complete(request_info));CHECK(request_info.kind==direct::events::event_kind::service_request&&request_info.writer_size==16);
  typename Service::Request copy{};CHECK(root->request->decode(request_record.bytes.get()+192,request_record.size-192,&copy)==0);
  if constexpr(std::is_same_v<Service,example_interfaces::srv::AddTwoInts>)CHECK(copy.a==-7&&copy.b==49);
  CHECK(server->commit(b,20,request_record.sequence,request_record.reservation)==0);size=SIZE_MAX;CHECK(root->response->encode(&response,body.data(),root->response->maximum_wire_bytes,&size)==0);
  CHECK(server->respond(b,20,c,respond,request_info.correlation,body.data(),size)==0);
  direct::snapshot response_record;end=std::chrono::steady_clock::now()+std::chrono::seconds(5);
  for(;;){code=client->poll(a,21);if(code!=0&&code!=11)break;code=client->peek(a,21,16,response_record);if(code!=11)break;if(std::chrono::steady_clock::now()>=end){code=110;break;}std::this_thread::sleep_for(std::chrono::milliseconds(1));}
  CHECK(code==0);passed=code==0;
  if(passed){direct::info::header response_info{};CHECK(direct::info::decode(response_record.bytes.get(),192,response_record.size-192,response_info)&&direct::info::complete(response_info));
   CHECK(response_info.correlation==ticket&&response_info.request_sequence==request_info.request_sequence&&response_info.writer==request_info.writer);
   typename Service::Response copy_response{};CHECK(root->response->decode(response_record.bytes.get()+192,response_record.size-192,&copy_response)==0);
   if constexpr(std::is_same_v<Service,example_interfaces::srv::AddTwoInts>)CHECK(copy_response.sum==42);
   CHECK(client->commit(a,21,response_record.sequence,response_record.reservation)==0);
  }
 }
 CHECK(client->retire(a,21)==0);CHECK(server->retire(b,20)==0);auto cc=cr->observe(),ss=sr->observe();CHECK(cc.fini_attempted&&cc.fini_returned&&cc.fini_result==RCL_RET_OK&&cc.native[static_cast<unsigned>(direct::operation::fini)].calls==1);CHECK(ss.fini_attempted&&ss.fini_returned&&ss.fini_result==RCL_RET_OK&&ss.native[static_cast<unsigned>(direct::operation::fini)].calls==1);
 return passed;
}
int main(int argc,char** argv){
 if(argc!=2||std::string_view{argv[1]}!="--owned-native-component")return 13;
#if !UWVM_NATIVE_RCL_SERVICE_API || !UWVM_NATIVE_RCL_AVAILABILITY_API || !UWVM_ROS2_HAVE_COMPLETE_RMW_QOS
 std::fprintf(stderr,"UNAVAILABLE_DIRECT_RCL_SERVICE actual compiled API feature=0\n");return 95;
#else
 auto registry=std::make_shared<uwvm_rmw_cpp::Registry const>(cpp_fixture::published(),128);if(registry->error())return registry->error(); // warm BEFORE any owned mapping checkpoint
 rclcpp::init(argc,argv);{auto node=std::make_shared<rclcpp::Node>("uwvm_direct_rcl_service_contract");
  // Real original NodeOptions defaults are retained. This executable never
  // adds our direct handles to a native rclcpp executor or spins that executor.
  CHECK(route<std_srvs::srv::Empty>(node,registry));CHECK(route<example_interfaces::srv::AddTwoInts>(node,registry));}
 rclcpp::shutdown();std::printf("%s_DIRECT_RCL_NATIVE_COMPONENT families=2 checks=%u failures=%u\n",failures?"FAIL":"PASS",checks,failures);return failures?1:0;
#endif
}
