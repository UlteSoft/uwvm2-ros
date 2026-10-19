/** Finite trusted original NVIDIA AprilTagNode owner. No Wasm device access. */
#include <isaac_ros_apriltag/apriltag_node.hpp>
#include <rclcpp/rclcpp.hpp>
#include "decimal.hpp"
#include "gateways/common/native_io.h"
#include <chrono>
#include <algorithm>
#include <cstdint>
#include <new>
#include <memory>
#include <string>
#include <string_view>
#include <fast_io.h>
#ifndef UWVM_NVIDIA_ENABLE_NATIVE_DETECTOR
#define UWVM_NVIDIA_ENABLE_NATIVE_DETECTOR 0
#endif
static_assert(UWVM_NVIDIA_ENABLE_NATIVE_DETECTOR==0||UWVM_NVIDIA_ENABLE_NATIVE_DETECTOR==1);
int main(int argc,char**argv){
 if(!UWVM_NVIDIA_ENABLE_NATIVE_DETECTOR)return 13;
 if(argc!=6)return 22;std::uint64_t domain{},milliseconds{};
 if(!uwvm_default_control_detail::decimal(argv[1],domain)||domain>232||!uwvm_default_control_detail::decimal(argv[2],milliseconds)||!milliseconds||milliseconds>10000)return 22;
 for(int i=3;i<6;++i){auto s=std::string_view{argv[i]};if(s.empty()||s.front()!='/'||s.size()>255)return 22;}
 std::shared_ptr<rclcpp::Context> context;std::shared_ptr<nvidia::isaac_ros::apriltag::AprilTagNode> node;
 std::unique_ptr<rclcpp::executors::SingleThreadedExecutor> executor;
 std::uint32_t primary{},executor_cleanup{},node_cleanup{},context_cleanup{};
 auto until=std::chrono::steady_clock::now()+std::chrono::milliseconds(milliseconds);
 try{
  context=std::make_shared<rclcpp::Context>();rclcpp::InitOptions init;init.set_domain_id(domain);context->init(0,nullptr,init);
  rclcpp::NodeOptions options;options.context(context).arguments({"--ros-args","-r",std::string("image:=")+argv[3],"-r",std::string("camera_info:=")+argv[4],"-r",std::string("tag_detections:=")+argv[5]});
  // Genuine default constructors, CUDA/VPI backends and full original fields.
  // Do not turn off rosout/parameter services or replace Image::data with an
  // assumed CPU vector. Original current backend promotes CPU buffers itself.
  node=std::make_shared<nvidia::isaac_ros::apriltag::AprilTagNode>(options);
  rclcpp::ExecutorOptions eo;eo.context=context;executor=std::make_unique<rclcpp::executors::SingleThreadedExecutor>(eo);executor->add_node(node);
  while(context->is_valid()){
   auto remaining=until-std::chrono::steady_clock::now();if(remaining<=remaining.zero())break;
   executor->spin_once(std::min(std::chrono::duration_cast<std::chrono::nanoseconds>(remaining),std::chrono::nanoseconds{50000000}));
  }
 }catch(std::bad_alloc const&){primary=12;}catch(...){primary=5;}
 // Separate real retirement attempts; an error is not overwritten by cleanup.
 try{if(executor){if(node)executor->remove_node(node);executor.reset();}}catch(...){executor_cleanup=5;}
 try{node.reset();}catch(...){node_cleanup=5;}
 try{if(context&&context->is_valid())if(!context->shutdown("explicit finite NVIDIA owner retired"))context_cleanup=125;}catch(...){context_cleanup=5;}
 auto out=fast_io::concat_std("NVIDIA_DETECTOR primary=",primary," executor_close=",executor_cleanup," node_close=",node_cleanup," context_close=",context_cleanup,"\n");
 bool printed=uwvm2::ros_gateway::native_io::write_all(2,out);
 return primary?primary:executor_cleanup?executor_cleanup:node_cleanup?node_cleanup:context_cleanup?context_cleanup:printed?0:5;
}
