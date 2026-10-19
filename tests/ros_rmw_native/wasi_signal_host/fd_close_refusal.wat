;; Merely importing this mutation primitive must reject the selected optional
;; signal profile BEFORE pipe creation. It need not be called to be forbidden.
(module
  (import "wasi_snapshot_preview1" "fd_close"
    (func $close (param i32) (result i32)))
  (memory (export "memory") 1 1 shared)
  (func (export "wasi_thread_start") (param i32 i32))
  (func (export "read_signal") (result i32) (i32.const 0)))
