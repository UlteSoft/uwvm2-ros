(module
 (type $cell (struct (field (mut i32))))
 (import "env" "memory" (memory 1 1 shared))
 (import "wasi" "thread-spawn" (func $spawn (param i32)(result i32)))
 (import "peer" "seed" (global $seed i32))
 (import "peer" "counter" (global $counter (mut i32)))
 (import "peer" "cell" (global $cell (mut (ref null $cell))))
 (import "peer" "initialized" (global $initialized (mut i32)))
 (global $derived i32 (i32.add (global.get $seed) (i32.const 5)))
 (func $check (param $tid i32)
  global.get $derived i32.const 42 i32.ne if unreachable end
  global.get $initialized i32.const 41 i32.ne if unreachable end
  global.get $counter i32.const 7 i32.ne if unreachable end
  global.get $cell ref.is_null i32.eqz if unreachable end
  local.get $tid i32.const 99 i32.add global.set $counter
  local.get $tid struct.new $cell global.set $cell
  global.get $counter local.get $tid i32.const 99 i32.add i32.ne if unreachable end
  global.get $cell struct.get $cell 0 local.get $tid i32.ne if unreachable end)
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
   br $again end end
  global.get $counter i32.const 99 i32.ne if unreachable end
  global.get $cell struct.get $cell 0 i32.eqz if else unreachable end))
