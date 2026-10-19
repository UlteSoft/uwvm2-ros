// Compile only against actual separately generated official-standard ROS2.
test "complete ROS2 signed second no seq and empty optional velocity" {
  let codec = @records.Ros2Sensor_msgsMsgJointStateCodec::new()
  let value = @records.Ros2Sensor_msgsMsgJointState::new()
  value.header.stamp.sec = -2
  value.header.stamp.nanosec = 300000000U
  value.header.frame_id = "base"
  value.name = ["left", "right"]
  value.position = [1.25, -2.5]
  value.velocity = []
  value.effort = [0.5, -0.25]
  let bytes = match codec.encode(value) { Ok(bytes) => bytes; Err(_) => abort("encode") }
  match codec.decode(bytes) {
    Ok(decoded) => assert_true(decoded.header.stamp.sec == -2 && decoded.header.stamp.nanosec == 300000000U &&
      decoded.header.frame_id == "base" && decoded.name == ["left", "right"] &&
      decoded.position == [1.25, -2.5] && decoded.velocity.length() == 0 && decoded.effort == [0.5, -0.25])
    Err(_) => assert_true(false)
  }
  value.effort = [0.5]
  assert_true(codec.encode(value) is Err(_))
  value.effort = [0.5, -0.25]
  value.header.stamp.nanosec = 1000000000U
  assert_true(codec.encode(value) is Err(_))
  assert_true(codec.decode(bytes[:bytes.length()-1].to_owned()) is Err(_))
}
