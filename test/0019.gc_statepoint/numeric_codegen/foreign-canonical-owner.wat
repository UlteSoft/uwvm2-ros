(module
(type $unused (struct))
(type $s (struct (field (mut i32)) (field (mut i64))))
(import "provider" "new" (func $new (result (ref $s))))
(import "provider" "get" (func $get (param (ref $s)) (result i32)))
(func (export "_start") (local $p (ref null $s))
 call $new local.set $p
 local.get $p struct.get $s 0 i32.const 4 i32.ne if unreachable end
 local.get $p struct.get $s 1 i64.const 0xdeadbeef81234567 i64.ne if unreachable end
 local.get $p i32.const 9 struct.set $s 0
 local.get $p ref.as_non_null call $get i32.const 9 i32.ne if unreachable end
 local.get $p i64.const -1 struct.set $s 1
 local.get $p struct.get $s 1 i64.const -1 i64.ne if unreachable end))