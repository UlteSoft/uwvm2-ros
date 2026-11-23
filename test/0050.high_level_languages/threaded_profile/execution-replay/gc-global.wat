(module
  (type $box (struct (field (mut i32))))
  (import "env" "memory" (memory 1 1 shared))
  (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
  (global $private (mut (ref null $box)) (ref.null $box))
  (func $init i32.const 7 struct.new $box global.set $private)
  (start $init)
  (func (export "wasi_thread_start") (param i32 i32)
    global.get $private struct.get $box 0 i32.const 7 i32.ne if unreachable end
    global.get $private i32.const 11 struct.set $box 0
    i32.const 8 i32.const 1 i32.atomic.rmw.add drop)
  (func (export "_start")
    global.get $private i32.const 99 struct.set $box 0
    i32.const 0 call $spawn i32.const 0 i32.lt_s if unreachable end
    i32.const 0 call $spawn i32.const 0 i32.lt_s if unreachable end
    (loop $wait i32.const 8 i32.atomic.load i32.const 2 i32.ne br_if $wait)
    global.get $private struct.get $box 0 i32.const 99 i32.ne if unreachable end))
