;; SPDX-License-Identifier: Apache-2.0
;; Independent invocations, recursion and a self-tail call use one throw site.
;; Each activation preserves two distinct values across later calls.
(module
  (type $capture_type (func (param i32 i32 i32)))
  (tag $number (param i32))
  (table $functions 1 funcref)
  (elem (table $functions) (i32.const 0) func $capture)
  (table $retained 10 (ref null exn))
  (global $completed (mut i32) (i32.const 0))
  ;; A diagnostic harness may replace this initializer to rethrow one value.
  (global $diagnostic_slot i32 (i32.const -1))

  (func $capture (type $capture_type) (param $recur i32) (param $first i32) (param $value i32)
    (local $i i32)
    local.get $recur i32.const 1 i32.eq
    if
      i32.const 0 i32.const 4 i32.const 300 i32.const 0
      call_indirect $functions (type $capture_type)
    end
    (loop $again
      local.get $first local.get $i i32.add
      (block $caught (result (ref exn))
        (try_table (catch_all_ref $caught)
          local.get $value local.get $i i32.add throw $number)
        unreachable)
      table.set $retained
      local.get $i i32.const 1 i32.add local.tee $i
      i32.const 2 i32.lt_u br_if $again)
    local.get $recur i32.const 2 i32.eq
    if
      i32.const 0 i32.const 8 i32.const 500 return_call $capture
    end)

  (func $first_caller
    i32.const 0 i32.const 0 i32.const 100 i32.const 0
    call_indirect $functions (type $capture_type)
    global.get $completed i32.const 1 i32.add global.set $completed)

  (func $second_caller
    i32.const 1 i32.const 2 i32.const 200 i32.const 0
    call_indirect $functions (type $capture_type)
    global.get $completed i32.const 2 i32.add global.set $completed)

  (func $later (param (ref null exn))
    local.get 0 throw_ref)

  (func $verify (param $slot i32) (param $expected i32)
    (block $value (result i32)
      (try_table (catch $number $value)
        local.get $slot table.get $retained call $later)
      unreachable)
    local.get $expected i32.ne if unreachable end)

  (func (export "_start")
    call $first_caller call $second_caller call $tail_caller
    global.get $completed i32.const 7 i32.ne if unreachable end
    i32.const 0 i32.const 100 call $verify
    i32.const 1 i32.const 101 call $verify
    i32.const 2 i32.const 200 call $verify
    i32.const 3 i32.const 201 call $verify
    i32.const 4 i32.const 300 call $verify
    i32.const 5 i32.const 301 call $verify
    i32.const 6 i32.const 400 call $verify
    i32.const 7 i32.const 401 call $verify
    i32.const 8 i32.const 500 call $verify
    i32.const 9 i32.const 501 call $verify
    global.get $diagnostic_slot i32.const 0 i32.ge_s
    if
      global.get $diagnostic_slot table.get $retained call $later
    end)

  (func $tail_caller
    i32.const 2 i32.const 6 i32.const 400 i32.const 0
    call_indirect $functions (type $capture_type)
    global.get $completed i32.const 4 i32.add global.set $completed))
