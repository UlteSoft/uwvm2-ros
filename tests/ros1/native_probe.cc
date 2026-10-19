#include "../ros_ipc/probe_common.h"
#include "../ros_ipc/service_replay.h"
#include "../ros_ipc/callback_teardown.h"
#include "../ros_ipc/active_witness.h"
#include <ros/ros.h>
#include <ros/master.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Image.h>
#include <std_srvs/Trigger.h>
#include <std_srvs/SetBool.h>
using namespace ros_probe;
#include "../ros_ingress/cleanup_ros1.h"
#include "../ros_ingress/revoke_ros1.h"
namespace
{
    std::vector<std::uint8_t> encode(std_msgs::String const& m) { std::vector<std::uint8_t> d; ipc::put(d,m.data.size(),4); d.insert(d.end(),m.data.begin(),m.data.end()); return d; }
    std::vector<std::uint8_t> encode(std_msgs::Bool const& m) { return {static_cast<std::uint8_t>(m.data?1:0)}; }
    std::vector<std::uint8_t> encode(geometry_msgs::Twist const& m) { std::vector<std::uint8_t> d; for(double v:{m.linear.x,m.linear.y,m.linear.z,m.angular.x,m.angular.y,m.angular.z}) { std::uint64_t bits{}; std::memcpy(&bits,&v,8); ipc::put(d,bits,8); } return d; }
    std::vector<std::uint8_t> encode(sensor_msgs::Image const& m) { return ipc::encode_image({m.header.seq,m.header.stamp.sec,m.header.stamp.nsec,m.height,m.width,m.step,m.header.frame_id,m.encoding,m.is_bigendian!=0,m.data}); }
    template<typename M> int witness(ros::NodeHandle& node,std::string const& mode,std::string const& resource,M const& sample,bool image=false)
    {
        auto expected{encode(sample)};if(image) { for(unsigned n{};n!=4;++n){expected[n]=0;} }
        bool counted{mode=="--observe-count2"||mode=="--observe-flood"};std::atomic<bool> received{},wrong{};std::atomic<unsigned> count{};
        boost::function<void(boost::shared_ptr<M const> const&)> cb{[&](boost::shared_ptr<M const> const& m) { auto actual{encode(*m)};if(image&&actual.size()>=4){for(unsigned n{};n!=4;++n){actual[n]=0;}}if(mode=="--absent"||actual!=expected){wrong=true;}else{received=true;if(++count>2&&counted){wrong=true;}} }};
        auto subscriber{(mode!="--publish"&&mode!="--absent-subscription")?node.subscribe<M>(resource,4,cb):ros::Subscriber{}};
        auto publisher{(mode=="--publish"||mode=="--absent-subscription")?node.advertise<M>(resource,4,false):ros::Publisher{}};
        auto end{std::chrono::steady_clock::now()+std::chrono::seconds(counted?5:(mode=="--absent"||mode=="--absent-subscription")?3:12)};
        if(counted){::ros_probe::print_fixture(STDOUT_FILENO,"READY actual ROS1 two-message counter\n");}
        while(std::chrono::steady_clock::now()<end&&(counted||!received)&&!wrong){if(mode=="--publish"||mode=="--absent-subscription"){publisher.publish(sample);}std::this_thread::sleep_for(std::chrono::milliseconds(50));}
        if(counted){require(count==2&&!wrong,"bounded actual ROS1 publication count expected2 actual="+std::to_string(count.load()));::ros_probe::print_fixture(STDOUT_FILENO,"BOUNDED_NATIVE_PUBLICATIONS native_count=2 excess_count=0\n");}
        if(mode=="--absent-subscription"){require(!publisher.getNumSubscribers(),"denied operation created no actual ROS1 subscriber");}
        if(mode=="--observe"){require(received&&!wrong,"actual native ROS1 exact content");}
        if(mode=="--absent")
        {
            require(!wrong,"denied call emitted no ROS1 message");ros::master::V_TopicInfo topics;require(ros::master::getTopics(topics),"ROS1 Master registration witness");
            for(auto const& topic:topics){require(topic.name!=resource,"denied call registered no ROS1 publisher");}
        }
        ::ros_probe::print_fixture(STDOUT_FILENO,"PASS actual ROS1 native witness ",mode," content + network side effect\n");return 0;
    }
    void flood_queue(std::string const& executable,ros::NodeHandle& node)
    {
        for(auto const& overflow:{"drop-oldest","drop-newest","reject"})
        {
            fixture bounded{core::version::ros1,executable,[&](gateway::configuration& cfg){auto cap{cfg.policy.capabilities.at("subString")};cfg.policy.capabilities.clear();cfg.policy.connection["node-name"]="uwvm_ros1_queue_probe";cap.limits["max-message-bytes"]="64";cap.limits["max-queue-bytes"]="128";cap.limits["burst"]="2";cap.limits["queue-depth"]="2";cap.limits["overflow-policy"]=overflow;cap.resource="/uwvm_test/event_queue_"+std::to_string(::getpid())+"_"+std::to_string(std::string{overflow}=="drop-oldest"?1:std::string{overflow}=="drop-newest"?2:3);cfg.policy.capabilities.emplace("subString",std::move(cap));}};
            ipc::client client;bounded.connect(client);require(!client.call("topic.subscribe","subString",{},1000).error,"bounded actual event subscription");auto publisher{node.advertise<std_msgs::String>(bounded.config.policy.capabilities.at("subString").resource,2,false)};
            for(unsigned n{};n!=200&&!publisher.getNumSubscribers();++n){std::this_thread::sleep_for(std::chrono::milliseconds(10));}
            require(publisher.getNumSubscribers(),"actual event flood subscriber discovery");
            std_msgs::String warmup;warmup.data="warmup";ipc::call_result connected{11,{}};
            for(unsigned n{};n!=100&&connected.error;++n){publisher.publish(warmup);std::this_thread::sleep_for(std::chrono::milliseconds(30));connected=client.call("event.poll","subString",{},1000);require(!connected.error||connected.error==11,"actual event readiness handshake");}
            require(!connected.error&&connected.payload==encode(warmup),"actual DDS/TCPROS data path readiness");std::this_thread::sleep_for(std::chrono::milliseconds(200));
            for(unsigned n{};n!=100;++n){auto drained{client.call("event.poll","subString",{},1000)};if(drained.error==11){break;}require(!drained.error&&drained.payload==encode(warmup),"drain only readiness warmup");std::this_thread::sleep_for(std::chrono::milliseconds(10));}
            std_msgs::String oversized;oversized.data.assign(65,'x');for(unsigned n{};n!=2;++n){publisher.publish(oversized);std::this_thread::sleep_for(std::chrono::milliseconds(30));}require(client.call("event.poll","subString",{},1000).error==11,"actual native oversized String dropped before bounded bridge copy/queue");
            for(unsigned n{1};n<=20;++n){std_msgs::String message;message.data="event-"+std::to_string(n);publisher.publish(message);std::this_thread::sleep_for(std::chrono::milliseconds(30));}
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            for(unsigned n{};n!=2;++n){std_msgs::String message;message.data="event-"+std::to_string(std::string{overflow}=="drop-oldest"?19+n:1+n);auto result{client.call("event.poll","subString",{},1000)};require(!result.error&&result.payload==encode(message),"actual native event overflow exact retained frame");std::this_thread::sleep_for(std::chrono::milliseconds(10));}
            require(client.call("event.poll","subString",{},1000).error==11,"actual event queue bounded to exactly two frames");client.close();
            ::ros_probe::print_fixture(STDOUT_FILENO,"PASS ROS1 actual native20-message event flood depth2/128bytes ",overflow," exact retained frames\n");
        }
    }
    template<typename M> void check(fixture& f,ipc::client& client,ros::NodeHandle& node,unsigned index,std::string const& name,M const& message)
    {
        auto expected{data(index)}; if(index==3) { for(unsigned n{};n!=4;++n) { expected[n]=0; } } std::mutex mutex; std::vector<std::uint8_t> received; unsigned count{};
        boost::function<void(boost::shared_ptr<M const> const&)> cb{[&](boost::shared_ptr<M const> const& m) { std::lock_guard<std::mutex> lock{mutex}; received=encode(*m); if(index==3) { for(unsigned n{};n!=4;++n) { received[n]=0; } } ++count; }};
        auto subscriber{node.subscribe<M>("/uwvm_test/pub"+name,8,cb)};
        auto publisher{node.advertise<M>("/uwvm_test/sub"+name,8,false)};
        require(!client.call("topic.subscribe","sub"+name,{},1000).error,"subscribe "+name);
        for(unsigned n{};n!=300;++n)
        {
            require(!client.call("topic.publish","pub"+name,expected,1000).error,"publish "+name);
            { std::lock_guard<std::mutex> lock{mutex}; if(received==expected) { break; } }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        { std::lock_guard<std::mutex> lock{mutex}; require(received==expected,"gateway -> native exact "+name); }
        ipc::call_result incoming{11,{}};
        for(unsigned n{};n!=300;++n)
        { publisher.publish(message); std::this_thread::sleep_for(std::chrono::milliseconds(10)); incoming=client.call("event.poll","sub"+name,{},1000); if(!incoming.error) { if(index==3) { for(unsigned j{};j!=4;++j) { incoming.payload[j]=0; } } break; } require(incoming.error==11,"poll pending"); }
        require(!incoming.error&&incoming.payload==expected,"native -> gateway exact "+name);
        unsigned before; { std::lock_guard<std::mutex> lock{mutex}; before=count; }
        require(client.call("topic.publish","denied",expected,1000).error==13,"unauthorized operation denied");
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        { std::lock_guard<std::mutex> lock{mutex}; require(count==before,"denial has no actual network publish side effect"); }
        require(!client.call("capability.revoke","sub"+name,{},1000).error,"revoke subscription");
        publisher.publish(message); require(client.call("event.poll","sub"+name,{},1000).error==125,"revoked cannot receive");
        require(!client.call("capability.close","pub"+name,{},1000).error,"close publisher");
        for(unsigned n{};n!=100&&subscriber.getNumPublishers();++n) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
        require(!subscriber.getNumPublishers(),"publisher removed from actual ROS network");
        ::ros_probe::print_fixture(STDOUT_FILENO,"PASS ROS1 ",name," bidirectional exact message, denial/no side effect, revoke, publisher cleanup\n");
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
            std::string mode{argv[1]},resource{argv[2]};ros::init(argc,argv,"uwvm_ros1_service_witness",ros::init_options::NoSigintHandler|ros::init_options::NoRosout);ros::NodeHandle node;ros::AsyncSpinner spinner{1};spinner.start();std::atomic<unsigned> count{};std::atomic<bool> wrong{};ros::ServiceServer server;
            if(mode=="--serve-trigger"||mode=="--deny-service"){boost::function<bool(std_srvs::Trigger::Request&,std_srvs::Trigger::Response&)> cb{[&](auto&,auto& response){++count;response.success=true;response.message="hello";return true;}};server=node.advertiseService(resource,cb);}
            else if(mode=="--serve-setbool"){boost::function<bool(std_srvs::SetBool::Request&,std_srvs::SetBool::Response&)> cb{[&](auto& request,auto& response){++count;if(!request.data){wrong=true;}response.success=true;response.message="hello";return true;}};server=node.advertiseService(resource,cb);}
            auto end{std::chrono::steady_clock::now()+std::chrono::seconds(mode=="--absent-service"?3:mode=="--deny-service"?4:12)};::ros_probe::print_fixture(STDOUT_FILENO,"READY real ROS1 service witness\n");
            while(std::chrono::steady_clock::now()<end&&!count){if(mode=="--absent-service"){require(!ros::service::exists(resource,false),"denied operation registered no actual ROS1 service");}std::this_thread::sleep_for(std::chrono::milliseconds(50));}
            if(mode=="--deny-service"){require(!count,"unauthorized Wasm service call reached no actual native Trigger callback");::ros_probe::print_fixture(STDOUT_FILENO,"NO_NATIVE_SERVICE_REQUESTS native_trigger_callbacks=0\n");}
            else if(mode!="--absent-service"){require(count==1&&!wrong,"actual Wasm service client exactly one valid typed native request");std::this_thread::sleep_for(std::chrono::milliseconds(500));}
            spinner.stop();ros::shutdown();::ros_probe::print_fixture(STDOUT_FILENO,"PASS actual ROS1 native service witness ",mode,"\n");return 0;
        }
        if(argc>=3&&(std::string{argv[1]}=="--call-trigger"||std::string{argv[1]}=="--call-setbool"))
        {
            std::string mode{argv[1]},resource{argv[2]};ros::init(argc,argv,"uwvm_ros1_service_witness",ros::init_options::NoSigintHandler|ros::init_options::NoRosout);ros::NodeHandle node;
            if(mode=="--call-trigger") { auto client{node.serviceClient<std_srvs::Trigger>(resource)};require(client.waitForExistence(ros::Duration{12}),"real Trigger server discovery");std_srvs::Trigger call;require(client.call(call)&&call.response.success&&call.response.message=="hello","actual Wasm Trigger server response"); }
            else { auto client{node.serviceClient<std_srvs::SetBool>(resource)};require(client.waitForExistence(ros::Duration{12}),"real SetBool server discovery");std_srvs::SetBool call;call.request.data=true;require(client.call(call)&&call.response.success&&call.response.message=="hello","actual Wasm SetBool server response"); }
            ::ros_probe::print_fixture(STDOUT_FILENO,"PASS real ROS1 Wasm queued service response true/hello\n");ros::shutdown();return 0;
        }
        if((argc==4||argc==5)&&(std::string{argv[1]}=="--observe"||std::string{argv[1]}=="--publish"||std::string{argv[1]}=="--absent"||std::string{argv[1]}=="--absent-subscription"||std::string{argv[1]}=="--observe-count2"||std::string{argv[1]}=="--observe-flood"))
        {
            std::string mode{argv[1]},resource{argv[2]},text{argv[3]},type{argc==5?argv[4]:"String"};
            ros::init(argc,argv,"uwvm_ros1_network_witness",ros::init_options::NoSigintHandler|ros::init_options::NoRosout);ros::NodeHandle node;ros::AsyncSpinner spinner{1};spinner.start();int result{};
            if(type=="String"){std_msgs::String s;s.data=text;result=witness(node,mode,resource,s);}
            else if(type=="Bool"){std_msgs::Bool m;m.data=true;result=witness(node,mode,resource,m);}
            else if(type=="Twist"){geometry_msgs::Twist m;m.linear.x=1.25;m.linear.y=-2.;m.linear.z=3.5;m.angular.x=.1;m.angular.y=.2;m.angular.z=.3;result=witness(node,mode,resource,m);}
            else if(type=="Image"){sensor_msgs::Image m;m.header.seq=7;m.header.stamp.sec=123;m.header.stamp.nsec=456;m.header.frame_id="camera_frame";m.height=2;m.width=2;m.encoding="rgb8";m.step=6;m.data={0,1,2,3,4,5,6,7,8,9,10,11};result=witness(node,mode,resource,m,true);}
            else{throw std::runtime_error("unsupported witness type");}spinner.stop();ros::shutdown();return result;
        }
        require(argc==2,"usage native-probe gateway"); fixture f{uwvm2::imported::ros::version::ros1,argv[1]}; ipc::client client; f.connect(client);
        ros::init(argc,argv,"uwvm_native_ros1_probe",ros::init_options::NoSigintHandler|ros::init_options::NoRosout); ros::NodeHandle node; ros::AsyncSpinner spinner{2}; spinner.start();
        {
            active_ipc_witness witness{[] { ros::master::V_TopicInfo topics;if(!ros::master::getTopics(topics)) { return false; }for(auto const& topic:topics) { if(topic.name=="/uwvm_test/pubString"||topic.name=="/uwvm_test/subString"||topic.name=="/uwvm_test/pubTwist"||topic.name=="/unauthorized") { return false; } }return true; }};
            boost::function<void(std_msgs::String::ConstPtr const&)> string_callback{[&](auto const&){witness.receive();}};
            boost::function<void(geometry_msgs::Twist::ConstPtr const&)> twist_callback{[&](auto const&){witness.receive();}};
            auto pub_string{node.subscribe<std_msgs::String>("/uwvm_test/pubString",32,string_callback)};
            auto sub_string{node.subscribe<std_msgs::String>("/uwvm_test/subString",32,string_callback)};
            auto unauthorized{node.subscribe<std_msgs::String>("/unauthorized",32,string_callback)};
            auto pub_twist{node.subscribe<geometry_msgs::Twist>("/uwvm_test/pubTwist",32,twist_callback)};
            std::this_thread::sleep_for(std::chrono::milliseconds(600));ipc_security(f,client);witness.finish();
        }
        {ros::master::V_TopicInfo names;require(ros::master::getTopics(names),"actual ROS1 IPC attack graph witness");for(auto const& topic:names){require(topic.name!="/uwvm_test/pubString"&&topic.name!="/uwvm_test/pubTwist"&&topic.name!="/unauthorized","IPC forged/malformed/unbound/different-PID attempts created no actual ROS1 publisher");}::ros_probe::print_fixture(STDOUT_FILENO,"PASS ROS1 actual Master zero publisher witness after forged session and same-UID different-PID attacks\n");}
        std_msgs::String s; s.data="uwvm real ROS exact payload"; check(f,client,node,0,"String",s);
        std_msgs::Bool b; b.data=true; check(f,client,node,1,"Bool",b);
        geometry_msgs::Twist t; t.linear.x=1.25;t.linear.y=-2.;t.linear.z=3.5;t.angular.x=.1;t.angular.y=.2;t.angular.z=.3;check(f,client,node,2,"Twist",t);
        sensor_msgs::Image image; image.header.seq=7;image.header.stamp.sec=123;image.header.stamp.nsec=456;image.header.frame_id="camera_frame";image.height=2;image.width=2;image.encoding="rgb8";image.step=6;image.data={0,1,2,3,4,5,6,7,8,9,10,11}; check(f,client,node,3,"Image",image);
        flood_queue(argv[1],node);
        {std::string resource{"/uwvm_test/callback_teardown_"+std::to_string(::getpid())};auto publisher{node.advertise<std_msgs::String>(resource,8,false)};std_msgs::String sample;sample.data="live";callback_teardown(core::version::ros1,argv[1],resource,[&]{publisher.publish(sample);},[&]{return publisher.getNumSubscribers();});}
        service_replay(core::version::ros1,argv[1],[&](std::string const& resource){return std::async(std::launch::async,[&,resource]{auto service{node.serviceClient<std_srvs::Trigger>(resource)};if(!service.waitForExistence(::ros::Duration{3})){return false;}std_srvs::Trigger call;return service.call(call)&&call.response.success&&call.response.message=="hello";});});
        client.close();f.connect(client); // Replay tests exceed the explicit10s idle lease; start a fresh authenticated session.
        std::atomic<bool> slow{};boost::function<bool(std_srvs::Trigger::Request&,std_srvs::Trigger::Response&)> callback{[&](auto&,auto& response) { if(slow){std::this_thread::sleep_for(std::chrono::seconds(2));}response.success=true;response.message="real service";return true; }};
        auto server{node.advertiseService("/uwvm_test/trigger",callback)};
        auto result{client.call("service.call","service",{},1500)};
        require(!result.error&&result.payload.size()==17&&result.payload[0]==1,"generated Trigger service response");
        slow=true;auto deadline_start{std::chrono::steady_clock::now()};auto expired{client.call("service.call","service",{},1500)};
        require(expired.error==110&&std::chrono::steady_clock::now()-deadline_start<std::chrono::milliseconds(1300),"ROS1 persistent service deadline cancels blocked native call");
        ::ros_probe::print_fixture(STDOUT_FILENO,"PASS ROS1 actual slow native service cancellation within explicit1s deadline\n");
        ros::master::V_TopicInfo topics; require(ros::master::getTopics(topics),"actual ROS Master graph query");
        for(auto const& topic:topics) { require(topic.name!="/unauthorized","denied publisher never registered"); }
        ::ros_probe::print_fixture(STDOUT_FILENO,"PASS ROS1 generated Trigger service and actual ROS Master registration/denied side effect verification\n");
        client.close(); spinner.stop(); ros::shutdown(); return 0;
    }
    catch(std::exception const& e) { try { ::ros_probe::print_fixture(STDERR_FILENO,std::string_view{e.what()},"\n"); } catch(...) {} return 1; }
}
