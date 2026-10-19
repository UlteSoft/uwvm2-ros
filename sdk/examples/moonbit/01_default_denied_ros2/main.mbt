// All 38/43 imports remain reachable only through this explicitly exported
// inspection probe. Do not execute valid selector indexes.
pub fn abi_coverage(index : Int) -> Int { @ros.abi_coverage(index) }

extern "wasm" fn seed_handle() -> Unit =
  #|(func (i64.store (i32.const 4259840) (i64.const -1)))

extern "wasm" fn handle_unchanged() -> Bool =
  #|(func (result i32) (i64.eq (i64.load (i32.const 4259840)) (i64.const -1)))

// Fresh Runtime instance, enabled matching business namespace, no grants.
// This is a real denied open, not a wrong-backend or invalid-memory negative.
fn main {
  let session = match @ros.new_session() { Ok(value) => value; Err(_) => abort("session") }
  let node = match session.node("moonbit-denied") { Ok(value) => value; Err(_) => abort("node") }
  let denied = node.publisher("moonbit-denied")
  let mut unexpected_cleanup = 0
  let code = match denied { Err(value) => value; Ok(owner) => { unexpected_cleanup = owner.close(); 22 } }
  let probe = @linear.perform(b"moonbit-denied", 8, true, fn(a, b, _) {
    seed_handle()
    let actual = @ros.raw_capability_open(0, a, 14UL, b)
    if actual == 13 && handle_unchanged() { 13 } else { 22 }
  })
  let cleanup = session.shutdown()
  if code != 13 || probe.code != 13 || unexpected_cleanup != 0 || cleanup.code != 0 { abort("default deny contract") }
  @stdio.line_ascii("PASS_MOONBIT_DEFAULT_DENIED_13")
}
