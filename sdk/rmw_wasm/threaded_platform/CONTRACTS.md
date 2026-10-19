# Future actual contracts; zero executions

Root alone configures/compiles/executes these sources under a real finite whole-
Group deadline and independent physical join. No fake WASI types or native
entities are used. One fresh actual Source/channel is required per case.

contracts/refusal.cpp contains eight actual feature-zero return/output checks:
prepare, handler exchange, activation, join, terminal query and observation
refuse 95, output sentinels remain unchanged, and null outputs return 22. Its
CMake refuses a feature-one library. These are not ROS or threaded Guest proofs.

contracts/receiver_guest.cpp uses the actual admitted WASI library, fd/cookie
compile roles, real mutex/CV/atomics and Root's inaccessible write endpoint.
Root drives that endpoint; the Guest never writes it. Its finite CV timeout is
genuine libc++ time, not counts. It joins/restores on success and finite failure.
Root still physically retires all participants after traps or unknown joins.

| Case | Real Host action | Required accepted counts and terminal |
|---|---|---|
| Pre-activation stop | no frame; build STOP_BEFORE_ACTIVATE=1 | no thread created, later activate17 |
| Interrupt | one valid seq1/event1 | INT1, TERM0, error0 |
| Terminate | one valid seq1/event2 | INT0, TERM1, error0 |
| Partial read | split one valid frame across writes | exactly its callback, error0 |
| Wrong cookie | otherwise valid seq1 | counts0, error22 |
| Wrong sequence | seq2 before seq1 | counts0, error22 |
| Bad magic/extent/reserved | malformed frame | counts0, error22 |
| Quota | limit1; valid seq1 then seq2 | exactly first callback, error90 |
| EOF/HUP | close Host writer | counts0, error5 |
| Missing real fd/poll/clock right | independently deny permission | actual refusal, no fake signal |
| Forced Group stop | empty pipe or incomplete frame | terminal Group + actual physical cleanup |

Set EXPECTED_SIGINT/EXPECTED_SIGTERM explicitly for error cases (zero except
quota) and EXPECTED_ERROR to the actual declared code. Missing rights can fail
at prepare and therefore are admission refusal, not a receiver completion PASS.
Actual provider HUP/error ordering must be retained and checked, not guessed.
Forced-stop acceptance requires Root's full physical retirement, never a zero
receiver return. Partial/uncertain Host writes remain independent raw failures.

The original_context.cpp witness keeps real public init, default All signals,
original NodeOptions/default endpoints, GraphListener and executor waits. Supply
Root's genuine A/B/C profiles/codecs and native peers. Verify default-denied
refusals, actual SIGINT/TERM shutdown, data/guard wakes, idle infinite wait,
active callbacks, pending external leaf and forced stop. Real shutdown must
wake/join original GraphListener/Executor/signal threads and retain native
cleanup diagnostics. Missing endpoint support cannot be hidden by quiet options.
A callback count/static link/model is not full original ROS qualification.

The r2 original_worker_reentry.cpp contract is a genuine original entry body
for Root's prepared bootstrap, not a mock native model. It registers an actual
old SIGINT callback before original init and an actual Context on_shutdown
callback. Root explicitly authorizes logger name uwvm.signal_reentry and waits for the
actual Host log record UWVM_SIGNAL_REENTRY_READY before writing INT; absence
is a real failure, not a reason to invoke a handler directly. One real Host-pipe
INT therefore reaches receiver and original deferred workers. Each calls original public install and uninstall and requires
both to refuse without mutating installed/options. Main performs genuine
shutdown/join only after both returned. A wrong old deferred implementation
could set installed=false before its self-join failed; this state assertion
would catch that defect. Callback role checks use actual private receiver TLS.
The source also exercises concurrent atomic options observation. Root must
provide a true whole-Group watchdog for negative/deadlock regressions; no
source/model success is claimed, and this body is not yet a standalone profile-
prepared executable or native ROS qualification.
