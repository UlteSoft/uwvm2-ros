// Production may never silently select the original Node with its implicit
// TimeSource subscription, including builds outside this CMake target.
#if defined(UWVM_ROS2_USE_COMPOSED_NODE) && !UWVM_ROS2_USE_COMPOSED_NODE
#error "Production ROS2 Gateway requires the endpoint-free composed Node"
#endif
#ifndef UWVM_ROS2_USE_COMPOSED_NODE
#define UWVM_ROS2_USE_COMPOSED_NODE 1
#endif
#include "compat/node.h"
#include "../common/server.h"
#include <rclcpp/rclcpp.hpp>
#include <rmw/rmw.h>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/bool.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <std_srvs/srv/set_bool.hpp>
#include <rosidl_typesupport_cpp/message_type_support.hpp>
#include <rosidl_typesupport_cpp/service_type_support.hpp>
#include <iomanip>
#include <limits>
#include "resources.h"
#include "../common/service.h"
#include "tf.h"
#include "lifecycle.h"
#include "action.h"
#include "qos.h"
#include "custom_topics.h"
#include "custom_services.h"
#include "service_availability.h"
#include "native_rmw_publisher_registry.h"
#include "native_rmw_subscription_registry.h"
#include "native_rmw_subscription_factory.h"
#include "custom_actions.h"
#include "environment.h"
#include "compat/identity.h"
#include "compat/deferred_service.h"
#ifndef UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
#define UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE 0
#endif
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
#include "owned-service-profile.h"
#endif
#ifndef UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
#define UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE 0
#endif
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
#include "default-control-profile.h"
#endif

