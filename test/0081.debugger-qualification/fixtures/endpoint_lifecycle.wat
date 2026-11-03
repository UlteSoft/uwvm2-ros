(module
  (memory (export "memory") 1)
  (data (i32.const 4) "\01\00\00\00")
  (func $leaf (export "numeric_leaf") (param i32) (result i32)
    local.get 0 i32.const 7 i32.add)
  (func (export "_start")
    (loop $again
      i32.const 0 i32.const 0 i32.load i32.const 1 i32.add i32.store
      i32.const 8 i32.const 5 call $leaf i32.store
      i32.const 4 i32.load br_if $again)
    ;; Only the authenticated manager clears the gate. These checks execute
    ;; after detach and turn an incorrect guest continuation into a real trap.
    i32.const 0 i32.load i32.eqz if unreachable end
    i32.const 8 i32.load i32.const 12 i32.ne if unreachable end))
