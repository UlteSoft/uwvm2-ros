(module
 (import "uwvm_ros1_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (memory (export "memory") 1) (data (i32.const 0) "pub")
 (func $init (if (call $open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8)) (then (call $exit (i32.const 212)))))
 (start $init)
 (func (export "handle") (result i64) (i64.load (i32.const 8))))
