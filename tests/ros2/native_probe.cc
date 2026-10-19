#include "../ros_ipc/probe_common.h"
#include "../ros_ipc/service_replay.h"
#include "../ros_ipc/callback_teardown.h"
#include "../ros_ipc/active_witness.h"
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/bool.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <std_srvs/srv/set_bool.hpp>
using namespace ros_probe;
#include "../ros_ingress/cleanup_ros2.h"
#include "../ros_ingress/revoke_ros2.h"
namespace
{
    std::vector<std::uint8_t> encode(std_msgs::msg::String const& m) { std::vector<std::uint8_t> d; ipc::put(d,m.data.size(),4); d.insert(d.end(),m.data.begin(),m.data.end()); return d; }
    std::vector<std::uint8_t> encode(std_msgs::msg::Bool const& m) { return {static_cast<std::uint8_t>(m.data?1:0)}; }
    std::vector<std::uint8_t> encode(geometry_msgs::msg::Twist const& m) { std::vector<std::uint8_t> d; for(double v:{m.linear.x,m.linear.y,m.linear.z,m.angular.x,m.angular.y,m.angular.z}) { std::uint64_t bits{}; std::memcpy(&bits,&v,8); ipc::put(d,bits,8); } return d; }
    std::vector<std::uint8_t> encode(sensor_msgs::msg::Image const& m) { return ipc::encode_image({0,static_cast<std::uint32_t>(m.header.stamp.sec),m.header.stamp.nanosec,m.height,m.width,m.step,m.header.frame_id,m.encoding,m.is_bigendian!=0,m.data}); }
    template<typename M> int witness(std::shared_ptr<rclcpp::Node> const& node,std::string const& mode,std::string const& resource,M const& sample,bool image=false)
    {
        auto expected{encode(sample)};if(image){for(unsigned n{};n!=4;++n){expected[n]=0;}}
        bool counted{mode=="--observe-count2"||mode=="--observe-flood"};std::atomic<bool> received{},wrong{};std::atomic<unsigned> count{};auto profile{rclcpp::QoS{4}.reliable()};if(mode=="--observe-flood"){profile.transient_local();}
        auto subscriber{(mode!="--publish"&&mode!="--absent-subscription")?node->create_subscription<M>(resource,profile,[&](std::shared_ptr<M const> m){auto actual{encode(*m)};if(image&&actual.size()>=4){for(unsigned n{};n!=4;++n){actual[n]=0;}}if(mode=="--absent"||actual!=expected){wrong=true;}else{received=true;if(++count>2&&counted){wrong=true;}}}):typename rclcpp::Subscription<M>::SharedPtr{}};
        auto publisher{(mode=="--publish"||mode=="--absent-subscription")?node->create_publisher<M>(resource,rclcpp::QoS{4}.reliable()):typename rclcpp::Publisher<M>::SharedPtr{}};
        auto end{std::chrono::steady_clock::now()+std::chrono::seconds(counted?5:(mode=="--absent"||mode=="--absent-subscription")?3:12)};
        if(counted){::ros_probe::print_fixture(STDOUT_FILENO,"READY actual ROS2 two-message counter\n");}
        while(std::chrono::steady_clock::now()<end&&(counted||!received)&&!wrong){rclcpp::spin_some(node);if(mode=="--publish"||mode=="--absent-subscription"){publisher->publish(sample);}std::this_thread::sleep_for(std::chrono::milliseconds(50));}
        if(counted){require(count==2&&!wrong,"bounded actual ROS2 publication count expected2 actual="+std::to_string(count.load()));::ros_probe::print_fixture(STDOUT_FILENO,"BOUNDED_NATIVE_PUBLICATIONS native_count=2 excess_count=0\n");}
        if(mode=="--absent-subscription"){require(node->get_subscriptions_info_by_topic(resource).empty(),"denied operation created no actual ROS2 subscriber");}
        if(mode=="--observe"){require(received&&!wrong,"actual native ROS2 exact content");}
        if(mode=="--absent"){require(!wrong,"denied call emitted no DDS message");require(node->get_publishers_info_by_topic(resource).empty(),"denied call created no DDS publisher");}
        ::ros_probe::print_fixture(STDOUT_FILENO,"PASS actual ROS2 native witness ",mode," content + network side effect\n");return 0;
    }
    void flood_queue(std::string const& executable,std::shared_ptr<rclcpp::Node> const& node)
    {
        for(auto const& overflow:{"drop-oldest","drop-newest","reject"})
        {
            fixture bounded{core::version::ros2,executable,[&](gateway::configuration& cfg){auto cap{cfg.policy.capabilities.at("subString")};cfg.policy.capabilities.clear();cfg.policy.connection["node-name"]="uwvm_ros2_queue_probe";cap.limits["max-message-bytes"]="64";cap.limits["max-queue-bytes"]="128";cap.limits["burst"]="2";cap.limits["qos.depth"]="2";cap.limits["overflow-policy"]=overflow;cap.resource="/uwvm_test/event_queue_"+std::to_string(::getpid())+"_"+std::to_string(std::string{overflow}=="drop-oldest"?1:std::string{overflow}=="drop-newest"?2:3);cfg.policy.capabilities.emplace("subString",std::move(cap));}};
            ipc::client client;bounded.connect(client);require(!client.call("topic.subscribe","subString",{},1000).error,"bounded actual event subscription");auto publisher{node->create_publisher<std_msgs::msg::String>(bounded.config.policy.capabilities.at("subString").resource,rclcpp::QoS{2}.reliable())};
            for(unsigned n{};n!=200&&!publisher->get_subscription_count();++n){std::this_thread::sleep_for(std::chrono::milliseconds(10));}
            require(publisher->get_subscription_count(),"actual event flood subscriber discovery");
            std_msgs::msg::String warmup;warmup.data="warmup";ipc::call_result connected{11,{}};
            for(unsigned n{};n!=100&&connected.error;++n){publisher->publish(warmup);std::this_thread::sleep_for(std::chrono::milliseconds(30));connected=client.call("event.poll","subString",{},1000);require(!connected.error||connected.error==11,"actual event readiness handshake");}
            require(!connected.error&&connected.payload==encode(warmup),"actual DDS/TCPROS data path readiness");std::this_thread::sleep_for(std::chrono::milliseconds(200));
            for(unsigned n{};n!=100;++n){auto drained{client.call("event.poll","subString",{},1000)};if(drained.error==11){break;}require(!drained.error&&drained.payload==encode(warmup),"drain only readiness warmup");std::this_thread::sleep_for(std::chrono::milliseconds(10));}
            std_msgs::msg::String oversized;oversized.data.assign(65,'x');for(unsigned n{};n!=2;++n){publisher->publish(oversized);std::this_thread::sleep_for(std::chrono::milliseconds(30));}require(client.call("event.poll","subString",{},1000).error==11,"actual native oversized String dropped before bounded bridge copy/queue");
            for(unsigned n{1};n<=20;++n){std_msgs::msg::String message;message.data="event-"+std::to_string(n);publisher->publish(message);std::this_thread::sleep_for(std::chrono::milliseconds(30));}
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            for(unsigned n{};n!=2;++n){std_msgs::msg::String message;message.data="event-"+std::to_string(std::string{overflow}=="drop-oldest"?19+n:1+n);auto result{client.call("event.poll","subString",{},1000)};require(!result.error&&result.payload==encode(message),"actual native event overflow exact retained frame expected="+message.data+" error="+std::to_string(result.error)+" actual="+(result.payload.size()>=4?std::string{result.payload.begin()+4,result.payload.end()}:"<empty>"));std::this_thread::sleep_for(std::chrono::milliseconds(10));}
            require(client.call("event.poll","subString",{},1000).error==11,"actual event queue bounded to exactly two frames");client.close();
            ::ros_probe::print_fixture(STDOUT_FILENO,"PASS ROS2 actual native20-message event flood depth2/128bytes ",overflow," exact retained frames\n");
        }
    }
    template<typename M> void check(fixture& f,ipc::client& client,std::shared_ptr<rclcpp::Node> const& node,unsigned index,std::string const& name,M const& message)
    {
        auto expected{data(index)}; if(index==3) { for(unsigned n{};n!=4;++n) { expected[n]=0; } } // ROS2 Header has no sequence field.
        std::mutex mutex; std::vector<std::uint8_t> received; unsigned count{};
        auto subscriber{node->create_subscription<M>("/uwvm_test/pub"+name,rclcpp::QoS{8}.reliable(),[&](std::shared_ptr<M const> m) { std::lock_guard<std::mutex> lock{mutex};received=encode(*m);++count; })};
        auto publisher{node->create_publisher<M>("/uwvm_test/sub"+name,rclcpp::QoS{8}.reliable())};
        require(!client.call("topic.subscribe","sub"+name,{},1000).error,"subscribe "+name);
        for(unsigned n{};n!=500;++n)
        {
            require(!client.call("topic.publish","pub"+name,data(index),1000).error,"publish "+name);
            { std::lock_guard<std::mutex> lock{mutex}; if(received==expected) { break; } }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        { std::lock_guard<std::mutex> lock{mutex};require(received==expected,"gateway -> native exact "+name); }
        bool verified{};
        for(unsigned n{};n!=300&&!verified;++n)
        {
            auto info{node->get_publishers_info_by_topic("/uwvm_test/pub"+name)};
            for(auto const& endpoint:info)
            {
                if(endpoint.node_name()=="uwvm_ros2_gateway_probe")
                {
                    auto q{endpoint.qos_profile().get_rmw_qos_profile()};
                    require(q.reliability==RMW_QOS_POLICY_RELIABILITY_RELIABLE&&q.durability==RMW_QOS_POLICY_DURABILITY_VOLATILE,"actual DDS reliability/durability endpoint"); verified=true;
                }
            }
            if(!verified) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
        }
        require(verified,"gateway DDS endpoint discovered");
        ipc::call_result incoming{11,{}};
        for(unsigned n{};n!=500;++n) { publisher->publish(message);std::this_thread::sleep_for(std::chrono::milliseconds(10));incoming=client.call("event.poll","sub"+name,{},1000);if(!incoming.error) { break; } require(incoming.error==11,"poll pending"); }
        require(!incoming.error&&incoming.payload==expected,"native -> gateway exact "+name);
        unsigned before;{std::lock_guard<std::mutex> lock{mutex};before=count;}
        require(client.call("topic.publish","denied",data(index),1000).error==13,"unauthorized denied");std::this_thread::sleep_for(std::chrono::milliseconds(30));
        {std::lock_guard<std::mutex> lock{mutex};require(count==before,"denial no actual DDS publication");}
        require(!client.call("capability.revoke","sub"+name,{},1000).error,"revoke");publisher->publish(message);require(client.call("event.poll","sub"+name,{},1000).error==125,"revoked no receipt");
        require(!client.call("capability.close","pub"+name,{},1000).error,"close");
        ::ros_probe::print_fixture(STDOUT_FILENO,"PASS ROS2 ",name," bidirectional exact message, real DDS discovery/QoS, denial/no side effect, revoke\n");
    }
}
int main(int argc,char** argv)
{
    try
    {
        if(argc==2&&std::string{argv[1]}=="--observe-cleanup"){return ros_trap_cleanup_witness(argc,argv);}
        if(argc==3&&std::string{argv[1]}=="--observe-revoke-race"){return ros_revoke_race_witness(argc,argv);}
        if(argc>=3&&(std::string{argv[1]}=="--serve-trigger"||std::string{argv[1]}=="--serve-setbool"||std::string{argv[1]}=="--absent-service"||std::string{argv[1]}=="--deny-service"))
        {
            std::string mode{argv[1]},resource{argv[2]};rclcpp::init(argc,argv);auto node{std::make_shared<rclcpp::Node>("uwvm_ros2_service_witness")};unsigned count{};bool wrong{};rclcpp::ServiceBase::SharedPtr server;
            if(mode=="--serve-trigger"||mode=="--deny-service"){server=node->create_service<std_srvs::srv::Trigger>(resource,[&](std::shared_ptr<std_srvs::srv::Trigger::Request>,std::shared_ptr<std_srvs::srv::Trigger::Response> response){++count;response->success=true;response->message="hello";});}
            else if(mode=="--serve-setbool"){server=node->create_service<std_srvs::srv::SetBool>(resource,[&](std::shared_ptr<std_srvs::srv::SetBool::Request> request,std::shared_ptr<std_srvs::srv::SetBool::Response> response){++count;if(!request->data){wrong=true;}response->success=true;response->message="hello";});}
            auto end{std::chrono::steady_clock::now()+std::chrono::seconds(mode=="--absent-service"?3:mode=="--deny-service"?4:12)};::ros_probe::print_fixture(STDOUT_FILENO,"READY real ROS2 service witness\n");
            while(std::chrono::steady_clock::now()<end&&!count){rclcpp::spin_some(node);if(mode=="--absent-service"){require(!node->get_service_names_and_types().count(resource),"denied operation registered no actual DDS service");}std::this_thread::sleep_for(std::chrono::milliseconds(50));}
            if(mode=="--deny-service"){require(!count,"unauthorized Wasm service call reached no actual native Trigger callback");::ros_probe::print_fixture(STDOUT_FILENO,"NO_NATIVE_SERVICE_REQUESTS native_trigger_callbacks=0\n");}
            else if(mode!="--absent-service"){require(count==1&&!wrong,"actual Wasm service client exactly one valid typed native request");std::this_thread::sleep_for(std::chrono::milliseconds(500));}
            rclcpp::shutdown();::ros_probe::print_fixture(STDOUT_FILENO,"PASS actual ROS2 native service witness ",mode,"\n");return 0;
        }
        if(argc>=3&&(std::string{argv[1]}=="--call-trigger"||std::string{argv[1]}=="--call-setbool"))
        {
            std::string mode{argv[1]},resource{argv[2]};rclcpp::init(argc,argv);auto node{std::make_shared<rclcpp::Node>("uwvm_ros2_service_witness")};
            if(mode=="--call-trigger") { auto client{node->create_client<std_srvs::srv::Trigger>(resource)};require(client->wait_for_service(std::chrono::seconds(12)),"real Trigger server discovery");auto request{std::make_shared<std_srvs::srv::Trigger::Request>()};auto result{client->async_send_request(request)};require(rclcpp::spin_until_future_complete(node,result,std::chrono::seconds(12))==rclcpp::FutureReturnCode::SUCCESS,"actual Trigger response deadline");auto response{result.get()};require(response->success&&response->message=="hello","actual Wasm Trigger server response"); }
            else { auto client{node->create_client<std_srvs::srv::SetBool>(resource)};require(client->wait_for_service(std::chrono::seconds(12)),"real SetBool server discovery");auto request{std::make_shared<std_srvs::srv::SetBool::Request>()};request->data=true;auto result{client->async_send_request(request)};require(rclcpp::spin_until_future_complete(node,result,std::chrono::seconds(12))==rclcpp::FutureReturnCode::SUCCESS,"actual SetBool response deadline");auto response{result.get()};require(response->success&&response->message=="hello","actual Wasm SetBool server response"); }
            ::ros_probe::print_fixture(STDOUT_FILENO,"PASS real ROS2 Wasm queued service response true/hello\n");rclcpp::shutdown();return 0;
        }
        if((argc==4||argc==5)&&(std::string{argv[1]}=="--observe"||std::string{argv[1]}=="--publish"||std::string{argv[1]}=="--absent"||std::string{argv[1]}=="--absent-subscription"||std::string{argv[1]}=="--observe-count2"||std::string{argv[1]}=="--observe-flood"))
        {
            std::string mode{argv[1]},resource{argv[2]},text{argv[3]},type{argc==5?argv[4]:"String"};rclcpp::init(argc,argv);auto node{std::make_shared<rclcpp::Node>("uwvm_ros2_network_witness")};int result{};
            if(type=="String"){std_msgs::msg::String s;s.data=text;result=witness(node,mode,resource,s);}
            else if(type=="Bool"){std_msgs::msg::Bool m;m.data=true;result=witness(node,mode,resource,m);}
            else if(type=="Twist"){geometry_msgs::msg::Twist m;m.linear.x=1.25;m.linear.y=-2.;m.linear.z=3.5;m.angular.x=.1;m.angular.y=.2;m.angular.z=.3;result=witness(node,mode,resource,m);}
            else if(type=="Image"){sensor_msgs::msg::Image m;m.header.stamp.sec=123;m.header.stamp.nanosec=456;m.header.frame_id="camera_frame";m.height=2;m.width=2;m.encoding="rgb8";m.step=6;m.data={0,1,2,3,4,5,6,7,8,9,10,11};result=witness(node,mode,resource,m,true);}
            else{throw std::runtime_error("unsupported witness type");}rclcpp::shutdown();return result;
        }
        require(argc==2,"usage native-probe gateway");::setenv("ROS_DOMAIN_ID","72",1);::setenv("RMW_IMPLEMENTATION","rmw_fastrtps_cpp",1);::setenv("ROS_SECURITY_ENABLE","false",1);
        fixture f{uwvm2::imported::ros::version::ros2,argv[1],[](gateway::configuration& cfg){cfg.policy.capabilities.at("service").limits["qos.depth"]="6";}};ipc::client client;f.connect(client);
        rclcpp::init(argc,argv);auto node{std::make_shared<rclcpp::Node>("uwvm_native_ros2_probe")};rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);std::thread spinner{[&]{executor.spin();}};
        try
        {
            {
                active_ipc_witness witness{[&] {for(auto const& topic:{"/uwvm_test/pubString","/uwvm_test/subString","/uwvm_test/pubTwist","/unauthorized"}) {if(!node->get_publishers_info_by_topic(topic).empty()) {return false;}}return true;}};
                auto pub_string{node->create_subscription<std_msgs::msg::String>("/uwvm_test/pubString",rclcpp::QoS{32}.reliable(),[&](std_msgs::msg::String::ConstSharedPtr){witness.receive();})};
                auto sub_string{node->create_subscription<std_msgs::msg::String>("/uwvm_test/subString",rclcpp::QoS{32}.reliable(),[&](std_msgs::msg::String::ConstSharedPtr){witness.receive();})};
                auto unauthorized{node->create_subscription<std_msgs::msg::String>("/unauthorized",rclcpp::QoS{32}.reliable(),[&](std_msgs::msg::String::ConstSharedPtr){witness.receive();})};
                auto pub_twist{node->create_subscription<geometry_msgs::msg::Twist>("/uwvm_test/pubTwist",rclcpp::QoS{32}.reliable(),[&](geometry_msgs::msg::Twist::ConstSharedPtr){witness.receive();})};
                std::this_thread::sleep_for(std::chrono::milliseconds(600));ipc_security(f,client);witness.finish();
            }
            {auto until{std::chrono::steady_clock::now()+std::chrono::milliseconds(500)};while(std::chrono::steady_clock::now()<until){require(node->get_publishers_info_by_topic("/uwvm_test/pubString").empty()&&node->get_publishers_info_by_topic("/uwvm_test/pubTwist").empty()&&node->get_publishers_info_by_topic("/unauthorized").empty(),"IPC forged/malformed/unbound/different-PID attempts created no actual DDS publisher");std::this_thread::sleep_for(std::chrono::milliseconds(20));}::ros_probe::print_fixture(STDOUT_FILENO,"PASS ROS2 actual DDS zero publisher witness after forged session and same-UID different-PID attacks\n");}
            std_msgs::msg::String s;s.data="uwvm real ROS exact payload";check(f,client,node,0,"String",s);
            std_msgs::msg::Bool b;b.data=true;check(f,client,node,1,"Bool",b);
            geometry_msgs::msg::Twist t;t.linear.x=1.25;t.linear.y=-2.;t.linear.z=3.5;t.angular.x=.1;t.angular.y=.2;t.angular.z=.3;check(f,client,node,2,"Twist",t);
            sensor_msgs::msg::Image image;image.header.stamp.sec=123;image.header.stamp.nanosec=456;image.header.frame_id="camera_frame";image.height=2;image.width=2;image.encoding="rgb8";image.step=6;image.data={0,1,2,3,4,5,6,7,8,9,10,11};check(f,client,node,3,"Image",image);
            flood_queue(argv[1],node);
            {std::string resource{"/uwvm_test/callback_teardown_"+std::to_string(::getpid())};auto publisher{node->create_publisher<std_msgs::msg::String>(resource,rclcpp::QoS{8}.reliable())};std_msgs::msg::String sample;sample.data="live";callback_teardown(core::version::ros2,argv[1],resource,[&]{publisher->publish(sample);},[&]{return publisher->get_subscription_count();});}
            service_replay(core::version::ros2,argv[1],[&](std::string const& resource){return std::async(std::launch::async,[node,resource]{auto service{node->create_client<std_srvs::srv::Trigger>(resource)};if(!service->wait_for_service(std::chrono::seconds(3))){return false;}auto request{std::make_shared<std_srvs::srv::Trigger::Request>()};auto sent{service->async_send_request(request)};if(sent.wait_for(std::chrono::seconds(5))!=std::future_status::ready){return false;}auto response{sent.get()};return response&&response->success&&response->message=="hello";});});
            client.close();f.connect(client); // Replay tests exceed the explicit10s idle lease; start a fresh authenticated session.
            std::atomic<unsigned> service_calls{};
            auto service{node->create_service<std_srvs::srv::Trigger>("/uwvm_test/trigger",[&](std::shared_ptr<std_srvs::srv::Trigger::Request>,std::shared_ptr<std_srvs::srv::Trigger::Response> response){++service_calls;response->success=true;response->message="real service";})};
            auto result{client.call("service.call","service",{},1500)};require(!result.error&&result.payload.size()==17&&result.payload[0]==1,"generated Trigger response");
            {
                fixture denied{core::version::ros2,argv[1],[](gateway::configuration& cfg){cfg.policy.connection["node-name"]="uwvm_incompatible_service_qos";cfg.policy.capabilities.clear();auto cap{ros_probe::service(core::version::ros2)};cap.limits["qos.reliability"]="best-effort";cap.limits["timeout-ms"]="300";cfg.policy.capabilities.emplace(cap.alias,std::move(cap));}};
                ipc::client request;denied.connect(request);auto before{service_calls.load()};auto reply{request.call("service.call","service",{},1000)};
                require(reply.error==110&&reply.payload.empty(),"explicit incompatible best-effort Service request times out without disclosure");
                std::this_thread::sleep_for(std::chrono::milliseconds(100));require(service_calls==before,"incompatible Service QoS emitted no actual native callback");request.close();
            }
            ::ros_probe::print_fixture(STDOUT_FILENO,"PASS ROS2 generated Trigger service via actual RMW network; explicit client depth6/server depth7, both request/response actual eight-field QoS checked and incompatible reliability has zero native callbacks\n");
            ipc::client incompatible;f.connect(incompatible);require(!incompatible.call("topic.subscribe","subString",{},1000).error,"QoS mismatch subscriber");
            auto mismatch{node->create_publisher<std_msgs::msg::String>("/uwvm_test/subString",rclcpp::QoS{8}.best_effort())};
            for(unsigned n{};n!=30;++n) { mismatch->publish(s);std::this_thread::sleep_for(std::chrono::milliseconds(50)); }
            require(incompatible.call("event.poll","subString",{},1000).error==11,"best-effort publisher cannot deliver to reliable subscriber");incompatible.close();
            ::ros_probe::print_fixture(STDOUT_FILENO,"PASS actual ROS2 QoS incompatibility has no delivered message\n");
        }
        catch(...) {executor.cancel();spinner.join();rclcpp::shutdown();throw;}
        client.close();executor.cancel();spinner.join();rclcpp::shutdown();return 0;
    }
    catch(std::exception const& e){try { ::ros_probe::print_fixture(STDERR_FILENO,std::string_view{e.what()},"\n"); } catch(...) {}return 1;}
}
