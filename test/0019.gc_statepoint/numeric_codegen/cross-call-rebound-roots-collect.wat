(module
  (type $s (struct (field (mut i32))))
  (func $collect (local $n i32)
    (loop $again
      (drop (struct.new $s (i32.const 0)))
      (local.set $n (i32.add (local.get $n) (i32.const 1)))
      (br_if $again (i32.lt_u (local.get $n) (i32.const 65536)))))
  (func $check (param $a (ref null $s)) (param $b (ref null $s))
    (call $collect)
    (if (i32.ne (struct.get $s 0 (local.get $a)) (i32.const 31)) (then unreachable))
    (if (i32.ne (struct.get $s 0 (local.get $b)) (i32.const 47)) (then unreachable)))
  (func (export "_start") (local $p (ref null $s)) (local $q (ref null $s))
    (local.set $p (struct.new $s (i32.const 31)))
    (local.set $q (struct.new $s (i32.const 47)))
    ;; Both old operand values must survive after the local is rebound.
    local.get $p
    local.get $p
    local.get $q
    local.set $p
    local.get $p
    call $check
    struct.get $s 0
    i32.const 31
    i32.ne
    if unreachable end
    (if (i32.ne (struct.get $s 0 (local.get $p)) (i32.const 47)) (then unreachable))))
