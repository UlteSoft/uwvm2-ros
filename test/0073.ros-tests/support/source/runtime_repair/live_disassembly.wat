;; @file live_disassembly.wat
;; @brief Keep one arithmetic function active for a genuine debugger attachment.
;; @details The loop has known instruction boundaries used by the maintained
;; native-window acceptance helper. No host calls or debugger replies are faked.
(module
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
      br $again)))
