# Complete original industrial protocol route witness

This is an explicitly enabled native-process integration test. It starts the
production ROS2 Gateway and independently compiled original ROS2 SDK peers.
It does not start a robot, load a Guest Wasm module or certify an upstream driver
test suite. Synthetic trajectories and vendor requests exercise field transport;
they are unsuitable for hardware and must remain in an isolated network namespace.

The containment owner must enforce a hard memory cgroup, no swap, serial builds,
a bounded private build volume, an isolated loopback-only network namespace and
exact immutable source/package/toolchain selection. It must reap its process
group and unmount owned views on failure as well as success. The SDK's normal
node is used only by the trusted native peer; the Gateway retains its composed
node with no implicit ROS endpoint or simulation clock subscription.

1. Restore the actual original interface-package overlay and the matching
   installed protocol IDL/source proof, bound registry and native provider.
2. Run `industrial_value_oracle.py` on those captured inputs, then run
   `prepare.py --catalog ABSOLUTE_CATALOG --installed-idl ABSOLUTE_INSTALLED_IDL
   --oracle ABSOLUTE_ORACLE_DIRECTORY --out FRESH_ABSOLUTE_DIRECTORY`.
   Preparation checks all three original digests and copies the complete
   independent native value specializations unchanged. Every registered Message,
   Service and Action gets its own translation unit.
3. Build the actual production Gateway with the explicit same registry, source
   provider and installed packages. Build this directory with
   `UWVM_BUILD_INDUSTRIAL_GATEWAY_WITNESS=ON`, `UWVM_INDUSTRIAL_WITNESS_VALUES`,
   `UWVM_FAST_IO_INCLUDE`, `UWVM_ROS_CUSTOM_REGISTRY_HEADER` and the explicit
   semicolon-separated `UWVM_NATIVE_PACKAGES`. Assertions must remain enabled.
4. Launch `uwvm-industrial-gateway-native --trusted-native-fixture GATEWAY
   CONFIGURATION_DIRECTORY` under the original parent. At the first `SIGSTOP`,
   the owner calls `configuration.prepare` with that actual child PID/UID and
   the selected `selection.json`; it then resumes the child and writes `I`.
5. At the second `SIGSTOP`, inspect the original native and Gateway processes,
   their cgroup/network namespace and kernel `map_files` objects before resuming
   and writing the final `I`. Retain their exact mapped ELF bytes, full compiler
   dependencies, source pins, logs and exit statuses.

The zero-capability policy is checked before any grants. The positive fixture
policy names every allowed resource and exact native/canonical identity; it is
never inferred from discovery. The test checks malformed and duplicate binding,
wrong process credentials, absent aliases, denied debugger/update operations,
three complete value modes per interface, bidirectional Topics and Services,
malformed Service payloads before native invocation, duplicate responses,
Action feedback/results/cancel and cross-session goal ownership. Native callback
equality checks all original fields; exact independent canonical byte comparisons
also check the return path. The separate value-oracle contract covers signed-zero
bits, canonical decoder destination preservation and native CDR conversion.

Use the explicit loopback UDP profile from `../legacy_protocol_gateway/loopback.xml`
inside an independently isolated network namespace with only loopback enabled.
The XML file alone is not containment. Do not run this witness on a robot network.
