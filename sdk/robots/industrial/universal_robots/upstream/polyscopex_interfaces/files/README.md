# urinterfaces

[![industrial_ci](https://github.com/UniversalRobots/urinterfaces/actions/workflows/industrial_ci.yml/badge.svg?branch=master)](https://github.com/UniversalRobots/urinterfaces/actions/workflows/industrial_ci.yml)

[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

This repository contains Universal Robots ROS(2) interfaces like messages, services and actions.

In contrast to the
[`ur_msgs`](https://index.ros.org/p/ur_msgs/github-ros-industrial-ur_msgs/#rolling) package, this
package contains the messages as they are published by the robot controller natively starting from
Polyscope 10.7.

The [`ur_robot_driver`](https://github.com/UniversalRobots/Universal_Robots_ROS2_Driver) still uses
the `ur_msgs` package, but it is planned to switch to this package in the future.
