;; Re-authored CLI fixture derived textually from the integrated Host14 reader.
;; Actual CLI profile imports a genuine preloaded shared memory and emits real
;; Guest readiness before Host OS signal delivery; failures use actual proc_exit.
;; Source fixture, not a generated/validated artifact. Root must compile and
;; independently admit its complete imports with the real threaded producer.
;; The read fd is selected explicitly by the Native contract's Root policy.
(module
  (import "wasi_snapshot_preview1" "fd_read"
    (func $read (param i32 i32 i32 i32) (result i32)))
  (import "wasi_snapshot_preview1" "poll_oneoff"
    (func $poll (param i32 i32 i32 i32) (result i32)))
  (import "shared_memory" "memory" (memory 1 1 shared))
  (import "wasi_snapshot_preview1" "fd_write"
    (func $write (param i32 i32 i32 i32) (result i32)))
  (import "wasi_snapshot_preview1" "proc_exit" (func $exit (param i32)))
  (export "memory" (memory 0))
  (data (i32.const 256) "CLI_SIGNAL_READY\0a")
  (data (i32.const 288) "CLI_SIGNAL_INT\0a")
  (data (i32.const 320) "CLI_SIGNAL_TERM\0a")
  (func $say (param $p i32) (param $n i32)
    (i32.store (i32.const 0) (local.get $p))
    (i32.store (i32.const 4) (local.get $n))
    (if (i32.ne (call $write (i32.const 1) (i32.const 0) (i32.const 1) (i32.const 8)) (i32.const 0))
      (then (call $exit (i32.const 80)) (unreachable)))
    (if (i32.ne (i32.load (i32.const 8)) (local.get $n))
      (then (call $exit (i32.const 81)) (unreachable))))
  (func (export "wasi_thread_start") (param i32 i32))
  (func (export "_start")
    (call $say (i32.const 256) (i32.const 17))
    ;; Real wasm32 WASI layout from the repository poll_oneoff structs and
    ;; fixed wasi-libc SDK33 original header: subscription [128,176), event
    ;; [176,208), nevents [208,212). Exactly one FD_READ subscription, no clock.
    ;; The private Host writer can send after asynchronous main has started;
    ;; an empty nonblocking pipe must not become an EAGAIN test failure.
    (i64.store (i32.const 128) (i64.const 0x4711))
    (i32.store8 (i32.const 136) (i32.const 1))
    (i32.store (i32.const 144) (i32.const 3))
    (if (i32.ne (call $poll (i32.const 128) (i32.const 176)
         (i32.const 1) (i32.const 208)) (i32.const 0))
      (then (call $exit (i32.const 28)) (unreachable)))
    (if (i32.ne (i32.load (i32.const 208)) (i32.const 1))
      (then (call $exit (i32.const 29)) (unreachable)))
    (if (i64.ne (i64.load (i32.const 176)) (i64.const 0x4711))
      (then (call $exit (i32.const 30)) (unreachable)))
    (if (i32.ne (i32.load16_u (i32.const 184)) (i32.const 0))
      (then (call $exit (i32.const 31)) (unreachable)))
    (if (i32.ne (i32.load8_u (i32.const 186)) (i32.const 1))
      (then (call $exit (i32.const 32)) (unreachable)))
    (if (i32.ne (i32.load16_u (i32.const 200)) (i32.const 0))
      (then (call $exit (i32.const 33)) (unreachable)))
    ;; Availability is advisory; this fixture has exactly one reader and the
    ;; writer sends atomic complete 32-byte frames. The actual read below must
    ;; still independently succeed and return the full extent.
    (if (i64.lt_u (i64.load (i32.const 192)) (i64.const 32))
      (then (call $exit (i32.const 34)) (unreachable)))
    ;; One iovec: destination [32,64); returned byte count at [8,12).
    (i32.store (i32.const 0) (i32.const 32))
    (i32.store (i32.const 4) (i32.const 32))
    (if (i32.ne (call $read (i32.const 3) (i32.const 0)
         (i32.const 1) (i32.const 8)) (i32.const 0))
      (then (call $exit (i32.const 21)) (unreachable)))
    (if (i32.ne (i32.load (i32.const 8)) (i32.const 32))
      (then (call $exit (i32.const 22)) (unreachable)))
    ;; Actual URS1/version/extent/cookie/sequence/reserved fields.
    (if (i32.ne (i32.load (i32.const 32)) (i32.const 0x31535255))
      (then (call $exit (i32.const 23)) (unreachable)))
    (if (i32.ne (i32.load (i32.const 36)) (i32.const 0x00200001))
      (then (call $exit (i32.const 24)) (unreachable)))
    (if (i64.ne (i64.load (i32.const 40)) (i64.const 0x105))
      (then (call $exit (i32.const 25)) (unreachable)))
    (if (i64.ne (i64.load (i32.const 48)) (i64.const 1))
      (then (call $exit (i32.const 26)) (unreachable)))
    (if (i32.ne (i32.load (i32.const 60)) (i32.const 0))
      (then (call $exit (i32.const 27)) (unreachable)))
    (if (i32.eq (i32.load (i32.const 56)) (i32.const 1))
      (then (call $say (i32.const 288) (i32.const 15)))
      (else
        (if (i32.ne (i32.load (i32.const 56)) (i32.const 2))
          (then (call $exit (i32.const 60)) (unreachable)))
        (call $say (i32.const 320) (i32.const 16))))
    (call $exit (i32.const 0))
    (unreachable)))
