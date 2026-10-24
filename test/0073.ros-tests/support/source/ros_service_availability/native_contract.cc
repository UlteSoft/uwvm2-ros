/** Real selected-SDK Trigger readiness/side-effect witness, not Guest/Host proof.
 * Root must supply an unused canonical ROS service name and actual configured
 * native environment. No SDK/version/identity/native success is forecast here.
 * Synchronous query APIs are checked against the absolute helper deadline;
 * those checks cannot preempt a stuck RMW call. Root must independently
 * bound this trusted fixture process and preserve native SDK teardown facts.
 * Unsupported real identity/QoS/quiet-Node APIs explicitly refuse95.
 */
#include "../../../../../src/ros/gateway/common/config.h"
#include <atomic>
#include <cstdlib>
#include <future>
#include <thread>
#if UWVM_ROS_BACKEND == 1
#include "../../../../../src/ros/gateway/ros1/service_availability.h"
#include <std_srvs/Trigger.h>
#elif UWVM_ROS_BACKEND == 2
#include "../../../../../src/ros/gateway/ros2/service_availability.h"
#include "../../../../../src/ros/gateway/ros2/compat/identity.h"
#include <std_srvs/srv/trigger.hpp>
#else
#error "Select real backend1 or2"
#endif
namespace gw=uwvm2::ros_gateway;namespace core=uwvm2::imported::ros;
namespace wire=core::ipc::client_availability;
static void require(bool c){if(!c){std::abort();}}
static core::capability_spec source(std::string const& resource)
{
    core::capability_spec c;c.alias="call";c.operation="service.call";c.resource=resource;
#if UWVM_ROS_BACKEND == 1
    c.type=::ros::service_traits::DataType<std_srvs::Trigger>::value();
    c.type_identity=::ros::service_traits::MD5Sum<std_srvs::Trigger>::value();
#else
    c.type="std_srvs/srv/Trigger";
    // This is the trusted expected catalog selector. The REAL installed Native
    // type support and complete identity check below must separately agree.
    c.type_identity=gw::ros2_compat::builtin_catalog_identity(c.type);
#endif
    c.limits={{"timeout-ms","1000"},{"max-inflight","1"},{"max-request-bytes","1"},{"max-response-bytes","128"},
      {"max-rate-hz","1000"},{"burst","100"},{"max-bytes-per-second","65536"},{"validator","none"},{"audit","deny-only"},
      {"audit-max-rate-hz","1000"},{"audit-burst","1000"},{"audit-max-records","1000"},{"audit-max-bytes","1048576"}};
#if UWVM_ROS_BACKEND == 2
    c.limits.insert({{"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.history","keep-last"},{"qos.depth","4"},
      {"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}});
#endif
    return c;
}
#if UWVM_ROS_BACKEND == 1
// Real installed XmlRpc parser and owned socket boundary contracts. These
// finite adversarial DATA responses do not stand in for the later real Master,
// server, header handshake or callback counter witness.
static void ros1_parser_boundaries()
{
    namespace probe=gw::ros1_service_availability;
    auto response=[](int status,std::string const& message,std::string const& address)
    {
        XmlRpc::XmlRpcValue value;value[0]=status;value[1]=message;value[2]=address;
        return std::string{"<?xml version=\"1.0\"?><methodResponse><params><param>"}+value.toXml()+"</param></params></methodResponse>";
    };
    bool found{true};std::string uri{"unchanged"};
    require(probe::lookup_result(response(-1,"no provider",""),found,uri)==0&&!found&&uri.empty());
    require(probe::lookup_result(response(0,"Internal failure: fixture",""),found,uri)==5&&!found&&uri.empty());
    require(probe::lookup_result(response(-1,"invalid parameter",""),found,uri)==22&&!found&&uri.empty());
    require(probe::lookup_result(response(1,"rosrpc URI","rosrpc://127.0.0.1:12345"),found,uri)==0&&found&&uri=="rosrpc://127.0.0.1:12345");
    require(probe::lookup_result(response(1,"invalid empty success",""),found,uri)==22&&!found&&uri.empty());
    require(probe::lookup_result(response(2,"unknown status",""),found,uri)==22&&!found&&uri.empty());
    std::string fault{"<methodResponse><fault><value><struct><member><name>faultCode</name><value><int>1</int></value></member></struct></value></fault></methodResponse>"};
    require(probe::lookup_result(fault,found,uri)==22&&!found&&uri.empty());
    require(probe::lookup_result("<!DOCTYPE x [<!ENTITY y SYSTEM 'file:///forbidden'>]>"+response(1,"ok","x"),found,uri)==22);
    require(probe::lookup_result(response(1,"ok","x")+"trailing",found,uri)==22);
    auto socket_case=[](std::string const& header,bool close_peer,bool expected,int expected_error)
    {
        int pair[2]{-1,-1};require(::socketpair(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK,0,pair)==0);
        gw::native_io::owned_fd input{pair[0]},output{pair[1]};
        if(!header.empty()){require(::send(output.get(),header.data(),header.size(),MSG_NOSIGNAL)==static_cast<ssize_t>(header.size()));}
        if(close_peer){require(::shutdown(output.get(),SHUT_WR)==0);}
        auto until{core::ipc::after(5)};std::string received;std::size_t start{},length{};errno=0;
        auto ok{probe::receive_header(input.get(),until,received,start,length)};require(ok==expected);
        if(!ok){require(errno==expected_error);}else{require(length==1&&start<=received.size());}
    };
    socket_case("HTTP/1.0 200 OK\r\nContent-Length: 1\r\n\r\nx",true,true,0);
    socket_case("HTTP/1.0 200 OK\r\nContent-Length: 1\r\n",true,false,ECONNRESET);
    socket_case("HTTP/1.0 200 OK\r\nContent-Length: 1\r\nContent-Length: 1\r\n\r\nx",true,false,EPROTO);
    socket_case("HTTP/1.0 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n",true,false,EPROTO);
    socket_case("HTTP/1.0 500 Fault\r\nContent-Length: 1\r\n\r\nx",true,false,EPROTO);
    socket_case("",false,false,ETIMEDOUT);
    // A complete header followed by a short body must also fail the SAME
    // production transfer primitive; no partial XML may reach fromXml.
    int pair[2]{-1,-1};require(::socketpair(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK,0,pair)==0);
    gw::native_io::owned_fd input{pair[0]},output{pair[1]};std::string header{"HTTP/1.0 200 OK\r\nContent-Length: 4\r\n\r\nx"};
    require(::send(output.get(),header.data(),header.size(),MSG_NOSIGNAL)==static_cast<ssize_t>(header.size()));
    require(::shutdown(output.get(),SHUT_WR)==0);std::string received;std::size_t start{},length{};auto until{core::ipc::after(5)};
    require(probe::receive_header(input.get(),until,received,start,length)&&length==4&&received.size()-start==1);
    char missing[3]{};require(!core::ipc::transfer(input.get(),missing,sizeof missing,false,until));
}
#endif
int main(int argc,char** argv)
{
    if(argc!=2||!gw::canonical(argv[1]))return 22;
    auto called{source(argv[1])},available{called};available.alias="available";available.operation="service.available";available.limits.erase("validator");
    available.limits["service-source-alias"]="call";available.limits["max-request-bytes"]="64";available.limits["max-response-bytes"]="64";
    available.limits["max-wait-ns"]="100000000";available.limits["max-native-clients"]="1";available.limits["max-binding-history"]="8";
    gw::validate(static_cast<core::version>(UWVM_ROS_BACKEND),called);gw::validate(static_cast<core::version>(UWVM_ROS_BACKEND),available);
    // Synthetic association ONLY for direct native helper invocation. True
    // Host handles/runtime generation are tested separately, never invented.
    wire::context association{UWVM_ROS_BACKEND,(std::uint64_t{1}<<32)|1,(std::uint64_t{1}<<32)|2,1,1};
    wire::packet request{wire::command::query,wire::retirement::none,0,association,0};
    std::atomic<unsigned> callbacks{};
#if UWVM_ROS_BACKEND == 1
    ros1_parser_boundaries();
    ::ros::M_string remapping;::ros::init(remapping,"uwvm_availability_native_contract",::ros::init_options::NoSigintHandler|::ros::init_options::NoRosout);
    ::ros::NodeHandle node;::ros::AsyncSpinner spinner{1};spinner.start();
    struct cleanup{::ros::AsyncSpinner& spinner;~cleanup(){spinner.stop();::ros::shutdown();}} stop{spinner};
    auto qualify=[](core::capability_spec const& cap){return cap.type==::ros::service_traits::DataType<std_srvs::Trigger>::value()&&cap.type_identity==::ros::service_traits::MD5Sum<std_srvs::Trigger>::value();};
    auto observe=[&](){return gw::ros1_service_availability::observe<std_srvs::Trigger>(available,called,request,1000,qualify);};
    auto feature{request};feature.operation=wire::command::features;
    auto facts{gw::ros1_service_availability::observe<std_srvs::Trigger>(available,called,feature,1000,qualify)};
    if(facts.error)return static_cast<int>(facts.error);require(facts.value==11);
    auto absent{observe()};require(!absent.error&&!absent.value&&callbacks==0);
    boost::function<bool(std_srvs::Trigger::Request&,std_srvs::Trigger::Response&)> callback{[&](auto&,auto& response){++callbacks;response.success=true;response.message="availability-native-control";return true;}};
    auto server{node.advertiseService(called.resource,callback)};require(bool(server));
#else
#if !UWVM_ROS2_HAVE_QUIET_NODE
    return 95;
#else
    if constexpr(!gw::ros2_service_availability::supports_query<rclcpp::Client<std_srvs::srv::Trigger>>::value||
                 !gw::ros2_qos::supports_client_qos<rclcpp::Client<std_srvs::srv::Trigger>>::value||
                 !gw::ros2_qos::supports_service_qos<rclcpp::Service<std_srvs::srv::Trigger>>::value)return 95;
    if(!gw::ros2_compat::builtin_service<std_srvs::srv::Trigger>(called.type,called.type_identity))return 95;
    rclcpp::init(argc,argv);rclcpp::NodeOptions options;options.enable_rosout(false);options.start_parameter_services(false);options.start_parameter_event_publisher(false);
    options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
    auto node{std::make_shared<rclcpp::Node>("uwvm_availability_native_contract","/",options)};
    rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
    std::thread spinner{[&]{executor.spin();}};
    struct cleanup{rclcpp::executors::SingleThreadedExecutor& executor;std::thread& spinner;~cleanup(){executor.cancel();if(spinner.joinable())spinner.join();rclcpp::shutdown();}} stop{executor,spinner};
    gw::ros2_service_availability::registry registry;
    auto qualify=[](core::capability_spec const& cap){return gw::ros2_compat::builtin_service<std_srvs::srv::Trigger>(cap.type,cap.type_identity);};
    auto observe=[&](){return registry.observe<std_srvs::srv::Trigger>(node,available,called,request,1000,qualify);};
    auto feature{request};feature.operation=wire::command::features;
    auto facts{registry.observe<std_srvs::srv::Trigger>(node,available,called,feature,1000,qualify)};
    if(facts.error)return static_cast<int>(facts.error);require(facts.value==7);
    auto absent{observe()};require(!absent.error&&!absent.value&&callbacks==0);
    auto callback=[&](std::shared_ptr<std_srvs::srv::Trigger::Request>,std::shared_ptr<std_srvs::srv::Trigger::Response> response)
    {++callbacks;response->success=true;response->message="availability-native-control";};
    auto configured{gw::ros2_qos::configured(called)};
    auto server{gw::ros2_compat::create_service<std_srvs::srv::Trigger>(node,called.resource,callback,configured)};
    if(!server||!gw::ros2_qos::service(*server,configured))return 95;
#endif
#endif
#if UWVM_ROS_BACKEND == 1 || UWVM_ROS2_HAVE_QUIET_NODE
    // Actual discovery has a separate bounded fixture deadline; every query is
    // independently finite. An unused resource is REQUIRED, never assumed.
    auto discover=std::chrono::steady_clock::now()+std::chrono::seconds(10);bool matched{};
    while(std::chrono::steady_clock::now()<discover)
    {auto observed{observe()};if(observed.error)return static_cast<int>(observed.error);if(observed.value){matched=true;break;}std::this_thread::sleep_for(std::chrono::milliseconds(20));}
    require(matched&&callbacks==0);
    for(unsigned i{};i!=3;++i){auto observed{observe()};require(!observed.error&&observed.value&&callbacks==0);}
    // Explicit trusted NATIVE positive control, distinct from the metadata-only
    // path. Proves this real server/executor could process a business request.
#if UWVM_ROS_BACKEND == 1
    std_srvs::Trigger operation;auto error{gw::ros1_service::call(called,operation,1000,[](std::vector<std::uint8_t> const& bytes)
    {return bytes.size()>=5&&bytes[0]<=1&&core::ipc::get(bytes.data()+1,4)==bytes.size()-5;})};
    require(!error&&operation.response.success&&operation.response.message=="availability-native-control");
#else
    auto control{gw::ros2_compat::create_client<std_srvs::srv::Trigger>(node,called.resource,configured)};
    require(control&&gw::ros2_qos::client(*control,configured));
    auto pending{control->async_send_request(std::make_shared<std_srvs::srv::Trigger::Request>())};
    struct pending_cleanup{decltype(control)& client;decltype(pending)& future;~pending_cleanup(){try{gw::ros2_compat::remove_pending(*client,future);}catch(...){}}} pending_owner{control,pending};
    require(pending.wait_for(std::chrono::seconds(2))==std::future_status::ready);auto response{pending.get()};
    require(response&&response->success&&response->message=="availability-native-control");
#endif
    require(callbacks==1);auto after{observe()};require(!after.error&&after.value&&callbacks==1);
#if UWVM_ROS_BACKEND == 1
    server.shutdown();
#else
    server.reset();gw::ros2_compat::notify(node);
#endif
    auto removal{std::chrono::steady_clock::now()+std::chrono::seconds(10)};bool removed{};
    while(std::chrono::steady_clock::now()<removal)
    {auto observed{observe()};if(observed.error)return static_cast<int>(observed.error);if(!observed.value){removed=true;break;}std::this_thread::sleep_for(std::chrono::milliseconds(20));}
    require(removed&&callbacks==1);
#if UWVM_ROS_BACKEND == 2
    registry.retire(association,wire::retirement::source_handle,available.alias,called.alias);gw::ros2_compat::notify(node);
#endif
    static constexpr char marker[]{"PASS_NATIVE_TRIGGER_AVAILABILITY_WITHOUT_REQUEST_PLUS_ONE_EXPLICIT_NATIVE_CONTROL\n"};
    require(::write(1,marker,sizeof(marker)-1)==static_cast<ssize_t>(sizeof(marker)-1));return 0;
#endif
}
