// Exact protocol values. Decoding grants no authority and invokes no raw import.
// GID32 and request writer GUID16 remain different byte domains. Native signed
// times/sequence preserve all64 bits, including zero, negatives and all ones.

pub const MAX_BODY : Int = 4193626
pub const MAX_PACKET : Int = 4194304
pub const MAX_WAIT_NS : UInt64 = 60000000000UL
const U64_MAX : UInt64 = 18446744073709551615UL

pub struct EndpointReceipt {
  frame : @linear.ReservedReply
  source : UInt64
  mut instance : UInt64
  mut generation : UInt64
  mut session : UInt64
  mut incarnation : UInt64
  role : Int
  mut valid : Bool
}

// Construct before activate/inspect. Admission afterwards performs only scalar
// writes into this retained receipt and returns an immediate numeric status.
fn prepare_receipt(frame : @linear.ReservedReply, source : UInt64, role : Int) -> EndpointReceipt {
  { frame, source, role, instance: 0UL, generation: 0UL, session: 0UL, incarnation: 0UL, valid: false }
}

// The owner creates this object BEFORE peek. A complete associated UMRC may
// authorize an exact cancel after a later metadata/codec refusal; a malformed
// header cannot invent a sequence/nonce. Resetting is local, never an ACK.
pub struct Borrow {
  mut associated : Bool
  mut sequence : UInt64
  mut nonce : UInt64
  mut ticket : UInt64
}

pub fn new_borrow() -> Borrow { { associated: false, sequence: 0UL, nonce: 0UL, ticket: 0UL } }

pub struct Delivery {
  borrow : Borrow
  kind : Int
  control : Bool
  control_flags : Int
  error : UInt
  body : Bytes
  writer : Bytes
  native_sequence : Int64
  source_timestamp : Int64
  received_timestamp : Int64
  mut abandoned : Bool
}

pub fn Delivery::ticket(self : Delivery) -> UInt64 { self.borrow.ticket }
pub fn Delivery::sequence(self : Delivery) -> UInt64 { self.borrow.sequence }
pub fn Delivery::reservation(self : Delivery) -> UInt64 { self.borrow.nonce }
pub fn Delivery::kind(self : Delivery) -> Int { self.kind }
pub fn Delivery::is_control(self : Delivery) -> Bool { self.control }
pub fn Delivery::control_flags(self : Delivery) -> Int { self.control_flags }
pub fn Delivery::error(self : Delivery) -> UInt { self.error }
pub fn Delivery::body(self : Delivery) -> Bytes { self.body }
pub fn Delivery::writer(self : Delivery) -> Bytes { self.writer }
pub fn Delivery::native_sequence(self : Delivery) -> Int64 { self.native_sequence }
pub fn Delivery::source_timestamp(self : Delivery) -> Int64 { self.source_timestamp }
pub fn Delivery::received_timestamp(self : Delivery) -> Int64 { self.received_timestamp }
pub fn Delivery::was_abandoned(self : Delivery) -> Bool { self.abandoned }

pub fn EndpointReceipt::source(self : EndpointReceipt) -> UInt64 { self.source }
pub fn EndpointReceipt::incarnation(self : EndpointReceipt) -> UInt64 { self.incarnation }
pub fn EndpointReceipt::instance(self : EndpointReceipt) -> UInt64 { self.instance }
pub fn EndpointReceipt::generation(self : EndpointReceipt) -> UInt64 { self.generation }
pub fn EndpointReceipt::session(self : EndpointReceipt) -> UInt64 { self.session }
pub fn EndpointReceipt::role(self : EndpointReceipt) -> Int { self.role }
pub fn EndpointReceipt::gid(self : EndpointReceipt) -> Result[Bytes, Int] {
  if !self.valid { return Err(9) }
  self.frame.copy_range(224, self.frame.byte_at(14))
}
pub fn EndpointReceipt::qos(self : EndpointReceipt, index : Int) -> Result[Bytes, Int] {
  if !self.valid { return Err(9) }
  if index != 0 && index != 1 { return Err(22) }
  self.frame.copy_range(64 + index * 80, 80)
}

fn word(frame : @linear.ReservedReply, first : Int, width : Int) -> UInt64 {
  let mut result = 0UL
  for index = 0; index < width; index = index + 1 {
    result = result | (frame.byte_at(first + index).to_uint64() << (index * 8))
  }
  result
}

fn zero(frame : @linear.ReservedReply, first : Int, count : Int) -> Bool {
  for index = 0; index < count; index = index + 1 {
    if frame.byte_at(first + index) != 0 { return false }
  }
  true
}

