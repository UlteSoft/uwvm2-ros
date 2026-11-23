(module
 (import "wasi_snapshot_preview1" "random_get_wasm64" (func $random (param i64 i64)(result i32)))
 (memory (export "memory") i64 1 2)
 (data (i64.const 0) "PEER")
 (func(export "touch")
  i64.const 0 i32.load i32.const 0x52454550 i32.ne if unreachable end
  i64.const 16 i64.load i64.eqz i32.eqz if unreachable end
  i64.const 16 i64.const 16 call $random if unreachable end
  i64.const 0 i32.const 0x1234 i32.store))
