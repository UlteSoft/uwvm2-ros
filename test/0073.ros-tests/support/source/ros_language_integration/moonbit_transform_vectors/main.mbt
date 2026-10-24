// Test-only corpus supplied as inert frozen generated source by the Linux owner.
fn main {
  let cases = @vectors.cases()
  for index = 0; index < cases.length(); index = index + 1 {
    let (backend,bytes) = cases[index]
    match @wire.read_transform(bytes,backend) {
      Err(code) => @stdio.line_ascii("TRANSFORM_CASE " + index.to_string() + " " + backend.to_string() + " " + code.to_string() + " 0")
      Ok(value) => {
        let encoded = match value.encode(backend) { Ok(v) => v; Err(_) => abort("accepted transform cannot encode") }
        @stdio.line_ascii("TRANSFORM_CASE " + index.to_string() + " " + backend.to_string() + " 0 " + encoded.length().to_string())
        for offset = 0; offset < encoded.length(); offset = offset + 1 {
          @stdio.line_ascii("TRANSFORM_BYTE " + index.to_string() + " " + offset.to_string() + " " + encoded[offset].to_int().to_string())
        }
      }
    }
  }
  @stdio.line_ascii("PASS_MOONBIT_TRANSFORM_CORPUS " + cases.length().to_string())
}
