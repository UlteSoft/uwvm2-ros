(module
(import "env" "memory" (memory 1 1 shared))
(import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
(func (export "wasi_thread_start") (param i32 i32)
 i32.const 0 i32.const 1 i32.atomic.rmw.add drop
 loop $wait i32.const 4 i32.atomic.load i32.eqz br_if $wait end)
(func (export "_start") (local $tid i32)
 i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
 i32.const 0 call $spawn i32.const 2 i32.ne if unreachable end
 loop $ready i32.const 0 i32.atomic.load i32.const 2 i32.ne br_if $ready end
 i32.const 0 call $spawn i32.const -1 i32.ne if unreachable end
 i32.const 4 i32.const 1 i32.atomic.store
 loop $retry i32.const 0 call $spawn local.tee $tid i32.const -1 i32.eq br_if $retry end
 local.get $tid i32.const 0 i32.le_s if unreachable end
 loop $last i32.const 0 i32.atomic.load i32.const 3 i32.ne br_if $last end))
