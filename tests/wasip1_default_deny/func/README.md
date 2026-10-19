# Low-level WASI Preview1 authority tests

`authority.cc` calls the real native host APIs with both WASI pointer widths.
Build it twice, setting `WASIP1_DEFAULT_DENY_WIDTH` to `32` and `64`; the latter
also needs `UWVM_ENABLE_LOCAL_IMPORTED_WASIP1_WASM64`. Use the same compiler,
C++ language mode, include paths, feature flags, and native memory configuration
as the current Worker build. The implementation uses the repository's current
C++ feature baseline, including pack indexing; an older compiler is not a valid
substitute.

Compilation and execution belong exclusively inside the previously verified
Linux 64 GiB cgroup. Freeze and record the current source closure, compiler
identity, cgroup identity, command lines, binary hashes, and raw results before
making a runtime PASS claim. The fixtures are prepared source; writing or
reading them is not evidence of native or guest execution.

The assertions cover default refusal of all four clocks, relative and absolute
clock subscriptions, randomness including zero-length buffers, signal and
yield callbacks, direct calls to the shared signal/yield implementations,
invalid clock identifiers, separate clock/random/process grants, and isolation
between environment instances. Every whole-call refusal checks the complete
output and event-count sentinels. Mixed polling arrays include a denied clock
after an authorized clock, a denied clock after an invalid FD, and an invalid
clock after an authorized clock. FD polling checks require POLL and the
subscription's READ or WRITE direction on each platform path. A denied FD
subscription is represented by an ENOTCAPABLE event; a denied clock refuses the
entire call before any platform setup or output.

Signal and yield positive cases use explicit native callback counters rather
than delivering a signal to the test process. Clock and randomness positive
cases use the actual native facilities. The zero-duration authorized poll case
avoids intentional waiting. The fixture does not establish real ROS execution,
Group/Single CLI routing, interpreter integration, aarch64 behavior, or native
syscall side-effect counts. Those require the broader Worker and QEMU acceptance
matrix in the same verified cgroup.
