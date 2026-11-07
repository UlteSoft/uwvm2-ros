(module
 (tag $caught (param i32)) (tag $other (param i32))
 (func $raise i32.const 73 throw $other)
 (func (export "_start")
  block (result i32)
   try_table (catch $caught 0)
    block br 0 end
    call $raise
   end
   unreachable
  end
  drop))
