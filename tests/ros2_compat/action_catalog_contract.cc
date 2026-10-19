/** @file SDK-free data regressions for production Action catalog pairing.
 * These checks deliberately do not supply ROS native types or execute probes.
 * Real selected Actions must additionally compile/link their complete public
 * client/server lifecycle and pass runtime typesupport/schema qualification.
 */
#include "../../gateways/ros2/compat/action_catalog.h"
#include <cassert>

namespace ipc=uwvm2::imported::ros::ipc;
namespace compat=uwvm2::ros_gateway::ros2_compat;
namespace
{
    struct GoalCodec {inline static constexpr char const* type="owned/action/Move_Goal";inline static constexpr char const* native_identity="goal-id";inline static constexpr char const* codec_identity="goal-codec";static constexpr std::size_t wire_bytes=4;};
    struct ResultCodec {inline static constexpr char const* type="owned/action/Move_Result";inline static constexpr char const* native_identity="result-id";inline static constexpr char const* codec_identity="result-codec";static constexpr std::size_t wire_bytes=8;};
    struct FeedbackCodec {inline static constexpr char const* type="owned/action/Move_Feedback";inline static constexpr char const* native_identity="feedback-id";inline static constexpr char const* codec_identity="feedback-codec";static constexpr std::size_t wire_bytes=12;};
    // Deliberately small equality fixtures, not a valid wire registry or native
    // identity claim. The separate production registry validator is mandatory.
    constexpr ipc::custom_topic_descriptor goal{2,GoalCodec::type,GoalCodec::native_identity,GoalCodec::codec_identity,GoalCodec::wire_bytes};
    constexpr ipc::custom_topic_descriptor result{2,ResultCodec::type,ResultCodec::native_identity,ResultCodec::codec_identity,ResultCodec::wire_bytes};
    constexpr ipc::custom_topic_descriptor feedback{2,FeedbackCodec::type,FeedbackCodec::native_identity,FeedbackCodec::codec_identity,FeedbackCodec::wire_bytes};
    constexpr ipc::custom_action_descriptor one{2,"owned/action/Move","native-one","canonical-one",&goal,&result,&feedback};
    constexpr ipc::custom_action_descriptor two{2,"owned/action/Stop","native-two","canonical-two",&goal,&result,&feedback};
    constexpr compat::action_catalog_row row_one{"owned/action/Move","native-one","canonical-one"};
    constexpr compat::action_catalog_row row_two{"owned/action/Stop","native-two","canonical-two"};
}
int main()
{
    // Group 1: absent and empty catalogs are unavailable, including null rows.
    assert(!compat::action_catalog_matches({nullptr,0},nullptr,0));
    assert(!compat::action_catalog_matches({&one,1},nullptr,1));
    assert(!compat::action_catalog_matches({nullptr,1},&row_one,1));
    // Group 2: one exact selected Action and all component roles pair.
    assert(compat::action_catalog_matches({&one,1},&row_one,1));
    assert((compat::action_components_match<GoalCodec,ResultCodec,FeedbackCodec>(&one)));
    // Group 3: every selected Action must pair; the native order may differ.
    ipc::custom_action_descriptor both[]{one,two};compat::action_catalog_row rows[]{row_two,row_one};
    assert(compat::action_catalog_matches({both,2},rows,2));
    assert(!compat::action_catalog_matches({both,2},&row_one,1));
    // Group 4: duplicate native/common rows cannot substitute for a missing row.
    compat::action_catalog_row repeated[]{row_one,row_one};
    assert(!compat::action_catalog_matches({both,2},repeated,2));
    ipc::custom_action_descriptor duplicate[]{one,one};
    assert(!compat::action_catalog_matches({duplicate,2},repeated,2));
    // Group 5: equal counts and names do not excuse either identity mismatch.
    auto changed=row_one;changed.native_identity="different";
    assert(!compat::action_catalog_matches({&one,1},&changed,1));
    changed=row_one;changed.codec_identity="different";
    assert(!compat::action_catalog_matches({&one,1},&changed,1));
    // Group 6: ROS1 entries are not mistaken for selected ROS2 native Actions.
    auto ros1=one;ros1.ros_version=1;ipc::custom_action_descriptor mixed[]{ros1,one};
    assert(compat::action_catalog_matches({mixed,2},&row_one,1));
    assert(!compat::action_catalog_matches({&ros1,1},&row_one,1));
    // Group 7: role swaps and absent components are refused independently.
    auto wrong=one;wrong.goal=&result;
    assert(!(compat::action_components_match<GoalCodec,ResultCodec,FeedbackCodec>(&wrong)));
    wrong=one;wrong.feedback=nullptr;
    assert(!(compat::action_components_match<GoalCodec,ResultCodec,FeedbackCodec>(&wrong)));
    // Group 8: each component's native identity, codec and extent stay exact.
    auto part=goal;part.native_identity="different";wrong=one;wrong.goal=&part;
    assert(!(compat::action_components_match<GoalCodec,ResultCodec,FeedbackCodec>(&wrong)));
    part=goal;part.codec_identity="different";
    assert(!(compat::action_components_match<GoalCodec,ResultCodec,FeedbackCodec>(&wrong)));
    part=goal;part.wire_bytes=5;
    assert(!(compat::action_components_match<GoalCodec,ResultCodec,FeedbackCodec>(&wrong)));
    part=goal;part.ros_version=1;
    assert(!(compat::action_components_match<GoalCodec,ResultCodec,FeedbackCodec>(&wrong)));
}
