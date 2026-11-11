(module
(type $s (struct (field (mut i32))))
(global $g (mut (ref null $s)) (ref.null $s))
(func (export "_start") (local $s (ref null $s))
 i32.const 5 struct.new $s local.set $s
 local.get $s i32.const 6 struct.set $s 0
 local.get $s ref.as_non_null call $mutate
 local.get $s struct.get $s 0 i32.const 9 i32.ne if unreachable end
 local.get $s global.set $g
 global.get $g i32.const 11 struct.set $s 0
 local.get $s struct.get $s 0 i32.const 11 i32.ne if unreachable end)
(func $mutate (param $s (ref $s))
 local.get $s i32.const 9 struct.set $s 0))