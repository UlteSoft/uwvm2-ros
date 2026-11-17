/** @file
 * @brief Verify trusted lifecycle and debugger hooks across header/module glue.
 * @details The header side compares actual variable addresses, invokes a
 * retirement callback and snapshots metadata installed by a named consumer.
 */
#include <uwvm2/imported/ros/scene/lifecycle.h>
#include <uwvm2/imported/ros/scene/debug.h>
bool configure_scene();
bool observe_scene(void*);
void* named_configuration_address();
void* named_lifecycle_address();
void* named_debug_address();
void* retirement_identity();
unsigned retirement_count();
int main()
{
    namespace scene=uwvm2::imported::ros::scene;
    if(!configure_scene()||!observe_scene(named_configuration_address()))return 1;
    if(named_lifecycle_address()!=&scene::lifecycle::hook)return 2;
    if(named_debug_address()!=&scene::debug::snapshot_hook)return 3;
    scene::lifecycle::retire(retirement_identity(),17,true);
    if(retirement_count()!=1)return 4;
    scene::debug::request query;scene::debug::view output;
    scene::debug::snapshot(query,output);
    if(output.result!=scene::debug::status::ok||output.total!=29)return 5;
    scene::lifecycle::hook.store(nullptr,std::memory_order_release);
    scene::debug::snapshot_hook.store(nullptr,std::memory_order_release);
}
