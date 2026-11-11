(module (type $s (struct (field (mut i32)) (field (mut i64))))
(func (export "new") (result (ref $s)) i32.const 4 i64.const 0xdeadbeef81234567 struct.new $s)
(func (export "get") (param (ref $s)) (result i32) local.get 0 struct.get $s 0))