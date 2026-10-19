// All 38/43 imports remain reachable only through this explicitly exported
// inspection probe. Do not execute valid selector indexes.
pub fn abi_coverage(index : Int) -> Int { @ros.abi_coverage(index) }

// Four separately authorized aliases; empty selection performs no opens.
// Only the explicit own-goal cancel role is requested, never cancel-any.
const SEND : String = ""
const FEEDBACK : String = ""
const RESULT : String = ""
const CANCEL : String = ""
fn main {
  let session = match @ros.new_session() { Ok(value) => value; Err(_) => abort("session") }
  let node = match session.node("moonbit-action") { Ok(value) => value; Err(_) => abort("node") }
  let mut status = 0
  if SEND.length() != 0 {
    match node.action_client(SEND, FEEDBACK, RESULT, CANCEL) {
      Err(code) => status = code
      Ok(client) => match @wire.fibonacci_goal(6) {
        Err(code) => status = code
        Ok(goal) => match client.send_goal(goal) {
          Err(code) => status = code
          Ok(id) => {
            // One bounded cancellation request; completion/result needs a later
            // explicit poll selected by the application, never an infinite loop.
            let reply = client.cancel_own(id)
            status = reply.code
            if status == 0 {
              match @wire.read_bool(reply.data) { Ok(_) => (); Err(code) => status = code }
            }
          }
        }
      }
    }
  }
  let cleanup = session.shutdown()
  if status != 0 || cleanup.code != 0 { abort("action or cleanup failure") }
  @stdio.line_ascii("PASS_MOONBIT_EXAMPLE_CHECKED_CLEANUP_04_ACTION_ROS2")
}
