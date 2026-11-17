/** @file
 * @brief Checked immutable-artifact records in uwvm_ros_v2.
 * @details All scalar words are little-endian u64. Open requires the exact
 * deployment generation followed by 32 raw SHA256 bytes; it returns a handle.
 * Read takes handle/offset/requested bytes and returns raw bytes, with actual
 * length in the common ABI slot. Describe takes a handle and returns generation,
 * size, stable purpose code, maximum transfer, then 32 raw SHA256 bytes.
 * Purpose codes are 1 package, 2 configuration, 3 calibration, 4 map, 5 weights.
 * No record contains a native path, descriptor or writable directory authority.
 */
#pragma once
#include <array>
#include <cstdint>
#include <string_view>
#define UWVM_ROS_ARTIFACT_FUNCTIONS(X) \
 X(artifact_open,open,"artifact.open",5,8) \
 X(artifact_read,read,"artifact.read",3,-1) \
 X(artifact_describe,describe,"artifact.describe",1,64)
#ifndef UWVM_SCENE_ARTIFACT_CONTRACT_MACROS_ONLY
namespace uwvm2::imported::ros::artifact_contract
{
    enum class operation : std::uint8_t { open,read,describe };
    struct descriptor {operation id;std::string_view name;unsigned words;int bytes;};
    inline constexpr std::array operations{
        descriptor{operation::open,"artifact.open",5,8},
        descriptor{operation::read,"artifact.read",3,-1},
        descriptor{operation::describe,"artifact.describe",1,64}};
}
#endif
