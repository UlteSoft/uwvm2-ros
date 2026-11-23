(module
(import "env" "memory" (memory 1 4 shared))
(import "wasi" "thread-spawn" (func $spawn (param i32)(result i32)))
(func $tag (param i32 i32)(result i32) local.get 0)
(func (export "wasi_thread_start") (param $tid i32)(param i32)(local $p i32)
 i32.const 16 i32.load local.set $p
 local.get $tid i32.const 1 i32.eq
 if
  local.get $p i32.const 1 i64.const -1 memory.atomic.wait32 i32.const 1 i32.ne if unreachable end
  local.get $p i32.const 0 i64.const 0 memory.atomic.wait32 i32.const 2 i32.ne if unreachable end
  local.get $p i32.const 0 i64.const -1 memory.atomic.wait32 if unreachable end
  local.get $p i32.const 8 i32.add i64.const 0 i64.const -1 memory.atomic.wait64 if unreachable end
 else
  loop $first local.get $p i32.const 1 memory.atomic.notify i32.eqz br_if $first end
  loop $second local.get $p i32.const 8 i32.add i32.const 1 memory.atomic.notify i32.eqz br_if $second end
 end
 i32.const 0 i32.const 1 i32.atomic.rmw.add drop
 i32.const 0 i32.const 1 memory.atomic.notify drop)
(func (export "_start")
 i32.const 16 i32.const 2048 i32.const 16 call $tag i32.store
 i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
 i32.const 0 call $spawn i32.const 2 i32.ne if unreachable end
 block $done loop $again
 i32.const 0 i32.atomic.load i32.const 2 i32.eq br_if $done
 i32.const 0 i32.const 0 i64.const 1000000 memory.atomic.wait32 drop
 br $again end end))
