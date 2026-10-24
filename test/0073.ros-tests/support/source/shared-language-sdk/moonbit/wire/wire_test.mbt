// Unexecuted codec specifications. These tests contain no ROS import or fake
// native registry. Native identity, delivery and permissions require real runs.

test "opaque full-width handle round trip" {
  assert_true(read_u64(u64(18446744073709551614UL)) == Ok(18446744073709551614UL))
}

test "u64 rejects short slot" { assert_true(read_u64(b"\x01") == Err(22)) }

test "u64 little endian" { assert_true(u64(0x0102030405060708UL) == b"\x08\x07\x06\x05\x04\x03\x02\x01") }

test "bool exact true" { assert_true(read_bool(bool_bytes(true)) == Ok(true)) }

test "bool exact false" { assert_true(read_bool(bool_bytes(false)) == Ok(false)) }

test "bool rejects other byte" { assert_true(read_bool(b"\x02") == Err(22)) }

test "bool rejects trailing byte" { assert_true(read_bool(b"\x01\x00") == Err(22)) }

test "String empty is length prefix" { assert_true(string_bytes("") == Ok(b"\x00\x00\x00\x00")) }

test "String Unicode and NUL round trip" {
  let input = "base\u{0000}机器人🙂"
  match string_bytes(input) { Ok(bytes) => assert_true(read_string(bytes) == Ok(input)); Err(_) => assert_true(false) }
}

test "String literal BOM is preserved" {
  assert_true(read_string(b"\x03\x00\x00\x00\xEF\xBB\xBF") == Ok("\u{FEFF}"))
}

test "String rejects invalid UTF8" { assert_true(read_string(b"\x01\x00\x00\x00\xFF") == Err(22)) }

test "String rejects trailing bytes" { assert_true(read_string(b"\x00\x00\x00\x00x") == Err(22)) }

test "UTF8 budget checked before encoding" { assert_true(bounded_utf8("机器人", 8) == Err(90)) }

test "Twist finite exact48 round trip" {
  let input = [1.25, -2.5, 0.0, 0.5, 0.25, -1.0]
  match twist(input) { Ok(bytes) => assert_true(read_twist(bytes) == Ok(input)); Err(_) => assert_true(false) }
}

test "Twist rejects wrong shape" { assert_true(twist([0.0]) == Err(22)) }

test "Twist rejects infinity" { assert_true(twist([1.0 / 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]) == Err(22)) }

test "service response exact Bool String" {
  match service_response(true, "held") {
    Ok(bytes) => match read_service_response(bytes) {
      Ok(value) => assert_true(value.success && value.message == "held")
      Err(_) => assert_true(false)
    }
    Err(_) => assert_true(false)
  }
}

test "service reply rejects zero ID" { assert_true(id_prefix(0UL, b"") == Err(22)) }

test "Fibonacci goal finite bound" { assert_true(fibonacci_goal(46) == Ok(b"\x2E\x00\x00\x00")) }

test "Fibonacci goal rejects overflow" { assert_true(fibonacci_goal(47) == Err(22)) }

test "Fibonacci terminal ID and outcome" {
  match fibonacci_sequence(u64(0x8000000000000001UL) + b"\x02\x02\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00", true) {
    Ok(value) => assert_true(value.goal == 0x8000000000000001UL && value.kind == 2 && value.values == [0, 1])
    Err(_) => assert_true(false)
  }
}

test "Fibonacci terminal rejects feedback kind" {
  assert_true(fibonacci_sequence(u64(1UL) + b"\x01\x00\x00\x00\x00", true) is Err(22))
}

test "Fibonacci sequence rejects forged count" {
  assert_true(fibonacci_sequence(u64(1UL) + b"\x02\x00\x00\x00", false) is Err(22))
}


test "action envelope checks full unsigned matching id" {
  assert_true(action_envelope(u64(18446744073709551615UL), 18446744073709551615UL, false))
  assert_true(!action_envelope(u64(19UL), 20UL, false))
  assert_true(!action_envelope(u64(0UL), 0UL, false))
}

