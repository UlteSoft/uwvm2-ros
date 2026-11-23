(module
 (import "env" "memory" (memory $primary 1 1 shared))
 (import "env" "wide" (memory $wide i64 1 4 shared))
 (import "env" "wide-alias" (memory $alias i64 1 4 shared))
 (import "env" "small" (memory $small 1 4 shared))
 (import "other" "memory" (memory $other i64 1 2 shared))
 (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
 (data $data "abcdefgh")
 (func (export "wasi_thread_start") (param $tid i32) (param i32) (local $offset i64)
  local.get $tid i64.extend_i32_u i64.const 256 i64.mul local.set $offset
  local.get $offset i32.const 0 i32.const 8 memory.init $wide $data
  local.get $offset i64.load $alias i64.const 0x6867666564636261 i64.ne if unreachable end
  local.get $offset i32.wrap_i64 local.get $offset i32.const 8 memory.copy $small $wide
  local.get $offset local.get $offset i32.wrap_i64 i32.const 8 memory.copy $other $small
  local.get $offset i64.load $other i64.const 0x6867666564636261 i64.ne if unreachable end
  local.get $offset i64.const 16 i64.add i32.const 42 i64.const 16 memory.fill $wide
  local.get $offset i64.const 16 i64.add v128.load $alias i8x16.extract_lane_u 15 i32.const 42 i32.ne if unreachable end
  i64.const 0 i64.const 1 i64.atomic.rmw.add $wide drop
  i64.const 8 i32.const 1 i64.const 0 memory.atomic.wait32 $alias i32.const 1 i32.ne if unreachable end
  i64.const 16 i64.const 1 i64.const 0 memory.atomic.wait64 $other i32.const 1 i32.ne if unreachable end
  i64.const 8 i32.const 1 memory.atomic.notify $wide drop
  i64.const 1 memory.grow $wide i64.const -1 i64.eq if unreachable end
  i64.const 0x100000000 memory.grow $wide i64.const -1 i64.ne if unreachable end
  i32.const 1 memory.grow $small i32.const -1 i32.eq if unreachable end
  i32.const 0 i32.const 1 i32.atomic.rmw.add $primary drop
  i32.const 0 i32.const 1 memory.atomic.notify $primary drop)
 (func (export "_start")
  i64.const 1 memory.grow $wide i64.const 1 i64.ne if unreachable end
  i32.const 1 memory.grow $small i32.const 1 i32.ne if unreachable end
  i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
  i32.const 0 call $spawn i32.const 2 i32.ne if unreachable end
  block $done loop $again
   i32.const 0 i32.atomic.load $primary i32.const 2 i32.eq br_if $done
   i32.const 0 i32.const 0 i64.const 1000000 memory.atomic.wait32 $primary drop
   br $again end end
  memory.size $alias i64.const 4 i64.ne if unreachable end
  memory.size $small i32.const 4 i32.ne if unreachable end
  i64.const 0 i64.atomic.load $alias i64.const 2 i64.ne if unreachable end))
