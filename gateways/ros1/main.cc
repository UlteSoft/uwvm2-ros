#include "../common/server.h"
#include <ros/ros.h>
#include <ros/master.h>
#include <ros/service.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Image.h>
#include <std_srvs/Trigger.h>
#include <std_srvs/SetBool.h>
#include <future>
#include "resources.h"
#include "bounded_service.h"
#include "../common/service.h"
#include "tf.h"
#include "action.h"
#include "custom_topics.h"
#include "custom_services.h"
#include "service_availability.h"
#include "custom_actions.h"

namespace gateway=uwvm2::ros_gateway;
namespace ipc=uwvm2::imported::ros::ipc;
using capability=uwvm2::imported::ros::capability_spec;
namespace
{
    template<typename Message> bool message_type(capability const& c)
    { return c.type==ros::message_traits::DataType<Message>::value()&&c.type_identity==ros::message_traits::MD5Sum<Message>::value(); }
    template<typename Service> bool service_type(capability const& c)
    { return c.type==ros::service_traits::DataType<Service>::value()&&c.type_identity==ros::service_traits::MD5Sum<Service>::value(); }
    bool known_type(capability const& c)
    {
        if(c.operation=="event.observe"&&c.type==ipc::event_type&&c.type_identity==ipc::event_type_identity) { return true; }
        if(c.type=="builtin/time-v1"&&c.type_identity=="builtin:time-nanoseconds-v1"&&(c.operation=="time.read-ros"||c.operation=="time.read-wall")) { return true; }
        if(c.type=="builtin/log-v1"&&c.type_identity=="builtin:log-utf8-v1"&&c.operation=="log.write") { return true; }
        if(gateway::ros1_action::known(c)) { return true; }
        if(gateway::ros1_custom_actions::known(c)) { return true; }
        if(gateway::ros1_resources::known(c)) { return true; }
        if(c.operation=="tf.lookup"||c.operation=="tf.broadcast") { return message_type<geometry_msgs::TransformStamped>(c); }
        if(gateway::ros1_custom_services::known(c)) { return true; }
        if(c.operation.rfind("service.",0)==0) { return service_type<std_srvs::Trigger>(c)||service_type<std_srvs::SetBool>(c); }
        if(gateway::ros1_custom::known(c)) { return true; }
        return message_type<std_msgs::String>(c)||message_type<std_msgs::Bool>(c)||message_type<geometry_msgs::Twist>(c)||message_type<sensor_msgs::Image>(c);
    }
    std_msgs::String string_message(std::vector<std::uint8_t> const& d)
    { std_msgs::String m; m.data.assign(reinterpret_cast<char const*>(d.data()+4),d.size()-4); return m; }
    std_msgs::Bool bool_message(std::vector<std::uint8_t> const& d) { std_msgs::Bool m; m.data=d[0]!=0; return m; }
    geometry_msgs::Twist twist_message(std::vector<std::uint8_t> const& d)
    {
        geometry_msgs::Twist m; double* v[]={&m.linear.x,&m.linear.y,&m.linear.z,&m.angular.x,&m.angular.y,&m.angular.z};
        for(unsigned i{};i!=6;++i) { auto bits{ipc::get(d.data()+8*i,8)}; std::memcpy(v[i],&bits,8); } return m;
    }
    std::vector<std::uint8_t> canonical(std_msgs::String const& m)
    { std::vector<std::uint8_t> d; d.reserve(4+m.data.size());ipc::put(d,m.data.size(),4); d.insert(d.end(),m.data.begin(),m.data.end()); return d; }
    std::vector<std::uint8_t> canonical(std_msgs::Bool const& m) { return {static_cast<std::uint8_t>(m.data?1:0)}; }
    std::vector<std::uint8_t> canonical(geometry_msgs::Twist const& m)
    {
        std::vector<std::uint8_t> d; d.reserve(48);for(double v:{m.linear.x,m.linear.y,m.linear.z,m.angular.x,m.angular.y,m.angular.z}) { std::uint64_t bits{}; std::memcpy(&bits,&v,8); ipc::put(d,bits,8); } return d;
    }
    std::vector<std::uint8_t> canonical(sensor_msgs::Image const& m)
    { return ipc::encode_image_view({m.header.seq,m.header.stamp.sec,m.header.stamp.nsec,m.height,m.width,m.step,m.header.frame_id,m.encoding,m.is_bigendian!=0,m.data.data(),m.data.size()}); }
    class ros1_adapter final:public gateway::adapter
    {
        ros::NodeHandle node_;
        gateway::tf_adapter tf_{node_};
        gateway::ros1_action::adapter actions_{node_};
        std::map<std::string,ros::Publisher> publishers_;
        std::map<std::string,ros::Subscriber> subscribers_;
        gateway::ros1_custom::topic_store custom_topics_{node_,publishers_,subscribers_};
        gateway::ros1_custom_services::adapter custom_services_{node_};
        gateway::ros1_custom_actions::adapter custom_actions_{node_};
        std::map<std::string,ros::ServiceServer> services_;
        std::map<std::string,std::shared_ptr<gateway::service_state>> service_states_;
        template<typename S> ipc::call_result serve(capability const& c,std::shared_ptr<gateway::event_queue> events)
        {
            auto state{std::make_shared<gateway::service_state>(c,std::move(events))};std::weak_ptr<gateway::service_state> weak{state};
            boost::function<bool(typename S::Request&,typename S::Response&)> callback{[weak](typename S::Request& request,typename S::Response& response)
            {
                auto state{weak.lock()};if(!state) { return false; }std::vector<std::uint8_t> bytes;
                if constexpr(std::is_same_v<S,std_srvs::SetBool>) { bytes.push_back(request.data?1:0); }
                auto pending{state->begin(std::move(bytes))};if(!pending||!state->wait(pending,bytes)) { return false; }
                response.success=bytes[0]!=0;response.message.assign(reinterpret_cast<char const*>(bytes.data()+5),bytes.size()-5);
                return std::chrono::steady_clock::now()<std::min(pending->deadline,pending->answer_deadline);
            }};
            auto server{node_.advertiseService(c.resource,callback)};if(!server) { return {17,{}}; }
            services_.emplace(c.alias,std::move(server));service_states_.emplace(c.alias,std::move(state));return {};
        }
        template<typename M> ipc::call_result send(capability const& c,M const& m)
        {
            auto p{publishers_.find(c.alias)};
            if(p==publishers_.end()) { p=publishers_.emplace(c.alias,node_.advertise<M>(c.resource,static_cast<std::uint32_t>(gateway::limit(c,"queue-depth")),gateway::field(c,"latch")=="true")).first; }
            p->second.publish(m); return {};
        }
        template<typename M> ipc::call_result listen(capability const& c,std::shared_ptr<gateway::event_queue> q)
        {
            std::weak_ptr<gateway::event_queue> weak{q};
            boost::function<void(boost::shared_ptr<M const> const&)> callback{[weak](boost::shared_ptr<M const> const& message)
            {
                if(auto queue=weak.lock())
                {
                    auto maximum{gateway::limit(queue->capability,"max-message-bytes")};
                    if constexpr(std::is_same_v<M,std_msgs::String>) { if(!gateway::wire_size_fits(maximum,{4,message->data.size()})) { return; } }
                    else if constexpr(std::is_same_v<M,sensor_msgs::Image>) { if(message->is_bigendian>1||message->header.frame_id.size()>255||message->encoding.size()>255||!gateway::wire_size_fits(maximum,{37,message->header.frame_id.size(),message->encoding.size(),message->data.size()})) { return; } }
                    else if constexpr(std::is_same_v<M,geometry_msgs::Twist>) { if(maximum<48) { return; } }
                    else { if(!maximum) { return; } }
                    queue->enqueue(canonical(*message));
                }
            }};
            subscribers_.emplace(c.alias,node_.subscribe<M>(c.resource,static_cast<std::uint32_t>(gateway::limit(c,"queue-depth")),callback,ros::VoidConstPtr{},ros::TransportHints{}.tcp())); return {};
        }
        template<typename S> ipc::call_result invoke(capability const& c,S request,std::uint32_t timeout)
        {
            auto until{ipc::after(static_cast<std::uint32_t>(std::min<std::uint64_t>(timeout,gateway::limit(c,"timeout-ms"))))};
            auto remaining{std::chrono::duration_cast<std::chrono::milliseconds>(until-std::chrono::steady_clock::now()).count()};
            if(remaining<=0) {return {110,{}};}
            auto error{gateway::ros1_service::call(c,request,static_cast<std::uint32_t>(remaining),[](std::vector<std::uint8_t> const& data)
            { return data.size()>=5&&data[0]<=1&&ipc::get(data.data()+1,4)==data.size()-5&&gateway::valid_utf8(data.data()+5,data.size()-5); })};
            if(error) { return {error,{}}; }
            auto const& text{request.response.message};if(!gateway::wire_size_fits(gateway::limit(c,"max-response-bytes"),{5,text.size()})) { return {90,{}}; }if(!gateway::valid_utf8(reinterpret_cast<std::uint8_t const*>(text.data()),text.size())) { return {22,{}}; }
            std::vector<std::uint8_t> d;d.reserve(5+text.size());d.push_back(static_cast<std::uint8_t>(request.response.success?1:0));
            ipc::put(d,request.response.message.size(),4); d.insert(d.end(),request.response.message.begin(),request.response.message.end());
            if(d.size()>gateway::limit(c,"max-response-bytes")) { return {90,{}}; }
            return std::chrono::steady_clock::now()>=until?ipc::call_result{110,{}}:ipc::call_result{0,std::move(d)};
        }
    public:
        ~ros1_adapter() override { for(auto const& s:service_states_) { s.second->deactivate(); } }
        ipc::call_result publish(capability const& c,std::vector<std::uint8_t> const& d) override
        {
            if(!known_type(c)) { return {13,{}}; }
            if(message_type<std_msgs::String>(c)) { return send(c,string_message(d)); }
            if(message_type<std_msgs::Bool>(c)) { return send(c,bool_message(d)); }
            if(message_type<geometry_msgs::Twist>(c)) { return send(c,twist_message(d)); }
            if(message_type<sensor_msgs::Image>(c))
            {
                ipc::image_view i; if(!ipc::decode_image_view(d.data(),d.size(),1,i)) { return {22,{}}; }
                sensor_msgs::Image m; m.header.seq=i.sequence; m.header.stamp.sec=i.seconds; m.header.stamp.nsec=i.nanoseconds; m.header.frame_id=i.frame_id;
                m.height=i.height; m.width=i.width; m.encoding=i.encoding; m.is_bigendian=i.bigendian?1:0; m.step=i.step; m.data.assign(i.data,i.data+i.data_size); return send(c,m);
            }
            return gateway::ros1_custom::publish(custom_topics_,c,d);
        }
        ipc::call_result subscribe(capability const& c,std::shared_ptr<gateway::event_queue> q) override
        {
            if(!known_type(c)) { return {13,{}}; }
            if(message_type<std_msgs::String>(c)) { return listen<std_msgs::String>(c,std::move(q)); }
            if(message_type<std_msgs::Bool>(c)) { return listen<std_msgs::Bool>(c,std::move(q)); }
            if(message_type<geometry_msgs::Twist>(c)) { return listen<geometry_msgs::Twist>(c,std::move(q)); }
            if(message_type<sensor_msgs::Image>(c)) { return listen<sensor_msgs::Image>(c,std::move(q)); }
            return gateway::ros1_custom::subscribe(custom_topics_,c,std::move(q));
        }
        ipc::call_result service(capability const& c,std::vector<std::uint8_t> const& d,std::uint32_t timeout) override
        {
            if(ipc::find_custom_service(1,c.type)) { return custom_services_.call(c,d,timeout); }
            if(service_type<std_srvs::Trigger>(c)) { return invoke(c,std_srvs::Trigger{},timeout); }
            if(service_type<std_srvs::SetBool>(c)) { std_srvs::SetBool s; s.request.data=d[0]!=0; return invoke(c,s,timeout); }
            return {95,{}};
        }
        ipc::call_result advertise(capability const& c,std::shared_ptr<gateway::event_queue> events) override
        {
            if(ipc::find_custom_service(1,c.type)) { return custom_services_.advertise(c,std::move(events)); }
            for(auto const& s:service_states_) { if(s.second->capability.resource==c.resource) { return {17,{}}; } }
            if(service_type<std_srvs::Trigger>(c)) { return serve<std_srvs::Trigger>(c,std::move(events)); }
            if(service_type<std_srvs::SetBool>(c)) { return serve<std_srvs::SetBool>(c,std::move(events)); }return {95,{}};
        }
        ipc::call_result advertise_action(capability const& c,std::shared_ptr<gateway::event_queue> events) override
        { return ipc::find_custom_action(1,c.type)?custom_actions_.advertise(c,std::move(events)):actions_.advertise(c,std::move(events)); }
        ipc::call_result observe_action(capability const& c,std::uint64_t goal) override
        { return ipc::find_custom_action(1,c.type)?custom_actions_.observe(c,goal):actions_.observe(c,goal); }
        void close(std::string const& alias) override
        { auto s{service_states_.find(alias)};if(s!=service_states_.end()){s->second->deactivate();service_states_.erase(s);}services_.erase(alias);custom_services_.close(alias);custom_actions_.close(alias);custom_topics_.close(alias);tf_.close(alias);actions_.close(alias); }
        ipc::call_result service_availability(capability const& availability,capability const& source,
            ipc::client_availability::packet const& request,std::uint32_t timeout) override
        {
            // Dispatch the ORIGINAL independently granted service.call contract
            // through existing native identity/complete custom schema checks.
            auto qualify=[](capability const& selected){return known_type(selected);};
            auto finish=[&](gateway::ros1_service_availability::result value)->ipc::call_result
            {
                if(value.error) { return {value.error,{}}; }
                auto response{request};response.value=value.value;auto raw{ipc::client_availability::encode(response)};
                return {0,{raw.begin(),raw.end()}};
            };
            if(source.type=="std_srvs/Trigger") { return finish(gateway::ros1_service_availability::observe<std_srvs::Trigger>(availability,source,request,timeout,qualify)); }
            if(source.type=="std_srvs/SetBool") { return finish(gateway::ros1_service_availability::observe<std_srvs::SetBool>(availability,source,request,timeout,qualify)); }
#define UWVM_CUSTOM_AVAILABILITY(Native,Type,Identity,Canonical,Request,Response) \
            if(source.type==Type){return finish(gateway::ros1_service_availability::observe<Native>(availability,source,request,timeout,qualify));}
            UWVM_ROS_CUSTOM_ROS1_SERVICE_TYPES(UWVM_CUSTOM_AVAILABILITY)
#undef UWVM_CUSTOM_AVAILABILITY
            return {95,{}};
        }
        ipc::call_result operation(capability const& c,std::vector<std::uint8_t> const& d,std::uint32_t timeout) override
        {
            auto started{std::chrono::steady_clock::now()};
            if(c.operation=="tf.lookup"||c.operation=="tf.broadcast") { return tf_.call(c,d,timeout); }
            if(c.operation.rfind("action.",0)==0) { return ipc::find_custom_action(1,c.type)?custom_actions_.call(c,d,timeout):actions_.call(c,d,timeout); }
            if(c.operation=="service.respond")
            { auto until{started+std::chrono::milliseconds(std::min<std::uint64_t>(timeout,gateway::limit(c,"timeout-ms")))};
              if(ipc::find_custom_service(1,c.type)) { return custom_services_.respond(c,d,timeout); }
              for(auto const& s:service_states_) { if(s.second->capability.resource==c.resource) { return s.second->answer(c,d,until); } }return {9,{}}; }
            if(c.operation=="time.read-ros"||c.operation=="time.read-wall")
            { std::vector<std::uint8_t> bytes; ipc::put(bytes,c.operation=="time.read-ros"?ros::Time::now().toNSec():ros::WallTime::now().toNSec(),8); return {0,std::move(bytes)}; }
            if(c.operation=="log.write")
            {
                std::string message{reinterpret_cast<char const*>(d.data()+4),d.size()-4}; auto severity{gateway::field(c,"severity")};
                if(severity=="debug") { ROS_DEBUG("%s",message.c_str()); }
                else if(severity=="info") { ROS_INFO("%s",message.c_str()); }
                else if(severity=="warn") { ROS_WARN("%s",message.c_str()); }
                else if(severity=="error") { ROS_ERROR("%s",message.c_str()); }
                else if(severity=="fatal") { ROS_FATAL("%s",message.c_str()); }
                else { return {22,{}}; } return {};
            }
            return gateway::ros1_resources::call(node_,c,d,timeout);
        }
    };
}
int main(int argc,char** argv)
{
    try
    {
        if(argc!=2) { throw std::runtime_error("usage: uwvm-ros1-gateway <trusted-config>"); }
        auto c{gateway::load(argv[1])}; if(c.policy.ros_version!=uwvm2::imported::ros::version::ros1) { throw std::runtime_error("wrong backend"); }
        for(auto const& p:c.policy.capabilities)
        {
            auto const* selected{&p.second};
            if(selected->operation=="service.available")
            {
                auto source{c.policy.capabilities.find(gateway::field(*selected,"service-source-alias"))};
                if(source==c.policy.capabilities.end()||!uwvm2::imported::ros::service_availability_pair(c.policy.ros_version,*selected,source->second))
                { throw std::runtime_error("availability source contract is not independently native-qualified"); }
                selected=&source->second;
            }
            if(!known_type(*selected)) { throw std::runtime_error("UNSUPPORTED or mismatched generated ROS1 adapter: "+selected->type); }
        }
        std::set<std::string> allowed{"master-uri","node-name","namespace","advertise-host","transport"};
        if(c.policy.connection.size()!=allowed.size()) { throw std::runtime_error("all ROS1 connection fields must be explicit"); }
        for(auto const& p:c.policy.connection) { if(!allowed.count(p.first)) { throw std::runtime_error("unknown ROS1 connection field"); } }
        auto const& connection{c.policy.connection};
        if(connection.at("transport")!="tcpros"||!gateway::canonical(connection.at("namespace"),true)||!ipc::valid_token(connection.at("node-name"))||connection.at("master-uri").rfind("http://",0)!=0||connection.at("advertise-host").empty()) { throw std::runtime_error("invalid connection identity"); }
        ::setenv("ROS_MASTER_URI",connection.at("master-uri").c_str(),1); ::setenv("ROS_IP",connection.at("advertise-host").c_str(),1); ::unsetenv("ROS_HOSTNAME");
        ros::M_string remapping; remapping["__ns"]=connection.at("namespace");
        ros::init(remapping,connection.at("node-name"),ros::init_options::NoSigintHandler|ros::init_options::NoRosout);
        ros::AsyncSpinner spinner{1}; spinner.start();
        int rc{gateway::run(c,[] { return std::make_unique<ros1_adapter>(); })}; spinner.stop(); ros::shutdown(); return rc;
    }
    catch(std::exception const& e) { gateway::native_io::diagnostic("BLOCKED ",e.what()); return 1; }
}
