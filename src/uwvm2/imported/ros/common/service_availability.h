/*************************************************************
 * UWVM2 paired native client observation contract. APL-2.0. *
 *************************************************************/
#pragma once
#include "types.h"
#include <initializer_list>
#include <uwvm2/imported/ros/ipc/service_availability.h>

namespace uwvm2::imported::ros
{
    // Both sides call this on their ORIGINAL configured grants. A Guest never
    // supplies a service name/type/QoS, and call authority alone grants no query.
    inline bool service_availability_pair(version backend,capability_spec const& availability,capability_spec const& source) noexcept
    {
        if(!valid_version(backend)||availability.operation!="service.available"||source.operation!="service.call"||
           availability.alias==source.alias||availability.resource!=source.resource||availability.type!=source.type||
           availability.type_identity!=source.type_identity){return false;}
        auto selected{availability.limits.find("service-source-alias")};
        if(selected==availability.limits.end()||selected->second!=source.alias){return false;}
        auto codec{availability.limits.find("adapter-codec-identity")},source_codec{source.limits.find("adapter-codec-identity")};
        if((codec==availability.limits.end())!=(source_codec==source.limits.end())||
           (codec!=availability.limits.end()&&codec->second!=source_codec->second)){return false;}
        if(backend==version::ros2)
        {
            for(auto field:{"qos.reliability","qos.durability","qos.history","qos.depth","qos.deadline","qos.lifespan","qos.liveliness","qos.liveliness-lease-duration"})
            {auto left{availability.limits.find(field)},right{source.limits.find(field)};if(left==availability.limits.end()||right==source.limits.end()||left->second!=right->second){return false;}}
        }
        return true;
    }
}
