/** Real installed native rclcpp/example_interfaces witness. No Guest include map.
 * Root owns all wall deadlines, pidfds, SDK/MD/link/ELF/maps and native policy.
 * I creates the exact endpoint; M checks actual native availability for client;
 * P performs ONE bounded executor pass (and the first client P submits once);
 * F requires complete values/closure. Characters are explicit driver credits.
 */
#include <rclcpp/rclcpp.hpp>
#include <example_interfaces/srv/add_two_ints.hpp>
#include <chrono>
#include <cstdio>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <limits>
int main(int argc,char** argv){
    if(argc!=2)return 22;bool client=std::string_view{argv[1]}=="client";
    if(!client&&std::string_view{argv[1]}!="server")return 22;
    using S=example_interfaces::srv::AddTwoInts;std::shared_ptr<rclcpp::Node> node;
    rclcpp::Service<S>::SharedPtr server;rclcpp::Client<S>::SharedPtr caller;
    std::optional<rclcpp::Client<S>::FutureAndRequestId> future;bool initialized{},matched{},submitted{},complete{};unsigned calls{};
    auto finish=[&]{future.reset();caller.reset();server.reset();node.reset();if(initialized)rclcpp::shutdown();};
    try{
        for(unsigned credits{};credits!=128;++credits){int c=std::getchar();if(c==EOF){finish();return 110;}
            if(c=='I'){
                if(initialized){finish();return 17;}rclcpp::init(0,nullptr);initialized=true;node=std::make_shared<rclcpp::Node>(client?"rclcpp_service_native_client":"rclcpp_service_native_server");
                if(client)caller=node->create_client<S>("/uwvm_rclcpp_service/add_two_ints");
                else server=node->create_service<S>("/uwvm_rclcpp_service/add_two_ints",[&](S::Request::SharedPtr request,S::Response::SharedPtr response){
                    if(request->a!=41||request->b!=1||calls++)throw std::runtime_error("complete request fixture mismatch or duplicate");response->sum=42;complete=true;std::puts("NATIVE_REQUEST a=41 b=1 RESPONSE sum=42");std::fflush(stdout);});
                std::puts("INITIALIZED");std::fflush(stdout);
            }else if(c=='M'){
                if(!initialized){finish();return 22;}if(client&&!caller->wait_for_service(std::chrono::nanoseconds(0))){std::puts("NOT_READY");std::fflush(stdout);continue;}
                matched=true;std::puts(client?"NATIVE_SERVICE_AVAILABLE":"NATIVE_SERVER_CREATED_NOT_CLIENT_MATCH_PROOF");std::fflush(stdout);
            }else if(c=='P'){
                if(!initialized||!matched){finish();return 22;}
                if(client){
                    if(complete){finish();return 17;}
                    if(!submitted){auto q=std::make_shared<S::Request>();q->a=41;q->b=1;future.emplace(caller->async_send_request(q));submitted=true;}
                    auto status=rclcpp::spin_until_future_complete(node,*future,std::chrono::nanoseconds(0));
                    if(status==rclcpp::FutureReturnCode::SUCCESS&&!complete){auto value=future->get();if(value->sum!=42)throw std::runtime_error("complete response sum mismatch");complete=true;std::puts("NATIVE_RESPONSE sum=42");}
                }else rclcpp::spin_some(node);
                std::fflush(stdout);
            }else if(c=='F'){
                bool pass=initialized&&matched&&complete;finish();if(!pass)return 22;std::puts("CLOSED_COMPLETE_VALUE_WITNESS_NOT_WHOLE_LANE_QUALIFICATION");return 0;
            }else{finish();return 22;}
        }
        finish();return 110;
    }catch(...){finish();return 22;}
}
