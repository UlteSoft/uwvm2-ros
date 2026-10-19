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
