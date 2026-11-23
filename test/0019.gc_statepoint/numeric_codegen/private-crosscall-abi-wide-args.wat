(module
 (type $a (array (mut i64)))
 ;; All arguments are consumed; the carrier and later integers exceed native argument registers.
 (func $leaf (param $n0 i64) (param $n1 i64) (param $n2 i64) (param $n3 i64) (param $n4 i64) (param $n5 i64) (param $n6 i64) (param $n7 i64) (param $n8 i64) (param $n9 i64) (param $n10 i64) (param $n11 i64) (param $p (ref null $a)) (result i64)
  local.get $p i32.const 0 array.get $a
local.get $n0 i64.add
local.get $n1 i64.add
local.get $n2 i64.add
local.get $n3 i64.add
local.get $n4 i64.add
local.get $n5 i64.add
local.get $n6 i64.add
local.get $n7 i64.add
local.get $n8 i64.add
local.get $n9 i64.add
local.get $n10 i64.add
local.get $n11 i64.add)
 (func (export "_start") (local $p (ref null $a))
  i64.const 9 i32.const 1 array.new $a local.set $p
  i64.const 1 i64.const 2 i64.const 3 i64.const 4 i64.const 5 i64.const 6 i64.const 7 i64.const 8 i64.const 9 i64.const 10 i64.const 11 i64.const 12 local.get $p call $leaf
  i64.const 87 i64.ne if unreachable end))
