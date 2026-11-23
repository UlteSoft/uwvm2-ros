(module (type $a (array (mut i64)))
 (func $leaf (param $p0 (ref null $a)) (param $p1 (ref null $a)) (param $p2 (ref null $a)) (param $p3 (ref null $a)) (param $p4 (ref null $a)) (param $p5 (ref null $a)) (param $p6 (ref null $a)) (param $p7 (ref null $a)) (param $p8 (ref null $a)) (result i64) local.get $p0 i32.const 0 array.get $a
local.get $p1 i32.const 0 array.get $a i64.add
local.get $p2 i32.const 0 array.get $a i64.add
local.get $p3 i32.const 0 array.get $a i64.add
local.get $p4 i32.const 0 array.get $a i64.add
local.get $p5 i32.const 0 array.get $a i64.add
local.get $p6 i32.const 0 array.get $a i64.add
local.get $p7 i32.const 0 array.get $a i64.add
local.get $p8 i32.const 0 array.get $a i64.add)
 (func (export "_start") (local $p0 (ref null $a)) (local $p1 (ref null $a)) (local $p2 (ref null $a)) (local $p3 (ref null $a)) (local $p4 (ref null $a)) (local $p5 (ref null $a)) (local $p6 (ref null $a)) (local $p7 (ref null $a)) (local $p8 (ref null $a))
  i64.const 1 i32.const 1 array.new $a local.set $p0
i64.const 2 i32.const 1 array.new $a local.set $p1
i64.const 3 i32.const 1 array.new $a local.set $p2
i64.const 4 i32.const 1 array.new $a local.set $p3
i64.const 5 i32.const 1 array.new $a local.set $p4
i64.const 6 i32.const 1 array.new $a local.set $p5
i64.const 7 i32.const 1 array.new $a local.set $p6
i64.const 8 i32.const 1 array.new $a local.set $p7
i64.const 9 i32.const 1 array.new $a local.set $p8
  local.get $p0 local.get $p1 local.get $p2 local.get $p3 local.get $p4 local.get $p5 local.get $p6 local.get $p7 local.get $p8 call $leaf i64.const 45 i64.ne if unreachable end))
