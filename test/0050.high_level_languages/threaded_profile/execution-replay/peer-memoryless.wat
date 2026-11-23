(module
  (import "env" "memory" (memory 1 1 shared))
  (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
  (import "peer" "get" (func $get (result i32)))
  (import "peer" "set" (func $set (param i32)))
  (func $init call $get i32.const 17 i32.ne if unreachable end)
  (start $init)
  (func (export "wasi_thread_start") (param $tid i32) (param i32)
    call $get i32.const 17 i32.ne if unreachable end
    local.get $tid call $set
    call $get local.get $tid i32.ne if unreachable end
    i32.const 8 i32.const 1 i32.atomic.rmw.add drop)
  (func (export "_start")
    i32.const 99 call $set
    i32.const 0 call $spawn i32.const 0 i32.lt_s if unreachable end
    i32.const 0 call $spawn i32.const 0 i32.lt_s if unreachable end
    (loop $wait i32.const 8 i32.atomic.load i32.const 2 i32.ne br_if $wait)
    call $get i32.const 99 i32.ne if unreachable end))
