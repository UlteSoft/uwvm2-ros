/** @file
 * Actual detached diagnostic primitives, capability registry and event queue.
 * This component neither starts ROS nor manufactures an authenticated Runtime.
 * All lock contention uses handshakes, not a timing-dependent sleep. The Root
 * runner must impose its ordinary external deadline and memory containment.
 */
#include <uwvm2/imported/ros/capability/manager.h>
#include <uwvm2/imported/ros/ipc/event_queue.h>
#include <future>
#include <thread>
#include <string>

namespace ros=uwvm2::imported::ros;
namespace debug=ros::debug;
namespace ipc=ros::ipc;
static unsigned checks{};
static void check(bool value,char const* message)
{
    ++checks;
    if(!value) {fast_io::io::perrln("FAIL_ROS_DEBUG_CORE ",fast_io::mnp::os_c_str(message));fast_io::fast_terminate();}
}
static ros::target_policy policy()
{
    ros::target_policy value{ros::version::ros2,ros::scope::global,"",true,
        {{"gateway-socket","/tmp/debug-component.sock"},{"gateway-uid","0"},{"node-name","debug_component"},
         {"namespace","/"},{"connect-timeout-ms","1000"},{"domain-id","205"},{"rmw-implementation","rmw_fastrtps_cpp"},
         {"security-mode","disabled-test"},{"keystore","not-applicable"},{"enclave","not-applicable"},
         {"audit-deny-max-rate-hz","100"},{"audit-deny-burst","100"},{"audit-deny-max-records","100"},{"audit-deny-max-bytes","65536"}}, {}};
    ros::capability_spec cap{"sub","topic.subscribe","/debug_component","std_msgs/msg/String",{},
        {{"max-message-bytes","64"},{"max-rate-hz","0.001"},{"burst","8"},{"max-bytes-per-second","1024"},
         {"max-queue-bytes","512"},{"overflow-policy","reject"},{"validator","none"},{"max-subscribers","2"},
         {"audit","allow-and-deny"},{"audit-max-rate-hz","100"},{"audit-burst","100"},
         {"audit-max-records","100"},{"audit-max-bytes","65536"},{"max-inflight","2"},
         {"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.history","keep-last"},{"qos.depth","8"},
         {"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},
         {"qos.liveliness-lease-duration","infinite"}}};
    cap.type_identity=ros::builtin_type_identity(ros::version::ros2,cap.type);
    value.capabilities.emplace(cap.alias,cap);return value;
}
int main()
{
    debug::request query{};debug::view first{},second{};
    debug::snapshot(query,first);
    check(first.result==debug::status::not_selected&&first.size==0,"ROS-off hook has no implicit Host");
    query.count=0;debug::snapshot(query,first);
    check(first.result==debug::status::invalid_request,"empty page is rejected");query.count=8;
    check(debug::public_resource("/safe_name/part_2"),"ROS name metadata accepted");
    check(!debug::public_resource("http://127.0.0.1:11311")&&!debug::public_resource("file:/secret")&&
          !debug::public_resource("/a?token=secret"),"connection and secret-bearing URI redacted");

    ros::capability_manager manager;int instance{},empty{};auto configured=policy();
    check(bool(ros::validate_target(configured)),"actual policy is valid");
    check(bool(manager.register_instance(&instance,7,"real_module",nullptr,&configured)),"register actual registry owner");
    check(bool(manager.register_instance(&empty,8,"empty_module",nullptr,nullptr)),"register default-deny owner");
    query.module.assign("empty_module");manager.debug_snapshot(query,first);
    check(first.result==debug::status::ok&&first.total==0,"no aliases are invented for empty grants");
    query.module.assign("real_module");query.backend=2;manager.debug_snapshot(query,first);
    check(first.result==debug::status::ok&&first.size==1&&first.rows[0].module.view()=="real_module"&&
          first.rows[0].generation==7&&first.rows[0].alias.view()=="sub","real module/generation and alias are copied");
    check(first.rows[0].active_handles==0&&first.rows[0].open_count==0,"snapshot does not open a handle");
    std::uint64_t handle{};check(bool(manager.open(&instance,7,ros::version::ros2,"sub",handle)),"explicit test open");
    manager.debug_snapshot(query,first);manager.debug_snapshot(query,second);
    check(first.rows[0].active_handles==1&&second.rows[0].active_handles==1,"repeated snapshots preserve handles");
    check(first.rows[0].operation_tokens==second.rows[0].operation_tokens&&
          first.rows[0].creation_tokens==second.rows[0].creation_tokens&&first.rows[0].byte_tokens==second.rows[0].byte_tokens,
          "stored quota values are neither refreshed nor consumed");
    check(first.rows[0].audit_records==second.rows[0].audit_records&&first.rows[0].audit_bytes==second.rows[0].audit_bytes&&
          first.rows[0].audit_suppressed==second.rows[0].audit_suppressed,"audit is not drained or appended by reads");
    auto identity=first.rows[0].instance;debug::trace_record label{};
    check(manager.debug_identity(&instance,7,ros::version::ros2,"sub",label)&&label.instance==identity&&
          label.generation==7&&label.operation.view()=="topic.subscribe","internal label lookup preserves public identity");
    query.selected=debug::selection::services;manager.debug_snapshot(query,first);
    check(first.result==debug::status::ok&&first.total==0,"family selection filters existing capabilities");
    query.selected=debug::selection::capabilities;query.instance=identity+100;manager.debug_snapshot(query,first);
    check(first.result==debug::status::ok&&first.total==0,"foreign public instance filter is empty");query.instance=identity;
    std::promise<void> locked,release;auto released=release.get_future();std::atomic<bool> leased{};
    std::thread owner([&]
    {
        ros::authorization authorization;leased=bool(manager.describe(&instance,7,ros::version::ros2,handle,authorization));
        locked.set_value();released.wait();
    });
    locked.get_future().wait();manager.debug_snapshot(query,first);release.set_value();owner.join();
    check(leased&&first.result==debug::status::busy&&first.size==0,"held real authorization lease yields explicit BUSY");
    manager.debug_snapshot(query,first);check(first.result==debug::status::ok&&first.size==1,"read succeeds after actual lease release");
    check(bool(manager.revoke(&instance,7,ros::version::ros2,"sub")),"explicit test revoke");manager.debug_snapshot(query,first);
    check(first.rows[0].revoked&&first.rows[0].active_handles==0,"revocation is observed without reopening");

    ipc::event_header binding{};binding.backend=2;binding.instance=41;binding.session=93;binding.source_handle=17;
    ipc::event_stream queue{{3,196,68,1,68,ipc::event_overflow::backpressure},binding,ipc::event_bit(ipc::event_kind::topic_message)};
    check(queue.enqueue({ipc::event_kind::topic_message,0,0,0,{1,0,0,0}})==0&&
          queue.enqueue({ipc::event_kind::topic_message,0,0,0,{2,0,0,0}})==0,"actual queued and pending records admitted");
    ipc::event_queue_statistics stats{};check(queue.try_statistics(stats)&&stats.depth==1&&stats.pending_depth==1&&stats.dequeued==0,
          "read-only statistics do not promote pending or dequeue");
    std::promise<void> queue_locked,queue_release;auto queue_released=queue_release.get_future();ipc::call_result retained{};
    std::thread consumer([&]{retained=queue.peek_if(68,[&](ipc::event_header const&){queue_locked.set_value();queue_released.wait();return std::uint32_t{};});});
    queue_locked.get_future().wait();stats.depth=987;stats.bytes=321;auto available=queue.try_statistics(stats);queue_release.set_value();consumer.join();
    check(!available&&stats.depth==987&&stats.bytes==321,"try-statistics busy leaves caller output unchanged");
    check(retained.error==0&&queue.peek(68).payload==retained.payload,"diagnostic read never consumes pinned record");
    check(queue.try_statistics(stats)&&stats.dequeued==0&&stats.pending_depth==1,"queue accounting remains unchanged after reads");

    debug::trace_store trace;debug::trace_record call{};call.backend=2;call.instance=identity;call.generation=7;
    call.module.assign("real_module");call.alias.assign("sub");call.operation.assign("event.poll");call.function.assign("event_poll_v1");call.error=11;
    for(unsigned i=0;i!=258;++i) {trace.append(call);}
    query={};query.selected=debug::selection::trace;query.count=16;query.after=1;
    trace.read(query,first);trace.read(query,second);
    check(first.result==debug::status::ok&&first.oldest==3&&first.newest==258&&first.overwritten==2&&first.gap,
          "bounded ring reports real overwrite and cursor gap");
    check(first.trace_size==16&&first.trace[0].sequence==3&&first.next==18&&first.more,
          "trace page is bounded and has a continuation cursor");
    check(second.trace_size==first.trace_size&&second.trace[0].sequence==first.trace[0].sequence&&second.total==first.total,
          "trace queries are nonconsuming and reset reused output");
    query.after=first.next;trace.read(query,second);check(second.trace[0].sequence==19,"cursor continuation does not replay previous page");
    query.function.assign("service_submit");trace.read(query,second);check(second.total==0,"exact import filter does not match another function");
    query.function.assign("event_poll_v1");query.operation.assign("event.poll");trace.read(query,second);
    check(second.total>0&&second.trace[0].function.view()=="event_poll_v1","matching import/operation filters retain real trace entries");
    query.error_filter=true;query.error=13;trace.read(query,second);check(second.total==0,"errno filter never invents matching calls");
    query.first=1;trace.read(query,second);check(second.result==debug::status::invalid_request&&second.trace_size==0,
          "trace refuses ignored positional paging");
    debug::trace_store long_names;call.module.assign(std::string(300,'x'));long_names.append(call);
    query={};query.selected=debug::selection::trace;query.module.assign(std::string(255,'x'));long_names.read(query,second);
    check(second.result==debug::status::ok&&second.total==0,"truncated module prefix is not an exact identity match");
    fast_io::io::println("PASS_ROS_DEBUG_CORE checks=",checks);
}
