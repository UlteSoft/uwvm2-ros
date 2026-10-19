/* SPDX-License-Identifier: Apache-2.0 */
/** Genuine original xArm generated-object opposite, SOURCE ONLY. This standalone
 * Native process owns its original rclcpp authority, not Guest capabilities. It
 * targets one laboratory resource, not a robot/controller/network safety claim.
 * Preserve exact original .srv/defaults. Native deferred callback does not prove
 * the same-take full timestamps required by the separate direct-RCL receipt.
 */
#include <rclcpp/rclcpp.hpp>
#include <xarm_msgs/srv/get_set_modbus_data.hpp>
#include "gateways/common/native_io.h"
#include <chrono>
#include <cstring>
using M=xarm_msgs::srv::GetSetModbusData;
static std::string message(){return std::string("native-\xC2\xB5",9);}
static bool default_request(M::Request const& q){return q.modbus_data.empty()&&q.modbus_length==0&&q.ret_length==0&&q.host_id==9&&!q.is_transparent_transmission&&!q.use_503_port;}
static bool value_request(M::Request const& q){return q.modbus_data==std::vector<std::uint8_t>{0,128,255}&&q.modbus_length==-3&&q.ret_length==64&&q.host_id==10&&q.is_transparent_transmission&&q.use_503_port;}
static bool response(M::Response const& s){return s.ret==-7&&s.message==message()&&s.ret_data==std::vector<std::uint8_t>{127,0,255};}
static int server(rclcpp::Node::SharedPtr const& node){
 unsigned count{};bool bad{};auto service=node->create_service<M>("/uwvm_custom/get_set_modbus_data",
 [&](std::shared_ptr<M::Request> q,std::shared_ptr<M::Response> s){
  if(!q||!s||++count!=1||(!default_request(*q)&&!value_request(*q))){bad=true;return;}
  s->ret=-7;s->message=message();s->ret_data={127,0,255};});
 rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);bool cleanup_failed{};
 auto until=std::chrono::steady_clock::now()+std::chrono::seconds(15);
 try{while(rclcpp::ok()&&std::chrono::steady_clock::now()<until&&!bad&&count!=1)executor.spin_once(std::chrono::milliseconds(100));}
 catch(...){try{executor.remove_node(node);}catch(...){uwvm2::ros_gateway::native_io::diagnostic("modbus peer cleanup: ","remove_node failed preserving primary");}throw;}
 try{executor.remove_node(node);}catch(...){cleanup_failed=true;}service.reset();
 return bad?22:count!=1?110:cleanup_failed?5:0;
}
static int client(rclcpp::Node::SharedPtr const& node,bool defaults){
 auto q=std::make_shared<M::Request>();if(!default_request(*q))return 22;
 if(!defaults){q->modbus_data={0,128,255};q->modbus_length=-3;q->ret_length=64;q->host_id=10;q->is_transparent_transmission=true;q->use_503_port=true;}
 if(!(defaults?default_request(*q):value_request(*q)))return 22;
 auto c=node->create_client<M>("/uwvm_custom/get_set_modbus_data");auto until=std::chrono::steady_clock::now()+std::chrono::seconds(10);
 if(!c->wait_for_service(std::chrono::seconds(3)))return 110;
 auto pending=c->async_send_request(q); // Exactly ONE send. Never replay timeout/error.
 auto remaining=until-std::chrono::steady_clock::now();
 if(remaining<=decltype(remaining)::zero()||rclcpp::spin_until_future_complete(node,pending.future,remaining)!=rclcpp::FutureReturnCode::SUCCESS){
  (void)c->remove_pending_request(pending.request_id);return 110;} // Local bookkeeping, not native cancellation.
 auto s=pending.future.get();return s&&response(*s)?0:22;
}
int main(int argc,char** argv){
 if(argc!=2||(std::strcmp(argv[1],"server")&&std::strcmp(argv[1],"client")&&std::strcmp(argv[1],"client-default")))return 22;
 bool active{};int primary=5;try{rclcpp::init(argc,argv);active=true;auto node=std::make_shared<rclcpp::Node>("uwvm_custom_scalar_native_peer");
  primary=!std::strcmp(argv[1],"server")?server(node):client(node,!std::strcmp(argv[1],"client-default"));node.reset();}
 catch(std::bad_alloc const&){primary=12;}catch(std::exception const& e){uwvm2::ros_gateway::native_io::diagnostic("modbus native peer: ",e.what());primary=5;}catch(...){primary=5;}
 if(active)try{rclcpp::shutdown();}catch(...){if(!primary)primary=5;}
 if(!primary&&!uwvm2::ros_gateway::native_io::write_all(STDOUT_FILENO,"PASS_NATIVE_MODBUS_COMPLETE_DEFAULT_VALUES_NOT_GUEST_OR_ROBOT_ACCEPTANCE\n"))primary=5;return primary;
}
