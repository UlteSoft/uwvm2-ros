/**
 * @file ros_import_guest.c
 * @brief Exercise lazy ROS namespace selection without granting a capability.
 * @details Build once for each real namespace or an explicitly rewritten alias.
 * The same exported check is callable from a separately loaded Wasm library.
 */
#ifndef TEST_MODULE
#define TEST_MODULE "uwvm_ros2_v1"
#endif

__attribute__((import_module(TEST_MODULE), import_name("abi_version"))) extern unsigned ros_abi_version(void);

__attribute__((export_name("check_version"))) void check_version(void)
{
    if(ros_abi_version() != 1u) { __builtin_trap(); }
}

void _start(void)
{
    check_version();
}
