/** @file
 * @brief Bounded volatile cache transactions in uwvm_ros_v2.
 * @details The seven-argument checked carrier accepts LE u64 headers. Append
 * has three header words followed by raw data; publish has two header words
 * [key, complete_payload_size] followed by exactly that payload. Other records
 * are fixed length. Mutations use one monotonically increasing key per grant.
 * Replaying the last identical request returns its retained receipt; the next
 * key acknowledges that receipt. Older, skipped or conflicting keys never run.
 * A successful carrier return means a 72-byte receipt was delivered; its
 * effect_status reports the mutation outcome. Delivery failure never erases
 * an admitted effect or its recovery receipt. Read-only status/receipt do not
 * acknowledge, mutate, grant authority or replenish any budget.
 * Volatile publication is session-local: flags bit 0 means published, bit 1
 * (durable) is always clear. Opaque cache bytes are not schema-validated data.
 */
#pragma once
#include <array>
#include <cstdint>
#include <string_view>
#define UWVM_ROS_OUTPUT_FUNCTIONS(X) \
 X(artifact_begin,begin,"artifact.begin",2) \
 X(artifact_append,append,"artifact.append",-3) \
 X(artifact_commit,commit,"artifact.commit",3) \
 X(artifact_abort,abort,"artifact.abort",2) \
 X(artifact_remove,remove,"artifact.remove",2) \
 X(artifact_status,status,"artifact.status",1) \
 X(artifact_receipt,receipt,"artifact.receipt",1) \
 X(artifact_publish,publish,"artifact.publish",-2)
#ifndef UWVM_SCENE_OUTPUT_CONTRACT_MACROS_ONLY
namespace uwvm2::imported::ros::output_contract
{
    enum class operation : std::uint8_t {begin,append,commit,abort,remove,status,receipt,publish};
    struct descriptor {operation id;std::string_view name;int words;};
    inline constexpr std::array operations{
#define UWVM_OUTPUT_DESCRIPTOR(symbol,action,name,words) descriptor{operation::action,name,words},
        UWVM_ROS_OUTPUT_FUNCTIONS(UWVM_OUTPUT_DESCRIPTOR)
#undef UWVM_OUTPUT_DESCRIPTOR
    };
    inline constexpr std::uint64_t receipt_bytes=72;
    /** @brief Complete publication pays the old nonempty one-append work cost.
     * @details Its independent action and all begin/append/commit dependencies
     * are required. Empty publication conservatively pays the same three units,
     * exceeding the old empty begin/commit path's two units. This trusted fixed
     * cost cannot be selected or reduced by the guest. The byte debit is the
     * complete payload plus 280, including the old three request/receipt pairs.
     */
    inline constexpr std::uint64_t publication_work_units=3,publication_framing=16,publication_byte_overhead=280;
}
#endif
