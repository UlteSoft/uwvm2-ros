(module (type $a (array (mut i32)))
(func $step (param $p (ref null $a))
 local.get $p i32.const 0
 local.get $p i32.const 0 array.get $a
 i32.const 1 i32.add array.set $a)

(global $published (mut (ref null $a)) (ref.null $a))
(func $publish (param $p (ref null $a)) local.get $p global.set $published)
(func (export "_start") (local $p (ref null $a))
 i32.const 1 array.new_default $a local.tee $p call $step
 local.get $p i32.const 0 array.get $a i32.const 1 i32.ne if unreachable end
 return
 ref.null $a call $publish))
