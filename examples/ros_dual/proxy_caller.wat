(module
 (import "provider" "probe" (func $probe (result i32)))
 (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
 (memory (export "memory") 1)
 (func $invoke (result i32) (call $probe))
 (func (export "_start") (if (i32.ne (call $invoke) (i32.const 13)) (then (call $exit (i32.const 204))))))
