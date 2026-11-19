;; SPDX-License-Identifier: Apache-2.0
(module
  (tag $number (param i32))
  (tag $other (param i32))
  (tag $pair (param i64 i32 i32))
  (tag $bits (param f32 f64 v128))
  (tag $empty)
  (global $effect (mut i32) (i32.const 0))

  (func $raise (param i32) (result i32)
    local.get 0
    throw $number)
  (func $reordered (param i32 i64)
    local.get 1
    local.get 0
    local.get 0
    throw $pair)
  (func $float_vector (param f32 f64 v128)
    local.get 0
    local.get 1
    local.get 2
    throw $bits)
  (func $empty_throw
    throw $empty)
  (func $effect_then_throw (param i32)
    i32.const 1
    global.set $effect
    local.get 0
    throw $number)
  (func $ordinary (param i32) (result i32)
    local.get 0
    i32.const 1
    i32.add)

  ;; Clause order matters even when a later typed clause also matches.
  (func $shadow (result i32)
    (block $wrong (result i32)
      (block $selected
        (try_table (catch_all $selected) (catch $number $wrong)
          (drop (call $raise (i32.const 2))))
        unreachable)
      (return (i32.const 83))))
  ;; Identical payload signatures do not make distinct tags interchangeable.
  (func $nested (result i32)
    (block $selected (result i32)
      (try_table (catch $number $selected)
        (block $wrong (result i32)
          (try_table (catch $other $wrong)
            (drop (call $raise (i32.const 41))))
          unreachable)
        (drop))
      unreachable))
  ;; Capturing exnref must still materialize the same object for throw_ref.
  (func $rethrow (result i32)
    (block $selected (result i32)
      (try_table (catch $number $selected)
        (block $ref (result exnref)
          (try_table (catch_all_ref $ref)
            (drop (call $raise (i32.const 19))))
          unreachable)
        throw_ref)
      unreachable))
  ;; A branch to a loop label delivers parameters and preserves the outer stack.
  (func $loop_target (result i32)
    (local $value i32)
    i32.const 7
    i32.const 0
    (loop $again (param i32) (result i32)
      local.tee $value
      i32.eqz
      if
        (try_table (catch $number $again)
          (drop (call $raise (i32.const 5))))
        unreachable
      end
      local.get $value)
    i32.add)
  ;; Source validation treats the call as returning. The other arm is live.
  (func $conditional (param i32) (result i32)
    (block $selected (result i32)
      local.get 0
      if (result i32)
        (try_table (result i32) (catch $number $selected)
          (call $raise (i32.const 21)))
        i32.const 100
        i32.add
      else
        (call $ordinary (i32.const 42))
      end))

  (func $equal_i32 (param i32 i32)
    local.get 0 local.get 1 i32.ne if unreachable end)
  (func $equal_i64 (param i64 i64)
    local.get 0 local.get 1 i64.ne if unreachable end)

  (func (export "_start")
    (call $equal_i32 (call $shadow) (i32.const 83))
    (call $equal_i32 (call $nested) (i32.const 41))
    (call $equal_i32 (call $rethrow) (i32.const 19))
    (call $equal_i32 (call $loop_target) (i32.const 12))
    (call $equal_i32 (call $conditional (i32.const 0)) (i32.const 43))
    (call $equal_i32 (call $conditional (i32.const 1)) (i32.const 21))

    (block $empty_caught
      (try_table (catch $empty $empty_caught)
        call $empty_throw)
      unreachable)
    (block $non_leaf (result i32)
      (try_table (catch $number $non_leaf)
        (call $effect_then_throw (i32.const 29)))
      unreachable)
    i32.const 29 call $equal_i32
    global.get $effect i32.const 1 call $equal_i32

    (block $mixed (result i64 i32 i32)
      (try_table (catch $pair $mixed)
        (call $reordered (i32.const -79) (i64.const 0x123456789abcdef0)))
      unreachable)
    i32.const -79 call $equal_i32
    i32.const -79 call $equal_i32
    i64.const 0x123456789abcdef0 call $equal_i64

    (block $numeric_bits (result f32 f64 v128)
      (try_table (catch $bits $numeric_bits)
        (call $float_vector
          (f32.reinterpret_i32 (i32.const 0x7fc12345))
          (f64.reinterpret_i64 (i64.const 0x8000000000000000))
          (v128.const i64x2 1 0x123456789abcdef0)))
      unreachable)
    i64x2.extract_lane 1 i64.const 0x123456789abcdef0 call $equal_i64
    i64.reinterpret_f64 i64.const 0x8000000000000000 call $equal_i64
    i32.reinterpret_f32 i32.const 0x7fc12345 call $equal_i32))
