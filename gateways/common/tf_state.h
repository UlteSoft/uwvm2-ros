/*************************************************************
 * UWVM2 filtered, bounded TF ingress. APL-2.0.               *
 *************************************************************/
#pragma once
#include "config.h"
#include <uwvm2/imported/ros/ipc/transform.h>
#include <mutex>
#include <deque>
#include <thread>
#include <cstring>

namespace uwvm2::ros_gateway
{
    inline std::uint32_t transform_permission(ros::capability_spec const& cap,ipc::transform_data const& transform)
    {
        if(!ipc::valid_transform(transform)) { return 22; }
        if(transform.parent!=field(cap,"target-frame")||transform.child!=field(cap,"source-frame")) { return 13; }
        auto bound{positive(field(cap,"tf.max-translation-abs"))},epsilon{positive(field(cap,"tf.quaternion-norm-tolerance"))};
        if(epsilon>0.001) { return 22; }
        for(unsigned i{};i!=3;++i) { if(std::abs(transform.values[i])>bound) { return 13; } }
        double norm{};for(unsigned i=3;i!=7;++i) { norm+=transform.values[i]*transform.values[i]; }
        return std::abs(norm-1.)<=epsilon?0:22;
    }
    struct tf_ingress
    {
        ros::capability_spec capability;
        std::mutex mutex;
        std::deque<ipc::transform_data> history{};
        std::uint64_t history_bytes{};
        double tokens,bytes;
        std::chrono::steady_clock::time_point updated{std::chrono::steady_clock::now()};
        explicit tf_ingress(ros::capability_spec c):capability{std::move(c)},tokens{static_cast<double>(limit(capability,"burst"))},bytes{static_cast<double>(limit(capability,"max-bytes-per-second"))} {}
        // Caller holds mutex through admission and the corresponding buffer update.
        bool admit(ipc::transform_data const& transform)
        {
            if(transform_permission(capability,transform)) { return false; }
            auto size{ipc::encode_transform(transform).size()};
            if(size>limit(capability,"max-message-bytes")||size>limit(capability,"max-queue-bytes")) { return false; }
            auto now{std::chrono::steady_clock::now()};
            auto elapsed{std::chrono::duration<double>(now-updated).count()}; updated=now;
            tokens=std::min(static_cast<double>(limit(capability,"burst")),tokens+elapsed*positive(field(capability,"max-rate-hz")));
            bytes=std::min(static_cast<double>(limit(capability,"max-bytes-per-second")),bytes+elapsed*static_cast<double>(limit(capability,"max-bytes-per-second")));
            if(tokens<1||bytes<static_cast<double>(size)) { return false; }
            tokens-=1; bytes-=static_cast<double>(size); return true;
        }
        bool retain(ipc::transform_data const& transform)
        {
            // Dynamic tf2 samples keep their existing duplicate-stamp rule.
            if(capability.resource!="/tf_static"&&!history.empty()&&history.back().seconds==transform.seconds&&history.back().nanoseconds==transform.nanoseconds) { return false; }
            auto size{ipc::encode_transform(transform).size()};
            auto depth{limit(capability,capability.type.find("/msg/")==std::string::npos?"queue-depth":"qos.depth")};
            auto maximum{limit(capability,"max-queue-bytes")};
            if(capability.resource=="/tf_static")
            {
                // A static frame has one latest value, including replacements
                // with the same timestamp. Admission already checked rate,
                // wire bytes and frame permissions before this cache update.
                if(!depth||size>maximum) { return false; }
                if(history.size()==1)
                {
                    auto const& previous{history.front()};
                    if(previous.sequence==transform.sequence&&previous.seconds==transform.seconds&&previous.nanoseconds==transform.nanoseconds&&
                       previous.parent==transform.parent&&previous.child==transform.child&&
                       !std::memcmp(previous.values.data(),transform.values.data(),previous.values.size()*sizeof(previous.values[0]))) { return false; }
                }
                if(history.empty()) { history.push_back(transform); }
                else { history.front()=transform;while(history.size()>1) { history.pop_back(); } }
                history_bytes=size;return true;
            }
            if(history.size()>=depth||size>maximum-history_bytes)
            {
                if(field(capability,"overflow-policy")!="drop-oldest") { return false; }
                while(!history.empty()&&(history.size()>=depth||size>maximum-history_bytes))
                { history_bytes-=ipc::encode_transform(history.front()).size(); history.pop_front(); }
            }
            history.push_back(transform); history_bytes+=size; return true;
        }
    };
}
