(module
(import "env" "memory" (memory 1 4 shared))
(import "wasi" "thread-spawn" (func $spawn (param i32)(result i32)))
(func $tag (param i32 i32)(result i32) local.get 0)
(func (export "wasi_thread_start") (param $tid i32)(param i32)(local $pointer i32)
 local.get $tid i32.const 64 i32.mul i32.const 1024 i32.add i32.const 16 call $tag local.set $pointer
 i32.const 4 i32.const 1 i32.atomic.rmw.add drop
 loop $ready i32.const 4 i32.atomic.load i32.const 2 i32.ne br_if $ready end
 local.get $pointer i32.const 0x10000000 i32.xor i32.load drop)
(func (export "_start")
 i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
 i32.const 0 call $spawn i32.const 2 i32.ne if unreachable end
 loop $again i32.const 0 i32.const 0 i64.const -1 memory.atomic.wait32 drop br $again end))
