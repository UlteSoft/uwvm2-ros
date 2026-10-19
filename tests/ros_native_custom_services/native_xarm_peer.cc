/* SPDX-License-Identifier: Apache-2.0 */
/** Real ORIGINAL xArm Native opposite source, not executed.
 * This executable has NO Guest ABI and receives NO implicit Guest rights. Root
 * must independently build against the exact original selected xarm_msgs SDK,
 * retain all compiler/linker/MD/ELF/maps and own its finite process/cgroup/network.
 * Mode server exposes two explicit laboratory services for ONE request each.
 * Mode client sends exactly ONCE per service; errors/timeouts never replay.
 * These are arbitrary laboratory resource names, not robot/controller authority.
 * The fixture preserves every field, signed extremes, empty Request and complete valid UTF8.
 * A deferred rclcpp callback supplies no full same-take timestamp receipt: only
 * the new direct-rcl provider can prove that separate Native metadata contract.
 */
#include <rclcpp/rclcpp.hpp>
#include <xarm_msgs/srv/get_float32_list.hpp>
#include <xarm_msgs/srv/set_int16_list.hpp>
#include <chrono>
#include "gateways/common/native_io.h"
#include <cstring>
#include <atomic>
namespace {
using Get=xarm_msgs::srv::GetFloat32List;using Set=xarm_msgs::srv::SetInt16List;
std::string expected_message(){return std::string("native-\xC2\xB5",9);}
template<class T>bool response(T const& v){return v.ret==-7&&v.message==expected_message();}
void fill(Get::Response& v){v.ret=-7;v.message=expected_message();v.datas={1.25f,-2.5f};}
void fill(Set::Response& v){v.ret=-7;v.message=expected_message();}
int server(rclcpp::Node::SharedPtr const& node){
 bool bad{};unsigned gets{},sets{};
 auto a=node->create_service<Get>("/uwvm_custom/get_float32_list",[&](std::shared_ptr<Get::Request> request,std::shared_ptr<Get::Response> result){
  if(!request||!result||++gets!=1||request->structure_needs_at_least_one_member!=0){bad=true;return;}fill(*result);});
 auto b=node->create_service<Set>("/uwvm_custom/set_int16_list",[&](std::shared_ptr<Set::Request> request,std::shared_ptr<Set::Response> result){
  if(!request||!result||++sets!=1||request->datas!=std::vector<std::int16_t>{-32768,0,32767}){bad=true;return;}fill(*result);});
 rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);
 auto until=std::chrono::steady_clock::now()+std::chrono::seconds(15);bool removal_failed{};
 try{while(rclcpp::ok()&&std::chrono::steady_clock::now()<until&&!bad&&(gets!=1||sets!=1))
  executor.spin_once(std::chrono::milliseconds(100));}
 catch(...){try{executor.remove_node(node);}catch(...){uwvm2::ros_gateway::native_io::diagnostic("native peer cleanup: ","executor remove_node failed while preserving primary");}throw;}
 try{executor.remove_node(node);}catch(...){removal_failed=true;}
 auto success=!bad&&gets==1&&sets==1;b.reset();a.reset();return success?(removal_failed?5:0):110;
}
template<class S,class Check>int once(rclcpp::Node::SharedPtr const& node,char const* resource,std::shared_ptr<typename S::Request> request,Check check){
 auto client=node->create_client<S>(resource);auto until=std::chrono::steady_clock::now()+std::chrono::seconds(10);
 if(!client->wait_for_service(std::chrono::seconds(3)))return 110;
 auto pending=client->async_send_request(request); // genuine native ID, exactly one send
 auto remaining=until-std::chrono::steady_clock::now();
 if(remaining<=decltype(remaining)::zero()||rclcpp::spin_until_future_complete(node,pending.future,remaining)!=rclcpp::FutureReturnCode::SUCCESS){
  // This is LOCAL pending bookkeeping; it cannot cancel an admitted native send.
  (void)client->remove_pending_request(pending.request_id);return 110;}
 auto result=pending.future.get();return result&&check(*result)?0:22;
}
int client(rclcpp::Node::SharedPtr const& node){
 auto get=std::make_shared<Get::Request>();get->structure_needs_at_least_one_member=0;
 int first=once<Get>(node,"/uwvm_custom/get_float32_list",get,[](auto const& v){return response(v)&&v.datas==std::vector<float>{1.25f,-2.5f};});if(first)return first;
 auto set=std::make_shared<Set::Request>();set->datas={-32768,0,32767};
 return once<Set>(node,"/uwvm_custom/set_int16_list",set,[](auto const& v){return response(v);});
}
}
int main(int argc,char** argv){
 if(argc!=2||(std::strcmp(argv[1],"server")&&std::strcmp(argv[1],"client")))return 22;
 bool active{};int primary=5;
 try{rclcpp::init(argc,argv);active=true;auto node=std::make_shared<rclcpp::Node>("uwvm_custom_service_native_peer");
  primary=!std::strcmp(argv[1],"server")?server(node):client(node);node.reset();}
 catch(std::bad_alloc const&){primary=12;}catch(std::exception const&e){uwvm2::ros_gateway::native_io::diagnostic("native peer: ",e.what());primary=5;}catch(...){primary=5;}
 if(active){try{rclcpp::shutdown();}catch(...){if(!primary)primary=5;}}
 if(!primary&&!uwvm2::ros_gateway::native_io::write_all(STDOUT_FILENO,"PASS_NATIVE_XARM_COMPLETE_VALUES_NOT_GUEST_OR_ROBOT_ACCEPTANCE\n"))primary=5;return primary;
}
