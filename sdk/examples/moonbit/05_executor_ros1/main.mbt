// All 38/43 imports remain reachable only through this explicitly exported
// inspection probe. Do not execute valid selector indexes.
pub fn abi_coverage(index : Int) -> Int { @ros.abi_coverage(index) }

// One explicit pass with at most64 entries; callbacks run on this Guest
// thread after copied receipt, not on a native ROS thread. Default is no opens.
const SELECTED_ALIAS : String = ""
fn main {
  let session = match @ros.new_session() { Ok(value) => value; Err(_) => abort("session") }
  let node = match session.node("moonbit-executor") { Ok(value) => value; Err(_) => abort("node") }
  let executor = match @ros.new_executor(4096) { Ok(value) => value; Err(_) => abort("executor") }
  let mut status = 0
  if SELECTED_ALIAS.length() != 0 {
    match node.subscription(SELECTED_ALIAS) {
      Err(code) => status = code
      Ok(source) => status = executor.add(source, fn(bytes) {
        match @wire.read_string(bytes) { Ok(_) => 0; Err(code) => code }
      })
    }
    if status == 0 { status = executor.spin_once().code }
  }
  let stop = executor.close()
  let cleanup = session.shutdown()
  if status != 0 || stop != 0 || cleanup.code != 0 { abort("executor or cleanup failure") }
  @stdio.line_ascii("PASS_MOONBIT_EXAMPLE_CHECKED_CLEANUP_05_EXECUTOR_ROS1")
}
