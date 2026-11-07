;; ARM base-AAPCS helpers must remain private to LLVM lowering.
(module
  (import "env" "__aeabi_l2d" (func $helper (param i64) (result f64)))
  (func (export "_start") (drop (call $helper (i64.const 27)))))
