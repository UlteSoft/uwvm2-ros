// This package never casts a MoonBit object to an address. A single ordinary
// wasm32 memory contains a linker-reserved arena. Every managed byte is copied
// through explicit Wasm loads/stores; this works independently of GC layouts.
// Read README.md before linking: the exact reservation is an artifact contract.

pub const MAX_INPUT : Int = 4193786
pub const MAX_OUTPUT : Int = 4194304
pub const INPUT : UInt64 = 65536UL
pub const OUTPUT : UInt64 = 4259840UL
pub const SLOT : UInt64 = 8454144UL
pub const ARENA_END : Int = 8454160
pub const HEAP_START : Int = 8519680
pub const SENTINEL : UInt64 = 18446744073709551615UL

struct ArenaState { mut busy : Bool }
let arena : ArenaState = { busy: false }

/// A failure owns no public data. Required is diagnostic and present only when
/// error90 actually overwrote the eight-byte sentinel. It never requests retry.
pub(all) struct Reply {
  code : Int
  data : Bytes
  required : UInt64?
}

pub fn failure(code : Int) -> Reply {
  { code, data: b"", required: None }
}

extern "wasm" fn pages() -> Int =
  #|(func (result i32) (memory.size))

extern "wasm" fn store_byte(address : Int, value : Int) -> Unit =
  #|(func (param i32 i32) (i32.store8 (local.get 0) (local.get 1)))

extern "wasm" fn load_byte(address : Int) -> Int =
  #|(func (param i32) (result i32) (i32.load8_u (local.get 0)))

extern "wasm" fn store_slot(value : UInt64) -> Unit =
  #|(func (param i64) (i64.store (i32.const 8454144) (local.get 0)))

extern "wasm" fn load_slot() -> UInt64 =
  #|(func (result i64) (i64.load (i32.const 8454144)))

/// Synchronous, non-raising import adapter. The closure receives real memory0
/// offsets already zero-extended to i64. It must perform exactly one raw call,
/// without invoking user callbacks. No clock, allocator or growth import exists.
/// fixed=false uses the conditional result-length slot; fixed=true copies the
/// fixed capacity only on success (capability handle, ready32, request ID8).
/// A Wasm trap aborts the instance: this package claims no trap recovery/replay.
pub fn perform(
  input : Bytes, capacity : Int, fixed : Bool,
  invoke : (UInt64, UInt64, UInt64) -> Int,
) -> Reply {
  if arena.busy { return failure(17) }
  if input.length() > MAX_INPUT || capacity < 0 || capacity > MAX_OUTPUT {
    return failure(90)
  }
  // Avoid i32 multiplication overflow even for the largest wasm32 memory.
  if pages() < 130 { return failure(22) }
  arena.busy = true
  for i = 0; i < input.length(); i = i + 1 {
    store_byte(65536 + i, input[i].to_int())
  }
  store_slot(SENTINEL)
  let code = invoke(INPUT, OUTPUT, SLOT)
  let written = load_slot()
  let result = if code == 0 {
    let count = if fixed { capacity.to_uint64() } else { written }
    if count == SENTINEL || count > capacity.to_uint64() {
      failure(22)
    } else {
      let copy = Bytes::makei(count.to_int(), fn(i) {
        load_byte(4259840 + i).to_byte()
      })
      { code: 0, data: copy, required: None }
    }
  } else if code == 90 && written != SENTINEL {
    { code, data: b"", required: Some(written) }
  } else {
    failure(code)
  }
  arena.busy = false
  result
}

/// No-output operations do not invent a required-length write on success.
pub fn write(input : Bytes, invoke : (UInt64) -> Int) -> Int {
  perform(input, 0, true, fn(a, _, _) { invoke(a) }).code
}

/// Kept public for an actual post-link arena guard test; not a claim that static
/// segments or compiler heap overlap were independently inspected here.
pub fn memory_available() -> Bool { pages() >= 130 }


extern "wasm" fn load_output_u64() -> UInt64 =
  #|(func (result i64) (i64.load (i32.const 4259840)))

/// Allocation-free successful u64 admission. The owner, tracking slot, success
/// Result and both non-raising closures MUST already exist before this call.
/// No Bytes/Some/Ok/list allocation follows a successful raw import here.
/// length_required=false is capability_open's fixed out8 form; true requires
/// an actual RPC length write of exactly8. The commit closure performs only
/// assignments into reserved local fields/slots and returns a status.
/// A malformed/unknown admitted reply is never retried. Instance termination
/// still requires the external Runtime owner to retire all actual native state.
pub fn admit_u64(
  input : Bytes, length_required : Bool,
  invoke : (UInt64, UInt64, UInt64) -> Int,
  commit : (UInt64) -> Int,
) -> Int {
  if arena.busy { return 17 }
  if input.length() > MAX_INPUT { return 90 }
  if pages() < 130 { return 22 }
  arena.busy = true
  for i = 0; i < input.length(); i = i + 1 {
    store_byte(65536 + i, input[i].to_int())
  }
  // Zero is not a valid handle/goal and detects an unwritten success reply.
  // Unlike UINT64_MAX, it does not exclude the valid full-width opaque value.
  for i = 0; i < 8; i = i + 1 { store_byte(4259840 + i, 0) }
  store_slot(SENTINEL)
  let raw_code = invoke(INPUT, OUTPUT, SLOT)
  let code = if raw_code != 0 { raw_code }
    else if length_required && load_slot() != 8UL { 22 }
    else {
      let value = load_output_u64()
      if value == 0UL { 22 } else { commit(value) }
    }
  arena.busy = false
  code
}
