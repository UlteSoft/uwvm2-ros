/** @file
 * @brief Install scene hooks and policy through the named configuration owner.
 * @details The companion textual translation unit must reach these exact hooks.
 */
#include <atomic>
#include <cstdint>
import uwvm2.uwvm.imported.scene.storage;

namespace scene=uwvm2::imported::ros::scene;
namespace host=uwvm2::uwvm::imported::scene;
static unsigned retirements{};
static void retired(void const* instance,std::uint64_t generation,bool all) noexcept
{if(instance==&retirements&&generation==17&&all)++retirements;}
static void inspected(scene::debug::request const&,scene::debug::view& output) noexcept
{output.result=scene::debug::status::ok;output.total=29;}

bool configure_scene()
{
    scene::lifecycle::hook.store(retired,std::memory_order_release);
    scene::debug::snapshot_hook.store(inspected,std::memory_order_release);
    return host::configuration.create("module-environment")==scene::error::success&&
        host::configuration.bind("module-environment","fixture")==scene::error::success&&
        host::configuration.expose("module-environment","guard.create",scene::exposure::allow)==scene::error::success;
}
void* named_configuration_address(){return &host::configuration;}
void* named_lifecycle_address(){return &scene::lifecycle::hook;}
void* named_debug_address(){return &scene::debug::snapshot_hook;}
void* retirement_identity(){return &retirements;}
unsigned retirement_count(){return retirements;}
