/** @file
 * @brief Time the production permission selection and caller enrollment used by the ROSv2 ABI query.
 * @details Both timed clients open the same logical interface for a registered module. This native
 * process supplies its caller identity directly. Only the actual VM resolves that identity from
 * sealed Wasm frames. The checks after the loop exercise denied and retired identities.
 */
#include <uwvm2/imported/ros/scene/deployment.h>
#include <cassert>
#include <charconv>
#include <cstdint>
#include <memory>
#include <string_view>

namespace scene = uwvm2::imported::ros::scene;

int main(int argc, char** argv)
{
    if(argc != 2) return 2;
    std::string_view input{argv[1]};
    std::uint32_t iterations{};
    auto const parsed = std::from_chars(input.data(), input.data() + input.size(), iterations);
    if(parsed.ec != std::errc{} || parsed.ptr != input.data() + input.size() || !iterations) return 2;

    scene::deployment policy;
    assert(policy.create("robot") == scene::error::success);
    assert(policy.bind("robot", "main") == scene::error::success);
    assert(policy.expose("robot", "abi", scene::exposure::allow) == scene::error::success);
    assert(policy.validate() == scene::error::success);
    scene::caller const principal{&policy, 1};
    std::uint32_t sum{};
    for(std::uint32_t i{}; i < iterations; ++i)
    {
        std::shared_ptr<scene::broker> admitted;
        if(policy.select_exposed("main", "abi", principal, admitted) != scene::error::success || !admitted) return 3;
        sum += 2;
    }
    std::shared_ptr<scene::broker> denied;
    assert(policy.select_exposed("other", "abi", principal, denied) == scene::error::denied && !denied);
    assert(policy.select_exposed("main", "capability.open", principal, denied) == scene::error::denied && !denied);
    policy.retire_member(principal);
    assert(policy.select_exposed("main", "abi", principal, denied) == scene::error::denied && !denied);
    return sum != 2 * iterations;
}
