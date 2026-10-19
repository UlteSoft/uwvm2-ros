// SOURCE ONLY: compile later against the ACTUAL generated maintained managed
// fixture catalog. No ROS backend, synthetic grant or transport model.
fn scalar() -> @records.Ros2Managed_fixtureMsgScalars {
  let value = @records.Ros2Managed_fixtureMsgScalars::new()
  value.flag = true
  value.u8 = 255
  value.i8 = -128
  value.u16 = 65535
  value.i16 = -32768
  value.u32 = 4294967295U
  value.i32 = -2147483647 - 1
  value.u64 = 18446744073709551615UL
  value.i64 = -9223372036854775807L - 1L
  value.f32 = (2147483648U).reinterpret_as_float()
  value.f64 = 1.25
  value.text = "π"
  value
}

test "complete independent49-byte scalar golden, signed zero and all u64 bits" {
  let codec = @records.Ros2Managed_fixtureMsgScalarsCodec::new()
  let golden = b"\x01\xff\x80\xff\xff\x00\x80\xff\xff\xff\xff\x00\x00\x00\x80\xff\xff\xff\xff\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x00\x80\x00\x00\x00\x80\x00\x00\x00\x00\x00\x00\xf4\x3f\x02\x00\x00\x00\xcf\x80"
  match codec.encode(scalar()) { Ok(wire) => assert_true(wire == golden); Err(_) => assert_true(false) }
  match codec.decode(golden) {
    Ok(value) => {
      assert_true(value.u8 == 255 && value.i8 == -128 && value.u16 == 65535 && value.i16 == -32768)
      assert_true(value.u32 == 4294967295U && value.i32 == -2147483647 - 1)
      assert_true(value.u64 == 18446744073709551615UL && value.i64 == -9223372036854775807L - 1L)
      assert_true(value.f32.reinterpret_as_uint() == 2147483648U && value.f64 == 1.25 && value.text == "π")
    }
    Err(_) => assert_true(false)
  }
}

test "small integers cannot silently narrow and nonfinite values are refused" {
  let codec = @records.Ros2Managed_fixtureMsgScalarsCodec::new()
  let value = scalar()
  value.u8 = 256
  assert_true(codec.encode(value) is Err(_))
  value.u8 = 0
  value.i8 = -129
  assert_true(codec.encode(value) is Err(_))
  value.i8 = 0
  value.u16 = -1
  assert_true(codec.encode(value) is Err(_))
  value.u16 = 0
  value.i16 = 32768
  assert_true(codec.encode(value) is Err(_))
  value.i16 = 0
  value.f32 = (2139095040U).reinterpret_as_float()
  assert_true(codec.encode(value) is Err(_))
  value.f32 = (0U).reinterpret_as_float()
  value.f64 = (9221120237041090560UL).reinterpret_as_double()
  assert_true(codec.encode(value) is Err(_))
}

test "UTF8 bound NUL refusal BOM preservation exact end and transactional input" {
  let codec = @records.Ros2Managed_fixtureMsgScalarsCodec::new()
  let value = scalar()
  value.text = "🙂🙂🙂🙂"
  let wire = match codec.encode(value) { Ok(bytes) => bytes; Err(_) => abort("encode") }
  let original = wire.copy()
  match codec.decode(wire) { Ok(decoded) => assert_true(decoded.text == value.text); Err(_) => assert_true(false) }
  assert_true(wire == original)
  value.text = "🙂🙂🙂🙂🙂"
  assert_true(codec.encode(value) is Err(_))
  value.text = "bad\u{0000}text"
  assert_true(codec.encode(value) is Err(_))
  value.text = "\u{FEFF}"
  match codec.encode(value) {
    Ok(bytes) => match codec.decode(bytes) { Ok(decoded) => assert_true(decoded.text == value.text); Err(_) => assert_true(false) }
    Err(_) => assert_true(false)
  }
  assert_true(codec.decode(wire[:wire.length()-1].to_owned()) is Err(_))
  assert_true(codec.decode(wire + b"\x00") is Err(_))
  let bad_bool = Bytes::makei(wire.length(), fn(index) { if index == 0 { (2).to_byte() } else { wire[index] } })
  assert_true(codec.decode(bad_bool) is Err(_))
  // Independent scalar prefix is43 bytes; this is the following u32 byte
  // count, not a decoded managed length. A huge count must fail before copy.
  let bad_count = Bytes::makei(wire.length(), fn(index) {
    if index >= 43 && index < 47 { (255).to_byte() } else { wire[index] }
  })
  assert_true(codec.decode(bad_count) is Err(_))
  let bad_utf8 = Bytes::makei(wire.length(), fn(index) {
    if index == wire.length()-2 { (192).to_byte() }
    else if index == wire.length()-1 { (128).to_byte() }
    else { wire[index] }
  })
  assert_true(codec.decode(bad_utf8) is Err(_))
}

test "actual nested fixed sequence and independent recursive defaults" {
  let codec = @records.Ros2Managed_fixtureMsgOuterCodec::new()
  let value = @records.Ros2Managed_fixtureMsgOuter::new()
  let other = @records.Ros2Managed_fixtureMsgOuter::new()
  assert_true(value.rotation.w == 1.0 && value.pose.orientation.w == 1.0 &&
    value.rotations[0].w == 1.0 && value.rotations[1].w == 1.0 && value.dynamic.length() == 0)
  value.inner.scalar = scalar()
  value.fixed[0].tags = ["first", "π"]
  value.fixed[1].pair = [3.0, -2.0]
  value.items = [@records.Ros2Managed_fixtureMsgInner::new()]
  value.items[0].scalar.i64 = -7L
  value.rotations[0].w = 0.0
  assert_true(value.rotations[1].w == 1.0 && other.rotations[0].w == 1.0 && other.fixed[0].pair[0] == 0.0)
  match codec.encode(value) {
    Ok(bytes) => match codec.decode(bytes) {
      Ok(decoded) => assert_true(decoded.inner.scalar.u64 == 18446744073709551615UL &&
        decoded.fixed[0].tags[1] == "π" && decoded.fixed[1].pair[1] == -2.0 &&
        decoded.items[0].scalar.i64 == -7L && decoded.rotations[0].w == 0.0)
      Err(_) => assert_true(false)
    }
    Err(_) => assert_true(false)
  }
  value.items = Array::makei(3, fn(_) { @records.Ros2Managed_fixtureMsgInner::new() })
  assert_true(codec.encode(value) is Err(_)) // actual declared canonical cap2
  value.items = []
  value.fixed = []
  assert_true(codec.encode(value) is Err(_))
}

test "complete empty Service request is zero bytes, not a dummy field" {
  let codec = @records.Ros2Managed_fixtureSrvProbe_RequestCodec::new()
  let value = @records.Ros2Managed_fixtureSrvProbe_Request::new()
  match codec.encode(value) { Ok(bytes) => assert_true(bytes == b""); Err(_) => assert_true(false) }
  assert_true(codec.decode(b"\x00") is Err(_))
  match codec.decode(b"") { Ok(_) => (); Err(_) => assert_true(false) }
}
