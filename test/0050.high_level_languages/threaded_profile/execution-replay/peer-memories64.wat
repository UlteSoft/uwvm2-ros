(module
 (import "env" "memory" (memory $shared 1 1 shared))
 (import "peer" "memory" (memory $private i64 1 2))
 (import "wasi" "thread-spawn" (func $spawn (param i32)(result i32)))
 (import "peer" "touch" (func $touch))
 (func $check
  call $touch
  i64.const 0 i32.load $private i32.const 0x1234 i32.ne if unreachable end
  i64.const 16 i64.const 0 i64.const 4 memory.copy $private $private
  i64.const 16 i32.load $private i32.const 0x1234 i32.ne if unreachable end)
 (func(export "wasi_thread_start")(param i32 i32) call $check
  i32.const 0 i32.const 1 i32.atomic.rmw.add drop i32.const 0 i32.const 1 memory.atomic.notify drop)
 (func(export "_start") call $check
  i32.const 0 i32.load $shared if unreachable end
  i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
  i32.const 0 call $spawn i32.const 2 i32.ne if unreachable end
  block $done loop $again
   i32.const 0 i32.atomic.load i32.const 2 i32.eq br_if $done
   i32.const 0 i32.const 0 i64.const 1000000 memory.atomic.wait32 drop br $again end end))
