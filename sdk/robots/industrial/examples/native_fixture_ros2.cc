/** Genuine native synthetic opponent, not an industrial controller.
 * Complete original control_msgs values and real rclcpp_action callbacks. There
 * is no robot IP, motor enable, trajectory controller or hardware selection.
 * Root must admit the exact linked SDK/process/maps before releasing SIGSTOP/I.
 * M permits traffic. F ends the finite observation. Native counters/values are
 * evidence only when actually run with the separately admitted Guest artifact.
 */
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <control_msgs/action/follow_joint_trajectory.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <fast_io.h>
#include <chrono>
#include <csignal>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <poll.h>
#include <unistd.h>
#include "tests/ros_custom_types/bounded-v2/native_protocol.h"
#ifndef UWVM_INDUSTRIAL_NATIVE_HEADER
#error "same-catalog actual generated native_ros2.hpp required"
#endif
#include UWVM_INDUSTRIAL_NATIVE_HEADER
using Action=control_msgs::action::FollowJointTrajectory;
using Handle=rclcpp_action::ServerGoalHandle<Action>;
using Clock=std::chrono::steady_clock;
void require(bool condition,char const* what){if(!condition)throw std::runtime_error{what};}
Action::Goal complete_goal(){
 Action::Goal g;g.trajectory.header.frame_id="industrial_fixture";g.trajectory.joint_names={"j1","j2"};g.trajectory.points.resize(2);
 for(size_t p=0;p<2;++p){auto& v=g.trajectory.points[p];v.positions={0.1*double(1+p),0.1*double(2+p)};v.velocities={0,0};v.accelerations={0,0};v.effort={.5,1.5};v.time_from_start.sec=static_cast<int32_t>(p+1);}
 g.multi_dof_trajectory.header.frame_id="tool";g.multi_dof_trajectory.joint_names={"virtual"};g.multi_dof_trajectory.points.resize(1);auto& m=g.multi_dof_trajectory.points[0];m.transforms.resize(1);m.velocities.resize(1);m.accelerations.resize(1);
 m.transforms[0].translation.x=1.25;m.transforms[0].translation.y=-2.5;m.transforms[0].translation.z=3.75;m.transforms[0].rotation.w=1;m.velocities[0].linear.x=.2;m.velocities[0].angular.z=-.3;m.accelerations[0].linear.y=.4;m.accelerations[0].angular.x=-.5;m.time_from_start.sec=1;
 g.path_tolerance.resize(1);g.path_tolerance[0].name="j1";g.path_tolerance[0].position=.01;g.path_tolerance[0].velocity=.02;g.path_tolerance[0].acceleration=.03;g.goal_tolerance=g.path_tolerance;
 g.component_path_tolerance.resize(1);g.component_path_tolerance[0].joint_name="virtual";g.component_path_tolerance[0].component=control_msgs::msg::JointComponentTolerance::X_AXIS;g.component_path_tolerance[0].position=.01;g.component_path_tolerance[0].velocity=.02;g.component_path_tolerance[0].acceleration=.03;g.component_goal_tolerance=g.component_path_tolerance;g.goal_time_tolerance.sec=1;return g;
}
rmw_qos_profile_t explicit_profile(){
 auto q=rmw_qos_profile_default;q.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;q.depth=8;
 q.reliability=RMW_QOS_POLICY_RELIABILITY_RELIABLE;q.durability=RMW_QOS_POLICY_DURABILITY_VOLATILE;
 q.deadline=RMW_DURATION_INFINITE;q.lifespan=RMW_DURATION_INFINITE;
 q.liveliness=RMW_QOS_POLICY_LIVELINESS_AUTOMATIC;q.liveliness_lease_duration=RMW_DURATION_INFINITE;return q;
}
int main(int argc,char** argv){
 bool initialized=false;
 try{require(argc==2,"mode success/cancel/denied required");std::string_view mode{argv[1]};require(mode=="success"||mode=="cancel"||mode=="denied","unknown native mode");
 auto expected=complete_goal();require(uwvm_custom_bounded_native::Ros2Control_msgsActionFollowJointTrajectory_GoalCodec::valid(expected),"actual complete Native value rejected");
 require(::raise(SIGSTOP)==0,"owned stop failed");bounded_native_io::gate('I');
 int ros_argc=1;rclcpp::init(ros_argc,argv);initialized=true;auto options=rclcpp::NodeOptions{}.start_parameter_services(false).start_parameter_event_publisher(false).enable_rosout(false);options.append_parameter_override("start_type_description_service",false);
 auto node=std::make_shared<rclcpp::Node>("uwvm_industrial_fixture",options);bool allowed=false,finished=false,bad=false;uint64_t goals=0,cancels=0,feedback=0,terminals=0;std::shared_ptr<Handle> live;
 auto q=explicit_profile();auto action_options=rcl_action_server_get_default_options();
 action_options.goal_service_qos=q;action_options.result_service_qos=q;action_options.cancel_service_qos=q;action_options.feedback_topic_qos=q;action_options.status_topic_qos=q;action_options.result_timeout.nanoseconds=5000000000;
 auto server=rclcpp_action::create_server<Action>(node,"/uwvm_industrial_fixture/action",
  [&](rclcpp_action::GoalUUID const&,std::shared_ptr<Action::Goal const> goal){++goals;if(!allowed||!goal||*goal!=expected||goals!=1){bad=true;return rclcpp_action::GoalResponse::REJECT;}return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;},
  [&](std::shared_ptr<Handle> goal){++cancels;if(mode!="cancel"||goal!=live||cancels!=1){bad=true;return rclcpp_action::CancelResponse::REJECT;}return rclcpp_action::CancelResponse::ACCEPT;},
  [&](std::shared_ptr<Handle> goal){live=std::move(goal);},action_options);
 auto states=node->create_publisher<sensor_msgs::msg::JointState>("/uwvm_industrial_fixture/joint_states",rclcpp::QoS{rclcpp::QoSInitialization::from_rmw(q),q});
 sensor_msgs::msg::JointState js;js.header=expected.trajectory.header;js.name=expected.trajectory.joint_names;js.position=expected.trajectory.points[0].positions;js.velocity=expected.trajectory.points[0].velocities;js.effort=expected.trajectory.points[0].effort;
 auto expected_feedback=std::make_shared<Action::Feedback>();expected_feedback->header=expected.trajectory.header;expected_feedback->joint_names=expected.trajectory.joint_names;expected_feedback->desired=expected.trajectory.points[0];expected_feedback->actual=expected.trajectory.points[1];expected_feedback->error.positions={0,0};expected_feedback->error.velocities={0,0};expected_feedback->error.accelerations={0,0};expected_feedback->error.effort={0,0};
 expected_feedback->multi_dof_joint_names=expected.multi_dof_trajectory.joint_names;expected_feedback->multi_dof_desired=expected_feedback->multi_dof_actual=expected.multi_dof_trajectory.points[0];expected_feedback->multi_dof_error.transforms.resize(1);expected_feedback->multi_dof_error.transforms[0].rotation.w=1;expected_feedback->multi_dof_error.velocities.resize(1);expected_feedback->multi_dof_error.accelerations.resize(1);
 rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);auto until=Clock::now()+std::chrono::seconds(30),last=Clock::now();fast_io::io::println("READY_INDUSTRIAL_NATIVE_V1");
 while(!finished&&rclcpp::ok()&&Clock::now()<until){pollfd input{STDIN_FILENO,POLLIN,0};auto ready=::poll(&input,1,0);require(ready>=0,"control poll failed");if(ready&&input.revents&POLLIN){char c{};require(::read(0,&c,1)==1,"control EOF");if(c=='M')allowed=true;else if(c=='F')finished=true;else throw std::runtime_error{"unknown control byte"};}
  executor.spin_once(std::chrono::milliseconds(5));if(allowed&&Clock::now()-last>std::chrono::milliseconds(20)){states->publish(js);last=Clock::now();}
  if(live&&!feedback){live->publish_feedback(expected_feedback);++feedback;}
  if(live&&!terminals&&(mode=="success"||live->is_canceling())){auto result=std::make_shared<Action::Result>();result->error_code=mode=="success"?Action::Result::SUCCESSFUL:Action::Result::PATH_TOLERANCE_VIOLATED;result->error_string=mode=="success"?"complete-native-success":"complete-native-cancel";if(mode=="success")live->succeed(result);else live->canceled(result);++terminals;}
 }
 require(finished,"native fixture deadline exhausted");require(!bad,"native full Goal/cancel mismatch");require(mode=="denied"?(goals==0&&cancels==0&&terminals==0):(goals==1&&terminals==1&&cancels==(mode=="cancel"?1u:0u)),"native effect counts differ");
 executor.remove_node(node);live.reset();server.reset();states.reset();node.reset();rclcpp::shutdown();initialized=false;fast_io::io::println("OBSERVED_INDUSTRIAL_NATIVE_V1 goals=",goals," cancels=",cancels," feedback=",feedback," terminals=",terminals);return 0;
 }catch(std::exception const& e){if(initialized)rclcpp::shutdown();fast_io::io::perrln("industrial native fixture refused: ",e.what());return 1;}
}
