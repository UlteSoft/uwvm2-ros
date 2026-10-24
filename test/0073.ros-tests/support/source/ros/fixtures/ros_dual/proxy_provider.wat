(module
 (import "uwvm_ros1_v1" "capability_open" (func $open (param i32 i64 i64 i64) (result i32)))
 (memory (export "memory") 1) (data (i32.const 0) "pub")
 (func (export "probe") (result i32) (call $open (i32.const 0) (i64.const 0) (i64.const 3) (i64.const 8))))
