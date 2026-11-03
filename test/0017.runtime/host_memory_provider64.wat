;; Memory64 counterpart: offsets/lengths retain their full 64-bit values.
;; Explicit Wasm backing for memory index 0 in the host API regression.
(module (memory (export "buffer") i64 0 2))
