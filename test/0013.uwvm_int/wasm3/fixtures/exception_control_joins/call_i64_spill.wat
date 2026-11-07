(module
 (func $add (param i64 i64) (result i64)
  local.get 0 local.get 1 i64.add)
 (func (export "_start")
  i64.const 0 i64.const 2147483647 call $add
  i64.const 2147483647 i64.ne if unreachable end
  i64.const 0 i64.const 4294967295 call $add
  i64.const 4294967295 i64.ne if unreachable end
  i64.const 4294967295 i64.const 4294967296 call $add
  i64.const 8589934591 i64.ne if unreachable end))
