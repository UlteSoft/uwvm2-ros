# WASI Preview 1 Default Authority Configuration

The implementation starts each concrete environment with an empty
`non_fd_permissions` mask. Global host environment inheritance defaults to
disabled. Stdin has `READ | FILESTAT_GET | POLL_FD_READWRITE` (`0x08200002`),
stdout/stderr have `WRITE | FILESTAT_GET | POLL_FD_READWRITE` (`0x08200040`), and
all three have zero inheriting rights. These masks retain basic stream operation
and metadata without implicit path, mutation, seek or socket derivation rights.

## Files and Evidence Boundaries

- `configuration.cc` is a prepared native C++ fixture. It calls the real policy
  helpers and public grant/deny callbacks, checks all eight named permissions,
  rejects unknown/repeated/conflicting commands, checks target denial of Global
  grants, and evaluates all 16,777,216 mask combinations against independent
  per-bit selection. It does not execute a Wasm Guest. It has not been compiled
  or executed merely by preparing this source.
- `check_configuration_sources.py` inspects exact source declarations and
  registration paths. It evaluates the extracted pure C++ merge expression in
  Python against independent sets for 655,360 cases. This is a source/model
  check, not execution of the C++ helper or a runtime acceptance test.
- `func/authority.cc` and `func/source_models.py` cover the separate low-level
  function gates; see their own README for exact scope and execution status.

Run the allowed source-only check with a new report path:

```bash
python3 -B test/0073.ros-tests/support/source/wasip1_default_deny/check_configuration_sources.py \
  --output /tmp/wasip1-configuration-source-check-new.json
```

The tool refuses to replace an existing report and records inspected source
SHA-256 values. A source-only `PASS` must never be reused as compiled native,
Guest, ROS or aarch64 evidence.

## Permission Policy

The finite `environment::permission_t` bits are realtime, monotonic, process
CPU-time and thread CPU-time clocks; random bytes; process signal delivery;
scheduler yield; and an explicit `stdio-full-rights` compatibility option.
`permits(permission_t{})` always returns false. Invalid clock IDs keep their
invalid-argument diagnostic instead of treating an unknown zero bit as a grant.

Global/Single/Group grant and deny commands take one exact permission name;
[the CLI reference](../../../../../documents/command-line/wasi.md#explicit-permission-commands)
lists names and usage. Target configuration uses a separate per-bit presence
mask. Unconfigured target bits follow Global, configured target bits replace
only their own value, and an explicit target denial can remove a Global grant.
Two commands for the same bit in one target are rejected, including repeated
denials. Unknown names and wildcards are rejected before state mutation.

Environment inheritance has a separate presence flag as well. The default safe
value does not claim an option was supplied. The existing explicit Global
noinherit command remains valid, and the new explicit Global inherit command
uses the same duplicate/conflict marker. Existing Single/Group inheritance
overrides remain unchanged.

The native API exposure switch controls visibility only. Clock/random/signal/
scheduler gates are in the underlying functions, so exposing the table does not
grant authority. A recorded checkpoint must not install a more permissive live
policy. Guest arguments and introspection of existing descriptors do not create
new authority. `proc_exit` remains Guest termination.

The private checkpoint world copies `original.non_fd_permissions` from the
current trusted environment, rather than from saved snapshot data. Same-object
checkpoint publication swaps descriptors/arguments/environment strings and
leaves the live mask untouched. Portable resource qualification uses temporary
FD-only scratch contexts, which deliberately need no non-descriptor authority.
Actual checkpoint regressions after this policy change are still required.

## Required Actual Acceptance

Compile and execute native fixtures only inside the existing Linux cgroup with
`memory.max=68719476736`, zero swap, exact leaf device/inode proof, compiler and
source hashes, bounded processes and before/after OOM evidence. Do not execute
these fixtures or compile Guest code on the local desktop as a substitute.
The required harness should use C++26, the repository `src` and bundled
`third-parties/fast_io/include` include paths, and retain enabled checks. Do not
define `NDEBUG` to remove validation. Native configuration callbacks use
synthetic parser tokens; full Worker CLI parsing is a separate requirement.

The concrete ARM entry now selects configuration, both authority widths and
14 existing positive regressions using `--suite wasip1-default-deny`. See
[its complete input and evidence requirements](README.arm-entry.md). Actual
compilation/execution of this new matrix remains pending.

Actual CLI acceptance must cover:

1. Fresh Global, empty Single and empty Group: empty inherited environment,
   directional basic stdio, denied four clocks/random/signal/yield, and no
   directory/socket preopens.
2. Each distinct permission granted alone: the selected operation is available,
   unrelated operations remain denied, and unsupported platform operations keep
   their existing supported/unsupported result.
3. A Global grant plus target denial: only the selected module/group loses that
   authority; unrelated module authority and unrelated bits remain unchanged.
4. Unknown, absent, uppercase, repeated and conflicting permission operands:
   reject before Guest initialization; missing Single/Group targets also reject.
5. Explicit noinherit: accepted despite the safe default. Explicit inherit:
   observe only a synthetic test environment. Repeat/opposite Global flags
   reject. Existing target inherit/noinherit overrides resolve correctly.
6. Default stdio rejects wrong-direction read/write, seek, flags, file mutation,
   path derivation, socket acceptance/shutdown and inheriting elevation. An
   explicitly granted full-mask case uses a launcher-owned private resource and
   remains distinct from a default authority claim.
7. Mixed `poll_oneoff` inputs containing a denied timer fail before any native
   wait or output-memory write. Descriptor readiness respects its read/write
   direction. wasm32/64 paths and exposed native APIs behave consistently.
8. Strict ROS startup accepts the safe environment default, still rejects an
   explicit inherited environment/preopens/plugin/socket stdio, and keeps ROS
   grants independent from WASI permissions.

Previously stored permissive-default evidence remains historical evidence. It
must not be changed to imply these newly prepared defaults were already tested.
