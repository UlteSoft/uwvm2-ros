/* SPDX-License-Identifier: Apache-2.0 */
/** SOURCE ONLY: complete original GetSetModbusData graph/default/object/wire and
 * direct-RCL loopback. No actual export/header/NativeID is provided or guessed.
 * All metadata/C typesupport warming and private object/byte preparation occur
 * in the externally owned loader phase BEFORE original Native endpoint init.
 * Real Native middleware can allocate a huge unbounded incoming object before
 * canonical refusal; an outer finite complete-process64GiB/RSS/output/deadline
 * owner remains mandatory. This is not Host/Guest, robot or vendor acceptance.
 */
#ifdef NDEBUG
#error "The actual scalar native contract requires active checks (-UNDEBUG)"
#endif
#include "custom_owned_service_endpoint.h"
#include "scalar_c_contract.h"
#include "scalar_contract_features.h"
#include "bounded_records.hpp"
#include <xarm_msgs/srv/get_set_modbus_data.hpp>
#include "gateways/common/native_io.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <thread>
namespace c=uwvm2::ros_gateway::custom_owned_services;namespace d=c::detail;
namespace n=uwvm2::ros_gateway::native_rcl_service;using namespace uwvm2::ros_gateway;
using M=xarm_msgs::srv::GetSetModbusData;
using Q=uwvm_custom_bounded_native::Ros2Xarm_msgsSrvGetSetModbusData_RequestCodec;
using S=uwvm_custom_bounded_native::Ros2Xarm_msgsSrvGetSetModbusData_ResponseCodec;
using Guest=uwvm_custom::Ros2Xarm_msgsSrvGetSetModbusData_Request;
using Proof=d::CompiledCanonicalIdentity<M,Q,S>;
static unsigned checks{},failures{};
#define CHECK(x) do{++checks;if(!(x)){++failures;native_io::fixed_text<96> record;if(record.append("FAIL_CUSTOM_SCALAR line=")&&record.number(unsigned(__LINE__))&&record.append("\n"))(void)native_io::write_audit_text(record.view());}}while(0)
static bool identities(char const* path,uwvm_rmw_cpp::NativeIdentities& out){
 int fd=::open(path,O_RDONLY|O_CLOEXEC|O_NOFOLLOW|O_NONBLOCK);if(fd<0)return false;struct guard{int fd;~guard(){::close(fd);}}g{fd};
 struct stat a{},b{};constexpr std::size_t bytes=11*72;std::array<char,bytes+1> raw{};std::size_t n{};
 if(::fstat(fd,&a)||!S_ISREG(a.st_mode)||a.st_size!=bytes)return false;
 while(n<raw.size()){auto count=::read(fd,raw.data()+n,raw.size()-n);if(count<0)return false;if(!count)break;n+=static_cast<std::size_t>(count);}
 if(n!=bytes||::fstat(fd,&b)||a.st_dev!=b.st_dev||a.st_ino!=b.st_ino||a.st_size!=b.st_size||a.st_mtim.tv_sec!=b.st_mtim.tv_sec||a.st_mtim.tv_nsec!=b.st_mtim.tv_nsec||a.st_ctim.tv_sec!=b.st_ctim.tv_sec||a.st_ctim.tv_nsec!=b.st_ctim.tv_nsec)return false;
 uwvm_rmw_cpp::NativeIdentities next{};for(std::size_t i{};i<11;++i){std::string_view id{raw.data()+i*72,71};if(raw[i*72+71]!='\n'||!d::native_identity(id))return false;
 auto digit=[](char x){return unsigned(x<='9'?x-'0':x-'a'+10);};for(std::size_t j{};j<32;++j)next.hashes[i][j]=static_cast<std::uint8_t>((digit(id[7+2*j])<<4)|digit(id[8+2*j]));}out=next;return true;
}
static bool defaults(M::Request const& q){return q.modbus_data.empty()&&q.modbus_length==0&&q.ret_length==0&&q.host_id==9&&!q.is_transparent_transmission&&!q.use_503_port;}
static bool request(M::Request const& q){return q.modbus_data==std::vector<std::uint8_t>{0,128,255}&&q.modbus_length==-3&&q.ret_length==64&&q.host_id==10&&q.is_transparent_transmission&&q.use_503_port;}
static bool response(M::Response const& s){return s.ret==-7&&s.message==std::string("native-\xC2\xB5",9)&&s.ret_data==std::vector<std::uint8_t>{127,0,255};}
static ros::capability_spec cap(char const* alias,char const* operation,d::row_identity row){
 ros::capability_spec p{};p.alias=alias;p.operation=operation;p.resource="/uwvm_custom/get_set_modbus_data";p.type=row.type;p.type_identity=row.native;
 for(auto const& pair:std::initializer_list<std::pair<std::string,std::string>>{{"validator","none"},{"adapter-codec-identity",std::string(row.canonical)},
 {"max-request-bytes",std::to_string(Q::wire_bytes)},{"max-response-bytes",std::to_string(S::wire_bytes)},{"max-inflight","4"},{"timeout-ms","5000"},
 {"qos.history","keep-last"},{"qos.depth","4"},{"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}})p.limits.emplace(pair);return p;
}
#if UWVM_NATIVE_CUSTOM_SERVICE_METADATA_API && UWVM_ROS2_HAVE_TYPE_DESCRIPTION && UWVM_ROS2_HAVE_OPENSSL && UWVM_CUSTOM_SCALAR_C_METADATA_API
/** Mutate ONLY private copies of actual modern TypeDescription DATA. No fake
 * Native object/TypeSupport is installed, no SDK memory is modified, and the
 * actual baked hash is unchanged. This directly exercises the SAME full-field
 * comparator used by factory metadata() for generic and actual handles. */
static void default_metadata(d::row_identity row,uwvm_custom_modbus_c_observation const& original_c){
 auto schema=ipc::custom_native_interface_schema<M>::schema();auto index=ros2_custom_topics::native_schema_index(schema,Q::type);
 CHECK(index<schema.record_count);if(index==schema.record_count)return;auto const& expected=schema.records[index];CHECK(expected.field_count==6);if(expected.field_count!=6)return;
 auto generic=rosidl_typesupport_cpp::get_message_type_support_handle<M::Request>();auto actual=generic&&generic->func?generic->func(generic,rosidl_typesupport_introspection_cpp::typesupport_identifier):nullptr;
 CHECK(generic&&actual&&generic->get_type_description_func&&actual->get_type_description_func);if(!generic||!actual||!generic->get_type_description_func||!actual->get_type_description_func)return;
 auto description=generic->get_type_description_func(generic);auto native=actual->get_type_description_func(actual);
 CHECK(description&&native&&ros2_custom_topics::native_record_matches(description->type_description,expected)&&ros2_custom_topics::native_record_matches(native->type_description,expected));
 if(!description||!native||description->type_description.fields.size!=6)return;
 CHECK(expected.fields[1].native_default=="0"&&expected.fields[3].native_default=="9"&&expected.fields[4].native_default=="False"&&expected.fields[5].native_default=="False");
 CHECK(ros2_compat::matches_rihs(schema,row.type,row.native));
 auto record=description->type_description;std::vector<rosidl_runtime_c__type_description__Field> changed(record.fields.data,record.fields.data+record.fields.size);
 record.fields.data=changed.data();record.fields.capacity=record.fields.size;char wrong[]="1";
 changed[1].default_value.data=wrong;changed[1].default_value.size=1;changed[1].default_value.capacity=1;
 CHECK(!ros2_custom_topics::native_record_matches(record,expected)); // Same hash/shape/width, wrong literal.
 changed[1].default_value.data=nullptr;changed[1].default_value.size=0;changed[1].default_value.capacity=0;
 CHECK(!ros2_custom_topics::native_record_matches(record,expected)); // Erased actual default is not UNKNOWN acceptance.
 std::vector<ipc::custom_native_field> erased(expected.fields,expected.fields+expected.field_count);erased[1].native_default={};
 auto erased_record=expected;erased_record.fields=erased.data();CHECK(!ros2_custom_topics::native_record_matches(description->type_description,erased_record));
 std::vector<ipc::custom_native_record> records(schema.records,schema.records+schema.record_count);records[index]=erased_record;
 CHECK(ros2_compat::matches_rihs({records.data(),records.size()},row.type,row.native)); // RIHS does NOT contain defaults.
 CHECK(original_c.request&&original_c.response&&original_c.event&&original_c.service);
 if(original_c.request&&original_c.request->get_type_description_func&&original_c.request->get_type_hash_func){
  auto cd=original_c.request->get_type_description_func(original_c.request);CHECK(cd&&ros2_custom_topics::native_record_matches(cd->type_description,expected));
  CHECK(ros2_compat::baked_hash(original_c.request)==Q::native_identity);
 }else CHECK(false);
 CHECK(ros2_compat::baked_hash(original_c.response)==S::native_identity&&ros2_compat::baked_hash(original_c.service)==row.native);
}
struct images {
 std::array<unsigned char,Q::wire_bytes+1> initial{},request{};
 std::array<unsigned char,S::wire_bytes+1> response{};uwvm_custom_modbus_c_observation c{};
};
static bool local(std::shared_ptr<Proof const> const& proof,d::row_identity row,images& out){
 auto root=proof->root();CHECK(root&&root->validate(root,root->generated_support)==0);if(!root)return false;
 CHECK(uwvm_custom_modbus_c(out.initial.data(),out.initial.size(),out.request.data(),out.request.size(),out.response.data(),out.response.size(),&out.c)==0);
 CHECK(out.c.default_request_bytes==11&&out.c.request_bytes==14&&out.c.response_bytes==22);
 auto original_images=out;CHECK(uwvm_custom_modbus_c(out.initial.data(),0,out.request.data(),out.request.size(),out.response.data(),out.response.size(),&out.c)==90&&out.initial==original_images.initial&&out.request==original_images.request&&out.response==original_images.response&&out.c.default_request_bytes==original_images.c.default_request_bytes&&out.c.request_bytes==original_images.c.request_bytes&&out.c.response_bytes==original_images.c.response_bytes&&out.c.request==original_images.c.request&&out.c.response==original_images.c.response&&out.c.event==original_images.c.event&&out.c.service==original_images.c.service);
 M::Request native{};CHECK(defaults(native));std::array<unsigned char,Q::wire_bytes+1> bytes{};std::size_t used=SIZE_MAX;
 CHECK(root->request->encode(&native,bytes.data(),bytes.size(),&used)==0&&used==11&&!std::memcmp(bytes.data(),out.initial.data(),11));
 auto guest=roswasmcpp::Codec<Guest>::default_value();CHECK(guest.modbus_data.size==0&&guest.modbus_length==0&&guest.ret_length==0&&guest.host_id==9&&!guest.is_transparent_transmission&&!guest.use_503_port);
 roswasmcpp::wire::bytes canonical;CHECK(roswasmcpp::Codec<Guest>::encode(guest,canonical)&&canonical.size()==11&&!std::memcmp(canonical.data(),out.initial.data(),11));
 CHECK(root->request->decode(out.request.data(),14,&native)==0&&request(native));auto saved=native;
 auto bad=out.request;bad[bad.size()-1]=0;bad[12]=2;CHECK(root->request->decode(bad.data(),14,&native)==22&&native.modbus_data==saved.modbus_data&&native.modbus_length==saved.modbus_length&&native.ret_length==saved.ret_length&&native.host_id==saved.host_id&&native.is_transparent_transmission==saved.is_transparent_transmission&&native.use_503_port==saved.use_503_port);
 CHECK(root->request->decode(out.request.data(),13,&native)==22&&request(native));bad=out.request;bad[14]=0;CHECK(root->request->decode(bad.data(),15,&native)==22&&request(native));
 native.modbus_data.resize(65);used=123;CHECK(root->request->encode(&native,bytes.data(),bytes.size(),&used)==22&&used==123);native=saved;
 M::Response result{};CHECK(root->response->decode(out.response.data(),22,&result)==0&&response(result));auto previous=result;
 auto invalid=out.response;invalid[6]=0;CHECK(root->response->decode(invalid.data(),22,&result)==22&&result.ret==previous.ret&&result.message==previous.message&&result.ret_data==previous.ret_data);
 CHECK(root->response->decode(out.response.data(),21,&result)==22&&response(result));result.ret_data.resize(65);std::array<unsigned char,S::wire_bytes+1> response_bytes{};used=123;CHECK(root->response->encode(&result,response_bytes.data(),response_bytes.size(),&used)==22&&used==123);result=previous;
 auto call=cap("scalar-call","service.call",row),respond=cap("scalar-respond","service.respond",row);c::canonical_limits bounds{};
 CHECK(c::canonical_bounds(call,bounds)==0&&bounds.request==Q::wire_bytes&&bounds.response==S::wire_bytes);
 CHECK(c::validate_body(call,false,out.initial.data(),11)==0&&c::validate_body(call,false,out.request.data(),14)==0&&c::validate_body(respond,true,out.response.data(),22)==0);
 CHECK(c::validate_body(call,false,bad.data(),15)==22&&c::validate_body(respond,true,invalid.data(),22)==22);
 default_metadata(row,out.c);
 // Genuine selected Event callbacks, complete default-bearing Request, Response
 // and actual Info/Time. No copied synthetic event struct replaces the Native.
 auto support=rosidl_typesupport_cpp::get_service_type_support_handle<M>();rosidl_service_introspection_info_t info{};info.event_type=1;info.stamp_sec=-2;info.stamp_nanosec=300000000;info.sequence_number=INT64_MIN;
 auto allocator=rcutils_get_default_allocator();void* event=support->event_message_create_handle_function(&info,&allocator,&native,&result);CHECK(event);
 if(event){auto const& value=*static_cast<M::Event const*>(event);CHECK(value.info.stamp.sec==-2&&value.info.stamp.nanosec==300000000&&value.info.sequence_number==INT64_MIN&&value.request.size()==1&&value.response.size()==1);if(value.request.size()==1)CHECK(request(value.request[0]));if(value.response.size()==1)CHECK(response(value.response[0]));for(auto byte:value.info.client_gid)CHECK(byte==0);CHECK(support->event_message_destroy_handle_function(event,&allocator));}
 return failures==0;
}
static bool loopback(rclcpp::Node::SharedPtr const& node,std::shared_ptr<uwvm_rmw_cpp::Registry const> const& original,d::row_identity row,images const& data){
 auto call=cap("scalar-call","service.call",row),advertise=cap("scalar-advertise","service.advertise",row),respond=cap("scalar-respond","service.respond",row);
 n::wire::binding a{2,UINT64_C(0x100000101),2,3,4},b{2,UINT64_C(0x100000102),2,3,4},r{2,UINT64_C(0x100000103),2,3,4};
 auto cr=std::make_shared<n::report>(),sr=std::make_shared<n::report>();std::shared_ptr<c::endpoint> client,server;n::wire::packet cp{},sp{};
 auto code=c::create_selected(node,original,advertise,b,30,8,sr,server,sp);CHECK(code==0);if(code)return false;
 struct retire{std::shared_ptr<c::endpoint>& object;n::wire::binding owner;std::uint64_t inc;bool armed=true;~retire(){if(armed&&object)CHECK(object->retire(owner,inc)==0);}} sg{server,b,30};
 code=c::create_selected(node,original,call,a,31,8,cr,client,cp);CHECK(code==0);if(code)return false;retire cg{client,a,31};
 auto query=cap("scalar-query","service.available",row);query.limits.erase("validator");query.limits["service-source-alias"]=call.alias;query.limits["max-request-bytes"]="64";query.limits["max-response-bytes"]="64";query.limits["max-inflight"]="1";query.limits["max-native-clients"]="1";query.limits["max-binding-history"]="8";query.limits["max-wait-ns"]="1000000";
 n::wire::binding q{2,UINT64_C(0x100000104),2,3,4};bool ready{};auto until=std::chrono::steady_clock::now()+std::chrono::seconds(5);
 do{code=client->available(a,31,q,query,ready);if(code||ready)break;std::this_thread::sleep_for(std::chrono::milliseconds(1));}while(std::chrono::steady_clock::now()<until);CHECK(code==0&&ready);if(code||!ready)return false;
 std::uint64_t ticket{};code=client->submit(a,31,data.request.data(),data.c.request_bytes,ticket);CHECK(code==0&&ticket);if(code)return false;
 auto take=[&](std::shared_ptr<c::endpoint> const& object,n::wire::binding owner,std::uint64_t inc,n::snapshot& snapshot){auto end=std::chrono::steady_clock::now()+std::chrono::seconds(5);for(;;){auto error=object->poll(owner,inc);if(error&&error!=11)return error;error=object->peek(owner,inc,16,snapshot,n::events::max_payload_bytes);if(error!=11)return error;if(std::chrono::steady_clock::now()>=end)return 110u;std::this_thread::sleep_for(std::chrono::milliseconds(1));}};
 n::snapshot request_record;code=take(server,b,30,request_record);CHECK(code==0);if(code)return false;
 CHECK(request_record.bytes&&request_record.size>=192);if(!request_record.bytes||request_record.size<192)return false;
 n::info::header request_info{};bool request_metadata=n::info::decode(request_record.bytes.get(),192,request_record.size-192,request_info)&&n::info::complete(request_info)&&request_info.correlation!=0;CHECK(request_metadata);if(!request_metadata)return false;
 M::Request actual_request{};CHECK(Q::decode(request_record.bytes.get()+192,request_record.size-192,actual_request)&&request(actual_request));
 CHECK(server->cancel(b,30,request_record.sequence,request_record.reservation)==0);n::snapshot next;CHECK(server->peek(b,30,16,next,n::events::max_payload_bytes)==0&&next.sequence==request_record.sequence&&next.reservation!=request_record.reservation);
 CHECK(server->commit(b,30,next.sequence,next.reservation)==0);
 CHECK(server->respond(b,30,r,respond,request_info.correlation,data.response.data(),data.c.response_bytes)==0);
 n::snapshot response_record;code=take(client,a,31,response_record);CHECK(code==0);if(code)return false;
 CHECK(response_record.bytes&&response_record.size>=192);if(!response_record.bytes||response_record.size<192)return false;
 n::info::header response_info{};bool response_metadata=n::info::decode(response_record.bytes.get(),192,response_record.size-192,response_info)&&n::info::complete(response_info)&&response_info.correlation==ticket&&response_info.writer_size==request_info.writer_size&&response_info.writer==request_info.writer&&response_info.request_sequence==request_info.request_sequence;CHECK(response_metadata);if(!response_metadata)return false;
 M::Response actual_response{};CHECK(S::decode(response_record.bytes.get()+192,response_record.size-192,actual_response)&&response(actual_response));CHECK(client->commit(a,31,response_record.sequence,response_record.reservation)==0);
 CHECK(cr->observe().native[static_cast<unsigned>(n::operation::send)].calls==1&&sr->observe().native[static_cast<unsigned>(n::operation::respond)].calls==1);
 CHECK(client->retire(a,31)==0);cg.armed=false;CHECK(server->retire(b,30)==0);sg.armed=false;
 CHECK(cr->observe().native[static_cast<unsigned>(n::operation::fini)].calls==1&&sr->observe().native[static_cast<unsigned>(n::operation::fini)].calls==1);return failures==0;
}
#endif
int main(int argc,char** argv){
#if !UWVM_NATIVE_CUSTOM_SERVICE_METADATA_API || !UWVM_ROS2_HAVE_TYPE_DESCRIPTION || !UWVM_ROS2_HAVE_OPENSSL || !UWVM_CUSTOM_SCALAR_C_METADATA_API
 (void)argc;(void)argv;return 95;
#else
 if(argc!=2)return 22;bool active{};int primary=5;
 try{d::row_identity row{};for(auto const& current:d::identities)if(current.type=="xarm_msgs/srv/GetSetModbusData")row=current;if(row.type.empty())return 95;
  std::shared_ptr<Proof const> proof;auto code=Proof::qualify(row,proof);if(code)return static_cast<int>(code);
  // Warm and verify all generated/C metadata/defaults in the owned loader phase.
  images data{};if(!local(proof,row,data))return 22;
  uwvm_rmw_cpp::NativeIdentities actual{};if(!identities(argv[1],actual))return 22;auto original=std::make_shared<uwvm_rmw_cpp::Registry const>(actual,4096);
  rclcpp::init(argc,argv);active=true;{auto node=std::make_shared<rclcpp::Node>("uwvm_custom_scalar_native_contract");primary=loopback(node,original,row,data)?0:22;node.reset();}
 }catch(std::bad_alloc const&){primary=12;}catch(std::exception const& e){native_io::diagnostic("scalar Native contract: ",e.what());primary=5;}catch(...){primary=5;}
 if(active)try{rclcpp::shutdown();}catch(...){if(!primary)primary=5;}
 if(failures&&!primary)primary=22;if(!primary&&!native_io::write_all(STDOUT_FILENO,"PASS_CUSTOM_SCALAR_ORIGINAL_NATIVE_COMPONENT_NOT_HOST_GUEST_OR_ROBOT_ACCEPTANCE\n"))primary=5;return primary;
#endif
}
