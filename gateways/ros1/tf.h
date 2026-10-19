/*************************************************************
 * UWVM2 ROS1 frame-filtered TF adapter. APL-2.0.              *
 *************************************************************/
#pragma once
#include "../common/tf_state.h"
#include <ros/ros.h>
#include "tf_compat.h"
#include <tf2_msgs/TFMessage.h>
#include <geometry_msgs/TransformStamped.h>

namespace uwvm2::ros_gateway
{
    class tf_adapter
    {
        struct state
        {
            tf_ingress ingress;
            ros1_compat::tf_buffer buffer;
            ::ros::Subscriber subscriber;
            // Groovy defaults Buffer's debug argument to true, which can
            // advertise a view_frames service. Always pass false explicitly:
            // selecting a cache is not authority to create a debug endpoint.
            explicit state(ros::capability_spec c):ingress{std::move(c)},buffer{::ros::Duration(static_cast<double>(limit(ingress.capability,"buffer-cache-ms"))/1000.),false} {}
        };
        ::ros::NodeHandle& node;
        std::map<std::string,std::shared_ptr<state>> states;
        std::map<std::string,::ros::Publisher> publishers;
        static ipc::transform_data wire(geometry_msgs::TransformStamped const& m)
        { return {m.header.seq,m.header.stamp.sec,m.header.stamp.nsec,m.header.frame_id,m.child_frame_id,{{m.transform.translation.x,m.transform.translation.y,m.transform.translation.z,m.transform.rotation.x,m.transform.rotation.y,m.transform.rotation.z,m.transform.rotation.w}}}; }
        static geometry_msgs::TransformStamped message(ipc::transform_data const& t)
        {
            geometry_msgs::TransformStamped m;
            m.header.seq=t.sequence; m.header.stamp=::ros::Time{t.seconds,t.nanoseconds}; m.header.frame_id=t.parent; m.child_frame_id=t.child;
            m.transform.translation.x=t.values[0]; m.transform.translation.y=t.values[1]; m.transform.translation.z=t.values[2];
            m.transform.rotation.x=t.values[3]; m.transform.rotation.y=t.values[4]; m.transform.rotation.z=t.values[5]; m.transform.rotation.w=t.values[6]; return m;
        }
        std::shared_ptr<state> listener(ros::capability_spec const& c,ipc::deadline until)
        {
            auto found{states.find(c.alias)}; if(found!=states.end()) { return found->second; }
            if(std::chrono::steady_clock::now()>=until) { return {}; }
            auto next{std::make_shared<state>(c)}; std::weak_ptr<state> weak{next};
            if(std::chrono::steady_clock::now()>=until) { return {}; }
            next->subscriber=node.subscribe<tf2_msgs::TFMessage>(c.resource,static_cast<std::uint32_t>(limit(c,"queue-depth")),[weak](tf2_msgs::TFMessage::ConstPtr const& batch)
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
                        // Rebuild from the explicit count/byte bounded history. No unauthorized frame enters tf2.
                        active->buffer.clear();
                        for(auto const& retained:active->ingress.history)
                        { active->buffer.setTransform(message(retained),"uwvm-authorized-tf",active->ingress.capability.resource=="/tf_static"); }
                    }
                }
            },::ros::VoidConstPtr{},::ros::TransportHints{}.tcp());
            if(std::chrono::steady_clock::now()>=until) { return {}; }
            states.emplace(c.alias,next); return next;
        }
    public:
        explicit tf_adapter(::ros::NodeHandle& n):node{n} {}
        void close(std::string const& alias) { states.erase(alias); publishers.erase(alias); }
        ipc::call_result call(ros::capability_spec const& c,std::vector<std::uint8_t> const& payload,std::uint32_t timeout)
        {
            auto until{std::chrono::steady_clock::now()+std::chrono::milliseconds(std::min<std::uint64_t>(timeout,limit(c,"timeout-ms")))};
            if(c.resource!="/tf"&&c.resource!="/tf_static") { return {13,{}}; }
            if(c.operation=="tf.broadcast")
            {
                ipc::transform_data t;
                if(!ipc::decode_transform(payload,t)) { return {22,{}}; }
                if(auto error{transform_permission(c,t)}) { return {error,{}}; }
                if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }
                auto found{publishers.find(c.alias)};
                if(found==publishers.end()) { auto created{node.advertise<tf2_msgs::TFMessage>(c.resource,static_cast<std::uint32_t>(limit(c,"queue-depth")),field(c,"latch")=="true")};if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }found=publishers.emplace(c.alias,std::move(created)).first; }
                tf2_msgs::TFMessage batch; batch.transforms.push_back(message(t));
                if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }
                found->second.publish(batch);
                return std::chrono::steady_clock::now()>=until?ipc::call_result{110,{}}:ipc::call_result{};
            }
            if(c.operation!="tf.lookup"||payload.size()!=8) { return {22,{}}; }
            auto nanoseconds{ipc::get(payload.data(),8)};
            if(nanoseconds>2147483647999999999ULL) { return {22,{}}; }
            if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }
            auto active{listener(c,until)};if(!active) { return {110,{}}; } ::ros::Time when{static_cast<std::uint32_t>(nanoseconds/1000000000ULL),static_cast<std::uint32_t>(nanoseconds%1000000000ULL)};
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
