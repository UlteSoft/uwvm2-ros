// Pure local-state contracts only. Synthetic scalar IDs below never reach a
// host import. Busy/revoked/closed preconditions must reject BEFORE that call.
// These are NOT fake transports and are NOT native ROS/Guest acceptance proof.

fn fixture_session() -> Session {
  { closed: false, leased: false, retiring: false, resources: [], last_cleanup: None, cleanup_reports: [] }
}

fn fixture_owner(session : Session, alias : String, handle : UInt64, closed : Bool) -> Resource {
  let owner : Resource = { session, operation: TopicSubscribe, alias, handle,
    closed, revoked: false, cleanup_attempted: false, cleanup_completed: false,
    cleanup_code: 0, cleanup_kind: 0 }
  session.resources.push(owner)
  owner
}

test "lease close refuses before scalar retirement" {
  let session = fixture_session()
  let owner = fixture_owner(session, "queue", 19UL, false)
  session.leased = true
  assert_true(owner.close() == 17 && !owner.closed && !owner.cleanup_attempted)
}

test "lease revoke refuses before every sibling changes" {
  let session = fixture_session()
  let first = fixture_owner(session, "same", 19UL, false)
  let second = fixture_owner(session, "same", 20UL, false)
  session.leased = true
  assert_true(first.revoke() == 17 && !first.closed && !second.closed)
}

test "shutdown checks whole group before any mutation" {
  let session = fixture_session()
  let first = fixture_owner(session, "one", 19UL, false)
  let second = fixture_owner(session, "two", 20UL, false)
  session.leased = true
  let report = session.shutdown()
  assert_true(report.code == 17 && !session.closed && !first.closed && !second.closed)
}

test "alias invalidation is complete and leaves unrelated owner live" {
  let session = fixture_session()
  let first = fixture_owner(session, "same", 19UL, false)
  let second = fixture_owner(session, "same", 20UL, false)
  let other = fixture_owner(session, "other", 21UL, false)
  first.invalidate_alias()
  assert_true(first.closed && second.closed && first.revoked && second.revoked)
  assert_true(!other.closed && !other.revoked)
}

test "revoked alias cannot reserve fresh open" {
  let session = fixture_session()
  let owner = fixture_owner(session, "same", 19UL, true)
  owner.revoked = true
  let node : Node = { session, label: "local" }
  match node.prepare_open("same", TopicPublish) {
    Err(code) => assert_true(code == 13 && session.resources.length() == 1)
    Ok(_) => assert_true(false)
  }
}

test "pending owner registered before any open effect" {
  let session = fixture_session()
  let node : Node = { session, label: "local" }
  match node.prepare_open("pending", TopicPublish) {
    Err(_) => assert_true(false)
    Ok(value) => assert_true(value.owner.closed && value.owner.handle == 0UL && session.resources.length() == 1)
  }
}

test "reservation refuses busy session without ledger mutation" {
  assert_true(!distinct_aliases(["same", "same"]))
  assert_true(!distinct_aliases(["", "other"]))
  assert_true(distinct_aliases(["send", "feedback", "result", "cancel"]))
  let session = fixture_session()
  session.leased = true
  let node : Node = { session, label: "local" }
  match node.prepare_open("pending", TopicPublish) {
    Err(code) => assert_true(code == 17 && session.resources.length() == 0)
    Ok(_) => assert_true(false)
  }
}

test "shutdown retired slots report reverse acquisition order" {
  let session = fixture_session()
  let _ = fixture_owner(session, "first", 19UL, true)
  let _ = fixture_owner(session, "last", 20UL, true)
  let report = session.shutdown()
  assert_true(report.code == 0 && report.outcomes.length() == 2)
  assert_true(report.outcomes[0].alias == "last" && report.outcomes[1].alias == "first")
  assert_true(!report.outcomes[0].attempted && report.outcomes[0].completed)
}

