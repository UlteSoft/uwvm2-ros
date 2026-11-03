;; Original qualification guest; no imported clock, native helper or payload I/O.
;; One frozen artifact is used in every mode. Each of 1024 passes makes 1024
;; calls and writes; the exact sum is asserted inside the guest after each pass.
(module
  (memory (export "memory") 1)
  (func $leaf (export "leaf") (param $x i32) (result i32)
    local.get $x
    i32.const -1
    i32.eq
    if
      nop ;; Related-function breakpoint: expression offset 7, never reached.
    end
    local.get $x
    i32.const 1
    i32.add)
  (func $never (export "never_called")
    nop ;; Expression offsets 0 and 1 are separate never-reached sites.
    nop)
  (func (export "_start") (local $pass i32) (local $i i32) (local $sum i32)
    loop $passes
      i32.const 0
      local.set $i
      i32.const 0
      local.set $sum
      loop $items
        i32.const 16
        local.get $sum
        local.get $i
        call $leaf
        i32.add
        local.tee $sum
        i32.store
        local.get $i
        i32.const 1
        i32.add
        local.tee $i
        i32.const 1024
        i32.lt_u
        br_if $items
      end
      local.get $sum
      i32.const 524800
      i32.ne
      if
        unreachable
      end
      local.get $pass
      i32.const 1
      i32.add
      local.tee $pass
      i32.const 1024
      i32.lt_u
      br_if $passes
    end))
