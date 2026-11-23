(module
  (import "env" "memory" (memory 1 1 shared))
  (global $value (mut i32) (i32.const 0))
  (func $init i32.const 17 global.set $value)
  (start $init)
  (func (export "get") (result i32) global.get $value))
