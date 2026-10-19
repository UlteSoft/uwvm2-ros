;; Standalone zero-host-right callback-reference graph. Requires callbacks and
;; callback_ref preloads, renamed exactly as imports below. Module-local decoy
;; index2 returns99, distinguishing a wrong-instance bare-index dispatch.
(module
  (type $callback (func (param i32 i32 i32 i32) (result i32)))
  (import "callbacks" "callback_begin" (func $begin (param i32) (result i32)))
  (import "callbacks" "callback_end" (func $end (param i32 i32) (result i32)))
  (import "callback_ref" "callback_ref" (global $reference funcref))
  (table 1 1 funcref)
  (func $decoy (type $callback) (param i32 i32 i32 i32) (result i32) i32.const 99)
  (func $entry (local $generation i32)
    i32.const 1297044558 call $begin local.tee $generation i32.eqz if unreachable end
    i32.const 0 global.get $reference table.set
    i32.const 1297044558 local.get $generation i32.const 60 i32.const 600
    i32.const 0 call_indirect (type $callback)
    i32.const 60 i32.ne if unreachable end
    i32.const 1297044558 local.get $generation call $end if unreachable end
    i32.const 1297044558 local.get $generation i32.const 60 i32.const 600
    i32.const 0 call_indirect (type $callback)
    i32.const -2147483639 i32.ne if unreachable end)
  (start $entry))
