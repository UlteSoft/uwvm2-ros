/** Real capability-manager/model contract; no Runtime, native SDK or ROS.
 * The fixture int addresses are MODEL identities registered through the actual
 * production manager; no actual future Runtime generation/handle is forecast. */
#include <uwvm2/imported/ros/capability/manager.h>
#include <uwvm2/imported/ros/common/service_availability.h>
#include <future>
#include <cstdlib>
namespace core=uwvm2::imported::ros;namespace wire=core::ipc::client_availability;
static void require(bool c){if(!c){std::abort();}}
static void require(core::result const& r,core::errc expected=core::errc::success){require(r.code==expected);}
static core::target_policy policy(core::version version)
{
    core::target_policy p{version,core::scope::global,"",true,{{"gateway-socket","/tmp/availability-model.sock"},
      {"gateway-uid","0"},{"node-name","availability_model"},{"namespace","/"},{"connect-timeout-ms","1000"},
      {"audit-deny-max-rate-hz","100"},{"audit-deny-burst","100"},{"audit-deny-max-records","100"},{"audit-deny-max-bytes","65536"}}, {}};
    if(version==core::version::ros1)p.connection.insert({{"master-uri","http://127.0.0.1:11311"},{"advertise-host","127.0.0.1"},{"transport","tcpros"}});
    else p.connection.insert({{"domain-id","187"},{"rmw-implementation","rmw_fastrtps_cpp"},{"security-mode","disabled-test"},{"keystore","not-applicable"},{"enclave","not-applicable"}});
    core::capability_spec c{"call","service.call","/availability_model",version==core::version::ros1?"std_srvs/Trigger":"std_srvs/srv/Trigger",{},
      {{"timeout-ms","1000"},{"max-inflight","1"},{"max-request-bytes","1"},{"max-response-bytes","128"},{"max-rate-hz","0.000001"},{"burst","1"},
       {"max-bytes-per-second","4096"},{"validator","none"},{"audit","deny-only"},{"audit-max-rate-hz","100"},{"audit-burst","100"},{"audit-max-records","100"},{"audit-max-bytes","65536"}}};
    c.type_identity=core::builtin_type_identity(version,c.type);
    if(version==core::version::ros2)c.limits.insert({{"qos.reliability","reliable"},{"qos.durability","volatile"},{"qos.history","keep-last"},{"qos.depth","4"},
        {"qos.deadline","infinite"},{"qos.lifespan","infinite"},{"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}});
    p.capabilities.emplace(c.alias,c);c.alias="available";c.operation="service.available";c.limits.erase("validator");
    c.limits["service-source-alias"]="call";c.limits["max-wait-ns"]="1000000";c.limits["max-native-clients"]="1";c.limits["max-binding-history"]="4";
    c.limits["max-request-bytes"]="64";c.limits["max-response-bytes"]="64";p.capabilities.emplace(c.alias,c);
    require(core::validate_target(p));return p;
}
int main()
{
    for(auto version:{core::version::ros1,core::version::ros2})
    {
        auto selected{policy(version)};auto const& a{selected.capabilities.at("available")};auto const& s{selected.capabilities.at("call")};
        require(core::service_availability_pair(version,a,s));
        auto invalid{selected};invalid.capabilities.at("available").limits["service-source-alias"]="missing";
        require(core::validate_target(invalid),core::errc::permission_denied);
        invalid=selected;invalid.capabilities.at("available").limits["timeout-ms"]="1";require(core::validate_target(invalid),core::errc::invalid_argument);
        invalid=selected;invalid.capabilities.at("available").limits["max-request-bytes"]="63";require(core::validate_target(invalid),core::errc::invalid_argument);
        if(version==core::version::ros2){invalid=selected;invalid.capabilities.at("available").limits["qos.depth"]="5";require(core::validate_target(invalid),core::errc::permission_denied);}
        int object{},inert{},foreign{};core::capability_manager manager;
        auto reg=[&](int& identity,core::target_policy const* target){return manager.register_instance(&identity,1,"model",version==core::version::ros1?target:nullptr,version==core::version::ros2?target:nullptr);};
        require(reg(object,&selected));require(reg(inert,nullptr));require(reg(foreign,&selected));
        std::uint64_t available{},source{},unused{};require(manager.open(&inert,1,version,"available",unused),core::errc::permission_denied);require(!unused);
        require(manager.open(&object,1,version,"call",source));require(manager.open(&object,1,version,"available",available));
        require(manager.open(&object,1,version,"available",unused),core::errc::rate_limited);require(!unused);
        core::authorization observation,called;
        require(manager.authorize(&object,1,version,source,"service.available",{},observation,false,false),core::errc::permission_denied);
        require(manager.describe(&foreign,1,version,available,observation),core::errc::bad_handle);
        require(manager.authorize(&object,1,version,source,"service.call",{},called,false,false));
        require(manager.authorize(&object,1,version,available,"service.available",{},observation,false,false));
        require(observation.target()==called.target()&&observation.instance_number()==called.instance_number()&&observation.instance_number());
        wire::packet request{wire::command::query,wire::retirement::none,0,
            {static_cast<std::uint8_t>(version),available,source,observation.instance_number(),1},0};
        auto raw{wire::encode(request)};core::authorization debit;
        require(manager.authorize(&object,1,version,available,"service.available",std::as_bytes(std::span{raw}),debit,true,true));debit.release();
        require(manager.authorize(&object,1,version,available,"service.available",std::as_bytes(std::span{raw}),debit,true,true),core::errc::rate_limited);
        // Metadata observation did not consume the real call's operation token.
        require(manager.authorize(&object,1,version,source,"service.call",{},debit,true,true));debit.release();
        wire::packet response{request};response.value=1;raw=wire::encode(response);
        require(manager.authorize_response(&object,1,version,available,std::as_bytes(std::span{raw}),debit));debit.release();
        // True source revoke waits for BOTH production leases. No borrowed
        // capability pointer remains live after these explicit releases.
        std::promise<void> started;auto begin{started.get_future()};
        auto revoke{std::async(std::launch::async,[&]{started.set_value();return manager.revoke(&object,1,version,"call");})};begin.get();
        require(revoke.wait_for(std::chrono::milliseconds(10))==std::future_status::timeout);
        observation.release();require(revoke.wait_for(std::chrono::milliseconds(10))==std::future_status::timeout);
        called.release();require(revoke.wait_for(std::chrono::seconds(1))==std::future_status::ready);require(revoke.get());
        require(manager.describe(&object,1,version,source,called),core::errc::bad_handle);
        require(manager.open(&object,1,version,"call",unused),core::errc::revoked);
        require(manager.close(&object,1,version,available));
        require(manager.describe(&object,1,version,available,observation),core::errc::bad_handle);
        require(manager.unregister_instance(&object,1));
        require(manager.describe(&object,1,version,available,observation),core::errc::permission_denied);
    }
}
