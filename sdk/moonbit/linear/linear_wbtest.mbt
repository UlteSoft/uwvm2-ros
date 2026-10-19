// Internal linear-memory tests only. The closures below model exact raw status
// writes to test sentinel/copy rules. They are not a native ROS implementation
// and are never accepted as Guest/Gateway/authorization evidence.

test "negative capacity refuses before callback" {
  let state = { busy: false }
  let reply = perform(b"", -1, false, fn(_, _, _) { state.busy = true; 0 })
  assert_true(reply.code == 90 && !state.busy)
}

test "oversized capacity refuses before callback" {
  let state = { busy: false }
  let reply = perform(b"", 4194305, false, fn(_, _, _) { state.busy = true; 0 })
  assert_true(reply.code == 90 && !state.busy)
}

test "unknown length90 leaves required absent" {
  let reply = perform(b"", 16, false, fn(_, _, _) { 90 })
  assert_true(reply.code == 90 && reply.required == None && reply.data.length() == 0)
}

test "known length90 is diagnostic only" {
  let reply = perform(b"", 16, false, fn(_, _, _) { store_slot(32UL); 90 })
  assert_true(reply.code == 90 && reply.required == Some(32UL) && reply.data.length() == 0)
}

test "other errors never publish slot" {
  let reply = perform(b"", 16, false, fn(_, _, _) { store_slot(8UL); 110 })
  assert_true(reply.code == 110 && reply.required == None && reply.data.length() == 0)
}

test "success missing length is invalid" {
  assert_true(perform(b"", 16, false, fn(_, _, _) { 0 }).code == 22)
}

test "success beyond capacity is invalid" {
  assert_true(perform(b"", 16, false, fn(_, _, _) { store_slot(17UL); 0 }).code == 22)
}

test "fixed handle preserves bit63" {
  let reply = perform(b"", 8, true, fn(_, _, _) {
    for i = 0; i < 8; i = i + 1 { store_byte(4259840 + i, 255) }
    0
  })
  assert_true(reply.code == 0 && reply.data == b"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF")
}

test "nested arena call refused without second import" {
  let reply = perform(b"", 0, true, fn(_, _, _) {
    perform(b"", 0, true, fn(_, _, _) { 95 }).code
  })
  assert_true(reply.code == 17)
}

test "output is owned after reuse" {
  let first = perform(b"", 1, true, fn(_, _, _) { store_byte(4259840, 65); 0 })
  let second = perform(b"", 1, true, fn(_, _, _) { store_byte(4259840, 66); 0 })
  assert_true(first.data == b"A" && second.data == b"B")
}


test "admit zero success refuses before commit" {
  let state = { busy: false }
  let code = admit_u64(b"", false, fn(_, _, _) { 0 }, fn(_) { state.busy = true; 0 })
  assert_true(code == 22 && !state.busy)
}

test "admit goal requires actual length8" {
  let state = { busy: false }
  let code = admit_u64(b"", true, fn(_, _, _) { store_byte(4259840, 19); store_slot(7UL); 0 }, fn(_) { state.busy = true; 0 })
  assert_true(code == 22 && !state.busy)
}

test "admit preserves maximum unsigned handle" {
  let observed = { busy: false }
  let code = admit_u64(b"", false, fn(_, _, _) {
    for i = 0; i < 8; i = i + 1 { store_byte(4259840 + i, 255) }
    0
  }, fn(value) { observed.busy = value == 18446744073709551615UL; 0 })
  assert_true(code == 0 && observed.busy)
}

test "admit failure cannot commit stale output" {
  let observed = { busy: false }
  let code = admit_u64(b"", false, fn(_, _, _) { store_byte(4259840, 19); 13 }, fn(_) { observed.busy = true; 0 })
  assert_true(code == 13 && !observed.busy)
}
