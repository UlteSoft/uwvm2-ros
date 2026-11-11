(module
(type $s (struct (field (mut i32))))
(type $r (array (mut (ref null $s))))
(func (export "_start") (local $a (ref null $r)) (local $n i32)
 i32.const 65537 array.new_default $r local.set $a
 loop $fill
 local.get $a local.get $n local.get $n struct.new $s array.set $r
 local.get $n i32.const 1 i32.add local.tee $n i32.const 65537 i32.lt_u br_if $fill end
 i32.const 0 local.set $n
 loop $read
 local.get $a local.get $n array.get $r struct.get $s 0 local.get $n i32.ne if unreachable end
 local.get $a local.get $n array.get $r local.get $n i32.const 17 i32.add struct.set $s 0
 local.get $a local.get $n array.get $r struct.get $s 0 local.get $n i32.const 17 i32.add i32.ne if unreachable end
 local.get $n i32.const 1 i32.add local.tee $n i32.const 65537 i32.lt_u br_if $read end))