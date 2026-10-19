/** Complete typed JointState/FJT Guest reactor. Source-only, no Native ABI.
 * Include from exactly one vendor binding. The external driver initializes the
 * actual reactor/constructors, then calls initialize_case once, submits once,
 * performs bounded step_once calls and finally close_case after callbacks.
 * This synthetic Goal belongs only to the independent native fixture resource;
 * it must never be forwarded unchanged to a physical controller.
 */
#include <roswasm/ros2.h>
#include <new>
#include <memory>
#include <array>
#include <type_traits>
#include <iterator>
#include "../include/trajectory_lifecycle.hpp"
#ifndef INDUSTRIAL_BINDING_NAMESPACE
#error "one whole reviewed vendor binding required"
#endif
using Joints=INDUSTRIAL_BINDING_NAMESPACE::sensor_msgs::JointState;
using Client=INDUSTRIAL_ACTION_OWNER;
using Goal=Client::Goal;using Feedback=Client::Feedback;using Result=Client::Result;
using Owner=uwvm_industrial::Trajectory<Client>;
// This synthetic trajectory writes two joints and two points. A caller-selected
// smaller catalog must fail compilation before any array access or ROS open.
static_assert(std::extent_v<decltype(Goal{}.trajectory.joint_names.data)> >= 2 &&
 std::extent_v<decltype(Goal{}.trajectory.points.data)> >= 2,
 "The fixture requires capacity for two joints and two trajectory points");
static_assert(std::extent_v<decltype(Goal{}.trajectory.points.data[0].positions.data)> >= 2 &&
 std::extent_v<decltype(Goal{}.trajectory.points.data[0].velocities.data)> >= 2 &&
 std::extent_v<decltype(Goal{}.trajectory.points.data[0].accelerations.data)> >= 2 &&
 std::extent_v<decltype(Goal{}.trajectory.points.data[0].effort.data)> >= 2,
 "The fixture requires capacity for two values in each joint state array");
