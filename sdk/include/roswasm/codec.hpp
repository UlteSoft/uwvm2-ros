/** @file
 * @brief Complete C++20 canonical values, including common resource envelopes.
 * This forwards the established codec unchanged and adds missing public value
 * shapes. No native ROS serialization, IDL guessing, permission or I/O is added.
 */
#pragma once
#include <utility>
#include "../../common/codec.hpp"
namespace uwvm_ros {
/** @brief Service envelope; its ID is assigned and owned by the gateway, not the guest. */
struct service_envelope {std::uint64_t request_id{};bytes payload;};
/** @brief Encode a service request/response envelope; zero IDs are rejected. */
inline bytes encode(service_envelope const& value) {
    if(!value.request_id||value.payload.size()>4193778u) return {};
    auto out=goal_id(value.request_id);out.insert(out.end(),value.payload.begin(),value.payload.end());return out;
}
/** @brief Decode an owned service envelope transactionally, retaining its canonical inner bytes. */
inline bool decode(std::span<unsigned char const> value,service_envelope& out) {
    if(value.size()<8||value.size()>4193786u) return false;reader in{value};service_envelope next;
    if(!in.integer(next.request_id,8)||!next.request_id) return false;
    next.payload.assign(value.begin()+8,value.end());out=std::move(next);return true;
}
/** @brief Incoming Fibonacci goal event; the goal ID originates from the gateway. */
struct incoming_goal {std::uint64_t goal_id{};std::int32_t order{};};
/** @brief Encode one nonzero goal ID followed by signed i32 order bits. */
inline bytes encode(incoming_goal const& value) {
    if(!value.goal_id) return {};auto out=goal_id(value.goal_id);append_u32(out,std::bit_cast<std::uint32_t>(value.order));return out;
}
/** @brief Decode one exact twelve-byte advertised goal event; output is preserved on failure. */
inline bool decode(std::span<unsigned char const> value,incoming_goal& out) {
    reader in{value};incoming_goal next;std::uint32_t order;
    if(!in.integer(next.goal_id,8)||!next.goal_id||!in.u32(order)||!in.done()) return false;
    next.order=std::bit_cast<std::int32_t>(order);out=next;return true;
}
/** @brief ROS2 Lifecycle state with owning label. */
struct lifecycle_state {std::uint8_t id{};std::string label;};
/** @brief Encode a state byte and canonical label; native state policy is enforced separately. */
inline bytes encode(lifecycle_state const& value) {
    bytes out{value.id};if(!append_string(out,value.label)) return {};return out;
}
/** @brief Decode exact state/label fields transactionally. */
inline bool decode(std::span<unsigned char const> value,lifecycle_state& out) {
    if(value.empty()) return false;lifecycle_state next;next.id=value.front();
    if(!decode_string(value.subspan(1),next.label)) return false;out=std::move(next);return true;
}
/** @brief Encode the legacy String parameter-event shape; deletion carries an empty value. */
inline bytes encode(parameter_event const& value) {
    if(value.kind<1||value.kind>3||(value.kind==3&&!value.value.empty())) return {};
    bytes out;append_u32(out,value.seconds);append_u32(out,value.nanoseconds);out.push_back(value.kind);
    if(!append_string(out,value.node)||!append_string(out,value.name)||!append_string(out,value.value)) return {};return out;
}
/** @brief Encode a clock response or TF lookup time as an eight-byte nanosecond integer. */
inline bytes nanoseconds(std::uint64_t value) {return lookup_time(value);}
/** @brief Decode one exact eight-byte time value, preserving output on failure. */
inline bool decode_nanoseconds(std::span<unsigned char const> value,std::uint64_t& out) {
    if(value.size()!=8) return false;reader in{value};std::uint64_t next;if(!in.integer(next,8)||!in.done()) return false;out=next;return true;
}
} // namespace uwvm_ros
