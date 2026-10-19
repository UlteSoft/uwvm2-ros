// Pure expected-value preflight before native activation. This vocabulary is
// the actual normalized protocol80, never an enum cast from an arbitrary DDS.
fn qos_word(bytes : Bytes, at : Int, width : Int) -> UInt64 {
  let mut result = 0UL
  for index = 0; index < width; index = index + 1 {
    result = result | (bytes[at + index].to_int().to_uint64() << (index * 8))
  }
  result
}

fn expected_qos_valid(bytes : Bytes) -> Bool {
  if bytes.length() != 80 { return false }
  let history = qos_word(bytes, 0, 4)
  for at = 0; at < 16; at = at + 4 {
    let value = qos_word(bytes, at, 4)
    if value < 1UL || value > 2UL { return false }
  }
  if history == 1UL && qos_word(bytes, 16, 8) == 0UL { return false }
  if qos_word(bytes, 32, 8) >= 1000000000UL || qos_word(bytes, 48, 8) >= 1000000000UL ||
      qos_word(bytes, 64, 8) >= 1000000000UL || bytes[72].to_int() > 1 { return false }
  for at = 73; at < 80; at = at + 1 { if bytes[at].to_int() != 0 { return false } }
  true
}
