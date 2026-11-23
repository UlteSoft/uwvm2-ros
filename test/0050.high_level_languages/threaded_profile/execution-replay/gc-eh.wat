(module
 (type $cell (struct (field (mut i32)) (field (mut (ref null $cell)))))
 (type $num (func (param i32)(result i32)))
 (import "env" "memory" (memory 1 1 shared))
 (import "wasi" "thread-spawn" (func $spawn (param i32)(result i32)))
 (tag $failure (param i32))
 (func $compute (type $num) local.get 0 i32.const 17 i32.mul)
 (table 1 funcref) (elem (i32.const 0) $compute)
 (func (export "wasi_thread_start") (param $tid i32)(param i32)(local $cell (ref null $cell))
  struct.new_default $cell local.set $cell
  local.get $cell local.get $tid struct.set $cell 0
  local.get $cell local.get $cell struct.set $cell 1
  block $caught (result i32)
   try_table (catch $failure $caught) local.get $tid throw $failure end
   unreachable
  end
  local.get $tid i32.ne if unreachable end
  local.get $cell struct.get $cell 1 struct.get $cell 0 i32.const 0 call_indirect (type $num)
  local.get $tid i32.const 17 i32.mul i32.ne if unreachable end
  i32.const 0 i32.const 1 i32.atomic.rmw.add drop
  i32.const 0 i32.const 1 memory.atomic.notify drop)
 (func(export "_start")
  i32.const 0 call $spawn i32.const 1 i32.ne if unreachable end
  i32.const 0 call $spawn i32.const 2 i32.ne if unreachable end
  block $done loop $again
   i32.const 0 i32.atomic.load i32.const 2 i32.eq br_if $done
   i32.const 0 i32.const 0 i64.const 1000000 memory.atomic.wait32 drop
   br $again end end))
