(module
 (import "uwvm_ros1_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (memory (export "memory") 1) (data (i32.const 0) "pub")
 (func (export "_start")
  (i64.store (i32.const 8) (i64.const -1))
  (if (i32.ne (call $open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)) (i32.const 13)) (then (call $exit (i32.const 224))))
  (if (i64.ne (i64.load (i32.const 8)) (i64.const -1)) (then (call $exit (i32.const 225))))
  (call $exit (i32.const 0))))