test "terminal envelope refuses feedback kind and short body" {
  assert_true(!action_envelope(u64(19UL), 19UL, true))
  assert_true(!action_envelope(u64(19UL) + b"\x01", 19UL, true))
  assert_true(action_envelope(u64(19UL) + b"\x02", 19UL, true))
  assert_true(action_envelope(u64(19UL) + b"\x03", 19UL, true))
  assert_true(action_envelope(u64(19UL) + b"\x04", 19UL, true))
  assert_true(!action_envelope(u64(19UL) + b"\x05", 19UL, true))
}

test "actual runtime Fibonacci48 bound accepts48 rejects49" {
  let allowed = u64(19UL) + i32(48) + Bytes::make(48 * 4, 0)
  let too_many = u64(19UL) + i32(49) + Bytes::make(49 * 4, 0)
  match fibonacci_sequence(allowed, false) { Ok(value) => assert_true(value.values.length() == 48); Err(_) => assert_true(false) }
  match fibonacci_sequence(too_many, false) { Err(code) => assert_true(code == 22); Ok(_) => assert_true(false) }
}


// Image tests exercise canonical bytes only; no native registry or grant.
fn image_case(sequence : UInt64, seconds : UInt64, nanos : UInt64, frame : String,
  width : UInt64, height : UInt64, encoding : String, step : UInt64, data : Bytes) -> Image {
  { sequence, seconds, nanoseconds: nanos, frame_id: frame, width, height,
    encoding, bigendian: false, step, data }
}
test "Image exact independent padded RGB vector" {
  let image = image_case(0UL, 1UL, 2UL, "camera/optical", 2UL, 2UL, "rgb8", 8UL, b"\xFF\x00\x00\x00\xFF\x00\xAA\xBB\x00\x00\xFF\xFF\xFF\xFF\xCC\xDD")
  assert_true(image.encode(2) == Ok(b"\x00\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x0E\x00\x00\x00\x63\x61\x6D\x65\x72\x61\x2F\x6F\x70\x74\x69\x63\x61\x6C\x02\x00\x00\x00\x02\x00\x00\x00\x04\x00\x00\x00\x72\x67\x62\x38\x00\x08\x00\x00\x00\x10\x00\x00\x00\xFF\x00\x00\x00\xFF\x00\xAA\xBB\x00\x00\xFF\xFF\xFF\xFF\xCC\xDD"))
}

test "Image complete roundtrip both backend profiles" {
  let image = image_case(0UL, 1UL, 2UL, "camera/optical", 2UL, 2UL, "rgb8", 8UL, b"\xFF\x00\x00\x00\xFF\x00\xAA\xBB\x00\x00\xFF\xFF\xFF\xFF\xCC\xDD")
  for backend = 1; backend <= 2; backend = backend + 1 {
    match image.encode(backend) { Ok(bytes) => assert_true(read_image(bytes, backend) == Ok(image)); Err(_) => assert_true(false) }
  }
}

test "Image RGB row padding becomes packed mono8" {
  let image = image_case(0UL, 1UL, 2UL, "camera/optical", 2UL, 2UL, "rgb8", 8UL, b"\xFF\x00\x00\x00\xFF\x00\xAA\xBB\x00\x00\xFF\xFF\xFF\xFF\xCC\xDD")
  match image.to_mono8(2) { Ok(value) => assert_true(value.data == b"\x4C\x95\x1C\xFF" && value.step == 2UL && value.frame_id == image.frame_id && value.encoding == "mono8"); Err(_) => assert_true(false) }
}

test "Image bgr8 channel order and ignored alpha" {
  let image = image_case(0UL,1UL,2UL,"camera",1UL,1UL,"bgr8",3UL,b"\x00\x00\xff")
  match image.to_mono8(2) { Ok(value) => assert_true(value.data == b"\x4C"); Err(_) => assert_true(false) }
}

test "Image rgba8 channel order and ignored alpha" {
  let image = image_case(0UL,1UL,2UL,"camera",1UL,1UL,"rgba8",4UL,b"\xff\x00\x00\x00")
  match image.to_mono8(2) { Ok(value) => assert_true(value.data == b"\x4C"); Err(_) => assert_true(false) }
}

