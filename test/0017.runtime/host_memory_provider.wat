;; Explicit Wasm backing for memory index 0 in the host API regression.
(module (memory (export "buffer") 0 2))
