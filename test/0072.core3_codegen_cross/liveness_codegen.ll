; Object-identity fixture: nonlocal kills, loop PHIs and call-clobbered values.
declare i64 @left(i64)
declare i64 @right(i64)
define i64 @branch_and_loop(i64 %seed, i64 %count, ptr %memory) {
entry:
  %base = add i64 %seed, 37
  %nonzero = icmp ne i64 %count, 0
  br i1 %nonzero, label %loop, label %exit
loop:
  %i = phi i64 [ 0, %entry ], [ %next, %merge ]
  %sum = phi i64 [ %base, %entry ], [ %updated, %merge ]
  %parity = and i64 %i, 1
  %even = icmp eq i64 %parity, 0
  br i1 %even, label %a, label %b
a:
  %av = call i64 @left(i64 %base)
  %ax = add i64 %av, %sum
  br label %merge
b:
  %bv = call i64 @right(i64 %base)
  %bx = xor i64 %bv, %sum
  br label %merge
merge:
  %updated = phi i64 [ %ax, %a ], [ %bx, %b ]
  %address = getelementptr i64, ptr %memory, i64 %i
  store volatile i64 %updated, ptr %address
  %next = add i64 %i, 1
  %done = icmp eq i64 %next, %count
  br i1 %done, label %exit, label %loop
exit:
  %result = phi i64 [ %base, %entry ], [ %updated, %merge ]
  ret i64 %result
}
