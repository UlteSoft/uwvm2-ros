(module
 (type $cell (struct (field (mut i32))))
 (type $cells (array (ref $cell)))
 (import "env" "memory" (memory 1 1 shared))
 (import "wasi" "thread-spawn" (func $spawn (param i32) (result i32)))
 (global $a (ref $cell) (struct.new $cell (i32.const 37)))
 (global $alias (ref $cell) (global.get $a))
 (global $array (ref $cells) (array.new $cells (global.get $a) (i32.const 3)))
 (global $i31 (ref i31) (ref.i31 (i32.const 4294967295)))
 (global $external (ref extern) (extern.convert_any (global.get $a)))
 (func $check (param $tid i32)
  global.get $a struct.get $cell 0 i32.const 37 i32.ne if unreachable end
  global.get $a global.get $alias ref.eq i32.eqz if unreachable end
  global.get $array i32.const 2 array.get $cells global.get $a ref.eq i32.eqz if unreachable end
  global.get $i31 i31.get_u i32.const 2147483647 i32.ne if unreachable end
  global.get $external any.convert_extern ref.cast (ref eq) global.get $a ref.eq i32.eqz if unreachable end
  local.get $tid if global.get $a i32.const 79 struct.set $cell 0 end)
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
   br $again end end
  global.get $a struct.get $cell 0 i32.const 37 i32.ne if unreachable end))
