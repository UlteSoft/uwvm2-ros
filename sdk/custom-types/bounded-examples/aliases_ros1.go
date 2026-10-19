//go:build tinygo && wasm && ros1 && !ros2

// Explicit ROS1 typed canonical component selection; no cross-backend fallback.
package main
import bounded "example.local/bounded/customtypes"
// Envelope is the complete generated component, with reviewed finite capacities.
type Envelope=bounded.Ros1Uwvm_ros_bounded_v2Envelope
// Point is the complete generated component, with reviewed finite capacities.
type Point=bounded.Ros1Uwvm_ros_bounded_v2Point
// Request is the complete generated component, with reviewed finite capacities.
type Request=bounded.Ros1Uwvm_ros_bounded_v2EchoRequest
// Response is the complete generated component, with reviewed finite capacities.
type Response=bounded.Ros1Uwvm_ros_bounded_v2EchoResponse
// EmptyRequest is the complete generated component, with reviewed finite capacities.
type EmptyRequest=bounded.Ros1Uwvm_ros_bounded_v2EmptyRequest
// EmptyResponse is the complete generated component, with reviewed finite capacities.
type EmptyResponse=bounded.Ros1Uwvm_ros_bounded_v2EmptyResponse
// Goal is the complete generated component, with reviewed finite capacities.
type Goal=bounded.Ros1Uwvm_ros_bounded_v2DigestGoal
// Feedback is the complete generated component, with reviewed finite capacities.
type Feedback=bounded.Ros1Uwvm_ros_bounded_v2DigestFeedback
// ResultValue is the complete generated component, with reviewed finite capacities.
type ResultValue=bounded.Ros1Uwvm_ros_bounded_v2DigestResult
const responseMax=bounded.Ros1Uwvm_ros_bounded_v2EchoResponseMaxWireBytes
const feedbackMax=bounded.Ros1Uwvm_ros_bounded_v2DigestFeedbackMaxWireBytes
const resultMax=bounded.Ros1Uwvm_ros_bounded_v2DigestResultMaxWireBytes
var decodeResponse=bounded.DecodeRos1Uwvm_ros_bounded_v2EchoResponse
var decodeEmptyResponse=bounded.DecodeRos1Uwvm_ros_bounded_v2EmptyResponse
var decodeFeedback=bounded.DecodeRos1Uwvm_ros_bounded_v2DigestFeedback
var decodeResult=bounded.DecodeRos1Uwvm_ros_bounded_v2DigestResult
const requestMax=bounded.Ros1Uwvm_ros_bounded_v2EchoRequestMaxWireBytes
const goalMax=bounded.Ros1Uwvm_ros_bounded_v2DigestGoalMaxWireBytes
var decodeRequest=bounded.DecodeRos1Uwvm_ros_bounded_v2EchoRequest
var decodeGoal=bounded.DecodeRos1Uwvm_ros_bounded_v2DigestGoal
