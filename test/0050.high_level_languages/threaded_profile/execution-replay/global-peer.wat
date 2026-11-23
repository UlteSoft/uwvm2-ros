(module
 (type $cell (struct (field (mut i32))))
 (global (export "seed") i32 (i32.const 37))
 (global (export "counter") (mut i32) (i32.const 7))
 (global (export "cell") (mut (ref null $cell)) (ref.null $cell))
 (global $initialized (export "initialized") (mut i32) (i32.const 0))
 (func $start i32.const 41 global.set $initialized)
 (start $start))
