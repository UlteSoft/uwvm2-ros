;; Explicit reactor adapter. Preload vision_go first, then rename this module
;; vision. Merely exporting _initialize in a library does not invoke it in uwvm.
;; A compiler-produced actual Wasm start section, if any, is still a different
;; lifecycle hook and must be inspected. The original Go module must expose the
;; exact two imports below; no generic symbol/host fallback is accepted.
(module
  (import "vision_go" "_initialize" (func $initialize))
  (import "vision_go" "vision_clearance_mm" (func $clearance (param i32 i32 i32 i32) (result i32)))
  (global $initialized (mut i32) (i32.const 0))
  (func (export "vision_initialize") (result i32)
    global.get $initialized
    if (result i32)
      i32.const 17
    else
      ;; Commit before invoking another module: a trap is terminal, not retryable.
      i32.const 1 global.set $initialized
      call $initialize
      i32.const 0
    end)
  (func (export "vision_clearance_mm") (param i32 i32 i32 i32) (result i32)
    global.get $initialized i32.eqz
    if (result i32)
      i32.const -2147483626
    else
      local.get 0 local.get 1 local.get 2 local.get 3 call $clearance
    end))
