(module
(type $a (array (mut i32)))
(func $step (param $p (ref null $a)) (local $n i32)
 loop $again
  local.get $p i32.const 0 local.get $p i32.const 0 array.get $a i32.const 1 i32.add array.set $a
  local.get $n i32.const 1 i32.add local.tee $n i32.const 3 i32.lt_u br_if $again
 end)
(func (export "_start") (local $p (ref null $a))
 i32.const 1 array.new_default $a local.tee $p call $step
 local.get $p i32.const 0 array.get $a i32.const 3 i32.ne if unreachable end))
