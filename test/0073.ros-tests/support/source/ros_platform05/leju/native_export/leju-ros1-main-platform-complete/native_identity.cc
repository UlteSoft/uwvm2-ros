/** Industrial native metadata source; no Node, endpoint or grant. */
#include "test/0073.ros-tests/support/source/ros_custom_types/bounded-v2/native_protocol.h"
#include <array>
#include <string_view>
#include <stdexcept>
#include <csignal>
#include <cstdint>
#if UWVM_CUSTOM_NATIVE_ROS_VERSION != 1
#error "Select the exact source-reviewed ROS major"
#endif
#include <aruco_msgs/Marker.h>
#include <aruco_msgs/MarkerArray.h>
#include <easy_handeye_msgs/CheckStartingPose.h>
#include <easy_handeye_msgs/ComputeCalibration.h>
#include <easy_handeye_msgs/EnumerateTargetPoses.h>
#include <easy_handeye_msgs/ExecutePlan.h>
#include <easy_handeye_msgs/HandeyeCalibration.h>
#include <easy_handeye_msgs/ListAlgorithms.h>
#include <easy_handeye_msgs/PlanToSelectedTargetPose.h>
#include <easy_handeye_msgs/RemoveSample.h>
#include <easy_handeye_msgs/SampleList.h>
#include <easy_handeye_msgs/SelectTargetPose.h>
#include <easy_handeye_msgs/SetAlgorithm.h>
#include <easy_handeye_msgs/TakeSample.h>
#include <easy_handeye_msgs/TargetPoseList.h>
#include <geographic_msgs/GeoPoint.h>
#include <geographic_msgs/GeoPose.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseWithCovariance.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/Transform.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>
#include <grid_map_msgs/GetGridMap.h>
#include <grid_map_msgs/GetGridMapInfo.h>
#include <grid_map_msgs/GridMap.h>
#include <grid_map_msgs/GridMapInfo.h>
#include <grid_map_msgs/ProcessFile.h>
#include <grid_map_msgs/SetGridMap.h>
#include <h12pro_controller_node/ECJointMotordata.h>
#include <h12pro_controller_node/ExecuteArmAction.h>
#include <h12pro_controller_node/RobotActionState.h>
#include <h12pro_controller_node/UpdateH12CustomizeConfig.h>
#include <h12pro_controller_node/changeAMBACCtrlMode.h>
#include <h12pro_controller_node/changeArmCtrlMode.h>
#include <h12pro_controller_node/changeHandArmPosesByConfigName.h>
#include <h12pro_controller_node/h12proRemoteControllerChannel.h>
#include <h12pro_controller_node/playmusic.h>
#include <h12pro_controller_node/robotPhase.h>
#include <h12pro_controller_node/robotQVTau.h>
#include <h12pro_controller_node/robotTorsoState.h>
#include <h12pro_controller_node/srvChangePhases.h>
#include <h12pro_controller_node/srvClearPositionCMD.h>
#include <h12pro_controller_node/srvchangeCtlMode.h>
#include <h12pro_controller_node/walkCommand.h>
#include <handcontrollerdemorosnode/armPoseWithTimeStamp.h>
#include <handcontrollerdemorosnode/changeHandTrackingMode.h>
#include <handcontrollerdemorosnode/controlEndHand.h>
#include <handcontrollerdemorosnode/handRotationEular.h>
#include <handcontrollerdemorosnode/robotArmPose.h>
#include <handcontrollerdemorosnode/robotHandPosition.h>
#include <handcontrollerdemorosnode/srvArmIK.h>
#include <hardware_node/horizontalMotionByStep.h>
#include <hardware_node/robotHeadMotionData.h>
#include <humanoid_arm_control/armControl.h>
#include <humanoid_arm_control/handUpAndGrab.h>
#include <humanoid_plan_arm_trajectory/ExecuteArmAction.h>
#include <humanoid_plan_arm_trajectory/RobotActionState.h>
#include <humanoid_plan_arm_trajectory/bezierCurveCubicPoint.h>
#include <humanoid_plan_arm_trajectory/jointBezierTrajectory.h>
#include <humanoid_plan_arm_trajectory/planArmState.h>
#include <humanoid_plan_arm_trajectory/planArmTrajectoryBezierCurve.h>
#include <humanoid_plan_arm_trajectory/planArmTrajectoryCubicSpline.h>
#include <kindr_msgs/VectorAtPosition.h>
#include <kuavo_audio_player/audio_status.h>
#include <kuavo_audio_player/playmusic.h>
#include <kuavo_mapping/DeleteMap.h>
#include <kuavo_mapping/EditMap.h>
#include <kuavo_mapping/GetAllMaps.h>
#include <kuavo_mapping/GetCurrentMap.h>
#include <kuavo_mapping/InitialPoseWithTaskPoint.h>
#include <kuavo_mapping/LoadMap.h>
#include <kuavo_mapping/NavigateToTaskPoint.h>
#include <kuavo_mapping/RenameMap.h>
#include <kuavo_mapping/SaveMap.h>
#include <kuavo_mapping/SetInitialPose.h>
#include <kuavo_mapping/StartMapping.h>
#include <kuavo_mapping/StopMapping.h>
#include <kuavo_mapping/TaskPoint.h>
#include <kuavo_mapping/TaskPointOperation.h>
#include <kuavo_msgs/AprilTagDetection.h>
#include <kuavo_msgs/AprilTagDetectionArray.h>
#include <kuavo_msgs/AudioPlaybackStatus.h>
#include <kuavo_msgs/AudioReceiverData.h>
#include <kuavo_msgs/BatteryInfo.h>
#include <kuavo_msgs/Color.h>
#include <kuavo_msgs/ControllerSwitchEvent.h>
#include <kuavo_msgs/CreatePath.h>
#include <kuavo_msgs/DanceTrajectoryState.h>
#include <kuavo_msgs/ExecuteArmAction.h>
#include <kuavo_msgs/FTsensorData.h>
#include <kuavo_msgs/FaceBoundingBox.h>
#include <kuavo_msgs/FallStandCommand.h>
#include <kuavo_msgs/Float32MultiArrayStamped.h>
#include <kuavo_msgs/GetAllMaps.h>
#include <kuavo_msgs/GetBatteryInfo.h>
#include <kuavo_msgs/GetCurrentMap.h>
#include <kuavo_msgs/GetPowerBoardStatus.h>
#include <kuavo_msgs/GetStringList.h>
#include <kuavo_msgs/GetTargetPartPoseInCamera.h>
#include <kuavo_msgs/InitialPoseWithTaskPoint.h>
#include <kuavo_msgs/JoySticks.h>
#include <kuavo_msgs/LoadMap.h>
#include <kuavo_msgs/Metadata.h>
#include <kuavo_msgs/MmDetectionMsg.h>
#include <kuavo_msgs/MocapPoseRetarget.h>
#include <kuavo_msgs/NavigateToTaskPoint.h>
#include <kuavo_msgs/PlayMusicImmediate.h>
#include <kuavo_msgs/PowerBoardStatus.h>
#include <kuavo_msgs/RelaySelectChannel.h>
#include <kuavo_msgs/RepublishTFs.h>
#include <kuavo_msgs/RigidBodyDescription.h>
#include <kuavo_msgs/RigidBodyFrame.h>
#include <kuavo_msgs/RobotActionState.h>
#include <kuavo_msgs/SetHeadControlMode.h>
#include <kuavo_msgs/SetIncrementalArmTrajLink.h>
#include <kuavo_msgs/SetInitialPose.h>
#include <kuavo_msgs/SetJoyTopic.h>
#include <kuavo_msgs/SetLEDMode.h>
#include <kuavo_msgs/SetLEDMode_free.h>
#include <kuavo_msgs/SetString.h>
#include <kuavo_msgs/SkeletonFrame.h>
#include <kuavo_msgs/SpeechSynthesis.h>
#include <kuavo_msgs/TFArray.h>
#include <kuavo_msgs/TaskPoint.h>
#include <kuavo_msgs/TaskPointOperation.h>
#include <kuavo_msgs/TransportModeCommand.h>
#include <kuavo_msgs/VMPTrajectoryState.h>
#include <kuavo_msgs/accessIkSolve.h>
#include <kuavo_msgs/adjustZeroPoint.h>
#include <kuavo_msgs/armCollisionCheckInfo.h>
#include <kuavo_msgs/armHandPose.h>
#include <kuavo_msgs/armHandPoseFree.h>
#include <kuavo_msgs/armPoseWithTimeStamp.h>
#include <kuavo_msgs/armTargetPoses.h>
#include <kuavo_msgs/bezierCurveCubicPoint.h>
#include <kuavo_msgs/changeArmCtrlMode.h>
#include <kuavo_msgs/changeArmCtrlModeKuavo.h>
#include <kuavo_msgs/changeLbMpcObsUpdateModeSrv.h>
#include <kuavo_msgs/changeLbQuickModeSrv.h>
#include <kuavo_msgs/changeMotorParam.h>
#include <kuavo_msgs/changeTorsoCtrlMode.h>
#include <kuavo_msgs/changeWaistCtrlMode.h>
#include <kuavo_msgs/chassisPoseCmdSrv.h>
#include <kuavo_msgs/controlLejuClaw.h>
#include <kuavo_msgs/dexhandCommand.h>
#include <kuavo_msgs/dexhandTouchState.h>
#include <kuavo_msgs/eePoseReachError.h>
#include <kuavo_msgs/enableHandTouchSensor.h>
#include <kuavo_msgs/endEffectorData.h>
#include <kuavo_msgs/fkSrv.h>
#include <kuavo_msgs/fkSrvWithReferFrame.h>
#include <kuavo_msgs/footPose.h>
#include <kuavo_msgs/footPose6D.h>
#include <kuavo_msgs/footPose6DTargetTrajectories.h>
#include <kuavo_msgs/footPose6DTargetTrajectoriesSrv.h>
#include <kuavo_msgs/footPoseTargetTrajectories.h>
#include <kuavo_msgs/footPoseTargetTrajectoriesSrv.h>
#include <kuavo_msgs/footPoseWithVision.h>
#include <kuavo_msgs/footPoseWithVisionArray.h>
#include <kuavo_msgs/footPoses.h>
#include <kuavo_msgs/footPoses6D.h>
#include <kuavo_msgs/fullBodyTargetTrajectories.h>
#include <kuavo_msgs/gaitTimeName.h>
#include <kuavo_msgs/gestureExecute.h>
#include <kuavo_msgs/gestureExecuteState.h>
#include <kuavo_msgs/gestureInfo.h>
#include <kuavo_msgs/gestureList.h>
#include <kuavo_msgs/gestureTask.h>
#include <kuavo_msgs/getControllerList.h>
#include <kuavo_msgs/getCurrentGaitName.h>
#include <kuavo_msgs/getLbTorsoInitialPose.h>
#include <kuavo_msgs/getMotorParam.h>
#include <kuavo_msgs/getMotorZeroPoints.h>
#include <kuavo_msgs/goToZeroPoint.h>
#include <kuavo_msgs/handForceLevel.h>
#include <kuavo_msgs/handPose.h>
#include <kuavo_msgs/headBodyPose.h>
#include <kuavo_msgs/headCtrlMode.h>
#include <kuavo_msgs/ikSolveError.h>
#include <kuavo_msgs/ikSolveParam.h>
#include <kuavo_msgs/imuData.h>
#include <kuavo_msgs/jointBezierTrajectory.h>
#include <kuavo_msgs/jointCmd.h>
#include <kuavo_msgs/jointData.h>
#include <kuavo_msgs/jointMoveTo.h>
#include <kuavo_msgs/kuavoModeSchedule.h>
#include <kuavo_msgs/lbBaseLinkPoseCmdSrv.h>
#include <kuavo_msgs/lbLegControlSrv.h>
#include <kuavo_msgs/lbMultiTimedOfflineTraj.h>
#include <kuavo_msgs/lbMultiTimedPosCmd.h>
#include <kuavo_msgs/lbTimedOfflineTraj.h>
#include <kuavo_msgs/lbTimedPosCmd.h>
#include <kuavo_msgs/lejuClawCommand.h>
#include <kuavo_msgs/lejuClawRecovery.h>
#include <kuavo_msgs/lejuClawState.h>
#include <kuavo_msgs/lejuClawTarget.h>
#include <kuavo_msgs/motorParam.h>
#include <kuavo_msgs/mpc_input.h>
#include <kuavo_msgs/mpc_state.h>
#include <kuavo_msgs/mpc_target_trajectories.h>
#include <kuavo_msgs/picoPoseInfo.h>
#include <kuavo_msgs/picoPoseInfoList.h>
#include <kuavo_msgs/picoPoseRetarget.h>
#include <kuavo_msgs/planArmState.h>
#include <kuavo_msgs/planArmTrajectoryBezierCurve.h>
#include <kuavo_msgs/planArmTrajectoryCubicSpline.h>
#include <kuavo_msgs/playmusic.h>
#include <kuavo_msgs/qv.h>
#include <kuavo_msgs/recordArmHandPose.h>
#include <kuavo_msgs/recordmusic.h>
#include <kuavo_msgs/robotArmQVVD.h>
#include <kuavo_msgs/robotBodyMatrices.h>
#include <kuavo_msgs/robotHandPosition.h>
#include <kuavo_msgs/robotHeadMotionData.h>
#include <kuavo_msgs/robotState.h>
#include <kuavo_msgs/robotSwitchPose.h>
#include <kuavo_msgs/robotWaistControl.h>
#include <kuavo_msgs/sensorsData.h>
#include <kuavo_msgs/setContactForceInterpParams.h>
#include <kuavo_msgs/setHwIntialState.h>
#include <kuavo_msgs/setMmCtrlFrame.h>
#include <kuavo_msgs/setMotorEncoderRoundService.h>
#include <kuavo_msgs/setRuckigPlannerParams.h>
#include <kuavo_msgs/setTagId.h>
#include <kuavo_msgs/singleStepControl.h>
#include <kuavo_msgs/stairAlignmentSrv.h>
#include <kuavo_msgs/switchController.h>
#include <kuavo_msgs/switchGaitByName.h>
#include <kuavo_msgs/switchToNextController.h>
#include <kuavo_msgs/tagDataArray.h>
#include <kuavo_msgs/timedPoint.h>
#include <kuavo_msgs/timedSingleCmd.h>
#include <kuavo_msgs/touchSensorStatus.h>
#include <kuavo_msgs/twoArmHandPose.h>
#include <kuavo_msgs/twoArmHandPoseCmd.h>
#include <kuavo_msgs/twoArmHandPoseCmdFree.h>
#include <kuavo_msgs/twoArmHandPoseCmdFreeSrv.h>
#include <kuavo_msgs/twoArmHandPoseCmdSrv.h>
#include <kuavo_msgs/twoArmHandPoseFree.h>
#include <kuavo_msgs/xsensePoseInfo.h>
#include <kuavo_msgs/xsensePoseInfoList.h>
#include <kuavo_msgs/xsensePoseRetarget.h>
#include <kuavo_msgs/yoloDetection.h>
#include <kuavo_msgs/yoloOutputData.h>
#include <kuavo_pico_gmr/AmpJoyRoute.h>
#include <kuavo_ros_interfaces/armTargetPoses.h>
#include <kuavo_ros_interfaces/bezierCurveCubicPoint.h>
#include <kuavo_ros_interfaces/jointBezierTrajectory.h>
#include <kuavo_ros_interfaces/ocs2ChangeArmCtrlMode.h>
#include <kuavo_ros_interfaces/planArmState.h>
#include <kuavo_ros_interfaces/planArmTrajectoryBezierCurve.h>
#include <kuavo_ros_interfaces/robotHandPosition.h>
#include <kuavo_ros_interfaces/robotHeadMotionData.h>
#include <kuavo_ros_interfaces/stopPlanArmTrajectory.h>
#include <leju_mobile_base_msgs/BMS.h>
#include <leju_mobile_base_msgs/BaseCmdVelStatus.h>
#include <leju_mobile_base_msgs/BaseMove.h>
#include <leju_mobile_base_msgs/Battery.h>
#include <leju_mobile_base_msgs/CheckArrived.h>
#include <leju_mobile_base_msgs/ControlLaserScan.h>
#include <leju_mobile_base_msgs/DispatchMove.h>
#include <leju_mobile_base_msgs/ExecTask.h>
#include <leju_mobile_base_msgs/GetGlobalOdometer.h>
#include <leju_mobile_base_msgs/GetLaserScanList.h>
#include <leju_mobile_base_msgs/HardwareStatus.h>
#include <leju_mobile_base_msgs/Information.h>
#include <leju_mobile_base_msgs/LED.h>
#include <leju_mobile_base_msgs/LiftControl3Feedback.h>
#include <leju_mobile_base_msgs/LiftControl3Goal.h>
#include <leju_mobile_base_msgs/LiftControl3Result.h>
#include <leju_mobile_base_msgs/LiftStatus.h>
#include <leju_mobile_base_msgs/MoveBaseActionGoal.h>
#include <leju_mobile_base_msgs/MoveToTarget.h>
#include <leju_mobile_base_msgs/MoveToTargetOptions.h>
#include <leju_mobile_base_msgs/NavigationModeGoal.h>
#include <leju_mobile_base_msgs/Parameter.h>
#include <leju_mobile_base_msgs/ResetTemporaryOdometer.h>
#include <leju_mobile_base_msgs/RobotStatus.h>
#include <leju_mobile_base_msgs/SetDispatchMode.h>
#include <leju_mobile_base_msgs/SetMapServer.h>
#include <leju_mobile_base_msgs/StartLocalization.h>
#include <leju_mobile_base_msgs/StepControlGoal.h>
#include <leju_mobile_base_msgs/SyncCostmap.h>
#include <leju_mobile_base_msgs/TaskDetailStatus.h>
#include <motioncapture/ForcePlateData.h>
#include <nav_msgs/Path.h>
#include <network_monitor/NetworkPing.h>
#include <network_monitor/NetworkPingMsg.h>
#include <nimservos_controller/motor_cmd.h>
#include <nimservos_controller/motor_state.h>
#include <nlink_parser/IotFrame0.h>
#include <nlink_parser/IotFrame0Node.h>
#include <nlink_parser/LinktrackAnchorframe0.h>
#include <nlink_parser/LinktrackAoaNode0.h>
#include <nlink_parser/LinktrackAoaNodeframe0.h>
#include <nlink_parser/LinktrackNode0.h>
#include <nlink_parser/LinktrackNode1.h>
#include <nlink_parser/LinktrackNode2.h>
#include <nlink_parser/LinktrackNode4Anchor.h>
#include <nlink_parser/LinktrackNode4Tag.h>
#include <nlink_parser/LinktrackNode5.h>
#include <nlink_parser/LinktrackNode6.h>
#include <nlink_parser/LinktrackNodeframe0.h>
#include <nlink_parser/LinktrackNodeframe1.h>
#include <nlink_parser/LinktrackNodeframe2.h>
#include <nlink_parser/LinktrackNodeframe3.h>
#include <nlink_parser/LinktrackNodeframe4.h>
#include <nlink_parser/LinktrackNodeframe5.h>
#include <nlink_parser/LinktrackNodeframe6.h>
#include <nlink_parser/LinktrackTag.h>
#include <nlink_parser/LinktrackTagframe0.h>
#include <nlink_parser/TofsenseCascade.h>
#include <nlink_parser/TofsenseFrame0.h>
#include <nlink_parser/TofsenseMFrame0.h>
#include <nlink_parser/TofsenseMFrame0Pixel.h>
#include <noitom_hi5_hand_udp_python/JoySticks.h>
#include <noitom_hi5_hand_udp_python/PoseInfo.h>
#include <noitom_hi5_hand_udp_python/PoseInfoList.h>
#include <noitom_hi5_hand_udp_python/QuaternionArray.h>
#include <noitom_hi5_hand_udp_python/Vector4.h>
#include <noitom_hi5_hand_udp_python/handRotation.h>
#include <noitom_hi5_hand_udp_python/handRotationEular.h>
#include <ocs2_msgs/constraint.h>
#include <ocs2_msgs/controller_data.h>
#include <ocs2_msgs/lagrangian_metrics.h>
#include <ocs2_msgs/mode_schedule.h>
#include <ocs2_msgs/mpc_flattened_controller.h>
#include <ocs2_msgs/mpc_input.h>
#include <ocs2_msgs/mpc_observation.h>
#include <ocs2_msgs/mpc_performance_indices.h>
#include <ocs2_msgs/mpc_solver_data.h>
#include <ocs2_msgs/mpc_state.h>
#include <ocs2_msgs/mpc_target_trajectories.h>
#include <ocs2_msgs/multiplier.h>
#include <ocs2_msgs/pause_resume.h>
#include <ocs2_msgs/reset.h>
#include <ocs2_switched_model_msgs/gait.h>
#include <ocs2_switched_model_msgs/gait_sequence.h>
#include <ocs2_switched_model_msgs/scheduled_gait_sequence.h>
#include <ocs2_switched_model_msgs/trajectory_request.h>
#include <robot_calibration_msgs/CalibrationData.h>
#include <robot_calibration_msgs/CameraParameter.h>
#include <robot_calibration_msgs/CaptureConfig.h>
#include <robot_calibration_msgs/ExtendedCameraInfo.h>
#include <robot_calibration_msgs/GripperLedCommandAction.h>
#include <robot_calibration_msgs/Observation.h>
#include <robot_localization/FromLL.h>
#include <robot_localization/GetState.h>
#include <robot_localization/SetDatum.h>
#include <robot_localization/SetPose.h>
#include <robot_localization/SetUTMZone.h>
#include <robot_localization/ToLL.h>
#include <robot_localization/ToggleFilterProcessing.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/JointState.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/PointField.h>
#include <sensor_msgs/RegionOfInterest.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Header.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/MultiArrayLayout.h>
#include <taiji_trigger_node/PerformTaiji.h>
#include <trajectory_msgs/JointTrajectory.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>
#include <user_pkg/piddebug.h>
#include <user_pkg/uwbdebug.h>
#include <wiimote/IrSourceInfo.h>
#include <wiimote/State.h>
#include <wiimote/TimedSwitch.h>
#include <ros/message_traits.h>
#include <ros/service_traits.h>
#include "native_constants.hpp"
namespace
{
    int output() {return STDOUT_FILENO;}
    /** Emit bounded JSON text with checked Unicode metadata supplied by ROS.
     * Escape ASCII controls and quotes; no arbitrary filename or code is loaded. */
    void quoted(std::string_view value)
    {
        if(value.size()>1048576) {throw std::length_error("native metadata ceiling");}
        size_t length{2};for(unsigned char c:value) {length+=c<32?6:c=='"'||c=='\\'?2:1;}std::string escaped;escaped.reserve(length);escaped.push_back('"');constexpr char digits[]{"0123456789abcdef"};
        for(unsigned char c:value)
        {
            if(c=='"'||c=='\\') {escaped.push_back('\\');escaped.push_back(static_cast<char>(c));}
            else if(c<32) {for(char prefix:std::string_view{"\\u00"}) {escaped.push_back(prefix);}escaped.push_back(digits[c>>4]);escaped.push_back(digits[c&15]);}
            else {escaped.push_back(static_cast<char>(c));}
        }
        escaped.push_back('"');bounded_native_io::emit(output(),escaped);
    }
#if UWVM_CUSTOM_NATIVE_ROS_VERSION == 1
    template<class Message> void component(char const* role,bool comma)
    {
        if(comma) {bounded_native_io::print(output(),',');}quoted(role);bounded_native_io::print(output(),":{");
        bounded_native_io::print(output(),"\"type\":");quoted(::ros::message_traits::DataType<Message>::value());
        bounded_native_io::print(output(),",\"native_identity\":");quoted(::ros::message_traits::MD5Sum<Message>::value());
        bounded_native_io::print(output(),",\"definition\":");quoted(::ros::message_traits::Definition<Message>::value());bounded_native_io::print(output(),'}');
    }
    template<class Message> void message(char const* name,bool comma)
    {if(comma) {bounded_native_io::print(output(),',');}quoted(name);bounded_native_io::print(output(),":{\"native_identity\":");quoted(::ros::message_traits::MD5Sum<Message>::value());bounded_native_io::print(output(),",\"components\":{");component<Message>("message",false);bounded_native_io::print(output(),"}}");}
    template<class Service> void service(char const* name)
    {bounded_native_io::print(output(),',');quoted(name);bounded_native_io::print(output(),":{\"native_identity\":");quoted(::ros::service_traits::MD5Sum<Service>::value());bounded_native_io::print(output(),",\"official_type\":");quoted(::ros::service_traits::DataType<Service>::value());bounded_native_io::print(output(),",\"components\":{");component<typename Service::Request>("request",false);component<typename Service::Response>("response",true);bounded_native_io::print(output(),"}}");}
#else
    /** Official static borrowed description strings may report capacity equal
     * to size, excluding the separately emitted NUL. Check that actual NUL,
     * pointer and finite length; this is not the Guest string admission rule. */
    template<class Text> std::string_view text(Text const& value)
    {if(value.size>1048576||value.size>value.capacity||!value.data||value.data[value.size]!=0) {throw std::runtime_error("invalid official metadata text");}return {value.data,value.size};}
    void record(rosidl_runtime_c__type_description__IndividualTypeDescription const& value)
    {
        if(value.fields.size>4096||value.fields.size>value.fields.capacity||(value.fields.size&&!value.fields.data)) {throw std::runtime_error("native field table ceiling");}
        bounded_native_io::print(output(),"{\"type\":");quoted(text(value.type_name));bounded_native_io::print(output(),",\"fields\":[");
        for(size_t i{};i!=value.fields.size;++i)
        {auto const& f{value.fields.data[i]};if(i) {bounded_native_io::print(output(),',');}bounded_native_io::print(output(),"{\"name\":");quoted(text(f.name));bounded_native_io::print(output(),",\"type_id\":",static_cast<unsigned>(f.type.type_id),",\"capacity\":",f.type.capacity,",\"string_capacity\":",f.type.string_capacity,",\"nested_type_name\":");
            if(!f.type.nested_type_name.data&&!f.type.nested_type_name.size&&!f.type.nested_type_name.capacity) {quoted("");}else {quoted(text(f.type.nested_type_name));}
            bounded_native_io::print(output(),",\"default_value\":");if(!f.default_value.data&&!f.default_value.size&&!f.default_value.capacity) {quoted("");}else {quoted(text(f.default_value));}bounded_native_io::print(output(),'}');}
        bounded_native_io::print(output(),"]}");
    }
    template<class Support> void metadata(Support const* support)
    {
        if(!support||!support->get_type_hash_func||!support->get_type_description_func) {throw std::runtime_error("official metadata unavailable");}
        auto hash{support->get_type_hash_func(support)};auto description{support->get_type_description_func(support)};
        if(!hash||hash->version!=1||!description||description->referenced_type_descriptions.size>512||description->referenced_type_descriptions.size>description->referenced_type_descriptions.capacity||(description->referenced_type_descriptions.size&&!description->referenced_type_descriptions.data)) {throw std::runtime_error("official type description ceiling");}
        std::array<char,71> identity{};constexpr char hex[]{"0123456789abcdef"};constexpr char prefix[]{"RIHS01_"};for(unsigned i{};i!=7;++i) {identity[i]=prefix[i];}for(unsigned i{};i!=32;++i) {identity[7+2*i]=hex[hash->value[i]>>4];identity[8+2*i]=hex[hash->value[i]&15];}
        bounded_native_io::print(output(),"{\"native_identity\":");quoted({identity.data(),identity.size()});bounded_native_io::print(output(),",\"root\":");record(description->type_description);bounded_native_io::print(output(),",\"references\":[");for(size_t i{};i!=description->referenced_type_descriptions.size;++i) {if(i) {bounded_native_io::print(output(),',');}record(description->referenced_type_descriptions.data[i]);}bounded_native_io::print(output(),"]}");
    }
    template<class Message> void component(char const* name,bool comma)
    {if(comma) {bounded_native_io::print(output(),',');}quoted(name);bounded_native_io::print(output(),':');metadata(rosidl_typesupport_cpp::get_message_type_support_handle<Message>());}
    template<class Message> void message(char const* name,bool comma)
    {if(comma) {bounded_native_io::print(output(),',');}quoted(name);bounded_native_io::print(output(),":{\"official\":");metadata(rosidl_typesupport_cpp::get_message_type_support_handle<Message>());bounded_native_io::print(output(),",\"components\":{");component<Message>("message",false);bounded_native_io::print(output(),"}}");}
    template<class Service> void service(char const* name)
    {bounded_native_io::print(output(),',');quoted(name);bounded_native_io::print(output(),":{\"official\":");metadata(rosidl_typesupport_cpp::get_service_type_support_handle<Service>());bounded_native_io::print(output(),",\"components\":{");component<typename Service::Request>("request",false);component<typename Service::Response>("response",true);bounded_native_io::print(output(),"}}");}
#endif
}
int main() {try {
if(!::uwvm_industrial_source_3292f4471f70cf79b1b8440b90bb4d6b5797eaccd8e86d6cf61d57eefcc26848::validate())throw std::runtime_error{"native source constants differ"};
if(::raise(SIGSTOP)!=0)throw std::runtime_error{"owned mapping stop failed"};
bounded_native_io::gate('I');
bounded_native_io::print(output(),"{\"format\":\"uwvm-ros-industrial-native-v1\",\"ros_version\":",UWVM_CUSTOM_NATIVE_ROS_VERSION,",\"source_profile\":\"uwvm-ros-vendor-complete-bounded-v1\",\"constant_schema_sha256\":\"3292f4471f70cf79b1b8440b90bb4d6b5797eaccd8e86d6cf61d57eefcc26848\",\"types\":{");
quoted("aruco_msgs/Marker");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::aruco_msgs::Marker>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::aruco_msgs::Marker>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("aruco_msgs/MarkerArray");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::aruco_msgs::MarkerArray>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::aruco_msgs::MarkerArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("easy_handeye_msgs/CheckStartingPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::easy_handeye_msgs::CheckStartingPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::easy_handeye_msgs::CheckStartingPose::Request>("request",false);
component<::easy_handeye_msgs::CheckStartingPose::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("easy_handeye_msgs/ComputeCalibration");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::easy_handeye_msgs::ComputeCalibration>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::easy_handeye_msgs::ComputeCalibration::Request>("request",false);
component<::easy_handeye_msgs::ComputeCalibration::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("easy_handeye_msgs/EnumerateTargetPoses");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::easy_handeye_msgs::EnumerateTargetPoses>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::easy_handeye_msgs::EnumerateTargetPoses::Request>("request",false);
component<::easy_handeye_msgs::EnumerateTargetPoses::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("easy_handeye_msgs/ExecutePlan");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::easy_handeye_msgs::ExecutePlan>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::easy_handeye_msgs::ExecutePlan::Request>("request",false);
component<::easy_handeye_msgs::ExecutePlan::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("easy_handeye_msgs/HandeyeCalibration");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::easy_handeye_msgs::HandeyeCalibration>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::easy_handeye_msgs::HandeyeCalibration>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("easy_handeye_msgs/ListAlgorithms");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::easy_handeye_msgs::ListAlgorithms>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::easy_handeye_msgs::ListAlgorithms::Request>("request",false);
component<::easy_handeye_msgs::ListAlgorithms::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("easy_handeye_msgs/PlanToSelectedTargetPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::easy_handeye_msgs::PlanToSelectedTargetPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::easy_handeye_msgs::PlanToSelectedTargetPose::Request>("request",false);
component<::easy_handeye_msgs::PlanToSelectedTargetPose::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("easy_handeye_msgs/RemoveSample");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::easy_handeye_msgs::RemoveSample>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::easy_handeye_msgs::RemoveSample::Request>("request",false);
component<::easy_handeye_msgs::RemoveSample::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("easy_handeye_msgs/SampleList");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::easy_handeye_msgs::SampleList>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::easy_handeye_msgs::SampleList>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("easy_handeye_msgs/SelectTargetPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::easy_handeye_msgs::SelectTargetPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::easy_handeye_msgs::SelectTargetPose::Request>("request",false);
component<::easy_handeye_msgs::SelectTargetPose::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("easy_handeye_msgs/SetAlgorithm");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::easy_handeye_msgs::SetAlgorithm>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::easy_handeye_msgs::SetAlgorithm::Request>("request",false);
component<::easy_handeye_msgs::SetAlgorithm::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("easy_handeye_msgs/TakeSample");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::easy_handeye_msgs::TakeSample>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::easy_handeye_msgs::TakeSample::Request>("request",false);
component<::easy_handeye_msgs::TakeSample::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("easy_handeye_msgs/TargetPoseList");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::easy_handeye_msgs::TargetPoseList>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::easy_handeye_msgs::TargetPoseList>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geographic_msgs/GeoPoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geographic_msgs::GeoPoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geographic_msgs::GeoPoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geographic_msgs/GeoPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geographic_msgs::GeoPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geographic_msgs::GeoPose>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/Point");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::Point>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::Point>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/PointStamped");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::PointStamped>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::PointStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/Pose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::Pose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::Pose>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/PoseStamped");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::PoseStamped>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::PoseStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/PoseWithCovariance");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::PoseWithCovariance>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::PoseWithCovariance>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/PoseWithCovarianceStamped");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::PoseWithCovarianceStamped>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::PoseWithCovarianceStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/Quaternion");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::Quaternion>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::Quaternion>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/Transform");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::Transform>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::Transform>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/TransformStamped");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::TransformStamped>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::TransformStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/Twist");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::Twist>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::Twist>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("geometry_msgs/Vector3");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::geometry_msgs::Vector3>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::geometry_msgs::Vector3>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("grid_map_msgs/GetGridMap");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::grid_map_msgs::GetGridMap>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::grid_map_msgs::GetGridMap::Request>("request",false);
component<::grid_map_msgs::GetGridMap::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("grid_map_msgs/GetGridMapInfo");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::grid_map_msgs::GetGridMapInfo>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::grid_map_msgs::GetGridMapInfo::Request>("request",false);
component<::grid_map_msgs::GetGridMapInfo::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("grid_map_msgs/GridMap");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::grid_map_msgs::GridMap>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::grid_map_msgs::GridMap>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("grid_map_msgs/GridMapInfo");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::grid_map_msgs::GridMapInfo>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::grid_map_msgs::GridMapInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("grid_map_msgs/ProcessFile");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::grid_map_msgs::ProcessFile>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::grid_map_msgs::ProcessFile::Request>("request",false);
component<::grid_map_msgs::ProcessFile::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("grid_map_msgs/SetGridMap");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::grid_map_msgs::SetGridMap>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::grid_map_msgs::SetGridMap::Request>("request",false);
component<::grid_map_msgs::SetGridMap::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("h12pro_controller_node/ECJointMotordata");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::h12pro_controller_node::ECJointMotordata>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::h12pro_controller_node::ECJointMotordata>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("h12pro_controller_node/ExecuteArmAction");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::h12pro_controller_node::ExecuteArmAction>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::h12pro_controller_node::ExecuteArmAction::Request>("request",false);
component<::h12pro_controller_node::ExecuteArmAction::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("h12pro_controller_node/RobotActionState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::h12pro_controller_node::RobotActionState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::h12pro_controller_node::RobotActionState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("h12pro_controller_node/UpdateH12CustomizeConfig");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::h12pro_controller_node::UpdateH12CustomizeConfig>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::h12pro_controller_node::UpdateH12CustomizeConfig>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("h12pro_controller_node/changeAMBACCtrlMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::h12pro_controller_node::changeAMBACCtrlMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::h12pro_controller_node::changeAMBACCtrlMode::Request>("request",false);
component<::h12pro_controller_node::changeAMBACCtrlMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("h12pro_controller_node/changeArmCtrlMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::h12pro_controller_node::changeArmCtrlMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::h12pro_controller_node::changeArmCtrlMode::Request>("request",false);
component<::h12pro_controller_node::changeArmCtrlMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("h12pro_controller_node/changeHandArmPosesByConfigName");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::h12pro_controller_node::changeHandArmPosesByConfigName>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::h12pro_controller_node::changeHandArmPosesByConfigName::Request>("request",false);
component<::h12pro_controller_node::changeHandArmPosesByConfigName::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("h12pro_controller_node/h12proRemoteControllerChannel");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::h12pro_controller_node::h12proRemoteControllerChannel>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::h12pro_controller_node::h12proRemoteControllerChannel>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("h12pro_controller_node/playmusic");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::h12pro_controller_node::playmusic>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::h12pro_controller_node::playmusic::Request>("request",false);
component<::h12pro_controller_node::playmusic::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("h12pro_controller_node/robotPhase");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::h12pro_controller_node::robotPhase>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::h12pro_controller_node::robotPhase>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("h12pro_controller_node/robotQVTau");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::h12pro_controller_node::robotQVTau>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::h12pro_controller_node::robotQVTau>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("h12pro_controller_node/robotTorsoState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::h12pro_controller_node::robotTorsoState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::h12pro_controller_node::robotTorsoState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("h12pro_controller_node/srvChangePhases");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::h12pro_controller_node::srvChangePhases>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::h12pro_controller_node::srvChangePhases::Request>("request",false);
component<::h12pro_controller_node::srvChangePhases::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("h12pro_controller_node/srvClearPositionCMD");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::h12pro_controller_node::srvClearPositionCMD>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::h12pro_controller_node::srvClearPositionCMD::Request>("request",false);
component<::h12pro_controller_node::srvClearPositionCMD::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("h12pro_controller_node/srvchangeCtlMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::h12pro_controller_node::srvchangeCtlMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::h12pro_controller_node::srvchangeCtlMode::Request>("request",false);
component<::h12pro_controller_node::srvchangeCtlMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("h12pro_controller_node/walkCommand");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::h12pro_controller_node::walkCommand>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::h12pro_controller_node::walkCommand>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("handcontrollerdemorosnode/armPoseWithTimeStamp");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::handcontrollerdemorosnode::armPoseWithTimeStamp>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::handcontrollerdemorosnode::armPoseWithTimeStamp>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("handcontrollerdemorosnode/changeHandTrackingMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::handcontrollerdemorosnode::changeHandTrackingMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::handcontrollerdemorosnode::changeHandTrackingMode::Request>("request",false);
component<::handcontrollerdemorosnode::changeHandTrackingMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("handcontrollerdemorosnode/controlEndHand");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::handcontrollerdemorosnode::controlEndHand>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::handcontrollerdemorosnode::controlEndHand::Request>("request",false);
component<::handcontrollerdemorosnode::controlEndHand::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("handcontrollerdemorosnode/handRotationEular");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::handcontrollerdemorosnode::handRotationEular>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::handcontrollerdemorosnode::handRotationEular>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("handcontrollerdemorosnode/robotArmPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::handcontrollerdemorosnode::robotArmPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::handcontrollerdemorosnode::robotArmPose>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("handcontrollerdemorosnode/robotHandPosition");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::handcontrollerdemorosnode::robotHandPosition>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::handcontrollerdemorosnode::robotHandPosition>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("handcontrollerdemorosnode/srvArmIK");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::handcontrollerdemorosnode::srvArmIK>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::handcontrollerdemorosnode::srvArmIK::Request>("request",false);
component<::handcontrollerdemorosnode::srvArmIK::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("hardware_node/horizontalMotionByStep");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::hardware_node::horizontalMotionByStep>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::hardware_node::horizontalMotionByStep>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("hardware_node/robotHeadMotionData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::hardware_node::robotHeadMotionData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::hardware_node::robotHeadMotionData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("humanoid_arm_control/armControl");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::humanoid_arm_control::armControl>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::humanoid_arm_control::armControl::Request>("request",false);
component<::humanoid_arm_control::armControl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("humanoid_arm_control/handUpAndGrab");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::humanoid_arm_control::handUpAndGrab>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::humanoid_arm_control::handUpAndGrab::Request>("request",false);
component<::humanoid_arm_control::handUpAndGrab::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("humanoid_plan_arm_trajectory/ExecuteArmAction");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::humanoid_plan_arm_trajectory::ExecuteArmAction>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::humanoid_plan_arm_trajectory::ExecuteArmAction::Request>("request",false);
component<::humanoid_plan_arm_trajectory::ExecuteArmAction::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("humanoid_plan_arm_trajectory/RobotActionState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::humanoid_plan_arm_trajectory::RobotActionState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::humanoid_plan_arm_trajectory::RobotActionState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("humanoid_plan_arm_trajectory/bezierCurveCubicPoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::humanoid_plan_arm_trajectory::bezierCurveCubicPoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::humanoid_plan_arm_trajectory::bezierCurveCubicPoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("humanoid_plan_arm_trajectory/jointBezierTrajectory");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::humanoid_plan_arm_trajectory::jointBezierTrajectory>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::humanoid_plan_arm_trajectory::jointBezierTrajectory>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("humanoid_plan_arm_trajectory/planArmState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::humanoid_plan_arm_trajectory::planArmState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::humanoid_plan_arm_trajectory::planArmState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("humanoid_plan_arm_trajectory/planArmTrajectoryBezierCurve");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::humanoid_plan_arm_trajectory::planArmTrajectoryBezierCurve>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::humanoid_plan_arm_trajectory::planArmTrajectoryBezierCurve::Request>("request",false);
component<::humanoid_plan_arm_trajectory::planArmTrajectoryBezierCurve::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("humanoid_plan_arm_trajectory/planArmTrajectoryCubicSpline");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::humanoid_plan_arm_trajectory::planArmTrajectoryCubicSpline>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::humanoid_plan_arm_trajectory::planArmTrajectoryCubicSpline::Request>("request",false);
component<::humanoid_plan_arm_trajectory::planArmTrajectoryCubicSpline::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kindr_msgs/VectorAtPosition");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kindr_msgs::VectorAtPosition>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kindr_msgs::VectorAtPosition>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_audio_player/audio_status");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_audio_player::audio_status>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_audio_player::audio_status::Request>("request",false);
component<::kuavo_audio_player::audio_status::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_audio_player/playmusic");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_audio_player::playmusic>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_audio_player::playmusic::Request>("request",false);
component<::kuavo_audio_player::playmusic::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_mapping/DeleteMap");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_mapping::DeleteMap>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_mapping::DeleteMap::Request>("request",false);
component<::kuavo_mapping::DeleteMap::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_mapping/EditMap");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_mapping::EditMap>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_mapping::EditMap::Request>("request",false);
component<::kuavo_mapping::EditMap::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_mapping/GetAllMaps");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_mapping::GetAllMaps>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_mapping::GetAllMaps::Request>("request",false);
component<::kuavo_mapping::GetAllMaps::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_mapping/GetCurrentMap");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_mapping::GetCurrentMap>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_mapping::GetCurrentMap::Request>("request",false);
component<::kuavo_mapping::GetCurrentMap::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_mapping/InitialPoseWithTaskPoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_mapping::InitialPoseWithTaskPoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_mapping::InitialPoseWithTaskPoint::Request>("request",false);
component<::kuavo_mapping::InitialPoseWithTaskPoint::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_mapping/LoadMap");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_mapping::LoadMap>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_mapping::LoadMap::Request>("request",false);
component<::kuavo_mapping::LoadMap::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_mapping/NavigateToTaskPoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_mapping::NavigateToTaskPoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_mapping::NavigateToTaskPoint::Request>("request",false);
component<::kuavo_mapping::NavigateToTaskPoint::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_mapping/RenameMap");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_mapping::RenameMap>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_mapping::RenameMap::Request>("request",false);
component<::kuavo_mapping::RenameMap::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_mapping/SaveMap");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_mapping::SaveMap>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_mapping::SaveMap::Request>("request",false);
component<::kuavo_mapping::SaveMap::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_mapping/SetInitialPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_mapping::SetInitialPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_mapping::SetInitialPose::Request>("request",false);
component<::kuavo_mapping::SetInitialPose::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_mapping/StartMapping");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_mapping::StartMapping>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_mapping::StartMapping::Request>("request",false);
component<::kuavo_mapping::StartMapping::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_mapping/StopMapping");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_mapping::StopMapping>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_mapping::StopMapping::Request>("request",false);
component<::kuavo_mapping::StopMapping::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_mapping/TaskPoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_mapping::TaskPoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_mapping::TaskPoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_mapping/TaskPointOperation");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_mapping::TaskPointOperation>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_mapping::TaskPointOperation::Request>("request",false);
component<::kuavo_mapping::TaskPointOperation::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/AprilTagDetection");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::AprilTagDetection>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::AprilTagDetection>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/AprilTagDetectionArray");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::AprilTagDetectionArray>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::AprilTagDetectionArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/AudioPlaybackStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::AudioPlaybackStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::AudioPlaybackStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/AudioReceiverData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::AudioReceiverData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::AudioReceiverData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/BatteryInfo");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::BatteryInfo>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::BatteryInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/Color");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::Color>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::Color>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/ControllerSwitchEvent");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::ControllerSwitchEvent>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::ControllerSwitchEvent>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/CreatePath");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::CreatePath>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::CreatePath::Request>("request",false);
component<::kuavo_msgs::CreatePath::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/DanceTrajectoryState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::DanceTrajectoryState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::DanceTrajectoryState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/ExecuteArmAction");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::ExecuteArmAction>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::ExecuteArmAction::Request>("request",false);
component<::kuavo_msgs::ExecuteArmAction::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/FTsensorData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::FTsensorData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::FTsensorData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/FaceBoundingBox");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::FaceBoundingBox>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::FaceBoundingBox>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/FallStandCommand");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::FallStandCommand>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::FallStandCommand::Request>("request",false);
component<::kuavo_msgs::FallStandCommand::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/Float32MultiArrayStamped");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::Float32MultiArrayStamped>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::Float32MultiArrayStamped>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/GetAllMaps");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::GetAllMaps>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::GetAllMaps::Request>("request",false);
component<::kuavo_msgs::GetAllMaps::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/GetBatteryInfo");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::GetBatteryInfo>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::GetBatteryInfo::Request>("request",false);
component<::kuavo_msgs::GetBatteryInfo::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/GetCurrentMap");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::GetCurrentMap>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::GetCurrentMap::Request>("request",false);
component<::kuavo_msgs::GetCurrentMap::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/GetPowerBoardStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::GetPowerBoardStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::GetPowerBoardStatus::Request>("request",false);
component<::kuavo_msgs::GetPowerBoardStatus::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/GetStringList");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::GetStringList>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::GetStringList::Request>("request",false);
component<::kuavo_msgs::GetStringList::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/GetTargetPartPoseInCamera");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::GetTargetPartPoseInCamera>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::GetTargetPartPoseInCamera::Request>("request",false);
component<::kuavo_msgs::GetTargetPartPoseInCamera::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/InitialPoseWithTaskPoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::InitialPoseWithTaskPoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::InitialPoseWithTaskPoint::Request>("request",false);
component<::kuavo_msgs::InitialPoseWithTaskPoint::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/JoySticks");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::JoySticks>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::JoySticks>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/LoadMap");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::LoadMap>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::LoadMap::Request>("request",false);
component<::kuavo_msgs::LoadMap::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/Metadata");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::Metadata>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::Metadata>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/MmDetectionMsg");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::MmDetectionMsg>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::MmDetectionMsg>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/MocapPoseRetarget");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::MocapPoseRetarget>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::MocapPoseRetarget>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/NavigateToTaskPoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::NavigateToTaskPoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::NavigateToTaskPoint::Request>("request",false);
component<::kuavo_msgs::NavigateToTaskPoint::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/PlayMusicImmediate");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::PlayMusicImmediate>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::PlayMusicImmediate::Request>("request",false);
component<::kuavo_msgs::PlayMusicImmediate::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/PowerBoardStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::PowerBoardStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::PowerBoardStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/RelaySelectChannel");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::RelaySelectChannel>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::RelaySelectChannel::Request>("request",false);
component<::kuavo_msgs::RelaySelectChannel::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/RepublishTFs");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::RepublishTFs>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::RepublishTFs::Request>("request",false);
component<::kuavo_msgs::RepublishTFs::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/RigidBodyDescription");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::RigidBodyDescription>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::RigidBodyDescription>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/RigidBodyFrame");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::RigidBodyFrame>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::RigidBodyFrame>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/RobotActionState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::RobotActionState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::RobotActionState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/SetHeadControlMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::SetHeadControlMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::SetHeadControlMode::Request>("request",false);
component<::kuavo_msgs::SetHeadControlMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/SetIncrementalArmTrajLink");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::SetIncrementalArmTrajLink>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::SetIncrementalArmTrajLink::Request>("request",false);
component<::kuavo_msgs::SetIncrementalArmTrajLink::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/SetInitialPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::SetInitialPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::SetInitialPose::Request>("request",false);
component<::kuavo_msgs::SetInitialPose::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/SetJoyTopic");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::SetJoyTopic>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::SetJoyTopic::Request>("request",false);
component<::kuavo_msgs::SetJoyTopic::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/SetLEDMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::SetLEDMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::SetLEDMode::Request>("request",false);
component<::kuavo_msgs::SetLEDMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/SetLEDMode_free");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::SetLEDMode_free>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::SetLEDMode_free::Request>("request",false);
component<::kuavo_msgs::SetLEDMode_free::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/SetString");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::SetString>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::SetString::Request>("request",false);
component<::kuavo_msgs::SetString::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/SkeletonFrame");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::SkeletonFrame>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::SkeletonFrame>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/SpeechSynthesis");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::SpeechSynthesis>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::SpeechSynthesis::Request>("request",false);
component<::kuavo_msgs::SpeechSynthesis::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/TFArray");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::TFArray>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::TFArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/TaskPoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::TaskPoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::TaskPoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/TaskPointOperation");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::TaskPointOperation>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::TaskPointOperation::Request>("request",false);
component<::kuavo_msgs::TaskPointOperation::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/TransportModeCommand");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::TransportModeCommand>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::TransportModeCommand::Request>("request",false);
component<::kuavo_msgs::TransportModeCommand::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/VMPTrajectoryState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::VMPTrajectoryState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::VMPTrajectoryState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/accessIkSolve");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::accessIkSolve>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::accessIkSolve::Request>("request",false);
component<::kuavo_msgs::accessIkSolve::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/adjustZeroPoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::adjustZeroPoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::adjustZeroPoint::Request>("request",false);
component<::kuavo_msgs::adjustZeroPoint::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/armCollisionCheckInfo");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::armCollisionCheckInfo>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::armCollisionCheckInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/armHandPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::armHandPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::armHandPose>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/armHandPoseFree");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::armHandPoseFree>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::armHandPoseFree>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/armPoseWithTimeStamp");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::armPoseWithTimeStamp>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::armPoseWithTimeStamp>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/armTargetPoses");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::armTargetPoses>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::armTargetPoses>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/bezierCurveCubicPoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::bezierCurveCubicPoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::bezierCurveCubicPoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/changeArmCtrlMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::changeArmCtrlMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::changeArmCtrlMode::Request>("request",false);
component<::kuavo_msgs::changeArmCtrlMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/changeArmCtrlModeKuavo");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::changeArmCtrlModeKuavo>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::changeArmCtrlModeKuavo::Request>("request",false);
component<::kuavo_msgs::changeArmCtrlModeKuavo::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/changeLbMpcObsUpdateModeSrv");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::changeLbMpcObsUpdateModeSrv>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::changeLbMpcObsUpdateModeSrv::Request>("request",false);
component<::kuavo_msgs::changeLbMpcObsUpdateModeSrv::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/changeLbQuickModeSrv");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::changeLbQuickModeSrv>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::changeLbQuickModeSrv::Request>("request",false);
component<::kuavo_msgs::changeLbQuickModeSrv::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/changeMotorParam");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::changeMotorParam>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::changeMotorParam::Request>("request",false);
component<::kuavo_msgs::changeMotorParam::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/changeTorsoCtrlMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::changeTorsoCtrlMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::changeTorsoCtrlMode::Request>("request",false);
component<::kuavo_msgs::changeTorsoCtrlMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/changeWaistCtrlMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::changeWaistCtrlMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::changeWaistCtrlMode::Request>("request",false);
component<::kuavo_msgs::changeWaistCtrlMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/chassisPoseCmdSrv");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::chassisPoseCmdSrv>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::chassisPoseCmdSrv::Request>("request",false);
component<::kuavo_msgs::chassisPoseCmdSrv::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/controlLejuClaw");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::controlLejuClaw>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::controlLejuClaw::Request>("request",false);
component<::kuavo_msgs::controlLejuClaw::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/dexhandCommand");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::dexhandCommand>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::dexhandCommand>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/dexhandTouchState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::dexhandTouchState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::dexhandTouchState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/eePoseReachError");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::eePoseReachError>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::eePoseReachError::Request>("request",false);
component<::kuavo_msgs::eePoseReachError::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/enableHandTouchSensor");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::enableHandTouchSensor>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::enableHandTouchSensor::Request>("request",false);
component<::kuavo_msgs::enableHandTouchSensor::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/endEffectorData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::endEffectorData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::endEffectorData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/fkSrv");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::fkSrv>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::fkSrv::Request>("request",false);
component<::kuavo_msgs::fkSrv::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/fkSrvWithReferFrame");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::fkSrvWithReferFrame>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::fkSrvWithReferFrame::Request>("request",false);
component<::kuavo_msgs::fkSrvWithReferFrame::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/footPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::footPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::footPose>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/footPose6D");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::footPose6D>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::footPose6D>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/footPose6DTargetTrajectories");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::footPose6DTargetTrajectories>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::footPose6DTargetTrajectories>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/footPose6DTargetTrajectoriesSrv");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::footPose6DTargetTrajectoriesSrv>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::footPose6DTargetTrajectoriesSrv::Request>("request",false);
component<::kuavo_msgs::footPose6DTargetTrajectoriesSrv::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/footPoseTargetTrajectories");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::footPoseTargetTrajectories>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::footPoseTargetTrajectories>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/footPoseTargetTrajectoriesSrv");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::footPoseTargetTrajectoriesSrv>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::footPoseTargetTrajectoriesSrv::Request>("request",false);
component<::kuavo_msgs::footPoseTargetTrajectoriesSrv::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/footPoseWithVision");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::footPoseWithVision>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::footPoseWithVision>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/footPoseWithVisionArray");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::footPoseWithVisionArray>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::footPoseWithVisionArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/footPoses");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::footPoses>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::footPoses>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/footPoses6D");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::footPoses6D>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::footPoses6D>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/fullBodyTargetTrajectories");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::fullBodyTargetTrajectories>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::fullBodyTargetTrajectories>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/gaitTimeName");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::gaitTimeName>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::gaitTimeName>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/gestureExecute");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::gestureExecute>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::gestureExecute::Request>("request",false);
component<::kuavo_msgs::gestureExecute::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/gestureExecuteState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::gestureExecuteState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::gestureExecuteState::Request>("request",false);
component<::kuavo_msgs::gestureExecuteState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/gestureInfo");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::gestureInfo>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::gestureInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/gestureList");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::gestureList>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::gestureList::Request>("request",false);
component<::kuavo_msgs::gestureList::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/gestureTask");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::gestureTask>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::gestureTask>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/getControllerList");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::getControllerList>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::getControllerList::Request>("request",false);
component<::kuavo_msgs::getControllerList::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/getCurrentGaitName");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::getCurrentGaitName>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::getCurrentGaitName::Request>("request",false);
component<::kuavo_msgs::getCurrentGaitName::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/getLbTorsoInitialPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::getLbTorsoInitialPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::getLbTorsoInitialPose::Request>("request",false);
component<::kuavo_msgs::getLbTorsoInitialPose::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/getMotorParam");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::getMotorParam>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::getMotorParam::Request>("request",false);
component<::kuavo_msgs::getMotorParam::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/getMotorZeroPoints");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::getMotorZeroPoints>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::getMotorZeroPoints::Request>("request",false);
component<::kuavo_msgs::getMotorZeroPoints::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/goToZeroPoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::goToZeroPoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::goToZeroPoint::Request>("request",false);
component<::kuavo_msgs::goToZeroPoint::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/handForceLevel");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::handForceLevel>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::handForceLevel::Request>("request",false);
component<::kuavo_msgs::handForceLevel::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/handPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::handPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::handPose>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/headBodyPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::headBodyPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::headBodyPose>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/headCtrlMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::headCtrlMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::headCtrlMode>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/ikSolveError");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::ikSolveError>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::ikSolveError>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/ikSolveParam");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::ikSolveParam>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::ikSolveParam>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/imuData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::imuData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::imuData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/jointBezierTrajectory");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::jointBezierTrajectory>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::jointBezierTrajectory>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/jointCmd");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::jointCmd>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::jointCmd>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/jointData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::jointData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::jointData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/jointMoveTo");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::jointMoveTo>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::jointMoveTo::Request>("request",false);
component<::kuavo_msgs::jointMoveTo::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/kuavoModeSchedule");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::kuavoModeSchedule>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::kuavoModeSchedule>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/lbBaseLinkPoseCmdSrv");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::lbBaseLinkPoseCmdSrv>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::lbBaseLinkPoseCmdSrv::Request>("request",false);
component<::kuavo_msgs::lbBaseLinkPoseCmdSrv::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/lbLegControlSrv");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::lbLegControlSrv>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::lbLegControlSrv::Request>("request",false);
component<::kuavo_msgs::lbLegControlSrv::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/lbMultiTimedOfflineTraj");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::lbMultiTimedOfflineTraj>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::lbMultiTimedOfflineTraj::Request>("request",false);
component<::kuavo_msgs::lbMultiTimedOfflineTraj::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/lbMultiTimedPosCmd");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::lbMultiTimedPosCmd>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::lbMultiTimedPosCmd::Request>("request",false);
component<::kuavo_msgs::lbMultiTimedPosCmd::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/lbTimedOfflineTraj");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::lbTimedOfflineTraj>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::lbTimedOfflineTraj>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/lbTimedPosCmd");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::lbTimedPosCmd>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::lbTimedPosCmd::Request>("request",false);
component<::kuavo_msgs::lbTimedPosCmd::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/lejuClawCommand");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::lejuClawCommand>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::lejuClawCommand>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/lejuClawRecovery");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::lejuClawRecovery>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::lejuClawRecovery::Request>("request",false);
component<::kuavo_msgs::lejuClawRecovery::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/lejuClawState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::lejuClawState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::lejuClawState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/lejuClawTarget");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::lejuClawTarget>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::lejuClawTarget>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/motorParam");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::motorParam>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::motorParam>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/mpc_input");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::mpc_input>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::mpc_input>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/mpc_state");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::mpc_state>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::mpc_state>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/mpc_target_trajectories");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::mpc_target_trajectories>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::mpc_target_trajectories>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/picoPoseInfo");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::picoPoseInfo>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::picoPoseInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/picoPoseInfoList");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::picoPoseInfoList>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::picoPoseInfoList>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/picoPoseRetarget");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::picoPoseRetarget>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::picoPoseRetarget>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/planArmState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::planArmState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::planArmState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/planArmTrajectoryBezierCurve");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::planArmTrajectoryBezierCurve>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::planArmTrajectoryBezierCurve::Request>("request",false);
component<::kuavo_msgs::planArmTrajectoryBezierCurve::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/planArmTrajectoryCubicSpline");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::planArmTrajectoryCubicSpline>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::planArmTrajectoryCubicSpline::Request>("request",false);
component<::kuavo_msgs::planArmTrajectoryCubicSpline::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/playmusic");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::playmusic>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::playmusic::Request>("request",false);
component<::kuavo_msgs::playmusic::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/qv");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::qv>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::qv>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/recordArmHandPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::recordArmHandPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::recordArmHandPose>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/recordmusic");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::recordmusic>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::recordmusic::Request>("request",false);
component<::kuavo_msgs::recordmusic::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/robotArmQVVD");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::robotArmQVVD>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::robotArmQVVD>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/robotBodyMatrices");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::robotBodyMatrices>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::robotBodyMatrices>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/robotHandPosition");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::robotHandPosition>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::robotHandPosition>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/robotHeadMotionData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::robotHeadMotionData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::robotHeadMotionData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/robotState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::robotState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::robotState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/robotSwitchPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::robotSwitchPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::robotSwitchPose::Request>("request",false);
component<::kuavo_msgs::robotSwitchPose::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/robotWaistControl");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::robotWaistControl>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::robotWaistControl>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/sensorsData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::sensorsData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::sensorsData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/setContactForceInterpParams");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::setContactForceInterpParams>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::setContactForceInterpParams::Request>("request",false);
component<::kuavo_msgs::setContactForceInterpParams::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/setHwIntialState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::setHwIntialState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::setHwIntialState::Request>("request",false);
component<::kuavo_msgs::setHwIntialState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/setMmCtrlFrame");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::setMmCtrlFrame>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::setMmCtrlFrame::Request>("request",false);
component<::kuavo_msgs::setMmCtrlFrame::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/setMotorEncoderRoundService");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::setMotorEncoderRoundService>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::setMotorEncoderRoundService::Request>("request",false);
component<::kuavo_msgs::setMotorEncoderRoundService::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/setRuckigPlannerParams");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::setRuckigPlannerParams>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::setRuckigPlannerParams::Request>("request",false);
component<::kuavo_msgs::setRuckigPlannerParams::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/setTagId");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::setTagId>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::setTagId::Request>("request",false);
component<::kuavo_msgs::setTagId::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/singleStepControl");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::singleStepControl>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::singleStepControl::Request>("request",false);
component<::kuavo_msgs::singleStepControl::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/stairAlignmentSrv");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::stairAlignmentSrv>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::stairAlignmentSrv::Request>("request",false);
component<::kuavo_msgs::stairAlignmentSrv::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/switchController");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::switchController>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::switchController::Request>("request",false);
component<::kuavo_msgs::switchController::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/switchGaitByName");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::switchGaitByName>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::switchGaitByName>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/switchToNextController");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::switchToNextController>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::switchToNextController::Request>("request",false);
component<::kuavo_msgs::switchToNextController::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/tagDataArray");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::tagDataArray>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::tagDataArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/timedPoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::timedPoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::timedPoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/timedSingleCmd");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::timedSingleCmd>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::timedSingleCmd>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/touchSensorStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::touchSensorStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::touchSensorStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/twoArmHandPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::twoArmHandPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::twoArmHandPose>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/twoArmHandPoseCmd");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::twoArmHandPoseCmd>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::twoArmHandPoseCmd>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/twoArmHandPoseCmdFree");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::twoArmHandPoseCmdFree>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::twoArmHandPoseCmdFree>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/twoArmHandPoseCmdFreeSrv");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::twoArmHandPoseCmdFreeSrv>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::twoArmHandPoseCmdFreeSrv::Request>("request",false);
component<::kuavo_msgs::twoArmHandPoseCmdFreeSrv::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/twoArmHandPoseCmdSrv");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_msgs::twoArmHandPoseCmdSrv>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::twoArmHandPoseCmdSrv::Request>("request",false);
component<::kuavo_msgs::twoArmHandPoseCmdSrv::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/twoArmHandPoseFree");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::twoArmHandPoseFree>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::twoArmHandPoseFree>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/xsensePoseInfo");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::xsensePoseInfo>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::xsensePoseInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/xsensePoseInfoList");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::xsensePoseInfoList>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::xsensePoseInfoList>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/xsensePoseRetarget");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::xsensePoseRetarget>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::xsensePoseRetarget>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/yoloDetection");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::yoloDetection>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::yoloDetection>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_msgs/yoloOutputData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_msgs::yoloOutputData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_msgs::yoloOutputData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_pico_gmr/AmpJoyRoute");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_pico_gmr::AmpJoyRoute>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_pico_gmr::AmpJoyRoute::Request>("request",false);
component<::kuavo_pico_gmr::AmpJoyRoute::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_ros_interfaces/armTargetPoses");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_ros_interfaces::armTargetPoses>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_ros_interfaces::armTargetPoses>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_ros_interfaces/bezierCurveCubicPoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_ros_interfaces::bezierCurveCubicPoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_ros_interfaces::bezierCurveCubicPoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_ros_interfaces/jointBezierTrajectory");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_ros_interfaces::jointBezierTrajectory>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_ros_interfaces::jointBezierTrajectory>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_ros_interfaces/ocs2ChangeArmCtrlMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_ros_interfaces::ocs2ChangeArmCtrlMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_ros_interfaces::ocs2ChangeArmCtrlMode::Request>("request",false);
component<::kuavo_ros_interfaces::ocs2ChangeArmCtrlMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_ros_interfaces/planArmState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_ros_interfaces::planArmState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_ros_interfaces::planArmState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_ros_interfaces/planArmTrajectoryBezierCurve");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_ros_interfaces::planArmTrajectoryBezierCurve>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_ros_interfaces::planArmTrajectoryBezierCurve::Request>("request",false);
component<::kuavo_ros_interfaces::planArmTrajectoryBezierCurve::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_ros_interfaces/robotHandPosition");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_ros_interfaces::robotHandPosition>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_ros_interfaces::robotHandPosition>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_ros_interfaces/robotHeadMotionData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::kuavo_ros_interfaces::robotHeadMotionData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_ros_interfaces::robotHeadMotionData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("kuavo_ros_interfaces/stopPlanArmTrajectory");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::kuavo_ros_interfaces::stopPlanArmTrajectory>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::kuavo_ros_interfaces::stopPlanArmTrajectory::Request>("request",false);
component<::kuavo_ros_interfaces::stopPlanArmTrajectory::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/BMS");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::BMS>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::BMS>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/BaseCmdVelStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::BaseCmdVelStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::BaseCmdVelStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/BaseMove");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::leju_mobile_base_msgs::BaseMove>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::BaseMove::Request>("request",false);
component<::leju_mobile_base_msgs::BaseMove::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/Battery");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::Battery>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::Battery>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/CheckArrived");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::leju_mobile_base_msgs::CheckArrived>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::CheckArrived::Request>("request",false);
component<::leju_mobile_base_msgs::CheckArrived::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/ControlLaserScan");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::leju_mobile_base_msgs::ControlLaserScan>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::ControlLaserScan::Request>("request",false);
component<::leju_mobile_base_msgs::ControlLaserScan::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/DispatchMove");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::leju_mobile_base_msgs::DispatchMove>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::DispatchMove::Request>("request",false);
component<::leju_mobile_base_msgs::DispatchMove::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/ExecTask");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::leju_mobile_base_msgs::ExecTask>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::ExecTask::Request>("request",false);
component<::leju_mobile_base_msgs::ExecTask::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/GetGlobalOdometer");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::leju_mobile_base_msgs::GetGlobalOdometer>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::GetGlobalOdometer::Request>("request",false);
component<::leju_mobile_base_msgs::GetGlobalOdometer::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/GetLaserScanList");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::leju_mobile_base_msgs::GetLaserScanList>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::GetLaserScanList::Request>("request",false);
component<::leju_mobile_base_msgs::GetLaserScanList::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/HardwareStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::HardwareStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::HardwareStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/Information");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::Information>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::Information>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/LED");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::LED>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::LED>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/LiftControl3Feedback");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::LiftControl3Feedback>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::LiftControl3Feedback>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/LiftControl3Goal");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::LiftControl3Goal>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::LiftControl3Goal>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/LiftControl3Result");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::LiftControl3Result>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::LiftControl3Result>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/LiftStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::LiftStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::LiftStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/MoveBaseActionGoal");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::MoveBaseActionGoal>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::MoveBaseActionGoal>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/MoveToTarget");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::leju_mobile_base_msgs::MoveToTarget>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::MoveToTarget::Request>("request",false);
component<::leju_mobile_base_msgs::MoveToTarget::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/MoveToTargetOptions");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::MoveToTargetOptions>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::MoveToTargetOptions>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/NavigationModeGoal");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::NavigationModeGoal>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::NavigationModeGoal>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/Parameter");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::Parameter>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::Parameter>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/ResetTemporaryOdometer");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::leju_mobile_base_msgs::ResetTemporaryOdometer>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::ResetTemporaryOdometer::Request>("request",false);
component<::leju_mobile_base_msgs::ResetTemporaryOdometer::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/RobotStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::RobotStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::RobotStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/SetDispatchMode");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::leju_mobile_base_msgs::SetDispatchMode>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::SetDispatchMode::Request>("request",false);
component<::leju_mobile_base_msgs::SetDispatchMode::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/SetMapServer");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::leju_mobile_base_msgs::SetMapServer>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::SetMapServer::Request>("request",false);
component<::leju_mobile_base_msgs::SetMapServer::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/StartLocalization");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::leju_mobile_base_msgs::StartLocalization>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::StartLocalization::Request>("request",false);
component<::leju_mobile_base_msgs::StartLocalization::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/StepControlGoal");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::StepControlGoal>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::StepControlGoal>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/SyncCostmap");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::leju_mobile_base_msgs::SyncCostmap>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::SyncCostmap::Request>("request",false);
component<::leju_mobile_base_msgs::SyncCostmap::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("leju_mobile_base_msgs/TaskDetailStatus");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::leju_mobile_base_msgs::TaskDetailStatus>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::leju_mobile_base_msgs::TaskDetailStatus>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("motioncapture/ForcePlateData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::motioncapture::ForcePlateData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::motioncapture::ForcePlateData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nav_msgs/Path");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nav_msgs::Path>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nav_msgs::Path>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("network_monitor/NetworkPing");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::network_monitor::NetworkPing>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::network_monitor::NetworkPing::Request>("request",false);
component<::network_monitor::NetworkPing::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("network_monitor/NetworkPingMsg");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::network_monitor::NetworkPingMsg>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::network_monitor::NetworkPingMsg>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nimservos_controller/motor_cmd");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nimservos_controller::motor_cmd>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nimservos_controller::motor_cmd>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nimservos_controller/motor_state");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nimservos_controller::motor_state>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nimservos_controller::motor_state>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/IotFrame0");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::IotFrame0>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::IotFrame0>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/IotFrame0Node");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::IotFrame0Node>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::IotFrame0Node>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackAnchorframe0");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackAnchorframe0>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackAnchorframe0>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackAoaNode0");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackAoaNode0>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackAoaNode0>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackAoaNodeframe0");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackAoaNodeframe0>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackAoaNodeframe0>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackNode0");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackNode0>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackNode0>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackNode1");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackNode1>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackNode1>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackNode2");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackNode2>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackNode2>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackNode4Anchor");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackNode4Anchor>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackNode4Anchor>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackNode4Tag");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackNode4Tag>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackNode4Tag>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackNode5");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackNode5>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackNode5>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackNode6");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackNode6>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackNode6>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackNodeframe0");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackNodeframe0>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackNodeframe0>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackNodeframe1");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackNodeframe1>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackNodeframe1>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackNodeframe2");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackNodeframe2>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackNodeframe2>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackNodeframe3");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackNodeframe3>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackNodeframe3>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackNodeframe4");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackNodeframe4>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackNodeframe4>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackNodeframe5");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackNodeframe5>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackNodeframe5>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackNodeframe6");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackNodeframe6>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackNodeframe6>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackTag");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackTag>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackTag>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/LinktrackTagframe0");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::LinktrackTagframe0>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::LinktrackTagframe0>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/TofsenseCascade");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::TofsenseCascade>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::TofsenseCascade>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/TofsenseFrame0");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::TofsenseFrame0>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::TofsenseFrame0>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/TofsenseMFrame0");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::TofsenseMFrame0>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::TofsenseMFrame0>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("nlink_parser/TofsenseMFrame0Pixel");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::nlink_parser::TofsenseMFrame0Pixel>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::nlink_parser::TofsenseMFrame0Pixel>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("noitom_hi5_hand_udp_python/JoySticks");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::noitom_hi5_hand_udp_python::JoySticks>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::noitom_hi5_hand_udp_python::JoySticks>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("noitom_hi5_hand_udp_python/PoseInfo");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::noitom_hi5_hand_udp_python::PoseInfo>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::noitom_hi5_hand_udp_python::PoseInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("noitom_hi5_hand_udp_python/PoseInfoList");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::noitom_hi5_hand_udp_python::PoseInfoList>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::noitom_hi5_hand_udp_python::PoseInfoList>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("noitom_hi5_hand_udp_python/QuaternionArray");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::noitom_hi5_hand_udp_python::QuaternionArray>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::noitom_hi5_hand_udp_python::QuaternionArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("noitom_hi5_hand_udp_python/Vector4");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::noitom_hi5_hand_udp_python::Vector4>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::noitom_hi5_hand_udp_python::Vector4>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("noitom_hi5_hand_udp_python/handRotation");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::noitom_hi5_hand_udp_python::handRotation>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::noitom_hi5_hand_udp_python::handRotation>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("noitom_hi5_hand_udp_python/handRotationEular");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::noitom_hi5_hand_udp_python::handRotationEular>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::noitom_hi5_hand_udp_python::handRotationEular>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_msgs/constraint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ocs2_msgs::constraint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_msgs::constraint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_msgs/controller_data");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ocs2_msgs::controller_data>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_msgs::controller_data>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_msgs/lagrangian_metrics");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ocs2_msgs::lagrangian_metrics>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_msgs::lagrangian_metrics>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_msgs/mode_schedule");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ocs2_msgs::mode_schedule>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_msgs::mode_schedule>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_msgs/mpc_flattened_controller");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ocs2_msgs::mpc_flattened_controller>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_msgs::mpc_flattened_controller>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_msgs/mpc_input");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ocs2_msgs::mpc_input>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_msgs::mpc_input>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_msgs/mpc_observation");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ocs2_msgs::mpc_observation>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_msgs::mpc_observation>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_msgs/mpc_performance_indices");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ocs2_msgs::mpc_performance_indices>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_msgs::mpc_performance_indices>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_msgs/mpc_solver_data");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ocs2_msgs::mpc_solver_data>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_msgs::mpc_solver_data>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_msgs/mpc_state");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ocs2_msgs::mpc_state>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_msgs::mpc_state>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_msgs/mpc_target_trajectories");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ocs2_msgs::mpc_target_trajectories>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_msgs::mpc_target_trajectories>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_msgs/multiplier");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ocs2_msgs::multiplier>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_msgs::multiplier>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_msgs/pause_resume");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ocs2_msgs::pause_resume>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_msgs::pause_resume::Request>("request",false);
component<::ocs2_msgs::pause_resume::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_msgs/reset");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ocs2_msgs::reset>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_msgs::reset::Request>("request",false);
component<::ocs2_msgs::reset::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_switched_model_msgs/gait");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ocs2_switched_model_msgs::gait>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_switched_model_msgs::gait>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_switched_model_msgs/gait_sequence");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ocs2_switched_model_msgs::gait_sequence>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_switched_model_msgs::gait_sequence>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_switched_model_msgs/scheduled_gait_sequence");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::ocs2_switched_model_msgs::scheduled_gait_sequence>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_switched_model_msgs::scheduled_gait_sequence>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("ocs2_switched_model_msgs/trajectory_request");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::ocs2_switched_model_msgs::trajectory_request>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::ocs2_switched_model_msgs::trajectory_request::Request>("request",false);
component<::ocs2_switched_model_msgs::trajectory_request::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("robot_calibration_msgs/CalibrationData");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::robot_calibration_msgs::CalibrationData>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::robot_calibration_msgs::CalibrationData>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("robot_calibration_msgs/CameraParameter");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::robot_calibration_msgs::CameraParameter>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::robot_calibration_msgs::CameraParameter>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("robot_calibration_msgs/CaptureConfig");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::robot_calibration_msgs::CaptureConfig>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::robot_calibration_msgs::CaptureConfig>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("robot_calibration_msgs/ExtendedCameraInfo");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::robot_calibration_msgs::ExtendedCameraInfo>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::robot_calibration_msgs::ExtendedCameraInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("robot_calibration_msgs/GripperLedCommand");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::robot_calibration_msgs::GripperLedCommandAction>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::robot_calibration_msgs::GripperLedCommandGoal>("goal",false);
component<::robot_calibration_msgs::GripperLedCommandResult>("result",true);
component<::robot_calibration_msgs::GripperLedCommandFeedback>("feedback",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("robot_calibration_msgs/Observation");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::robot_calibration_msgs::Observation>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::robot_calibration_msgs::Observation>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("robot_localization/FromLL");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::robot_localization::FromLL>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::robot_localization::FromLL::Request>("request",false);
component<::robot_localization::FromLL::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("robot_localization/GetState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::robot_localization::GetState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::robot_localization::GetState::Request>("request",false);
component<::robot_localization::GetState::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("robot_localization/SetDatum");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::robot_localization::SetDatum>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::robot_localization::SetDatum::Request>("request",false);
component<::robot_localization::SetDatum::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("robot_localization/SetPose");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::robot_localization::SetPose>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::robot_localization::SetPose::Request>("request",false);
component<::robot_localization::SetPose::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("robot_localization/SetUTMZone");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::robot_localization::SetUTMZone>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::robot_localization::SetUTMZone::Request>("request",false);
component<::robot_localization::SetUTMZone::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("robot_localization/ToLL");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::robot_localization::ToLL>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::robot_localization::ToLL::Request>("request",false);
component<::robot_localization::ToLL::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("robot_localization/ToggleFilterProcessing");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::robot_localization::ToggleFilterProcessing>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::robot_localization::ToggleFilterProcessing::Request>("request",false);
component<::robot_localization::ToggleFilterProcessing::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("sensor_msgs/CameraInfo");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::sensor_msgs::CameraInfo>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::sensor_msgs::CameraInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("sensor_msgs/Image");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::sensor_msgs::Image>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::sensor_msgs::Image>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("sensor_msgs/JointState");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::sensor_msgs::JointState>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::sensor_msgs::JointState>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("sensor_msgs/PointCloud2");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::sensor_msgs::PointCloud2>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::sensor_msgs::PointCloud2>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("sensor_msgs/PointField");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::sensor_msgs::PointField>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::sensor_msgs::PointField>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("sensor_msgs/RegionOfInterest");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::sensor_msgs::RegionOfInterest>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::sensor_msgs::RegionOfInterest>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/Float32MultiArray");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::std_msgs::Float32MultiArray>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::Float32MultiArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/Float64MultiArray");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::std_msgs::Float64MultiArray>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::Float64MultiArray>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/Header");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::std_msgs::Header>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::Header>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/MultiArrayDimension");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::std_msgs::MultiArrayDimension>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::MultiArrayDimension>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("std_msgs/MultiArrayLayout");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::std_msgs::MultiArrayLayout>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::std_msgs::MultiArrayLayout>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("taiji_trigger_node/PerformTaiji");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::service_traits::MD5Sum<::taiji_trigger_node::PerformTaiji>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::taiji_trigger_node::PerformTaiji::Request>("request",false);
component<::taiji_trigger_node::PerformTaiji::Response>("response",true);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("trajectory_msgs/JointTrajectory");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::trajectory_msgs::JointTrajectory>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::trajectory_msgs::JointTrajectory>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("trajectory_msgs/JointTrajectoryPoint");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::trajectory_msgs::JointTrajectoryPoint>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::trajectory_msgs::JointTrajectoryPoint>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("user_pkg/piddebug");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::user_pkg::piddebug>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::user_pkg::piddebug>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("user_pkg/uwbdebug");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::user_pkg::uwbdebug>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::user_pkg::uwbdebug>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("wiimote/IrSourceInfo");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::wiimote::IrSourceInfo>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::wiimote::IrSourceInfo>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("wiimote/State");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::wiimote::State>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::wiimote::State>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),',');
quoted("wiimote/TimedSwitch");
bounded_native_io::print(output(),":{\"native_identity\":");
quoted(::ros::message_traits::MD5Sum<::wiimote::TimedSwitch>::value());
bounded_native_io::print(output(),",\"components\":{");
component<::wiimote::TimedSwitch>("message",false);
bounded_native_io::print(output(),"}}");
bounded_native_io::print(output(),"}}\n");return 0;
}catch(std::exception const&){bounded_native_io::print(STDERR_FILENO,"native industrial metadata refused\n");return 1;}}
