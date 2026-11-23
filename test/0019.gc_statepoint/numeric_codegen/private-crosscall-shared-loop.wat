(module (type $a (array (mut i32))) (global $published (mut (ref null $a)) (ref.null $a))
 (func $step (param $p (ref null $a)) local.get $p i32.const 0 local.get $p i32.const 0 array.get $a i32.const 1 i32.add array.set $a) (func (export "_start") (local $i i32) (local $p (ref null $a)) i32.const 1 array.new_default $a local.set $p
 local.get $p global.set $published loop $run local.get $p call $step local.get $i i32.const 1 i32.add local.tee $i i32.const 4096 i32.lt_u br_if $run end local.get $p i32.const 0 array.get $a i32.const 4096 i32.ne if unreachable end))
