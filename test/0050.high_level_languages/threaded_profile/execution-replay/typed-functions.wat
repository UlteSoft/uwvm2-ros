(module
 (type $spawn-type (func (param i32) (result i32)))
 (type $base (sub (func (param i32) (result i32))))
 (type $sub (sub $base (func (param i32) (result i32))))
 (type $random (func (param i32 i32) (result i32)))
 (import "env" "memory" (memory 1 1 shared))
 (import "wasi" "thread-spawn" (func $spawn (type $spawn-type)))
 (import "wasi_snapshot_preview1" "random_get" (func $random (type $random)))
 (elem declare func $inc $random)
 (func $inc (type $sub) (param i32) (result i32) local.get 0 i32.const 1 i32.add)
 (func $invoke (param i32) (param (ref null $base)) (result i32)
  local.get 0 local.get 1 call_ref $base)
 (func $tail (param i32) (param (ref null $base)) (result i32)
  local.get 0 local.get 1 return_call_ref $base)
 (func $check (param $tid i32) (local $f (ref null $base))
  ref.func $inc local.set $f
  i32.const 41 local.get $f call $invoke i32.const 42 i32.ne if unreachable end
  i32.const 98 local.get $f call $tail i32.const 99 i32.ne if unreachable end
  local.get $tid i32.const 16 i32.mul i32.const 128 i32.add i32.const 8 ref.func $random
  call_ref $random if unreachable end)
 (func (export "wasi_thread_start") (param $tid i32)(param i32)
  local.get $tid call $check
  i32.const 0 i32.const 1 i32.atomic.rmw.add drop
  i32.const 0 i32.const 1 memory.atomic.notify drop)
 (func (export "_start")
  i32.const 0 call $check
  i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
  i32.const 0 call $spawn i32.const 2 i32.ne if unreachable end
  block $done loop $again
   i32.const 0 i32.atomic.load i32.const 2 i32.eq br_if $done
   i32.const 0 i32.const 0 i64.const 1000000 memory.atomic.wait32 drop
   br $again end end))
