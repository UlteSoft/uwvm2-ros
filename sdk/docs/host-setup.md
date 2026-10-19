# Bind a guest to explicit host authority

Compile the selected language example first and inspect its binary imports.
The resulting Wasm file has no ROS resources until the trusted Runtime policy
grants aliases to its actual Instance/Single/Group binding. An SDK factory
accepts an alias such as `pub`; it cannot replace the trusted caller identity,
resource, native type identity or QoS.

## Prepare a bounded String tutorial

The SDK policy generator produces a new private `gateway.conf` and a JSON array
of exact Worker arguments. It does not execute either file. Supply the actual
Worker UID/PID and Gateway UID yourself, choose a private short Unix socket,
and review every generated limit before using it. Select `--operation publish` or `--operation subscribe` explicitly. Repeat the
flag to select both. No other operation or topic is granted.

The following is a **generation example**. Replace the identity values with the
actual bound Worker process; PID12345 is not a usable test identity:

```sh
python3 -B sdk/tools/example_policy.py --backend 2 \
  --worker-uid 1000 --worker-pid 12345 --gateway-uid 1000 \
  --socket /absolute/private/ros2.sock \
  --node-name sdk_demo --namespace /sdk_demo --resource /sdk_demo/chatter \
  --operation publish --operation subscribe \
  --domain-id 74 --disabled-security-tutorial \
  --out /absolute/private/new-sdk-policy
```

This ROS2 tutorial explicitly opts into `security-mode=disabled-test` and
`not-applicable` keystore/enclave. It is a local test profile, not a security
default or an SROS2 Enforce recipe. For an Enforce deployment, provide a
reviewed keystore/enclave and the real Runtime's required policy fields.

For ROS1, replace the ROS2-specific flags with explicit connection values:

```sh
python3 -B sdk/tools/example_policy.py --backend 1 \
  --worker-uid 1000 --worker-pid 12345 --gateway-uid 1000 \
  --socket /absolute/private/ros1.sock \
  --node-name sdk_demo --namespace /sdk_demo --resource /sdk_demo/chatter \
  --operation publish --operation subscribe \
  --master-uri http://127.0.0.1:11311 --advertise-host 127.0.0.1 \
  --out /absolute/private/new-sdk-policy-ros1
```

Every tutorial grant sets byte/rate/burst/queue/audit limits explicitly. ROS1
also sets TCPROS, queue depth and publisher latch. ROS2 sets all eight endpoint
QoS values explicitly. These visible sample values are not inherited Runtime
defaults. Review `policy.json` before using its `Worker_arguments` array.

## Validate before starting a real session

Use the `Worker_policy_check_arguments` array with the freshly qualified Worker
to check policy syntax. Passing that check does not start ROS, prove native type
availability or establish the IPC peer binding. A complete startup also needs
the exact backend Runtime build, prepared native Gateway, native ROS SDK,
private owned configuration and socket, and the real Worker PID/UID.

The repository integration runner holds the **actual Worker PID** before exec
and binds the Gateway's peer credentials to that same process. A native
`chroot` launcher may instead fork an ARM child; those two PID models are not
interchangeable. Use the existing qualified integration runner rather than
inventing a second Worker PID after writing the Gateway configuration.

All actual SDK/ROS/ARM compilation and behavior validation for this task runs
inside the existing 64 GiB Linux cgroup with zero swap. Preserve current source/SDK manifests,
actual compiler inputs, live ELF183/QEMU mapping evidence, original assertions,
raw failures and process/mount/binfmt cleanup. See
[verification.md](verification.md) for current status.

## Add resources deliberately

A service server needs separate advertise/respond aliases with matching
resource/type contracts. An Action client needs distinct send/feedback/result
aliases; cancel-own and cancel-any are separate permissions. A ROS2 Action
server needs advertise/respond and matching 40-field QoS contracts. Parameter
event projections, graph filters, clocks, TF frame pairs, lifecycle transitions
and logging require their own grants. The String tutorial does not authorize
these additional examples.

Keep the guest's own failure path useful. A default-deny open returning13 is
correct behavior. Do not catch that error and substitute another backend, a
wildcard alias, a native socket or a fabricated handle. The SDK does not grant
directory/socket preopens, WASI environment inheritance, clocks or threads.

The current WASIp1 default also refuses clocks, randomness, host signals and
yield operations. If a guest deliberately needs a monotonic clock, select that
one permission with `--wasip1-global-grant monotonic-clock`, or grant it to the
specific WASI Single/Group. A target `--wasip1-single-deny` or
`--wasip1-group-deny` removes an explicitly granted Global bit for that target.
ROS resource grants never supply these WASI permissions. The `stdio-full-rights`
compatibility permission deliberately broadens standard-descriptor rights;
the basic String example does not need it. See the repository's
[WASI CLI guide](../../documents/command-line/wasi.md) for the finite names,
scope merge rules and explicit environment inheritance option.

## Keep module management outside Guest authority

The ROS Guest ABI provides no Runtime update, debugger or policy-configuration
import. Successful trusted code replacement permanently retires ROS authority
for that module's current incarnation, including its old handles, connections
and queued events. Using ROS again requires a fresh trusted launch and explicit
CLI grants. Guest stdout and ROS logs are application data and must not be fed
into the trusted control console. See
[update-and-debug-boundary.md](update-and-debug-boundary.md) for commit ordering,
retired diagnostic state and temporary DAP reference lifetimes.
