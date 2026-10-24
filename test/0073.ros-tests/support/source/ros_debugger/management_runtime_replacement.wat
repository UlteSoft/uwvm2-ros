;; Official-assembler cross-check for the native component's complete body
;; constants. Extract Code-section bodies including their local-declaration
;; count: function0 = 00 41 09 0b; function1 = 00 01 0b.
(module
  (func (export "value9") (result i32) i32.const 9)
  (func (export "active_void") nop))
