/** Source-selected policy contracts, not native ROS evidence.
 * These tests call the production validators and capability manager directly.
 * No fabricated RCL handle, RMW metadata, network peer or native type support
 * is installed. The future bounded qualification owner compiles this complete
 * source tree before running this executable. Assertions stay active in every
 * build so a release configuration cannot turn checks into unconditional PASS.
 */
#include <uwvm2/imported/ros/capability/manager.h>
#include "../../gateways/common/config.h"
#include <array>
#include <cstdlib>
#include <span>
#include <string>
#include <vector>
#include <fast_io.h>

namespace ros = uwvm2::imported::ros;
static void need(bool ok) { if(!ok) { ::std::abort(); } }
static void expect(ros::result const& result, ros::errc code = ros::errc::success)
{
    if(result.code != code)
    {
        ::fast_io::io::perrln("service policy expected ",static_cast<unsigned>(code),
            " got ",static_cast<unsigned>(result.code),": ",result.detail);
        ::std::abort();
    }
}
/** Build every explicit field required by the actual selected operation schema.
 * Test values are local grants only. Production SDKs do not acquire defaults
 * from this fixture and no native Registry identities are synthesized here.
 */
static ros::capability_spec grant(std::string operation, std::string type)
{
    ros::capability_spec cap{"service",std::move(operation),"/contracts/service",std::move(type),{}, {}};
    for(auto key : ros::operation_schema(ros::version::ros2,cap.operation,cap.type))
    { cap.limits.emplace(key,"1"); }
    cap.type_identity=ros::builtin_type_identity(ros::version::ros2,cap.type);
    for(auto key : {"max-request-bytes","max-response-bytes"}) { cap.limits[key]="64"; }
    cap.limits["timeout-ms"]="100"; cap.limits["max-rate-hz"]="0.001";
    cap.limits["burst"]="4";cap.limits["max-bytes-per-second"]="65536";
    cap.limits["max-inflight"]="4";
    cap.limits["audit"]="allow-and-deny"; cap.limits["audit-max-rate-hz"]="10";
    cap.limits["audit-burst"]="10";cap.limits["audit-max-records"]="128";
    cap.limits["audit-max-bytes"]="65536";
    if(cap.limits.contains("validator")) { cap.limits["validator"]="none"; }
    if(cap.limits.contains("overflow-policy")) { cap.limits["overflow-policy"]="reject"; }
    if(cap.limits.contains("max-event-queue-bytes")) { cap.limits["max-event-queue-bytes"]="65536"; }
    if(cap.limits.contains("max-event-queue-depth")) { cap.limits["max-event-queue-depth"]="4"; }
    cap.limits["qos.reliability"]="reliable"; cap.limits["qos.durability"]="volatile";
    cap.limits["qos.history"]="keep-last";cap.limits["qos.depth"]="4";
    cap.limits["qos.liveliness"]="automatic";
    for(auto key : {"qos.deadline","qos.lifespan","qos.liveliness-lease-duration"}) { cap.limits[key]="infinite"; }
    if(cap.operation=="service.available")
    {
        cap.limits["service-source-alias"]="source";
        cap.limits["max-wait-ns"]="1";cap.limits["max-binding-history"]="2";
        cap.limits["max-native-clients"]="1";cap.limits["max-inflight"]="1";
    }
    return cap;
}
static ros::target_policy target(ros::capability_spec const& cap)
{
    ros::target_policy policy{ros::version::ros2,ros::scope::global,"",true,
        {{"gateway-socket","/tmp/uwvm-service-policy-contract.sock"},{"gateway-uid","1000"},
         {"node-name","service_contract"},{"namespace","/"},{"connect-timeout-ms","1000"},
         {"audit-deny-max-rate-hz","10"},{"audit-deny-burst","10"},
         {"audit-deny-max-records","128"},{"audit-deny-max-bytes","65536"},
         {"domain-id","1"},{"rmw-implementation","rmw_fastrtps_cpp"},
         {"security-mode","disabled-test"},{"keystore","not-applicable"},{"enclave","not-applicable"}}, {}};
    policy.capabilities.emplace(cap.alias,cap);return policy;
}
static std::vector<std::byte> ticketed(std::size_t body)
{
    // A positive host-owned ticket followed by canonical bytes, never a CDR
    // packet. Fill includes negative-i64 and high-bit patterns without casting.
    std::vector<std::byte> bytes(8+body,std::byte{0xff});bytes[0]=std::byte{1};
    return bytes;
}
int main()
{
    unsigned groups{};
    auto run=[&](auto body){body();++groups;};
    run([]{ // Exact service type names and published identities, ROS2 only.
        need(ros::builtin_type_identity(ros::version::ros2,"std_srvs/srv/Empty")==
            "RIHS01_5888399dedec5ccc85ea6451949fd2c9f97bfdf963f9a588821639fcd31b5d19");
        need(ros::builtin_type_identity(ros::version::ros2,"example_interfaces/srv/AddTwoInts")==
            "RIHS01_e118de6bf5eeb66a2491b5bda11202e7b68f198d6f67922cf30364858239c81a");
        for(auto name:{"std_srvs/Empty","example_interfaces/AddTwoInts","std_srvs/srv/Empty","example_interfaces/srv/AddTwoInts"})
        { need(!ros::backend_supported(ros::version::ros1,"service.call",name)); }
        for(auto name:{"std_srvs/Empty","example_interfaces/AddTwoInts"})
        { need(!ros::backend_supported(ros::version::ros2,"service.call",name)); }
    });
    run([]{ // All four genuine service operations reach actual Host/Gateway policy.
        for(auto type:{"std_srvs/srv/Empty","example_interfaces/srv/AddTwoInts"})
        for(auto op:{"service.call","service.advertise","service.respond","service.available"})
        {
            auto cap=grant(op,type);expect(ros::validate_capability(ros::version::ros2,cap));
            uwvm2::ros_gateway::validate(ros::version::ros2,cap);
            need(!ros::backend_supported(ros::version::ros2,"topic.publish",type));
            need(!ros::backend_supported(ros::version::ros2,"topic.subscribe",type));
            cap.type_identity="RIHS01_wrong";
            expect(ros::validate_capability(ros::version::ros2,cap),ros::errc::permission_denied);
        }
    });
    run([]{ // Native Empty's placeholder never becomes a wire payload.
        auto cap=grant("service.call","std_srvs/srv/Empty");
        expect(ros::validate_message(ros::version::ros2,cap,{}));
        expect(ros::validate_service_response(ros::version::ros2,cap,{}));
        std::array<std::byte,1> byte{};
        expect(ros::validate_message(ros::version::ros2,cap,byte),ros::errc::invalid_argument);
        expect(ros::validate_service_response(ros::version::ros2,cap,byte),ros::errc::invalid_argument);
    });
    run([]{ // Full fixed signed-i64 representation, including all high bits.
        auto cap=grant("service.call","example_interfaces/srv/AddTwoInts");
        std::array<std::byte,17> bytes{};bytes.fill(std::byte{0xff});
        expect(ros::validate_message(ros::version::ros2,cap,{bytes.data(),16}));
        expect(ros::validate_service_response(ros::version::ros2,cap,{bytes.data(),8}));
        for(std::size_t size:{0u,7u,8u,15u,17u})
        { expect(ros::validate_message(ros::version::ros2,cap,{bytes.data(),size}),ros::errc::invalid_argument); }
        for(std::size_t size:{0u,7u,9u,16u})
        { expect(ros::validate_service_response(ros::version::ros2,cap,{bytes.data(),size}),ros::errc::invalid_argument); }
        cap.limits["max-request-bytes"]="15";cap.limits["max-response-bytes"]="7";
        expect(ros::validate_message(ros::version::ros2,cap,{bytes.data(),16}),ros::errc::too_large);
        expect(ros::validate_service_response(ros::version::ros2,cap,{bytes.data(),8}),ros::errc::too_large);
    });
    run([]{ // Existing Trigger/SetBool canonical contracts remain genuine paths.
        for(auto type:{"std_srvs/srv/Trigger","std_srvs/srv/SetBool"})
        {
            auto cap=grant("service.call",type);std::array<std::byte,5> response{};
            expect(ros::validate_service_response(ros::version::ros2,cap,response));
            if(cap.type=="std_srvs/srv/Trigger") { expect(ros::validate_message(ros::version::ros2,cap,{})); }
            else { std::array<std::byte,1> boolean{std::byte{1}};expect(ros::validate_message(ros::version::ros2,cap,boolean));boolean[0]=std::byte{2};expect(ros::validate_message(ros::version::ros2,cap,boolean),ros::errc::invalid_argument); }
        }
    });
    run([]{ // New admission spends body bytes; original admission still spends envelope.
        for(auto op:{"service.advertise","service.respond"})
        {
            auto cap=grant(op,"example_interfaces/srv/AddTwoInts");
            auto body=cap.operation=="service.advertise"?16u:8u;
            cap.limits["max-bytes-per-second"]=body==16?"16":"8";
            auto policy=target(cap);int object{};ros::capability_manager manager;
            expect(manager.register_instance(&object,1,"body_accounting",nullptr,&policy));
            std::uint64_t handle{};expect(manager.open(&object,1,ros::version::ros2,cap.alias,handle));
            ros::authorization lease;auto envelope=ticketed(body);
            expect(manager.authorize_rmw_service_envelope(&object,1,ros::version::ros2,handle,op,envelope,lease));
            need(lease.capability()&&lease.capability()->alias==cap.alias);lease.release();
            ros::capability_manager legacy;int legacy_object{};
            expect(legacy.register_instance(&legacy_object,1,"legacy_accounting",nullptr,&policy));
            expect(legacy.open(&legacy_object,1,ros::version::ros2,cap.alias,handle));
            expect(legacy.authorize(&legacy_object,1,ros::version::ros2,handle,op,envelope,lease),ros::errc::rate_limited);
            need(!lease);
        }
    });
    run([]{ // A zero-byte Empty body still needs a real positive transport ticket.
        auto cap=grant("service.advertise","std_srvs/srv/Empty");cap.limits["max-bytes-per-second"]="1";
        auto policy=target(cap);int object{};ros::capability_manager manager;
        expect(manager.register_instance(&object,1,"empty_ticket",nullptr,&policy));
        std::uint64_t handle{};expect(manager.open(&object,1,ros::version::ros2,cap.alias,handle));ros::authorization lease;
        std::array<std::byte,8> zero{};
        expect(manager.authorize_rmw_service_envelope(&object,1,ros::version::ros2,handle,cap.operation,{},lease),ros::errc::invalid_argument);
        expect(manager.authorize_rmw_service_envelope(&object,1,ros::version::ros2,handle,cap.operation,zero,lease),ros::errc::invalid_argument);
        auto actual=ticketed(0);
        expect(manager.authorize_rmw_service_envelope(&object,1,ros::version::ros2,handle,cap.operation,actual,lease));
    });
    run([]{ // Wrong operation, version, epoch, handle and revoked owners fail closed.
        auto cap=grant("service.respond","example_interfaces/srv/AddTwoInts");auto policy=target(cap);
        int object{},foreign{};ros::capability_manager manager;
        expect(manager.register_instance(&object,1,"owner",nullptr,&policy));
        expect(manager.register_instance(&foreign,1,"foreign",nullptr,&policy));
        std::uint64_t handle{};expect(manager.open(&object,1,ros::version::ros2,cap.alias,handle));
        auto bytes=ticketed(8);ros::authorization lease;
        expect(manager.authorize_rmw_service_envelope(&object,1,ros::version::ros2,handle,"service.call",bytes,lease),ros::errc::permission_denied);
        expect(manager.authorize_rmw_service_envelope(&object,1,ros::version::ros1,handle,cap.operation,bytes,lease),ros::errc::permission_denied);
        expect(manager.authorize_rmw_service_envelope(&object,2,ros::version::ros2,handle,cap.operation,bytes,lease),ros::errc::permission_denied);
        expect(manager.authorize_rmw_service_envelope(&foreign,1,ros::version::ros2,handle,cap.operation,bytes,lease),ros::errc::bad_handle);
        expect(manager.revoke(&object,1,ros::version::ros2,cap.alias));
        expect(manager.authorize_rmw_service_envelope(&object,1,ros::version::ros2,handle,cap.operation,bytes,lease),ros::errc::bad_handle);need(!lease);
    });
    run([]{ // Registering an ordinary instance never grants a ROS capability.
        ros::capability_manager manager;int object{};std::uint64_t handle{99};
        expect(manager.register_instance(&object,1,"unprivileged",nullptr,nullptr));
        expect(manager.open(&object,1,ros::version::ros2,"service",handle),ros::errc::permission_denied);need(!handle);
        ros::authorization lease;auto bytes=ticketed(8);
        expect(manager.authorize_rmw_service_envelope(&object,1,ros::version::ros2,99,"service.respond",bytes,lease),ros::errc::bad_handle);need(!lease);
    });
    ::fast_io::io::println("service policy contract groups=",groups);
}
