(module
(import "env" "memory" (memory 1 4 shared))
(import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
(import "wasi_snapshot_preview1" "fd_read" (func $read (param i32 i32 i32 i32)(result i32)))
(import "wasi_snapshot_preview1" "fd_write" (func $write (param i32 i32 i32 i32)(result i32)))

(func (export "wasi_thread_start") (param $tid i32) (param $arg i32) (local $ptr i32)
 local.get $tid i32.const 64 i32.mul local.set $ptr
 local.get $ptr i32.const 32 i32.add local.get $ptr i32.store
 local.get $ptr i32.const 36 i32.add i32.const 1 i32.store
 i32.const 0 local.get $ptr i32.const 32 i32.add i32.const 1 local.get $ptr i32.const 48 i32.add call $read if unreachable end
 local.get $ptr i32.const 32 i32.add i32.const 1024 i32.store
 local.get $ptr i32.const 36 i32.add i32.const 12 i32.store
 i32.const 1 local.get $ptr i32.const 32 i32.add i32.const 1 local.get $ptr i32.const 48 i32.add call $write if unreachable end
 i32.const 2048 local.get $tid i32.const 4 i32.mul i32.add local.get $ptr i32.load i32.store
 i32.const 0 i32.const 1 i32.atomic.rmw.add drop
 i32.const 0 i32.const 1 memory.atomic.notify drop)
(func (export "_start")
 i32.const 1024 i64.const 0x4f2d444145524854 i64.store
 i32.const 1032 i32.const 0x0a45434e i32.store
 i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
 i32.const 0 call $spawn i32.const 2 i32.ne if unreachable end
 block $done loop $again
 i32.const 0 i32.atomic.load i32.const 2 i32.eq br_if $done
 i32.const 0 i32.const 0 i64.const 1000000 memory.atomic.wait32 drop
 br $again end end))