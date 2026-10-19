;; Real unresolved-import Guest fixture, authored but not executed.
;; Select a genuine Worker with this ROS business registry compiled in.
;; Import linking must report the exact missing export before _start runs.
;; A trap, arbitrary nonzero exit or an absent whole ROS module is insufficient.
;; WASI stdout is basic application output, never a management command channel.
(module
  (import "wasi_snapshot_preview1" "fd_write"
    (func $write (param i32 i32 i32 i32) (result i32)))
  (import "uwvm_ros1_v1" "runtime_update" (func $forbidden (result i32)))
  (memory (export "memory") 1 1)
  (data (i32.const 64) "FORBIDDEN_MANAGEMENT_START ros1 runtime_update\0a")
  (func (export "_start")
    i32.const 0
    i32.const 64
    i32.store
    i32.const 4
    i32.const 47
    i32.store
    i32.const 1
    i32.const 0
    i32.const 1
    i32.const 8
    call $write
    drop
    call $forbidden
    drop))
