;; A write-free call graph is insufficient: whole-import admission must reject
;; a producer that could replace the trusted read slot through fd_renumber.
(module
  (import "wasi_snapshot_preview1" "fd_renumber"
    (func $renumber (param i32 i32) (result i32)))
  (memory (export "memory") 1 1 shared)
  (func (export "wasi_thread_start") (param i32 i32))
  (func (export "read_signal") (result i32) (i32.const 0)))
