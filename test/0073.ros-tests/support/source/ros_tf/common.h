#pragma once
#include "../ros_ipc/probe_common.h"
#include <uwvm2/imported/ros/ipc/transform.h>
namespace ros_tf_probe
{
    inline ros_probe::core::capability_spec capability(ros_probe::core::version version,std::string alias,std::string operation)
    {
        using namespace ros_probe;
        auto c{topic(version,alias,"topic.subscribe","/tf",version==core::version::ros1?"geometry_msgs/TransformStamped":"geometry_msgs/msg/TransformStamped",version==core::version::ros1?"b5764a33bfeb3588febc2682852579b0":"RIHS01_0a241f87d04668d94099cbb5ba11691d5ad32c2f29682e4eb5653424bd275206")};
        c.operation=std::move(operation); c.limits.erase("max-subscribers");
        c.limits["validator"]="transform-bounds";
        c.limits.insert({{"source-frame","base"},{"target-frame","map"},{"tf.max-translation-abs","10"},{"tf.quaternion-norm-tolerance","0.000001"},{"timeout-ms","1500"},{"max-inflight","1"},{"buffer-cache-ms","1000"}});
        if(version==core::version::ros1&&c.operation=="tf.broadcast") { c.limits["latch"]="false"; }
        return c;
    }
    inline void configure(ros_probe::gateway::configuration& configuration)
    {
        configuration.policy.capabilities.clear();
        for(auto const& item:std::vector<std::pair<std::string,std::string>>{{"lookup","tf.lookup"},{"broadcast","tf.broadcast"}})
        { auto c{capability(configuration.policy.ros_version,item.first,item.second)}; configuration.policy.capabilities.emplace(c.alias,std::move(c)); }
    }
    inline ros_probe::ipc::transform_data transform()
    { return {0,123,456,"map","base",{{1.25,2.5,3.75,0,0,0,1}}}; }
    inline void assert_lookup(ros_probe::ipc::call_result const& result)
    {
        using namespace ros_probe;
        require(!result.error,"actual filtered TF lookup"); ipc::transform_data value;
        require(ipc::decode_transform(result.payload,value),"TF lookup response structure");
        auto expected{transform()}; require(value.parent==expected.parent&&value.child==expected.child&&value.values==expected.values,"TF exact permitted frame and transform");
    }
    inline void assert_capability_deadline(ros_probe::core::version version,std::string const& executable)
    {
        using namespace ros_probe;
        fixture bounded{version,executable,[&](gateway::configuration& cfg)
        {cfg.policy.capabilities.clear();cfg.policy.connection["node-name"]="uwvm_tf_deadline_probe";auto cap{capability(version,"lookup","tf.lookup")};cap.limits["source-frame"]="never_seen";cap.limits["timeout-ms"]="120";cfg.policy.capabilities.emplace(cap.alias,std::move(cap));}};
        ipc::client client;bounded.connect(client);auto start{std::chrono::steady_clock::now()};auto result{client.call("tf.lookup","lookup",std::vector<std::uint8_t>(8),2000)};auto elapsed{std::chrono::steady_clock::now()-start};
        require(result.error==110&&result.payload.empty()&&elapsed<std::chrono::milliseconds(600)&&elapsed>=std::chrono::milliseconds(80),"actual TF lookup obeys capability120ms instead of caller2000ms");client.close();
        ::ros_probe::print_fixture(STDOUT_FILENO,"PASS actual native TF absent-frame lookup deadline capability120ms/caller2000ms\n");
    }
}
