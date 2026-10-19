#pragma once
#include "../ros_ipc/probe_common.h"
#include <uwvm2/imported/ros/ipc/action.h>
namespace ros_action_probe
{
    using namespace ros_probe;
    inline core::capability_spec grant(core::version v,std::string alias,std::string operation,std::string resource)
    {
        core::capability_spec cap{alias,operation,resource,v==core::version::ros1?"actionlib_tutorials/Fibonacci":"action_tutorials_interfaces/action/Fibonacci",v==core::version::ros1?"f59df5767bf7634684781c92598b2406":"RIHS01_525d2c3c8a3979003645ffa01186e360347194550426993cad2fb950058ab5a0",{}};
        cap.limits={{"fibonacci.max-order","10"},{"max-active-goals","2"},{"max-actions","1"},{"timeout-ms","5000"},{"max-goal-bytes","64"},{"max-feedback-bytes","1024"},{"max-result-bytes","1024"},{"cancel-scope",operation=="action.cancel-any-goal"?"any":operation=="action.cancel-own-goal"?"own":"not-applicable"},{"max-rate-hz","1000"},{"burst","10"},{"max-bytes-per-second","65536"},{"max-event-queue-depth","10"},{"max-event-queue-bytes","10240"},{"overflow-policy","reject"},{"validator","none"},{"audit","allow-and-deny"},{"audit-max-rate-hz","100"},{"audit-burst","10"},{"audit-max-records","10000"},{"audit-max-bytes","1048576"}};return cap;
    }
    inline void configure(gateway::configuration& c)
    {
        c.policy.capabilities.clear();
        for(auto const& pair:std::map<std::string,std::string>{{"goal","action.send-goal"},{"feedback","action.receive-feedback"},{"result","action.receive-result"},{"own","action.cancel-own-goal"},{"any","action.cancel-any-goal"}})
        {auto cap{grant(c.policy.ros_version,pair.first,pair.second,"/uwvm_action/native")};c.policy.capabilities.emplace(cap.alias,std::move(cap));}
        for(auto const& pair:std::map<std::string,std::string>{{"server","action.advertise"},{"responder","action.respond"}})
        {auto cap{grant(c.policy.ros_version,pair.first,pair.second,"/uwvm_action/guest")};c.policy.capabilities.emplace(cap.alias,std::move(cap));}
    }
    inline std::vector<std::uint8_t> id_bytes(std::uint64_t id){std::vector<std::uint8_t> data;ipc::put(data,id,8);return data;}
    inline std::vector<std::uint8_t> order(std::int32_t value){std::vector<std::uint8_t> data;ipc::put(data,static_cast<std::uint32_t>(value),4);return data;}
    inline std::uint64_t send(ipc::client& client,std::int32_t value)
    {auto reply{client.call("action.send-goal","goal",order(value),1500)};require(!reply.error&&reply.payload.size()==8&&ipc::get(reply.payload.data(),8),"real generated Action goal accepted");return ipc::get(reply.payload.data(),8);}
    inline ipc::call_result poll(ipc::client& client,std::string operation,std::string alias,std::vector<std::uint8_t> const& data)
    {for(unsigned n{};n!=300;++n){auto result{client.call(operation,alias,data,1000)};if(result.error!=11){return result;}std::this_thread::sleep_for(std::chrono::milliseconds(10));}throw std::runtime_error("Action event timeout");}
    inline std::vector<std::int32_t> expected{0,1,1,2,3,5};
}
