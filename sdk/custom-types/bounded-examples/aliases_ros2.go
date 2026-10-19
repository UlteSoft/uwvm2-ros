//go:build tinygo && wasm && ros2 && !ros1

// Explicit ROS2 typed canonical component selection; no cross-backend fallback.
package main
import bounded "example.local/bounded/customtypes"
// Envelope is the complete generated component, with reviewed finite capacities.
type Envelope=bounded.Ros2Uwvm_ros_bounded_v2MsgEnvelope
// Point is the complete generated component, with reviewed finite capacities.
type Point=bounded.Ros2Uwvm_ros_bounded_v2MsgPoint
// Request is the complete generated component, with reviewed finite capacities.
type Request=bounded.Ros2Uwvm_ros_bounded_v2SrvEcho_Request
// Response is the complete generated component, with reviewed finite capacities.
type Response=bounded.Ros2Uwvm_ros_bounded_v2SrvEcho_Response
// EmptyRequest is the complete generated component, with reviewed finite capacities.
type EmptyRequest=bounded.Ros2Uwvm_ros_bounded_v2SrvEmpty_Request
// EmptyResponse is the complete generated component, with reviewed finite capacities.
type EmptyResponse=bounded.Ros2Uwvm_ros_bounded_v2SrvEmpty_Response
// Goal is the complete generated component, with reviewed finite capacities.
type Goal=bounded.Ros2Uwvm_ros_bounded_v2ActionDigest_Goal
// Feedback is the complete generated component, with reviewed finite capacities.
type Feedback=bounded.Ros2Uwvm_ros_bounded_v2ActionDigest_Feedback
// ResultValue is the complete generated component, with reviewed finite capacities.
type ResultValue=bounded.Ros2Uwvm_ros_bounded_v2ActionDigest_Result
const responseMax=bounded.Ros2Uwvm_ros_bounded_v2SrvEcho_ResponseMaxWireBytes
const feedbackMax=bounded.Ros2Uwvm_ros_bounded_v2ActionDigest_FeedbackMaxWireBytes
const resultMax=bounded.Ros2Uwvm_ros_bounded_v2ActionDigest_ResultMaxWireBytes
var decodeResponse=bounded.DecodeRos2Uwvm_ros_bounded_v2SrvEcho_Response
var decodeEmptyResponse=bounded.DecodeRos2Uwvm_ros_bounded_v2SrvEmpty_Response
var decodeFeedback=bounded.DecodeRos2Uwvm_ros_bounded_v2ActionDigest_Feedback
var decodeResult=bounded.DecodeRos2Uwvm_ros_bounded_v2ActionDigest_Result
const requestMax=bounded.Ros2Uwvm_ros_bounded_v2SrvEcho_RequestMaxWireBytes
const goalMax=bounded.Ros2Uwvm_ros_bounded_v2ActionDigest_GoalMaxWireBytes
var decodeRequest=bounded.DecodeRos2Uwvm_ros_bounded_v2SrvEcho_Request
var decodeGoal=bounded.DecodeRos2Uwvm_ros_bounded_v2ActionDigest_Goal
