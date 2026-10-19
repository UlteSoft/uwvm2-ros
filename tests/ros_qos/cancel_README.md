# Explicit ROS2 cancellation QoS regressions

These fixtures are **prepared, not compiled or executed** while Linux is
unavailable. They extend the G04 response work without replacing its sources,
reports or frozen artifacts. Every actual compilation, native invocation and
Guest test must run in the existing verified Linux 64 GiB cgroup, with fresh
outputs and the usual source/compiler/SDK/ELF/cgroup evidence.

Outgoing cancellation reuses the client created by the send-goal capability.
The eight `qos.cancel-service.*` fields must therefore match that Goal's immutable
contract. A separate preflight pass checks every selected active outgoing Goal
before any native cancellation. It also checks pending Goals whose native handle
has not arrived yet. An `action.cancel-any-goal` request cannot cancel an earlier
matching Goal and then discover a later mismatch. Missing fields and different
decimal spellings fail closed with error 13. Ownership selection, selection
quota and timeout priority retain their existing errors. The other four Action
channels do not constrain a cancellation operation.

An incoming Goal instead uses a newly created ordinary `CancelGoal` service
client. Both its actual request-publisher and response-subscription QoS must
match the configured profile before service discovery or a request; an
unavailable or mismatched actual profile returns error 95. This does not add
cancellation authority or change the Goal's owner, resource or type checks.

`cancel_matrix.json` and `cancel_matrix.h` contain independent expectations for
all eight fields. `cancel_contract.cc` calls the real comparator and checks
reversed mutations, equal mutations, missing fields, padded decimals and unused
channels. Equality is distinct from individual validation: `keep-all` remains
unsupported even when both texts match.

`cancel_native_ros2.cc` prepares the actual adapter/native Action server cases:
seven individually legal field mutations plus a padded decimal for own-goal
cancellation, an any-goal selection whose later Goal has a conflicting depth,
selection quota and timeout priority, and matching own/any/incoming cancellation.
The server counts native cancel callbacks and the fixture preserves registry,
Goal flags, native handles, timers and deadlines around each refusal. Matching
calls must produce a real canceled result and retire cleanup occupancy.

Four incoming actual-QoS fault cases interpose the two public rcl query functions
on the test's calling thread: request/response mismatch and unavailable profile.
Separate interposers count discovery and service requests; neither may occur
after a rejected profile. The fixture requires the hooks actually run, so an
unsupported dynamic-link arrangement cannot silently pass. Link this test with
`libdl`; never link its interposers into a production Worker or Gateway. These
are controlled fault injections, not naturally observed middleware downgrades.
The exact query entry points were checked against the official
[Jazzy rclcpp client source](https://github.com/ros2/rclcpp/blob/jazzy/rclcpp/src/rclcpp/client.cpp).

The current `tests/ros_aarch64/run_action_qos.py` prepares four mandatory C++17
targets: the original loader and incoming-response probes, plus independent
`action-qos-cancel-contract` and `action-qos-cancel-native` executables. The
cancellation native probe links `libdl`. All four retain serial native GCC13,
the stop barrier, final dependency files, source/SDK closures, compiler/link
recipes and strict ELF183/QEMU mapping evidence. Preparation format 2 records
the exact four recipes and executed driver/helper hashes; execute rejects older
two-probe preparations and changed inputs. Retain the original G04 tests and
run them against the new build; source preservation alone is not execution.

Freeze the current source and complete driver set into new paths. Inside the
verified Linux cgroup and a private mount namespace, use the following entries
with absolute paths supplied by the parent evidence launch:

```sh
unshare -m /usr/bin/python3 -B "$QOS_DRIVERS/run_action_qos.py" \
  --mode prepare --source "$QOS_SOURCE" --rootfs "$QOS_JAZZY_ROOTFS" \
  --qemu "$QOS_QEMU" --binfmt-evidence "$QOS_LEASE_JSON" \
  --container-started-at "$QOS_CONTAINER_STARTED_AT" --out "$QOS_PREPARE_OUT"

unshare -m /usr/bin/python3 -B "$QOS_DRIVERS/run_action_qos.py" \
  --mode execute --source "$QOS_SOURCE" --rootfs "$QOS_JAZZY_ROOTFS" \
  --qemu "$QOS_QEMU" --binfmt-evidence "$QOS_LEASE_JSON" \
  --container-started-at "$QOS_CONTAINER_STARTED_AT" \
  --prepared "$QOS_PREPARE_OUT/report.json" --domain 74 --out "$QOS_EXECUTE_OUT"
```

The full `run_all.py` acceptance entry now additionally requires
`--prepared-action-qos "$QOS_PREPARE_OUT/report.json"` and
`--container-started-at "$QOS_CONTAINER_STARTED_AT"`. Its mandatory `action-qos`
stage executes all four products in a nested private mount namespace. Even an
explicit suite selection cannot omit this regression. The artifact pool must
contain exactly all 26 qualified product paths: four newly compiled Workers,
18 original native products and four current-source QoS probes. An old 18/22
product pool, a substituted 26th product, or an older preparation fails closed.

Because WASIp1 default authority changed, the four Workers must be built fresh.
Reports qualified by reusing original Worker dependencies are rejected. Each
Worker's three actual compile commands, raw MMD dependency sets, current source
bytes and ELF183 objects are checked, including the current environment/storage
permission headers and both CLI permission parameter/callback registrations.
This source-binding gate is independent of the actual ROS behavior stage.

The following permitted source/model command performs no native or Guest work:

```sh
python3 -B tests/ros_qos/cancel_model.py --out /tmp/uwvm-cancel-qos-source-model-r1.json
python3 -B tests/ros_qos/cancel_driver_models.py
```

Choose a fresh output path on every run. Its result checks textual ordering and
an independent Python transaction model, including a later mismatch and pending
handles. It does not parse C++ semantically or establish compilation, middleware
behavior, cancellation atomicity under external concurrent actors, or completion
of the original ROS task. Native cancellation can still fail or time out after a
successful preflight; external effects already admitted cannot be rolled back.
The driver models use plain-text fake products and explicitly mock ELF checking
only within the tests; production ELF and live cgroup assertions remain enabled.
