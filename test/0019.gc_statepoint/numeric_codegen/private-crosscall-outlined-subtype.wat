(module
 (type $base (sub (array (mut i32))))
 (type $derived (sub $base (array (mut i32))))
 ;; The exact-layout guard must take the checked path for a legal subtype.
 ;; Its outlined helper must return normally and increment exactly once.
 (func $step (param $p (ref null $base))
  local.get $p i32.const 0
  local.get $p i32.const 0 array.get $base i32.const 1 i32.add
  array.set $base)
 (func (export "_start") (local $p (ref null $base))
  i32.const 40 i32.const 1 array.new $derived local.set $p
  local.get $p call $step local.get $p call $step
  local.get $p i32.const 0 array.get $base i32.const 42 i32.ne
  if unreachable end))
