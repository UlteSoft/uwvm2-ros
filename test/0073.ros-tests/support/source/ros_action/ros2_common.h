#pragma once
#include "../ros_ipc/probe_common.h"
#include <uwvm2/imported/ros/ipc/action.h>
namespace action_probe
{
    using namespace ros_probe;
    inline core::capability_spec action(core::version v,std::string alias,std::string operation,std::string resource)
    {
        core::capability_spec c{alias,operation,resource,v==core::version::ros1?"actionlib_tutorials/Fibonacci":"action_tutorials_interfaces/action/Fibonacci",v==core::version::ros1?"f59df5767bf7634684781c92598b2406":"RIHS01_525d2c3c8a3979003645ffa01186e360347194550426993cad2fb950058ab5a0",{}};
        c.limits={{"max-active-goals","8"},{"max-actions","2"},{"timeout-ms","5000"},{"max-goal-bytes","256"},{"max-feedback-bytes","256"},{"max-result-bytes","256"},{"fibonacci.max-order","46"},{"cancel-scope",operation=="action.cancel-own-goal"?"own":operation=="action.cancel-any-goal"?"any":"not-applicable"},{"max-rate-hz","100"},{"burst","20"},{"max-bytes-per-second","1048576"},{"max-event-queue-depth","8"},{"max-event-queue-bytes","2048"},{"overflow-policy","drop-oldest"},{"validator","none"},{"audit","allow-and-deny"},{"audit-max-rate-hz","100"},{"audit-burst","20"},{"audit-max-records","10000"},{"audit-max-bytes","1048576"}};
        if(v==core::version::ros2) { for(auto channel:{"goal-service","cancel-service","result-service","feedback","status"}) { auto prefix{std::string{"qos."}+channel+"."};c.limits.insert({{prefix+"reliability","reliable"},{prefix+"durability",std::string{channel}=="status"?"transient-local":"volatile"},{prefix+"history","keep-last"},{prefix+"depth","8"},{prefix+"deadline","infinite"},{prefix+"lifespan","infinite"},{prefix+"liveliness","automatic"},{prefix+"liveliness-lease-duration","infinite"}}); } }return c;
    }
    inline std::vector<std::uint8_t> number(std::uint64_t n,unsigned width=8) { std::vector<std::uint8_t> data;ipc::put(data,n,width);return data; }
    inline ipc::call_result poll(ipc::client& client,std::string const& operation,std::string const& alias,std::vector<std::uint8_t> const& request)
    { for(unsigned n{};n!=200;++n) { auto result{client.call(operation,alias,request,5500)};if(result.error!=11) { return result; }std::this_thread::sleep_for(std::chrono::milliseconds(10)); }return {110,{}}; }
}
