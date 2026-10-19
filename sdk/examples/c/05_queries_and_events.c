/** @file
 * @brief Runnable allocation-free parameter/graph/time/log/TF/lifecycle application.
 * Select ROSWASM_EXAMPLE_CASE0..25 as documented in ADVANCED.md. Default0 gets
 * one Float64 parameter; it returns13 when no alias is explicitly granted.
 * The complete response buffer is1024 bytes. Views borrow it until next use.
 * Events run one poll;11 is idle, never event-delivery acceptance. Missing ROS
 * version entries return95 before alias acquisition; no API simulates support.
 */
#include "example_support.h"
#ifndef ROSWASM_EXAMPLE_CASE
#define ROSWASM_EXAMPLE_CASE 0
#endif
#if ROSWASM_EXAMPLE_CASE < 0 || ROSWASM_EXAMPLE_CASE > 25
#error "ROSWASM_EXAMPLE_CASE must be 0..25"
#endif
/** @brief Choose only a compile-time operation; every alias is independently granted. */
int main(void) {
    roswasmc_node node;roswasmc_capability owner;uint32_t error;unsigned char bytes[1024],scratch[1024];
    roswasmc_string text;roswasmc_parameter_value parameter={0};roswasmc_transform transform={0};
    roswasmc_typed_parameter_event event;roswasmc_lifecycle_state lifecycle;uint64_t now=0;int accepted=0;
    error=example_node(&node);if(error) return (int)error;roswasmc_capability_init(&owner,&node.context);
#if ROSWASM_EXAMPLE_CASE <= 8
    {static roswasmc_operation const operations[]={ROSWASMC_PARAMETER_GET,ROSWASMC_PARAMETER_SET,ROSWASMC_PARAMETER_DELETE,ROSWASMC_PARAMETER_EXISTS,
        ROSWASMC_PARAMETER_LIST,ROSWASMC_PARAMETER_SEARCH,ROSWASMC_PARAMETER_DESCRIBE,ROSWASMC_PARAMETER_DECLARE,ROSWASMC_PARAMETER_UNDECLARE};
     static char const* const aliases[]={"parameter-get","parameter-set","parameter-delete","parameter-exists","parameter-list","parameter-search","parameter-describe","parameter-declare","parameter-undeclare"};
     static size_t const lengths[]={13,13,16,16,14,16,18,17,19};
     roswasmc_string alias={aliases[ROSWASM_EXAMPLE_CASE],lengths[ROSWASM_EXAMPLE_CASE]};roswasmc_operation operation=operations[ROSWASM_EXAMPLE_CASE];
     error=roswasmc_node_create_parameters(&node,&owner,alias,operation);
     if(!error) {
        if(ROSWASM_EXAMPLE_CASE==0) error=roswasmc_parameter_read(&owner,ROSWASMC_PARAM_FLOAT64,bytes,sizeof bytes,&parameter);
        else if(ROSWASM_EXAMPLE_CASE==1||ROSWASM_EXAMPLE_CASE==7) {parameter.tag=ROSWASMC_PARAM_FLOAT64;parameter.real=1.25;error=roswasmc_parameter_write(&owner,operation,&parameter,scratch,sizeof scratch,bytes,sizeof bytes);}
        else if(ROSWASM_EXAMPLE_CASE==3) error=roswasmc_parameter_exists_value(&owner,&accepted);
        else if(ROSWASM_EXAMPLE_CASE==2||ROSWASM_EXAMPLE_CASE==8) error=roswasmc_request(&owner,operation,0,0,bytes,sizeof bytes).error;
        else error=roswasmc_parameter_metadata(&owner,operation,bytes,sizeof bytes,&text);
     }}
#elif ROSWASM_EXAMPLE_CASE == 9
    {roswasmc_string alias={"parameter-events",16};error=roswasmc_node_create_parameter_events(&node,&owner,alias);}
    if(!error) {roswasmc_result result=roswasmc_poll(&owner,bytes,sizeof bytes);error=result.error;
        if(!error) error=roswasmc_decode_typed_parameter_event(bytes,(size_t)result.length,&event);}
#elif ROSWASM_EXAMPLE_CASE <= 16
    {static roswasmc_operation const operations[]={ROSWASMC_GRAPH_INSPECT_TOPIC,ROSWASMC_GRAPH_INSPECT_NODE,ROSWASMC_GRAPH_INSPECT_SERVICE,ROSWASMC_GRAPH_INSPECT_ACTION,
        ROSWASMC_GRAPH_LIST_TOPICS,ROSWASMC_GRAPH_LIST_NODES,ROSWASMC_GRAPH_LIST_SERVICES};
     static char const* const aliases[]={"graph-topic","graph-node","graph-service","graph-action","graph-topics","graph-nodes","graph-services"};
     static size_t const lengths[]={11,10,13,12,12,11,14};unsigned index=ROSWASM_EXAMPLE_CASE-10;
     roswasmc_string alias={aliases[index],lengths[index]};error=roswasmc_node_create_graph(&node,&owner,alias,operations[index]);
     if(!error) error=roswasmc_graph_read(&owner,operations[index],bytes,sizeof bytes,&text);}
#elif ROSWASM_EXAMPLE_CASE <= 20
    {static roswasmc_operation const operations[]={ROSWASMC_TIME_READ_ROS,ROSWASMC_TIME_READ_WALL,ROSWASMC_TIME_READ_SYSTEM,ROSWASMC_TIME_READ_STEADY};
     static char const* const aliases[]={"ros-time","wall-time","system-time","steady-time"};static size_t const lengths[]={8,9,11,11};unsigned index=ROSWASM_EXAMPLE_CASE-17;
     roswasmc_string alias={aliases[index],lengths[index]};error=roswasmc_node_create_clock(&node,&owner,alias,operations[index]);
     if(!error) error=roswasmc_clock_read(&owner,operations[index],&now);}
#elif ROSWASM_EXAMPLE_CASE == 21
    {roswasmc_string alias={"log-info",8};error=roswasmc_node_create_logger(&node,&owner,alias);}
    if(!error) {roswasmc_string message={"guest application",17};error=roswasmc_logger_write(&owner,message,scratch,sizeof scratch);}
#elif ROSWASM_EXAMPLE_CASE == 22 || ROSWASM_EXAMPLE_CASE == 23
    {roswasmc_string alias={ROSWASM_EXAMPLE_CASE==22?"tf-lookup":"tf-broadcast",ROSWASM_EXAMPLE_CASE==22?9:12};
     error=roswasmc_node_create_tf(&node,&owner,alias,ROSWASM_EXAMPLE_CASE==22?ROSWASMC_TF_LOOKUP:ROSWASMC_TF_BROADCAST);}
    if(!error) {
        if(ROSWASM_EXAMPLE_CASE==22) error=roswasmc_tf_read(&owner,0,bytes,sizeof bytes,&transform);
        else {roswasmc_string parent={"map",3},child={"base_link",9};transform.parent_frame=parent;transform.child_frame=child;transform.quaternion[3]=1.0;
            error=roswasmc_tf_write(&owner,&transform,scratch,sizeof scratch);}
    }
#else
    {roswasmc_string alias={ROSWASM_EXAMPLE_CASE==24?"lifecycle-get":"lifecycle-change",ROSWASM_EXAMPLE_CASE==24?13:16};
     error=roswasmc_node_create_lifecycle(&node,&owner,alias,ROSWASM_EXAMPLE_CASE==24?ROSWASMC_LIFECYCLE_GET_STATE:ROSWASMC_LIFECYCLE_CHANGE_STATE);}
    if(!error) error=ROSWASM_EXAMPLE_CASE==24?roswasmc_lifecycle_read(&owner,bytes,sizeof bytes,&lifecycle):roswasmc_lifecycle_change(&owner,&accepted);
#endif
    return (int)example_finish(&owner,error);
}
