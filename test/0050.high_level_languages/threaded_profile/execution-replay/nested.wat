(module
(import "env" "memory" (memory 1 1 shared))
(import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
(func $sum (param i32) (result i32) local.get 0 i32.const 17 i32.mul i32.const 3 i32.add)
(func (export "wasi_thread_start") (param $tid i32) (param $arg i32) (local $saved i32)
 local.get $arg call $sum local.set $saved
 local.get $arg i32.eqz if i32.const 2 call $spawn i32.const 0 i32.le_s if unreachable end end
 local.get $arg i32.const 4 i32.mul i32.const 16 i32.add local.get $saved i32.atomic.store
 i32.const 0 i32.const 1 i32.atomic.rmw.add drop
 loop $again i32.const 0 i32.atomic.load i32.const 3 i32.ne br_if $again end
 local.get $saved local.get $arg call $sum i32.ne if unreachable end)
(func (export "_start")
 i32.const 0 call $spawn i32.const 0 i32.le_s if unreachable end
 i32.const 1 call $spawn i32.const 0 i32.le_s if unreachable end
 loop $again i32.const 0 i32.atomic.load i32.const 3 i32.ne br_if $again end
 i32.const 16 i32.atomic.load i32.const 3 i32.ne if unreachable end
 i32.const 20 i32.atomic.load i32.const 20 i32.ne if unreachable end
 i32.const 24 i32.atomic.load i32.const 37 i32.ne if unreachable end))
