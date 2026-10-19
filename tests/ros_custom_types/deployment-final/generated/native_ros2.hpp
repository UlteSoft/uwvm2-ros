/** Materialized exact native provider row sets; never macro-overwrite substitution. */
#pragma once
#include "fixed/native_ros2.hpp"
#undef UWVM_ROS_CUSTOM_ROS2_TOPIC_TYPES
#undef UWVM_ROS_CUSTOM_ROS2_TYPES
#include "bounded/native_ros2.hpp"
#undef UWVM_ROS_CUSTOM_ROS2_TOPIC_TYPES
#undef UWVM_ROS_CUSTOM_ROS2_TYPES
#undef UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES
#undef UWVM_ROS_CUSTOM_ROS2_ACTION_TYPES
#define UWVM_ROS_CUSTOM_ROS2_FIXED_TYPES(M) \
  M(::uwvm_ros_custom_test_msgs::msg::FixedSample,"uwvm_ros_custom_test_msgs/msg/FixedSample","RIHS01_5026d3427fd5649d1cb11a7cf9e79429d2775096e9bec98669dde689b4d20fc2","UWVMROS1_3f390efa2dccaf91a53c14a50fcead8790bb1bbc103c6ac6b29f176d045e2516",45,::uwvm_custom_native::Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec)
#define UWVM_ROS_CUSTOM_ROS2_BOUNDED_TYPES(M) \
  M(::uwvm_ros_bounded_v2::msg::Point,"uwvm_ros_bounded_v2/msg/Point","RIHS01_7f58e859cebde43421f1dc78d6f7c71effd3adad3bd3620f64c48405a2f1cc91","UWVMROS1_60309848e4857d663e4b90a42790335a74352469d0d30bc3bf33083b529fb333",5,::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2MsgPointCodec) \
  M(::uwvm_ros_bounded_v2::msg::Envelope,"uwvm_ros_bounded_v2/msg/Envelope","RIHS01_db84bfb7426f4f7bbd57581e2ec9774e6d69693f18194f18138d2bb435b384ca","UWVMROS1_0a1a27d41a3aa4946fd3e118be4a76e32b496b4aff9206ba7c45f69f85374c36",120,::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec)
#define UWVM_ROS_CUSTOM_ROS2_TYPES(M) \
  M(::uwvm_ros_custom_test_msgs::msg::FixedSample,"uwvm_ros_custom_test_msgs/msg/FixedSample","RIHS01_5026d3427fd5649d1cb11a7cf9e79429d2775096e9bec98669dde689b4d20fc2","UWVMROS1_3f390efa2dccaf91a53c14a50fcead8790bb1bbc103c6ac6b29f176d045e2516",45,::uwvm_custom_native::Ros2Uwvm_ros_custom_test_msgsFixedSampleCodec) \
  M(::uwvm_ros_bounded_v2::msg::Point,"uwvm_ros_bounded_v2/msg/Point","RIHS01_7f58e859cebde43421f1dc78d6f7c71effd3adad3bd3620f64c48405a2f1cc91","UWVMROS1_60309848e4857d663e4b90a42790335a74352469d0d30bc3bf33083b529fb333",5,::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2MsgPointCodec) \
  M(::uwvm_ros_bounded_v2::msg::Envelope,"uwvm_ros_bounded_v2/msg/Envelope","RIHS01_db84bfb7426f4f7bbd57581e2ec9774e6d69693f18194f18138d2bb435b384ca","UWVMROS1_0a1a27d41a3aa4946fd3e118be4a76e32b496b4aff9206ba7c45f69f85374c36",120,::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2MsgEnvelopeCodec)
#define UWVM_ROS_CUSTOM_ROS2_SERVICE_TYPES(M) \
  M(::uwvm_ros_bounded_v2::srv::Echo,"uwvm_ros_bounded_v2/srv/Echo","RIHS01_b6a5373556a7278835d767fffd496e993656833bf4c72d008e10f88669e7ff46","UWVMROS1_f8a4ed1cffd983c44306305ca4a3d4836d05d182fd3e42bdb15e8ca8043be2f4",::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2SrvEcho_RequestCodec,::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2SrvEcho_ResponseCodec) \
  M(::uwvm_ros_bounded_v2::srv::Empty,"uwvm_ros_bounded_v2/srv/Empty","RIHS01_a0f5e07741124efbd845dab80bdb5cc355cabe501c2718b51d2377a3cfa6527b","UWVMROS1_958622d138e27210d360895e1d132afb01e3050ca2da9ff6cf5652100ced504a",::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2SrvEmpty_RequestCodec,::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2SrvEmpty_ResponseCodec)
#define UWVM_ROS_CUSTOM_ROS2_ACTION_TYPES(M) \
  M(::uwvm_ros_bounded_v2::action::Digest,"uwvm_ros_bounded_v2/action/Digest","RIHS01_2b3b1c551cb6a47adad62f07b8b1e56ed08509eccb07a3515e23db1730647a86","UWVMROS1_a4a829b06fdfc8a0cab20854f410c3366c85b9b8113fe8e80e459bf08a43c3a5",::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2ActionDigest_GoalCodec,::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2ActionDigest_ResultCodec,::uwvm_custom_bounded_native::Ros2Uwvm_ros_bounded_v2ActionDigest_FeedbackCodec)
#define UWVM_ROS_CUSTOM_ROS2_TOPIC_TYPES(M) UWVM_ROS_CUSTOM_ROS2_TYPES(M)
