// Stage as stdio/read.mbt next to the exact original bounded ASCII writer.
// Real basic WASIp1 input, not an object pointer or a host-language model.
// Scratch320..332 + byte352 are disjoint from marker0..289 and ROS65536+.
fn fd_read(fd : Int, iovs : Int, count : Int, read_out : Int) -> Int =
  "wasi_snapshot_preview1" "fd_read"
extern "wasm" fn prepare_credit() -> Unit =
  #|(func
  #|  (i32.store (i32.const 320) (i32.const 352))
  #|  (i32.store (i32.const 324) (i32.const 1))
  #|  (i32.store (i32.const 328) (i32.const -1))
  #|  (i32.store8 (i32.const 352) (i32.const 0)))
extern "wasm" fn credit_complete() -> Bool =
  #|(func (result i32)
  #|  (i32.and (i32.eq (i32.load (i32.const 328)) (i32.const 1))
  #|           (i32.eq (i32.load8_u (i32.const 352)) (i32.const 82))))
pub fn read_credit() -> Int {
  prepare_credit()
  let code = fd_read(0, 320, 1, 328)
  if code != 0 { code } else if credit_complete() { 0 } else { 22 }
}
