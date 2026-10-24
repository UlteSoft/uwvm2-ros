/** Strict syntax inventory; no imported operation is executed by this fixture. */
#include <platform/ros/c/roswasm/roswasm.h>
#include <platform/ros1/c/roswasm/ros1.h>
#include <platform/ros2/c/roswasm/ros2.h>
#include <platform/ros1/c/roswasm/service_client_ros1.h>
#include <platform/ros2/c/roswasm/service_client_ros2.h>
#include <platform/ros/c/roswasm/rmw_publisher_v1.h>
#include <platform/ros/c/roswasm/rmw_subscription_v1.h>
#include <platform/ros/c/roswasm/rmw_service_v1.h>
#include <platform/ros/c/roswasm/library_profile.h>
int main(void) {return sizeof(uwvm_ros_u64)!=8;}
