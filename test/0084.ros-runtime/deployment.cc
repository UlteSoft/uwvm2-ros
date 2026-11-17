/** @file
 * @brief Exercise deployment activation and runtime-generation retirement.
 * @details Initial runtime preparation retires an empty old epoch. It must
 * leave unactivated policy available while retirement of a used epoch must
 * permanently deny reuse, including a reused address with a new generation.
 */
#include <uwvm2/imported/ros/scene/deployment.h>
#include <cassert>
#include <barrier>
#include <thread>
using namespace uwvm2::imported::ros::scene;
int main()
{
    deployment config;
    assert(config.create("robot")==error::success);
    assert(config.bind("robot","main")==error::success);
    assert(config.expose("robot","abi",exposure::allow)==error::success);
    config.retire_generation(1);
    assert(config.validate()==error::success);
    config.retire_generation(2); // Frozen policy still has no active runtime.
    int instance{};std::shared_ptr<broker> first,second;
    assert(config.select("main",{&instance,3},first)==error::success);
    config.retire_generation(2);
    assert(config.select("main",{&instance,3},second)==error::success&&first==second);
    assert(config.select("main",{&instance,4},second)==error::denied);
    config.retire_generation(3);
    assert(config.visible("main","abi")==exposure::hidden);
    assert(config.select("main",{&instance,3},second)==error::denied);
    assert(config.select("main",{&instance,4},second)==error::denied);
    deployment replacement;
    assert(replacement.create("robot")==error::success);
    assert(replacement.bind("robot","main")==error::success);
    assert(replacement.select("main",{&instance,9},first)==error::success);
    replacement.retire_member({&instance,9});
    assert(replacement.select("main",{&instance,9},second)==error::denied);
    assert(replacement.select("main",{&instance,10},second)==error::denied);

    deployment checked;
    assert(checked.create("robot")==error::success);
    assert(checked.bind("robot","main")==error::success);
    assert(checked.expose("robot","clock.read",exposure::deny)==error::success);
    assert(checked.expose("robot","guard.take",exposure::hidden)==error::success);
    auto sentinel=first;
    for(auto action:{"clock.read","guard.take","guard.trigger","clock.read.extra",""})
        assert(checked.select_exposed("main",action,{&instance,10},first)==error::denied&&first==sentinel);
    assert(checked.select_exposed("other","abi",{&instance,10},first)==error::denied&&first==sentinel);
    // Refused exposure must not freeze policy or bind the rejected epoch.
    assert(checked.expose("robot","abi",exposure::allow)==error::success);
    assert(checked.select_exposed("main","abi",{&instance,11},first)==error::success);
    assert(first!=sentinel);
    assert(checked.select_exposed("main","clock.read",{&instance,11},first)==error::denied);
    assert(checked.select_exposed("main","abi",{&instance,12},second)==error::denied);
    handle denied_cap{};
    assert(first->permissions.open({&instance,11},"ungranted",denied_cap)==error::denied);
    checked.retire_generation(10);
    assert(checked.select_exposed("main","abi",{&instance,11},second)==error::success&&first==second);
    checked.retire_member({&instance,11});
    assert(checked.select_exposed("main","abi",{&instance,11},second)==error::denied);
    checked.retire_generation(11);
    assert(checked.select_exposed("main","abi",{&instance,12},second)==error::denied);

    // Concurrent first calls cannot enroll two runtime epochs in one deployment.
    deployment concurrent;
    assert(concurrent.create("robot")==error::success);
    assert(concurrent.bind("robot","main")==error::success);
    assert(concurrent.expose("robot","abi",exposure::allow)==error::success);
    std::barrier start{3};
    std::array<error,2> results{};
    std::array<std::shared_ptr<broker>,2> selected{};
    auto enter=[&](std::size_t index)
    {start.arrive_and_wait();results[index]=concurrent.select_exposed("main","abi",{&instance,20+index},selected[index]);};
    std::thread left{enter,0u},right{enter,1u};start.arrive_and_wait();left.join();right.join();
    assert((results[0]==error::success)!=(results[1]==error::success));
    auto winner=results[0]==error::success?0u:1u;
    assert(results[1u-winner]==error::denied&&!selected[1u-winner]);
    concurrent.retire_generation(20+winner);
    assert(concurrent.select_exposed("main","abi",{&instance,20+winner},first)==error::denied);
}
