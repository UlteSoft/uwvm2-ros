(module
(import "env" "memory" (memory 1 4 shared))
(import "wasi" "thread-spawn" (func $spawn (param i32)(result i32)))
(func(export "wasi_thread_start")(param $tid i32)(param i32)
 i32.const 0 i32.const 1 i32.atomic.rmw.add drop
 block $ready loop $wait
 i32.const 0 i32.atomic.load i32.const 2 i32.eq br_if $ready
 i32.const 0 i32.const 0 i64.const 1000000 memory.atomic.wait32 drop br $wait end end
 local.get $tid i32.const 1 i32.eq if i32.const 65536 i32.load drop end
 loop $forever i32.const 4 i32.const 0 i64.const -1 memory.atomic.wait32 drop br $forever end)
(func(export "_start")
 i32.const 0 call $spawn drop i32.const 0 call $spawn drop
 loop $forever i32.const 4 i32.const 0 i64.const -1 memory.atomic.wait32 drop br $forever end))
