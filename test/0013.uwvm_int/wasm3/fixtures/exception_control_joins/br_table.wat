(module
 (tag $e (param i32))
 (func $raise i32.const 73 throw $e)
 (func (export "_start")
  block (result i32 exnref)
   try_table (catch_ref $e 0)
block
 i32.const 0
 br_table 0 0
 unreachable
end
    call $raise
   end
   unreachable
  end
  drop
  i32.const 73
  i32.ne
  if unreachable end))
