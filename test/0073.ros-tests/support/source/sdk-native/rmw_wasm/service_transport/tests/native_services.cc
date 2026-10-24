/* SPDX-License-Identifier: Apache-2.0
 * Separate ORIGINAL native ROS2 counterparts, UNEXECUTED. No Guest provider,
 * raw model, fake native handle, TypeSupport or callback lease is supplied.
 * Root owns actual SDK/ELF/MD/pidfd/tree/domain/network/deadline/cleanup.
 */
#include <rclcpp/rclcpp.hpp>
#include <std_srvs/srv/empty.hpp>
#include <example_interfaces/srv/add_two_ints.hpp>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <memory>
#include <thread>
#include <type_traits>
#include <exception>
#include <string_view>
using namespace std::chrono_literals;
static bool check(bool b){if(!b)std::fputs("FAIL_NATIVE_SERVICE_VALUE\n",stderr);return b;}
template<class Service>static int peer(std::shared_ptr<rclcpp::Node> const&node,bool server,char const*name,bool add){
 int result=110;
 if(server){bool completed=false,valid=false;
 auto service=node->create_service<Service>(name,[&](std::shared_ptr<rmw_request_id_t> header,std::shared_ptr<typename Service::Request> request,std::shared_ptr<typename Service::Response> response){
 if constexpr(std::is_same_v<Service,example_interfaces::srv::AddTwoInts>){valid=request->a==17&&request->b==-9;response->sum=8;}else{valid=request->structure_needs_at_least_one_member==0;response->structure_needs_at_least_one_member=0;}
 // This actual original callback header has GUID/sequence only. Do NOT invent
 // source/receive timestamps that this rclcpp callback already discarded.
 std::printf("NATIVE_REQUEST sequence=%lld writer=",(long long)header->sequence_number);for(auto b:header->writer_guid)std::printf("%02x",b);std::putchar('\n');completed=true;
 });
 auto until=std::chrono::steady_clock::now()+10s;
 while(!completed&&std::chrono::steady_clock::now()<until){rclcpp::spin_some(node);std::this_thread::sleep_for(1ms);}
 if(completed&&check(valid))result=0;service.reset();
 }else{auto client=node->create_client<Service>(name);if(client->wait_for_service(5s)){
 auto request=std::make_shared<typename Service::Request>();if constexpr(std::is_same_v<Service,example_interfaces::srv::AddTwoInts>){request->a=17;request->b=-9;}
 auto pending=client->async_send_request(request);auto future=pending.future.share();
 if(rclcpp::spin_until_future_complete(node,future,5s)==rclcpp::FutureReturnCode::SUCCESS){auto response=future.get();bool valid=false;
 if constexpr(std::is_same_v<Service,example_interfaces::srv::AddTwoInts>)valid=response->sum==8;else valid=response->structure_needs_at_least_one_member==0;
 if(check(valid))result=0;
 }else{(void)client->remove_pending_request(pending.request_id);} // Local bookkeeping, NOT native cancellation.
 }client.reset();}
 if(!result)std::printf("PASS_ORIGINAL_NATIVE_%s_%s_NOT_GUEST_ACCEPTANCE\n",server?"SERVER":"CLIENT",add?"ADDTWOINTS":"EMPTY");return result;
}
int main(int argc,char**argv){
 if(argc!=5||std::string_view(argv[1])!="--owned-native-fixture"||((std::strcmp(argv[2],"client")!=0)&&(std::strcmp(argv[2],"server")!=0))||((std::strcmp(argv[3],"empty")!=0)&&(std::strcmp(argv[3],"add")!=0))||argv[4][0]!='/')return 22;
 rclcpp::init(argc,argv);int result=5;try{auto node=std::make_shared<rclcpp::Node>("uwvm_actual_native_service_peer");bool server=!std::strcmp(argv[2],"server"),add=!std::strcmp(argv[3],"add");result=add?peer<example_interfaces::srv::AddTwoInts>(node,server,argv[4],true):peer<std_srvs::srv::Empty>(node,server,argv[4],false);node.reset();}
 catch(std::exception const&e){std::fprintf(stderr,"Native service exception: %s\n",e.what());result=5;}catch(...){result=5;}
 rclcpp::shutdown();return result;
}
