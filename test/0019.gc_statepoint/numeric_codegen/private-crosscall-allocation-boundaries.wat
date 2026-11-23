(module
 (type $a (array (mut i32)))
 (type $s (struct (field i32)))
 (func $step (param $p (ref null $a))
  local.get $p i32.const 0 local.get $p i32.const 0 array.get $a i32.const 1 i32.add array.set $a)
 (func (export "_start") (local $p (ref null $a)) (local $q (ref null $a)) (local $i i32)
  i32.const 1 array.new_default $a local.tee $p local.set $q
  loop $again
   local.get $p call $step
   ;; Both sides of the safepoint retain the same live object, including a
   ;; separate operand carrier. The closed caller/leaf optimization applies.
   local.get $q
   local.get $i struct.new $s drop
   call $step
   local.get $i i32.const 1 i32.add local.tee $i i32.const 65536 i32.lt_u br_if $again
  end
  local.get $p i32.const 0 array.get $a i32.const 131072 i32.ne if unreachable end
  local.get $q i32.const 0 array.get $a i32.const 131072 i32.ne if unreachable end))
