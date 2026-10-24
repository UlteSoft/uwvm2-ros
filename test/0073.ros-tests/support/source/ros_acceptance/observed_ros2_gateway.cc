/** @file observed_ros2_gateway.cc
 * @brief Test-only compilation of the real Gateway with real Graph forwarding.
 *
 * This translation unit includes the production main unchanged. The observer
 * records actual calls and forwards RTLD_NEXT to the selected native RMW; it
 * neither substitutes results nor expands authority. Qualify this separate
 * ELF, original full-MD dependencies, libdl and full SDK before using it. Never
 * present its test instrumentation as an ordinary production Gateway build.
 */
#include "ros2_graph_observer.h"
#include "../../../../../src/ros/gateway/ros2/main.cc"
