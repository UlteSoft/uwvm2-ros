// SPDX-License-Identifier: Apache-2.0
// Actual installed SDK and original default Node witness. This is NOT a Native
// stand-in, generated identifier oracle, Guest permission grant, or run receipt.
// Original C-aware Service headers and real C lifecycle link admission are mandatory.
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/service.hpp>
#include <rclcpp/any_service_callback.hpp>
#include <type_description_interfaces/srv/get_type_description.hpp>
#include <type_description_interfaces/srv/detail/get_type_description__type_support.h>
#include <rosidl_runtime_c/service_type_support_struct.h>
#include <rosidl_typesupport_interface/macros.h>
#include "bounded_records.h"
#include "control_wire.hpp"
#include <algorithm>
#include <fast_io.h>
#include <chrono>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#if !defined(UWVM_RCLCPP_C_LIFECYCLE_PORT) || UWVM_RCLCPP_C_LIFECYCLE_PORT != 1
#error genuine fixed-original C lifecycle port required
#endif
#if !defined(RCLCPP_UWVM_SERVICE_C_LIFECYCLE_HEADER_VERSION) || RCLCPP_UWVM_SERVICE_C_LIFECYCLE_HEADER_VERSION != 1
#error actual fixed Service source port required
#endif
#if !defined(RCLCPP_UWVM_CALLBACK_C_LIFECYCLE_HEADER_VERSION) || RCLCPP_UWVM_CALLBACK_C_LIFECYCLE_HEADER_VERSION != 1
#error actual fixed AnyServiceCallback source port required
#endif
namespace {
using Native=type_description_interfaces::srv::GetTypeDescription;
using Canonical=roswasmc_bounded_2_type_description_interfaces_srv_GetTypeDescription_Response;
constexpr auto measure=&roswasmc_bounded_2_type_description_interfaces_srv_GetTypeDescription_Response_measure;
constexpr auto encode=&roswasmc_bounded_2_type_description_interfaces_srv_GetTypeDescription_Response_encode;
constexpr auto decode=&roswasmc_bounded_2_type_description_interfaces_srv_GetTypeDescription_Response_decode;
void require(bool condition){if(!condition)throw std::runtime_error("complete source-domain Native witness refused");}
bool text(rosidl_runtime_c__String const& a,std::string const& b){return a.capacity>=a.size&&(a.data||!a.size)&&a.size==b.size()&&(!a.size||!std::memcmp(a.data,b.data(),a.size));}
template<class C>bool same(C const&a,std::string const&b){return a.size<=sizeof a.data&&a.size==b.size()&&(!a.size||!std::memcmp(a.data,b.data(),a.size));}
template<class C>void copy(C&out,std::string const&in){require(in.size()<=sizeof out.data);out.size=static_cast<std::uint32_t>(in.size());if(!in.empty())std::memcpy(out.data,in.data(),in.size());}
template<class C,class N>void field(C&c,N const&n){copy(c.name,n.name);c.type.type_id=n.type.type_id;c.type.capacity=n.type.capacity;c.type.string_capacity=n.type.string_capacity;copy(c.type.nested_type_name,n.type.nested_type_name);copy(c.default_value,n.default_value);}
template<class C,class N>bool same_field(C const&c,N const&n){return same(c.name,n.name)&&c.type.type_id==n.type.type_id&&c.type.capacity==n.type.capacity&&c.type.string_capacity==n.type.string_capacity&&same(c.type.nested_type_name,n.type.nested_type_name)&&same(c.default_value,n.default_value);}
template<class C,class N>void individual(C&c,N const&n){copy(c.type_name,n.type_name);require(n.fields.size()<=sizeof c.fields.data/sizeof c.fields.data[0]);c.fields.size=static_cast<std::uint32_t>(n.fields.size());for(std::size_t i=0;i<n.fields.size();++i)field(c.fields.data[i],n.fields[i]);}
template<class C,class N>bool same_individual(C const&c,N const&n){if(!same(c.type_name,n.type_name)||c.fields.size!=n.fields.size())return false;for(std::size_t i=0;i<n.fields.size();++i)if(!same_field(c.fields.data[i],n.fields[i]))return false;return true;}
template<class N>bool actual_individual(rosidl_runtime_c__type_description__IndividualTypeDescription const&c,N const&n){
 if(!text(c.type_name,n.type_name)||c.fields.size!=n.fields.size()||c.fields.capacity<c.fields.size||(c.fields.size&&!c.fields.data))return false;
 for(std::size_t i=0;i<n.fields.size();++i){auto const&a=c.fields.data[i];auto const&b=n.fields[i];if(!text(a.name,b.name)||a.type.type_id!=b.type.type_id||a.type.capacity!=b.type.capacity||a.type.string_capacity!=b.type.string_capacity||!text(a.type.nested_type_name,b.type.nested_type_name)||!text(a.default_value,b.default_value))return false;}return true;
}
void capture(Canonical&out,Native::Response const&in){
 out.successful=in.successful;copy(out.failure_reason,in.failure_reason);individual(out.type_description.type_description,in.type_description.type_description);
 auto const&refs=in.type_description.referenced_type_descriptions;require(refs.size()<=11);out.type_description.referenced_type_descriptions.size=static_cast<std::uint32_t>(refs.size());
 for(std::size_t i=0;i<refs.size();++i)individual(out.type_description.referenced_type_descriptions.data[i],refs[i]);
 require(in.type_sources.size()<=12);out.type_sources.size=static_cast<std::uint32_t>(in.type_sources.size());
 for(std::size_t i=0;i<in.type_sources.size();++i){auto&to=out.type_sources.data[i];auto const&from=in.type_sources[i];copy(to.type_name,from.type_name);copy(to.encoding,from.encoding);copy(to.raw_file_contents,from.raw_file_contents);}
 require(in.extra_information.size()<=2);out.extra_information.size=static_cast<std::uint32_t>(in.extra_information.size());
 for(std::size_t i=0;i<in.extra_information.size();++i){copy(out.extra_information.data[i].key,in.extra_information[i].key);copy(out.extra_information.data[i].value,in.extra_information[i].value);}
}
bool complete(Canonical const&c,Native::Response const&n){
 if(c.successful!=n.successful||!same(c.failure_reason,n.failure_reason)||!same_individual(c.type_description.type_description,n.type_description.type_description)||c.type_description.referenced_type_descriptions.size!=n.type_description.referenced_type_descriptions.size()||c.type_sources.size!=n.type_sources.size()||c.extra_information.size!=n.extra_information.size())return false;
 for(std::size_t i=0;i<n.type_description.referenced_type_descriptions.size();++i)if(!same_individual(c.type_description.referenced_type_descriptions.data[i],n.type_description.referenced_type_descriptions[i]))return false;
 for(std::size_t i=0;i<n.type_sources.size();++i){auto const&a=c.type_sources.data[i];auto const&b=n.type_sources[i];if(!same(a.type_name,b.type_name)||!same(a.encoding,b.encoding)||!same(a.raw_file_contents,b.raw_file_contents))return false;}
 for(std::size_t i=0;i<n.extra_information.size();++i)if(!same(c.extra_information.data[i].key,n.extra_information[i].key)||!same(c.extra_information.data[i].value,n.extra_information[i].value))return false;return true;
}
void witness(std::size_t domain,char const*program){
 // Warm the actual generated C root metadata before Node construction. The
 // TypeSupport owns these views; no guessed RIHS/native layout is introduced.
 auto support=ROSIDL_GET_SRV_TYPE_SUPPORT(type_description_interfaces,srv,GetTypeDescription);
 require(support&&support->get_type_hash_func&&support->get_type_description_func&&support->get_type_description_sources_func);
 auto hash=support->get_type_hash_func(support);auto description=support->get_type_description_func(support);auto sources=support->get_type_description_sources_func(support);
 require(hash&&hash->version==1&&description&&sources&&description->referenced_type_descriptions.size==11&&sources->size==12);
 require(description->referenced_type_descriptions.capacity>=11&&description->referenced_type_descriptions.data&&sources->capacity>=12&&sources->data);
 std::string hash_text="RIHS01_";constexpr char hex[]="0123456789abcdef";for(auto b:hash->value){hash_text.push_back(hex[b>>4]);hash_text.push_back(hex[b&15]);}require(hash_text.size()==71);
 rclcpp::InitOptions options;options.set_domain_id(domain);char const*args[]{program};rclcpp::init(1,args,options);
 // Unchanged defaults deliberately retain the genuine rcl C GTD handler. This
 // local native witness is separate from a future Guest bootstrap/grant closure.
 auto node=std::make_shared<rclcpp::Node>("control_sources_true");rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
 auto client=node->create_client<Native>(std::string(node->get_fully_qualified_name())+"/get_type_description");
 auto until=std::chrono::steady_clock::now()+std::chrono::seconds(15);
 while(!client->service_is_ready()){require(rclcpp::ok()&&std::chrono::steady_clock::now()<until);(void)client->wait_for_service(std::chrono::milliseconds(100));}
 auto request=std::make_shared<Native::Request>();require(request->include_type_sources); // actual original default
 request->type_name="type_description_interfaces/srv/GetTypeDescription";request->type_hash=hash_text;request->include_type_sources=true;
 // A service readiness result is not a remaining business-send budget.
 // Check the actual steady deadline again after preparing the request and
 // BEFORE the single native send; no expired entry may create a pending ID.
 require(std::chrono::steady_clock::now()<until);
 auto pending=client->async_send_request(request);auto remaining=until-std::chrono::steady_clock::now();
 // The actual send can consume the last budget. Retire this exact local SDK
 // pending entry before failure; do not spin with zero/negative (infinite) time.
 if(remaining<=decltype(remaining)::zero()){
  (void)client->remove_pending_request(pending.request_id);
  throw std::runtime_error("deadline expired during actual send; local pending removed, request never replayed");
 }
 if(executor.spin_until_future_complete(pending.future,remaining)!=rclcpp::FutureReturnCode::SUCCESS){(void)client->remove_pending_request(pending.request_id);throw std::runtime_error("real request incomplete; no request replay");}
 auto response=pending.future.get();require(response&&response->successful&&response->failure_reason.empty()&&response->extra_information.empty());
 require(actual_individual(description->type_description,response->type_description.type_description));require(response->type_description.referenced_type_descriptions.size()==11&&response->type_sources.size()==12);
 for(std::size_t i=0;i<11;++i)require(actual_individual(description->referenced_type_descriptions.data[i],response->type_description.referenced_type_descriptions[i]));
 std::size_t maximum=0;bool terminal_nul=false;
 for(std::size_t i=0;i<12;++i){auto const&a=sources->data[i];auto const&b=response->type_sources[i];require(text(a.type_name,b.type_name)&&text(a.encoding,b.encoding)&&text(a.raw_file_contents,b.raw_file_contents));maximum=std::max(maximum,b.raw_file_contents.size());terminal_nul|=!b.raw_file_contents.empty()&&b.raw_file_contents.back()=='\0';}
 require(maximum==6162&&terminal_nul); // exact fixed source family, not arbitrary installed version admission
 auto canonical=std::make_unique<Canonical>();auto decoded=std::make_unique<Canonical>();capture(*canonical,*response);require(complete(*canonical,*response));
 std::size_t length=SIZE_MAX;require(measure(canonical.get(),&length)==0&&length<=32768);std::vector<unsigned char> bytes(length);std::size_t used=SIZE_MAX;
 require(encode(canonical.get(),bytes.data(),bytes.size(),&used)==0&&used==length);
 require(uwvm2::imported::ros::ipc::validate_custom_wire(uwvm_control_wire::schema_Ros2Type_description_interfacesSrvGetTypeDescription_Response,bytes.data(),bytes.size()));
 require(decode(bytes.data(),bytes.size(),decoded.get())==0&&complete(*decoded,*response));
 executor.remove_node(node);client.reset();node.reset();require(rclcpp::shutdown());
}
}
int main(int argc,char**argv){
 if(argc!=2)return 22;std::size_t domain{};std::string_view word=argv[1];if(word.empty()||word.size()>20)return 22;
 // Actual fast_io decimal scanner: no whitespace skip, redundant leading
 // zeros accepted explicitly, leading plus disabled, complete token required.
 auto const [tail,status]=::fast_io::parse_by_scan(word.data(),word.data()+word.size(),::fast_io::mnp::dec_get<true,true>(domain));
 if(status!=::fast_io::parse_code::ok||tail!=word.data()+word.size()||domain>232)return 22;
 try{witness(domain,argv[0]);return 0;}catch(...){try{if(rclcpp::ok())(void)rclcpp::shutdown();}catch(...){}return 1;}
}
