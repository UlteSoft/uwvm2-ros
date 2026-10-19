// Real generated-record Guest value fixture. No Native activation/business call.
// The runner must separately admit and provide the actual55 imports. A local
// codec success says nothing about Native grant/type/performance compatibility.
pub fn abi_coverage(index : Int) -> Int {
  if index < 0 { 95 }
  else if index < 43 { @ros2.abi_coverage(index) }
  else { @owned_service.abi_coverage(index - 43) }
}

fn main {
  if @records.complete_control_default_contracts() != 0 { abort("complete32 defaults") }
  let descriptor = @records.Ros2Rcl_interfacesMsgParameterDescriptor::new()
  descriptor.type_value = 9
  let descriptor_codec = @records.Ros2Rcl_interfacesMsgParameterDescriptorCodec::new()
  let descriptor_bytes = match descriptor_codec.encode(descriptor) { Ok(value) => value; Err(_) => abort("descriptor encode") }
  match descriptor_codec.decode(descriptor_bytes) {
    Ok(value) => if value.type_value != 9 { abort("original type member lost") }
    Err(_) => abort("descriptor decode")
  }
  let parameter = @records.Ros2Rcl_interfacesMsgParameterValue::new()
  parameter.type_value = 255
  parameter.integer_value = -9223372036854775807L - 1L
  let parameter_codec = @records.Ros2Rcl_interfacesMsgParameterValueCodec::new()
  let parameter_bytes = match parameter_codec.encode(parameter) { Ok(value) => value; Err(_) => abort("parameter encode") }
  match parameter_codec.decode(parameter_bytes) {
    Ok(value) => if value.type_value != 255 || value.integer_value != -9223372036854775807L - 1L { abort("scalar bit pattern lost") }
    Err(_) => abort("parameter decode")
  }
  let field = @records.Ros2Type_description_interfacesMsgField::new()
  field.type_value.type_id = 255
  let field_codec = @records.Ros2Type_description_interfacesMsgFieldCodec::new()
  let field_bytes = match field_codec.encode(field) { Ok(value) => value; Err(_) => abort("nested original type encode") }
  match field_codec.decode(field_bytes) {
    Ok(value) => if value.type_value.type_id != 255 { abort("nested original type member lost") }
    Err(_) => abort("nested original type decode")
  }
  let query = @records.Ros2Type_description_interfacesSrvGetTypeDescription_Request::new()
  if !query.include_type_sources { abort("original true default changed") }
  let query_codec = @records.Ros2Type_description_interfacesSrvGetTypeDescription_RequestCodec::new()
  // Independent exact empty-string request vector: length0,length0,booltrue.
  match query_codec.encode(query) {
    Ok(value) => if value != b"\x00\x00\x00\x00\x00\x00\x00\x00\x01" { abort("query golden") }
    Err(_) => abort("query encode")
  }
  match query_codec.decode(b"\x00\x00\x00\x00\x00\x00\x00\x00\x02") {
    Ok(_) => abort("invalid boolean accepted"); Err(_) => ()
  }
  let source = @records.Ros2Type_description_interfacesMsgTypeSource::new()
  source.type_name = "caller/msg/Binary"
  source.encoding = "msg"
  source.raw_file_contents = b"\x00\xff\xc0\x80\x0a"
  let source_codec = @records.Ros2Type_description_interfacesMsgTypeSourceCodec::new()
  let source_bytes = match source_codec.encode(source) { Ok(value) => value; Err(_) => abort("raw original source encode") }
  match source_codec.decode(source_bytes) {
    Ok(value) => if value.raw_file_contents != source.raw_file_contents { abort("raw original source changed") }
    Err(_) => abort("raw original source decode")
  }
  // Actual CONTROL canonical IR adds no Time normalization rule. Preserve its
  // complete uint32 domain; protocol QoS80 durations have separate constraints.
  let time = @records.Ros2Builtin_interfacesMsgTime::new()
  time.nanosec = 4294967295U
  let time_codec = @records.Ros2Builtin_interfacesMsgTimeCodec::new()
  let time_bytes = match time_codec.encode(time) { Ok(value) => value; Err(_) => abort("control Time domain narrowed") }
  match time_codec.decode(time_bytes) {
    Ok(value) => if value.nanosec != 4294967295U { abort("Time bits changed") }
    Err(_) => abort("Time decode")
  }
  @stdio.line_ascii("MOONBIT_OWNED_LOCAL_RECORD_CONTRACTS_COMPLETE")
}
