/**
 * @file ros_import_guest.c
 * @brief Exercise lazy ROS namespace selection without granting a capability.
 * @details Build once for each real namespace or an explicitly rewritten alias.
 * The same exported check is callable from a separately loaded Wasm library.
 */
#ifndef TEST_MODULE
#define TEST_MODULE "uwvm_ros2_v1"
#endif

#if defined(TEST_SCENE_ABI)
#define TEST_INTERFACE "abi"
#define TEST_VERSION 2u
#else
#define TEST_INTERFACE "abi_version"
#define TEST_VERSION 1u
#endif
#ifdef TEST_NONDELEGATED_SCENE_CALL
#undef TEST_VERSION
#define TEST_VERSION 13u
#endif

__attribute__((import_module(TEST_MODULE), import_name(TEST_INTERFACE))) extern unsigned ros_abi_version(void);

__attribute__((export_name("check_version"))) void check_version(void)
{
    if(ros_abi_version() != TEST_VERSION) { __builtin_trap(); }
}

void _start(void)
{
    check_version();
}
