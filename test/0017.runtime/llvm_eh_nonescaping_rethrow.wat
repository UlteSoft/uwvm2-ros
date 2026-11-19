;; SPDX-License-Identifier: Apache-2.0
(module
  (type $box (struct (field (mut i32))))
  (tag $number (param i32))
  (tag $other_number (param i32))
  (tag $object (param (ref $box)))
  (tag $empty)
  (memory $samples i64 1)
  (data (memory $samples) (i64.const 0) "\05\00\00\00\07\00\00\00")
  (global $effect (mut i32) (i32.const 0))
  (global $saved (mut exnref) (ref.null exn))

  (func $raise (param i32) (result i32)
    local.get 0
    throw $number)
  (func $raise_empty
    throw $empty)
  (func $raise_box (param (ref $box))
    local.get 0
    throw $object)

  (func $assert_i32 (param i32 i32)
    local.get 0 local.get 1 i32.ne if unreachable end)

  ;; A typed catch carries both the original i32 and its exception reference.
  ;; throw_ref consumes the reference and the numeric value arrives at $observed.
  (func $typed_ref (param i32) (result i32)
    (block $observed (result i32)
      (try_table (catch $number $observed)
        (block $captured (result i32 (ref exn))
          (try_table (catch_ref $number $captured)
            local.get 0 call $raise drop)
          unreachable)
        throw_ref)
      unreachable))

  ;; The first matching handler selects this rethrow even when another clause
  ;; could catch the same tag. A different tag may have the same payload type.
  (func $ordered (result i32)
    (block $wrong (result i32)
      (block $observed (result i32)
        (try_table (catch $other_number $wrong) (catch $number $observed)
          (block $captured (result (ref exn))
            (try_table (catch_all_ref $captured) (catch $number $wrong)
              i32.const 31 call $raise drop)
            unreachable)
          throw_ref)
        unreachable)
      return)
    drop
    i32.const -1)

  (func $zero_fields (result i32)
    (block $handled
      (try_table (catch $empty $handled)
        (block $captured (result (ref exn))
          (try_table (catch_ref $empty $captured) call $raise_empty)
          unreachable)
        throw_ref)
      unreachable)
    i32.const 17)

  (func $saved_number (result (ref exn))
    (block $captured (result (ref exn))
      (try_table (catch_all_ref $captured) i32.const 97 call $raise drop)
      unreachable))

  ;; The same capture block can receive a previously materialized reference
  ;; on a normal branch. Only the exact throwing call edge may be folded.
  (func $merged_edges (param $saved (ref exn)) (param $use_saved i32) (result i32)
    (block $observed (result i32)
      (try_table (catch $number $observed)
        (block $captured (result (ref exn))
          (try_table (catch_all_ref $captured)
            local.get $saved local.get $use_saved br_if $captured
            drop
            i32.const 67 call $raise drop)
          unreachable)
        throw_ref)
      unreachable))

  ;; A visible instruction after the capture cannot be discarded.
  (func $visible_effect (result i32)
    (block $observed (result i32)
      (try_table (catch $number $observed)
        (block $captured (result (ref exn))
          (try_table (catch_all_ref $captured)
            i32.const 43 call $raise drop)
          unreachable)
        i32.const 71 global.set $effect
        throw_ref)
      unreachable))

  ;; The first throw_ref in byte order follows $near. The selected handler
  ;; targets $far, where the effect is observable before the actual rethrow.
  (func $far_target (result i32)
    (block $observed (result i32)
      (try_table (catch $number $observed)
        (block $far (result (ref exn))
          (block $near (result (ref exn))
            (try_table (catch_all_ref $far)
              i32.const 53 call $raise drop)
            unreachable)
          throw_ref)
        i32.const 89 global.set $effect
        throw_ref)
      unreachable))

  ;; The loop's first incoming reference is null. The catch supplies a
  ;; nonnull backedge value. Bound the two-iteration check independently of
  ;; the reference test so stale nullness fails with an ordinary Wasm trap.
  (func $loop_reference (result i32)
    (local $reference exnref)
    (local $null_visits i32)
    (block $observed (result i32)
      (try_table (catch $number $observed)
        ref.null exn
        (loop $retry (param exnref) (result exnref)
          local.tee $reference ref.is_null
          if
          else
            i32.const 101 global.set $effect
            local.get $reference throw_ref
          end
          local.get $null_visits i32.const 1 i32.add local.tee $null_visits
          i32.const 1 i32.gt_u if unreachable end
          (try_table (catch_all_ref $retry)
            i32.const 59 call $raise drop)
          unreachable)
        throw_ref)
      unreachable))

  ;; An outer reference observer can save the rethrown object for later use.
  ;; Repeated throws of that reference retain the same mutable GC payload.
  (func $remember (param (ref $box))
    (block $saved_ref (result (ref exn))
      (try_table (catch_all_ref $saved_ref)
        (block $captured (result (ref exn))
          (try_table (catch_all_ref $captured)
            local.get 0 call $raise_box)
          unreachable)
        throw_ref)
      unreachable)
    global.set $saved)
  (func $replay (result (ref $box))
    (block $received (result (ref $box))
      (try_table (catch $object $received)
        global.get $saved throw_ref)
      unreachable))

  ;; Read two distinct in-bounds memory64 locations through a loop parameter.
  ;; The second load has a constant mask independent of the incoming address.
  (func $sample_sum (result i32)
    (local $address i64)
    (local $sum i32)
    i64.const 0
    (loop $next (param i64) (result i32)
      local.tee $address i32.load $samples
      local.get $address i64.const 4095 i64.and i32.load $samples
      i32.add local.get $sum i32.add local.set $sum
      local.get $address i64.const 4 i64.add local.tee $address
      local.get $address i64.const 8 i64.lt_u br_if $next
      drop
      local.get $sum))

  (func (export "_start")
    (local $original (ref null $box))
    (local $alternative exnref)
    i32.const -19 call $typed_ref i32.const -19 call $assert_i32
    call $ordered i32.const 31 call $assert_i32
    call $zero_fields i32.const 17 call $assert_i32
    call $saved_number local.set $alternative
    local.get $alternative ref.as_non_null i32.const 0 call $merged_edges i32.const 67 call $assert_i32
    local.get $alternative ref.as_non_null i32.const 1 call $merged_edges i32.const 97 call $assert_i32
    call $visible_effect i32.const 43 call $assert_i32
    global.get $effect i32.const 71 call $assert_i32
    call $far_target i32.const 53 call $assert_i32
    global.get $effect i32.const 89 call $assert_i32
    call $loop_reference i32.const 59 call $assert_i32
    global.get $effect i32.const 101 call $assert_i32
    call $sample_sum i32.const 24 call $assert_i32

    i32.const 11 struct.new $box local.tee $original ref.as_non_null call $remember
    call $replay i32.const 109 struct.set $box 0
    local.get $original struct.get $box 0 i32.const 109 call $assert_i32
    call $replay struct.get $box 0 i32.const 109 call $assert_i32))
