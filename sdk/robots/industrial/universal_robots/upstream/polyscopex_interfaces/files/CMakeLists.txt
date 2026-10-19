cmake_minimum_required(VERSION 3.14.4)

project(urinterfaces)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

set(msg_files
  "msg/ActiveTCP.msg"
  "msg/Analog.msg"
  "msg/AnalogStamped.msg"
  "msg/AnalogIO.msg"
  "msg/AnalogIOStateStamped.msg"
  "msg/BoolStamped.msg"
  "msg/ControlCycle.msg"
  "msg/Current.msg"
  "msg/Digital.msg"
  "msg/DigitalStamped.msg"
  "msg/DigitalIO.msg"
  "msg/DigitalIOStateStamped.msg"
  "msg/DoubleMultiArrayStamped.msg"
  "msg/Float32Stamped.msg"
  "msg/Float64Stamped.msg"
  "msg/JointMode.msg"
  "msg/JointTemperature.msg"
  "msg/JointVoltage.msg"
  "msg/RobotMode.msg"
  "msg/RuntimeState.msg"
  "msg/SafetyStatus.msg"
  "msg/SpeedSliderFraction.msg"
  "msg/TCPState.msg"
  "msg/ToolOutputMode.msg"
  "msg/VoltageCurrentStamped.msg"
  "msg/WorldModelNames.msg"
  "msg/JsonStringType.msg"
  "msg/VariableUpdate.msg"
  "msg/SharedVariablesEditResponse.msg"
)

set(srv_files
  "srv/SetAnalogOutput.srv"
  "srv/SetDigitalOutput.srv"
  "srv/SetSpeedFraction.srv"
  "srv/SharedVariablesEditor.srv"
  "srv/VariablesViewer.srv"
)

set(action_files
  "action/SetMode.action"
)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${action_files}
  ${msg_files}
  ${srv_files}
  DEPENDENCIES builtin_interfaces std_msgs geometry_msgs sensor_msgs
  ADD_LINTER_TESTS
)

ament_export_dependencies(rosidl_default_runtime)
ament_package()
