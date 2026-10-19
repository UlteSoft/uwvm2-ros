// Complete raw ABI inspection only. Never invoke a valid probe selector.
pub fn abi_coverage(index : Int) -> Int { @ros.abi_coverage(index) }

fn main {
  let session = match @ros.new_session() { Ok(value) => value; Err(_) => abort("session") }
  let node = match session.node("moonbit-telemetry") { Ok(value) => value; Err(_) => abort("node") }
  // The real Host, not a local label, must refuse this independently ungranted
  // alias. No policy or stronger grant is inferred by the SDK.
  match node.publisher("ungranted-telemetry") {
    Err(code) => if code != 13 { abort("expected permission denied13") }
    Ok(owner) => { let _ = owner.close(); abort("unexpected ungranted owner") }
  }
  let publisher = match node.publisher("pub") {
    Ok(value) => value
    Err(_) => abort("explicit pub grant required")
  }
  let warmup = match @wire.string_bytes("moonbit-telemetry-ready") {
    Ok(value) => value
    Err(_) => abort("warmup codec")
  }
  if publisher.write(warmup) != 0 { abort("warmup publication") }
  @stdio.line_ascii("READY_MOONBIT_NATIVE_TELEMETRY")
  @stdio.await_publish_gate()
  let payload = match @wire.string_bytes("bounded MoonBit telemetry") {
    Ok(value) => value
    Err(_) => abort("telemetry codec")
  }
  if publisher.write(payload) != 0 { abort("telemetry publication") }
  @stdio.line_ascii("AWAIT_NATIVE_TELEMETRY_DELIVERY")
  @stdio.await_delivery_gate()
  if publisher.close() != 0 { abort("explicit publisher close") }
  let cleanup = session.shutdown()
  if cleanup.code != 0 { abort("session cleanup") }
  @stdio.line_ascii("PASS_MOONBIT_NATIVE_TELEMETRY")
}
