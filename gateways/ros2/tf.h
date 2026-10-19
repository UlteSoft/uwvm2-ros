/*************************************************************
 * UWVM2 ROS2 frame-filtered TF adapter. APL-2.0.              *
 *************************************************************/
#pragma once
#include "compat/node.h"
#include "../common/tf_state.h"
#include "qos.h"
#include <rclcpp/rclcpp.hpp>
#if UWVM_ROS2_HAVE_TF
#if __has_include(<tf2/buffer_core.hpp>)
#include <tf2/buffer_core.hpp>
#else
#include <tf2/buffer_core.h>
#endif
#include <tf2_msgs/msg/tf_message.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>

namespace uwvm2::ros_gateway
{
    class tf_adapter
    {
        struct state
        {
            tf_ingress ingress;
            tf2::BufferCore buffer;
            rclcpp::Subscription<tf2_msgs::msg::TFMessage>::SharedPtr subscriber;
            explicit state(ros::capability_spec c):ingress{std::move(c)},buffer{std::chrono::milliseconds(limit(ingress.capability,"buffer-cache-ms"))} {}
        };
        ::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node;
        std::map<std::string,std::shared_ptr<state>> states;
        std::map<std::string,rclcpp::Publisher<tf2_msgs::msg::TFMessage>::SharedPtr> publishers;
        static ros2_compat::qos_type qos(ros::capability_spec const& cap) { return ros2_qos::configured(cap); }
        static bool actual_qos(ros2_compat::qos_type const& actual,ros2_compat::qos_type const& expected)
        { return ros2_qos::equal(actual,expected); }
        static ipc::transform_data wire(geometry_msgs::msg::TransformStamped const& m)
        { return {0,static_cast<std::uint32_t>(m.header.stamp.sec),m.header.stamp.nanosec,m.header.frame_id,m.child_frame_id,{{m.transform.translation.x,m.transform.translation.y,m.transform.translation.z,m.transform.rotation.x,m.transform.rotation.y,m.transform.rotation.z,m.transform.rotation.w}}}; }
        static geometry_msgs::msg::TransformStamped message(ipc::transform_data const& t)
        {
            geometry_msgs::msg::TransformStamped m;
            m.header.stamp.sec=static_cast<std::int32_t>(t.seconds); m.header.stamp.nanosec=t.nanoseconds; m.header.frame_id=t.parent; m.child_frame_id=t.child;
            m.transform.translation.x=t.values[0]; m.transform.translation.y=t.values[1]; m.transform.translation.z=t.values[2];
            m.transform.rotation.x=t.values[3]; m.transform.rotation.y=t.values[4]; m.transform.rotation.z=t.values[5]; m.transform.rotation.w=t.values[6]; return m;
        }
        std::shared_ptr<state> listener(ros::capability_spec const& c,ipc::deadline until)
        {
            auto found{states.find(c.alias)}; if(found!=states.end()) { return found->second; }
            if(std::chrono::steady_clock::now()>=until) { return {}; }
            auto next{std::make_shared<state>(c)}; std::weak_ptr<state> weak{next};
            auto configured{qos(c)};
            if(std::chrono::steady_clock::now()>=until) { return {}; }
            next->subscriber=ros2_compat::create_subscription<tf2_msgs::msg::TFMessage>(node,c.resource,configured,[weak](tf2_msgs::msg::TFMessage::ConstSharedPtr batch)
            {
                if(auto active=weak.lock())
                {
                    std::lock_guard<std::mutex> lock{active->ingress.mutex};
                    if(batch->transforms.size()>limit(active->ingress.capability,"max-message-bytes")/78) { return; }
                    for(auto const& m:batch->transforms)
                    {
                        if(m.header.frame_id!=field(active->ingress.capability,"target-frame")||m.child_frame_id!=field(active->ingress.capability,"source-frame")) { continue; }
                        if(!wire_size_fits(limit(active->ingress.capability,"max-message-bytes"),{76,m.header.frame_id.size(),m.child_frame_id.size()})) { continue; }
                        auto t{wire(m)};
                        if(!active->ingress.admit(t)||!active->ingress.retain(t)) { continue; }
                        active->buffer.clear();
                        for(auto const& retained:active->ingress.history)
                        { active->buffer.setTransform(message(retained),"uwvm-authorized-tf",active->ingress.capability.resource=="/tf_static"); }
                    }
                }
            });
            if(!ros2_qos::entity(*next->subscriber,configured)||std::chrono::steady_clock::now()>=until)
            {
                next->subscriber.reset();
                // An executor may already own the newly created entity in its
                // wait set. Wake it after dropping the rejected subscription.
                ros2_compat::notify(node);
                return {};
            }
            states.emplace(c.alias,next); return next;
        }
    public:
        explicit tf_adapter(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr n):node{std::move(n)} {}
        void close(std::string const& alias) { states.erase(alias); publishers.erase(alias); }
        ipc::call_result call(ros::capability_spec const& c,std::vector<std::uint8_t> const& payload,std::uint32_t timeout)
        {
            auto until{std::chrono::steady_clock::now()+std::chrono::milliseconds(std::min<std::uint64_t>(timeout,limit(c,"timeout-ms")))};
            if(c.resource!="/tf"&&c.resource!="/tf_static") { return {13,{}}; }
            if(c.operation=="tf.broadcast")
            {
                ipc::transform_data t;
                if(!ipc::decode_transform(payload,t)||t.sequence) { return {22,{}}; }
                if(auto error{transform_permission(c,t)}) { return {error,{}}; }
                if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }
                auto found{publishers.find(c.alias)};
                if(found==publishers.end()) { auto configured{qos(c)};if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }auto created{ros2_compat::create_publisher<tf2_msgs::msg::TFMessage>(node,c.resource,configured)};if(!ros2_qos::entity(*created,configured)) { return {95,{}}; }if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }found=publishers.emplace(c.alias,std::move(created)).first; }
                tf2_msgs::msg::TFMessage batch; batch.transforms.push_back(message(t));
                if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }
                found->second->publish(batch);
                return std::chrono::steady_clock::now()>=until?ipc::call_result{110,{}}:ipc::call_result{};
            }
            if(c.operation!="tf.lookup"||payload.size()!=8) { return {22,{}}; }
            auto nanoseconds{ipc::get(payload.data(),8)};
            if(nanoseconds>2147483647999999999ULL) { return {22,{}}; }
            if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }
            auto active{listener(c,until)};if(!active) { return {std::chrono::steady_clock::now()>=until?110u:95u,{}}; }tf2::TimePoint when{std::chrono::nanoseconds(nanoseconds)};
            while(std::chrono::steady_clock::now()<until)
            {
                {
                    std::lock_guard<std::mutex> lock{active->ingress.mutex};
                    if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }
                    try
                    {
                        if(active->buffer.canTransform(field(c,"target-frame"),field(c,"source-frame"),when))
                        { auto data{ipc::encode_transform(wire(active->buffer.lookupTransform(field(c,"target-frame"),field(c,"source-frame"),when)))};if(std::chrono::steady_clock::now()>=until) { return {110,{}}; } if(data.size()>limit(c,"max-message-bytes")) { return {90,{}}; } return {0,std::move(data)}; }
                    }
                    catch(tf2::TransformException const&) { return {22,{}}; }
                }
                auto remaining{until-std::chrono::steady_clock::now()};
                if(remaining>decltype(remaining)::zero()) { std::this_thread::sleep_for(std::min(remaining,std::chrono::duration_cast<decltype(remaining)>(std::chrono::milliseconds(5)))); }
            }
            return {110,{}};
        }
    };
}

#else
namespace uwvm2::ros_gateway
{
    class tf_adapter
    {
    public:
        explicit tf_adapter(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr) {}
        void close(std::string const&) noexcept {}
        ipc::call_result call(ros::capability_spec const&,std::vector<std::uint8_t> const&,std::uint32_t) { return {95,{}}; }
    };
}
#endif