#define INDUSTRIAL_EXPORT(n) extern "C" __attribute__((export_name(n)))
namespace {
roswasmc_context context{};
alignas(Owner) std::byte action_storage[sizeof(Owner)];
alignas(roswasmcpp::Subscription<Joints>) std::byte joint_storage[sizeof(roswasmcpp::Subscription<Joints>)];
Owner* action{};roswasmcpp::Subscription<Joints>* joints{};
bool initialized{},closed{},denied{},submitted{};uint32_t primary{},cleanup{},passes{},joint_count{},feedback_count{},result_count{};uint64_t saved_goal{};
Joints last_joints{};Feedback last_feedback{};Result last_result{};uint8_t last_kind{};
std::array<uint32_t,5> role_cleanup{};
roswasmc_string alias(char const* p,size_t n){return {p,n};}
uint32_t close_all(uint32_t e=0){
 if(e&&!primary)primary=e;if(closed)return primary?primary:9;closed=true;
 if(joints){if(joints->handle()){auto s=joints->close();if(!cleanup)cleanup=s;}std::destroy_at(joints);joints=nullptr;}
 if(action){auto r=action->close();role_cleanup=r.role_cleanup;if(!primary)primary=r.primary;if(!cleanup)cleanup=r.cleanup;std::destroy_at(action);action=nullptr;}
 return primary?primary:cleanup;
}
void text(auto& dest,char const* value,size_t n,uint32_t& status) noexcept {if(n>sizeof dest.data){status=90;return;}dest.size=static_cast<uint32_t>(n);for(size_t i=0;i<n;++i)dest.data[i]=static_cast<unsigned char>(value[i]);}
/** Generated fields have distinct C struct types even for equal capacities.
 * Copy the fixture's counted values, checking each destination independently;
 * neither a native-layout cast nor a byte copy can establish that relationship.
 * A failed copy preserves the destination and propagates its numeric error. */
template<class Destination,class Source>
void copy_fixture(Destination& dest,Source const& source,uint32_t& status) noexcept {
 if(status)return;
 if constexpr(requires{dest.size;source.size;dest.data[0];source.data[0];}){
  if(source.size>std::size(source.data)||source.size>std::size(dest.data)){status=90;return;}
  Destination next{};
  for(size_t i=0;i<source.size;++i)copy_fixture(next.data[i],source.data[i],status);
  if(!status){next.size=source.size;dest=next;}
 }else{static_assert(std::is_same_v<Destination,Source>);dest=source;}
}
Goal fixture_goal(uint32_t& status){
 status=0;Goal g{};text(g.trajectory.header.frame_id,"industrial_fixture",18,status);g.trajectory.joint_names.size=2;
 text(g.trajectory.joint_names.data[0],"j1",2,status);text(g.trajectory.joint_names.data[1],"j2",2,status);g.trajectory.points.size=2;
 for(size_t p=0;p<2;++p){auto& v=g.trajectory.points.data[p];v.positions.size=v.velocities.size=v.accelerations.size=v.effort.size=2;
  for(size_t j=0;j<2;++j){v.positions.data[j]=0.1*double(1+j+p);v.velocities.data[j]=0;v.accelerations.data[j]=0;v.effort.data[j]=0.5+double(j);}
  v.time_from_start.sec=static_cast<int32_t>(p+1);v.time_from_start.nanosec=0;}
 text(g.multi_dof_trajectory.header.frame_id,"tool",4,status);g.multi_dof_trajectory.joint_names.size=1;text(g.multi_dof_trajectory.joint_names.data[0],"virtual",7,status);
 g.multi_dof_trajectory.points.size=1;auto& m=g.multi_dof_trajectory.points.data[0];m.transforms.size=m.velocities.size=m.accelerations.size=1;
 m.transforms.data[0].translation.x=1.25;m.transforms.data[0].translation.y=-2.5;m.transforms.data[0].translation.z=3.75;m.transforms.data[0].rotation.w=1;
 m.velocities.data[0].linear.x=0.2;m.velocities.data[0].angular.z=-0.3;m.accelerations.data[0].linear.y=0.4;m.accelerations.data[0].angular.x=-0.5;m.time_from_start.sec=1;
 g.path_tolerance.size=g.goal_tolerance.size=1;text(g.path_tolerance.data[0].name,"j1",2,status);g.path_tolerance.data[0].position=.01;g.path_tolerance.data[0].velocity=.02;g.path_tolerance.data[0].acceleration=.03;g.goal_tolerance.data[0]=g.path_tolerance.data[0];
 g.component_path_tolerance.size=g.component_goal_tolerance.size=1;text(g.component_path_tolerance.data[0].joint_name,"virtual",7,status);g.component_path_tolerance.data[0].component=1;g.component_path_tolerance.data[0].position=.01;g.component_path_tolerance.data[0].velocity=.02;g.component_path_tolerance.data[0].acceleration=.03;g.component_goal_tolerance.data[0]=g.component_path_tolerance.data[0];
 g.goal_time_tolerance.sec=1;return g;
}
Feedback fixture_feedback(uint32_t& status){auto g=fixture_goal(status);Feedback f{};f.header=g.trajectory.header;copy_fixture(f.joint_names,g.trajectory.joint_names,status);f.desired=g.trajectory.points.data[0];f.actual=g.trajectory.points.data[1];
 f.error.positions.size=f.error.velocities.size=f.error.accelerations.size=f.error.effort.size=2;
 copy_fixture(f.multi_dof_joint_names,g.multi_dof_trajectory.joint_names,status);f.multi_dof_desired=f.multi_dof_actual=g.multi_dof_trajectory.points.data[0];
 f.multi_dof_error.transforms.size=f.multi_dof_error.velocities.size=f.multi_dof_error.accelerations.size=1;f.multi_dof_error.transforms.data[0].rotation.w=1;return f;}
Joints fixture_joints(uint32_t& status){auto g=fixture_goal(status);Joints value{};value.header=g.trajectory.header;copy_fixture(value.name,g.trajectory.joint_names,status);copy_fixture(value.position,g.trajectory.points.data[0].positions,status);copy_fixture(value.velocity,g.trajectory.points.data[0].velocities,status);copy_fixture(value.effort,g.trajectory.points.data[0].effort,status);return value;}
Result fixture_result(uint8_t kind,uint32_t& status){status=0;Result r{};r.error_code=kind==2?0:-4;if(kind==2)text(r.error_string,"complete-native-success",23,status);else text(r.error_string,"complete-native-cancel",22,status);return r;}
/** Preserve native unwinding when enabled; WASI may explicitly omit exceptions.
 * Without exceptions, allocation failure follows the standard library's
 * termination policy. Protocol and fixture-bound errors remain numeric. */
template<class Function>uint32_t guarded(Function&& function){
#if defined(__cpp_exceptions) && __cpp_exceptions
 try{return function();}catch(std::bad_alloc const&){return close_all(12);}catch(...){return close_all(5);}
#else
 return function();
#endif
}
template<class T>bool same_complete(T const& a,T const& b){roswasmcpp::wire::bytes x,y;return roswasmcpp::Codec<T>::encode(a,x)&&roswasmcpp::Codec<T>::encode(b,y)&&x==y;}
}
INDUSTRIAL_EXPORT("initialize_case") uint32_t initialize_case(uint32_t mode){
 if(initialized||closed)return 9;if(mode>1)return 22;initialized=true;
 if(uwvm_ros2_abi_version()!=1)return close_all(95);context.backend=roswasmc_ros2_backend();
 roswasmc_action_aliases roles{alias("trajectory-send",15),alias("trajectory-cancel",17),alias("trajectory-feedback",19),alias("trajectory-result",17),{nullptr,0}};
 return guarded([&]() -> uint32_t {action=new(action_storage)Owner(context,roles,1);auto e=action->error();
  if(mode==0){denied=e==13;return close_all(denied?0:e?e:22);}if(e)return close_all(e);
  joints=new(joint_storage)roswasmcpp::Subscription<Joints>(context,"joint-state",roswasmcpp::Codec<Joints>::max_wire_bytes);
  if(joints->error())return close_all(joints->error());
  joints->on_message([](Joints const& value){uint32_t status{};auto expected=fixture_joints(status);
   if(status||!same_complete(value,expected)){primary=status?status:22;return;}last_joints=value;++joint_count;});return 0;
 });
}
INDUSTRIAL_EXPORT("submit_once") uint32_t submit_once(){
 if(!initialized||closed||submitted||!action)return 9;submitted=true;
 return guarded([&]() -> uint32_t {uint32_t status{};auto goal=fixture_goal(status);if(status)return close_all(status);
  auto e=action->submit(goal);saved_goal=action->journal().outcome().goal;return e?close_all(e):0;});
}
INDUSTRIAL_EXPORT("step_once") uint32_t step_once(){
 if(!submitted||closed)return 9;if(passes++==4096){action->timeout();return close_all(110);}bool received=false;
 return guarded([&]() -> uint32_t {auto e=joints->poll_once();if(e&&e!=11)return close_all(e);if(primary)return close_all(primary);received|=!e;
  e=action->feedback(last_feedback);if(e&&e!=11)return close_all(e);if(!e){uint32_t status{};auto expected=fixture_feedback(status);if(status)return close_all(status);if(!same_complete(last_feedback,expected))return close_all(22);++feedback_count;received=true;}
  e=action->result(last_result,last_kind);if(e&&e!=11)return close_all(e);if(!e){uint32_t status{};auto expected=fixture_result(last_kind,status);if(status)return close_all(status);if((last_kind!=2&&last_kind!=4)||!same_complete(last_result,expected))return close_all(22);++result_count;return close_all();}return received?0:11;
 });
}
INDUSTRIAL_EXPORT("cancel_once") uint32_t cancel_once(){if(!submitted||closed||!action)return 9;bool accepted{};auto e=action->cancel_own(accepted);if(e)return close_all(e);return accepted?0:11;}
INDUSTRIAL_EXPORT("goal_low") uint32_t goal_low(){return static_cast<uint32_t>(saved_goal);}
INDUSTRIAL_EXPORT("goal_high") uint32_t goal_high(){return static_cast<uint32_t>(saved_goal>>32);}
INDUSTRIAL_EXPORT("role_cleanup") uint32_t role_status(uint32_t index){return index<role_cleanup.size()?role_cleanup[index]:22;}
INDUSTRIAL_EXPORT("joint_count") uint32_t joints_received(){return joint_count;}
INDUSTRIAL_EXPORT("feedback_count") uint32_t feedback_received(){return feedback_count;}
INDUSTRIAL_EXPORT("result_count") uint32_t results_received(){return result_count;}
INDUSTRIAL_EXPORT("terminal_kind") uint32_t terminal_kind(){return last_kind;}
INDUSTRIAL_EXPORT("controller_result") int32_t controller_result(){return last_result.error_code;}
INDUSTRIAL_EXPORT("denied_verified") uint32_t denied_verified(){return denied;}
INDUSTRIAL_EXPORT("primary_status") uint32_t primary_status(){return primary;}
INDUSTRIAL_EXPORT("cleanup_status") uint32_t cleanup_status(){return cleanup;}
INDUSTRIAL_EXPORT("close_case") uint32_t close_case(){return close_all();}
