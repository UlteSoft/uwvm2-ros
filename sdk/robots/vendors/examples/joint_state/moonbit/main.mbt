// Stage as guest/main.mbt with one exact backend observer and mode source.
// Actual MoonBit Guest SOURCE_ONLY_UNEXECUTED; never a host callback model.
struct Diagnostics {
  mut primary : Int
  mut cleanup : Int
  mut received : Int
  mut denied : Bool
  mut session : @ros.Session?
}
let diagnostics : Diagnostics = {
  primary: 0, cleanup: 0, received: 0, denied: false, session: None,
}

fn positive(node : @ros.Node) -> Int {
  let executor = match @ros.new_executor(selected_max_wire()) {
    Ok(value) => value
    Err(code) => return code
  }
  let mut status = 0
  match node.subscription("joint-state") {
    Err(code) => status = code
    Ok(source) => {
      status = executor.add(source, decode_observe)
      if status == 0 {
        for credit = 0; credit < 64 && diagnostics.received == 0; credit = credit + 1 {
          status = @stdio.read_credit()
          if status != 0 { break }
          let spin = executor.spin_once()
          // Actual poll11 becomes no delivery/code0. Callback11 is FAILURE.
          if spin.code != 0 { status = spin.code; break }
        }
        if status == 0 && diagnostics.received != 1 { status = 110 }
      }
    }
  }
  let closed = executor.close()
  if status == 0 { status = closed }
  status
}

fn main {
  if MODE != 0 && MODE != 1 { abort("explicit fresh mode") }
  let maximum = selected_max_wire()
  if maximum < 1 || maximum > 32768 { abort("actual full codec bound") }
  // Actual new_session checks raw ABI1 and the existing real linear arena.
  let session = match @ros.new_session() { Ok(value) => value; Err(_) => abort("ABI/arena") }
  diagnostics.session = Some(session) // retained before any alias acquisition
  match session.node("joint-state-moonbit") {
    Err(code) => diagnostics.primary = code
    Ok(node) => {
      if MODE == 0 {
        // A genuine denied open only; no fake grant, stub or wrong-memory call.
        match node.open("joint-state", @ros.TopicSubscribe) {
          Err(code) => {
            if code == 13 { diagnostics.denied = true }
            else { diagnostics.primary = code }
          }
          Ok(_) => diagnostics.primary = 22 // actual owner stays in Session
        }
      } else {
        diagnostics.primary = positive(node)
      }
    }
  }
  let cleanup = session.shutdown()
  diagnostics.cleanup = cleanup.code
  if !cleanup.started { abort("cleanup was not performed") }
  for outcome in cleanup.outcomes {
    if !outcome.completed || outcome.code != 0 { abort("actual close/previous cleanup failure") }
    if MODE == 0 && outcome.handle != 0UL { abort("denied case acquired handle") }
  }
  // Primary and cleanup remain distinct in the retained actual Session ledger.
  if diagnostics.primary != 0 || diagnostics.cleanup != 0 { abort("application/cleanup failure") }
  if MODE == 0 {
    if !diagnostics.denied || diagnostics.received != 0 { abort("actual open13 required") }
    @stdio.line_ascii("PASS_JOINT_STATE_DEFAULT_DENIED")
  } else {
    if diagnostics.received != 1 { abort("one complete observer fixture required") }
    @stdio.line_ascii("PASS_JOINT_STATE_FULL_VALUE")
  }
}
