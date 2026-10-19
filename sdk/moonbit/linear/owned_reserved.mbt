// One-shot copied numeric frame for optional retained endpoint imports.
// It shares the actual existing arena lock. No managed object is an address.
// The frame/output/closure exist before Host entry. After entry this helper
// only writes reserved array elements and scalar fields; no Bytes/Result/Some
// construction, list growth, user callback or implicit retry follows an effect.

pub struct ReservedReply {
  data : Array[Byte]
  mut used : Bool
  mut attempted : Bool
  mut completed : Bool
  mut raw_code : Int
  mut code : Int
}

/// Allocate a fresh one-shot frame before any native effect. Capacity is an
/// explicit caller/Host quota, never an instruction to allocate the IPC limit.
pub fn reserve_reply(capacity : Int) -> Result[ReservedReply, Int] {
  if capacity < 0 || capacity > MAX_OUTPUT { return Err(90) }
  Ok({ data: Array::makei(capacity, fn(_) { (165).to_byte() }), used: false,
       attempted: false, completed: false, raw_code: 0, code: 9 })
}

pub fn ReservedReply::capacity(self : ReservedReply) -> Int { self.data.length() }
pub fn ReservedReply::attempted(self : ReservedReply) -> Bool { self.attempted }
pub fn ReservedReply::completed(self : ReservedReply) -> Bool { self.completed }
pub fn ReservedReply::raw_code(self : ReservedReply) -> Int { self.raw_code }
pub fn ReservedReply::code(self : ReservedReply) -> Int { self.code }
pub fn arena_idle() -> Bool { !arena.busy }

/// Bytes are usable only after a successful complete call. An invalid access
/// returns -1 without allocating an error wrapper or exposing prior scratch.
pub fn ReservedReply::byte_at(self : ReservedReply, index : Int) -> Int {
  if !self.completed || self.code != 0 || index < 0 || index >= self.data.length() { -1 }
  else { self.data[index].to_int() }
}

/// Synchronous SDK-internal numeric closure, exactly one import, no callbacks.
/// Memory0 INPUT/OUTPUT are real zero-extended offsets. A frame is never reused,
/// even after a returned failure. Reentrant arena admission does not mutate a
/// currently active frame. A Wasm trap is terminal and cannot become a code0.
pub fn ReservedReply::invoke(
  self : ReservedReply, input : Bytes,
  invoke : (UInt64, UInt64) -> Int,
) -> Int {
  if arena.busy { return 17 }
  if self.used { return 17 }
  self.used = true
  if input.length() > MAX_INPUT { self.code = 90; return 90 }
  if pages() < 130 { self.code = 22; return 22 }
  arena.busy = true
  for index = 0; index < input.length(); index = index + 1 {
    store_byte(65536 + index, input[index].to_int())
  }
  // Every selected output byte is initialized; an unwritten tail never reveals
  // a prior call. Guards fit the existing reserved padding even at MAX_OUTPUT.
  for index = 0; index < self.data.length(); index = index + 1 {
    store_byte(4259840 + index, 165)
  }
  for index = 0; index < 16; index = index + 1 {
    store_byte(4259824 + index, 219)
    store_byte(4259840 + self.data.length() + index, 219)
  }
  self.attempted = true
  let code = invoke(INPUT, OUTPUT)
  self.raw_code = code
  self.completed = true
  self.code = code
  let mut intact = true
  for index = 0; index < 16; index = index + 1 {
    if load_byte(4259824 + index) != 219 ||
        load_byte(4259840 + self.data.length() + index) != 219 { intact = false }
  }
  if !intact {
    // A damaged frame makes the result unknown, even if the raw status was0.
    // Actual Host instance retirement is required; never retry or claim undo.
    self.code = 107
  } else if code == 0 {
    for index = 0; index < self.data.length(); index = index + 1 {
      self.data[index] = load_byte(4259840 + index).to_byte()
    }
  }
  arena.busy = false
  self.code
}

/// Copy only an already validated exact range. This MAY allocate, so endpoint
/// code does it before commit/response and keeps the associated borrow alive.
/// Failure/trap while allocating does not mean native rollback or cleanupPASS.
pub fn ReservedReply::copy_range(self : ReservedReply, first : Int, count : Int) -> Result[Bytes, Int] {
  if !self.completed || self.code != 0 { return Err(9) }
  if first < 0 || count < 0 || first > self.data.length() || count > self.data.length() - first {
    return Err(22)
  }
  Ok(Bytes::makei(count, fn(index) { self.data[first + index] }))
}
