pub fn abi_coverage(index : Int) -> Int {
  if index < 0 { 95 }
  else if index < 43 { @ros2.abi_coverage(index) }
  else { @owned_service.abi_coverage(index - 43) }
}

fn main {
  let application = match @app.prepare(selected()) {
    Ok(value) => value; Err(_) => abort("application preparation failed")
  }
  let mut status = application.start_parameter_types()
  if status == 0 {
    status = 110
    for _poll = 0; _poll < selected_poll_limit(); _poll = _poll + 1 {
      let code = application.step_parameter_types(resolve_selected_type)
      if code == 0 { status = 0; break }
      if code != 11 || application.primary() != 0 { status = code; break }
    }
  }
  if status != 0 { let _code = application.fail(status) }
  let cleanup = application.close()
  if status != 0 || cleanup != 0 { abort("parameter service or cleanup failed") }
  @stdio.line_ascii("MOONBIT_OWNED_PARAMETER_RESPONSE_COMPLETE_AND_CLEANED")
}
