;; Source fixture, not a generated/validated artifact. Root must compile and
;; independently admit its complete imports with the real threaded producer.
;; The read fd is selected explicitly by the Native contract's Root policy.
(module
  (import "wasi_snapshot_preview1" "fd_read"
    (func $read (param i32 i32 i32 i32) (result i32)))
  (import "wasi_snapshot_preview1" "poll_oneoff"
    (func $poll (param i32 i32 i32 i32) (result i32)))
  (memory (export "memory") 1 1 shared)
  (func (export "wasi_thread_start") (param i32 i32))
  (func (export "read_signal") (result i32)
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
      (then (return (i32.const -8))))
    (if (i32.ne (i32.load (i32.const 208)) (i32.const 1))
      (then (return (i32.const -9))))
    (if (i64.ne (i64.load (i32.const 176)) (i64.const 0x4711))
      (then (return (i32.const -10))))
    (if (i32.ne (i32.load16_u (i32.const 184)) (i32.const 0))
      (then (return (i32.const -11))))
    (if (i32.ne (i32.load8_u (i32.const 186)) (i32.const 1))
      (then (return (i32.const -12))))
    (if (i32.ne (i32.load16_u (i32.const 200)) (i32.const 0))
      (then (return (i32.const -13))))
    ;; Availability is advisory; this fixture has exactly one reader and the
    ;; writer sends atomic complete 32-byte frames. The actual read below must
    ;; still independently succeed and return the full extent.
    (if (i64.lt_u (i64.load (i32.const 192)) (i64.const 32))
      (then (return (i32.const -14))))
    ;; One iovec: destination [32,64); returned byte count at [8,12).
    (i32.store (i32.const 0) (i32.const 32))
    (i32.store (i32.const 4) (i32.const 32))
    (if (i32.ne (call $read (i32.const 3) (i32.const 0)
         (i32.const 1) (i32.const 8)) (i32.const 0))
      (then (return (i32.const -1))))
    (if (i32.ne (i32.load (i32.const 8)) (i32.const 32))
      (then (return (i32.const -2))))
    ;; Actual URS1/version/extent/cookie/sequence/reserved fields.
    (if (i32.ne (i32.load (i32.const 32)) (i32.const 0x31535255))
      (then (return (i32.const -3))))
    (if (i32.ne (i32.load (i32.const 36)) (i32.const 0x00200001))
      (then (return (i32.const -4))))
    (if (i64.ne (i64.load (i32.const 40)) (i64.const 0x105))
      (then (return (i32.const -5))))
    (if (i64.ne (i64.load (i32.const 48)) (i64.const 1))
      (then (return (i32.const -6))))
    (if (i32.ne (i32.load (i32.const 60)) (i32.const 0))
      (then (return (i32.const -7))))
    (i32.load (i32.const 56))))
