(module
(import "env" "memory" (memory 1 1 shared))
(import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
(func (export "wasi_thread_start") (param $tid i32) (param $arg i32) (local $value i32)
 local.get $tid i32.const 37 i32.mul local.set $value
 loop $again local.get $value i32.const 1 i32.add local.set $value
 local.get $tid i32.const 4 i32.mul local.get $value i32.atomic.store
 br $again end)
(func (export "_start")
 i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
 i32.const 0 call $spawn i32.const 2 i32.ne if unreachable end
 loop $again i32.const 0 i32.const 1 i32.atomic.rmw.add drop br $again end))
