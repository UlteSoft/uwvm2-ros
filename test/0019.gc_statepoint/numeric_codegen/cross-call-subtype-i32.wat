(module
  ;; The callee's exact-type fast path must decline a genuine subtype.
  ;; Each checked fallback must preserve the real reference and raw bits.
  (type $base (sub (array (mut i32))))
  (type $child (sub $base (array (mut i32))))
  (func $step (param $p (ref null $base)) (result i32)
    local.get $p i32.const 0
    local.get $p i32.const 0 array.get $base
    i32.const 1 i32.add
    array.set $base
    local.get $p i32.const 0 array.get $base)
  (func (export "_start")
    i64.const 1234605616436508552
    i32.const 2147483647 i32.const 1 array.new $child
    call $step
    i32.const -2147483648 i32.ne if unreachable end
    i64.const 1234605616436508552 i64.ne if unreachable end))
