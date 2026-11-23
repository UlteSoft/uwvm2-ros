(module (type $a (array (mut i32)))
(func $step (param $p (ref null $a))
 local.get $p i32.const 0
 local.get $p i32.const 0 array.get $a
 i32.const 1 i32.add array.set $a)

(func (export "_start") i32.const 0 array.new_default $a call $step))
