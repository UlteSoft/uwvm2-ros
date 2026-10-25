;; @file ros_live_disassembly.wat
;; @brief Exercise real ROS admission between debugger stops in a numeric loop.
;; @details The fixed arithmetic prefix supplies independently checked Wasm
;; offsets for the native stepping witness. Every iteration must authenticate
;; the actual module before the malformed capability request returns EINVAL.
;; No endpoint or ROS graph operation is authorized by this fixture.
(module
  (import "uwvm_ros2_v1" "capability_open"
    (func $open (param i32 i64 i64 i64) (result i32)))
  (memory (export "memory") 1 1)
  (func $window (export "_start") (local $value i32)
    i32.const 17
    local.set $value
    (loop $again
      local.get $value
      i32.const 41
      i32.xor
      local.set $value
      local.get $value
      i32.const 1
      i32.rotl
      local.set $value
      (if (i32.ne
            (call $open (i32.const 0) (i64.const 0) (i64.const 0) (i64.const 16))
            (i32.const 22))
        (then unreachable))
      br $again)))
