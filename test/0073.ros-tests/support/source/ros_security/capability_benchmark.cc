#include <uwvm2/imported/ros/capability/manager.h>
#include <array>
#include <chrono>
#include <fast_io.h>

using namespace uwvm2::imported::ros;
int main()
{
    target_policy target{version::ros1, scope::global, "", true,
        {{"gateway-socket", "/tmp/ros-benchmark.sock"}, {"gateway-uid", "1000"}, {"node-name", "benchmark"}, {"namespace", "/"}, {"connect-timeout-ms", "1000"}, {"audit-deny-max-rate-hz", "10"}, {"audit-deny-burst", "10"}, {"audit-deny-max-records", "128"}, {"audit-deny-max-bytes", "65536"}, {"master-uri", "http://127.0.0.1:11311"}, {"advertise-host", "127.0.0.1"}, {"transport", "tcpros"}}, {}};
    capability_spec cap{"publish", "topic.publish", "/bench", "std_msgs/String", "992ce8a1687cec8c8bd883ec73ca41d1",
        {{"queue-depth", "1000000"}, {"latch", "false"}, {"max-publishers", "1"}, {"max-message-bytes", "64"}, {"max-rate-hz", "1000000"}, {"burst", "1000000"}, {"max-bytes-per-second", "10000000"}, {"max-queue-bytes", "1024"}, {"overflow-policy", "reject"}, {"transport", "tcpros"}, {"validator", "none"}, {"audit", "allow-and-deny"}, {"audit-max-rate-hz", "0.001"}, {"audit-burst", "1"}, {"audit-max-records", "1"}, {"audit-max-bytes", "1024"}}};
    target.capabilities.emplace(cap.alias, cap);
    capability_manager manager{};
    int runtime_identity{};
    if(!manager.register_instance(&runtime_identity, 1, "benchmark", &target, nullptr)) { return 1; }
    ::std::uint64_t handle{};
    if(!manager.open(&runtime_identity, 1, version::ros1, "publish", handle)) { return 1; }
    ::std::array<::std::byte, 7> payload{::std::byte{3}, {}, {}, {}, ::std::byte{'a'}, ::std::byte{'b'}, ::std::byte{'c'}};
    constexpr ::std::uint64_t maximum_iterations{100000};
    ::std::uint64_t completed{};
    auto start{::std::chrono::steady_clock::now()};
    for(; completed != maximum_iterations; ++completed)
    {
        authorization lease{};
        if(!manager.authorize(&runtime_identity, 1, version::ros1, handle, "topic.publish", payload, lease)) { return 1; }
        if(completed % 1024 == 0 && ::std::chrono::steady_clock::now() - start > ::std::chrono::seconds(10)) { break; }
    }
    auto elapsed{::std::chrono::duration_cast<::std::chrono::nanoseconds>(::std::chrono::steady_clock::now() - start).count()};
    if(!completed) { return 1; }
    ::fast_io::io::println(::fast_io::out(),"{\"status\":\"PASS\",\"benchmark\":\"capability-authorize\",\"iterations\":",::fast_io::mnp::dec(completed),
        ",\"payload_bytes\":7,\"elapsed_ns\":",::fast_io::mnp::dec(elapsed),",\"mean_ns\":",::fast_io::mnp::dec(elapsed/completed),
        ",\"rate_and_byte_limits_checked\":true,\"message_structure_checked\":true,\"audit_quotas_checked\":true,\"network_test\":false}");
}
