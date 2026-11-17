/** @file
 * @brief New scene configuration storage, independent of all older host policies.
 */
#pragma once
#ifndef UWVM_MODULE
#include <uwvm2/imported/ros/scene/deployment.h>
#elif !defined(UWVM_SCENE_CONFIGURATION_OWNER)
#error "Import uwvm2.uwvm.imported.scene.storage instead of including its storage header in a named module."
#endif
namespace uwvm2::uwvm::imported::scene
{
    inline ::uwvm2::imported::ros::scene::deployment configuration;
}