fn full_handle(handle : UInt64) -> Bool {
  (handle & 4294967295UL) != 0UL && (handle >> 32) != 0UL
}

fn owner_matches(frame : @linear.ReservedReply, at : Int, endpoint : EndpointReceipt) -> Bool {
  frame.byte_at(at + 7) == 2 && word(frame, at + 16, 8) == endpoint.source &&
  word(frame, at + 24, 8) == endpoint.instance && word(frame, at + 32, 8) == endpoint.generation &&
  word(frame, at + 40, 8) == endpoint.session
}

fn concrete_qos(frame : @linear.ReservedReply, at : Int) -> Bool {
  let history = word(frame, at, 4)
  let reliability = word(frame, at + 4, 4)
  let durability = word(frame, at + 8, 4)
  let liveliness = word(frame, at + 12, 4)
  history >= 1UL && history <= 2UL && reliability >= 1UL && reliability <= 2UL &&
  durability >= 1UL && durability <= 2UL && liveliness >= 1UL && liveliness <= 2UL &&
  (history != 1UL || word(frame, at + 16, 8) != 0UL) &&
  word(frame, at + 32, 8) < 1000000000UL && word(frame, at + 48, 8) < 1000000000UL &&
  word(frame, at + 64, 8) < 1000000000UL && frame.byte_at(at + 72) <= 1 && zero(frame, at + 73, 7)
}

/// expected_qos is exactly TWO caller-selected protocol80 rows, captured before
/// native activation. Every native byte is compared; these are assertions, not
/// permission, native identity or a requested native mutation.
fn admit_receipt(
  receipt : EndpointReceipt, operation : Int, expected_qos : Array[Bytes],
) -> Int {
  let frame = receipt.frame
  let source = receipt.source
  let role = receipt.role
  if frame.code() != 0 || !frame.completed() || frame.capacity() != 256 ||
      !full_handle(source) || (role != 3 && role != 4) ||
      (operation != 1 && operation != 2) || expected_qos.length() != 2 { return 22 }
  for row in expected_qos { if row.length() != 80 { return 22 } }
  let gid_count = frame.byte_at(14)
  if word(frame, 0, 4) != 1163021653UL || word(frame, 4, 2) != 1UL ||
      frame.byte_at(6) != operation || frame.byte_at(7) != 2 || word(frame, 8, 4) != 256UL ||
      frame.byte_at(12) != role || frame.byte_at(13) != 2 || gid_count < 0 || gid_count > 32 ||
      frame.byte_at(15) != 0 || !zero(frame, 56, 8) || word(frame, 16, 8) != source ||
      word(frame, 24, 8) == 0UL || word(frame, 32, 8) == 0UL || word(frame, 40, 8) == 0UL ||
      word(frame, 48, 8) == 0UL || (role == 4 && gid_count != 0) ||
      !zero(frame, 224 + gid_count, 32 - gid_count) ||
      !concrete_qos(frame, 64) || !concrete_qos(frame, 144) { return 22 }
  // The complete actual structurally valid receipt proves a retained native
  // binding independently of the caller's expected-QoS assertion. Save those
  // real facts before returning17 on a mismatch, so close can once-retire this
  // exact incarnation and retain its status before independent raw cleanup.
  receipt.instance = word(frame, 24, 8)
  receipt.generation = word(frame, 32, 8)
  receipt.session = word(frame, 40, 8)
  receipt.incarnation = word(frame, 48, 8)
  receipt.valid = true
  for row = 0; row < 2; row = row + 1 {
    for index = 0; index < 80; index = index + 1 {
      if frame.byte_at(64 + row * 80 + index) != expected_qos[row][index].to_int() { return 17 }
    }
  }
  0
}

pub fn same_receipt(original : EndpointReceipt, inspected : EndpointReceipt) -> Bool {
  if !original.valid || !inspected.valid || original.frame.byte_at(6) != 1 || inspected.frame.byte_at(6) != 2 { return false }
  for index = 0; index < 256; index = index + 1 {
    if index != 6 && original.frame.byte_at(index) != inspected.frame.byte_at(index) { return false }
  }
  true
}

