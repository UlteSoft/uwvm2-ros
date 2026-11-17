# Transform inspection

`info scene resources` and DAP `uwvm/sceneState` expose one detached aggregate
per `local-transform` binding. The editor's Robot scene view displays the same
metadata. Exposure and inspection permissions remain independent of transform
publication, lookup and query authority.

A transform row has `type=transform`, `binding=local-transform` and `id=0`.
It carries no session handle, query identity, frame name, transform value,
timestamp, clock epoch or profile digest. Counters remain unsigned decimal
strings through DAP and the editor; JavaScript does not round large values.

- `open_handles` counts live guest sessions; `members` counts retained sessions,
  including closed sessions whose reservations are still owned elsewhere.
- `reserved_bytes`, `workspace_bytes` and `query_bytes` describe retained
  session data, workspace and request-storage capacity. `carrier_bytes`
  describes currently retained copied-carrier capacity.
- `sample_count`, `query_count` and `pending_queries` are cached counts across
  published sessions. Pending queries cannot exceed retained queries. Closing
  a session can remove its visible cached counts before its capacity is freed.
- `retired` reports the cached binding retirement flag.

The formatter and DAP parser independently enforce the closed row schema.
Transform counters are zero on other row types. A transform row cannot carry
model results, buffer leases, lifecycle state/deadlines, timer state or output
transaction metadata. Invalid rows withhold the entire page. Unknown fields,
duplicate fields and incomplete older scene schemas are rejected.

Inspection uses the registry's nonblocking cached snapshot. It never reads the
clock, advances an epoch, expires a query, interpolates a transform, consumes a
result, allocates a new session or performs native I/O. Lock contention yields
an unavailable page. Current cooperative-stop fencing, bounded pagination,
exact alias filtering and debugger inspection permission apply as usual. The
editor exposes no transform mutation action from the metadata panel.

`test/0084.ros-runtime/run_scene_debug.py` qualifies the C++ formatter with
Clang ASan/UBSan and GCC, then checks strict DAP parsing and editor/controller
behavior. The tests include exact values above 2^53, uint64 overflow,
cross-row field contamination, private payload injection, stale stops and
refused mutation messages. This protocol suite is separate from actual VM/DAP
execution and graphical editor acceptance. Run it through the existing 64 GiB,
zero-swap cgroup gate.

`run_scene_transform_dap_live.py` additionally runs the actual optimized VM,
broker and DAP transport. A Wasm guest creates a ROS clock and transform
session, submits an unresolved query with a 50 ms steady deadline, then stops.
After more than 100 ms at that stop, repeated full/read-only inspection still
reports the cached pending query. It exposes only aggregate reservations and
counts; the limited permission cannot inspect the row. The full-permission
case also checks rejection of a stale stop after stepping.

`transform-dap-first/results.json` records all three actual permission sessions
passing against the full `-O3 -g` `vm-transform-central` binary and its retained
3,451-file source cohort. Its separately pinned `transform-debug-tools` snapshot
contains the matching parser and broker, avoiding schema mixing with newer
workspace changes. This remains headless transport qualification; graphical
IDE acceptance and native TF transport require their own evidence.
