/** Cold typed source-constant proof. No Node, socket or authority. */
#pragma once
#include <string_view>
#include <cstdint>
namespace uwvm_industrial_source_ea77af3eb29988df0d13d0078d3edad76737df00cedef5eba0d0f85f5c2d33db {
inline bool validate() {
if(::sensor_msgs::BatteryState::POWER_SUPPLY_STATUS_UNKNOWN!=UINT32_C(0))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_STATUS_CHARGING!=UINT32_C(1))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_STATUS_DISCHARGING!=UINT32_C(2))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_STATUS_NOT_CHARGING!=UINT32_C(3))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_STATUS_FULL!=UINT32_C(4))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_HEALTH_UNKNOWN!=UINT32_C(0))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_HEALTH_GOOD!=UINT32_C(1))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_HEALTH_OVERHEAT!=UINT32_C(2))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_HEALTH_DEAD!=UINT32_C(3))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_HEALTH_OVERVOLTAGE!=UINT32_C(4))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_HEALTH_UNSPEC_FAILURE!=UINT32_C(5))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_HEALTH_COLD!=UINT32_C(6))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_HEALTH_WATCHDOG_TIMER_EXPIRE!=UINT32_C(7))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_HEALTH_SAFETY_TIMER_EXPIRE!=UINT32_C(8))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_TECHNOLOGY_UNKNOWN!=UINT32_C(0))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_TECHNOLOGY_NIMH!=UINT32_C(1))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_TECHNOLOGY_LION!=UINT32_C(2))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_TECHNOLOGY_LIPO!=UINT32_C(3))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_TECHNOLOGY_LIFE!=UINT32_C(4))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_TECHNOLOGY_NICD!=UINT32_C(5))return false;
if(::sensor_msgs::BatteryState::POWER_SUPPLY_TECHNOLOGY_LIMN!=UINT32_C(6))return false;
if(::turtlebot3_msgs::SensorState::BUMPER_FORWARD!=UINT32_C(1))return false;
if(::turtlebot3_msgs::SensorState::BUMPER_BACKWARD!=UINT32_C(2))return false;
if(::turtlebot3_msgs::SensorState::CLIFF!=UINT32_C(1))return false;
if(::turtlebot3_msgs::SensorState::SONAR!=UINT32_C(1))return false;
if(::turtlebot3_msgs::SensorState::ILLUMINATION!=UINT32_C(1))return false;
if(::turtlebot3_msgs::SensorState::BUTTON0!=UINT32_C(1))return false;
if(::turtlebot3_msgs::SensorState::BUTTON1!=UINT32_C(2))return false;
if(::turtlebot3_msgs::SensorState::ERROR_LEFT_MOTOR!=UINT32_C(1))return false;
if(::turtlebot3_msgs::SensorState::ERROR_RIGHT_MOTOR!=UINT32_C(2))return false;
if(::turtlebot3_msgs::SensorState::TORQUE_ON!=UINT32_C(1))return false;
if(::turtlebot3_msgs::SensorState::TORQUE_OFF!=UINT32_C(2))return false;
if(::turtlebot3_msgs::Sound::OFF!=UINT32_C(0))return false;
if(::turtlebot3_msgs::Sound::ON!=UINT32_C(1))return false;
if(::turtlebot3_msgs::Sound::LOW_BATTERY!=UINT32_C(2))return false;
if(::turtlebot3_msgs::Sound::ERROR!=UINT32_C(3))return false;
if(::turtlebot3_msgs::Sound::BUTTON1!=UINT32_C(4))return false;
if(::turtlebot3_msgs::Sound::BUTTON2!=UINT32_C(5))return false;
return true;}
}
