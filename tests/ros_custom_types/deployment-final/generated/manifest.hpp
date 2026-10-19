/** Exact immutable fixed-v1 + bounded-v2 registry composition.
 * Original product bytes/codec identities remain in separate pinned subtrees.
 * This table registers supported types and grants no authority. */
#pragma once
#include <array>
#define custom_generated_topics uwvm_composite_fixed_topics
#define custom_topic_entries uwvm_composite_fixed_entries
#include "fixed/manifest.hpp"
#undef custom_topic_entries
#undef custom_generated_topics
#define custom_generated_topics uwvm_composite_bounded_topics
#define custom_topic_entries uwvm_composite_bounded_entries
#define custom_service_entries uwvm_composite_bounded_services
#define custom_action_entries uwvm_composite_bounded_actions
#include "bounded/manifest.hpp"
#undef custom_action_entries
#undef custom_service_entries
#undef custom_topic_entries
#undef custom_generated_topics
namespace uwvm2::imported::ros::ipc {
inline constexpr auto uwvm_composite_topics() noexcept {
constexpr auto fixed=uwvm_composite_fixed_entries(),bounded=uwvm_composite_bounded_entries();
std::array<custom_topic_descriptor,fixed.size+bounded.size> out{};
for(size_t i=0;i<fixed.size;++i)out[i]=fixed.data[i];for(size_t i=0;i<bounded.size;++i)out[fixed.size+i]=bounded.data[i];return out;}
inline constexpr auto uwvm_composite_all_topics=uwvm_composite_topics();
inline constexpr custom_topic_registry custom_topic_entries() noexcept {return {uwvm_composite_all_topics.data(),uwvm_composite_all_topics.size()};}
inline constexpr custom_service_registry custom_service_entries() noexcept {return uwvm_composite_bounded_services();}
inline constexpr custom_action_registry custom_action_entries() noexcept {return uwvm_composite_bounded_actions();}
}
