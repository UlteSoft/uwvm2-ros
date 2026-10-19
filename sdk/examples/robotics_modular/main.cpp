/** @file Actual C++ Node/typed Publisher, with the same independently linked
 * libraries and root-only ROS boundary as main.c. No C++ object is exported.
 * Constructed objects are destroyed before the explicit WASIp1 process exit.
 */
#include "graph.h"
#include "main_io.h"
#include "denial.h"
#include <roswasm/roswasmcpp.hpp>
#ifndef ROBOT_MODULAR_PUBLISH
#define ROBOT_MODULAR_PUBLISH 0
#endif
static uint32_t robot_main() {
    int32_t decision = ROBOT_INVALID;
    auto error = robot_graph(&decision); if(error) return error;
    if(decision != 60) return UWVM_ROS_INVALID;
    if(ROBOT_ROS(abi_version)() != 1) return UWVM_ROS_UNSUPPORTED;
#if ROBOT_MODULAR_PUBLISH
    auto backend = *ROBOT_BACKEND();
    roswasmcpp::Context context{backend}; roswasmcpp::Node node{context,"modular-main"};
    auto publisher = node.create_publisher<roswasmcpp::msg::String>("modular-out");
    error = publisher.error(); if(error) return error;
    error = publisher.publish({"robotics-modular:warmup"});
    if(!error) error = ROBOT_MARKER("MODULAR_ENDPOINT_CREATED\n");
    if(!error) error = robot_gate();
    if(!error) error = publisher.publish({"robotics-modular:command=60"});
    auto closed = publisher.close(); if(!error) error = closed;
    if(!error) error = ROBOT_MARKER("MODULAR_PUBLISHED\n");
    return error;
#else
    error = robot_denied_open();
    if(!error) error = ROBOT_MARKER("PASS_MODULAR_DEFAULT_DENIED\n");
    return error;
#endif
}
ROBOT_EXTERN ROBOT_EXPORT("_start") __attribute__((noreturn)) void _start() {robot_exit(robot_main());}
