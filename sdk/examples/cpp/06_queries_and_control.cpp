/** @file
 * @brief Runnable typed C++ parameter/graph/time/log/TF/lifecycle application.
 * ROSWASM_EXAMPLE_CASE selects the same0..25 operation table as C05, except
 * parameters use String and case9 uses an owned TypedParameterEvent callback.
 * Each selected owner is explicitly closed. Default0 attempts parameter.get;
 * denied13/unsupported95 are preserved failures, not implementation PASS.
 * See ../c/ADVANCED.md for the full CLI policy and native prerequisites.
 */
#include "example_support.hpp"
#ifndef ROSWASM_EXAMPLE_CASE
#define ROSWASM_EXAMPLE_CASE 0
#endif
#if ROSWASM_EXAMPLE_CASE < 0 || ROSWASM_EXAMPLE_CASE > 25
#error "ROSWASM_EXAMPLE_CASE must be 0..25"
#endif
int main() {
    auto error=example_abi();if(error) return static_cast<int>(error);
    roswasmcpp::Context context{*ROSWASM_BACKEND()};roswasmcpp::Node node{context,"cpp-queries"};
#if ROSWASM_EXAMPLE_CASE <= 8
    constexpr roswasmc_operation operations[]={ROSWASMC_PARAMETER_GET,ROSWASMC_PARAMETER_SET,ROSWASMC_PARAMETER_DELETE,ROSWASMC_PARAMETER_EXISTS,
        ROSWASMC_PARAMETER_LIST,ROSWASMC_PARAMETER_SEARCH,ROSWASMC_PARAMETER_DESCRIBE,ROSWASMC_PARAMETER_DECLARE,ROSWASMC_PARAMETER_UNDECLARE};
    constexpr char const* aliases[]={"parameter-get","parameter-set","parameter-delete","parameter-exists","parameter-list","parameter-search","parameter-describe","parameter-declare","parameter-undeclare"};
    auto owner=node.parameters(aliases[ROSWASM_EXAMPLE_CASE],operations[ROSWASM_EXAMPLE_CASE],1024);error=owner.error();std::string text;bool exists{};
    if(!error) {
        if constexpr(ROSWASM_EXAMPLE_CASE==0) error=owner.get(text);
        else if constexpr(ROSWASM_EXAMPLE_CASE==1||ROSWASM_EXAMPLE_CASE==7) error=owner.set(std::string{"guest value"});
        else if constexpr(ROSWASM_EXAMPLE_CASE==2||ROSWASM_EXAMPLE_CASE==8) error=owner.erase();
        else if constexpr(ROSWASM_EXAMPLE_CASE==3) error=owner.exists(exists);
        else error=owner.metadata(text);
    }
    return static_cast<int>(example_finish(owner,error));
#elif ROSWASM_EXAMPLE_CASE == 9
    auto owner=node.create_parameter_events<roswasmcpp::msg::TypedParameterEvent>("parameter-events",1024);error=owner.error();size_t callbacks{};
    owner.on_message([&](auto const& event){++callbacks;(void)event;});
    roswasmcpp::Executor executor{context};size_t delivered{};auto registration=error?0:executor.add(owner);
    if(!error&&!registration) error=UWVM_ROS_INVALID;if(!error) error=executor.spin_some(1,delivered);executor.remove(registration);
    if(!error&&callbacks!=1) error=UWVM_ROS_AGAIN;return static_cast<int>(example_finish(owner,error));
#elif ROSWASM_EXAMPLE_CASE <= 16
    constexpr roswasmc_operation operations[]={ROSWASMC_GRAPH_INSPECT_TOPIC,ROSWASMC_GRAPH_INSPECT_NODE,ROSWASMC_GRAPH_INSPECT_SERVICE,ROSWASMC_GRAPH_INSPECT_ACTION,
        ROSWASMC_GRAPH_LIST_TOPICS,ROSWASMC_GRAPH_LIST_NODES,ROSWASMC_GRAPH_LIST_SERVICES};
    constexpr char const* aliases[]={"graph-topic","graph-node","graph-service","graph-action","graph-topics","graph-nodes","graph-services"};
    auto owner=node.graph(aliases[ROSWASM_EXAMPLE_CASE-10],operations[ROSWASM_EXAMPLE_CASE-10],1024);error=owner.error();std::string text;
    if(!error) error=owner.inspect(text);return static_cast<int>(example_finish(owner,error));
#elif ROSWASM_EXAMPLE_CASE <= 20
    constexpr roswasmc_operation operations[]={ROSWASMC_TIME_READ_ROS,ROSWASMC_TIME_READ_WALL,ROSWASMC_TIME_READ_SYSTEM,ROSWASMC_TIME_READ_STEADY};
    constexpr char const* aliases[]={"ros-time","wall-time","system-time","steady-time"};
    auto owner=node.clock(aliases[ROSWASM_EXAMPLE_CASE-17],operations[ROSWASM_EXAMPLE_CASE-17]);error=owner.error();uint64_t now{};
    if(!error) error=owner.now(now);return static_cast<int>(example_finish(owner,error));
#elif ROSWASM_EXAMPLE_CASE == 21
    auto owner=node.logger("log-info");error=owner.error();if(!error) error=owner.write("guest application");return static_cast<int>(example_finish(owner,error));
#elif ROSWASM_EXAMPLE_CASE == 22 || ROSWASM_EXAMPLE_CASE == 23
    auto owner=node.tf(ROSWASM_EXAMPLE_CASE==22?"tf-lookup":"tf-broadcast",ROSWASM_EXAMPLE_CASE==22?ROSWASMC_TF_LOOKUP:ROSWASMC_TF_BROADCAST,1024);
    error=owner.error();roswasmcpp::msg::Transform transform{};
    if(!error) {
        if constexpr(ROSWASM_EXAMPLE_CASE==22) error=owner.lookup(0,transform);
        else {transform.parent_frame="map";transform.child_frame="base_link";transform.quaternion={0,0,0,1};error=owner.broadcast(transform);}
    }
    return static_cast<int>(example_finish(owner,error));
#else
    auto owner=node.lifecycle(ROSWASM_EXAMPLE_CASE==24?"lifecycle-get":"lifecycle-change",ROSWASM_EXAMPLE_CASE==24?ROSWASMC_LIFECYCLE_GET_STATE:ROSWASMC_LIFECYCLE_CHANGE_STATE,1024);
    error=owner.error();roswasmcpp::LifecycleState state{};bool accepted{};
    if(!error) {if constexpr(ROSWASM_EXAMPLE_CASE==24) error=owner.get_state(state);else error=owner.change_state(accepted);}
    return static_cast<int>(example_finish(owner,error));
#endif
}
