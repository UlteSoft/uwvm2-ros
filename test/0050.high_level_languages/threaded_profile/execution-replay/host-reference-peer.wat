(module
 (type $random (func (param i32 i32) (result i32)))
 (import "env" "memory" (memory 1 1 shared))
 (import "wasi_snapshot_preview1" "random_get" (func $random (type $random)))
 (elem declare func $random)
 (func (export "reference") (result (ref $random)) ref.func $random))
