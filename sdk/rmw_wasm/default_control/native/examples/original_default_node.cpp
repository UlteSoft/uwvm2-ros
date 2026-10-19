// SPDX-License-Identifier: Apache-2.0
// Complete explicit bootstrap; no unresolved extern factory, weak raw imports,
// fake native IDs or quiet NodeOptions. This SOURCE requires the genuine fixed
// original C-aware Service ownership port before it is qualified or executed.
// Actual Node/GraphListener/signal/thread Host composition is independently
// required. Existing project-hook FATAL is deliberately not changed here.
#include <uwvm_rmw/default_control.h>
#include <uwvm_rmw/codec_composer.h>
#include <uwvm_default_control_identity_input.hpp>
#include <uwvm_default_control_deployment.hpp>
#include <uwvm_rmw/bridge.h>
#include <uwvm_rmw/threaded.h>
#include <uwvm_rmw_c/provider.h>
#include <uwvm_rmw/publisher_transport.h>
#include <uwvm_rmw/service_transport.h>
#include <uwvm_rmw/subscription_transport.h>
#include "../detail/decimal.hpp"
#include <uwvm_rmw/wasi_wait.h>
#include <uwvm_rmw/wasi_signal_bootstrap.hpp>
#include <uwvm_rmw_platform.h>
#include <roswasm/ros2.h>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/service.hpp>
#include <rclcpp/any_service_callback.hpp>
#if !defined(UWVM_ORIGINAL_RCLCPP_COMPOSITE) || UWVM_ORIGINAL_RCLCPP_COMPOSITE != 1
#error "Actual source-rebuilt original rclcpp composite marker required; this is not a permission grant"
#endif
#if !defined(UWVM_RCLCPP_C_LIFECYCLE_PORT) || UWVM_RCLCPP_C_LIFECYCLE_PORT != 1 || !defined(RCLCPP_UWVM_SERVICE_C_LIFECYCLE_HEADER_VERSION) || RCLCPP_UWVM_SERVICE_C_LIFECYCLE_HEADER_VERSION != 1 || !defined(RCLCPP_UWVM_CALLBACK_C_LIFECYCLE_HEADER_VERSION) || RCLCPP_UWVM_CALLBACK_C_LIFECYCLE_HEADER_VERSION != 1
#error "Genuine actual source-recompiled original C Service ownership port required"
#endif
#include <rcl/context.h>
#include <rcl/service.h>
#include <fast_io.h>
#include <array>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <exception>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
namespace {
namespace input=uwvm_default_control_deployment;
struct SourceState {
 uwvm_default_control_registry*registry{};uwvm_rmw_codec_composer*composer{};
 uwvm_rmw_codec const*codecs{};std::size_t codec_count{};
 uwvm_rmw_profile b{};uwvm_rmw_c_profile c{};
 uwvm_rmw_node_selection node{"default_control_node","/"};std::array<uwvm_rmw_endpoint_selection,10>endpoints{};
 uwvm_rmw_subscription_transport*subscriptions{};
 uwvm_rmw_publisher_transport*publishers{};uwvm_rmw_service_transport*services{};uwvm_rmw_wasi_wait*wait{};
 uwvm_rmw_platform_profile platform{};uwvm_rmw_platform_diagnostics platform_report{};
 uwvm_rmw_diagnostics ownership{};uwvm_rmw_threaded_diagnostics threaded{};
 std::array<uwvm_rmw_service_transport_diagnostic,7>service_reports{};
 std::array<uwvm_rmw_publisher_transport_diagnostic,2>publisher_reports{};
 std::array<uwvm_rmw_subscription_transport_diagnostic,1>subscription_reports{};
 uwvm_rmw_wasi_wait_diagnostics wait_report{};
 uwvm_rmw::wasi_signal::bootstrap_report signal_report{};
 std::exception_ptr original_primary{};std::uint32_t primary{},cleanup{};
 bool platform_prepared{},platform_retained{},b_installed{},b_uninstall_attempted{},context_retired{};
};
// Guest Source-local sticky owner, not a Host process singleton. Exported C
// vtables have no uninstall: never free this storage while a future Context
// could still borrow it. Physical Source/Group retirement reclaims its memory.
SourceState*source{};
void require(bool okay){if(!okay)throw std::runtime_error("actual default Node prerequisite failed");}
std::size_t find(char const*name){for(std::size_t i{};i<source->codec_count;++i)if(!std::strcmp(source->codecs[i].native_type,name))return i;throw std::runtime_error("complete default codec missing");}
void raw(std::uint32_t code){if(code){if(!source->primary)source->primary=code;throw std::runtime_error("explicit raw bootstrap failed");}}
bool number(char const*value,std::uint64_t&out){return value&&uwvm_default_control_detail::decimal(value,out);}
void save_ownership(rclcpp::Context::SharedPtr const&context){
 auto original=context->get_rcl_context();if(!original)return;
 auto rmw=rcl_context_get_rmw_context(original.get());if(!rmw){++source->cleanup;return;}
 if(uwvm_rmw_get_diagnostics(rmw,&source->ownership)!=RMW_RET_OK)++source->cleanup;
 source->cleanup+=static_cast<std::uint32_t>(source->ownership.owner_close_failures+source->ownership.native_retire_failures+source->ownership.local_retirement_failures);
 for(std::size_t i{};i<UWVM_RMW_MAX_NODES;++i)if(source->ownership.graph_close_attempted[i]&&source->ownership.graph_close_codes[i])++source->cleanup;
 if(uwvm_rmw_get_threaded_diagnostics(rmw,&source->threaded)!=RMW_RET_OK)++source->cleanup;
}
int original_application(int argc,char**argv){
 rclcpp::Context::SharedPtr context;std::shared_ptr<rclcpp::Node>node;
 std::unique_ptr<rclcpp::executors::SingleThreadedExecutor>executor;bool added=false;
 try {
 context=std::make_shared<rclcpp::Context>();rclcpp::InitOptions init;init.set_domain_id(input::domain_id);
 char const*args[]{argv[0]};context->init(1,args,init);
 (void)rclcpp::install_signal_handlers(); // actual default All, not None
 rclcpp::NodeOptions options;options.context(context);
 require(options.enable_rosout()&&options.start_parameter_services()&&options.start_parameter_event_publisher()&&options.use_clock_thread());
 node=std::make_shared<rclcpp::Node>("default_control_node",options);
 require(node->get_parameter("start_type_description_service").as_bool());
 // This original operation emits the actual complete default ParameterEvent.
 // It does not replace six original service handlers or make new permissions.
 node->declare_parameter<std::string>("source_value","native-\xC2\xB5");
 RCLCPP_INFO(node->get_logger(),"Original default Node retains rosout and all default control endpoints");
 rclcpp::ExecutorOptions execution;execution.context=context;
 executor=std::make_unique<rclcpp::executors::SingleThreadedExecutor>(execution);executor->add_node(node);added=true;
 std::int64_t start{};raw(uwvm_rmw_platform_now(UWVM_RMW_STEADY_CLOCK,&start));require(start>=0&&start<=std::numeric_limits<std::int64_t>::max()-INT64_C(15000000000));
 auto until=start+INT64_C(15000000000);auto previous=start;
 for(std::size_t i{};i<120&&context->is_valid();++i){
 executor->spin_once(std::chrono::milliseconds(100)); // real RMW wait/provider
 std::int64_t now{};raw(uwvm_rmw_platform_now(UWVM_RMW_STEADY_CLOCK,&now));require(now>=previous&&now<until);previous=now;
 }
 require(context->is_valid());
 }catch(...){source->original_primary=std::current_exception();if(!source->primary)source->primary=5;}
 // Retain the first primary; every separately attempted cleanup is observable.
 if(executor&&added){try{executor->remove_node(node);}catch(...){++source->cleanup;}}
 executor.reset();node.reset();
 if(context){try{if(context->is_valid())(void)context->shutdown("bounded original default Node completed");}catch(...){++source->cleanup;}
 try{save_ownership(context);}catch(...){++source->cleanup;}context.reset();}
 // B checks actual live options/contexts; no count is called physical joins.
 if(source->b_installed){source->b_uninstall_attempted=true;if(uwvm_rmw_install_profile(nullptr)==RMW_RET_OK){source->b_installed=false;source->context_retired=true;}else ++source->cleanup;}
 (void)argc;return static_cast<int>(source->primary);
}
void prepare(){
 auto const&ids=uwvm_default_control_input::identities;raw(uwvm_default_control_create(ids.data(),ids.size(),&source->registry));
 uwvm_rmw_codec const*original{};std::size_t original_count{};raw(uwvm_default_control_export(source->registry,&original,&original_count));
 uwvm_rmw_codec_table_view table{original,original_count};raw(uwvm_rmw_codec_composer_create(&table,1,&source->composer));
 raw(uwvm_rmw_codec_composer_export(source->composer,&source->codecs,&source->codec_count));
 rclcpp::NodeOptions defaults;auto rosout=defaults.rosout_qos().get_rmw_qos_profile();auto events=defaults.parameter_event_qos().get_rmw_qos_profile();
 auto parameter_subscription=rclcpp::ParameterEventsQoS().get_rmw_qos_profile();
 auto parameters=rclcpp::ParametersQoS().get_rmw_qos_profile();auto description=rcl_service_get_default_options().qos;
 constexpr char const*types[]{"rcl_interfaces/msg/Log","rcl_interfaces/msg/ParameterEvent","rcl_interfaces/srv/DescribeParameters","rcl_interfaces/srv/GetParameters","rcl_interfaces/srv/GetParameterTypes","rcl_interfaces/srv/ListParameters","rcl_interfaces/srv/SetParameters","rcl_interfaces/srv/SetParametersAtomically","type_description_interfaces/srv/GetTypeDescription"};
 constexpr char const*names[]{"/rosout","/parameter_events","/default_control_node/describe_parameters","/default_control_node/get_parameters","/default_control_node/get_parameter_types","/default_control_node/list_parameters","/default_control_node/set_parameters","/default_control_node/set_parameters_atomically","/default_control_node/get_type_description"};
 std::size_t request_max{},response_max{};
 for(std::size_t i{};i<9;++i){auto&e=source->endpoints[i];e.node_index=0;e.codec_index=find(types[i]);e.kind=i<2?UWVM_RMW_PUBLISHER:UWVM_RMW_SERVICE;e.resolved_name=names[i];
 e.requested_qos=i==0?rosout:i==1?events:i==8?description:parameters;e.effective_qos=input::endpoints[i].effective_qos;
 e.alias_count=i<2?1:2;for(std::size_t j{};j<e.alias_count;++j)e.aliases[j]=input::endpoints[i].aliases[j];
 if(i>=2){auto const&c=source->codecs[e.codec_index];request_max=std::max(request_max,c.request->maximum_wire_bytes);response_max=std::max(response_max,c.response->maximum_wire_bytes);}}
 // Original TimeSource attaches this subscription UNCONDITIONALLY, even
 // when use_sim_time stays false. It is a distinct granted owner on the same
 // canonical ParameterEvent type, never a replacement for its publisher.
 auto&sub=source->endpoints[9];sub.node_index=0;sub.codec_index=find("rcl_interfaces/msg/ParameterEvent");sub.kind=UWVM_RMW_SUBSCRIPTION;
 sub.resolved_name="/parameter_events";sub.requested_qos=parameter_subscription;sub.effective_qos=input::endpoints[9].effective_qos;
 sub.alias_count=1;sub.aliases[0]=input::endpoints[9].aliases[0];
 rcutils_allocator_t allocator=rcutils_get_default_allocator();
 raw(uwvm_rmw_publisher_transport_create(2,&allocator,&source->publishers));raw(uwvm_rmw_publisher_transport_callbacks(source->publishers,&source->b.publisher));
 raw(uwvm_rmw_service_transport_create(7,16,request_max,response_max,&allocator,&source->services));
 raw(uwvm_rmw_subscription_transport_create(1,source->codecs[sub.codec_index].maximum_wire_bytes,&allocator,&source->subscriptions));
 uwvm_rmw_c_activation_provider subscription_activation{};uwvm_rmw_c_native_metadata_provider subscription_metadata{};
 raw(uwvm_rmw_subscription_transport_callbacks(source->subscriptions,&subscription_activation,&subscription_metadata));
 // Both genuine states remain in SourceState for the complete published C
 // singleton lifetime. Typed composition copies, never replaces, the real
 // subscription activation/metadata and service request/response providers.
 raw(uwvm_rmw_service_transport_compose_subscription(source->services,&subscription_activation,&subscription_metadata,
  &source->c.activation,&source->c.requests,&source->c.metadata));
 raw(uwvm_rmw_wasi_wait_create(UINT64_C(60000000000),UINT64_C(100000000),4096,&allocator,&source->wait));raw(uwvm_rmw_wasi_wait_callbacks(source->wait,&source->c.wait));source->c.version=1;
 source->b.version=1;source->b.domain_id=input::domain_id;source->b.backend=roswasmc_ros2_backend();source->b.nodes=&source->node;source->b.node_count=1;
 source->b.codecs=source->codecs;source->b.codec_count=source->codec_count;source->b.endpoints=source->endpoints.data();source->b.endpoint_count=source->endpoints.size();source->b.graph=uwvm_rmw_c_graph_provider();
 require(uwvm_rmw_install_profile(&source->b)==RMW_RET_OK);source->b_installed=true;require(uwvm_rmw_c_install_profile(&source->c)==RMW_RET_OK);
 source->platform={UWVM_RMW_PLATFORM_VERSION,source->b.backend,input::system_clock_alias,input::steady_clock_alias,input::loggers.data(),input::loggers.size()};
 raw(uwvm_rmw_platform_prepare(&source->platform));source->platform_prepared=true;raw(uwvm_rmw_platform_retain());source->platform_retained=true;
}
}
int main(int argc,char**argv){
 // FD/cookie are actual Root-owned source/channel facts, not generated IDs.
 std::uint64_t fd{},cookie{};if(argc!=3||!number(argv[1],fd)||fd<3||fd>std::uint64_t(std::numeric_limits<int>::max())||!number(argv[2],cookie)||!cookie)return 22;
 try{source=new SourceState;prepare();uwvm_rmw::wasi_signal::profile channel{uwvm_rmw::wasi_signal::profile_version,static_cast<std::uint32_t>(fd),cookie,uwvm_rmw::wasi_signal::maximum_frame_count,uwvm_rmw::wasi_signal::maximum_slice_ns};
 auto result=uwvm_rmw::wasi_signal::run_original(channel,&original_application,argc,argv,source->signal_report);if(result&&!source->primary)source->primary=static_cast<std::uint32_t>(result);
 }catch(...){if(!source)return 12;if(!source->primary)source->primary=5;}
 if(source->b_installed&&!source->b_uninstall_attempted){source->b_uninstall_attempted=true;if(uwvm_rmw_install_profile(nullptr)==RMW_RET_OK){source->b_installed=false;source->context_retired=true;}else ++source->cleanup;}
 if(source->publishers){std::size_t n{};if(uwvm_rmw_publisher_transport_count(source->publishers,&n)||n>2)++source->cleanup;else for(std::size_t i{};i<n;++i)if(uwvm_rmw_publisher_transport_diagnostics(source->publishers,i,&source->publisher_reports[i]))++source->cleanup;}
 if(source->services){std::size_t n{};if(uwvm_rmw_service_transport_count(source->services,&n)||n>7)++source->cleanup;else for(std::size_t i{};i<n;++i)if(uwvm_rmw_service_transport_diagnostics(source->services,i,&source->service_reports[i]))++source->cleanup;}
 if(source->subscriptions){std::size_t n{};if(uwvm_rmw_subscription_transport_count(source->subscriptions,&n)||n>1)++source->cleanup;else for(std::size_t i{};i<n;++i)if(uwvm_rmw_subscription_transport_diagnostics(source->subscriptions,i,&source->subscription_reports[i]))++source->cleanup;}
 if(source->wait){if(uwvm_rmw_wasi_wait_observe(source->wait,&source->wait_report))++source->cleanup;else if(source->wait_report.first_error)++source->cleanup;}
 // Join the actual receiver before releasing the platform's original-context
 // lease. Failed join/quiescence keeps retained state, never an unsafe free.
 if(source->platform_retained&&source->context_retired&&!source->signal_report.receiver_join&&!source->signal_report.uninstall_threw){if(uwvm_rmw_platform_release())++source->cleanup;else source->platform_retained=false;}
 if(source->platform_prepared&&!source->platform_retained&&uwvm_rmw_platform_close())++source->cleanup;
 if(uwvm_rmw_platform_get_diagnostics(&source->platform_report))++source->cleanup;
 fast_io::print("default_node primary=",source->primary," cleanup=",source->cleanup," native_retire_attempts=",source->ownership.native_retire_attempts," raw_close_attempts=",source->ownership.owner_close_attempts,"\n");
 // This local result is not actual external endpoint/native/Host qualification.
 return source->primary||source->cleanup?1:0;
}
