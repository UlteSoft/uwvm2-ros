/** @file @brief Exact ingress-only sensor operations; no guest endpoint or transport selection. */
#pragma once
#define UWVM_ROS_SENSOR_FUNCTIONS(X) \
 X(sensor_open,open,"sensor.open") \
 X(sensor_take,take,"sensor.take") \
 X(sensor_describe,describe,"sensor.describe")
#if !defined(UWVM_SCENE_SENSOR_CONTRACT_MACROS_ONLY)
#include <array>
#include <cstdint>
#include <string_view>
namespace uwvm2::imported::ros::sensor_contract
{
    enum class operation:std::uint8_t{open,take,describe};
    struct descriptor{operation id;std::string_view name;};
    inline constexpr std::array operations{
#define UWVM_SENSOR_DESCRIPTOR(symbol,op,name) descriptor{operation::op,name},
        UWVM_ROS_SENSOR_FUNCTIONS(UWVM_SENSOR_DESCRIPTOR)
#undef UWVM_SENSOR_DESCRIPTOR
    };
    inline constexpr std::size_t maximum_request_bytes=144,maximum_reply_bytes=4u*1024u*1024u+2048;
}
#endif
