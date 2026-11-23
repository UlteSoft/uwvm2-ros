(module
(import "env" "memory" (memory 1 1 shared))
(import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
(func (export "wasi_thread_start") (param i32 i32)
 i32.const 60000 i32.const 1 i32.atomic.store)
(func (export "_start")
 i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
 (loop $main i32.const 60004 i32.const 60004 i32.atomic.load i32.const 1 i32.add i32.atomic.store br $main)))
