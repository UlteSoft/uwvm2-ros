;; Optional function-reference identity lesson. The function reference belongs
;; to this Wasm instance; its function index must never be interpreted in main.
;; This is a pure adapter, not a ROS/native callback registration mechanism.
(module
  (type $callback (func (param i32 i32 i32 i32) (result i32)))
  (import "callbacks" "on_motion" (func $registered (type $callback)))
  ;; Retain index1 so forward has index2, matching main's intentional decoy2.
  (import "callbacks" "callback_count" (func $count (param i32 i32) (result i32)))
  (func $forward (type $callback) (param i32 i32 i32 i32) (result i32)
    local.get 0 local.get 1 local.get 2 local.get 3 call $registered)
  (global (export "callback_ref") funcref (ref.func $forward)))