/// Peek has no length-out slot: valid extent is160+UMRC.record_bytes. Bytes
/// beyond that extent are initialized scratch, never another logical record.
/// The exact associated borrow is retained even when the later metadata fails.
pub fn decode_delivery(
  frame : @linear.ReservedReply, endpoint : EndpointReceipt, peer : UInt64,
  maximum_body : Int, last_committed : UInt64, last_nonce : UInt64, borrow : Borrow,
) -> Result[Delivery, Int] {
  borrow.associated = false
  if !endpoint.valid || frame.code() != 0 || !frame.completed() || frame.capacity() < 352 ||
      frame.capacity() > MAX_PACKET || maximum_body < 0 || maximum_body > MAX_BODY ||
      !full_handle(peer) || peer == endpoint.source { return Err(22) }
  let capacity = frame.capacity().to_uint64()
  if word(frame, 0, 4) != 1129467221UL || word(frame, 4, 2) != 1UL ||
      frame.byte_at(6) != 5 || word(frame, 8, 4) != 160UL ||
      frame.byte_at(12) != endpoint.role || frame.byte_at(13) != 16 || !zero(frame, 14, 2) ||
      !zero(frame, 128, 32) || !owner_matches(frame, 0, endpoint) ||
      word(frame, 48, 8) != endpoint.incarnation || word(frame, 56, 8) != peer ||
      word(frame, 88, 8) != 0UL || word(frame, 96, 8) != capacity ||
      word(frame, 112, 8) != 0UL || word(frame, 120, 8) != 0UL { return Err(22) }
  let record_bytes = word(frame, 104, 8)
  if record_bytes < 192UL || record_bytes > capacity - 160UL ||
      record_bytes - 192UL > maximum_body.to_uint64() { return Err(90) }
  let sequence = word(frame, 72, 8)
  let nonce = word(frame, 80, 8)
  if sequence == 0UL || sequence <= last_committed || nonce == 0UL || nonce <= last_nonce ||
      (endpoint.role == 3 && (last_committed == U64_MAX || sequence != last_committed + 1UL)) { return Err(13) }
  borrow.sequence = sequence
  borrow.nonce = nonce
  borrow.ticket = word(frame, 64, 8)
  borrow.associated = true
  let body_count = (record_bytes - 192UL).to_int()
  if word(frame, 160, 4) != 1229804117UL || word(frame, 164, 2) != 1UL ||
      word(frame, 168, 4) != 192UL || !owner_matches(frame, 160, endpoint) ||
      word(frame, 208, 8) != endpoint.incarnation || word(frame, 216, 8) != sequence ||
      word(frame, 224, 8) != borrow.ticket || word(frame, 272, 8) != body_count.to_uint64() ||
      frame.byte_at(283) != 0 || !zero(frame, 336, 16) { return Err(22) }
  let kind = frame.byte_at(166)
  let flags = word(frame, 172, 4)
  let controls = frame.byte_at(282)
  let error = word(frame, 284, 4).to_uint()
  let source_time = word(frame, 232, 8).reinterpret_as_int64()
  let received_time = word(frame, 240, 8).reinterpret_as_int64()
  let native_sequence = word(frame, 264, 8).reinterpret_as_int64()
  if kind == 6 || kind == 7 {
    if flags != 0UL || body_count != 0 || frame.byte_at(280) != 0 || frame.byte_at(281) != 0 ||
        !zero(frame, 288, 48) || source_time != 0L || received_time != 0L || native_sequence != 0L ||
        word(frame, 248, 8) != U64_MAX || word(frame, 256, 8) != U64_MAX ||
        (controls & 252) != 0 || (kind == 6 && error == 0U) || (kind == 7 && (controls & 1) == 0) { return Err(22) }
    return Ok({ borrow, kind, control: true, control_flags: controls, error, body: b"", writer: b"",
      native_sequence, source_timestamp: source_time, received_timestamp: received_time, abandoned: false })
  }
  if frame.byte_at(281) != 16 { return Err(95) }
  if kind != (if endpoint.role == 3 { 3 } else { 2 }) || flags != 35UL || borrow.ticket == 0UL ||
      error != 0U || controls != 0 || frame.byte_at(280) != 0 || !zero(frame, 288, 32) ||
      word(frame, 248, 8) != U64_MAX || word(frame, 256, 8) != U64_MAX { return Err(22) }
  let body = match frame.copy_range(352, body_count) { Ok(value) => value; Err(code) => return Err(code) }
  let writer = match frame.copy_range(320, 16) { Ok(value) => value; Err(code) => return Err(code) }
  Ok({ borrow, kind, control: false, control_flags: 0, error: 0U, body, writer,
       native_sequence, source_timestamp: source_time, received_timestamp: received_time, abandoned: false })
}