test "Image bgra8 channel order and ignored alpha" {
  let image = image_case(0UL,1UL,2UL,"camera",1UL,1UL,"bgra8",4UL,b"\x00\x00\xff\x00")
  match image.to_mono8(2) { Ok(value) => assert_true(value.data == b"\x4C"); Err(_) => assert_true(false) }
}

test "Image mono16 retained but conversion unsupported" {
  let image = image_case(0UL,1UL,2UL,"camera",1UL,1UL,"mono16",2UL,b"\x12\x34")
  assert_true(image.validate(2) == 0 && image.to_mono8(2) == Err(95))
}

test "Image ROS1 full u32 sequence and seconds" {
  let image = image_case(4294967295UL,4294967295UL,999999999UL,"camera",1UL,1UL,"mono8",1UL,b"\xAA")
  match image.encode(1) { Ok(bytes) => assert_true(read_image(bytes,1) == Ok(image)); Err(_) => assert_true(false) }
  assert_true(image.validate(2) == 22)
}

test "Image ROS2 requires sequence zero" {
  assert_true(image_case(1UL,1UL,2UL,"camera",1UL,1UL,"mono8",1UL,b"x").validate(2) == 22)
}

test "Image selected backend and u32 timestamp bounds" {
  assert_true(image_case(0UL,2147483648UL,2UL,"camera",1UL,1UL,"mono8",1UL,b"x").validate(2) == 22)
  assert_true(image_case(0UL,4294967296UL,2UL,"camera",1UL,1UL,"mono8",1UL,b"x").validate(1) == 22)
  assert_true((image_case(0UL, 1UL, 2UL, "camera/optical", 2UL, 2UL, "rgb8", 8UL, b"\xFF\x00\x00\x00\xFF\x00\xAA\xBB\x00\x00\xFF\xFF\xFF\xFF\xCC\xDD")).validate(0) == 22)
  assert_true(read_image(b"",3) == Err(22))
}

test "Image nanoseconds bound" {
  assert_true(image_case(0UL,1UL,1000000000UL,"camera",1UL,1UL,"mono8",1UL,b"x").validate(2) == 22)
}

test "Image refuses malformed frame names" {
  for frame in ["","/camera","camera/","camera//optical","camera-base","cam\u{0000}era","机器人"] {
    assert_true(image_case(0UL,1UL,2UL,frame,1UL,1UL,"mono8",1UL,b"x").validate(2) == 22)
  }
}

test "Image refuses unknown encoding" {
  assert_true(image_case(0UL,1UL,2UL,"camera",1UL,1UL,"jpeg",1UL,b"x").validate(2) == 22)
}

test "Image refuses short stride and incorrect body length" {
  assert_true(image_case(0UL,1UL,2UL,"camera",1UL,1UL,"rgb8",2UL,b"xx").validate(2) == 22)
  assert_true(image_case(0UL,1UL,2UL,"camera",1UL,2UL,"mono8",1UL,b"x").validate(2) == 22)
}

test "Image zero and oversized dimensions cannot wrap" {
  for n in [0UL,4294967296UL,18446744073709551615UL] { assert_true(image_case(0UL,1UL,2UL,"camera",n,1UL,"mono8",1UL,b"x").validate(2) == 22) }
  assert_true(image_case(0UL,1UL,2UL,"camera",4294967295UL,4294967295UL,"bgra8",4294967295UL,b"x").validate(2) == 22)
}

test "Image all truncated prefixes refuse without trap" {
  let bytes = b"\x00\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x0E\x00\x00\x00\x63\x61\x6D\x65\x72\x61\x2F\x6F\x70\x74\x69\x63\x61\x6C\x02\x00\x00\x00\x02\x00\x00\x00\x04\x00\x00\x00\x72\x67\x62\x38\x00\x08\x00\x00\x00\x10\x00\x00\x00\xFF\x00\x00\x00\xFF\x00\xAA\xBB\x00\x00\xFF\xFF\xFF\xFF\xCC\xDD"
  for n = 0; n < bytes.length(); n = n + 1 { assert_true(read_image(bytes[:n].to_owned(),2) is Err(_)) }
}

