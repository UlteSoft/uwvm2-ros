# One source-locked rospy API port

`rospy_pubsub_order.py` implements the original
`TestPubSubOrder.test_subscriber_first` assertion path from
[ros/ros_comm commit30483a9f218f1545eec16d3934bf3cb042e2cb5b](https://github.com/ros/ros_comm/tree/30483a9f218f1545eec16d3934bf3cb042e2cb5b).
The actual source observations in `rospy_pubsub_order.source.json` bind the
original test, its launch file and `listenerpublisher.py` by byte length,
SHA256 and Git blob identity. These are remote source observations, not a
checkout, SDK, execution or passed testcase. Root still acquires/verifies the
complete exact Git tree through the official runner's source-lock contract.

The original native ROS unittest/rostest result remains independently executed
and discovered. This port accepts only that exact source lock and actual
`test_pubsub_order.TestPubSubOrder.test_subscriber_first` loaded-method identity,
or the same original class/method in an actual fresh JUnit result-file universe.
The raw JUnit file hash/classname/name/ordinal is retained. No generic test ID,
Unitree demo, prior custom transport PASS or total count can select this port.

The current concrete provider is native Noetic/Focal x86_64. Other distros,
architectures, RMWs and official cases remain separate missing prerequisites.
Original rostest source is not switched to unittest merely to get a result:
its ROS initialization, package resolution, real Master, installed executable
shebangs and applicable native/chroot workflow must actually work first.
Pure unittest discovery does not provide those fixtures. An unavailable
original native workflow leaves its case unexecuted/unqualified.

## What executes

* `rospy_pubsub_order_peer.py` uses the actual selected Focal Python and imports
  the exact original native `listenerpublisher.py`. It runs its original
  `callback()` and `start_publishing()` implementation. The wrapper records
  the subscriber's actual zero-publisher gate before the Guest opens an alias,
  waits for the actual later Gateway publisher, and requires all ten original
  String callbacks. It does not synthesize an echo on the coordinator.
* `rospy_pubsub_order_guest.c` is a real freestanding ROS1 Wasm program using
  the production C Node/codec/import headers. It opens exactly `pub` and `sub`,
  publishes ten `hi [42]` String messages on owner-paced100-ms gates, polls a
  genuine native echo, and compares complete decoded text.42 is one explicit
  value inside the original `randint(0,109812312)` domain, not exhaustive random
  coverage. The Guest imports only ROS1 plus basic stdin/stdout/proc_exit.
* Positive and default-denied cases use the same artifact, actual SDK, Worker,
  original fixtures and source. A separate stdin mode selects explicit grants
  or two actual alias-open denials13 with untouched output slots. The negative
  native witness samples at least two seconds and thirty real observations of
  zero callbacks. A scheduling pause with one sample does not qualify. No Gateway is
  started for a disabled main principal. These negative observations supplement
  the original official case; they are not extra official upstream testcases.

All actors run in the caller's fresh private loopback-only namespace and
original64-GiB/zero-swap cgroup. Root supplies separately owned output/provider
views and budgets. Actual process birth/pidfd, direct parent, cgroup membership,
opened `/proc/PID/exe` and mapped images/DSOs are checked before effect release.
For Focal Python launched by its explicit SDK loader, the kernel executable is
the loader and the mapped image is the actual Python binary; those are distinct
physical observations. Neither is inferred from the command line. The native
Master is stopped through its retained same-birth pidfd with SIGINT and must
exit normally. The Gateway retires after the actual bound Worker exits.

## Actual prerequisites and invocation

First freeze the complete new source. Root admits every helper through the
same complete-MF loader, creates an actual standard Unitree Frame with immutable
tool/SDK/provider roots and fresh output, and invokes:

```python
producer.build(frame, current_complete_MF_loader,
               selection_input='official-pubsub-tools')
exit_status = frame.finish(producer.STATUS)
```

The selected input JSON has exactly `cc,wasm_ld,libc,builtins,validator,sysroot,
wasm_ld_alias`. The first five are actual existing Frame input labels. The
sysroot is its actual complete canonical provider directory. The alias is its
real selected wasm-ld multicall path, resolving to the selected physical tool.
No null template is an executable input. The producer runs exactly three serial
owned commands: actual C11 full-MD compilation, traced standalone link, full
Wasm code-body validation. RSS is capped at768MiB and each command60seconds.
Root's Frame finalization additionally reopens all actual inputs/products and
complete roots. The resulting `PASS_PREPARED_...NOT_EXECUTED` status is build
evidence only. The runtime port replays the exact argv recipe, real full-MD,
all traced inputs, actual validator/artifact association, import signatures,
same-module local `_start` index and2..256-page bounded unshared32-bit memory.

The official differential configuration retains its closed v1 shape. For this
port it needs:

* `sdk_evidence`: exactly `[actual_completed_current_native3_report,
  actual_completed_current_Guest_build_report]`. The current native producer
  schema `uwvm-unitree-noetic-native-products-v1` is reopened: all three real
  compileDB/full-MD/verbose-link/ELF proofs and complete literal-CHROOT SDK/work
  catalogs are replayed. Its Gateway binary is the actual selected one.
* `worker_evidence`: exactly one actual completed current-source four-profile
  interpreter build report. The selected ROS1 profile's real Runtime/main/host
  TUs, full-MD, linked native objects/archives/stdlib and binary are replayed.
  A historical Source37 Worker is not silently relabelled current.
* `fixture_inputs`: current full7s of all three exact original Git files in the
  observed source identity JSON. `source_inputs` includes actual current full7s
  of the C Guest and native witness scripts. Provider/evidence/source roots must
  already fit the original whole-root budget; this port never adds a root.
  The exact native producer parent is included once in plan roots and its
  actual `report.json` full7 is explicitly selected in `literal_provider_reports`
  so copied-SDK absolute aliases retain literal CHROOT meaning. This selects no
  host target authority. Ordinary physical files and real map_files remain the
  independent executable/provider proof.
* `roles`: actual `master,native_peer,worker` plus positive-only `gateway`.
  Master/peer use the actual native report's Focal loader/Python and its exact
  SDK environment. Master argv is the original report's `launch.master` with
  one explicit current loopback port substituted. Peer argv is the same loader,
  `--library-path`, actual SDK library path, actual Python, current peer source,
  `--fixture`, original listener source, `--mode`, the selected authority mode.
  Every role's actual ELF/DSO mapping selection remains explicit.
* The Worker selects the actual admitted Wasm and its inspected local entry
  through one `--wasm-set-start-func` operand. `policy_arguments()` returns only
  the exact ROS policy suffix; Root provides its original independently
  admitted interpreter/main/basic-WASI argv. Positive topic limits and
  `tcpros`/unlatched semantics are identical in Worker and Gateway. Both global
  defaults stay disabled. Gateway argv is exactly `['{out}/gateway.conf']`;
  the port writes this fresh config from the actual live Worker PID, grants
  and private socket. Socket length must fit107 bytes.

The dispatcher consumes `official-api-port-v2` and its source-locked
`supports_discovered_case` function. The empty static support list does not
disable this port; dynamic matching admits only an actually discovered exact
original method, needed because fresh JUnit file universes include their real
varying file SHA. `all_official_tests_qualified` remains false. Missing methods,
projects, fixtures, providers, skipped/disabled tests and hardware stay gaps.

## Regression scope

`test_rospy_pubsub_port.suite(port,producer,differential)` contains17 DATA/policy
regressions, supplied modules must first be admitted against the complete MF.
It starts no child and does not import candidates itself. Its policy/argv
success is not compilation, ROS delivery or official-suite acceptance. Actual
native original workflow, three-command Guest build, positive echo and
default-denied zero effects remain future Root-owned executions.
