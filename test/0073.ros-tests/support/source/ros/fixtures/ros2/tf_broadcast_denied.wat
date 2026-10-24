(module
  (import "uwvm_ros2_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
  (import "uwvm_ros2_v1" "tf_broadcast" (func $rpc (param i64 i32 i64 i64 i64 i64 i64) (result i32)))
  (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
  (memory (export "memory") 1) (data (i32.const 0) "cap") (data (i32.const 8192) "\00\00\00\00\7b\00\00\00\c8\01\00\00\03\00\00\00\6d\61\70\04\00\00\00\62\61\73\65\00\00\00\00\00\00\f4\3f\00\00\00\00\00\00\04\40\00\00\00\00\00\00\0e\40\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\f0\3f")
  (func (export "_start")
   (if (call $open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)) (then (call $exit (i32.const 204))))
   (if (i32.ne (call $rpc (i64.load (i32.const 8)) (i32.const 0) (i64.const 8192) (i64.const 83) (i64.const 128) (i64.const 4096) (i64.const 16)) (i32.const 13)) (then (call $exit (i32.const 205))))))
