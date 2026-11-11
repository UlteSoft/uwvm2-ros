(module
(type $base (sub (struct (field (mut i32)))))
(type $child (sub $base (struct (field (mut i32)) (field i64))))
(func (export "_start") (local $p (ref null $base)) (local $q (ref null $child))
 i32.const 7 i64.const -1 struct.new $child local.tee $q local.set $p
 local.get $p i32.const 99 struct.set $base 0
 local.get $q struct.get $child 0 i32.const 99 i32.ne if unreachable end
 local.get $p struct.get $base 0 i32.const 99 i32.ne if unreachable end))