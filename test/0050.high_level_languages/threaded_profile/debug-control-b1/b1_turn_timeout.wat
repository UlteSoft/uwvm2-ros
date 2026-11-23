;; A real bulk opcode remains in flight after the host's one-millisecond
;; cooperative deadline. The sibling's actual native wait must stay parked.
(module
  (import "env" "memory" (memory 1024 1024 shared))
  (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
  (func (export "wasi_thread_start") (param $tid i32) (param i32)
    local.get $tid i32.const 1 i32.ne if unreachable end
    i32.const 60000 i32.const 1 i32.atomic.store
    i32.const 60000 i32.const -1 memory.atomic.notify drop
    i32.const 60016 i32.const 0 i64.const -1 memory.atomic.wait32 drop
    i32.const 60008 i32.const -1 i32.atomic.store
    unreachable)
  (func (export "_start")
    i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
    block $ready loop $wait
      i32.const 60000 i32.atomic.load br_if $ready
      i32.const 60000 i32.const 0 i64.const -1 memory.atomic.wait32 drop br $wait
    end end
    loop $main
      i32.const 60004 i32.const 60004 i32.atomic.load i32.const 1 i32.add i32.atomic.store
      i32.const 65536 i32.const 42 i32.const 33554432 memory.fill
      br $main
    end))