test "Image trailing data refused" {
  assert_true(read_image(b"\x00\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x0E\x00\x00\x00\x63\x61\x6D\x65\x72\x61\x2F\x6F\x70\x74\x69\x63\x61\x6C\x02\x00\x00\x00\x02\x00\x00\x00\x04\x00\x00\x00\x72\x67\x62\x38\x00\x08\x00\x00\x00\x10\x00\x00\x00\xFF\x00\x00\x00\xFF\x00\xAA\xBB\x00\x00\xFF\xFF\xFF\xFF\xCC\xDD" + b"x",2) == Err(22))
}

test "Image forged u32 frame length refused before narrowing" {
  let bytes = b"\x00\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x0E\x00\x00\x00\x63\x61\x6D\x65\x72\x61\x2F\x6F\x70\x74\x69\x63\x61\x6C\x02\x00\x00\x00\x02\x00\x00\x00\x04\x00\x00\x00\x72\x67\x62\x38\x00\x08\x00\x00\x00\x10\x00\x00\x00\xFF\x00\x00\x00\xFF\x00\xAA\xBB\x00\x00\xFF\xFF\xFF\xFF\xCC\xDD"
  let forged = Bytes::makei(bytes.length(), fn(i) { if i >= 12 && i < 16 { (255).to_byte() } else { bytes[i] } })
  assert_true(read_image(forged,2) == Err(22))
}

test "Image invalid endian flag refused" {
  let bytes = b"\x00\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x0E\x00\x00\x00\x63\x61\x6D\x65\x72\x61\x2F\x6F\x70\x74\x69\x63\x61\x6C\x02\x00\x00\x00\x02\x00\x00\x00\x04\x00\x00\x00\x72\x67\x62\x38\x00\x08\x00\x00\x00\x10\x00\x00\x00\xFF\x00\x00\x00\xFF\x00\xAA\xBB\x00\x00\xFF\xFF\xFF\xFF\xCC\xDD"
  let forged = Bytes::makei(bytes.length(), fn(i) { if i == 46 { (2).to_byte() } else { bytes[i] } })
  assert_true(read_image(forged,2) == Err(22))
}

test "Image forged encoding and pixel lengths refused" {
  let bytes = b"\x00\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x0E\x00\x00\x00\x63\x61\x6D\x65\x72\x61\x2F\x6F\x70\x74\x69\x63\x61\x6C\x02\x00\x00\x00\x02\x00\x00\x00\x04\x00\x00\x00\x72\x67\x62\x38\x00\x08\x00\x00\x00\x10\x00\x00\x00\xFF\x00\x00\x00\xFF\x00\xAA\xBB\x00\x00\xFF\xFF\xFF\xFF\xCC\xDD"
  for offset in [38, 51] {
    let forged = Bytes::makei(bytes.length(), fn(i) { if i >= offset && i < offset + 4 { (255).to_byte() } else { bytes[i] } })
    assert_true(read_image(forged,2) == Err(22))
  }
}

test "Image canonical input budget is enforced" {
  assert_true(read_image(Bytes::make(4193787,0),2) == Err(90))
  assert_true(image_case(0UL,1UL,2UL,"camera",4193741UL,1UL,"mono8",4193741UL,Bytes::make(4193741,0)).encode(2) == Err(90))
}

test "Image mono8 strips padding without mutating input" {
  let image = image_case(0UL,1UL,2UL,"camera",1UL,2UL,"mono8",2UL,b"\x01\xEE\x02\xDD")
  match image.to_mono8(2) { Ok(value) => assert_true(value.data == b"\x01\x02" && image.data == b"\x01\xEE\x02\xDD"); Err(_) => assert_true(false) }
}


fn tf_case(sequence : UInt64, seconds : UInt64, nanoseconds : UInt64,
           parent : String, child : String, values : Array[Double]) -> TransformStamped {
  { sequence, seconds, nanoseconds, parent_frame: parent, child_frame: child,
    translation: (values[0], values[1], values[2]),
    quaternion: (values[3], values[4], values[5], values[6]) }
}

fn tf_value(sequence : UInt64) -> TransformStamped {
  tf_case(sequence, 2UL, 3UL, "map", "base_link", [1.25, -2.5, 3.0, 0.0, 0.0, 0.0, 1.0])
}

