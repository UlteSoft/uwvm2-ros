(module
 (type $a (array (mut i64)))
 ;; Exercise FP register overflow, vectors, reference carriers and aggregate return storage.
 (func $leaf (param $f0 f64) (param $f1 f64) (param $f2 f64) (param $f3 f64) (param $f4 f64) (param $f5 f64) (param $f6 f64) (param $f7 f64) (param $f8 f64) (param $f9 f64) (param $p (ref null $a)) (param $v v128) (param $f f64)
   (result i64 f64 v128 (ref null $a))
  local.get $f0 f64.const 1 f64.ne if unreachable end
local.get $f1 f64.const 2 f64.ne if unreachable end
local.get $f2 f64.const 3 f64.ne if unreachable end
local.get $f3 f64.const 4 f64.ne if unreachable end
local.get $f4 f64.const 5 f64.ne if unreachable end
local.get $f5 f64.const 6 f64.ne if unreachable end
local.get $f6 f64.const 7 f64.ne if unreachable end
local.get $f7 f64.const 8 f64.ne if unreachable end
local.get $f8 f64.const 9 f64.ne if unreachable end
local.get $f9 f64.const 10 f64.ne if unreachable end
  local.get $p i32.const 0 array.get $a
  local.get $f local.get $v local.get $p)
 (func (export "_start") (local $p (ref null $a)) (local $q (ref null $a))
   (local $v v128) (local $f f64) (local $n i64)
  i64.const 0xfedcba9876543210 i32.const 1 array.new $a local.set $p
  f64.const 1 f64.const 2 f64.const 3 f64.const 4 f64.const 5 f64.const 6 f64.const 7 f64.const 8 f64.const 9 f64.const 10 local.get $p
  v128.const i32x4 0x01234567 0x89abcdef 0x80000000 0xffffffff
  i64.const 0x7ff0000000000042 f64.reinterpret_i64 call $leaf
  local.set $q local.set $v local.set $f local.set $n
  local.get $q local.get $p ref.eq i32.eqz if unreachable end
  local.get $n i64.const 0xfedcba9876543210 i64.ne if unreachable end
  local.get $f i64.reinterpret_f64 i64.const 0x7ff0000000000042 i64.ne if unreachable end
  local.get $v v128.const i32x4 0x01234567 0x89abcdef 0x80000000 0xffffffff
  v128.xor v128.any_true if unreachable end))
