# Explicit platform for original ROS client libraries

The platform uses the existing real C SDK dispatch table. It does not alter the
original ABI-v1 catalog or add Host functions. It is an independent source-only
candidate, not an already working original ROS client SDK.

Prepare an immutable version-1 selection before any original rcl logging or time
initialization. All aliases/names are bounded/copied and validated first, all
owners have fixed storage, and each successful open is retained before the next
import. A failed open closes all earlier owners once in reverse order. Native
permission remains entirely in the actual Host policy; a selected name grants
nothing. All aliases are distinct, including system/steady clocks and each of
the five severity owners per exact logger. There are at most8 loggers/42 owners.

`uwvm_rmw_platform_now` requests the actual selected clock, validates a complete
8-byte canonical nonnegative signed-int64 timestamp, and commits the caller
output only on success. No environment/ambient WASI clock substitutes for this
request. RAW/slew-free is unsupported, and no clock API claims to sleep.

The real external logging backend handles DEBUG/INFO/WARN/ERROR/FATAL through
five independently selected `log.write` aliases. Their actual Host policies set
severity. Exact logger-name selection does not inherit child logger rights.
The original rcutils severity filter remains active. Local levels add filtering
without increasing authority. Messages use the existing canonical String writer,
strict UTF-8 and a4096-byte maximum; no truncation/printf reimplementation or
encode→decode allocation is introduced. Explicit config-file or log-file-prefix
requests are refused, not silently accepted. The original console/rosout paths
remain unchanged; neither their permissions nor their outputs are suppressed.

Original external_log has no return value. Its admitted raw failures are sticky
diagnostics; a concurrent/reentrant log refusal has its own atomic flag because
the refusing caller cannot safely edit an already-busy diagnostics structure.
The flag does not claim an exact concurrent-attempt count or ordering. Every
admitted owner close retains its individual result plus the first close error,
separate from the first primary raw error. No cleanup error causes automatic
close replay. Platform operations use a nonwaiting C11 atomic entry guard and
return17 when busy, including reentrant close from an actual backend callback.

Explicit retain/release lifetime references must span original application
init through complete original object/context finalization. Close refuses live
references or an active external logger. They are Guest-local lifecycle leases,
not a claim that native resources elsewhere have closed. Save diagnostics before
another successful prepare starts a new epoch. Closing an already-retired epoch
does not repeat imports; an earlier close failure remains visible.

`tests/platform_contract.c` contains20 authored source-only host-backend groups:
denial, pre-effect validation, missing operation, duplicate alias, reverse setup
rollback, all-close/primary preservation, full-u64 owners, clock/error atomicity,
clock extent/signed range, reentrant retirement, lifetime references, unknown
logger refusal, all five real severity owners/wire, UTF-8, exact log bound,
file/config refusal, local filtering, backend/version admission, no close replay,
and duplicate backend-handle refusal. They have not been compiled or run. A
model dispatch table cannot establish actual imports, native ROS or upstream
compatibility. See `../cmake/BUILD.md` for the actual static package recipe and
the explicit remaining default-endpoint/threading/type-support build frontier.
