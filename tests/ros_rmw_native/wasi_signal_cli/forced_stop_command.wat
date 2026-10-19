;; Native-contract negative: Guest deliberately never consumes the signal frame.
;; The real engine must observe genuine Group forced cancellation after grace.
(module
  (import "shared_memory" "memory" (memory 1 1 shared))
  (import "wasi_snapshot_preview1" "fd_write"
    (func $write (param i32 i32 i32 i32) (result i32)))
  (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
  (export "memory" (memory 0))
  (data (i32.const 256) "CLI_SIGNAL_READY\0a")
  (func (export "wasi_thread_start") (param i32 i32))
  (func (export "_start")
    (i32.store (i32.const 0) (i32.const 256))
    (i32.store (i32.const 4) (i32.const 17))
    (if (i32.ne (call $write (i32.const 1) (i32.const 0) (i32.const 1) (i32.const 8)) (i32.const 0))
      (then (call $exit (i32.const 80)) (unreachable)))
    (if (i32.ne (i32.load (i32.const 8)) (i32.const 17))
      (then (call $exit (i32.const 81)) (unreachable)))
    (loop $busy (br $busy))))
