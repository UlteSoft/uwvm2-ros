(module
(import "env" "memory" (memory 1 4 shared))
(import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
(import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
(func (export "wasi_thread_start") (param i32 i32)
 local.get 1 i32.const 1 i32.atomic.rmw.add drop
 i32.const 0 i32.const 1 memory.atomic.notify drop)
(func (export "_start")
 i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
 i32.const 0 call $spawn i32.const 2 i32.ne if unreachable end
 block $done loop $again
 i32.const 0 i32.atomic.load i32.const 2 i32.eq br_if $done
 i32.const 0 i32.const 0 i64.const 1000000 memory.atomic.wait32 drop
 br $again end end i32.const 73 call $exit unreachable))
