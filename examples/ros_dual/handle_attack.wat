(module
 (import "provider" "handle" (func $handle (result i64)))
 (import "uwvm_ros1_v1" "topic_publish" (func $publish (param i64 i32 i64 i64) (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (memory (export "memory") 1) (data (i32.const 0) "\05\00\00\00hello")
 (func (export "_start")
  (if (i32.ne (call $publish (call $handle) (i32.const 0) (i64.const 0) (i64.const 9)) (i32.const 9)) (then (call $exit (i32.const 213))))))
