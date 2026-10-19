// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "../common/config.h"
#include <uwvm2/imported/ros/common/service_availability.h>
namespace uwvm_default_control_gateway_detail {
using namespace ::uwvm2::ros_gateway;
inline std::string identity_text(std::uint8_t const(&bytes)[32]) {
 static constexpr char hex[]="0123456789abcdef";std::string text{"RIHS01_"};text.reserve(71);
 for(auto b:bytes){text.push_back(hex[b>>4]);text.push_back(hex[b&15]);}return text;
}
inline bool explicit_qos(ros::capability_spec const&cap) {
 for(auto key:{"qos.history","qos.depth","qos.reliability","qos.durability","qos.liveliness","qos.deadline","qos.lifespan","qos.liveliness-lease-duration"})
  if(cap.limits.find(key)==cap.limits.end())return false;
 // Actual original ParameterEventsQoS/ParametersQoS can have depth1000. This
 // native DDS depth is not the canonical lifetime/history slot count. Preserve
 // original schema/QoS; independent queue/process storage must be admitted too.
 if(field(cap,"qos.history")!="keep-last"||!limit(cap,"qos.depth")||limit(cap,"qos.depth")>1000)return false;
 auto const&r=field(cap,"qos.reliability");auto const&d=field(cap,"qos.durability");auto const&l=field(cap,"qos.liveliness");
 if((r!="reliable"&&r!="best-effort")||(d!="volatile"&&d!="transient-local")||(l!="automatic"&&l!="manual-by-topic"))return false;
 for(auto key:{"qos.deadline","qos.lifespan","qos.liveliness-lease-duration"}){auto const&t=field(cap,key);if(t!="infinite")(void)integer(t,86400000000000ULL);}return true;
}
    inline bool availability_selected(ros::capability_spec const& availability,ros::capability_spec const& source)
    {
        if(availability.alias.empty()||!explicit_qos(availability)||availability.limits.find("validator")!=availability.limits.end()||
            !ros::service_availability_pair(ros::version::ros2,availability,source))return false;
        for(auto key:{"max-inflight","max-request-bytes","max-response-bytes","max-wait-ns","timeout-ms","max-native-clients","max-binding-history"})
            if(availability.limits.find(key)==availability.limits.end())return false;
        auto wait=limit(availability,"max-wait-ns"),timeout=limit(availability,"timeout-ms"),clients=limit(availability,"max-native-clients"),history=limit(availability,"max-binding-history");
        return limit(availability,"max-inflight")==1&&limit(availability,"max-request-bytes")==64&&limit(availability,"max-response-bytes")==64&&
            wait&&wait<=60000000000ULL&&timeout&&timeout<=60000&&(wait+999999)/1000000+1<=timeout&&
            clients&&clients<=1024&&history&&history<=65536&&clients<=history;
    }
}
