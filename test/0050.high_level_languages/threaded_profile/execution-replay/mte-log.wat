(module
(import "env" "memory" (memory 1 4 shared))
(import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
(func $tag (param i32 i32)(result i32) local.get 0)
(func (export "wasi_thread_start") (param i32 i32)
(local $pointer i32)
 local.get 0 i32.const 64 i32.mul i32.const 1024 i32.add i32.const 16 call $tag local.set $pointer
 ;; Both real children must enter before the deliberately invalid access can
 ;; terminate the group in crash mode. Also exercise the same barrier in log mode.
 i32.const 4 i32.const 1 i32.atomic.rmw.add drop
 loop $ready i32.const 4 i32.atomic.load i32.const 2 i32.ne br_if $ready end
 local.get $pointer i32.const 0x10000000 i32.xor i32.load drop
 local.get $pointer i32.const 0x12345678 i32.store
 local.get $pointer i32.load i32.const 0x12345678 i32.ne if unreachable end
 i32.const 1 memory.grow i32.const -1 i32.eq if unreachable end
 i32.const 99 memory.grow i32.const -1 i32.ne if unreachable end

 local.get 1 i32.const 1 i32.atomic.rmw.add drop
 i32.const 0 i32.const 1 memory.atomic.notify drop)
(func (export "_start")
 i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
 i32.const 0 call $spawn i32.const 2 i32.ne if unreachable end
 block $done loop $again
 i32.const 0 i32.atomic.load i32.const 2 i32.eq br_if $done
 i32.const 0 i32.const 0 i64.const 1000000 memory.atomic.wait32 drop
 br $again end end))
