;; Use debug_checkpoint_actual_nested_reference_runtime with expected class "wrapper".
;; Pause before allocation, then resume a child returning nine distinct typed
;; carriers. Dropping the suffix must preserve the original externref/root.
(module
  (type $cell (struct (field (mut i32))))
  (type $bytes (array (mut i8)))
  (tag $raised)
  (func (export "_start") (result externref)
    call $child drop drop drop drop drop drop drop drop)
  (func $child (result externref i32 f32 i64 f64 v128 (ref $cell) (ref $bytes) exnref)
    i64.const 99 nop drop
    struct.new_default $cell extern.convert_any
    i32.const -17 f32.const -0 i64.const 12345 f64.const 3.5
    v128.const i64x2 1 2
    struct.new_default $cell
    i32.const 5 array.new_default $bytes
    block $caught (result exnref)
      try_table (catch_all_ref $caught) throw $raised end unreachable
    end))
