//! Optional Rust replacement for the pure vision preload, not a ROS client.
//! wasm32-unknown-unknown + no_std avoids an assumed WASI startup. No allocator,
//! callback pointer, SDK handle or Rust reference crosses the scalar interface.
#![no_std]
use core::panic::PanicInfo;
const INVALID: i32 = i32::MIN + 22;
#[panic_handler]
fn panic(_: &PanicInfo<'_>) -> ! { core::arch::wasm32::unreachable() }
/// Explicit initializer, called by main after all Wasm preloads exist.
#[no_mangle]
pub extern "C" fn vision_initialize() -> u32 { 0 }
/// Exact integer counterpart of vision.c: all four inputs are synthetic mm.
#[no_mangle]
pub extern "C" fn vision_clearance_mm(a: i32,b: i32,c: i32,d: i32) -> i32 {
    if a < 0 || a > 5000 || b < 0 || b > 5000 || c < 0 || c > 5000 || d < 0 || d > 5000 { return INVALID; }
    let mut value = if a < b {a} else {b};
    if c < value {value = c;} if d < value {value = d;} value
}
