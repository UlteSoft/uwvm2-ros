/** @file
 * @brief Check the explicit unsupported-platform volatile-output admission.
 * @details A native -U__linux__ build probes header isolation and refusal only;
 * it is not a foreign OS ABI or allocator qualification.
 */
#include <uwvm2/imported/ros/scene/deployment.h>
#include <cassert>
#include <iostream>
#if defined(__linux__)
#error This probe must select the unsupported-platform branch.
#endif
int main()
{
    namespace scene=uwvm2::imported::ros::scene;
    scene::outputs::registry registry;
    assert(registry.bind("cache","scratch",{4,1,4,4})==scene::error::unsupported);
    scene::deployment deployment;assert(deployment.create("robot")==scene::error::success);
    scene::grant grant;grant.alias="cache";grant.resource="scratch";grant.kind=scene::purpose::cache;
    grant.binding="volatile-output";grant.actions={"artifact.begin","artifact.receipt"};
    assert(deployment.allow("robot",grant)==scene::error::unsupported);
    std::cout<<"PASS unsupported volatile-output binding and grant fail closed\n";
}
