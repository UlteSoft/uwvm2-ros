(module
 (import "uwvm_ros1_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
 (import "uwvm_ros1_v1" "capability_close" (func $close (param i64) (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (memory (export "memory") 1) (memory 1) (data (memory 1) (i32.const 0) "pub")
 (func (export "_start")
  (if (call $open (i32.const 1) (i64.const 0) (i64.const 3) (i64.const 8)) (then (call $exit (i32.const 209))))
  (if (call $close (i64.load 1 (i32.const 8))) (then (call $exit (i32.const 210))))
  (if (i32.ne (call $open (i32.const 2) (i64.const 0) (i64.const 3) (i64.const 8)) (i32.const 22)) (then (call $exit (i32.const 211))))))
