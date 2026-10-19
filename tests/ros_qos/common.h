#pragma once
#include "../../gateways/common/config.h"
#include <array>
#include <stdexcept>
#include <string>

namespace action_qos_test
{
    namespace core=uwvm2::imported::ros;
    namespace gateway=uwvm2::ros_gateway;
    #include "action_matrix.inc"
    inline constexpr char diagnostic[]{"action response QoS differs from its explicit server contract"};
    inline void check(bool value,std::string const& message)
    { if(!value) { throw std::runtime_error(message); } }
    inline core::capability_spec capability(std::string alias,std::string operation,
        std::string resource="/uwvm_action_qos/action")
    {
        core::capability_spec cap{std::move(alias),std::move(operation),std::move(resource),
            "action_tutorials_interfaces/action/Fibonacci",
            "RIHS01_525d2c3c8a3979003645ffa01186e360347194550426993cad2fb950058ab5a0",{}};
        cap.limits={{"max-active-goals","1"},{"max-actions","1"},{"timeout-ms","15000"},
            {"max-goal-bytes","256"},{"max-feedback-bytes","256"},{"max-result-bytes","256"},
            {"fibonacci.max-order","46"},{"cancel-scope","not-applicable"},
            {"max-rate-hz","100"},{"burst","8"},{"max-bytes-per-second","1048576"},
            {"max-event-queue-depth","8"},{"max-event-queue-bytes","2048"},
            {"overflow-policy","drop-oldest"},{"validator","none"},{"audit","allow-and-deny"},
            {"audit-max-rate-hz","100"},{"audit-burst","8"},{"audit-max-records","10000"},
            {"audit-max-bytes","1048576"}};
        for(auto const& row:mutations) { cap.limits.emplace(row.key,row.baseline); }
        return cap;
    }
    inline core::target_policy pair(bool enabled=true)
    {
        core::target_policy policy{core::version::ros2,core::scope::global,"",enabled,
            {{"gateway-socket","/tmp/uwvm-action-qos-unused.sock"},{"gateway-uid","0"},
             {"node-name","uwvm_action_qos"},{"namespace","/uwvm_action_qos"},
             {"connect-timeout-ms","1000"},{"audit-deny-max-rate-hz","100"},
             {"audit-deny-burst","8"},{"audit-deny-max-records","10000"},
             {"audit-deny-max-bytes","1048576"},{"domain-id","74"},
             {"rmw-implementation","rmw_fastrtps_cpp"},{"security-mode","disabled-test"},
             {"keystore","not-applicable"},{"enclave","not-applicable"}}, {}};
        policy.capabilities.emplace("advertise",capability("advertise","action.advertise"));
        policy.capabilities.emplace("respond",capability("respond","action.respond"));
        return policy;
    }
    inline std::string config_text(core::target_policy const& policy,std::string const& socket)
    {
        std::string text{"backend ros2\nworker-uid "};text+=std::to_string(::getuid());
        text+="\nworker-pid ";text+=std::to_string(::getpid());
        text+="\nsocket ";text+=socket;
        text+="\ntimeout-ms 15000\nmax-sessions 1\naudit-max-records-per-second 100\naudit-max-records 10000\naudit-max-bytes 1048576\n";
        for(auto key:{"node-name","namespace","domain-id","rmw-implementation","security-mode","keystore","enclave"})
        { text+="connection "+std::string{key}+" "+policy.connection.at(key)+"\n"; }
        for(auto const& value:policy.capabilities)
        {
            auto const& cap{value.second};text+="grant "+cap.alias+" "+cap.operation+" "+cap.resource+" "+cap.type+" "+cap.type_identity+"\n";
            for(auto const& limit:cap.limits) { text+="set "+cap.alias+" "+limit.first+" "+limit.second+"\n"; }
        }
        return text;
    }
    inline void line(std::string const& text)
    { check(gateway::native_io::write_all(STDOUT_FILENO,text+"\n"),"test output write failed"); }
}