fn tf_bytes() -> Bytes {
  b"\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x03\x00\x00\x00map\x09\x00\x00\x00base_link\x00\x00\x00\x00\x00\x00\xF4\x3F\x00\x00\x00\x00\x00\x00\x04\xC0\x00\x00\x00\x00\x00\x00\x08\x40\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xF0\x3F"
}

test "Transform ROS1 exact canonical bytes" {
  assert_true(tf_value(1UL).encode(1) == Ok(tf_bytes()))
  assert_true(read_transform(tf_bytes(), 1) == Ok(tf_value(1UL)))
}

test "Transform ROS2 exact complete round trip" {
  let value = tf_value(0UL)
  match value.encode(2) { Ok(bytes) => assert_true(read_transform(bytes, 2) == Ok(value)); Err(_) => assert_true(false) }
}

test "Transform signed zero bit is preserved" {
  let value = tf_case(0UL, 2UL, 3UL, "map", "base_link", [-0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0])
  let bytes = match value.encode(2) { Ok(v) => v; Err(_) => { assert_true(false); b"" } }
  match read_transform(bytes,2) { Ok(v) => assert_true(v.translation.0.reinterpret_as_uint64() == 9223372036854775808UL && v.quaternion.3 == -1.0); Err(_) => assert_true(false) }
}

test "Transform last valid timestamp" {
  for backend in [1, 2] {
    let value = tf_case(0UL,2147483647UL,999999999UL,"map","base_link",[0.0,0.0,0.0,0.0,0.0,0.0,1.0])
    assert_true(value.validate(backend) == 0)
    match value.encode(backend) { Ok(bytes) => assert_true(read_transform(bytes,backend) == Ok(value)); Err(_) => assert_true(false) }
  }
}

test "Transform ROS1 sequence preserves u32 and ROS2 rejects loss" {
  let value = tf_value(4294967295UL)
  assert_true(value.validate(1) == 0 && value.validate(2) == 22 && tf_value(1UL).validate(2) == 22)
  match value.encode(1) { Ok(bytes) => assert_true(read_transform(bytes,1) == Ok(value) && read_transform(bytes,2) == Err(22)); Err(_) => assert_true(false) }
}

test "Transform oversized sequence cannot narrow" {
  for sequence in [4294967296UL,18446744073709551615UL] { assert_true(tf_value(sequence).encode(1) == Err(22)) }
}

test "Transform both backends retain Host signed seconds profile" {
  for backend in [1,2] {
    for seconds in [2147483648UL,4294967295UL,18446744073709551615UL] {
      assert_true(tf_case(0UL,seconds,0UL,"map","base_link",[0.0,0.0,0.0,0.0,0.0,0.0,1.0]).validate(backend) == 22)
    }
  }
}

test "Transform timestamp and backend admission" {
  for nanos in [1000000000UL,18446744073709551615UL] { assert_true(tf_case(0UL,1UL,nanos,"map","base_link",[0.0,0.0,0.0,0.0,0.0,0.0,1.0]).validate(1) == 22) }
  assert_true(tf_value(0UL).validate(0) == 22 && tf_value(0UL).encode(3) == Err(22) && read_transform(tf_bytes(),0) == Err(22))
}

test "Transform strict frame segment grammar" {
  for frame in ["","/map","map/","map//base","1map","map/1base","map-base","map.base","ma p","map\u{0000}","地图"] {
    assert_true(tf_case(0UL,1UL,0UL,frame,"base_link",[0.0,0.0,0.0,0.0,0.0,0.0,1.0]).validate(2) == 22)
    assert_true(tf_case(0UL,1UL,0UL,"map",frame,[0.0,0.0,0.0,0.0,0.0,0.0,1.0]).validate(2) == 22)
  }
  assert_true(tf_case(0UL,1UL,0UL,"map2/_optical0","_link0",[0.0,0.0,0.0,0.0,0.0,0.0,1.0]).validate(2) == 0)
}

test "Transform same frame refused" {
  assert_true(tf_case(0UL,1UL,0UL,"map","map",[0.0,0.0,0.0,0.0,0.0,0.0,1.0]).validate(2) == 22)
}

