/** @file
 * @brief Sole named-module owner of the scene deployment configuration.
 * @details CLI, import validation and the host import this same object; none
 * textually instantiates another deployment in its own module ownership.
 */
module;
export module uwvm2.uwvm.imported.scene.storage;
export import uwvm2.imported.ros.scene;
#define UWVM_MODULE
#define UWVM_SCENE_CONFIGURATION_OWNER
export {
#include "storage.h"
}
#undef UWVM_SCENE_CONFIGURATION_OWNER
