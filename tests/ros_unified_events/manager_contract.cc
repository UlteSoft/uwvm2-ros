/** @file
 * @brief Exercise actual capability-manager atomic event authority/quota logic.
 * C++26/fast_io; no ROS, Guest or network runs. Request/goal ownership and native
 * session pins are separate Host/Gateway contracts, not simulated by this test.
 */
#include <uwvm2/imported/ros/capability/manager.h>
#include <array>
#include <cassert>
#include <fast_io.h>
namespace ros=uwvm2::imported::ros;
static ros::target_policy target(){
    ros::target_policy p{ros::version::ros1,ros::scope::global,"",true,{{"gateway-socket","/tmp/uwvm-event-test.sock"},{"gateway-uid","1000"},{"node-name","test_node"},{"namespace","/"},{"connect-timeout-ms","1000"},{"audit-deny-max-rate-hz","10"},{"audit-deny-burst","10"},{"audit-deny-max-records","128"},{"audit-deny-max-bytes","65536"},{"master-uri","http://127.0.0.1:11311"},{"advertise-host","127.0.0.1"},{"transport","tcpros"}}, {}};
    ros::capability_spec source{"client","service.call","/test/service","std_srvs/SetBool",{},{{"timeout-ms","1000"},{"max-inflight","4"},{"max-request-bytes","1"},{"max-response-bytes","5"},{"max-rate-hz","0.001"},{"burst","4"},{"max-bytes-per-second","5"},{"validator","none"},{"audit","deny-only"},{"audit-max-rate-hz","10"},{"audit-burst","10"},{"audit-max-records","128"},{"audit-max-bytes","65536"}}};
    source.type_identity=ros::builtin_type_identity(ros::version::ros1,source.type);
    ros::capability_spec observer{"events","event.observe","/test/service",std::string(ros::ipc::event_type),std::string(ros::ipc::event_type_identity),{{"event-source-alias","client"},{"event-kinds","service-response,resource-error,resource-closed"},{"max-event-bytes","69"},{"max-event-queue-depth","3"},{"max-event-queue-bytes","197"},{"max-inflight","4"},{"max-rate-hz","0.001"},{"burst","3"},{"max-bytes-per-second","69"},{"overflow-policy","reject"},{"audit","deny-only"},{"audit-max-rate-hz","10"},{"audit-burst","10"},{"audit-max-records","128"},{"audit-max-bytes","65536"}}};
    p.capabilities.emplace(source.alias,source);p.capabilities.emplace(observer.alias,observer);return p;
}
int main(){
    std::array<std::byte,5> body{std::byte{1},std::byte{0},std::byte{0},std::byte{0},std::byte{0}};
    for(bool source_exhausted:{false,true}){
        auto policy=target();assert(ros::validate_target(policy));int object{};ros::capability_manager manager;
        assert(manager.register_instance(&object,7,"guest",&policy,nullptr));std::uint64_t source{},observer{};
        assert(manager.open(&object,7,ros::version::ros1,"client",source));assert(manager.open(&object,7,ros::version::ros1,"events",observer));
        ros::authorization source_lease,observer_lease;assert(manager.describe(&object,7,ros::version::ros1,source,source_lease));
        ros::ipc::event_header header{};header.kind=ros::ipc::event_kind::service_response;header.backend=1;header.source_handle=source;header.instance=source_lease.instance_number();header.session=18;header.correlation=17;header.sequence=1;header.payload_bytes=5;source_lease.release();
        if(source_exhausted){assert(manager.authorize_response(&object,7,ros::version::ros1,source,body,source_lease));source_lease.release();}
        else {assert(manager.charge_observation(&object,7,ros::version::ros1,observer,69,observer_lease));observer_lease.release();}
        auto denied=manager.authorize_event_delivery(&object,7,ros::version::ros1,observer,source,header,body,observer_lease,source_lease);
        assert(denied.code==ros::errc::rate_limited&&!observer_lease&&!source_lease);
        // Refusal debits neither side: the unexhausted full budget still admits
        // its exact maximum. A split Source-first debit would fail this check.
        if(source_exhausted){assert(manager.charge_observation(&object,7,ros::version::ros1,observer,69,observer_lease));}
        else {assert(manager.authorize_response(&object,7,ros::version::ros1,source,body,source_lease));}
    }
    auto policy=target();policy.capabilities.at("events").limits["max-bytes-per-second"]="138";int object{},foreign{};ros::capability_manager manager;assert(manager.register_instance(&object,9,"guest",&policy,nullptr));assert(manager.register_instance(&foreign,9,"guest",&policy,nullptr));
    std::uint64_t source{},observer{};assert(manager.open(&object,9,ros::version::ros1,"client",source));assert(manager.open(&object,9,ros::version::ros1,"events",observer));
    ros::authorization s,o;assert(manager.describe(&object,9,ros::version::ros1,source,s));ros::ipc::event_header h{};h.kind=ros::ipc::event_kind::service_response;h.backend=1;h.source_handle=source;h.instance=s.instance_number();h.session=19;h.correlation=21;h.sequence=1;h.payload_bytes=5;s.release();
    auto wrong=h;wrong.instance++;assert(manager.authorize_event_delivery(&object,9,ros::version::ros1,observer,source,wrong,body,o,s).code==ros::errc::permission_denied);
    assert(manager.authorize_event_delivery(&foreign,9,ros::version::ros1,observer,source,h,body,o,s).code!=ros::errc::success);
    assert(manager.authorize_event_delivery(&object,10,ros::version::ros1,observer,source,h,body,o,s).code==ros::errc::permission_denied);
    assert(manager.authorize_event_delivery(&object,9,ros::version::ros1,observer,source,h,body,o,s));o.release();s.release();
    assert(manager.close(&object,9,ros::version::ros1,source));h.kind=ros::ipc::event_kind::resource_closed;h.flags=1;h.correlation=0;h.payload_bytes=0;
    // Metadata after Source close has no Source payload/read authority. The
    // independent observer still needs its own current Instance/version lease.
    h.sequence=2;assert(manager.authorize_event_delivery(&object,9,ros::version::ros1,observer,source,h,{},o,s)&&o&&!s);o.release();
    assert(manager.authorize_event_delivery(&object,9,ros::version::ros2,observer,source,h,{},o,s).code!=ros::errc::success);
    assert(manager.revoke(&object,9,ros::version::ros1,"events"));assert(manager.authorize_event_delivery(&object,9,ros::version::ros1,observer,source,h,{},o,s).code!=ros::errc::success);
    fast_io::println("PASS_UNIFIED_MANAGER");
}
