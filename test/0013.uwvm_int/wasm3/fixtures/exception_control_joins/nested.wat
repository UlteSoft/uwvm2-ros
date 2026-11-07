(module
 (tag $e (param i32))
 (func $raise i32.const 73 throw $e)
 (func (export "_start")
  block (result i32 exnref)
   try_table (catch_ref $e 0)
block
 block
  block
   br 2
  end
  unreachable
 end
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
