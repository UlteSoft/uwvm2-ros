;; SPDX-License-Identifier: Apache-2.0
(module
  (type $cell (struct (field (mut i32))))
  (tag $mixed (param i32 i64 f32 f64 v128))
  (tag $same_signature (param i32 i64 f32 f64 v128))
  (tag $empty)
  (tag $number (param i32))
  (tag $cell_tag (param (ref $cell)))
  (global $saved (mut exnref) (ref.null exn))
  (global $cell_exception (mut exnref) (ref.null exn))
  (table $refs 1 exnref)

  (func $check_tuple (param i32 i64 f32 f64 v128)
    local.get 0 i32.const -7 i32.ne if unreachable end
    local.get 1 i64.const 0xf1a23344e5f67788 i64.ne if unreachable end
    local.get 2 i32.reinterpret_f32 i32.const 0x7fa12345 i32.ne if unreachable end
    local.get 3 i64.reinterpret_f64 i64.const 0x8000000000000000 i64.ne if unreachable end
    local.get 4 v128.const i64x2 0x5566778899aabbcc 0x123456789abcdef0
    v128.xor v128.any_true if unreachable end)

  ;; The first clause with the same signature owns a different tag instance.
  ;; The exceptional branch preserves both the prefix and the local GC roots.
  (func $capture_typed (result i32)
    (local $local_root (ref null $cell))
    i32.const 82 struct.new $cell local.set $local_root
    i32.const 47 struct.new $cell
    (block $wrong (param (ref $cell)) (result i32 i64 f32 f64 v128 (ref exn))
      (block $caught (result i32 i64 f32 f64 v128 (ref exn))
        (try_table (catch_ref $same_signature $wrong) (catch_ref $mixed $caught)
          i32.const -7
          i64.const 0xf1a23344e5f67788
          i32.const 0x7fa12345 f32.reinterpret_i32
          i64.const 0x8000000000000000 f64.reinterpret_i64
          v128.const i64x2 0x5566778899aabbcc 0x123456789abcdef0
          throw $mixed)
        unreachable)
      global.set $saved
      call $check_tuple
      struct.get $cell 0
      local.get $local_root struct.get $cell 0 i32.add
      return)
    unreachable)

  (func $replay_mixed
    (block $caught (result i32 i64 f32 f64 v128)
      (try_table (catch $mixed $caught)
        global.get $saved throw_ref)
      unreachable)
    call $check_tuple)

  (func $capture_empty
    i32.const 0
    (block $caught (result (ref exn))
      (try_table (catch_all_ref $caught) throw $empty)
      unreachable)
    table.set $refs)

  (func $replay_empty
    (block $caught
      (try_table (catch $empty $caught)
        i32.const 0 table.get $refs throw_ref)
      unreachable))

  ;; A reference catch can also hand its retained value to a loop header.
  (func $capture_on_backedge (result i32)
    (local $reference exnref)
    (local $null_visits i32)
    (block $matched (result i32)
      (try_table (catch $number $matched)
        ref.null exn
        (loop $again (param exnref)
          local.tee $reference ref.is_null
          if
          else
            local.get $reference throw_ref
          end
          local.get $null_visits i32.const 1 i32.add local.tee $null_visits
          i32.const 1 i32.gt_u if unreachable end
          (try_table (catch_all_ref $again)
            i32.const 37 throw $number)
          unreachable)
        unreachable)
      unreachable))

  ;; A reference in the payload needs the existing rooted carrier handling.
  (func $capture_cell (param (ref $cell))
    (block $caught (result (ref exn))
      (try_table (catch_all_ref $caught)
        local.get 0 throw $cell_tag)
      unreachable)
    global.set $cell_exception)

  (func $recover_cell (result (ref $cell))
    (block $caught (result (ref $cell))
      (try_table (catch $cell_tag $caught)
        global.get $cell_exception throw_ref)
      unreachable))

  (func (export "_start")
    (local $root (ref null $cell))
    call $capture_typed i32.const 129 i32.ne if unreachable end
    call $replay_mixed
    call $replay_mixed
    call $capture_empty
    call $replay_empty
    call $replay_empty
    call $capture_on_backedge i32.const 37 i32.ne if unreachable end

    i32.const 21 struct.new $cell local.tee $root ref.as_non_null call $capture_cell
    call $recover_cell i32.const 55 struct.set $cell 0
    local.get $root struct.get $cell 0 i32.const 55 i32.ne if unreachable end
    call $recover_cell struct.get $cell 0 i32.const 55 i32.ne if unreachable end))
