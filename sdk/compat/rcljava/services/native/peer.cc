/** Actual installed ROS2 native peer; never includes the Guest compatibility
 * classes or a fake type provider. The Root supplies finite wall/resource/pidfd,
 * SDK/include/MD/link/ELF/map/namespace and isolated-domain qualification.
 * I creates one explicit endpoint, M checks native client availability (server
 * creation is labelled separately), P submits once/progresses once, F verifies
 * complete fields and closes. Every mode is a fresh process, <=128 credits.
 */
#include <rclcpp/rclcpp.hpp>
#include <example_interfaces/srv/add_two_ints.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <std_srvs/srv/set_bool.hpp>
#include <chrono>
#include <cstdio>
#include <memory>
#include <optional>
#include <string_view>
#include <type_traits>
#include <stdexcept>
template<class S> char const* resource(){
 if constexpr(std::is_same_v<S,example_interfaces::srv::AddTwoInts>)return "/add_two_ints";
 else if constexpr(std::is_same_v<S,std_srvs::srv::Trigger>)return "/rcljava_trigger";
 else return "/rcljava_set_bool";
}
template<class S> void prepare(typename S::Request& q){
 if constexpr(std::is_same_v<S,example_interfaces::srv::AddTwoInts>){q.a=41;q.b=1;}
 else if constexpr(std::is_same_v<S,std_srvs::srv::SetBool>)q.data=true;
}
template<class S> void answer(typename S::Request const& q,typename S::Response& r){
 if constexpr(std::is_same_v<S,example_interfaces::srv::AddTwoInts>){if(q.a!=41||q.b!=1)throw std::runtime_error("complete AddTwoInts request");r.sum=42;}
 else if constexpr(std::is_same_v<S,std_srvs::srv::Trigger>){r.success=true;r.message="ready";}
 else{if(!q.data)throw std::runtime_error("complete SetBool request");r.success=true;r.message="accepted";}
}
template<class S> void verify(typename S::Response const& r){
 if constexpr(std::is_same_v<S,example_interfaces::srv::AddTwoInts>){if(r.sum!=42)throw std::runtime_error("sum");}
 else if constexpr(std::is_same_v<S,std_srvs::srv::Trigger>){if(!r.success||r.message!="ready")throw std::runtime_error("complete Trigger response");}
 else if(!r.success||r.message!="accepted")throw std::runtime_error("complete SetBool response");
}
template<class S> int run(bool client){
 std::shared_ptr<rclcpp::Node> node;typename rclcpp::Service<S>::SharedPtr server;typename rclcpp::Client<S>::SharedPtr caller;
 std::optional<typename rclcpp::Client<S>::FutureAndRequestId> future;bool initialized{},matched{},submitted{},complete{};unsigned calls{};
 auto finish=[&]{future.reset();caller.reset();server.reset();node.reset();if(initialized){rclcpp::shutdown();initialized=false;}};
 try{for(unsigned credits{};credits!=128;++credits){int c=std::getchar();if(c==EOF){finish();return 110;}
  if(c=='I'){if(initialized){finish();return 17;}rclcpp::init(0,nullptr);initialized=true;node=std::make_shared<rclcpp::Node>(client?"rcljava_service_native_client":"rcljava_service_native_server");
   if(client)caller=node->create_client<S>(resource<S>());
   else server=node->create_service<S>(resource<S>(),[&](typename S::Request::SharedPtr q,typename S::Response::SharedPtr r){if(calls++)throw std::runtime_error("duplicate native request");answer<S>(*q,*r);complete=true;std::puts("NATIVE_COMPLETE_REQUEST_RESPONSE");std::fflush(stdout);});
   std::puts("INITIALIZED");
  }else if(c=='M'){if(!initialized){finish();return 22;}if(client&&!caller->wait_for_service(std::chrono::nanoseconds(0))){std::puts("NOT_READY");std::fflush(stdout);continue;}matched=true;std::puts(client?"NATIVE_SERVICE_AVAILABLE":"NATIVE_SERVER_CREATED_NOT_CLIENT_MATCH_PROOF");
  }else if(c=='P'){if(!initialized||!matched){finish();return 22;}
   if(client){if(complete){finish();return 17;}if(!submitted){auto q=std::make_shared<typename S::Request>();prepare<S>(*q);future.emplace(caller->async_send_request(q));submitted=true;}
    if(rclcpp::spin_until_future_complete(node,future->future,std::chrono::nanoseconds(0))==rclcpp::FutureReturnCode::SUCCESS){verify<S>(*future->future.get());complete=true;std::puts("NATIVE_COMPLETE_RESPONSE");}
   }else rclcpp::spin_some(node);
  }else if(c=='F'){bool pass=initialized&&matched&&complete;finish();if(!pass)return 22;std::puts("CLOSED_COMPLETE_VALUE_NOT_WHOLE_LANE_QUALIFICATION");return 0;
  }else{finish();return 22;}std::fflush(stdout);
 }finish();return 110;}catch(...){finish();return 22;}
}
int main(int argc,char** argv){if(argc!=3)return 22;bool client=std::string_view{argv[1]}=="client";if(!client&&std::string_view{argv[1]}!="server")return 22;
 if(std::string_view{argv[2]}=="add")return run<example_interfaces::srv::AddTwoInts>(client);
 if(std::string_view{argv[2]}=="trigger")return run<std_srvs::srv::Trigger>(client);
 if(std::string_view{argv[2]}=="setbool")return run<std_srvs::srv::SetBool>(client);return 22;}
