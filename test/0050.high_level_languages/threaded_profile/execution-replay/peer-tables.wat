(module
 (type $f (func (param i32) (result i32)))
 (type $cell (struct (field (mut i32))))
 (import "env" "memory" (memory 1 1 shared))
 (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
 (import "peer" "functions" (table $functions 4 8 funcref))
 (import "peer" "values" (table $values 4 8 (ref null eq)))
 (import "peer" "wide" (table $wide i64 2 4 funcref))
 (elem (table $functions) (i32.const 0) func $override)
 (func $override (type $f)(param i32)(result i32)local.get 0 i32.const 2 i32.add)
 (elem $passive func $inc)
 (elem $gc (ref null eq) (ref.i31 (i32.const 27)) (struct.new $cell (i32.const 38)))
 (func $inc (type $f) (param i32) (result i32) local.get 0 i32.const 1 i32.add)
 (func $check (param $tid i32) (local $object (ref null $cell))
  table.size $functions i32.const 4 i32.ne if unreachable end
  i32.const 22 i32.const 0 call_indirect $functions (type $f) i32.const 24 i32.ne if unreachable end
  i32.const 0 ref.func $inc table.set $functions
  i32.const 41 i32.const 0 call_indirect $functions (type $f) i32.const 42 i32.ne if unreachable end
  i32.const 8 i32.const 0 table.get $functions ref.cast (ref $f) call_ref $f i32.const 9 i32.ne if unreachable end
  ref.func $inc i32.const 2 table.grow $functions i32.const 4 i32.ne if unreachable end
  ref.null func i32.const -1 table.grow $functions i32.const -1 i32.ne if unreachable end
  table.size $functions i32.const 6 i32.ne if unreachable end
  i32.const 1 ref.func $inc i32.const 3 table.fill $functions
  i32.const 2 i32.const 0 i32.const 4 table.copy $functions $functions
  i32.const 88 i32.const 5 call_indirect $functions (type $f) i32.const 89 i32.ne if unreachable end
  i32.const 0 i32.const 0 i32.const 1 table.init $functions $passive
  elem.drop $passive i32.const 6 i32.const 0 i32.const 0 table.init $functions $passive
  i32.const 0 i32.const 0 i32.const 2 table.init $values $gc
  i32.const 0 table.get $values ref.cast (ref i31) i31.get_u i32.const 27 i32.ne if unreachable end
  i32.const 1 table.get $values ref.cast (ref $cell) struct.get $cell 0 i32.const 38 i32.ne if unreachable end
  local.get $tid struct.new $cell local.set $object
  i32.const 0 local.get $object i32.const 4 table.fill $values
  i32.const 3 table.get $values local.get $object ref.eq i32.eqz if unreachable end
  local.get $object i32.const 2 table.grow $values i32.const 4 i32.ne if unreachable end
  i32.const 5 table.get $values local.get $object ref.eq i32.eqz if unreachable end
  elem.drop $gc
  i64.const 1 ref.func $inc table.set $wide
  i32.const 96 i64.const 1 call_indirect $wide (type $f) i32.const 97 i32.ne if unreachable end
  ref.func $inc i64.const 1 table.grow $wide i64.const 2 i64.ne if unreachable end
  table.size $wide i64.const 3 i64.ne if unreachable end
  i32.const 0 i64.const 1 i32.const 1 table.copy $functions $wide
  i32.const 72 i32.const 0 call_indirect $functions (type $f) i32.const 73 i32.ne if unreachable end)
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
