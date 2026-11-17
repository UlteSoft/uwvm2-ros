/** @file
 * @brief Compile and exercise the unsupported-platform input preparation branch.
 * @details A native compiler with __linux__ undefined checks header portability
 * and fail-closed behavior only. This is not a foreign-platform ABI qualification.
 */
#include <uwvm2/uwvm/imported/scene/input.h>
#include <uwvm2/imported/ros/scene/deployment.h>
#include <cassert>
#include <iostream>
#if defined(__linux__)
#error This probe must select the unsupported-platform branch.
#endif
int main()
{
    namespace scene=uwvm2::imported::ros::scene;
    scene::artifacts::input_plan plan;plan.path="/deployment/input";plan.generation=1;
    std::shared_ptr<scene::storage::immutable_input> output;
    assert(uwvm2::uwvm::imported::scene::prepare_input(plan,{1,"input"},scene::purpose::package,output)==scene::error::unsupported);
    assert(!output);
    scene::deployment deployment;assert(deployment.create("robot")==scene::error::success);
    scene::grant grant;grant.alias="input";grant.resource="bytes";grant.binding="sealed-input";
    grant.kind=scene::purpose::package;grant.actions={"artifact.open","artifact.read"};
    assert(deployment.allow("robot",grant)==scene::error::unsupported);
    std::cout<<"PASS unsupported-platform headers compile and sealed-input preparation/admission fail closed\n";
}
