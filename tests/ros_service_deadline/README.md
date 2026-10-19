# Service response deadline probes

`respond_native_ros2.cc` includes the real ROS2 Gateway adapter and uses a generated native Trigger client over DDS. Its six independent modes are `normal`, `zero`, `call-wire`, `cap-wire`, `request-wire`, and `native-copy`. The last four delay one exact allocation on the operation's calling thread for 150 ms against a 50 ms deadline; discovery and executor threads are not armed. The probe observes the real native response before asserting that an expired operation returns 110 with no native response admission. Live requests remain retryable after a short call timeout; a request whose own deadline expires is replaced with a fresh native request. Normal and retry responses must preserve every native String byte and retire their request IDs.

The fix checks an absolute deadline captured at the adapter operation entry, including response QoS checks, waits for the service-state lock, wire preparation, and native response preparation. ROS2's native send is admitted under the service-state lock only after the final preparation check. ROS1 carries that admission deadline to its waiting native callback and checks it again after preparing the generated response. Ordinary mutex acquisition and the ROS SDK send/serialization call are not interruptible by this code; neither a hard wall-clock return bound nor a bound on DDS/TCP network delivery latency is claimed.

`state_test.cc` checks both ROS1 and ROS2 grant forms without a ROS network. It covers zero budget, retry, exact wire, duplicate and completed IDs, native request expiry, lock waits, close during wire preparation, native preparation, late wait output preservation, and a foreign resource. `run_cpp17_checks.py` builds and runs this test with Noetic GCC9 in C++17 mode and also freshly compiles and links the ROS1 Gateway. That stage does not execute a ROS1 network service.

Use fresh workspaces and a frozen source snapshot with `source/`, the original 18 native binaries, the original report, and the Gateway build's `flags.make` and `link.txt`. The input directory must contain the selected new probe/driver and, for the after phase, the three modified production files. Example:

```sh
python3 -B tests/ros_service_deadline/run_native.py \
  --workspace /absolute/new-workspace \
  --baseline /absolute/frozen-events-r4 \
  --sdk /absolute/native-sdk-workspace \
  --inputs /absolute/frozen-inputs --phase after

python3 -B tests/ros_service_deadline/run_cpp17_checks.py \
  --workspace /absolute/new-noetic-workspace \
  --baseline /absolute/frozen-events-r4 \
  --sdk /absolute/native-sdk-workspace --inputs /absolute/frozen-inputs
```

The focused before/after runs use domain 193, a private mounted rootfs, actual C++17 compiler dependencies, pinned-ELF PID evidence, and the existing 64 GiB cgroup. Before evidence retains the five real rc1 semantic failures. Each report preserves raw command results, source hashes, mount cleanup, readable ancestor OOM counters, and the unchanged baseline source, binaries, and report. These focused snapshots precede the separate `config.h` getter optimization; the final complete project integration must bind any later production revision separately.
