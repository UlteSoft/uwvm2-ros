(module
 (memory 1)
 (func (export "_start")
  block
   try_table (catch_all 0)
    block br 0 end
    i32.const 65536 i32.load drop
   end
   return
  end
  unreachable))
