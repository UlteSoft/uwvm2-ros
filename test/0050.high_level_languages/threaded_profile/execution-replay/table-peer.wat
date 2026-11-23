(module
 (type $f (func (param i32) (result i32)))
 (type $cell (struct (field (mut i32))))
 (table (export "functions") 4 8 funcref)
 (table (export "values") 4 8 (ref null eq))
 (table (export "wide") i64 2 4 funcref)
 (elem (i32.const 0) func $inc)
 (func $inc (type $f)(param i32)(result i32)local.get 0 i32.const 1 i32.add))
