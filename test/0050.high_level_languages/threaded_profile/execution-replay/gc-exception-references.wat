(module
 (import "env" "memory" (memory 1 1 shared))
 (import "wasi" "thread-spawn" (func $spawn (param i32)(result i32)))
 (tag $tag (param i32))
 (func $yes (param i32) local.get 0 i32.eqz if unreachable end)
 (func $no (param i32) local.get 0 if unreachable end)
 (func $check (param $tid i32) (local $saved exnref)
  block $caught (result i32 exnref)
   try_table (catch_ref $tag $caught) local.get $tid throw $tag end
   unreachable
  end
  local.set $saved local.get $tid i32.eq call $yes
  local.get $saved ref.test (ref exn) call $yes
  local.get $saved ref.test (ref null exn) call $yes
  local.get $saved ref.test (ref null noexn) call $no
  ref.null exn ref.test (ref exn) call $no
  ref.null noexn ref.test (ref null noexn) call $yes
  ref.null exn ref.cast (ref null noexn) ref.is_null call $yes
  local.get $saved ref.null exn i32.const 1 select (result exnref)
  ref.test (ref exn) call $yes
  block $branch (result (ref exn))
   local.get $saved br_on_cast $branch (ref null exn) (ref exn)
   drop unreachable
  end ref.is_null call $no
  block $branch (result exnref)
   local.get $saved br_on_cast_fail $branch (ref null exn) (ref null noexn)
   drop unreachable
  end ref.is_null call $no
  block $rethrow (result i32)
   try_table (catch $tag $rethrow)
    local.get $saved ref.cast (ref exn) throw_ref
   end
   unreachable
  end local.get $tid i32.eq call $yes)
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
