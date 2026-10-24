/* SPDX-License-Identifier: Apache-2.0 */
/** Actual SDK/full-value contract SOURCE, never executed by author.
 * Requires BOTH actual generated bounded native+manifest provider and immutable
 * independently acquired full native export header. It reads eleven original
 * CPP10 expected RIHS01 lines as explicit Root-owned input, never fabricates
 * NativeIDs or substitutes a mock registry. No Host/Guest permission is granted.
 * Native takes into ORIGINAL dynamic objects can allocate before the canonical
 * post-take bound: Root must enforce real RSS/cgroup/full-process deadlines.
 */
#ifdef NDEBUG
#error "Native contract requires active checks (-UNDEBUG)"
#endif
#include "custom_owned_service_endpoint.h"
#include <xarm_msgs/srv/get_float32_list.hpp>
#include <xarm_msgs/srv/set_int16_list.hpp>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <thread>
#include "src/ros/gateway/common/native_io.h"
#include <climits>
#include <csignal>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/syscall.h>
namespace c=uwvm2::ros_gateway::custom_owned_services;namespace d=c::detail;
namespace n=uwvm2::ros_gateway::native_rcl_service;using namespace uwvm2::ros_gateway;
static unsigned checks,failures;
#define CHECK(x) do{++checks;if(!(x)){++failures;native_io::fixed_text<96> record;if(record.append("FAIL_CUSTOM_NATIVE line=")&&record.number(unsigned(__LINE__))&&record.append("\n"))(void)native_io::write_audit_text(record.view());}}while(0)
using G=xarm_msgs::srv::GetFloat32List;using S=xarm_msgs::srv::SetInt16List;
using GQ=uwvm_custom_bounded_native::Ros2Xarm_msgsSrvGetFloat32List_RequestCodec;
using GS=uwvm_custom_bounded_native::Ros2Xarm_msgsSrvGetFloat32List_ResponseCodec;
using SQ=uwvm_custom_bounded_native::Ros2Xarm_msgsSrvSetInt16List_RequestCodec;
using SS=uwvm_custom_bounded_native::Ros2Xarm_msgsSrvSetInt16List_ResponseCodec;
static bool identities(char const* path,uwvm_rmw_cpp::NativeIdentities& out){
 int fd=::open(path,O_RDONLY|O_CLOEXEC|O_NOFOLLOW|O_NONBLOCK);if(fd<0)return false;struct guard{int fd;~guard(){::close(fd);}}g{fd};
 struct stat a{},b{};constexpr std::size_t bytes=11*72;std::array<char,bytes+1> raw{};std::size_t n{};
 if(::fstat(fd,&a)||!S_ISREG(a.st_mode)||a.st_size!=bytes)return false;
 while(n<raw.size()){auto count=::read(fd,raw.data()+n,raw.size()-n);if(count<0)return false;if(!count)break;n+=static_cast<std::size_t>(count);}
 if(n!=bytes||::fstat(fd,&b)||a.st_dev!=b.st_dev||a.st_ino!=b.st_ino||a.st_size!=b.st_size||a.st_mtim.tv_sec!=b.st_mtim.tv_sec||a.st_mtim.tv_nsec!=b.st_mtim.tv_nsec||a.st_ctim.tv_sec!=b.st_ctim.tv_sec||a.st_ctim.tv_nsec!=b.st_ctim.tv_nsec)return false;
 uwvm_rmw_cpp::NativeIdentities next{};for(std::size_t i{};i<11;++i){std::string_view id{raw.data()+i*72,71};if(raw[i*72+71]!='\n'||!d::native_identity(id))return false;
 auto digit=[](char x){return unsigned(x<='9'?x-'0':x-'a'+10);};for(std::size_t j{};j<32;++j)next.hashes[i][j]=static_cast<std::uint8_t>((digit(id[7+2*j])<<4)|digit(id[8+2*j]));}out=next;return true;
}
static ros::capability_spec capability(std::string alias,std::string operation,d::row_identity row,std::size_t request,std::size_t response){
 ros::capability_spec p{};p.alias=std::move(alias);p.operation=std::move(operation);p.resource="/uwvm_custom/"+std::string(row.type.substr(row.type.rfind('/')+1));p.type=row.type;p.type_identity=row.native;
 for(auto const& pair:std::initializer_list<std::pair<std::string,std::string>>{{"validator","none"},{"adapter-codec-identity",std::string(row.canonical)},{"max-request-bytes",std::to_string(request)},{"max-response-bytes",std::to_string(response)},{"max-inflight","4"},{"timeout-ms","5000"},{"qos.history","keep-last"},{"qos.depth","4"},{"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}})p.limits.emplace(pair);return p;
}
template<class Service,class Q,class R>bool route(rclcpp::Node::SharedPtr const& node,std::shared_ptr<uwvm_rmw_cpp::Registry const> const& original){
 d::row_identity row{};for(auto const& facts:d::identities)if(facts.type==std::string_view{Q::type}.substr(0,std::string_view{Q::type}.size()-8))row=facts;
 if(row.type.empty())return false;using Proof=d::CompiledCanonicalIdentity<Service,Q,R>;std::shared_ptr<Proof const> proof;auto code=Proof::qualify(row,proof);CHECK(code==0);if(code)return false;
 auto root=proof->root();typename Service::Request request{};typename Service::Response response{};if constexpr(std::is_same_v<Service,G>)request.structure_needs_at_least_one_member=0;else request.datas={-32768,0,32767};
 response.ret=-7;response.message=std::string("native-\xC2\xB5",9);if constexpr(std::is_same_v<Service,G>)response.datas={1.25f,-2.5f};
 std::vector<std::uint8_t> bytes(std::max(Q::wire_bytes,R::wire_bytes)+1);std::size_t size=SIZE_MAX;CHECK(root->request->encode(&request,bytes.data(),bytes.size(),&size)==0);typename Service::Request decoded{};CHECK(root->request->decode(bytes.data(),size,&decoded)==0);
 if constexpr(std::is_same_v<Service,G>){CHECK(size==0&&decoded.structure_needs_at_least_one_member==0);request.structure_needs_at_least_one_member=1;auto untouched=std::size_t{123};CHECK(root->request->encode(&request,bytes.data(),bytes.size(),&untouched)==22&&untouched==123);request.structure_needs_at_least_one_member=0;}
 else{CHECK(decoded.datas==request.datas);request.datas.resize(65);CHECK(root->request->encode(&request,bytes.data(),bytes.size(),&size)!=0);request.datas={-32768,0,32767};}
 CHECK(root->response->encode(&response,bytes.data(),bytes.size(),&size)==0);typename Service::Response decoded_response{};CHECK(root->response->decode(bytes.data(),size,&decoded_response)==0&&decoded_response.ret==-7&&decoded_response.message==response.message);
 if constexpr(std::is_same_v<Service,G>)CHECK(decoded_response.datas==response.datas);
 auto saved=decoded_response;CHECK(size>0&&root->response->decode(bytes.data(),size-1,&decoded_response)==22&&decoded_response.message==saved.message);response.message.resize(257,'a');CHECK(root->response->encode(&response,bytes.data(),bytes.size(),&size)!=0);response.message=std::string("native-\xC2\xB5",9);
 // NUL is expressly forbidden by this bounded canonical identity, unlike
 // raw builtin String. Encode must preserve its length sentinel; decode must
 // preserve EVERY original destination field when a real canonical byte fails.
 auto valid_message=response.message;response.message=std::string("native\0ok",9);std::size_t rejected_extent=123;
 CHECK(root->response->encode(&response,bytes.data(),bytes.size(),&rejected_extent)==22&&rejected_extent==123);
 response.message=valid_message;size=SIZE_MAX;CHECK(root->response->encode(&response,bytes.data(),bytes.size(),&size)==0);
 auto nul_bytes=bytes;nul_bytes[6]=0;auto before_nul=decoded_response;
 CHECK(root->response->decode(nul_bytes.data(),size,&decoded_response)==22&&decoded_response.ret==before_nul.ret&&decoded_response.message==before_nul.message);
 if constexpr(std::is_same_v<Service,G>)CHECK(decoded_response.datas==before_nul.datas);
 // Exercise genuine generated event constructor/destroy callbacks, not pointer
 // presence as a substitute for the independent full graph/source proof.
 auto support=rosidl_typesupport_cpp::get_service_type_support_handle<Service>();rosidl_service_introspection_info_t details{};details.event_type=1;details.stamp_sec=-2;details.stamp_nanosec=300000000;details.sequence_number=INT64_MIN;
 auto allocator=rcutils_get_default_allocator();void* event=support->event_message_create_handle_function(&details,&allocator,&request,&response);CHECK(event!=nullptr);
 if(event){auto const& value=*static_cast<typename Service::Event const*>(event);CHECK(value.info.event_type==1&&value.info.stamp.sec==-2&&value.info.stamp.nanosec==300000000&&value.info.sequence_number==INT64_MIN&&value.request.size()==1&&value.response.size()==1);for(auto b:value.info.client_gid)CHECK(b==0);CHECK(value.response[0].ret==-7&&value.response[0].message==response.message);if constexpr(std::is_same_v<Service,G>)CHECK(value.response[0].datas==response.datas);else CHECK(value.request[0].datas==request.datas);CHECK(support->event_message_destroy_handle_function(event,&allocator));}
 auto call=capability("custom-call","service.call",row,Q::wire_bytes,R::wire_bytes),advertise=capability("custom-advertise","service.advertise",row,Q::wire_bytes,R::wire_bytes),respond=capability("custom-respond","service.respond",row,Q::wire_bytes,R::wire_bytes);
 // The public whole-body companion must reject bad extent/identity BEFORE
 // any Native entity or borrowed record. All actual metadata is warmed above.
 size=SIZE_MAX;CHECK(root->request->encode(&request,bytes.data(),bytes.size(),&size)==0);
 CHECK(c::validate_body(call,false,bytes.data(),size)==0);
 auto request_extent=size;bytes[size]=0;CHECK(c::validate_body(call,false,bytes.data(),size+1)==22);
 if constexpr(std::is_same_v<Service,G>)CHECK(c::validate_body(call,false,nullptr,0)==0);
 else CHECK(size>0&&c::validate_body(call,false,bytes.data(),size-1)==22);
 auto mismatch=call;mismatch.limits["adapter-codec-identity"]="different";
 CHECK(c::validate_body(mismatch,false,bytes.data(),request_extent)==13);
 mismatch=call;mismatch.type_identity="RIHS01_different";CHECK(c::validate_body(mismatch,false,bytes.data(),request_extent)==13);
 mismatch=call;mismatch.type="not_compiled/srv/Missing";CHECK(c::validate_body(mismatch,false,nullptr,0)==95);
 size=SIZE_MAX;CHECK(root->response->encode(&response,bytes.data(),bytes.size(),&size)==0);
 CHECK(c::validate_body(respond,true,bytes.data(),size)==0);
 CHECK(c::validate_body(call,true,nul_bytes.data(),size)==22);
 CHECK(c::validate_body(call,true,nullptr,1)==22);
 // Original xArm Response is int16 ret + uint32 UTF8 extent + message +
 // optional complete float32 sequence. The independent native codec produced
 // these bytes above; mutate one real field, never reinterpret native layout.
 CHECK(size>=15);auto utf8_byte=bytes[6];bytes[6]=0xff;
 CHECK(c::validate_body(call,true,bytes.data(),size)==22);bytes[6]=utf8_byte;
 if constexpr(std::is_same_v<Service,G>){auto clean=bytes;ipc::custom_write_bits(bytes.data()+size-4,UINT32_C(0x7fc00000),4);
  CHECK(c::validate_body(call,true,bytes.data(),size)==22);bytes=std::move(clean);}
 CHECK(size>0&&c::validate_body(call,true,bytes.data(),size-1)==22);
 auto complete=bytes;bytes[size]=0;CHECK(c::validate_body(call,true,bytes.data(),size+1)==22);bytes=std::move(complete);
 n::wire::binding a{2,UINT64_C(0x100000001),2,3,4},b{2,UINT64_C(0x100000002),2,3,4},r{2,UINT64_C(0x100000003),2,3,4};auto cr=std::make_shared<n::report>(),sr=std::make_shared<n::report>();std::shared_ptr<c::endpoint> client,server;n::wire::packet cp{},sp{};
 code=c::create_selected(node,original,advertise,b,20,8,sr,server,sp);CHECK(code==0);if(code)return false;
 struct retirement{std::shared_ptr<c::endpoint>& e;n::wire::binding o;std::uint64_t i;~retirement(){if(e)CHECK(e->retire(o,i)==0);}} server_guard{server,b,20};
 code=c::create_selected(node,original,call,a,21,8,cr,client,cp);CHECK(code==0);if(code)return false;retirement client_guard{client,a,21};
 // One actual immediate readiness query, caller-paced native polling. This is
 // NOT an implementation of positive available_until on the public endpoint.
 auto available=call;available.alias="custom-query";available.operation="service.available";available.limits.erase("validator");available.limits["service-source-alias"]=call.alias;available.limits["max-request-bytes"]="64";available.limits["max-response-bytes"]="64";available.limits["max-inflight"]="1";available.limits["max-native-clients"]="1";available.limits["max-binding-history"]="8";available.limits["max-wait-ns"]="1000000";available.limits["timeout-ms"]="5000";
 n::wire::binding query{2,UINT64_C(0x100000004),2,3,4};
 // A direct factory call does not acquire Runtime socket/pidfd/stop ownership.
 // Missing actual borrowed admission must not create a domain or fake a wait.
 bool untouched=true;auto no_owner_calls=cr->observe();
 CHECK(client->available_until(a,21,query,available,1000000,std::chrono::steady_clock::now()+std::chrono::seconds(1),{},untouched)==95&&untouched);
 auto after_no_owner=cr->observe();CHECK(after_no_owner.native[static_cast<unsigned>(n::operation::available)].calls==no_owner_calls.native[static_cast<unsigned>(n::operation::available)].calls);
 bool ready{};auto end=std::chrono::steady_clock::now()+std::chrono::seconds(5);do{code=client->available(a,21,query,available,ready);if(code||ready)break;std::this_thread::sleep_for(std::chrono::milliseconds(1));}while(std::chrono::steady_clock::now()<end);CHECK(code==0&&ready);if(code||!ready)return false;
 // A failed NUL encode is a local refusal, never a native send/retry.
 auto before_send=cr->observe();response.message=std::string("native\0ok",9);rejected_extent=123;
 CHECK(root->response->encode(&response,bytes.data(),bytes.size(),&rejected_extent)==22&&rejected_extent==123);
 auto after_send=cr->observe();CHECK(after_send.native[static_cast<unsigned>(n::operation::send)].calls==before_send.native[static_cast<unsigned>(n::operation::send)].calls);
 response.message=valid_message;
 size=SIZE_MAX;CHECK(root->request->encode(&request,bytes.data(),bytes.size(),&size)==0);std::uint64_t ticket{};code=client->submit(a,21,bytes.data(),size,ticket);CHECK(code==0&&ticket);if(code)return false;
 auto take=[&](std::shared_ptr<c::endpoint> const& e,n::wire::binding owner,std::uint64_t incarnation,n::snapshot& snapshot){auto until=std::chrono::steady_clock::now()+std::chrono::seconds(5);for(;;){auto result=e->poll(owner,incarnation);if(result&&result!=11)return result;result=e->peek(owner,incarnation,16,snapshot,n::events::max_payload_bytes);if(result!=11)return result;if(std::chrono::steady_clock::now()>=until)return 110u;std::this_thread::sleep_for(std::chrono::milliseconds(1));}};
 n::snapshot req;code=take(server,b,20,req);CHECK(code==0);if(code)return false;n::info::header qi{};CHECK(n::info::decode(req.bytes.get(),192,req.size-192,qi)&&n::info::complete(qi));CHECK(root->request->decode(req.bytes.get()+192,req.size-192,&decoded)==0);if constexpr(std::is_same_v<Service,S>)CHECK(decoded.datas==request.datas);CHECK(server->commit(b,20,req.sequence,req.reservation)==0);
 size=SIZE_MAX;CHECK(root->response->encode(&response,bytes.data(),bytes.size(),&size)==0);CHECK(server->respond(b,20,r,respond,qi.correlation,bytes.data(),size)==0);
 n::snapshot resp;code=take(client,a,21,resp);CHECK(code==0);if(code)return false;n::info::header ri{};CHECK(n::info::decode(resp.bytes.get(),192,resp.size-192,ri)&&n::info::complete(ri)&&ri.correlation==ticket&&ri.request_sequence==qi.request_sequence&&ri.writer==qi.writer);CHECK(root->response->decode(resp.bytes.get()+192,resp.size-192,&decoded_response)==0&&decoded_response.ret==-7&&decoded_response.message==response.message);if constexpr(std::is_same_v<Service,G>)CHECK(decoded_response.datas==response.datas);CHECK(client->commit(a,21,resp.sequence,resp.reservation)==0);return true;
}
// Separate genuine Native graph component. The authenticated UNIX peer and
// pidfd are inherited Root-owned facts, never fabricated by a local socketpair
// or inferred from an alias. Only the borrowed pidfd is owned by this scope.
namespace graph=native_rcl_graph_wait;
static std::atomic<std::sig_atomic_t> stopped{0};
static void stop_graph(int)noexcept{stopped.store(1,std::memory_order_relaxed);}
static void require(bool value){CHECK(value);if(!value)throw std::runtime_error("Actual custom graph wait contract failed");}
struct graph_borrow
{
 int session{-1},worker{-1};struct sigaction previous{};bool installed{};
 void retire()noexcept{if(installed){installed=false;CHECK(::sigaction(SIGUSR1,&previous,nullptr)==0);}if(worker>=0){auto selected=worker;worker=-1;CHECK(::close(selected)==0);}}
 ~graph_borrow(){retire();}
 bool admit(char const* descriptor,char const* process)noexcept
 {
  try{session=static_cast<int>(integer(descriptor,INT_MAX));auto pid=static_cast<int>(integer(process,INT_MAX));if(pid<=0)return false;
   struct stat actual{};if(::fstat(session,&actual)||!S_ISSOCK(actual.st_mode))return false;
   int type{};socklen_t count=sizeof(type);if(::getsockopt(session,SOL_SOCKET,SO_TYPE,&type,&count)||count!=sizeof(type)||type!=SOCK_STREAM)return false;
   sockaddr_un peer{};count=sizeof(peer);if(::getpeername(session,reinterpret_cast<sockaddr*>(&peer),&count)||peer.sun_family!=AF_UNIX)return false;
   ucred credentials{};count=sizeof(credentials);if(::getsockopt(session,SOL_SOCKET,SO_PEERCRED,&credentials,&count)||count!=sizeof(credentials)||credentials.uid!=::geteuid()||credentials.pid!=pid)return false;
   worker=static_cast<int>(::syscall(SYS_pidfd_open,pid,0));if(worker<0)return false;
   struct sigaction action{};action.sa_handler=&stop_graph;if(::sigemptyset(&action.sa_mask)||::sigaction(SIGUSR1,&action,&previous))return false;
   installed=true;return true;
  }catch(...){return false;}
 }
 graph::lifetime life()const noexcept{return {session,worker,&stopped};}
};
template<class Service,class Q,class R>void graph_route(rclcpp::Node::SharedPtr const& node,
 std::shared_ptr<uwvm_rmw_cpp::Registry const> const& original,std::string_view mode,graph::lifetime const& lifetime)
{
 d::row_identity row{};for(auto const& facts:d::identities)if(facts.type==std::string_view{Q::type}.substr(0,std::string_view{Q::type}.size()-8))row=facts;
 require(!row.type.empty());auto call=capability("custom-graph-call","service.call",row,Q::wire_bytes,R::wire_bytes);
 call.resource=std::is_same_v<Service,G>?"/uwvm_custom/get_float32_list":"/uwvm_custom/set_int16_list";
 auto query=call;query.alias="custom-graph-query";query.operation="service.available";query.limits.erase("validator");
 query.limits["service-source-alias"]=call.alias;query.limits["max-request-bytes"]="64";query.limits["max-response-bytes"]="64";
 query.limits["max-inflight"]="1";query.limits["max-native-clients"]="1";query.limits["max-binding-history"]="8";
 query.limits["max-wait-ns"]="2000000000";query.limits["timeout-ms"]="5000";
 n::wire::binding owner{2,UINT64_C(0x200000001),2,3,4},query_owner{2,UINT64_C(0x200000002),2,3,4};
 // One actual Node/profile domain is retained throughout this synchronous
 // Native fixture. Production must borrow its existing owned profile domain.
 graph::domain domain{node};graph::admission waiting{&domain,lifetime};
 auto diagnostics=std::make_shared<n::report>();std::shared_ptr<c::endpoint> client;n::wire::packet observed{};
 auto code=c::create_selected(node,original,call,owner,31,8,diagnostics,client,observed);require(code==0&&client);
 struct retire{std::shared_ptr<c::endpoint>& value;n::wire::binding owner;bool armed{true};~retire(){if(armed&&value)CHECK(value->retire(owner,31)==0);}} cleanup{client,owner};
 bool value=true;require(client->available_until(owner,31,query_owner,query,0,graph::clock::now()+std::chrono::seconds(5),{},value)==0&&!value);
 require(domain.observe().registration_attempts==0);
 auto denied=query;denied.limits["service-source-alias"]="different-owner";value=true;
 require(client->available_until(owner,31,query_owner,denied,1000000,graph::clock::now()+std::chrono::seconds(5),waiting,value)==13&&value);
 require(client->available_until(owner,31,query_owner,query,1000000,graph::clock::now()+std::chrono::seconds(5),{},value)==95&&value);
 require(domain.observe().registration_attempts==0);
 native_io::fixed_text<512> ready;require(ready.append("CUSTOM_NATIVE_GRAPH_READY family=")&&ready.append(row.type)&&ready.append(" mode=")&&ready.append(mode)&&ready.append("\n")&&native_io::write_all(STDOUT_FILENO,ready.view()));
 // Root may now start the real opposite for ready mode, close its peer for
 // disconnect, signal SIGUSR1 for stop, or leave it absent. This finite owner
 // must also bound the handshake. This program creates no child process.
 char go{};require(::read(STDIN_FILENO,&go,1)==1&&go=='G');
 auto entered=graph::clock::now();auto wait_ns=mode=="absent"?UINT64_C(120000000):UINT64_C(2000000000);
 auto until=entered+(mode=="transport"?std::chrono::milliseconds(25):std::chrono::milliseconds(4000));
 value=true;code=client->available_until(owner,31,query_owner,query,wait_ns,until,waiting,value);
 auto elapsed=graph::clock::now()-entered;auto facts=domain.observe();auto calls=diagnostics->observe();
 require(facts.registration_attempts==1&&facts.registration_completed==1&&!facts.poisoned&&facts.wait_attempts>0);
 if(mode=="ready")require(code==0&&value);
 else if(mode=="absent")require(code==0&&!value&&elapsed>=std::chrono::nanoseconds(wait_ns));
 else if(mode=="disconnect")require(code==107&&value);
 else if(mode=="stop")require(code==125&&value);
 else if(mode=="transport")require(code==110&&value);
 else require(false);
 require(calls.native[static_cast<unsigned>(n::operation::init)].calls==1);
 for(auto op:{n::operation::send,n::operation::take,n::operation::respond})require(calls.native[static_cast<unsigned>(op)].calls==0);
 require(calls.native[static_cast<unsigned>(n::operation::available)].calls>=2);
 require(client->retire(owner,31)==0);cleanup.armed=false;calls=diagnostics->observe();
 require(calls.fini_attempted&&calls.fini_returned&&calls.fini_result==RCL_RET_OK&&calls.native[static_cast<unsigned>(n::operation::fini)].calls==1);
 native_io::fixed_text<512> done;require(done.append("CUSTOM_NATIVE_GRAPH_OBSERVED result=")&&done.number(code)&&done.append(" value=")&&done.number(unsigned(value))&&done.append(" registrations=")&&done.number(facts.registration_attempts)&&done.append(" waits=")&&done.number(facts.wait_attempts)&&done.append(" elapsed_ns=")&&done.number(std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count())&&done.append(" Host_Guest_acceptance=0\n")&&native_io::write_all(STDOUT_FILENO,done.view()));
}
int main(int argc,char** argv){
 bool component=argc==3&&std::string_view{argv[1]}=="--owned-native-custom-service";
 bool graph_component=argc==7&&std::string_view{argv[1]}=="--owned-native-custom-graph-wait";
 if(!component&&!graph_component)return 13;
#if !UWVM_NATIVE_CUSTOM_SERVICE_METADATA_API || !UWVM_NATIVE_RCL_SERVICE_API || !UWVM_NATIVE_RCL_AVAILABILITY_API || !UWVM_ROS2_HAVE_COMPLETE_RMW_QOS
 return 95;
#else
#if !UWVM_NATIVE_RCL_FINITE_GRAPH_WAIT_API
 if(graph_component)return 95;
#endif
 graph_borrow borrowed;std::string_view family=graph_component?std::string_view{argv[3]}:std::string_view{},mode=graph_component?std::string_view{argv[4]}:std::string_view{};
 if(graph_component){if((family!="get"&&family!="set")||(mode!="absent"&&mode!="ready"&&mode!="disconnect"&&mode!="stop"&&mode!="transport")||!borrowed.admit(argv[5],argv[6]))return 13;}
 uwvm_rmw_cpp::NativeIdentities ids{};if(!identities(argv[2],ids))return 22;auto original=std::make_shared<uwvm_rmw_cpp::Registry const>(ids,512);if(original->error())return original->error();bool initialized{};int primary=5;
 try{rclcpp::init(argc,argv);initialized=true;{auto node=std::make_shared<rclcpp::Node>("uwvm_custom_native_contract");
  if(component){CHECK((route<G,GQ,GS>(node,original)));CHECK((route<S,SQ,SS>(node,original)));}
  else if(family=="get")graph_route<G,GQ,GS>(node,original,mode,borrowed.life());else graph_route<S,SQ,SS>(node,original,mode,borrowed.life());
 }primary=failures?1:0;}catch(std::bad_alloc const&){primary=12;}catch(...){primary=5;}
 if(initialized){try{rclcpp::shutdown();}catch(...){if(!primary)primary=5;}}borrowed.retire();if(!primary&&failures)primary=1;native_io::fixed_text<256> terminal;bool written=terminal.append(primary?"FAIL":"PASS")&&terminal.append("_CUSTOM_NATIVE_COMPLETE_COMPONENT checks=")&&terminal.number(checks)&&terminal.append(" failures=")&&terminal.number(failures)&&terminal.append("\n")&&native_io::write_all(STDOUT_FILENO,terminal.view());if(!written&&!primary)primary=5;return primary;
#endif
}
