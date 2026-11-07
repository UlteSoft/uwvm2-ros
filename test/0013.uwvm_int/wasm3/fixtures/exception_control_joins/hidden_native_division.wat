;; Native compiler-runtime relocation helpers are not guest host imports.
(module
  (import "env" "__divdi3" (func $helper (param i64 i64) (result i64)))
  (func (export "_start") (drop (call $helper (i64.const 27) (i64.const 3)))))
