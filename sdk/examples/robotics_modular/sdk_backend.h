/** Compile exactly one real SDK backend. A module name never selects authority. */
#ifndef ROSWASM_ROBOTICS_SDK_BACKEND_H
#define ROSWASM_ROBOTICS_SDK_BACKEND_H
#ifndef ROSWASM_EXAMPLE_ROS_VERSION
#define ROSWASM_EXAMPLE_ROS_VERSION 2
#endif
#if ROSWASM_EXAMPLE_ROS_VERSION == 1
#include <roswasm/ros1.h>
#define ROBOT_ROS(name) uwvm_ros1_##name
#define ROBOT_BACKEND() roswasmc_ros1_backend()
#elif ROSWASM_EXAMPLE_ROS_VERSION == 2
#include <roswasm/ros2.h>
#define ROBOT_ROS(name) uwvm_ros2_##name
#define ROBOT_BACKEND() roswasmc_ros2_backend()
#else
#error "Select exactly ROSWASM_EXAMPLE_ROS_VERSION=1 or =2"
#endif
#endif
