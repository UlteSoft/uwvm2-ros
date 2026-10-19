//! Exact ABI imports with explicit memory indices and u64 offsets.
//! Feature selection removes declarations for disabled ROS backends.

/// ROS 1 imports from `uwvm_ros1_v1`; enable the `ros1` Cargo feature.
#[cfg(feature = "ros1")]
pub mod ros1;
/// ROS 2 imports from `uwvm_ros2_v1`; enable the `ros2` Cargo feature.
#[cfg(feature = "ros2")]
pub mod ros2;
