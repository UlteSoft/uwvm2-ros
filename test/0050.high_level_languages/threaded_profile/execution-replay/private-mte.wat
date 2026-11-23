(module
 (import "env" "memory" (memory 1 1 shared))
 (import "wasi" "thread-spawn" (func $spawn (param i32)(result i32)))
 (memory $small 1 2)
 (memory $wide i64 1 2)
 (data (memory $small) (i32.const 16) "ABCD")
 (data (memory $wide) (i64.const 16) "EFGH")
 (func $tag32 (param i32 i32)(result i32) local.get 0)
 (func $tag64 (param i64 i64)(result i64) local.get 0)
 (func $load32 (param i32)(result i32) local.get 0)
 (func $load64 (param i64)(result i64) local.get 0)
 (func $check (local $p i32)(local $q i64)
  i32.const 16 call $load32 local.tee $p i32.load $small i32.const 0x44434241 i32.ne if unreachable end
  local.get $p i32.const 28 i32.shr_u i32.eqz if unreachable end
  i64.const 16 call $load64 local.tee $q i32.load $wide i32.const 0x48474645 i32.ne if unreachable end
  local.get $q i64.const 56 i64.shr_u i64.eqz if unreachable end
  i32.const 128 i32.const 16 call $tag32 local.set $p
  i64.const 128 i64.const 16 call $tag64 local.set $q
  local.get $p i32.const 61 i32.store $small
  local.get $q i32.const 72 i32.store $wide
  local.get $p i32.load $small i32.const 61 i32.ne if unreachable end
  local.get $q i32.load $wide i32.const 72 i32.ne if unreachable end
  local.get $q local.get $p i32.const 4 memory.copy $wide $small
  local.get $q i32.load $wide i32.const 61 i32.ne if unreachable end
  local.get $q i32.const 42 i64.const 16 memory.fill $wide
  local.get $q v128.load $wide i8x16.extract_lane_u 15 i32.const 42 i32.ne if unreachable end)
 (func(export "wasi_thread_start")(param i32 i32) call $check
  i32.const 0 i32.const 1 i32.atomic.rmw.add drop i32.const 0 i32.const 1 memory.atomic.notify drop)
 (func(export "_start") call $check
  i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
  i32.const 0 call $spawn i32.const 2 i32.ne if unreachable end
  block $done loop $again
   i32.const 0 i32.atomic.load i32.const 2 i32.eq br_if $done
   i32.const 0 i32.const 0 i64.const 1000000 memory.atomic.wait32 drop br $again end end))
