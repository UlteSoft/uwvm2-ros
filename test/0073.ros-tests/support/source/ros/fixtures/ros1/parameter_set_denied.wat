(module
  (import "uwvm_ros1_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
  (import "uwvm_ros1_v1" "parameter_set" (func $rpc (param i64 i32 i64 i64 i64 i64 i64) (result i32)))
  (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
  (memory (export "memory") 1) (data (i32.const 0) "cap") (data (i32.const 8192) "\0c\00\00\00\75\6e\61\75\74\68\6f\72\69\7a\65\64")
  (func (export "_start")
   (if (call $open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)) (then (call $exit (i32.const 204))))
   (if (i32.ne (call $rpc (i64.load (i32.const 8)) (i32.const 0) (i64.const 8192) (i64.const 16) (i64.const 128) (i64.const 4096) (i64.const 16)) (i32.const 13)) (then (call $exit (i32.const 205))))))
