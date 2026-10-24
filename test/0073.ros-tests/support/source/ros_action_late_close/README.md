# ROS2 Action lifetime regression

The native Jazzy server holds acceptance and cancellation callbacks behind
bounded barriers. The test talks to a real Gateway through authenticated IPC
and observes native request, cancellation and terminal-result effects.

Modes: `accept`, `reject`, `accepted`, `disconnect`, `refuse`, `expired`,
`expired-owner`. The first six configure one session and one active outgoing
goal. `expired-owner` explicitly permits two sessions, so global capacity is
two while this live owner's quota remains one. Consuming an expired result
must retire the Guest GoalID (error 9), retain the owner's quota (error 11),
and permit a new goal only after an actual native terminal result.

`run_native.py` requires a fresh output workspace, an immutable source tree,
a prebuilt C++17 Gateway and its exact source/MMD/build flags/link command,
the prepared Jazzy SDK root, testcase overlay and Gateway origin report:

```sh
python3 run_native.py --workspace NEW_WORKSPACE --source SOURCE \
  --gateway GATEWAY --gateway-source GATEWAY_SOURCE --gateway-mmd GATEWAY_MMD \
  --flags FLAGS_MAKE --link LINK_TXT --sdk SDK_ROOT --inputs TEST_OVERLAY \
  --origin-report GATEWAY_REPORT --phase after
```

Run in a private Linux mount namespace (`unshare --mount --propagation private`)
inside the existing 64 GiB cgroup. The probe uses C++20 for fast_io and the
official Jazzy SDK. The Gateway retains its C++17 production requirement.
Every native mode has a 30-second limit; the runner records real executable
identities, private read-only source mounts, OOM deltas and cleanup. `--modes`
selects comma-separated cases. `--phase before --modes expired-owner` records
the expected C2 owner-quota failure, not a passing product regression.

These are explicit test permissions, not runtime defaults. Cancellation
acknowledgement does not prove a goal has ended; a server may refuse cancel or
never finish. Such goals retain bounded cleanup capacity until a known native
terminal result or Gateway shutdown. No broad deadline-race or crash-recovery
guarantee is inferred.
