(module
 (import "dl.example" "add_i32" (func $native (param i32 i32) (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (memory 1)
 (func (export "_start")
  (drop (call $native (i32.const 1) (i32.const 2)))
  (call $exit (i32.const 201))))