test "cleanup reservation independent outcome slots" {
  let session = fixture_session()
  let first = fixture_owner(session, "first", 19UL, true)
  let last = fixture_owner(session, "last", 20UL, true)
  let report = reserve_cleanup([first, last], 110)
  report.outcomes[0].code = 5
  report.outcomes[1].code = 9
  assert_true(report.code == 110 && report.outcomes[0].code == 5 && report.outcomes[1].code == 9)
}

test "executor upfront invalid tail prevents head poll" {
  let session = fixture_session()
  let first = fixture_owner(session, "first", 19UL, false)
  let last = fixture_owner(session, "last", 20UL, true)
  let executor : Executor = { entries: [(first, fn(_) { 95 }), (last, fn(_) { 95 })], capacity: 0, busy: false, closed: false }
  let result = executor.spin_once()
  assert_true(result.code == 9 && result.delivered == 0 && !session.leased && !executor.busy)
}

test "executor busy session upfront leaves all other sessions unleased" {
  let one = fixture_session()
  let two = fixture_session()
  let first = fixture_owner(one, "first", 19UL, false)
  let last = fixture_owner(two, "last", 20UL, false)
  two.leased = true
  let executor : Executor = { entries: [(first, fn(_) { 95 }), (last, fn(_) { 95 })], capacity: 0, busy: false, closed: false }
  let result = executor.spin_once()
  assert_true(result.code == 17 && !one.leased && two.leased && !executor.busy)
}

test "closed resource never reaches actual close" {
  let session = fixture_session()
  let owner = fixture_owner(session, "closed", 19UL, true)
  assert_true(owner.close() == 9 && !owner.cleanup_attempted)
}


test "prior failed cleanup remains visible without retry" {
  let session = fixture_session()
  let owner = fixture_owner(session, "prior", 19UL, true)
  owner.cleanup_attempted = true
  owner.cleanup_completed = true
  owner.cleanup_code = 110
  owner.cleanup_kind = 1
  let report = session.shutdown()
  assert_true(report.code == 110 && report.outcomes[0].previous)
  assert_true(!report.outcomes[0].attempted && report.outcomes[0].completed)
  assert_true(session.cleanup_history().length() == 1 && report.started)
}


fn tf_owner(operation : Operation, closed : Bool, revoked : Bool) -> Resource {
  let session = fixture_session()
  { session, operation, alias: "model-tf", handle: 19UL, closed, revoked,
    cleanup_attempted: false, cleanup_completed: false, cleanup_code: 0, cleanup_kind: 0 }
}

fn tf_fixture() -> @wire.TransformStamped {
  { sequence: 0UL, seconds: 1UL, nanoseconds: 2UL, parent_frame: "map", child_frame: "base_link",
    translation: (1.0,2.0,3.0), quaternion: (0.0,0.0,0.0,1.0) }
}

test "typed TF wrong operation cannot invoke an import" {
  let owner = tf_owner(TopicSubscribe,false,false)
  assert_true(owner.broadcast_transform(tf_fixture()) == 95 && owner.lookup_transform(0UL,586) == Err(95))
}

test "typed TF closed owners cannot invoke an import" {
  assert_true(tf_owner(TfBroadcast,true,false).broadcast_transform(tf_fixture()) == 9)
  assert_true(tf_owner(TfLookup,true,false).lookup_transform(0UL,586) == Err(9))
}

test "typed TF revoked closed owners cannot invoke an import" {
  assert_true(tf_owner(TfBroadcast,true,true).broadcast_transform(tf_fixture()) == 9)
  assert_true(tf_owner(TfLookup,true,true).lookup_transform(0UL,586) == Err(9))
}

test "typed TF malformed input and invalid capacity refuse before import" {
  let value : @wire.TransformStamped = { sequence: 0UL, seconds: 1UL, nanoseconds: 2UL,
    parent_frame: "map", child_frame: "map", translation: (0.0,0.0,0.0), quaternion: (0.0,0.0,0.0,1.0) }
  assert_true(tf_owner(TfBroadcast,false,false).broadcast_transform(value) == 22)
  assert_true(tf_owner(TfLookup,false,false).lookup_transform(0UL,-1) == Err(90))
  assert_true(tf_owner(TfLookup,false,false).lookup_transform(0UL,4194305) == Err(90))
}
