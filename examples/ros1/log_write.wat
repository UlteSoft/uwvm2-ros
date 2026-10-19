(module
  (import "uwvm_ros1_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
  (import "uwvm_ros1_v1" "log_write" (func $log (param i64 i32 i64 i64) (result i32)))
  (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
  (memory (export "memory") 1) (data (i32.const 0) "cap") (data (i32.const 32) "\05\00\00\00hello")
  (func $check (param $e i32) (if (local.get $e) (then (call $exit (local.get $e)))))
  (func (export "_start")
   (call $check (call $open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)))
   (call $check (call $log (i64.load (i32.const 8)) (i32.const 0) (i64.const 32) (i64.const 9)))))
