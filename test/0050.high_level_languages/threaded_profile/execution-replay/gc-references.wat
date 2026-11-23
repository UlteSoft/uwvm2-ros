(module
 (type $base (sub (struct (field (mut i32)))))
 (type $child (sub $base (struct (field (mut i32)) (field i64))))
 (type $other (struct (field f64)))
 (type $array (array (mut i32)))
 (import "env" "memory" (memory 1 1 shared))
 (import "wasi" "thread-spawn" (func $spawn (param i32)(result i32)))
 (func $yes (param i32) local.get 0 i32.eqz if unreachable end)
 (func $no (param i32) local.get 0 if unreachable end)
 (func $check (param $tid i32)
  (local $a (ref null $child)) (local $b (ref null $child))
  (local $array (ref null $array)) (local $external externref)
  local.get $tid i64.const 987 struct.new $child local.set $a
  local.get $tid i64.const 987 struct.new $child local.set $b
  i32.const 7 i32.const 3 array.new $array local.set $array
  local.get $a local.get $a ref.eq call $yes
  local.get $a local.get $b ref.eq call $no
  local.get $a local.get $array ref.eq call $no
  local.get $array local.get $array ref.eq call $yes
  ref.null eq ref.null none ref.eq call $yes
  i32.const 0 ref.i31 ref.null eq ref.eq call $no
  i32.const -1 ref.i31 i32.const 2147483647 ref.i31 ref.eq call $yes
  i32.const 1 ref.i31 i32.const 2 ref.i31 ref.eq call $no
  local.get $a ref.test (ref $child) call $yes
  local.get $a ref.test (ref $base) call $yes
  local.get $a ref.test (ref $other) call $no
  local.get $a ref.test (ref struct) call $yes
  local.get $a ref.test (ref array) call $no
  local.get $a ref.test (ref eq) call $yes
  local.get $a ref.test (ref any) call $yes
  local.get $array ref.test (ref $array) call $yes
  i32.const 0 ref.i31 ref.test (ref i31) call $yes
  i32.const 0 ref.i31 ref.test (ref struct) call $no
  ref.null any ref.test (ref $child) call $no
  ref.null any ref.test (ref null $child) call $yes
  ref.null any ref.test (ref null none) call $yes
  local.get $a ref.test (ref null none) call $no
  local.get $a ref.cast (ref $base) struct.get $base 0 local.get $tid i32.eq call $yes
  ref.null any ref.cast (ref null $child) ref.is_null call $yes
  local.get $a local.get $b i32.const 1 select (result (ref null $child)) local.get $a ref.eq call $yes
  local.get $a local.get $b i32.const 0 select (result (ref null $child)) local.get $b ref.eq call $yes
  block $cast (result (ref $base))
   local.get $a br_on_cast $cast (ref null any) (ref $base)
   drop unreachable
  end
  local.get $a ref.eq call $yes
  block $cast (result i64 (ref $base))
   i64.const 123 local.get $a br_on_cast $cast (ref null any) (ref $base)
   drop drop unreachable
  end
  local.get $a ref.eq call $yes i64.const 123 i64.eq call $yes
  block $fail (result (ref null any))
   local.get $a br_on_cast_fail $fail (ref null any) (ref $array)
   drop unreachable
  end
  ref.cast (ref $base) struct.get $base 0 local.get $tid i32.eq call $yes
  block $fall (result (ref null any))
   local.get $a br_on_cast $fall (ref null any) (ref $array)
  end
  ref.cast (ref eq) local.get $a ref.eq call $yes
  block $fall (result (ref null any))
   local.get $a br_on_cast_fail $fall (ref null any) (ref $base)
   local.get $a ref.eq call $yes ref.null any
  end
  ref.is_null call $yes
  block $nullable (result (ref null $base))
   ref.null any br_on_cast $nullable (ref null any) (ref null $base)
   drop unreachable
  end ref.is_null call $yes
  block $null-fail (result (ref null any))
   ref.null any br_on_cast_fail $null-fail (ref null any) (ref $base)
   drop unreachable
  end ref.is_null call $yes
  local.get $a extern.convert_any local.tee $external ref.is_null call $no
  local.get $external any.convert_extern ref.cast (ref eq) local.get $a ref.eq call $yes
  i32.const 0 ref.i31 extern.convert_any any.convert_extern ref.cast (ref eq) i32.const 0 ref.i31 ref.eq call $yes
  ref.null any extern.convert_any ref.is_null call $yes
  ref.null extern any.convert_extern ref.is_null call $yes)
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
