(module
(import "env" "memory" (memory 1 4 shared))
(import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
(func (export "wasi_thread_start") (param i32 i32)
 i32.const 0 memory.grow i32.const 1 i32.ne if unreachable end
 i32.const 1 memory.grow i32.const -1 i32.ne if unreachable end
 memory.size i32.const 1 i32.ne if unreachable end
 i32.const 0 i32.const 1 i32.atomic.rmw.add drop)
(func (export "_start")
 i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
 i32.const 0 call $spawn i32.const 2 i32.ne if unreachable end
 loop $again i32.const 0 i32.atomic.load i32.const 2 i32.ne br_if $again end))
