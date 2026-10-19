/* Trusted source selection, NOT a Host/native authorization or waitset.
 * The unchanged original rclc mains literally use this node and /addtwoints.
 * Root must separately admit exact same native Service graph, canonical codec,
 * default QoS, resource pairing and each alias. No graph-list readiness proxy. */
#include <roswasm/ros2.h>
#include <stdio.h>
static uint32_t rclc_service_step(void*user,uint64_t requested_ns){
 unsigned char byte;(void)user;(void)requested_ns;
 /* Only pacing/shutdown credits. R is NOT elapsed time; real sleep separately
  * queries the granted steady clock before/after every external driver step.
  * The owning Host driver must enforce a finite read/whole-process deadline. */
 if(fread(&byte,1,1,stdin)!=1)return 110;return byte=='R'?0:byte=='Q'?125:22;
}
#define SERVICE_QOS {RMW_QOS_POLICY_HISTORY_KEEP_LAST,10,RMW_QOS_POLICY_RELIABILITY_RELIABLE,RMW_QOS_POLICY_DURABILITY_VOLATILE,{0,0},{0,0},RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT,{0,0},false}
uwvm_rclc_selection const*uwvm_rclc_selected_configuration(void){
 static uwvm_rclc_node_binding const nodes[]={{"add_twoints_client_rclc",""},{"checked-service",""}};
 static uwvm_rclc_service_binding const bindings[]={
  {"add_twoints_client_rclc","","/addtwoints","example_interfaces/srv/AddTwoInts",UWVM_RCLC_SERVICE_SERVER,"original-advertise","original-respond",SERVICE_QOS,16,4},
  {"add_twoints_client_rclc","","/addtwoints","example_interfaces/srv/AddTwoInts",UWVM_RCLC_SERVICE_CLIENT,"original-call","original-observe",SERVICE_QOS,16,4},
  {"checked-service","","/rclc_profile/add","example_interfaces/srv/AddTwoInts",UWVM_RCLC_SERVICE_SERVER,"add-advertise","add-respond",SERVICE_QOS,16,4},
  {"checked-service","","/rclc_profile/add","example_interfaces/srv/AddTwoInts",UWVM_RCLC_SERVICE_CLIENT,"add-call","add-observe",SERVICE_QOS,16,4},
  {"checked-service","","/rclc_profile/trigger","std_srvs/srv/Trigger",UWVM_RCLC_SERVICE_SERVER,"trigger-advertise","trigger-respond",SERVICE_QOS,261,4},
  {"checked-service","","/rclc_profile/trigger","std_srvs/srv/Trigger",UWVM_RCLC_SERVICE_CLIENT,"trigger-call","trigger-observe",SERVICE_QOS,261,4},
  {"checked-service","","/rclc_profile/bool","std_srvs/srv/SetBool",UWVM_RCLC_SERVICE_SERVER,"bool-advertise","bool-respond",SERVICE_QOS,261,4},
  {"checked-service","","/rclc_profile/bool","std_srvs/srv/SetBool",UWVM_RCLC_SERVICE_CLIENT,"bool-call","bool-observe",SERVICE_QOS,261,4}};
 static struct uwvm_rclc_service_profile profile;
 static uwvm_rclc_selection selection;
 /* Single-threaded setup; this storage and its pointed-to rows are immutable
  * from the first support construction until the last Context is retired. */
 if(!selection.backend){profile.events=roswasmc_ros2_event_backend();profile.bindings=bindings;profile.binding_count=8;profile.maximum_sleep_ns=3000000000u;profile.maximum_sleep_steps=512;
  selection.backend=roswasmc_ros2_backend();selection.nodes=nodes;selection.node_count=2;selection.steady_clock_alias="service-steady";
  selection.step=rclc_service_step;selection.maximum_spin_steps=512;selection.services=&profile;}
 return &selection;
}
#undef SERVICE_QOS
