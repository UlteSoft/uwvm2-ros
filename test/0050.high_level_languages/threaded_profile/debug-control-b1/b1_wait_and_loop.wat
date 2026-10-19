;; UNEXECUTED SOURCE. An actual wasi.thread-spawn child stays linked in a real
;; atomic.wait32 queue while the actual main makes ordinary decoded/LLVM polls.
(module
  (import "env" "memory" (memory 1 1 shared))
  (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
  (func (export "wasi_thread_start") (param $tid i32) (param i32)
    local.get $tid i32.const 1 i32.ne if unreachable end
    i32.const 60000 i32.const 1 i32.atomic.store
    i32.const 60000 i32.const -1 memory.atomic.notify drop
    i32.const 60016 i32.const 0 i64.const -1 memory.atomic.wait32
    ;; A pause alone must not become a guest result/cancellation/spurious notify.
    drop
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
      br $main
    end))
