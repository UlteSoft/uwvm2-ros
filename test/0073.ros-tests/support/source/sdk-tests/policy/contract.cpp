/**
 * @file contract.cpp
 * @brief Check SDK policy output with the actual Worker/Gateway validators.
 *
 * The serial local runner compiles this against a frozen Runtime source copy
 * and passes only generated /tmp config filenames. Loading/validating a config
 * does not create a native ROS object, connect a socket, authenticate a caller
 * or apply a capability. Test PIDs/UIDs are synthetic input-format values.
 * Each supported profile must satisfy both validators. Regression mutations
 * independently check the old Action QoS names and missing exact fields.
 */
#include <uwvm2/imported/ros/policy/policy.h>
#include "src/ros/gateway/common/config.h"
#include <cassert>

namespace core = uwvm2::imported::ros;
namespace gateway = uwvm2::ros_gateway;

/** Require both producers' admission checks to refuse a malformed grant. */
static void rejected(core::version backend, core::capability_spec const& cap) {
    assert(core::validate_capability(backend, cap).code == core::errc::invalid_argument);
    bool refused{};
    try { gateway::validate(backend, cap); }
    catch (std::runtime_error const&) { refused = true; }
    assert(refused);
}

int main(int argc, char** argv) {
    unsigned profiles{}, grants{}, mutation_checks{};
    try {
        assert(argc == 76);
        for(int index = 1; index < argc; ++index) {
            auto config = gateway::load(argv[index]);
            config.policy.enabled = true; // validate the emitted enabled blueprint only
            auto checked = core::validate_target(config.policy);
            if(!checked) {
                fast_io::perrln("FAIL policy ", std::string_view{argv[index]}, ": ",
                               std::string_view{checked.detail});
                return 1;
            }
            ++profiles;
            grants += static_cast<unsigned>(config.policy.capabilities.size());
            if(config.policy.ros_version != core::version::ros2 ||
               config.policy.capabilities.size() != 2 ||
               !config.policy.capabilities.contains("fib-advertise")) continue;
            auto const& cap = config.policy.capabilities.at("fib-advertise");
            auto old_names = cap;
            for(std::string_view channel : {"feedback", "status"}) {
                for(std::string_view property : {"reliability", "durability", "history", "depth",
                                     "deadline", "lifespan", "liveliness", "liveliness-lease-duration"}) {
                    auto key = fast_io::concat_std("qos.", channel, ".", property);
                    auto node = old_names.limits.extract(key);
                    assert(!node.empty());
                    node.key() = fast_io::concat_std("qos.", channel, "-topic.", property);
                    old_names.limits.insert(std::move(node));
                }
            }
            rejected(core::version::ros2, old_names); mutation_checks += 2;
            for(std::string_view channel : {"goal-service", "cancel-service", "result-service", "feedback", "status"}) {
                for(std::string_view property : {"reliability", "durability", "history", "depth",
                                     "deadline", "lifespan", "liveliness", "liveliness-lease-duration"}) {
                    auto missing = cap;
                    assert(missing.limits.erase(fast_io::concat_std("qos.", channel, ".", property)) == 1);
                    rejected(core::version::ros2, missing); mutation_checks += 2;
                }
            }
        }
        assert(profiles == 75 && mutation_checks == 82);
        core::target_policy empty{};
        assert(core::validate_target(empty) && !empty.enabled && empty.capabilities.empty());
        fast_io::println("{\"status\":\"ACTUAL_POLICY_VALIDATORS_PASS\",\"profiles\":", profiles,
                         ",\"grants\":", grants, ",\"mutation_checks\":", mutation_checks,
                         ",\"native_ROS_execution\":false}");
        return 0;
    } catch(std::exception const& error) {
        fast_io::perrln("FAIL policy config: ", std::string_view{error.what()});
        return 1;
    }
}
