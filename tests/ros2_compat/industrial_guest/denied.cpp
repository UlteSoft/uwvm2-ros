/** Actual complete C++ reactor, pure fixture values and zero-grant execution.
 * Include the reviewed vendor translation unit so the fixture's internal value
 * builders are tested without exposing them as a new Guest ABI or permission.
 * No goal is submitted. Positive controller/Gateway execution remains separate.
 */
#ifndef UWVM_INDUSTRIAL_TRAJECTORY_SOURCE
#error "Select the exact actual industrial trajectory example source"
#endif
#include UWVM_INDUSTRIAL_TRAJECTORY_SOURCE

int main() {
    uint32_t status{};
    auto goal=fixture_goal(status);
    if(status)return 1;
    auto feedback=fixture_feedback(status);
    if(status)return 2;
    auto state=fixture_joints(status);
    if(status)return 3;
    auto result=fixture_result(2,status);
    if(status)return 4;
    roswasmcpp::wire::bytes bytes;
    if(!roswasmcpp::Codec<Goal>::encode(goal,bytes)||bytes.empty())return 5;
    if(!roswasmcpp::Codec<Feedback>::encode(feedback,bytes)||bytes.empty())return 6;
    if(!roswasmcpp::Codec<Joints>::encode(state,bytes)||bytes.empty())return 7;
    if(!roswasmcpp::Codec<Result>::encode(result,bytes)||bytes.empty())return 8;
    if(feedback.joint_names.size!=2||feedback.multi_dof_joint_names.size!=1||
       state.name.size!=2||state.position.size!=2||state.velocity.size!=2||state.effort.size!=2)
        return 9;

    // An invalid source count and a nested destination-capacity failure must
    // leave every previously stored destination value intact.
    auto original=feedback;
    auto names=goal.trajectory.joint_names;
    names.size=static_cast<uint32_t>(std::size(names.data)+1);
    copy_fixture(feedback.joint_names,names,status);
    if(status!=90||!same_complete(feedback,original))return 10;
    struct TinyText {uint32_t size;unsigned char data[1];};
    struct TinyNames {uint32_t size;TinyText data[2];};
    TinyNames tiny{1,{{1,{42}},{1,{43}}}};
    status=0;copy_fixture(tiny,goal.trajectory.joint_names,status);
    if(status!=90||tiny.size!=1||tiny.data[0].size!=1||tiny.data[0].data[0]!=42||
       tiny.data[1].size!=1||tiny.data[1].data[0]!=43)return 11;
    status=0;text(tiny.data[0],"AB",2,status);
    if(status!=90||tiny.data[0].size!=1||tiny.data[0].data[0]!=42)return 12;

    if(initialize_case(2)!=22)return 13;
    if(initialize_case(0)!=13)return 14;
    if(denied_verified()!=1||primary_status()!=13||cleanup_status()!=0)return 15;
    if(initialize_case(0)!=9||submit_once()!=9||step_once()!=9||cancel_once()!=9)
        return 16;
    if(joints_received()||feedback_received()||results_received()||goal_low()||goal_high())
        return 17;
    return 0;
}
