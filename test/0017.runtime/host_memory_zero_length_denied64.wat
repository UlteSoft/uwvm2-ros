;; Memory64 counterpart: offsets/lengths retain their full 64-bit values.
;; The same real host entry must refuse even zero bytes when memory access is none.
(module
  (import "host-memory-bounds" "probe" (func $probe (param i32 i64 i32) (result i32)))
  (memory i64 0 2)
  (memory i64 2 4)
  (func (export "_start")
    (if (call $probe (i32.const 0) (i64.const 0) (i32.const 0)) (then unreachable))
    (if (call $probe (i32.const 1) (i64.const 131072) (i32.const 0)) (then unreachable))
    (if (call $probe (i32.const -1) (i64.const 0) (i32.const 0)) (then unreachable))))
