(module
  (global $value (mut i32) (i32.const 0))
  (func $init i32.const 17 global.set $value)
  (start $init)
  (func (export "get") (result i32) global.get $value)
  (func (export "set") (param i32) local.get 0 global.set $value))
