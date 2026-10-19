// Compile/import inventory only. An entry exports a forwarding function and
// the actual pinned linker must retain it. NEVER execute valid selectors.
// Selecting this function does not open a capability or grant any authority.
pub fn abi_coverage(index : Int) -> Int {
  match index {
    0 => raw_abi_version()
    1 => raw_client_activate(0UL, 0UL, 0, 0UL)
    2 => raw_service_activate(0UL, 0UL, 0, 0UL)
    3 => raw_endpoint_inspect(0UL, 0UL, 0, 0UL)
    4 => raw_client_submit(0UL, 0UL, 0, 0UL, 0UL, 0UL)
    5 => raw_client_abandon(0UL, 0UL, 0UL)
    6 => raw_service_respond(0UL, 0UL, 0UL, 0UL, 0, 0UL, 0UL)
    7 => raw_endpoint_peek(0UL, 0UL, 0, 0, 0UL, 0UL)
    8 => raw_endpoint_commit(0UL, 0UL, 0UL, 0UL)
    9 => raw_endpoint_cancel(0UL, 0UL, 0UL, 0UL)
    10 => raw_client_available(0UL, 0UL, 0UL, 0UL, 0, 0UL)
    11 => raw_endpoint_retire(0UL, 0UL)
    _ => 95
  }
}