test "Transform frame byte length maximum" {
  let frame = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
  assert_true(frame.length() == 255)
  let value = tf_case(0UL,1UL,0UL,frame,frame + "b",[0.0,0.0,0.0,0.0,0.0,0.0,1.0])
  assert_true(value.validate(2) == 22)
  let valid = tf_case(0UL,1UL,0UL,frame,"base_link",[0.0,0.0,0.0,0.0,0.0,0.0,1.0])
  match valid.encode(2) { Ok(bytes) => assert_true(read_transform(bytes,2) == Ok(valid)); Err(_) => assert_true(false) }
}

test "Transform every scalar rejects NaN and infinities" {
  for n = 0; n < 7; n = n + 1 {
    for invalid in [0.0 / 0.0, 1.0 / 0.0, -1.0 / 0.0] {
      let values = [1.25,-2.5,3.0,0.0,0.0,0.0,1.0]; values[n] = invalid
      assert_true(tf_case(0UL,2UL,3UL,"map","base_link",values).encode(2) == Err(22))
    }
  }
}

test "Transform zero nonunit and overflowing quaternion refused" {
  for values in [[0.0,0.0,0.0,0.0,0.0,0.0,0.0], [0.0,0.0,0.0,0.0,0.0,0.0,2.0], [0.0,0.0,0.0,1.0e308,0.0,0.0,0.0]] {
    assert_true(tf_case(0UL,1UL,0UL,"map","base_link",values).validate(2) == 22)
  }
}

test "Transform baseline norm tolerance without normalization" {
  let accepted = tf_case(0UL,1UL,0UL,"map","base_link",[0.0,0.0,0.0,0.0,0.0,0.0,1.00025])
  assert_true(accepted.validate(2) == 0)
  match accepted.encode(2) { Ok(bytes) => assert_true(read_transform(bytes,2) == Ok(accepted)); Err(_) => assert_true(false) }
  assert_true(tf_case(0UL,1UL,0UL,"map","base_link",[0.0,0.0,0.0,0.0,0.0,0.0,1.001]).validate(2) == 22)
}

test "Transform all truncated prefixes refuse without trap" {
  let bytes = tf_bytes()
  for n = 0; n < bytes.length(); n = n + 1 { assert_true(read_transform(bytes[:n].to_owned(),1) == Err(22)) }
}

test "Transform exact extent no trailing bytes" {
  assert_true(read_transform(tf_bytes() + b"x",1) == Err(22))
  assert_true(read_transform(b"",1) == Err(22) && read_transform(Bytes::make(77,0),1) == Err(22) && read_transform(Bytes::make(587,0),1) == Err(22))
}

test "Transform forged and empty frame lengths refuse before narrowing" {
  let bytes = tf_bytes()
  for offset in [12,19] {
    for octet in [0,255] {
      let bad = Bytes::makei(bytes.length(), fn(i) { if i >= offset && i < offset + 4 { octet.to_byte() } else { bytes[i] } })
      assert_true(read_transform(bad,1) == Err(22))
    }
  }
}

test "Transform malformed frame bytes are strict" {
  let bytes = tf_bytes()
  for octet in [0,49,255] {
    for offset in [16,23] {
      let bad = Bytes::makei(bytes.length(),fn(i) { if i == offset { octet.to_byte() } else { bytes[i] } })
      assert_true(read_transform(bad,1) == Err(22))
    }
  }
}

test "Transform nonfinite wire scalars are refused" {
  let bytes = tf_bytes()
  for n = 0; n < 7; n = n + 1 {
    for bits in [9221120237041090560UL,9218868437227405312UL] {
      let raw = u64(bits)
      let bad = Bytes::makei(bytes.length(),fn(i) { if i >= 32 + n * 8 && i < 40 + n * 8 { raw[i - 32 - n * 8] } else { bytes[i] } })
      assert_true(read_transform(bad,1) == Err(22))
    }
  }
}

test "Transform quaternion sign and components preserved" {
  for values in [[1.0,2.0,3.0,0.0,0.0,0.0,-1.0], [1.0,2.0,3.0,0.6,0.0,0.0,0.8]] {
    let value = tf_case(0UL,1UL,0UL,"map","base_link",values)
    match value.encode(2) { Ok(bytes) => assert_true(read_transform(bytes,2) == Ok(value)); Err(_) => assert_true(false) }
  }
}
