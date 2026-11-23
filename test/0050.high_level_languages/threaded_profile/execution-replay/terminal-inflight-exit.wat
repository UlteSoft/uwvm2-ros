(module
(import "env" "memory" (memory 1 4 shared))
(import "wasi" "thread-spawn" (func $spawn (param i32)(result i32)))
(import "wasi_snapshot_preview1" "fd_read" (func $read (param i32 i32 i32 i32)(result i32)))
(import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
(func (export "wasi_thread_start") (param $tid i32)(param i32)(local $n i32)
 local.get $tid i32.const 1 i32.eq if
  i32.const 0 i32.const 1 i32.atomic.store
  i32.const 0 i32.const 64 i32.const 1 i32.const 80 call $read drop
 else
  loop $ready i32.const 0 i32.atomic.load i32.eqz br_if $ready end
  i32.const 10000 local.set $n
  loop $delay local.get $n i32.const 1 i32.sub local.tee $n br_if $delay end
  i32.const 73 call $exit unreachable
 end)
(func (export "_start")
 i32.const 64 i32.const 128 i32.store i32.const 68 i32.const 1 i32.store
 i32.const 0 call $spawn drop i32.const 0 call $spawn drop
 loop $forever i32.const 4 i32.const 0 i64.const -1 memory.atomic.wait32 drop br $forever end))
