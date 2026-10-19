// SPDX-License-Identifier: Apache-2.0
// AUTHORED, UNEXECUTED. Genuine generated C and C++ objects, no mock native
// structs/TypeSupport. Run each mode in a fresh independently bounded process:
// native metadata warm/create is deliberately single-attempt per Source.
#include <uwvm_rmw/default_control.h>
#include <uwvm_rmw/codec_composer.h>
#include <uwvm_default_control_identity_input.hpp>
#include <bounded_records.h>
#include <rcl_interfaces/msg/log.h>
#include <rcl_interfaces/msg/parameter_event.hpp>
#include <rcl_interfaces/srv/get_parameters.hpp>
#include <type_description_interfaces/srv/get_type_description.h>
#include <rosidl_runtime_c/string_functions.h>
#include <type_description_interfaces/msg/type_description.h>
#include <type_description_interfaces/msg/individual_type_description.h>
#include <type_description_interfaces/msg/field.h>
#include <type_description_interfaces/msg/type_source.h>
#include <rcutils/allocator.h>
#include <fast_io.h>
#include <algorithm>
#include <array>
#include <atomic>
#include "../detail/decimal.hpp"
#include <cstring>
#include <limits>
#include <new>
#include <string>
#include <string_view>
// Only THIS Registry/composer arena's explicit nothrow allocation is faulted.
// This does not pretend to inject failures into original SDK C allocators.
static std::atomic<bool>fail_arena{};
void*operator new(std::size_t n,std::nothrow_t const&)noexcept {if(fail_arena.load())return nullptr;try{return ::operator new(n);}catch(...){return nullptr;}}
void operator delete(void*p,std::nothrow_t const&)noexcept {::operator delete(p);}
namespace {
struct Failure{};
#define REQUIRE(x) do{if(!(x)){fast_io::perr("contract failed: ",#x,"\n");throw Failure{};}}while(false)
std::array<unsigned char,32768> wire{},roundtrip{};
uwvm_default_control_registry*owner{};uwvm_rmw_codec const*table{};std::size_t count{};
std::uint32_t event_cleanup_failures{};
void prepare(){auto const&ids=uwvm_default_control_input::identities;
 REQUIRE(uwvm_default_control_create(ids.data(),ids.size(),&owner)==0);
 REQUIRE(uwvm_default_control_export(owner,&table,&count)==0);REQUIRE(table&&count>0);
 REQUIRE(uwvm_default_control_destroy_unexported(&owner)==17&&owner);}
uwvm_rmw_codec const&codec(char const*name){for(std::size_t i{};i<count;++i)if(!std::strcmp(table[i].native_type,name))return table[i];throw Failure{};}
void text(rosidl_runtime_c__String&out,std::string_view in){REQUIRE(rosidl_runtime_c__String__assignn(&out,in.data(),in.size()));}
template<std::size_t N>void canonical_text(unsigned char(&out)[N],std::uint32_t&size,std::string_view in){REQUIRE(in.size()<=N);size=static_cast<std::uint32_t>(in.size());std::memcpy(out,in.data(),in.size());}
void log_roundtrip(){
 rcl_interfaces__msg__Log input{},output{};REQUIRE(rcl_interfaces__msg__Log__init(&input));
 struct Guard{rcl_interfaces__msg__Log*a,*b;~Guard(){rcl_interfaces__msg__Log__fini(a);rcl_interfaces__msg__Log__fini(b);}}guard{&input,&output};
 REQUIRE(rcl_interfaces__msg__Log__init(&output));input.stamp.sec=-7;input.stamp.nanosec=999999999;
 input.level=rcl_interfaces__msg__Log__WARN;input.line=4294967295u;
 text(input.name,"logger");text(input.msg,"native-\xC2\xB5");text(input.file,"original.cc");text(input.function,"all_fields");
 auto const&c=codec("rcl_interfaces/msg/Log");std::size_t length=719;
 REQUIRE(c.encode(&input,wire.data(),wire.size(),&length)==0);
 REQUIRE(c.decode(wire.data(),length,&output)==0);
 REQUIRE(output.stamp.sec==input.stamp.sec&&output.stamp.nanosec==input.stamp.nanosec&&output.level==input.level&&output.line==input.line);
 REQUIRE(output.msg.size==9&&std::memcmp(output.msg.data,"native-\xC2\xB5",9)==0);
 REQUIRE(output.name.size==6&&output.file.size==11&&output.function.size==10);
 // Genuine custom canonical strings forbid NUL; builtin String has a different
 // codec domain. No global UTF-8/profile/identity relaxation is appropriate.
 text(input.msg,std::string_view("x\0y",3));wire.fill(0xa5);auto before=wire;length=719;
 REQUIRE(c.encode(&input,wire.data(),wire.size(),&length)==22&&length==719&&wire==before);
 text(input.msg,"valid");length=719;REQUIRE(c.encode(&input,wire.data(),0,&length)==90&&length==719&&wire==before);
 text(input.msg,std::string(1025,'x'));REQUIRE(c.encode(&input,wire.data(),wire.size(),&length)==90&&length==719&&wire==before);
}
void parameter_fullvalue(){
 rcl_interfaces::msg::ParameterEvent value;value.stamp.sec=-2;value.stamp.nanosec=300000000;value.node="/node";
 rcl_interfaces::msg::Parameter p;p.name="all_value_fields";p.value.type=0;
 // Type0 does NOT license dropping the inactive fields of ParameterValue.
 p.value.bool_value=true;p.value.integer_value=std::numeric_limits<std::int64_t>::min();p.value.double_value=-1.25;p.value.string_value="text";
 p.value.byte_array_value={0,255};p.value.bool_array_value={true,false,true};p.value.integer_array_value={-7,9};p.value.double_array_value={1.25,-2.5};p.value.string_array_value={"left","right"};
 value.new_parameters={p};value.changed_parameters={p};value.deleted_parameters={p};
 auto const&c=codec("rcl_interfaces/msg/ParameterEvent");std::size_t length=719;REQUIRE(c.encode(&value,wire.data(),wire.size(),&length)==0);
 rcl_interfaces::msg::ParameterEvent output;REQUIRE(c.decode(wire.data(),length,&output)==0&&output==value);
 auto previous=output;REQUIRE(c.decode(wire.data(),length-1,&output)==22&&output==previous);
 value.changed_parameters[0].value.double_value=std::numeric_limits<double>::infinity();length=719;wire.fill(0xa5);auto before=wire;
 REQUIRE(c.encode(&value,wire.data(),wire.size(),&length)==22&&length==719&&wire==before);
}
void get_type_request(){
 std::uint64_t parsed=719;REQUIRE(uwvm_default_control_detail::decimal("18446744073709551615",parsed)&&parsed==UINT64_MAX);
 // Preserve the original decimal input domain, including redundant leading zeroes.
 REQUIRE(uwvm_default_control_detail::decimal("003",parsed)&&parsed==3);
 REQUIRE(uwvm_default_control_detail::decimal("00",parsed)&&parsed==0);
 REQUIRE(uwvm_default_control_detail::decimal("00018446744073709551615",parsed)&&parsed==UINT64_MAX);
 for(auto invalid:{"","+3","-3"," 3","3 ","3x","18446744073709551616","00018446744073709551616","00x","000+3"}){parsed=719;REQUIRE(!uwvm_default_control_detail::decimal(invalid,parsed)&&parsed==719);}

 using T=type_description_interfaces__srv__GetTypeDescription_Request;T value{},output{};
 REQUIRE(type_description_interfaces__srv__GetTypeDescription_Request__init(&value));
 struct Guard{T*a,*b;~Guard(){type_description_interfaces__srv__GetTypeDescription_Request__fini(a);type_description_interfaces__srv__GetTypeDescription_Request__fini(b);}}guard{&value,&output};
 REQUIRE(type_description_interfaces__srv__GetTypeDescription_Request__init(&output));REQUIRE(value.include_type_sources);
 auto const&root=codec("type_description_interfaces/srv/GetTypeDescription");auto const&c=*root.request;
 text(value.type_name,"rcl_interfaces/msg/Log");text(value.type_hash,"");std::size_t length=719;
 REQUIRE(c.encode(&value,wire.data(),wire.size(),&length)==0&&c.decode(wire.data(),length,&output)==0&&output.include_type_sources&&output.type_hash.size==0);
 // Build 71 characters from an ACTUAL independently exported selected hash.
 constexpr char digits[]="0123456789abcdef";std::string actual="RIHS01_";
 auto const&log=codec("rcl_interfaces/msg/Log");for(auto b:log.native_rihs01){actual.push_back(digits[b>>4]);actual.push_back(digits[b&15]);}
 REQUIRE(actual.size()==71);text(value.type_hash,actual);value.include_type_sources=false;
 REQUIRE(c.encode(&value,wire.data(),wire.size(),&length)==0&&c.decode(wire.data(),length,&output)==0);
 REQUIRE(output.type_hash.size==71&&!output.include_type_sources&&std::memcmp(output.type_hash.data,actual.data(),71)==0);
 auto previous=output;std::array<unsigned char,sizeof(T)>saved{};std::memcpy(saved.data(),&output,sizeof(T));
 REQUIRE(c.decode(wire.data(),length-1,&output)==22&&std::memcmp(saved.data(),&output,sizeof(T))==0&&output.type_hash.data==previous.type_hash.data);
 text(value.type_hash,actual+"x");wire.fill(0xa5);auto before=wire;length=719;
 REQUIRE(c.encode(&value,wire.data(),wire.size(),&length)==90&&length==719&&wire==before);
}
void metadata_individual(rosidl_runtime_c__type_description__IndividualTypeDescription const&source,type_description_interfaces__msg__IndividualTypeDescription&target){
 text(target.type_name,std::string_view{source.type_name.data,source.type_name.size});
 REQUIRE(source.fields.capacity>=source.fields.size&&(!source.fields.size||source.fields.data));
 type_description_interfaces__msg__Field__Sequence__fini(&target.fields);target.fields={};REQUIRE(type_description_interfaces__msg__Field__Sequence__init(&target.fields,source.fields.size));
 for(std::size_t i{};i<source.fields.size;++i){auto const&a=source.fields.data[i];auto&v=target.fields.data[i];text(v.name,std::string_view{a.name.data,a.name.size});v.type.type_id=a.type.type_id;v.type.capacity=a.type.capacity;v.type.string_capacity=a.type.string_capacity;text(v.type.nested_type_name,std::string_view{a.type.nested_type_name.data,a.type.nested_type_name.size});text(v.default_value,std::string_view{a.default_value.data,a.default_value.size});}
}
void get_type_response(){
 using R=roswasmc_bounded_2_type_description_interfaces_srv_GetTypeDescription_Response;
 R expected{};expected.successful=1;
 canonical_text(expected.failure_reason.data,expected.failure_reason.size,"retained even on success");
 auto&individual=expected.type_description.type_description;canonical_text(individual.type_name.data,individual.type_name.size,"actual/Type");individual.fields.size=1;
 auto&field=individual.fields.data[0];canonical_text(field.name.data,field.name.size,"type");field.type.type_id=16;field.type.capacity=9;field.type.string_capacity=7;
 canonical_text(field.type.nested_type_name.data,field.type.nested_type_name.size,"nested/Name");canonical_text(field.default_value.data,field.default_value.size,"false");
 expected.type_description.referenced_type_descriptions.size=1;expected.type_description.referenced_type_descriptions.data[0]=individual;
 expected.type_sources.size=1;auto&source=expected.type_sources.data[0];canonical_text(source.type_name.data,source.type_name.size,"source/Name");canonical_text(source.encoding.data,source.encoding.size,"msg");canonical_text(source.raw_file_contents.data,source.raw_file_contents.size,"uint8 type\n");
 expected.extra_information.size=1;canonical_text(expected.extra_information.data[0].key.data,expected.extra_information.data[0].key.size,"key");canonical_text(expected.extra_information.data[0].value.data,expected.extra_information.data[0].value.size,"value");
 std::size_t length=719;REQUIRE(roswasmc_bounded_2_type_description_interfaces_srv_GetTypeDescription_Response_encode(&expected,wire.data(),wire.size(),&length)==0);
 type_description_interfaces__srv__GetTypeDescription_Response value{};REQUIRE(type_description_interfaces__srv__GetTypeDescription_Response__init(&value));
 struct Guard{decltype(value)*p;~Guard(){type_description_interfaces__srv__GetTypeDescription_Response__fini(p);}}guard{&value};
 auto const&c=*codec("type_description_interfaces/srv/GetTypeDescription").response;REQUIRE(c.decode(wire.data(),length,&value)==0);
 REQUIRE(value.successful&&value.failure_reason.size==24&&value.type_description.type_description.fields.size==1&&value.type_sources.size==1&&value.extra_information.size==1);
 std::size_t returned=719;REQUIRE(c.encode(&value,roundtrip.data(),roundtrip.size(),&returned)==0&&returned==length&&std::equal(wire.begin(),wire.begin()+length,roundtrip.begin()));
 // Complete actual service getter closure; real typed business C objects own
 // every assigned string/sequence. Runtime metadata structs are DIFFERENT types. The original native source may include NUL
 // in its counted size; raw_file_contents is the sole admitted raw domain.
 auto support=static_cast<rosidl_service_type_support_t const*>(codec("type_description_interfaces/srv/GetTypeDescription").generated_support);
 auto description=support->get_type_description_func(support);auto sources=support->get_type_description_sources_func(support);
 REQUIRE(description&&sources&&description->referenced_type_descriptions.size==11&&sources->size==12);
 metadata_individual(description->type_description,value.type_description.type_description);
 auto&refs=value.type_description.referenced_type_descriptions;type_description_interfaces__msg__IndividualTypeDescription__Sequence__fini(&refs);refs={};REQUIRE(type_description_interfaces__msg__IndividualTypeDescription__Sequence__init(&refs,description->referenced_type_descriptions.size));
 for(std::size_t i{};i<refs.size;++i)metadata_individual(description->referenced_type_descriptions.data[i],refs.data[i]);
 type_description_interfaces__msg__TypeSource__Sequence__fini(&value.type_sources);value.type_sources={};REQUIRE(type_description_interfaces__msg__TypeSource__Sequence__init(&value.type_sources,sources->size));
 for(std::size_t i{};i<sources->size;++i){auto const&a=sources->data[i];auto&v=value.type_sources.data[i];text(v.type_name,std::string_view{a.type_name.data,a.type_name.size});text(v.encoding,std::string_view{a.encoding.data,a.encoding.size});text(v.raw_file_contents,std::string_view{a.raw_file_contents.data,a.raw_file_contents.size});}
 value.successful=true;length=719;REQUIRE(c.encode(&value,wire.data(),wire.size(),&length)==0&&length<=32768);
 type_description_interfaces__srv__GetTypeDescription_Response complete{};struct CompleteGuard{decltype(complete)*p;~CompleteGuard(){type_description_interfaces__srv__GetTypeDescription_Response__fini(p);}}whole{&complete};
 REQUIRE(c.decode(wire.data(),length,&complete)==0&&complete.type_sources.size==12&&complete.type_description.referenced_type_descriptions.size==11);
 for(std::size_t i{};i<12;++i){auto const&a=value.type_sources.data[i];auto const&v=complete.type_sources.data[i];REQUIRE(a.raw_file_contents.size==v.raw_file_contents.size&&!std::memcmp(a.raw_file_contents.data,v.raw_file_contents.data,a.raw_file_contents.size));}
 // Each source obeys its own finite bound, yet their Cartesian combination
 // exceeds the separate total response quota. Preserve public bytes/length90.
 for(std::size_t i{};i<12;++i)text(value.type_sources.data[i].raw_file_contents,std::string(6162,'x'));
 wire.fill(0xa5);auto saved=wire;length=719;REQUIRE(c.encode(&value,wire.data(),wire.size(),&length)==90&&wire==saved&&length==719);

}
void composer_contract(){
 uwvm_rmw_codec_table_view view{table,count};uwvm_rmw_codec_composer*composed{};REQUIRE(uwvm_rmw_codec_composer_create(&view,1,&composed)==0);
 uwvm_rmw_codec const*clones{};std::size_t total{};REQUIRE(uwvm_rmw_codec_composer_export(composed,&clones,&total)==0&&total==count&&clones!=table);
 for(std::size_t i{};i<total;++i){REQUIRE(clones[i].validate(&clones[i],table[i].generated_support)==0);
 if(clones[i].kind==UWVM_RMW_SERVICE_CODEC){REQUIRE(clones[i].request>=clones&&clones[i].request<clones+total&&clones[i].response>=clones&&clones[i].response<clones+total&&clones[i].request!=table[i].request);}}
 // A copied descriptor at an unrelated address must fail, even if every byte
 // and original support pointer is copied. Original table remains valid.
 auto stolen=clones[0];REQUIRE(stolen.validate(&stolen,stolen.generated_support)==22);
 REQUIRE(table[0].validate(&table[0],table[0].generated_support)==0);
 REQUIRE(uwvm_rmw_codec_composer_destroy_unexported(&composed)==17&&composed);
 uwvm_rmw_codec_table_view duplicates[]{view,view};uwvm_rmw_codec_composer*untouched=reinterpret_cast<uwvm_rmw_codec_composer*>(1);
 REQUIRE(uwvm_rmw_codec_composer_create(duplicates,2,&untouched)==22&&untouched==reinterpret_cast<uwvm_rmw_codec_composer*>(1));
}
void event_callbacks(){
 auto allocator=rcutils_get_default_allocator();rosidl_service_introspection_info_t info{};info.event_type=1;info.stamp_sec=-2;info.stamp_nanosec=300000000;info.sequence_number=std::numeric_limits<std::int64_t>::min();
 struct EventGuard{void*p;rosidl_service_type_support_t const*h;rcutils_allocator_t*a;~EventGuard(){if(p&&!h->event_message_destroy_handle_function(p,a))++event_cleanup_failures;}};
 type_description_interfaces__srv__GetTypeDescription_Request request{};type_description_interfaces__srv__GetTypeDescription_Response response{};
 REQUIRE(type_description_interfaces__srv__GetTypeDescription_Request__init(&request));
 struct NativeGuard{decltype(request)*q;decltype(response)*s;~NativeGuard(){type_description_interfaces__srv__GetTypeDescription_Request__fini(q);type_description_interfaces__srv__GetTypeDescription_Response__fini(s);}}native{&request,&response};
 REQUIRE(type_description_interfaces__srv__GetTypeDescription_Response__init(&response));text(request.type_name,"rcl_interfaces/msg/Log");
 auto h=static_cast<rosidl_service_type_support_t const*>(codec("type_description_interfaces/srv/GetTypeDescription").generated_support);
 EventGuard c{h->event_message_create_handle_function(&info,&allocator,&request,&response),h,&allocator};REQUIRE(c.p);
 auto event=static_cast<type_description_interfaces__srv__GetTypeDescription_Event*>(c.p);
 REQUIRE(event->request.size==1&&event->response.size==1&&event->request.data[0].include_type_sources&&event->request.data[0].type_name.size==request.type_name.size&&event->info.stamp.sec==-2&&event->info.stamp.nanosec==300000000&&event->info.sequence_number==info.sequence_number);
 for(auto b:event->info.client_gid)REQUIRE(b==0); // genuine zero GID is data
 rcl_interfaces::srv::GetParameters::Request q;q.names={"source_value"};rcl_interfaces::srv::GetParameters::Response s;rcl_interfaces::msg::ParameterValue value;value.integer_value=-17;s.values={value};
 h=static_cast<rosidl_service_type_support_t const*>(codec("rcl_interfaces/srv/GetParameters").generated_support);
 EventGuard cpp{h->event_message_create_handle_function(&info,&allocator,&q,&s),h,&allocator};REQUIRE(cpp.p);
 auto cpp_event=static_cast<rcl_interfaces::srv::GetParameters_Event*>(cpp.p);
 REQUIRE(cpp_event->request.size()==1&&cpp_event->response.size()==1&&cpp_event->request[0]==q&&cpp_event->response[0]==s&&cpp_event->info.sequence_number==info.sequence_number);
}
}
int main(int argc,char**argv){
 if(argc!=2&&argc!=3)return 22;try{std::string_view mode=argv[1];
 if(mode=="allocation"){
  auto const&ids=uwvm_default_control_input::identities;auto*sentinel=reinterpret_cast<uwvm_default_control_registry*>(1);fail_arena.store(true);
  auto code=uwvm_default_control_create(ids.data(),ids.size(),&sentinel);fail_arena.store(false);REQUIRE(code==12&&sentinel==reinterpret_cast<uwvm_default_control_registry*>(1));
  REQUIRE(uwvm_default_control_create(ids.data(),ids.size(),&sentinel)==17);return 0;
 }
 if(mode=="bad-identity"){
  if(argc!=3)return 22;std::uint64_t index{};bool parsed=uwvm_default_control_detail::decimal(argv[2],index);
  auto bad=uwvm_default_control_input::identities;if(!parsed||index>=bad.size())return 22;
  bad[index].rihs01[0]^=0x80;bool nonzero{};for(auto b:bad[index].rihs01)nonzero|=b!=0;if(!nonzero)bad[index].rihs01[1]=1;
  auto*sentinel=reinterpret_cast<uwvm_default_control_registry*>(1);REQUIRE(uwvm_default_control_create(bad.data(),bad.size(),&sentinel)==22&&sentinel==reinterpret_cast<uwvm_default_control_registry*>(1));
  auto const&ids=uwvm_default_control_input::identities;REQUIRE(uwvm_default_control_create(ids.data(),ids.size(),&sentinel)==17);return 0;
 }
 if(argc!=2)return 22;prepare();
 if(mode=="log")log_roundtrip();else if(mode=="parameter")parameter_fullvalue();else if(mode=="request")get_type_request();else if(mode=="response")get_type_response();else if(mode=="composer")composer_contract();else if(mode=="events")event_callbacks();else return 22;
 REQUIRE(!event_cleanup_failures);
 fast_io::print("PASS_ACTUAL_OBJECT_CONTRACT_NOT_NATIVE_OR_GUEST_QUALIFICATION\n");return 0;
 }catch(...){return 22;}
}
