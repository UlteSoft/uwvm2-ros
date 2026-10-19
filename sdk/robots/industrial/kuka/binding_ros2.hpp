/** Complete original family typed aliases; never Native layout aliases or authority. */
#pragma once
#ifndef UWVM_INDUSTRIAL_GENERATED_RECORDS
#error "Select the actual same-source generated bounded_records.hpp"
#endif
#include UWVM_INDUSTRIAL_GENERATED_RECORDS
#ifndef UWVM_INDUSTRIAL_GENERATED_INTERFACES
#error "Select the matching actual generated bounded_interfaces.hpp"
#endif
#include UWVM_INDUSTRIAL_GENERATED_INTERFACES
namespace uwvm_industrial::kuka_ros2 {
namespace builtin_interfaces {
using Duration=::uwvm_custom::Ros2Builtin_interfacesMsgDuration;
using Time=::uwvm_custom::Ros2Builtin_interfacesMsgTime;
}
namespace control_msgs {
using FollowJointTrajectoryClient=::uwvm_custom::Ros2Control_msgsActionFollowJointTrajectoryClient;
using FollowJointTrajectoryServer=::uwvm_custom::Ros2Control_msgsActionFollowJointTrajectoryServer;
using FollowJointTrajectoryGoal=::uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Goal;
using FollowJointTrajectoryResult=::uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Result;
using FollowJointTrajectoryFeedback=::uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Feedback;
using JointComponentTolerance=::uwvm_custom::Ros2Control_msgsMsgJointComponentTolerance;
using JointTolerance=::uwvm_custom::Ros2Control_msgsMsgJointTolerance;
}
namespace geometry_msgs {
using Quaternion=::uwvm_custom::Ros2Geometry_msgsMsgQuaternion;
using Transform=::uwvm_custom::Ros2Geometry_msgsMsgTransform;
using Twist=::uwvm_custom::Ros2Geometry_msgsMsgTwist;
using Vector3=::uwvm_custom::Ros2Geometry_msgsMsgVector3;
}
namespace kuka_driver_interfaces {
using FRIState=::uwvm_custom::Ros2Kuka_driver_interfacesMsgFRIState;
using FRIStateArray=::uwvm_custom::Ros2Kuka_driver_interfacesMsgFRIStateArray;
using FriConfiguration=::uwvm_custom::Ros2Kuka_driver_interfacesMsgFriConfiguration;
using HardwareEvent=::uwvm_custom::Ros2Kuka_driver_interfacesMsgHardwareEvent;
using KssStatus=::uwvm_custom::Ros2Kuka_driver_interfacesMsgKssStatus;
using KssStatusArray=::uwvm_custom::Ros2Kuka_driver_interfacesMsgKssStatusArray;
}
namespace lifecycle_msgs {
using State=::uwvm_custom::Ros2Lifecycle_msgsMsgState;
using Transition=::uwvm_custom::Ros2Lifecycle_msgsMsgTransition;
using ChangeStateClient=::uwvm_custom::Ros2Lifecycle_msgsSrvChangeStateClient;
using ChangeStateServer=::uwvm_custom::Ros2Lifecycle_msgsSrvChangeStateServer;
using ChangeStateRequest=::uwvm_custom::Ros2Lifecycle_msgsSrvChangeState_Request;
using ChangeStateResponse=::uwvm_custom::Ros2Lifecycle_msgsSrvChangeState_Response;
using GetStateClient=::uwvm_custom::Ros2Lifecycle_msgsSrvGetStateClient;
using GetStateServer=::uwvm_custom::Ros2Lifecycle_msgsSrvGetStateServer;
using GetStateRequest=::uwvm_custom::Ros2Lifecycle_msgsSrvGetState_Request;
using GetStateResponse=::uwvm_custom::Ros2Lifecycle_msgsSrvGetState_Response;
}
namespace sensor_msgs {
using JointState=::uwvm_custom::Ros2Sensor_msgsMsgJointState;
}
namespace std_msgs {
using Header=::uwvm_custom::Ros2Std_msgsMsgHeader;
}
namespace std_srvs {
using TriggerClient=::uwvm_custom::Ros2Std_srvsSrvTriggerClient;
using TriggerServer=::uwvm_custom::Ros2Std_srvsSrvTriggerServer;
using TriggerRequest=::uwvm_custom::Ros2Std_srvsSrvTrigger_Request;
using TriggerResponse=::uwvm_custom::Ros2Std_srvsSrvTrigger_Response;
}
namespace trajectory_msgs {
using JointTrajectory=::uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectory;
using JointTrajectoryPoint=::uwvm_custom::Ros2Trajectory_msgsMsgJointTrajectoryPoint;
using MultiDOFJointTrajectory=::uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectory;
using MultiDOFJointTrajectoryPoint=::uwvm_custom::Ros2Trajectory_msgsMsgMultiDOFJointTrajectoryPoint;
}
}
