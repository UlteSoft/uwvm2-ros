// Compile only against actual separately generated official-standard ROS1.
test "complete ROS1 Header unsigned fields and optional JointState arrays" {
  let codec = @records.Ros1Sensor_msgsJointStateCodec::new()
  let value = @records.Ros1Sensor_msgsJointState::new()
  value.header.seq = 19U
  value.header.stamp.sec = 42U
  value.header.stamp.nsec = 300000000U
  value.header.frame_id = "base"
  value.name = ["left", "right"]
  value.position = [1.25, -2.5]
  value.velocity = []
  value.effort = [0.5, -0.25]
  let bytes = match codec.encode(value) { Ok(bytes) => bytes; Err(_) => abort("encode") }
  match codec.decode(bytes) {
    Ok(decoded) => assert_true(decoded.header.seq == 19U && decoded.header.stamp.sec == 42U &&
      decoded.header.stamp.nsec == 300000000U && decoded.header.frame_id == "base" &&
      decoded.name == ["left", "right"] && decoded.position == [1.25, -2.5] &&
      decoded.velocity.length() == 0 && decoded.effort == [0.5, -0.25])
    Err(_) => assert_true(false)
  }
  value.velocity = [0.0]
  assert_true(codec.encode(value) is Err(_))
  value.velocity = []
  value.header.stamp.nsec = 1000000000U
  assert_true(codec.encode(value) is Err(_))
  assert_true(codec.decode(bytes + b"\x00") is Err(_))
}
