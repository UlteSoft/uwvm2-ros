/** @file
 * @brief Small compiled registry for native ROS serialized Bool/String recordings.
 * @details This registry validates original ROS payload bytes, not the scene's
 * canonical communication carriers. ROS1 uses its little-endian serialization;
 * ROS2 accepts plain CDR big/little endian encapsulation with zero options.
 * Unknown CDR representations, extension tails and malformed length prefixes
 * are refused. ROS1 string content preserves embedded NULs; ROS2 strings need
 * one terminal NUL and reject embedded NULs, matching the qualified Fast-CDR
 * std::string serializer. Content is never normalized or interpreted as a path.
 *
 * These four compiled schemas are explicit initial deployment choices, not a
 * generic IDL implementation or an assertion of all-package portability.
 * Additional trusted generated validators use recording::codec and must carry
 * the exact native schema identity/encoding/definition digest. No guest can
 * provide a validator address, plugin or arbitrary shared-library path.
 */
#pragma once
#include "recording_configuration.h"

namespace uwvm2::imported::ros::scene::recording::codecs
{
    inline std::uint32_t length(wire::bytes input,bool little)noexcept
    {
        std::uint32_t value{};
        for(unsigned i{};i<4;++i)value|=static_cast<std::uint32_t>(input[i])<<(8*(little?i:3-i));
        return value;
    }
    inline bool ros1_boolean(wire::bytes value)noexcept
    {return value.size()==1&&value[0]<=1;}
    inline bool ros1_string(wire::bytes value)noexcept
    {return value.size()>=4&&length(value,true)==value.size()-4;}
    /** @brief Accept only complete plain CDR encapsulation with explicitly known options. */
    inline bool encapsulation(wire::bytes value)noexcept
    {return value.size()>=4&&!value[0]&&value[1]<=1&&!value[2]&&!value[3];}
    inline bool ros2_boolean(wire::bytes value)noexcept
    {return value.size()==5&&encapsulation(value)&&value[4]<=1;}
    inline bool ros2_string(wire::bytes value)noexcept
    {return value.size()>=9&&encapsulation(value)&&length(value.subspan(4),value[1]!=0)==value.size()-8&&!value.back()&&
        !std::memchr(value.data()+8,0,value.size()-9);}
    /** @brief Build fixed metadata without allocation; schema bytes and all validators are compiled. */
    inline std::array<codec,4> registry()noexcept
    {
        auto boolean=buffers::hash(mcap::raw("bool data\n")),string=buffers::hash(mcap::raw("string data\n"));
        return {codec{"ros1-std-bool","ros1","std_msgs/Bool","ros1msg","ros1",boolean,ros1_boolean},
                codec{"ros1-std-string","ros1","std_msgs/String","ros1msg","ros1",string,ros1_string},
                codec{"ros2-std-bool","ros2","std_msgs/msg/Bool","ros2msg","cdr",boolean,ros2_boolean},
                codec{"ros2-std-string","ros2","std_msgs/msg/String","ros2msg","cdr",string,ros2_string}};
    }
}
