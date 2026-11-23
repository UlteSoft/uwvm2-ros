(module
 (type $cell (struct (field i32)))
 (import "env" "memory" (memory 1 1 shared))
 (import "wasi" "thread-spawn" (func $spawn (param i32)(result i32)))
 (func (export "wasi_thread_start") (param i32 i32)
  i32.const 4 i32.const 1 i32.atomic.rmw.add drop
  block $ready loop $again
   i32.const 4 i32.atomic.load i32.const 2 i32.eq br_if $ready
   br $again end end
  i32.const 0 ref.i31 ref.cast (ref $cell) drop)
 (func (export "_start")
  i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
  i32.const 0 call $spawn i32.const 2 i32.ne if unreachable end
  loop $wait
   i32.const 0 i32.const 0 i64.const 1000000 memory.atomic.wait32 drop
   br $wait end))
