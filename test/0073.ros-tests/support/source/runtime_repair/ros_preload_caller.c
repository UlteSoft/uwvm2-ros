/**
 * @file ros_preload_caller.c
 * @brief Call a Wasm preload whose own imports require a ROS namespace.
 * @details The executable has no direct ROS import, so successful execution
 * proves that built-in selection also inspects the loaded library.
 */
__attribute__((import_module("ros-library"), import_name("check_version"))) extern void check_version(void);

void _start(void)
{
    check_version();
}
