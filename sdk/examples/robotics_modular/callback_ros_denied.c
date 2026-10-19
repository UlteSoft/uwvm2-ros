/** @file Negative callback variant: a ROS host import below main->motion->
 * callback must be denied13 even if the host separately grants this module the
 * same alias. The Runtime refuses cross-module ROS ancestry; this example does
 * not request a delegation exception. The ordinary callback.c has no ROS import.
 */
#include "denial.h"
#define ROBOT_CALLBACK_CHAIN_CHECK() robot_denied_open()
#define ROBOT_CALLBACK_COUNTS_DENIALS 1
#include "callback_impl.h"
