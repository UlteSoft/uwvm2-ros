/** @file
 * @brief Observe a policy configured by another named-module consumer.
 * @details A separately owned inline deployment would leave the import hidden.
 */
import uwvm2.uwvm.imported.scene.storage;
bool observe_scene(void* expected)
{
    namespace scene=uwvm2::imported::ros::scene;
    auto& config=uwvm2::uwvm::imported::scene::configuration;
    return &config==expected&&
        config.visible("fixture","guard.create")==scene::exposure::allow&&
        config.visible("unbound","guard.create")==scene::exposure::hidden&&
        config.visible("fixture","guard.trigger")==scene::exposure::hidden;
}
