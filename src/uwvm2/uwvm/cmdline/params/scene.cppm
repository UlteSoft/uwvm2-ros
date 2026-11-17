/** @file
 * @brief Scene CLI partition using the sole deployment configuration owner.
 * @details Configuration exposes/grants nothing implicitly and importing this
 * partition performs no provider I/O. Its semantic model has no CLI dependency.
 */
module;
#include <uwvm2/utils/container/standard.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <uwvm2/utils/macro/push_macros.h>
#include <uwvm2/uwvm/runtime/macro/push_macros.h>
export module uwvm2.uwvm.cmdline.params:scene;
import fast_io;
import uwvm2.utils.container;
import uwvm2.utils.cmdline;
import uwvm2.uwvm.io;
import uwvm2.uwvm.imported.scene.storage;
#define UWVM_MODULE
export {
#include "scene.h"
}
