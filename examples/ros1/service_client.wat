(module
 (import "uwvm_ros1_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
 (import "uwvm_ros1_v1" "service_call" (func $call (param i64 i32 i64 i64 i64 i64 i64) (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (memory (export "memory") 1) (data (i32.const 0) "client")
 (func (export "_start")
  (if (call $open (i32.const 0) (i64.const 0) (i64.const 6) (i64.const 8)) (then (call $exit (i32.const 216))))
  (if (call $call (i64.load (i32.const 8)) (i32.const 0) (i64.const 0) (i64.const 0) (i64.const 128) (i64.const 4096) (i64.const 16)) (then (call $exit (i32.const 217))))
  (if (i32.ne (i32.load8_u (i32.const 128)) (i32.const 1)) (then (call $exit (i32.const 218))))))
