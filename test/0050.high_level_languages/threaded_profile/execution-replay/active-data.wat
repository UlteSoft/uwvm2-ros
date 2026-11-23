(module
  (import "env" "memory" (memory 1 4 shared))
  (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
  (data (i32.const 128) "\2a")
  (func (export "wasi_thread_start") (param i32 i32)
    ;; Each actual new instance reapplies its active data before entry.
    i32.const 128 i32.load8_u i32.const 42 i32.ne if unreachable end
    i32.const 0 i32.const 1 i32.atomic.rmw.add drop
    i32.const 0 i32.const 1 memory.atomic.notify drop)
  (func $wait (param $n i32)
    block $done loop $again
      i32.const 0 i32.atomic.load local.get $n i32.eq br_if $done
      i32.const 0 i32.const 0 i64.const 1000000 memory.atomic.wait32 drop
      br $again
    end end)
  (func (export "_start")
    i32.const 128 i32.load8_u i32.const 42 i32.ne if unreachable end
    i32.const 128 i32.const 9 i32.store8
    i32.const 0 call $spawn i32.const 0 i32.le_s if unreachable end
    i32.const 1 call $wait
    i32.const 128 i32.const 99 i32.store8
    i32.const 0 call $spawn i32.const 0 i32.le_s if unreachable end
    i32.const 2 call $wait
    i32.const 128 i32.load8_u i32.const 42 i32.ne if unreachable end))
