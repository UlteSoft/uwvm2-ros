(module
 (type $cell (struct (field (mut i32))))
 (type $bounce (func (param (ref $cell)) (result (ref $cell))))
 (func $bounce (export "bounce") (type $bounce) (param (ref $cell)) (result (ref $cell)) local.get 0)
 (elem declare func $bounce)
 (func (export "reference") (result (ref $bounce)) ref.func $bounce))
