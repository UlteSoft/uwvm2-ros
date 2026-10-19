/** Original rclcpp_action native witness. Source-only; never a Guest grant.
 * Keeps original Node defaults and one actual ActionClient/Executor/GoalHandle.
 * Prints COMPLETE typed feedback/result YAML through actual fast_io.
 */
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/executors/single_threaded_executor.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <control_msgs/action/follow_joint_trajectory.hpp>
#include <trajectory_msgs/msg/joint_trajectory_point.hpp>
#include <fast_io.h>
#include <array>
#include <chrono>
#include <cmath>
#include <cstring>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
using Action=control_msgs::action::FollowJointTrajectory;
using Handle=rclcpp_action::ClientGoalHandle<Action>;
using Clock=std::chrono::steady_clock;
static double number(char const* text) {
    if(!text||!*text)throw std::invalid_argument("explicit target required");
    std::string_view input{text};
    if(input.find_first_of(" \t\r\n\f\v")!=std::string_view::npos)throw std::invalid_argument("target whitespace");
    double value{};auto first{input.data()},last{first+input.size()};
    auto [next,status]{fast_io::parse_by_scan(first,last,value)};
    if(status!=fast_io::parse_code::ok||next!=last||!std::isfinite(value))throw std::invalid_argument("finite complete target token required");
    return value;
}
template<typename Future> static void finish(rclcpp::executors::SingleThreadedExecutor& executor,
    Future& future,Clock::time_point deadline) {
    auto now{Clock::now()};if(now>=deadline)throw std::runtime_error("actual deadline expired; unknown outcome");
    if(executor.spin_until_future_complete(future,deadline-now)!=rclcpp::FutureReturnCode::SUCCESS)
        throw std::runtime_error("actual native future did not complete; no replay");
}
int main(int argc,char** argv) {
    // No Node/participant/client exists before the explicit operator selection.
    if(argc<4||std::string_view{argv[1]}!="--execute")return 13;
    Action::Goal goal;std::string action;
    std::string_view model{argv[2]};std::size_t count{};
    if(model=="gen3-6dof"||model=="gen3-7dof") {
        count=model=="gen3-6dof"?6:7;action="/joint_trajectory_controller/follow_joint_trajectory";
        if(static_cast<std::size_t>(argc)!=count+3)return 22;
        for(std::size_t i{};i<count;++i)goal.trajectory.joint_names.push_back("joint_"+std::to_string(i+1));
    } else if(model=="stretch-RE1V0"||model=="stretch-RE2V0"||model=="stretch-SE3") {
        count=1;action="/stretch_controller/follow_joint_trajectory";
        if(argc!=4)return 22;goal.trajectory.joint_names={"joint_lift"};
    } else return 22;
    trajectory_msgs::msg::JointTrajectoryPoint point;
    for(std::size_t i{};i<count;++i)point.positions.push_back(number(argv[3+i]));
    point.time_from_start.sec=2;goal.trajectory.points.push_back(std::move(point));
    // This convenience goal has actual empty tolerance/MultiDOF arrays. The
    // standard type/codec is complete and permits filling all of them.
    std::array<std::shared_ptr<Action::Feedback const>,256> observations{};
    std::size_t observed{};bool overflow{};
    rclcpp::init(0,nullptr);
    std::exception_ptr primary,shutdown_error;
    try {
        auto node{std::make_shared<rclcpp::Node>("uwvm_mobile_original_cpp_peer")};
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
        auto client{rclcpp_action::create_client<Action>(node,action)};
        if(!client->wait_for_action_server(std::chrono::seconds{3}))throw std::runtime_error("actual same-client server readiness unavailable");
        rclcpp_action::Client<Action>::SendGoalOptions options;
        options.feedback_callback=[&](Handle::SharedPtr,std::shared_ptr<Action::Feedback const> value) {
            if(observed==observations.size()){overflow=true;return;}
            observations[observed++]=std::move(value);
        };
        auto deadline{Clock::now()+std::chrono::seconds{5}};
        auto sent{client->async_send_goal(goal,options)};finish(executor,sent,deadline);
        auto handle{sent.get()};if(!handle)throw std::runtime_error("real server rejected goal");
        auto result{client->async_get_result(handle)};
        finish(executor,result,Clock::now()+std::chrono::seconds{10});
        auto actual{result.get()};if(!actual.result||overflow)throw std::runtime_error("result absent or bounded feedback storage exhausted");
        for(std::size_t i{};i<observed;++i) {
            auto yaml{control_msgs::action::to_yaml(*observations[i])};
            fast_io::println(fast_io::out(),std::string_view{yaml});
        }
        auto yaml{control_msgs::action::to_yaml(*actual.result)};
        fast_io::println(fast_io::out(),"actual_result_code=",static_cast<int>(actual.code),"\n",std::string_view{yaml});
        // No inference that a successful native result is a safety stop. The
        // operator must inspect full values and independent actual telemetry.
        executor.remove_node(node);client.reset();node.reset();
    } catch(...) {primary=std::current_exception();}
    try {rclcpp::shutdown();}catch(...){shutdown_error=std::current_exception();}
    if(primary) {
        if(shutdown_error) {try {fast_io::println(fast_io::err(),"independent native context shutdown failed");}catch(...) {}}
        std::rethrow_exception(primary);
    }
    if(shutdown_error)std::rethrow_exception(shutdown_error);
    return 0;
}
