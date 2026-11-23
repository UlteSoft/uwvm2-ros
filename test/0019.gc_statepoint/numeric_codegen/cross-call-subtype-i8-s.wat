(module
  ;; The callee's exact-type fast path must decline a genuine subtype.
  ;; Each checked fallback must preserve the real reference and raw bits.
  (type $base (sub (array (mut i8))))
  (type $child (sub $base (array (mut i8))))
  (func $step (param $p (ref null $base)) (result i32)
    local.get $p i32.const 0
    local.get $p i32.const 0 array.get_s $base
    i32.const 1 i32.add
    array.set $base
    local.get $p i32.const 0 array.get_s $base)
  (func (export "_start")
    i64.const 1234605616436508552
    i32.const 128 i32.const 1 array.new $child
    call $step
    i32.const -127 i32.ne if unreachable end
    i64.const 1234605616436508552 i64.ne if unreachable end))
