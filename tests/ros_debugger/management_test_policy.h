/** Explicit native component policies, never Runtime defaults or Guest grants.
 * Both backends are bounded String subscriptions. No connection is started by
 * constructing or registering these values. Keep this fixture independent of
 * installed ROS SDK headers so the real capability manager remains testable.
 */
#pragma once
#include <uwvm2/imported/ros/capability/manager.h>
namespace ros_management_test
{
    inline uwvm2::imported::ros::target_policy policy(uwvm2::imported::ros::version version)
    {
        namespace ros=uwvm2::imported::ros;
        ros::target_policy value{version,ros::scope::global,"",true,
            {{"gateway-socket","/tmp/unused-management-component.sock"},{"gateway-uid","0"},
             {"node-name","management_component"},{"namespace","/"},{"connect-timeout-ms","1000"},
             {"audit-deny-max-rate-hz","10"},{"audit-deny-burst","10"},
             {"audit-deny-max-records","128"},{"audit-deny-max-bytes","65536"}}, {}};
        ros::capability_spec cap{"sub","topic.subscribe","/management_component",
            version==ros::version::ros1?"std_msgs/String":"std_msgs/msg/String",{},
            {{"max-message-bytes","64"},{"max-rate-hz","0.001"},{"burst","4"},
             {"max-bytes-per-second","1024"},{"max-queue-bytes","512"},{"overflow-policy","reject"},
             {"validator","none"},{"max-subscribers","4"},{"audit","allow-and-deny"},
             {"audit-max-rate-hz","10"},{"audit-burst","10"},{"audit-max-records","128"},
             {"audit-max-bytes","65536"}}};
        if(version==ros::version::ros1)
        {
            value.connection.insert({{"master-uri","http://127.0.0.1:11311"},
                {"advertise-host","127.0.0.1"},{"transport","tcpros"}});
            cap.limits.insert({{"queue-depth","4"},{"transport","tcpros"}});
        }
        else
        {
            value.connection.insert({{"domain-id","1"},{"rmw-implementation","rmw_fastrtps_cpp"},
                {"security-mode","disabled-test"},{"keystore","not-applicable"},{"enclave","not-applicable"}});
            cap.limits.insert({{"max-inflight","4"},{"qos.reliability","reliable"},
                {"qos.durability","volatile"},{"qos.history","keep-last"},{"qos.depth","4"},
                {"qos.deadline","infinite"},{"qos.lifespan","infinite"},
                {"qos.liveliness","automatic"},{"qos.liveliness-lease-duration","infinite"}});
        }
        cap.type_identity=ros::builtin_type_identity(version,cap.type);
        value.capabilities.emplace(cap.alias,cap);
        // Revocation is an independent fixture resource. The real launch
        // validator correctly rejects duplicate operation/resource/type grants
        // hidden behind different aliases; this test must obey that boundary.
        cap.alias="revoked";cap.resource="/management_component_revoked";
        value.capabilities.emplace(cap.alias,cap);
        return value;
    }
}
