// Stage byte-exact as guest/observe.mbt; no nonexistent ROS2 Header.seq access.
fn selected_max_wire() -> Int { @records.Ros2Sensor_msgsMsgJointStateCodec::new().max_wire_bytes }
fn decode_observe(bytes : Bytes) -> Int {
  let codec = @records.Ros2Sensor_msgsMsgJointStateCodec::new()
  if codec.ros_version != 2 || codec.native_type != "sensor_msgs/msg/JointState" { return 22 }
  match codec.decode(bytes) {
    Err(code) => code
    Ok(value) => {
      if value.header.stamp.sec != -2 || value.header.stamp.nanosec != 300000000U ||
          value.header.frame_id != "base" ||
          value.name.length() != 2 || value.name[0] != "left" || value.name[1] != "right" ||
          value.position.length() != 2 || value.position[0] != 1.25 || value.position[1] != -2.5 ||
          value.velocity.length() != 0 || value.effort.length() != 2 ||
          value.effort[0] != 0.5 || value.effort[1] != -0.25 { return 22 }
      diagnostics.received = diagnostics.received + 1
      0
    }
  }
}
