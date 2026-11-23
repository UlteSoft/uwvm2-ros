(module
(type $a (array (mut i32)))
(global $published (mut (ref null $a)) (ref.null $a))
(func $publish (param $p (ref null $a))
 local.get $p global.set $published
 local.get $p i32.const 0 i32.const 42 array.set $a)
(func (export "_start") (local $p (ref null $a))
 i32.const 1 array.new_default $a local.tee $p call $publish
 local.get $p i32.const 0 array.get $a i32.const 42 i32.ne if unreachable end
 global.get $published i32.const 0 array.get $a i32.const 42 i32.ne if unreachable end))
