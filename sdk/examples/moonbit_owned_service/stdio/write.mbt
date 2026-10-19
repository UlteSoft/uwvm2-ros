// Real WASIp1 numeric FFI, with one bounded write for a complete ASCII marker.
// Linker reservation: descriptor0..16, data32..289 (257 bytes at most), all
// disjoint from ROS input65536+. Bytes/GC object addresses are never exposed.
// Output is inert data and never interpreted as a management command.
fn fd_write(fd : Int, iovs : Int, count : Int, written : Int) -> Int =
  "wasi_snapshot_preview1" "fd_write"

extern "wasm" fn store_ascii(index : Int, code : Int) -> Unit =
  #|(func (param i32 i32)
  #|  (i32.store8 (i32.add (i32.const 32) (local.get 0)) (local.get 1)))

extern "wasm" fn prepare(count : Int) -> Unit =
  #|(func (param i32)
  #|  (i32.store (i32.const 0) (i32.const 32))
  #|  (i32.store (i32.const 4) (local.get 0))
  #|  (i32.store (i32.const 8) (i32.const -1)))

extern "wasm" fn complete(expected : Int) -> Bool =
  #|(func (param i32) (result i32)
  #|  (i32.eq (i32.load (i32.const 8)) (local.get 0)))

pub fn line_ascii(text : String) -> Unit {
  if text.length() > 256 { abort("stdout marker bound") }
  // Validate the WHOLE marker before touching the output descriptor or fd.
  // Every admitted UTF-16 unit is ASCII and therefore exactly one byte.
  for i = 0; i < text.length(); i = i + 1 {
    let value = text[i].to_int()
    if value < 32 || value > 126 { abort("ASCII marker only") }
  }
  for i = 0; i < text.length(); i = i + 1 {
    store_ascii(i, text[i].to_int())
  }
  store_ascii(text.length(), 10)
  let count = text.length() + 1
  prepare(count)
  // Exactly one syscall/import, including the newline. A nonzero errno,
  // unwritten slot or short write fails; there is no implicit retry/replay.
  if fd_write(1, 0, 1, 8) != 0 || !complete(count) { abort("stdout incomplete write") }
}
