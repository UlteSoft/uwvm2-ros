/** @file
 * @brief Closed capture, completed-model publication, read and inert admission names.
 * @details proposal.capture returns authentic ordered sealed buffer references;
 * proposal.publish authenticates the complete successful model output against
 * that original capture. proposal.read exposes retained provenance and
 * proposal.accept records local admission without executing the target.
 * proposal_codec.h supplies bounded canonical carriers. These descriptors alone
 * do not register imports or activate a native driver.
 * Internal identities below refer to trusted immutable registry
 * entries; they are not substitutes for the catalogue's profile text/digest.
 */
#pragma once
#define UWVM_ROS_PROPOSAL_FUNCTIONS(X) \
 X(proposal_read,read,"proposal.read") \
 X(proposal_accept,accept,"proposal.accept") \
 X(proposal_capture,capture,"proposal.capture") \
 X(proposal_publish,publish,"proposal.publish")

#if !defined(UWVM_SCENE_PROPOSAL_CONTRACT_MACROS_ONLY)
#include <array>
#include <cstdint>
#include <string_view>
namespace uwvm2::imported::ros::proposal_contract
{
    enum class operation : std::uint8_t {read,accept,capture,publish};
    inline constexpr std::array names{std::string_view{"proposal.read"},std::string_view{"proposal.accept"},std::string_view{"proposal.capture"},std::string_view{"proposal.publish"}};
    struct descriptor{operation id;std::string_view name;};
    inline constexpr std::array operations{
#define UWVM_PROPOSAL_DESCRIPTOR(symbol,action,name) descriptor{operation::action,name},
        UWVM_ROS_PROPOSAL_FUNCTIONS(UWVM_PROPOSAL_DESCRIPTOR)
#undef UWVM_PROPOSAL_DESCRIPTOR
    };
    inline constexpr std::array read_fields{std::string_view{"proposal"}};
    inline constexpr std::array accept_fields{
        std::string_view{"key"},std::string_view{"proposal_capability"},std::string_view{"proposal"},
        std::string_view{"target_capability"},std::string_view{"target"},std::string_view{"deadline"}};
    inline constexpr std::array capture_fields{std::string_view{"key"},std::string_view{"profile"},std::string_view{"sources"},std::string_view{"expiry"}};
    inline constexpr std::array publish_fields{std::string_view{"key"},std::string_view{"capture"},std::string_view{"model_request"},std::string_view{"envelope"},std::string_view{"expiry"}};

    /** @brief Closed target action set already specified by the catalogue.
     * @details No policy management, credential access, native plugin loading,
     * generic filesystem or socket operation can be selected by a model tool.
     * Enum ordinals are internal and are not assigned external ABI numbers.
     */
    enum class target_action : std::uint8_t
    {device_submit,control_submit,topic_publish,service_submit,action_submit,parameter_set,parameter_set_atomic,lifecycle_submit};
    inline constexpr std::array target_names{
        std::string_view{"device.submit"},std::string_view{"control.submit"},std::string_view{"topic.publish"},
        std::string_view{"service.submit"},std::string_view{"action.submit"},std::string_view{"parameter.set"},
        std::string_view{"parameter.set.atomic"},std::string_view{"lifecycle.submit"}};
    inline constexpr bool valid(target_action value) noexcept
    {return static_cast<unsigned>(value)<target_names.size();}
    inline constexpr std::uint64_t action_bit(target_action value) noexcept
    {return valid(value)?std::uint64_t{1}<<static_cast<unsigned>(value):0;}
    inline constexpr std::uint64_t all_actions=(std::uint64_t{1}<<target_names.size())-1;

    /** @brief Local admission uses existing receipt words, never completion.
     * @details An admitted/locally_accepted receipt is inert recovery data.
     * It is not a command, an authority lease, a physical completion, or a
     * permission to execute later without checking current target authority.
     */
    inline constexpr std::string_view receipt_state="admitted",receipt_effect="locally_accepted";
}
#endif
