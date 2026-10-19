// The separately generated selection.mbt contains explicit caller DATA. It
// grants nothing: real CLI policy independently chooses all Native resources.
pub fn abi_coverage(index : Int) -> Int {
  if index < 0 { 95 }
  else if index < 43 { @ros2.abi_coverage(index) }
  else { @owned_service.abi_coverage(index - 43) }
}

fn main {
  let application = match @app.prepare(selected()) {
    Ok(value) => value; Err(_) => abort("application preparation failed")
  }
  let mut status = application.start_description(selected_type_name(), selected_type_hash(),
    selected_include_sources(), selected_wait_ns())
  if status == 0 {
    status = 110
    for _poll = 0; _poll < selected_poll_limit(); _poll = _poll + 1 {
      let code = application.step_description()
      if code == 0 { status = 0; break }
      if code != 11 || application.primary() != 0 { status = code; break }
    }
  }
  if status == 110 && application.primary() == 0 {
    // Local timeout has no DDS cancellation meaning. No second submit occurs.
    let abandoned = application.abandon_description()
    if abandoned != 0 { status = abandoned }
  }
  if status != 0 { let _code = application.fail(status) }
  if status == 0 {
    match application.description_response() {
      Some(value) => if !value.successful { status = application.fail(5) }
      None => status = application.fail(22)
    }
  }
  let cleanup = application.close()
  if status != 0 || cleanup != 0 { abort("description or cleanup failed") }
  let response = match application.description_response() {
    Some(value) => value; None => abort("complete response unavailable")
  }
  if !response.successful { abort("native type-description application refusal") }
  // The complete TypeDescription/type_sources/extra_information remain on the
  // application. raw_file_contents is Bytes; it is never decoded as UTF-8 here.
  @stdio.line_ascii("MOONBIT_OWNED_DESCRIPTION_COMPLETE_AND_CLEANED")
}
