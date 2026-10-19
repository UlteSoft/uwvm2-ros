// SPDX-License-Identifier: Apache-2.0
// Complete source example using original Context/Node/GraphListener classes.
// This application explicitly chooses quiet options itself; no upstream default
// is patched. Two exact separately granted Clock aliases are mandatory inputs.
#include <rclcpp/rclcpp.hpp>
#include <uwvm_rmw/bridge.h>
#include <uwvm_rmw_c/provider.h>
#include <uwvm_rmw_platform.h>
#include <roswasm/ros2.h>
#include <cstdio>
#include <memory>
#include <exception>
void uwvm_threaded_original_graph_shutdown_contract(std::shared_ptr<rclcpp::Context>const&,rclcpp::NodeOptions const&);
static uwvm_rmw_profile selection;
static uwvm_rmw_c_profile provider={};
static uwvm_rmw_node_selection selected_node={"threaded_original_graph","/"};
int main(int argc,char**argv){
 if(argc!=3)return 2;
 uwvm_rmw_platform_profile platform={1,roswasmc_ros2_backend(),argv[1],argv[2],nullptr,0};
 unsigned failures=0;bool retained=false,selected=false;
 if(uwvm_rmw_platform_prepare(&platform))return 13;
 if(uwvm_rmw_platform_retain()){(void)uwvm_rmw_platform_close();return 17;}retained=true;
 selection.version=1;selection.backend=roswasmc_ros2_backend();selection.nodes=&selected_node;selection.node_count=1;
 selection.graph=uwvm_rmw_c_graph_provider();provider.version=1;
 selected=uwvm_rmw_install_profile(&selection)==RMW_RET_OK;
 if(!selected||uwvm_rmw_c_install_profile(&provider)!=RMW_RET_OK)++failures;
 else {
  try {
   // Actual explicit original APIs from the selected immutable SDK headers.
   rclcpp::InitOptions init;init.auto_initialize_logging(false);init.set_domain_id(0);
   auto context=std::make_shared<rclcpp::Context>();context->init(1,argv,init);
   rclcpp::NodeOptions options;options.context(context).enable_rosout(false)
    .start_parameter_services(false).start_parameter_event_publisher(false)
    // The original NodeTypeDescriptions default is independent of those flags.
    // This graph-only application explicitly declines its native service.
    .append_parameter_override("start_type_description_service",false);
   uwvm_threaded_original_graph_shutdown_contract(context,options);
   context.reset(); // options also retains it; both die before profile release
  }catch(std::exception const&error){std::fprintf(stderr,"Original graph contract: %s\n",error.what());++failures;}
  catch(...){++failures;}
 }
 // A retained original InitOptions/Context is not replaced by a copied bool.
 // Profile uninstall is the actual B lifetime check before platform retirement.
 if(selected&&uwvm_rmw_install_profile(nullptr)!=RMW_RET_OK)return 17;
 if(retained&&uwvm_rmw_platform_release())++failures;
 if(uwvm_rmw_platform_close())++failures;
 uwvm_rmw_platform_diagnostics d{};
 if(uwvm_rmw_platform_get_diagnostics(&d)||d.lifetime_references||d.close_failures||d.first_raw_error)++failures;
 if(failures)return 1;
 std::puts("PASS_ORIGINAL_GRAPH_LOCAL_SHUTDOWN_REQUIRES_SEPARATE_GUEST_QUALIFICATION");return 0;
}
