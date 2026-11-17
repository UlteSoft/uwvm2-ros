/** @file
 * @brief Trusted deployment selection for one fixed receive-only sensor schema.
 * @details The CLI registry is deliberately closed. Octets preserve opaque
 * measured bytes; they do not assert a ROS message schema or physical units.
 * The authenticated definition supplies units/frame and exact source epochs.
 * Descriptor selection is separate trusted configuration, never definition or
 * guest input. Static validation never probes or duplicates that descriptor;
 * activation performs the existing read-only pipe/raw-terminal checks.
 */
#pragma once
#include "sensor_configuration.h"
namespace uwvm2::imported::ros::scene
{
    inline bool sensor_octets_valid(wire::bytes body)noexcept
    {return !body.empty()&&body.size()<=buffers::maximum_transfer;}
    inline sensors::schema_registration sensor_octets_registration()noexcept
    {
        sensors::schema_registration value;value.schema.identity.assign("ros.sensor.octets");
        constexpr std::string_view definition="uwvm.ros.sensor.octets:nonempty opaque bytes; maximum=4194304; no implicit ROS message or physical schema";
        value.schema.sha256=buffers::hash({reinterpret_cast<std::uint8_t const*>(definition.data()),definition.size()});
        value.type.assign("ros.sensor.bytes");value.encoding.assign("octets");value.validate=sensor_octets_valid;return value;
    }
    class sensor_deployment_configuration
    {
        buffers::text<255> alias_;buffers::profile selected_;sensors::schema_registration registered_;
        sensors::configuration parsed_;int descriptor_{-1};bool profiled_{},defined_{},descriptor_set_{};
    public:
        error profile(std::string_view alias,buffers::profile selected,std::string_view schema)noexcept
        {
            if(profiled_)return error::conflict;
            if(!identifier(alias)||!proposals::envelope_profile(selected)||schema!="octets")return error::invalid;
            alias_.assign(alias);selected_=selected;registered_=sensor_octets_registration();profiled_=true;return error::success;
        }
        bool definition_pending()const noexcept{return profiled_&&!defined_;}
        error definition(wire::bytes bytes)noexcept
        {
            if(!profiled_)return error::denied;
            if(defined_)return error::conflict;
            auto status=parsed_.begin(alias_.view(),selected_,bytes,registered_);
            if(status==error::success)defined_=true;
            return status;
        }
        error descriptor(std::uint64_t value)noexcept
        {
            if(!profiled_)return error::denied;
            if(descriptor_set_)return error::conflict;
            if(value>INT_MAX)return error::invalid;
            descriptor_=static_cast<int>(value);descriptor_set_=true;return error::success;
        }
        error limit(std::string_view name,std::uint64_t value)noexcept{return parsed_.limit(name,value);}
        error freeze(sensor_dispatch::configuration& output,int& descriptor)noexcept
        {
            if(!defined_||!descriptor_set_)return error::invalid;
            auto status=parsed_.freeze(output);if(status==error::success)descriptor=descriptor_;return status;
        }
    };
}
