// SPDX-License-Identifier: Apache-2.0
// Actual independently owned original Native peer, never a synthetic Host or
// generated-ID oracle. Default Native Node options remain enabled here too.
#include <rclcpp/rclcpp.hpp>
#include <rcl_interfaces/msg/log.hpp>
#include <rcl_interfaces/msg/parameter_event.hpp>
#include <rcl_interfaces/msg/parameter_type.hpp>
#include <rcl_interfaces/srv/describe_parameters.hpp>
#include <rcl_interfaces/srv/get_parameters.hpp>
#include <rcl_interfaces/srv/get_parameter_types.hpp>
#include <rcl_interfaces/srv/list_parameters.hpp>
#include <rcl_interfaces/srv/set_parameters.hpp>
#include <rcl_interfaces/srv/set_parameters_atomically.hpp>
#include <type_description_interfaces/srv/get_type_description.hpp>
#include <rosidl_typesupport_cpp/message_type_support.hpp>
#include <rosidl_runtime_c/type_description/type_description__struct.h>
#include <fast_io.h>
#include <algorithm>
#include <array>
#include "../../../../../../../../../sdk/origin/rmw_wasm/default_control/native/detail/decimal.hpp"
#include <chrono>
#include <cstring>
#include <exception>
#include <rosidl_typesupport_cpp/service_type_support.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
namespace {
using Clock=std::chrono::steady_clock;
void require(bool value){if(!value)throw std::runtime_error("complete original native peer assertion failed");}
template<class Service>typename Service::Response::SharedPtr call(rclcpp::Node::SharedPtr const&node,
 rclcpp::executors::SingleThreadedExecutor&executor,char const*name,typename Service::Request::SharedPtr const&request,Clock::time_point until,rclcpp::QoS const&qos=rclcpp::ParametersQoS()){
 auto client=node->create_client<Service>(name,qos);
 while(!client->service_is_ready()){require(rclcpp::ok()&&Clock::now()<until);auto remaining=until-Clock::now();require(remaining>Clock::duration::zero());(void)client->wait_for_service(std::min(remaining,std::chrono::duration_cast<Clock::duration>(std::chrono::milliseconds(100))));}
 // Even a ready service is not permission to send after this peer's deadline.
 require(rclcpp::ok()&&Clock::now()<until);auto pending=client->async_send_request(request);
 try{auto remaining=until-Clock::now();require(remaining>Clock::duration::zero());
 auto result=executor.spin_until_future_complete(pending.future,remaining);
 if(result!=rclcpp::FutureReturnCode::SUCCESS)throw std::runtime_error("actual native service incomplete; no replay");
 return pending.future.get();
 }catch(...){auto primary=std::current_exception();try{(void)client->remove_pending_request(pending.request_id);}catch(...){fast_io::perrln("Native local pending-bookkeeping cleanup failed");}std::rethrow_exception(primary);}

}
bool text(rosidl_runtime_c__String const&a,std::string const&b){return a.size==b.size()&&a.capacity>=a.size&&(a.data||!a.size)&&(!a.size||!std::memcmp(a.data,b.data(),a.size));}
template<class Individual>bool individual(rosidl_runtime_c__type_description__IndividualTypeDescription const&a,Individual const&b){
 if(!text(a.type_name,b.type_name)||a.fields.size!=b.fields.size())return false;
 for(std::size_t i{};i<a.fields.size;++i){auto const&x=a.fields.data[i];auto const&y=b.fields[i];if(!text(x.name,y.name)||x.type.type_id!=y.type.type_id||x.type.capacity!=y.type.capacity||x.type.string_capacity!=y.type.string_capacity||!text(x.type.nested_type_name,y.type.nested_type_name)||!text(x.default_value,y.default_value))return false;}
 return true;
}
}
int main(int argc,char**argv){
 if(argc!=2)return 22;std::uint64_t domain{};if(!uwvm_default_control_detail::decimal(argv[1],domain)||domain>232)return 22;
 int status=1;try {
 rclcpp::InitOptions options;options.set_domain_id(domain);char const*args[]{argv[0]};rclcpp::init(1,args,options);
 auto node=std::make_shared<rclcpp::Node>("default_control_native_peer");rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
 bool log_seen=false,event_seen=false;
 auto logs=node->create_subscription<rcl_interfaces::msg::Log>("/rosout",rclcpp::RosoutQoS(),[&](rcl_interfaces::msg::Log::SharedPtr msg){
 if(msg->name=="default_control_node"){require(msg->level==rcl_interfaces::msg::Log::INFO&&msg->msg=="Original default Node retains rosout and all default control endpoints"&&!msg->file.empty()&&!msg->function.empty()&&msg->line>0&&msg->stamp.nanosec<1000000000);log_seen=true;}});
 auto events=node->create_subscription<rcl_interfaces::msg::ParameterEvent>("/parameter_events",rclcpp::ParameterEventsQoS(),[&](rcl_interfaces::msg::ParameterEvent::SharedPtr msg){
 if(msg->node!="/default_control_node")return;require(msg->stamp.nanosec<1000000000);
 for(auto const&p:msg->changed_parameters)if(p.name=="source_value"){require(p.value.type==rcl_interfaces::msg::ParameterType::PARAMETER_STRING&&(p.value.string_value=="peer-value"||p.value.string_value=="atomic-value")&&p.value.byte_array_value.empty()&&p.value.bool_array_value.empty()&&p.value.integer_array_value.empty()&&p.value.double_array_value.empty()&&p.value.string_array_value.empty());if(p.value.string_value=="peer-value")event_seen=true;}});
 auto until=Clock::now()+std::chrono::seconds(15);
 using Set=rcl_interfaces::srv::SetParameters;auto set=std::make_shared<Set::Request>();rcl_interfaces::msg::Parameter parameter;parameter.name="source_value";parameter.value.type=rcl_interfaces::msg::ParameterType::PARAMETER_STRING;parameter.value.string_value="peer-value";set->parameters={parameter};
 auto set_out=call<Set>(node,executor,"/default_control_node/set_parameters",set,until);require(set_out->results.size()==1&&set_out->results[0].successful&&set_out->results[0].reason.empty());
 using Get=rcl_interfaces::srv::GetParameters;auto get=std::make_shared<Get::Request>();get->names={"source_value","start_type_description_service"};auto got=call<Get>(node,executor,"/default_control_node/get_parameters",get,until);
 require(got->values.size()==2&&got->values[0]==parameter.value&&got->values[1].type==rcl_interfaces::msg::ParameterType::PARAMETER_BOOL&&got->values[1].bool_value);
 using Types=rcl_interfaces::srv::GetParameterTypes;auto types=std::make_shared<Types::Request>();types->names=get->names;auto typed=call<Types>(node,executor,"/default_control_node/get_parameter_types",types,until);
 require(typed->types==std::vector<std::uint8_t>({rcl_interfaces::msg::ParameterType::PARAMETER_STRING,rcl_interfaces::msg::ParameterType::PARAMETER_BOOL}));
 using Describe=rcl_interfaces::srv::DescribeParameters;auto describe=std::make_shared<Describe::Request>();describe->names=get->names;auto descriptions=call<Describe>(node,executor,"/default_control_node/describe_parameters",describe,until);
 require(descriptions->descriptors.size()==2&&descriptions->descriptors[0].name=="source_value"&&!descriptions->descriptors[0].read_only&&!descriptions->descriptors[0].dynamic_typing&&descriptions->descriptors[1].read_only);
 using List=rcl_interfaces::srv::ListParameters;auto list=std::make_shared<List::Request>();list->depth=List::Request::DEPTH_RECURSIVE;auto listed=call<List>(node,executor,"/default_control_node/list_parameters",list,until);
 require(std::find(listed->result.names.begin(),listed->result.names.end(),"source_value")!=listed->result.names.end());
 using Atomic=rcl_interfaces::srv::SetParametersAtomically;auto atomic=std::make_shared<Atomic::Request>();parameter.value.string_value="atomic-value";atomic->parameters={parameter};auto atom=call<Atomic>(node,executor,"/default_control_node/set_parameters_atomically",atomic,until);require(atom->result.successful&&atom->result.reason.empty());
 using Description=type_description_interfaces::srv::GetTypeDescription;auto request=std::make_shared<Description::Request>();request->type_name="type_description_interfaces/srv/GetTypeDescription";require(request->include_type_sources);
 auto support=rosidl_typesupport_cpp::get_service_type_support_handle<Description>();require(support&&support->get_type_hash_func&&support->get_type_description_func);auto hash=support->get_type_hash_func(support);require(hash&&hash->version==1);
 constexpr char digits[]="0123456789abcdef";request->type_hash="RIHS01_";for(auto b:hash->value){request->type_hash.push_back(digits[b>>4]);request->type_hash.push_back(digits[b&15]);}require(request->type_hash.size()==71);
 // This one original service uses genuine default rcl service QoS, which is
 // the same default native ServicesQoS; do not claim ParametersQoS negotiation.
 auto response=call<Description>(node,executor,"/default_control_node/get_type_description",request,until,rclcpp::ServicesQoS());auto expected=support->get_type_description_func(support);require(response->successful&&response->failure_reason.empty()&&expected&&individual(expected->type_description,response->type_description.type_description)&&expected->referenced_type_descriptions.size==response->type_description.referenced_type_descriptions.size());
 for(std::size_t i{};i<expected->referenced_type_descriptions.size;++i){auto const&e=expected->referenced_type_descriptions.data[i];auto found=std::find_if(response->type_description.referenced_type_descriptions.begin(),response->type_description.referenced_type_descriptions.end(),[&](auto const&r){return text(e.type_name,r.type_name);});require(found!=response->type_description.referenced_type_descriptions.end()&&individual(e,*found));}
 auto sources=support->get_type_description_sources_func(support);require(sources&&sources->capacity>=sources->size&&sources->data&&sources->size==12&&response->type_sources.size()==sources->size&&expected->referenced_type_descriptions.size==11);
 for(std::size_t i{};i<sources->size;++i){auto const&e=sources->data[i];auto found=std::find_if(response->type_sources.begin(),response->type_sources.end(),[&](auto const&r){return text(e.type_name,r.type_name);});require(found!=response->type_sources.end()&&text(e.encoding,found->encoding)&&text(e.raw_file_contents,found->raw_file_contents));}
 // The comparison retains each actual counted byte, including a source
 // getter's final NUL when present. Native middleware serialization must prove
 // this exact response too; a false-only request cannot stand in for it.
 while(!log_seen||!event_seen){require(Clock::now()<until);executor.spin_once(std::chrono::milliseconds(100));}
 executor.remove_node(node);events.reset();logs.reset();node.reset();status=0;
 }catch(...){fast_io::perr("Actual original native default-control peer failed\n");}
 try{if(rclcpp::ok())(void)rclcpp::shutdown();}catch(...){status=1;}
 if(!status)fast_io::print("PASS_NATIVE_DEFAULT_CONTROL_PEER_NOT_FULL_HOST_GUEST_QUALIFICATION\n");return status;
}
