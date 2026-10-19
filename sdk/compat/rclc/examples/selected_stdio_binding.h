/* Source-selected REAL ROS2 import backend. It grants nothing by itself.
 * The Host must separately grant each exact resolved endpoint/type/QoS/alias.
 * Stdio R permits a pass, Q requests local shutdown; EOF/other bytes fail.
 * Include this only into compat.c using UWVM_RCLC_COMPAT_SELECTION_HEADER. */
#include <roswasm/ros2.h>
#include <stdio.h>
static uint32_t uwvm_rclc_stdio_step(void*user,uint64_t timeout_ns){
    unsigned char byte;
    (void)user;
    (void)timeout_ns;
    if(fread(&byte,1,1,stdin)!=1)return 110;
    return byte=='R'?0:byte=='Q'?125:22;
}
#define UWVM_RCLC_DEFAULT_QOS {RMW_QOS_POLICY_HISTORY_KEEP_LAST,10,RMW_QOS_POLICY_RELIABILITY_RELIABLE,RMW_QOS_POLICY_DURABILITY_VOLATILE,{0,0},{0,0},RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT,{0,0},false}
uwvm_rclc_selection const*uwvm_rclc_selected_configuration(void){
    static uwvm_rclc_node_binding const nodes[]={
        {
            "node_0","executor_examples"
        }
    }
    ;
    static uwvm_rclc_topic_binding const topics[]={
        {
            "node_0","executor_examples","topic_0","std_msgs/msg/String",UWVM_RCLC_PUBLISH,"rclc-pub-0",UWVM_RCLC_DEFAULT_QOS,1028
        }
        ,
        {
            "node_0","executor_examples","topic_0","std_msgs/msg/String",UWVM_RCLC_SUBSCRIBE,"rclc-sub-0",UWVM_RCLC_DEFAULT_QOS,1028
        }
        ,
        {
            "node_0","executor_examples","topic_foo","std_msgs/msg/String",UWVM_RCLC_PUBLISH,"rclc-pub-foo",UWVM_RCLC_DEFAULT_QOS,1028
        }
        ,
        {
            "node_0","executor_examples","topic_foo","std_msgs/msg/String",UWVM_RCLC_SUBSCRIBE,"rclc-sub-foo",UWVM_RCLC_DEFAULT_QOS,1028
        }
        ,
        {
            "node_0","executor_examples","topic_bar","std_msgs/msg/String",UWVM_RCLC_PUBLISH,"rclc-pub-bar",UWVM_RCLC_DEFAULT_QOS,1028
        }
        ,
        {
            "node_0","executor_examples","topic_bar","std_msgs/msg/String",UWVM_RCLC_SUBSCRIBE,"rclc-sub-bar",UWVM_RCLC_DEFAULT_QOS,1028
        }
        ,
        {
            "node_0","executor_examples","topic_baz","std_msgs/msg/String",UWVM_RCLC_PUBLISH,"rclc-pub-baz",UWVM_RCLC_DEFAULT_QOS,1028
        }
        ,
        {
            "node_0","executor_examples","topic_baz","std_msgs/msg/String",UWVM_RCLC_SUBSCRIBE,"rclc-sub-baz",UWVM_RCLC_DEFAULT_QOS,1028
        }
    }
    ;
    static uwvm_rclc_selection selection;
    /* This is called only during single-threaded support construction, before an
  * effect/lease. The static selection and its pointed-to arrays are then never
  * changed. Copying aliases or translating them into native grants is forbidden. */
    if(!selection.backend){
        selection.backend=roswasmc_ros2_backend();
        selection.topics=topics;
        selection.topic_count=8;
        selection.nodes=nodes;
        selection.node_count=1;
        selection.steady_clock_alias="rclc-steady";
        selection.step=uwvm_rclc_stdio_step;
        selection.maximum_spin_steps=4096;
    }
    return &selection;
}
#undef UWVM_RCLC_DEFAULT_QOS
