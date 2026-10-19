// Future Wasm unit fixtures for actual pure wire/frame code. These closures
// write local memory0 test data; they are NOT Host or Native ROS operations.
// Run only with the admitted real arena/link layout and full target validator.
extern "wasm" fn fixture_store(address : Int, value : Int) -> Unit =
  #|(func (param i32 i32) (i32.store8 (local.get 0) (local.get 1)))

fn fixture_word(data : Array[Byte], first : Int, width : Int, value : UInt64) -> Unit {
  for index = 0; index < width; index = index + 1 {
    data[first + index] = ((value >> (index * 8)) & 255UL).to_int().to_byte()
  }
}

fn fixture_qos(depth : UInt64) -> Bytes {
  let data = Array::makei(80, fn(_) { (0).to_byte() })
  for at = 0; at < 16; at = at + 4 { fixture_word(data, at, 4, 1UL) }
  fixture_word(data, 16, 8, depth)
  Bytes::makei(80, fn(index) { data[index] })
}

fn fixture_receipt() -> Array[Byte] {
  let data = Array::makei(256, fn(_) { (0).to_byte() })
  fixture_word(data, 0, 4, 1163021653UL)
  fixture_word(data, 4, 2, 1UL)
  data[6] = (1).to_byte()
  data[7] = (2).to_byte()
  fixture_word(data, 8, 4, 256UL)
  data[12] = (3).to_byte()
  data[13] = (2).to_byte()
  data[14] = (32).to_byte()
  fixture_word(data, 16, 8, 4294967297UL)
  fixture_word(data, 24, 8, 9223372036854775809UL)
  fixture_word(data, 32, 8, 1UL)
  fixture_word(data, 40, 8, 2UL)
  fixture_word(data, 48, 8, 18446744073709551615UL)
  let qos = fixture_qos(1UL)
  for index = 0; index < 80; index = index + 1 {
    data[64 + index] = qos[index]
    data[144 + index] = qos[index]
  }
  data
}

fn fixture_frame(data : Array[Byte]) -> @linear.ReservedReply {
  let frame = match @linear.reserve_reply(data.length()) { Ok(value) => value; Err(_) => abort("frame preparation") }
  let code = frame.invoke(b"", fn(_, output) {
    for index = 0; index < data.length(); index = index + 1 {
      fixture_store(output.to_int() + index, data[index].to_int())
    }
    0
  })
  if code != 0 { abort("local fixture frame") }
  frame
}

test "genuine activation QoS mismatch keeps exact retire binding without live admission" {
  let frame = fixture_frame(fixture_receipt())
  let receipt = prepare_receipt(frame, 4294967297UL, 3)
  let code = admit_receipt(receipt, 1, [fixture_qos(2UL), fixture_qos(1UL)])
  assert_true(code == 17 && receipt.valid)
  assert_true(receipt.instance == 9223372036854775809UL && receipt.incarnation == 18446744073709551615UL)
  assert_true(receipt.source == 4294967297UL && receipt.session == 2UL && receipt.generation == 1UL)
}

test "unassociated or malformed receipt cannot mint native retire eligibility" {
  let wrong_owner = prepare_receipt(fixture_frame(fixture_receipt()), 8589934593UL, 3)
  assert_true(admit_receipt(wrong_owner, 1, [fixture_qos(1UL), fixture_qos(1UL)]) == 22)
  assert_true(!wrong_owner.valid)
  let bad = fixture_receipt()
  bad[56] = (1).to_byte()
  let reserved = prepare_receipt(fixture_frame(bad), 4294967297UL, 3)
  assert_true(admit_receipt(reserved, 1, [fixture_qos(1UL), fixture_qos(1UL)]) == 22)
  assert_true(!reserved.valid)
}

test "expected QoS domain uses actual80 not a guessed native enum cast" {
  assert_true(expected_qos_valid(fixture_qos(1UL)))
  assert_true(!expected_qos_valid(fixture_qos(0UL)))
  assert_true(!expected_qos_valid(b""))
  let qos = fixture_qos(1UL)
  let data = Array::makei(80, fn(index) { qos[index] })
  fixture_word(data, 32, 8, 1000000000UL)
  assert_true(!expected_qos_valid(Bytes::makei(80, fn(index) { data[index] })))
}

test "retained frame guard damage preserves raw0 as unknown107 without publishing scratch" {
  let frame = match @linear.reserve_reply(8) { Ok(value) => value; Err(_) => abort("frame preparation") }
  let code = frame.invoke(b"", fn(_, output) {
    fixture_store(output.to_int() + 8, 0)
    0
  })
  assert_true(code == 107 && frame.raw_code() == 0 && frame.attempted() && frame.completed())
  assert_true(frame.byte_at(0) == -1)
  assert_true(frame.invoke(b"", fn(_, _) { abort("replayed local effect") }) == 17)
}
