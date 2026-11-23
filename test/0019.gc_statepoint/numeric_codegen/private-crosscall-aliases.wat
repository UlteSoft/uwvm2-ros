(module (type $a (array (mut i32)))
(func $step (param $p (ref null $a))
 local.get $p i32.const 0
 local.get $p i32.const 0 array.get $a
 i32.const 1 i32.add array.set $a)

(func (export "_start") (local $p (ref null $a)) (local $q (ref null $a))
 i32.const 40 i32.const 1 array.new $a local.tee $p local.set $q
 local.get $p call $step local.get $q call $step
 local.get $p i32.const 0 array.get $a i32.const 42 i32.ne if unreachable end))
