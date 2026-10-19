// All 38/43 imports remain reachable only through this explicitly exported
// inspection probe. Do not execute valid selector indexes.
pub fn abi_coverage(index : Int) -> Int { @ros.abi_coverage(index) }

// There is intentionally no default alias. The owner must review and edit
// exactly this source selection plus a bounded topic.publish grant before build.
const SELECTED_ALIAS : String = ""
fn main {
  let session = match @ros.new_session() { Ok(value) => value; Err(_) => abort("session") }
  let node = match session.node("moonbit-topic") { Ok(value) => value; Err(_) => abort("node") }
  let mut status = 0
  if SELECTED_ALIAS.length() != 0 {
    match node.publisher(SELECTED_ALIAS) {
      Err(code) => status = code
      Ok(owner) => match @wire.string_bytes("bounded MoonBit telemetry") {
        Err(code) => status = code
        Ok(payload) => status = owner.write(payload)
      }
    }
  }
  let cleanup = session.shutdown()
  if status != 0 || cleanup.code != 0 { abort("topic or cleanup failure") }
  @stdio.line_ascii("PASS_MOONBIT_EXAMPLE_CHECKED_CLEANUP_02_TOPIC_ROS1")
}
