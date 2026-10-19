# Explicit policy to executable application mapping

[POLICY_COVERAGE.json](POLICY_COVERAGE.json) records all10 C and10 C++ entries,
each compile-time case and its explicit alias set. [ADVANCED.md](ADVANCED.md)
provides complete grant/limit generation and external native prerequisites.
This is a source requirement map, not validated policy or native acceptance.

For10, every case explicitly constructs four independent roles: goal-send,
goal-cancel-own, goal-feedback and goal-result. CANCEL_ANY=0 opens no fifth role;
CANCEL_ANY=1 explicitly supplies goal-cancel-any. Cases2/4 without that role
return13 locally without cancel dispatch after successful group construction.
Case0 sends then polls one feedback/result; case1 uses own-cancel then one result;
case2 requests zero/all; case3 defers one synchronous send; case4 uses a nonzero
selector through the stronger fifth role. Runtime authority remains decisive
for known/nonlocal IDs and same resource/type. REVOKE=1 deliberately mutates each
alias; default cleanup only closes owners. No alias is acquired from a guessed
name or a default permission.

The one-shot10 main reports pending11 and closes its owners. A long-lived app
must retain the bundle and externally advance bounded receives before closing.
No automatic send/cancel replay, clock, sleep or thread is provided. The local
ID table is1..1024; the example selects1 and512-byte complete receive storage.
CLI field validity, compiler validation, native effect/readiness, exact source
binding and cleanup proof are separate evidence phases.
