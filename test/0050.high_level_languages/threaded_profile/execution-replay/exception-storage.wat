(module
 (import "env" "memory" (memory 1 1 shared))
 (import "wasi" "thread-spawn" (func $spawn (param i32)(result i32)))
 (type $box (struct (field (mut exnref))))
 (type $object (struct (field i32)))
 (tag $tag (param i32 (ref $object)))
 (tag $nested (param exnref))
 (table $refs 2 8 exnref)
 (table $wide i64 2 8 exnref)
 (global $saved (mut exnref) (ref.null exn))
 (func $yes (param i32) local.get 0 i32.eqz if unreachable end)
 (func $identity (param exnref)(result exnref) local.get 0)
 (func $check (param $tid i32)(local $ref exnref)(local $box (ref null $box))
  global.get $saved ref.is_null call $yes
  block $caught (result i32 (ref $object) exnref)
   try_table (catch_ref $tag $caught)
    local.get $tid i32.const 41 struct.new $object throw $tag
   end unreachable
  end
  local.set $ref struct.get $object 0 i32.const 41 i32.eq call $yes
  local.get $tid i32.eq call $yes
  local.get $ref call $identity global.set $saved
  i32.const 0 global.get $saved table.set $refs
  i64.const 0 global.get $saved table.set $wide
  i32.const 1 i32.const 0 i32.const 1 table.copy $refs $refs
  i32.const 1 table.get $refs struct.new $box local.set $box
  local.get $box global.get $saved struct.set $box 0
  global.get $saved i64.const 1 table.grow $wide i64.const 2 i64.eq call $yes
  block $nested_caught (result exnref)
   try_table (catch $nested $nested_caught)
    local.get $box struct.get $box 0 throw $nested
   end unreachable
  end local.set $ref
  block $again (result i32 (ref $object))
   try_table (catch $tag $again) local.get $ref throw_ref end unreachable
  end struct.get $object 0 i32.const 41 i32.eq call $yes
  local.get $tid i32.eq call $yes
  block $again2 (result i32 (ref $object))
   try_table (catch $tag $again2) i64.const 2 table.get $wide throw_ref end unreachable
  end struct.get $object 0 i32.const 41 i32.eq call $yes
  local.get $tid i32.eq call $yes)
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