namespace gateway=uwvm2::ros_gateway;
namespace ipc=uwvm2::imported::ros::ipc;
using capability=uwvm2::imported::ros::capability_spec;
namespace
{
    bool builtin_known_type(capability const& c)
    {
        namespace compat=gateway::ros2_compat;
        if(c.operation=="tf.lookup"||c.operation=="tf.broadcast") { return UWVM_ROS2_HAVE_TF&&c.type=="geometry_msgs/msg/TransformStamped"&&compat::builtin_message<geometry_msgs::msg::TransformStamped>(c.type,c.type_identity); }
        if(c.operation.rfind("service.",0)==0)
        {
            if(c.type=="std_srvs/srv/Trigger") { return compat::builtin_service<std_srvs::srv::Trigger>(c.type,c.type_identity); }
            if(c.type=="std_srvs/srv/SetBool") { return compat::builtin_service<std_srvs::srv::SetBool>(c.type,c.type_identity); }
            return false;
        }
        if(c.type=="std_msgs/msg/String") { return compat::builtin_message<std_msgs::msg::String>(c.type,c.type_identity); }
        if(c.type=="std_msgs/msg/Bool") { return compat::builtin_message<std_msgs::msg::Bool>(c.type,c.type_identity); }
        if(c.type=="geometry_msgs/msg/Twist") { return compat::builtin_message<geometry_msgs::msg::Twist>(c.type,c.type_identity); }
        if(c.type=="sensor_msgs/msg/Image") { return compat::builtin_message<sensor_msgs::msg::Image>(c.type,c.type_identity); }
        return false;
    }
    bool known_type(capability const& c)
    {
        if(c.operation=="event.observe"&&c.type==ipc::event_type&&c.type_identity==ipc::event_type_identity) { return true; }
        if(c.type=="builtin/time-v1"&&c.type_identity=="builtin:time-nanoseconds-v1"&&(c.operation=="time.read-ros"||c.operation=="time.read-system"||c.operation=="time.read-steady")) { return true; }
        if(c.type=="builtin/log-v1"&&c.type_identity=="builtin:log-utf8-v1"&&c.operation=="log.write") { return true; }
        if(gateway::ros2_action::known(c)) { return true; }
        if(gateway::ros2_lifecycle::known(c)) { return true; }
        if(gateway::ros2_resources::known(c)) { return true; }
        if(gateway::ros2_custom_topics::known(c)) { return true; }
        if(gateway::ros2_custom_services::known(c)||gateway::ros2_custom_actions::known(c)) { return true; }
        return builtin_known_type(c);
    }
    gateway::ros2_compat::qos_type qos(capability const& c)
    { return gateway::ros2_qos::configured(c); }
    bool actual_qos(gateway::ros2_compat::qos_type const& actual,gateway::ros2_compat::qos_type const& expected)
    { return gateway::ros2_qos::equal(actual,expected); }
    std::vector<std::uint8_t> canonical(std_msgs::msg::String const& m)
    { std::vector<std::uint8_t> d; d.reserve(4+m.data.size());ipc::put(d,m.data.size(),4); d.insert(d.end(),m.data.begin(),m.data.end()); return d; }
    std::vector<std::uint8_t> canonical(std_msgs::msg::Bool const& m) { return {static_cast<std::uint8_t>(m.data?1:0)}; }
    std::vector<std::uint8_t> canonical(geometry_msgs::msg::Twist const& m)
    {
        std::vector<std::uint8_t> d; d.reserve(48);for(double v:{m.linear.x,m.linear.y,m.linear.z,m.angular.x,m.angular.y,m.angular.z}) { std::uint64_t bits{}; std::memcpy(&bits,&v,8); ipc::put(d,bits,8); } return d;
    }
    std::vector<std::uint8_t> canonical(sensor_msgs::msg::Image const& m)
    { return ipc::encode_image_view({0,static_cast<std::uint32_t>(m.header.stamp.sec),m.header.stamp.nanosec,m.height,m.width,m.step,m.header.frame_id,m.encoding,m.is_bigendian!=0,m.data.data(),m.data.size()}); }
    class ros2_adapter final:public gateway::adapter
    {
        std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> node_;
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
        std::shared_ptr<gateway::default_control_profile::selection const> control_profile_;
#endif
        struct removal_notification
        {
            std::weak_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> node;
            ~removal_notification() noexcept
            {
                // The Jazzy wait set owns entities while waiting. Notify only
                // after the adapter's resource members have released them.
                if(auto active=node.lock())
                {
                    try { gateway::ros2_compat::notify(active); }
                    catch(...) {} // Destruction also runs during context shutdown.
                }
            }
        } removal_notification_{node_};
        gateway::tf_adapter tf_{node_};
        gateway::ros2_action::adapter actions_{node_};
        gateway::ros2_custom_topics::adapter custom_topics_{node_};
        gateway::ros2_custom_services::adapter custom_services_{node_};
        gateway::ros2_service_availability::registry availability_clients_;
        gateway::native_rmw_publisher::registry owned_publishers_{node_};
        gateway::native_rmw_subscription::registry owned_subscriptions_{node_,&gateway::native_rmw_subscription::create_selected};
        // A topic alias has one receive route until final close; independent
        // source handles cannot bypass max-subscribers through legacy listen.
        std::map<std::string,bool> subscription_routes_;
        // A configured alias uses one publication route until final close.
        // This prevents legacy lazy activation bypassing the owned count.
        std::map<std::string,bool> publisher_routes_;
        gateway::ros2_custom_actions::adapter custom_actions_;
        std::map<std::string,std::shared_ptr<rclcpp::PublisherBase>> publishers_;
        std::map<std::string,std::shared_ptr<rclcpp::SubscriptionBase>> subscribers_;
        std::map<std::string,std::shared_ptr<rclcpp::ServiceBase>> services_;
        std::map<std::string,std::shared_ptr<gateway::service_state>> service_states_;
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
        std::shared_ptr<gateway::owned_service_profile::selection const> service_profile_;
        std::shared_ptr<gateway::native_rcl_graph_wait::domain> service_graph_;
        std::unique_ptr<gateway::native_rcl_service_registry::registry> owned_services_;
        std::map<std::string,bool> service_routes_;
        std::set<std::uint64_t> owned_service_sources_;
        bool service_reports_written_{};
        bool legacy_service_route(capability const& cap)
        {
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
            if(!service_profile_&&!control_profile_)return true;
#else
            if(!service_profile_)return true;
#endif
            return !service_routes_.emplace(cap.alias,false).first->second;
        }
        void report_owned_services() noexcept
        {
            if(service_reports_written_||!owned_services_)return;service_reports_written_=true;
            for(auto source:owned_service_sources_)
            {
                gateway::native_rcl_service_registry::diagnostics transport{};gateway::native_rcl_service::diagnostic native{};
                auto error=owned_services_->observe_after_quiescence(source,transport,native);
                gateway::native_io::fixed_text<1024> row;
                bool ok=row.append("rmw_owned_service_retire source=")&&row.number(source)&&row.append(" observation=")&&row.number(error)&&
                    row.append(" primary=")&&row.number(transport.first_error)&&row.append(" commit_attempted=")&&row.number(unsigned(transport.commit_attempted))&&
                    row.append(" commit=")&&row.number(transport.last_commit_error)&&row.append(" cancel_attempted=")&&row.number(unsigned(transport.cancel_attempted))&&
                    row.append(" cancel=")&&row.number(transport.last_cancel_error)&&row.append(" retire_attempted=")&&row.number(unsigned(transport.retire_attempted))&&
                    row.append(" retire=")&&row.number(transport.last_retire_error)&&row.append(" native_initialized=")&&row.number(unsigned(native.native_initialized))&&
                    row.append(" fini_attempted=")&&row.number(unsigned(native.fini_attempted))&&row.append(" fini_returned=")&&row.number(unsigned(native.fini_returned))&&
                    row.append(" fini_result=")&&row.number(native.fini_result)&&row.append("\n");
                if(ok)(void)gateway::native_io::write_audit_text(row.view());
                if(!error)for(unsigned i{};i<static_cast<unsigned>(gateway::native_rcl_service::operation::count);++i)
                {
                    gateway::native_io::fixed_text<192> call;
                    if(call.append("rmw_owned_service_native source=")&&call.number(source)&&call.append(" operation=")&&call.number(i)&&
                        call.append(" calls=")&&call.number(native.native[i].calls)&&call.append(" last_result=")&&call.number(native.native[i].last_native_result)&&call.append("\n"))
                        (void)gateway::native_io::write_audit_text(call.view());
                }
            }
        }
#endif
        template<typename S> ipc::call_result serve(capability const& c,std::shared_ptr<gateway::event_queue> events)
        {
            if constexpr(!gateway::ros2_qos::supports_service_qos<rclcpp::Service<S>>::value) { return {95,{}}; }
            auto state{std::make_shared<gateway::service_state>(c,std::move(events))};std::weak_ptr<gateway::service_state> weak{state};
            auto verified{std::make_shared<std::atomic<bool>>(false)};
            auto callback{[weak,verified](std::shared_ptr<rclcpp::Service<S>> service,std::shared_ptr<rmw_request_id_t> header,std::shared_ptr<typename S::Request> request)
            {
                if(!verified->load()) { return; }
                auto state{weak.lock()};if(!state) { return; }std::vector<std::uint8_t> bytes;
                if constexpr(std::is_same_v<S,std_srvs::srv::SetBool>) { bytes.push_back(request->data?1:0); }
                state->begin(std::move(bytes),[service,header](std::vector<std::uint8_t> const& bytes,ipc::deadline until)->std::uint32_t
                { typename S::Response response;response.success=bytes[0]!=0;response.message.assign(reinterpret_cast<char const*>(bytes.data()+5),bytes.size()-5);
                  if(std::chrono::steady_clock::now()>=until) { return 110; }service->send_response(*header,response);return 0; });
            }};
            auto configured{qos(c)};
            auto server{gateway::ros2_compat::create_deferred_service<S>(node_,c.resource,callback,configured)};
            if(!gateway::ros2_qos::service(*server,configured)) { return {95,{}}; }
            services_.emplace(c.alias,std::move(server));service_states_.emplace(c.alias,std::move(state));verified->store(true);return {};
        }
        template<typename M> ipc::call_result send(capability const& c,M const& m)
        {
            if constexpr(!gateway::ros2_qos::supports_entity_qos<rclcpp::Publisher<M>>::value) { return {95,{}}; }
            auto p{publishers_.find(c.alias)};
            if(p==publishers_.end())
            {
                auto configured{qos(c)};
                auto created{gateway::ros2_compat::create_publisher<M>(node_,c.resource,configured)};
                if(!gateway::ros2_qos::entity(*created,configured)) { return {95,{}}; }
                p=publishers_.emplace(c.alias,std::move(created)).first;
            }
            std::static_pointer_cast<rclcpp::Publisher<M>>(p->second)->publish(m); return {};
        }
        template<typename M> ipc::call_result listen(capability const& c,std::shared_ptr<gateway::event_queue> q)
        {
            if constexpr(!gateway::ros2_qos::supports_entity_qos<rclcpp::Subscription<M>>::value) { return {95,{}}; }
            std::weak_ptr<gateway::event_queue> weak{q};
            auto callback{[weak](std::shared_ptr<M const> message)
            {
                if(auto queue=weak.lock())
                {
                    auto maximum{gateway::limit(queue->capability,"max-message-bytes")};
                    if constexpr(std::is_same_v<M,std_msgs::msg::String>) { if(!gateway::wire_size_fits(maximum,{4,message->data.size()})) { return; } }
                    else if constexpr(std::is_same_v<M,sensor_msgs::msg::Image>) { if(!gateway::ros2_compat::native_bytes_accessible(message->data)||message->is_bigendian>1||message->header.frame_id.size()>255||message->encoding.size()>255||!gateway::wire_size_fits(maximum,{37,message->header.frame_id.size(),message->encoding.size(),message->data.size()})) { return; } }
                    else if constexpr(std::is_same_v<M,geometry_msgs::msg::Twist>) { if(maximum<48) { return; } }
                    else { if(!maximum) { return; } }
                    queue->enqueue(canonical(*message));
                }
            }};
            auto configured{qos(c)};
            auto created{gateway::ros2_compat::create_subscription<M>(node_,c.resource,configured,callback)};
            if(!gateway::ros2_qos::entity(*created,configured)) { return {95,{}}; }
            subscribers_.emplace(c.alias,std::move(created)); return {};
        }
        template<typename S> ipc::call_result invoke(capability const& c,std::shared_ptr<typename S::Request> request,std::uint32_t timeout)
        {
            if constexpr(!gateway::ros2_qos::supports_client_qos<rclcpp::Client<S>>::value) { return {95,{}}; }
            auto end{std::chrono::steady_clock::now()+std::chrono::milliseconds(timeout)};
            auto configured{qos(c)};
            auto client{gateway::ros2_compat::create_client<S>(node_,c.resource,configured)};
            if(!gateway::ros2_qos::client(*client,configured)) { return {95,{}}; }
            auto remaining{end-std::chrono::steady_clock::now()};
            if(remaining<=decltype(remaining)::zero()||!client->wait_for_service(remaining)) { return {110,{}}; }
            if(std::chrono::steady_clock::now()>=end) { return {110,{}}; }
            auto pending{client->async_send_request(request)};
            if(pending.wait_until(end)!=std::future_status::ready) { gateway::ros2_compat::remove_pending(*client,pending); return {110,{}}; }
            if(std::chrono::steady_clock::now()>=end) { return {110,{}}; }
            auto response{pending.get()};
            auto const& text{response->message};if(!gateway::wire_size_fits(gateway::limit(c,"max-response-bytes"),{5,text.size()})) { return {90,{}}; }if(!gateway::valid_utf8(reinterpret_cast<std::uint8_t const*>(text.data()),text.size())) { return {22,{}}; }
            std::vector<std::uint8_t> d;d.reserve(5+text.size());d.push_back(static_cast<std::uint8_t>(response->success?1:0));
            ipc::put(d,response->message.size(),4); d.insert(d.end(),response->message.begin(),response->message.end());
            if(std::chrono::steady_clock::now()>=end) { return {110,{}}; }
            if(d.size()>gateway::limit(c,"max-response-bytes")) { return {90,{}}; } return {0,std::move(d)};
        }
    public:
        explicit ros2_adapter(std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> n,std::shared_ptr<gateway::ros2_action::cleanup_pool> cleanup,
            std::shared_ptr<gateway::ros2_custom_actions::cleanup_pool> custom_cleanup
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
            ,std::shared_ptr<gateway::owned_service_profile::selection const> service_profile={},
            std::shared_ptr<gateway::native_rcl_graph_wait::domain> service_graph={}
#endif
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
            ,std::shared_ptr<gateway::default_control_profile::selection const> control_profile={}
#endif
            ):
            node_{std::move(n)}
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
            ,control_profile_{std::move(control_profile)}
#endif
            ,actions_{node_,std::move(cleanup)}
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
            ,owned_publishers_{node_,gateway::default_control_profile::publisher_factory(control_profile_)}
            ,owned_subscriptions_{node_,gateway::default_control_profile::subscription_factory(control_profile_)}
#endif
            ,custom_actions_{node_,std::move(custom_cleanup)}
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
            ,service_profile_{std::move(service_profile)},service_graph_{std::move(service_graph)}
#endif
        {
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
            if(control_profile_)
            {
                // No unrelated CPP10 warm-up is required by the complete
                // independently proved original control graph.
                auto old_registry=service_profile_?service_profile_->registry:std::shared_ptr<uwvm_rmw_cpp::Registry const>{};
                owned_services_=std::make_unique<gateway::native_rcl_service_registry::registry>(node_,std::move(old_registry),control_profile_->storage,
                    gateway::default_control_profile::service_factory(control_profile_,bool(service_profile_)),
                    gateway::default_control_profile::bounds_factory(control_profile_,bool(service_profile_)));
            }
            else
#endif
            if(service_profile_)owned_services_=std::make_unique<gateway::native_rcl_service_registry::registry>(node_,service_profile_->registry,service_profile_->storage,
                &gateway::owned_service_profile::create_selected,&gateway::owned_service_profile::canonical_bounds);
#endif
        }
        ~ros2_adapter() override
        {
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
            clear_owned_rmw_services();
#endif
            owned_subscriptions_.clear();owned_publishers_.clear();for(auto const& s:service_states_){s.second->deactivate();}}
        ipc::call_result publish(capability const& c,std::vector<std::uint8_t> const& d) override
        {
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
            if(gateway::default_control_profile::handles(c))return {95,{}};
#endif
            if(!known_type(c)) { return {13,{}}; }
            auto route{publisher_routes_.emplace(c.alias,false)};
            if(route.first->second){return {17,{}};}
            if(ipc::find_custom_topic(2,c.type)) { return custom_topics_.publish(c,d); }
            if(c.type=="std_msgs/msg/String") { std_msgs::msg::String m; m.data.assign(reinterpret_cast<char const*>(d.data()+4),d.size()-4); return send(c,m); }
            if(c.type=="std_msgs/msg/Bool") { std_msgs::msg::Bool m; m.data=d[0]!=0; return send(c,m); }
            if(c.type=="geometry_msgs/msg/Twist")
            {
                geometry_msgs::msg::Twist m; double* v[]={&m.linear.x,&m.linear.y,&m.linear.z,&m.angular.x,&m.angular.y,&m.angular.z};
                for(unsigned i{};i!=6;++i) { auto bits{ipc::get(d.data()+8*i,8)}; std::memcpy(v[i],&bits,8); } return send(c,m);
            }
            if(c.type=="sensor_msgs/msg/Image")
            {
                ipc::image_view i; if(!ipc::decode_image_view(d.data(),d.size(),2,i)) { return {22,{}}; }
                sensor_msgs::msg::Image m; m.header.stamp.sec=static_cast<std::int32_t>(i.seconds); m.header.stamp.nanosec=i.nanoseconds; m.header.frame_id=i.frame_id;
                m.height=i.height; m.width=i.width; m.encoding=i.encoding; m.is_bigendian=i.bigendian?1:0; m.step=i.step; gateway::ros2_compat::assign_bytes(m.data,i.data,i.data+i.data_size); return send(c,m);
            }
            return {95,{}};
        }
        ipc::call_result subscribe(capability const& c,std::shared_ptr<gateway::event_queue> q) override
        {
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
            if(gateway::default_control_profile::handles(c))return {95,{}};
#endif
            if(!known_type(c)) { return {13,{}}; }
            if(c.operation=="param.subscribe-events")
            {
                auto configured{qos(c)};std::shared_ptr<rclcpp::SubscriptionBase> created;
                auto result{gateway::ros2_resources::subscribe_events(node_,c,std::move(q),configured,created)};
                if(result.error) { return result; }
                if(!created||!gateway::ros2_qos::entity(*created,configured)) { return {95,{}}; }
                subscribers_.emplace(c.alias,std::move(created));return {};
            }
            if(c.operation=="topic.subscribe")
            {auto route=subscription_routes_.emplace(c.alias,false);if(route.first->second){return {17,{}};}}
            if(ipc::find_custom_topic(2,c.type)) { return custom_topics_.subscribe(c,std::move(q)); }
            if(c.type=="std_msgs/msg/String") { return listen<std_msgs::msg::String>(c,std::move(q)); }
            if(c.type=="std_msgs/msg/Bool") { return listen<std_msgs::msg::Bool>(c,std::move(q)); }
            if(c.type=="geometry_msgs/msg/Twist") { return listen<geometry_msgs::msg::Twist>(c,std::move(q)); }
            if(c.type=="sensor_msgs/msg/Image") { return listen<sensor_msgs::msg::Image>(c,std::move(q)); }
            return {95,{}};
        }
        ipc::call_result service(capability const& c,std::vector<std::uint8_t> const& d,std::uint32_t timeout) override
        {
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
            if(gateway::default_control_profile::handles(c))return {95,{}};
#endif
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
            if(!legacy_service_route(c))return {17,{}};
#endif
            if(ipc::find_custom_service(2,c.type)) { return custom_services_.call(c,d,timeout); }
            if(c.type=="std_srvs/srv/Trigger") { return invoke<std_srvs::srv::Trigger>(c,std::make_shared<std_srvs::srv::Trigger::Request>(),timeout); }
            if(c.type=="std_srvs/srv/SetBool") { auto s{std::make_shared<std_srvs::srv::SetBool::Request>()}; s->data=d[0]!=0; return invoke<std_srvs::srv::SetBool>(c,s,timeout); }
            return {95,{}};
        }
        ipc::call_result advertise(capability const& c,std::shared_ptr<gateway::event_queue> events) override
        {
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
            if(gateway::default_control_profile::handles(c))return {95,{}};
#endif
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
            if(!legacy_service_route(c))return {17,{}};
#endif
            if(ipc::find_custom_service(2,c.type)) { return custom_services_.advertise(c,std::move(events)); }
            for(auto const& s:service_states_) { if(s.second->capability.resource==c.resource) { return {17,{}}; } }
            if(c.type=="std_srvs/srv/Trigger") { return serve<std_srvs::srv::Trigger>(c,std::move(events)); }
            if(c.type=="std_srvs/srv/SetBool") { return serve<std_srvs::srv::SetBool>(c,std::move(events)); }return {95,{}};
        }
        ipc::call_result advertise_action(capability const& c,std::shared_ptr<gateway::event_queue> events) override
        { return ipc::find_custom_action(2,c.type)?custom_actions_.advertise(c,std::move(events)):actions_.advertise(c,std::move(events)); }
        ipc::call_result observe_action(capability const& c,std::uint64_t goal) override
        { return ipc::find_custom_action(2,c.type)?custom_actions_.observe(c,goal):actions_.observe(c,goal); }
        void close(std::string const& alias) override
        {
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
            service_routes_.erase(alias);
#endif
            // Tombstone receive ingress/borrow before releasing any alias route
            // or legacy local owner. Real registry cleanup keeps all outcomes.
            owned_subscriptions_.close_alias(alias);subscription_routes_.erase(alias);
            auto s{service_states_.find(alias)};if(s!=service_states_.end()){s->second->deactivate();service_states_.erase(s);}
            owned_publishers_.close_alias(alias);publisher_routes_.erase(alias);
            services_.erase(alias);publishers_.erase(alias);subscribers_.erase(alias);tf_.close(alias);actions_.close(alias);custom_actions_.close(alias);custom_services_.close(alias);custom_topics_.close(alias);
            // Dropping the map's shared_ptr alone leaves an idle subscription
            // alive in the executor's blocking wait set. Wake it so ownership
            // is released and expired entries are removed without new traffic.
            gateway::ros2_compat::notify(node_);
        }
        ipc::call_result owned_rmw_publisher(capability const& cap,ipc::rmw_endpoint::packet const& request,ipc::deadline until) override
        {
            if(request.operation==ipc::rmw_endpoint::command::activate)
            {
                auto route{publisher_routes_.emplace(cap.alias,true)};
                if(!route.first->second){return {17,{}};}
            }
            return owned_publishers_.control(cap,request,until);
        }
        ipc::call_result owned_rmw_publish(capability const& cap,ipc::rmw_publish::header const& request,
            std::uint8_t const* bytes,std::size_t count,ipc::deadline until) override
        {return owned_publishers_.publish(cap,request,bytes,count,until);}
        void clear_owned_rmw_publishers() noexcept override{owned_publishers_.clear();}
        ipc::call_result owned_rmw_subscription(capability const& cap,ipc::rmw_endpoint::packet const& request,ipc::deadline until) override
        {
            if(request.operation==ipc::rmw_endpoint::command::activate)
            {auto route=subscription_routes_.emplace(cap.alias,true);if(!route.first->second){return {17,{}};}}
            return owned_subscriptions_.control(cap,request,until);
        }
        ipc::call_result owned_rmw_subscription_transfer(capability const& cap,
            ipc::rmw_subscription::control const& request,ipc::deadline until) override
        {return owned_subscriptions_.transfer(cap,request,until);}
        std::uint32_t owned_rmw_subscription_delivery_failed(capability const& cap,
            ipc::rmw_endpoint::binding const& owner,std::uint64_t incarnation,std::uint32_t reason) noexcept override
        {return owned_subscriptions_.delivery_failed(cap,owner,incarnation,reason);}
        void clear_owned_rmw_subscriptions() noexcept override{owned_subscriptions_.clear();}
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
        std::uint32_t validate_owned_rmw_service_body(capability const& cap,bool response,
            std::uint8_t const* bytes,std::size_t size)noexcept override
        {
            if(!owned_services_)return 95;
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
            if(gateway::default_control_profile::handles(cap))return control_profile_?
                gateway::default_control_gateway::validate_body(control_profile_->proof,cap,response,bytes,size):95u;
#endif
            if(!service_profile_)return 95;
            if(cap.type=="std_srvs/srv/Empty"||cap.type=="example_interfaces/srv/AddTwoInts")
                return gateway::owned_service_body(cap,response,bytes,size)?0u:22u;
#if UWVM_ROS2_OWNED_CUSTOM_RCL_SERVICE_PROFILE
            return gateway::custom_owned_services::validate_body(cap,response,bytes,size);
#else
            return 95;
#endif
        }
        std::size_t owned_rmw_service_capacity() const noexcept override
        {
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
            if(control_profile_)return control_profile_->storage.maximum_entries;
#endif
            return service_profile_?service_profile_->storage.maximum_entries:0;
        }
        ipc::call_result owned_rmw_service(gateway::owned_service_admission const& a,
            ipc::rmw_service::control const& request,std::uint8_t const* bytes,std::size_t size,ipc::deadline until) override
        {
            if(!owned_services_||!a.source||!a.paired)return {95,{}};
            if(request.operation==ipc::rmw_service::command::activate)
            {
                // Claim BOTH original aliases and diagnostics storage BEFORE
                // any original RCL creation. Failure leaves a finite refusal,
                // never a fallback to the old transient-client route.
                auto source=service_routes_.emplace(a.source->alias,true);
                auto paired=service_routes_.emplace(a.paired->alias,true);
                if(!source.first->second||!paired.first->second)return {17,{}};
                owned_service_sources_.insert(request.owner.source);
            }
            gateway::native_rcl_service_registry::admitted real{a.source,a.paired,a.paired_owner,a.availability,a.availability_owner};
            real.waiting={service_graph_.get(),{a.session_fd,a.worker_pidfd,a.stopping}};
            return owned_services_->execute(real,request,bytes,size,until);
        }
        std::uint32_t owned_rmw_service_delivery_failed(ipc::rmw_endpoint::binding const& owner,
            std::uint64_t incarnation,std::uint32_t reason) noexcept override
        {return owned_services_?owned_services_->delivery_failed(owner,incarnation,reason):95;}
        void close_owned_rmw_service(ipc::rmw_endpoint::binding const& owner,std::uint64_t handle,
            std::string_view alias,bool revoke,std::uint32_t reason) noexcept override
        {if(owned_services_)owned_services_->close_owner(owner,handle,alias,revoke,reason);}
        void clear_owned_rmw_services() noexcept override
        {if(owned_services_){owned_services_->clear();report_owned_services();}}
#endif
        ipc::call_result service_availability(capability const& availability,capability const& source,
            ipc::client_availability::packet const& request,std::uint32_t timeout) override
        {
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
            if(gateway::default_control_profile::handles(source))return {95,{}};
#endif
            if(!legacy_service_route(source))return {17,{}};
#endif
            // Dispatch the ORIGINAL independently granted service.call contract
            // through existing native identity/complete custom schema checks.
            auto qualify=[](capability const& selected){return known_type(selected);};
            auto finish=[&](gateway::ros2_service_availability::result value)->ipc::call_result
            {
                if(value.error) { return {value.error,{}}; }
                auto response{request};response.value=value.value;auto raw{ipc::client_availability::encode(response)};
                return {0,{raw.begin(),raw.end()}};
            };
            if(source.type=="std_srvs/srv/Trigger") { return finish(availability_clients_.observe<std_srvs::srv::Trigger>(node_,availability,source,request,timeout,qualify)); }
            if(source.type=="std_srvs/srv/SetBool") { return finish(availability_clients_.observe<std_srvs::srv::SetBool>(node_,availability,source,request,timeout,qualify)); }
#define UWVM_CUSTOM_AVAILABILITY(Native,Type,Identity,Canonical,Request,Response) \
            if(source.type==Type){return finish(availability_clients_.observe<Native>(node_,availability,source,request,timeout,qualify));}
            UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(UWVM_CUSTOM_AVAILABILITY)
#undef UWVM_CUSTOM_AVAILABILITY
            return {95,{}};
        }
        void retire_service_availability(ipc::client_availability::context const& binding,
            ipc::client_availability::retirement scope,std::string_view availability,std::string_view source) noexcept override
        {
            availability_clients_.retire(binding,scope,availability,source);
            // Executor wait sets can temporarily retain a shared native client.
            // Wake the existing executor after releasing registry ownership.
            try { gateway::ros2_compat::notify(node_); } catch(...) {}
        }
        void clear_service_availability() noexcept override
        {
            // Abandon query clients BEFORE waiting for unrelated async Service
            // tasks that may still retain this shared adapter after IPC loss.
            availability_clients_.clear();
            try { gateway::ros2_compat::notify(node_); } catch(...) {}
        }
        ipc::call_result operation(capability const& c,std::vector<std::uint8_t> const& d,std::uint32_t timeout) override
        {
            auto started{std::chrono::steady_clock::now()};
            if(c.operation=="tf.lookup"||c.operation=="tf.broadcast") { return tf_.call(c,d,timeout); }
            if(c.operation.rfind("action.",0)==0) { return ipc::find_custom_action(2,c.type)?custom_actions_.operation(c,d,timeout):actions_.operation(c,d,timeout); }
            if(c.operation.rfind("lifecycle.",0)==0) { return gateway::ros2_lifecycle::call(node_,c,d,timeout); }
            if(c.operation=="service.respond")
            {
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
                if(gateway::default_control_profile::handles(c))return {95,{}};
#endif
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
                if(!legacy_service_route(c))return {17,{}};
#endif
                if(ipc::find_custom_service(2,c.type)) { return custom_services_.respond(c,d,timeout); }
                auto until{started+std::chrono::milliseconds(std::min<std::uint64_t>(timeout,gateway::limit(c,"timeout-ms")))};
                for(auto const& s:service_states_)
                {
                    if(s.second->capability.resource!=c.resource) { continue; }
                    auto configured{qos(c)};
                    if(!actual_qos(qos(s.second->capability),configured)) { return {13,{}}; }
                    auto server{services_.find(s.first)};
                    if(server==services_.end()||!gateway::ros2_qos::service(*server->second,configured)) { return {95,{}}; }
                    return s.second->answer(c,d,until);
                }
                return {9,{}};
            }
            if(c.operation=="time.read-ros"||c.operation=="time.read-system"||c.operation=="time.read-steady")
            {
                std::int64_t ns{};
                if(c.operation=="time.read-ros") { ns=node_->get_clock()->now().nanoseconds(); }
                else if(c.operation=="time.read-system") { ns=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); }
                else { ns=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(); }
                if(ns<0) { return {22,{}}; } std::vector<std::uint8_t> bytes; ipc::put(bytes,static_cast<std::uint64_t>(ns),8); return {0,std::move(bytes)};
            }
            if(c.operation=="log.write")
            {
                std::string message{reinterpret_cast<char const*>(d.data()+4),d.size()-4}; auto severity{gateway::field(c,"severity")}; auto logger{node_->get_logger()};
                if(severity=="debug") { RCLCPP_DEBUG(logger,"%s",message.c_str()); }
                else if(severity=="info") { RCLCPP_INFO(logger,"%s",message.c_str()); }
                else if(severity=="warn") { RCLCPP_WARN(logger,"%s",message.c_str()); }
                else if(severity=="error") { RCLCPP_ERROR(logger,"%s",message.c_str()); }
                else if(severity=="fatal") { RCLCPP_FATAL(logger,"%s",message.c_str()); }
                else { return {22,{}}; } return {};
            }
            return gateway::ros2_resources::call(node_,c,d,timeout);
        }
    };
    void credential(std::string const& path,bool secret=false)
    {
        struct stat s{};
        if(::lstat(path.c_str(),&s)<0||!S_ISREG(s.st_mode)||s.st_uid!=::geteuid()||(s.st_mode&0022)||(secret&&(s.st_mode&0077))) { throw std::runtime_error("missing or insecure DDS-Security credential "+path); }
    }
}
int main(int argc,char** argv)
{
    try
    {
        char const* owned_profile_path{};char const* control_profile_path{};
        if(argc<2||(argc-2)%2)throw std::runtime_error("usage: uwvm-ros2-gateway <trusted-config> [--owned-rmw-service-profile <trusted-file>] [--default-rmw-control-profile <trusted-file>]");
        for(int i=2;i<argc;i+=2)
        {
            std::string_view option{argv[i]};
            if(option=="--owned-rmw-service-profile"&&!owned_profile_path)owned_profile_path=argv[i+1];
            else if(option=="--default-rmw-control-profile"&&!control_profile_path)control_profile_path=argv[i+1];
            else throw std::runtime_error("Unknown or duplicate explicit Gateway profile selection");
        }
#if !UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
        if(owned_profile_path)throw std::runtime_error("UNSUPPORTED: explicit original RCL service profile was not built");
#endif
#if !UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
        if(control_profile_path)throw std::runtime_error("UNSUPPORTED: complete original control profile was not built");
#endif
#if !UWVM_ROS2_HAVE_QUIET_NODE
        throw std::runtime_error("UNSUPPORTED: selected SDK cannot disable implicit Node publishers/services");
#else
        auto c{gateway::load(argv[1])}; if(c.policy.ros_version!=uwvm2::imported::ros::version::ros2) { throw std::runtime_error("wrong backend"); }
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
        std::shared_ptr<gateway::owned_service_profile::selection const> service_profile;
        if(owned_profile_path)
        {
            auto selected=std::make_shared<gateway::owned_service_profile::selection>(gateway::owned_service_profile::load(owned_profile_path));
            gateway::owned_service_profile::warm(*selected);service_profile=std::move(selected);
        }
#endif
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
        std::shared_ptr<gateway::default_control_profile::selection const> control_profile;
        if(control_profile_path)
        {
            auto selected=std::make_shared<gateway::default_control_profile::selection>(gateway::default_control_profile::load(control_profile_path));
            gateway::default_control_profile::warm(*selected);control_profile=std::move(selected);
            // A single registry owns both selected service families. Refuse
            // conflicting storage choices rather than silently enlarging one.
            if(service_profile&&(service_profile->storage.maximum_entries!=control_profile->storage.maximum_entries||
                service_profile->storage.history_per_endpoint!=control_profile->storage.history_per_endpoint))
                throw std::runtime_error("Combined original profiles require identical explicit service storage limits");
        }
#endif
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
            bool known{};
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
            if(gateway::default_control_profile::handles(*selected))
                known=control_profile&&gateway::default_control_gateway::known(control_profile->proof,*selected);
            else
#endif
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE && UWVM_ROS2_OWNED_CUSTOM_RCL_SERVICE_PROFILE
            if(service_profile&&selected->limits.count("adapter-codec-identity")&&
                (selected->operation=="service.call"||selected->operation=="service.advertise"||selected->operation=="service.respond"))
                known=gateway::owned_service_profile::known(*service_profile,*selected);
            else
#endif
            {
                known=known_type(*selected);
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
                if(!known&&service_profile)known=gateway::owned_service_profile::known(*service_profile,*selected);
#endif
            }
            if(!known) { throw std::runtime_error("UNSUPPORTED or mismatched generated ROS2 adapter: "+selected->type); }
        }
        std::set<std::string> allowed{"node-name","namespace","domain-id","rmw-implementation","security-mode","keystore","enclave"};
        if(c.policy.connection.size()!=allowed.size()) { throw std::runtime_error("all ROS2 connection fields must be explicit"); }
        for(auto const& p:c.policy.connection) { if(!allowed.count(p.first)) { throw std::runtime_error("unknown ROS2 connection field"); } }
        auto const& connection{c.policy.connection};
        if(!gateway::canonical(connection.at("namespace"),true)||!ipc::valid_token(connection.at("node-name"))) { throw std::runtime_error("invalid connection identity"); }
        auto domain{gateway::integer(connection.at("domain-id"),232)};
        auto rmw{connection.at("rmw-implementation")}; if(rmw!="rmw_fastrtps_cpp"&&rmw!="rmw_cyclonedds_cpp") { throw std::runtime_error("RMW has no qualified security adapter"); }
        gateway::ros2_environment::clear_enclave_override();
        gateway::native_io::fixed_text<8> domain_text;
        if(!domain_text.number(domain)||!domain_text.append(std::string_view{"\0",1})) { throw std::runtime_error("domain identifier formatting failed"); }
        gateway::ros2_environment::set("RMW_IMPLEMENTATION",rmw.c_str()); gateway::ros2_environment::set("ROS_DOMAIN_ID",domain_text.view().data());
        auto mode{connection.at("security-mode")};
        std::vector<std::string> args{"uwvm-ros2-gateway"};
        if(mode=="enforce")
        {
            auto keystore{connection.at("keystore")},enclave{connection.at("enclave")};
            if(keystore.empty()||keystore[0]!='/'||!gateway::canonical(enclave,true)) { throw std::runtime_error("explicit keystore/enclave required"); }
            auto directory{keystore+"/enclaves"+enclave};
            for(auto name:{"identity_ca.cert.pem","cert.pem","permissions_ca.cert.pem","governance.p7s","permissions.p7s"}) { credential(directory+"/"+name); }
            credential(directory+"/key.pem",true);
            gateway::ros2_environment::set("ROS_SECURITY_ENABLE","true"); gateway::ros2_environment::set("ROS_SECURITY_STRATEGY","Enforce");
#if UWVM_ROS2_HAVE_ENCLAVE_ARGUMENT
            gateway::ros2_environment::set("ROS_SECURITY_KEYSTORE",keystore.c_str());
            args.insert(args.end(),{"--ros-args","--enclave",enclave});
#else
            // Older rcl selects a node directory. Pin the exact already checked
            // directory; never allow its implicit longest-prefix search.
            gateway::ros2_environment::set("ROS_SECURITY_ROOT_DIRECTORY",keystore.c_str());
            gateway::ros2_environment::set("ROS_SECURITY_NODE_DIRECTORY",directory.c_str());
#endif
        }
        else if(mode=="disabled-test"&&connection.at("keystore")=="not-applicable"&&connection.at("enclave")=="not-applicable")
        { gateway::ros2_environment::set("ROS_SECURITY_ENABLE","false"); gateway::ros2_environment::clear("ROS_SECURITY_STRATEGY"); gateway::ros2_environment::clear("ROS_SECURITY_KEYSTORE"); }
        else { throw std::runtime_error("invalid explicit security mode"); }
        std::vector<char const*> raw; for(auto const& a:args) { raw.push_back(a.c_str()); }
        auto context{std::make_shared<rclcpp::Context>()}; context->init(static_cast<int>(raw.size()),raw.data());
        if(rmw!=rmw_get_implementation_identifier()) { throw std::runtime_error("loaded RMW differs from explicit configuration"); }
        rclcpp::NodeOptions options; options.context(context); options.use_global_arguments(true); options.enable_rosout(false); options.start_parameter_services(false); options.start_parameter_event_publisher(false);options.parameter_overrides({rclcpp::Parameter("start_type_description_service",false)});
        auto node{std::make_shared<::uwvm2::ros_gateway::ros2_compat::node_type>(connection.at("node-name"),connection.at("namespace"),options)};
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
        // Allocating the domain registers NO event or listener thread. Only
        // a later authorized positive wait may prepare its one real loan.
        bool selected_owned_service=bool(service_profile);
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
        selected_owned_service=selected_owned_service||bool(control_profile);
#endif
        auto service_graph=selected_owned_service?std::make_shared<gateway::native_rcl_graph_wait::domain>(node):nullptr;
        auto report_service_graph=[&]()noexcept
        {
            if(!service_graph)return;auto facts=service_graph->observe();gateway::native_io::fixed_text<384> line;
            if(line.append("rmw_owned_service_graph_domain registrations=")&&line.number(facts.registration_attempts)&&
                line.append(" registered=")&&line.number(facts.registration_completed)&&line.append(" waits=")&&line.number(facts.wait_attempts)&&
                line.append(" completed=")&&line.number(facts.wait_completed)&&line.append(" first_error=")&&line.number(facts.first_error)&&
                line.append(" poisoned=")&&line.number(unsigned(facts.poisoned))&&line.append("\n"))
                (void)gateway::native_io::write_audit_text(line.view());
        };
#endif
        rclcpp::ExecutorOptions executor_options; executor_options.context=context;
        rclcpp::executors::SingleThreadedExecutor executor{executor_options}; executor.add_node(node->get_node_base_interface());
        // Derive cleanup capacity only from explicitly configured session and
        // Action send quotas. It grants no operation or additional resource.
        std::uint64_t pending_per_session{},custom_pending_per_session{};
        for(auto const& item:c.policy.capabilities)
        {
            if(item.second.operation!="action.send-goal") { continue; }
            auto maximum{gateway::limit(item.second,"max-active-goals")};
            auto& selected{ipc::find_custom_action(2,item.second.type)?custom_pending_per_session:pending_per_session};
            if(maximum>std::numeric_limits<std::uint64_t>::max()-selected) { throw std::runtime_error("Action cleanup quota sum overflow"); }
            selected+=maximum;
        }
        if(pending_per_session>std::numeric_limits<std::uint64_t>::max()/c.max_sessions||custom_pending_per_session>std::numeric_limits<std::uint64_t>::max()/c.max_sessions) { throw std::runtime_error("Action cleanup session quota overflow"); }
        auto cleanup{std::make_shared<gateway::ros2_action::cleanup_pool>(pending_per_session*c.max_sessions)};
        auto custom_cleanup{std::make_shared<gateway::ros2_custom_actions::cleanup_pool>(custom_pending_per_session*c.max_sessions)};
        std::thread spinner{[&] { executor.spin(); }};
        int rc{};
        try { rc=gateway::run(c,[node,cleanup,custom_cleanup
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
            ,service_profile,service_graph
#endif
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
            ,control_profile
#endif
            ] { return std::make_unique<ros2_adapter>(node,cleanup,custom_cleanup
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
                ,service_profile,service_graph
#endif
#if UWVM_ROS2_DEFAULT_RMW_CONTROL_PROFILE
                ,control_profile
#endif
                ); }); }
        catch(...)
        {
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
            report_service_graph();
#endif
            executor.cancel(); spinner.join(); context->shutdown("gateway exit"); throw;
        }
#if UWVM_ROS2_OWNED_RCL_SERVICE_PROFILE
        report_service_graph(); // actual run/session owners have quiesced
#endif
        executor.cancel(); spinner.join(); context->shutdown("gateway exit"); return rc;
#endif
    }
    catch(std::exception const& e) { gateway::native_io::diagnostic("BLOCKED ",e.what()); return 1; }
}
