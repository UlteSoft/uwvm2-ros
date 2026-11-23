(module
  (type $result (func (result i32)))
  (import "env" "memory" (memory 1 4 shared))
  (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
  (global $answer i32 (i32.add (i32.const 19) (i32.const 23)))
  (global $private (mut i32) (i32.const 0))
  (table 4 funcref)
  (elem (i32.add (i32.const 1) (i32.const 1)) funcref (ref.func $answer))
  (func $answer (type $result) global.get $answer)
  (func $init
    i32.const 2 call_indirect (type $result) i32.const 42 i32.ne if unreachable end
    global.get $private if unreachable end
    i32.const 7 global.set $private
    ;; A normal start can use control flow and an ordinary helper call.
    block i32.const 2 call_indirect (type $result) i32.const 42 i32.eq br_if 0 unreachable end
    i32.const 16 i32.const 1 i32.atomic.rmw.add drop)
  (start $init)
  (func (export "wasi_thread_start") (param i32 i32)
    global.get $private i32.const 7 i32.ne if unreachable end
    i32.const 0 i32.const 1 i32.atomic.rmw.add drop
    i32.const 0 i32.const 1 memory.atomic.notify drop)
  (func (export "_start")
    global.get $private i32.const 7 i32.ne if unreachable end
    i32.const 16 i32.atomic.load i32.const 1 i32.ne if unreachable end
    i32.const 99 global.set $private
    i32.const 0 call $spawn i32.const 0 i32.le_s if unreachable end
    i32.const 0 call $spawn i32.const 0 i32.le_s if unreachable end
    block $done loop $wait
      i32.const 0 i32.atomic.load i32.const 2 i32.eq br_if $done
      i32.const 0 i32.const 0 i64.const 1000000 memory.atomic.wait32 drop
      br $wait
    end end
    global.get $private i32.const 99 i32.ne if unreachable end
    i32.const 16 i32.atomic.load i32.const 3 i32.ne if unreachable end))
