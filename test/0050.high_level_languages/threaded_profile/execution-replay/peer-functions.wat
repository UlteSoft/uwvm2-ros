(module
 (type $cell (struct (field (mut i32))))
 (type $bounce (func (param (ref $cell)) (result (ref $cell))))
 (import "env" "memory" (memory 1 1 shared))
 (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
 (import "peer" "bounce" (func $bounce (type $bounce)))
 (import "peer" "reference" (func $reference (result (ref $bounce))))
 (elem declare func $bounce)
 (func $check (param $tid i32) (local $value (ref null $cell))
  local.get $tid i32.const 37 i32.add struct.new $cell local.tee $value ref.as_non_null
  call $bounce local.get $value ref.eq i32.eqz if unreachable end
  local.get $value ref.as_non_null ref.func $bounce call_ref $bounce local.get $value ref.eq i32.eqz if unreachable end
  local.get $value ref.as_non_null call $reference call_ref $bounce local.get $value ref.eq i32.eqz if unreachable end)
 (func (export "wasi_thread_start") (param $tid i32)(param i32)
  local.get $tid call $check
  i32.const 0 i32.const 1 i32.atomic.rmw.add drop
  i32.const 0 i32.const 1 memory.atomic.notify drop)
 (func (export "_start")
  i32.const 0 call $check
  i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
  i32.const 0 call $spawn i32.const 2 i32.ne if unreachable end
  block $done loop $again
   i32.const 0 i32.atomic.load i32.const 2 i32.eq br_if $done
   i32.const 0 i32.const 0 i64.const 1000000 memory.atomic.wait32 drop
   br $again end end))
