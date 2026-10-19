;; Actual LLVM-full business-import fixture. Assemble with the selected genuine
;; wat2wasm. Three independent initialized instances use these same bytes under
;; management-main, management-other and management-cold; no import connects
;; those instances and no Guest imports a management/debug operation.
(module
  (import "uwvm_ros1_v1" "capability_open"
    (func $open (param i32 i64 i64 i64) (result i32)))
  (import "uwvm_ros1_v1" "capability_close"
    (func $close (param i64) (result i32)))
  (memory (export "memory") 1 1)
  (data (i32.const 16) "sub")
  ;; Function indices include the two real builtin imports.
  (func $value (export "value") (result i32) i32.const 7) ;; index 2
  (func $initialize (export "initialize") (result i32) ;; index 3
    (if (call $open (i32.const 0) (i64.const 16) (i64.const 3) (i64.const 64)) (then unreachable))
    (if (call $open (i32.const 0) (i64.const 16) (i64.const 3) (i64.const 72)) (then unreachable))
    i32.const 42)
  (func $fresh (export "fresh") (param $expected i32) (result i32) (local $status i32) ;; index 4
    (i64.store (i32.const 80) (i64.const -1))
    (local.set $status (call $open (i32.const 0) (i64.const 16) (i64.const 3) (i64.const 80)))
    (if (i32.ne (local.get $status) (local.get $expected)) (then unreachable))
    (if (i32.eqz (local.get $expected))
      (then (if (call $close (i64.load (i32.const 80))) (then unreachable)))
      (else (if (i64.ne (i64.load (i32.const 80)) (i64.const -1)) (then unreachable))))
    i32.const 42)
  (func $first (export "first") nop) ;; index 5, real before-nop breakpoint
  (func $second (export "second") nop) ;; index 6
  (func $replacement_run (export "replacement_run") (result i32) ;; index 7
    call $first
    ;; All rejected actual preparation/commit paths must leave original code
    ;; and authority usable by a real generated caller in this same instance.
    (if (i32.ne (call $fresh (i32.const 0)) (i32.const 42)) (then unreachable))
    (if (call $close (i64.load (i32.const 64))) (then unreachable))
    (if (i32.ne (call $value) (i32.const 7)) (then unreachable))
    call $second
    ;; $fresh and this caller were NOT replaced. Whole-module retirement must
    ;; nevertheless refuse both a new open and the original second handle.
    (if (i32.ne (call $fresh (i32.const 13)) (i32.const 42)) (then unreachable))
    (if (i32.ne (call $close (i64.load (i32.const 72))) (i32.const 13)) (then unreachable))
    (if (i32.ne (call $value) (i32.const 9)) (then unreachable))
    i32.const 42)
  (func $other_run (export "other_run") (result i32) ;; index 8
    (if (i32.ne (call $fresh (i32.const 0)) (i32.const 42)) (then unreachable))
    (if (call $close (i64.load (i32.const 64))) (then unreachable))
    (if (call $close (i64.load (i32.const 72))) (then unreachable))
    (if (i32.ne (call $value) (i32.const 7)) (then unreachable))
    i32.const 42)
  (func $reset_run (export "reset_run") (result i32) ;; index 9
    call $first
    ;; The genuine central reset first closes the actual debugger domain and
    ;; drains this execution entry. This real business call must complete while
    ;; authority still exists, before the old-epoch Host retirement callback.
    (if (call $close (i64.load (i32.const 72))) (then unreachable))
    i32.const 42)
  ;; Real Worker route: Root installs breakpoints before resuming this actual
  ;; command entry. No WASI/management import or stdout impersonation is used.
  (func (export "_start") ;; index 10
    (drop (call $initialize))
    (drop (call $replacement_run))))
