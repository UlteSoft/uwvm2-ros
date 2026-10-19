// All 38/43 imports remain reachable only through this explicitly exported
// inspection probe. Do not execute valid selector indexes.
pub fn abi_coverage(index : Int) -> Int { @ros.abi_coverage(index) }

// Trigger request is empty, response is Bool+String. This example never
// supplies a topic name as an alias and never repeats an admitted timeout.
const SELECTED_ALIAS : String = ""
fn main {
  let session = match @ros.new_session() { Ok(value) => value; Err(_) => abort("session") }
  let node = match session.node("moonbit-service") { Ok(value) => value; Err(_) => abort("node") }
  let mut status = 0
  if SELECTED_ALIAS.length() != 0 {
    match node.client(SELECTED_ALIAS) {
      Err(code) => status = code
      Ok(owner) => {
        let reply = owner.request(b"", 4096)
        status = reply.code
        if status == 0 {
          match @wire.read_service_response(reply.data) { Ok(_) => (); Err(code) => status = code }
        }
      }
    }
  }
  let cleanup = session.shutdown()
  if status != 0 || cleanup.code != 0 { abort("service or cleanup failure") }
  @stdio.line_ascii("PASS_MOONBIT_EXAMPLE_CHECKED_CLEANUP_03_SERVICE_ROS2")
}
