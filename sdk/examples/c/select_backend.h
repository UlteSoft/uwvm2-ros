/** @file
 * @brief Compile-time ROS1/ROS2 selection used by all examples in this directory.
 * Define ROSWASM_EXAMPLE_ROS_VERSION=1 for ROS1 or =2 for ROS2 (the default).
 * Only the selected backend header is included; no other module is fabricated.
 * Host grants and enabled Worker backend must match the compiled import module.
 */
#ifndef ROSWASM_EXAMPLE_SELECT_BACKEND_H
#define ROSWASM_EXAMPLE_SELECT_BACKEND_H
#ifndef ROSWASM_EXAMPLE_ROS_VERSION
#define ROSWASM_EXAMPLE_ROS_VERSION 2
#endif
/** @brief Give a callable tutorial helper an explicit WebAssembly export name.
 * Exporting a helper keeps its real SDK operations available to guest-side
 * integration when main deliberately performs no ROS work. It opens no alias
 * and adds no permission. Native builds retain the ordinary function declaration.
 */
#if defined(__wasm__) && defined(__clang__)
#define ROSWASM_EXAMPLE_EXPORT(name) __attribute__((export_name(name)))
#else
#define ROSWASM_EXAMPLE_EXPORT(name)
#endif
#if ROSWASM_EXAMPLE_ROS_VERSION == 1
#include <roswasm/ros1.h>
#define ROSWASM_IMPORT(name) uwvm_ros1_##name
#define ROSWASM_BACKEND() roswasmc_ros1_backend()
#elif ROSWASM_EXAMPLE_ROS_VERSION == 2
#include <roswasm/ros2.h>
#define ROSWASM_IMPORT(name) uwvm_ros2_##name
#define ROSWASM_BACKEND() roswasmc_ros2_backend()
#else
#error "ROSWASM_EXAMPLE_ROS_VERSION must be 1 or 2"
#endif
#endif
