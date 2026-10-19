# Borrowed scalar String validation and native result construction

`string_validation_test.cc` checks canonical String framing, UTF-8 (including
embedded NUL), exact quota and identity errors, and unchanged output on refusal.
The allocation interval starts after the policy and input bytes exist. Validation
without a requested decoded value must allocate zero times; requesting a decoded
value still creates an owning String.

`native_string_result_test.cc` exercises both generated ROS 2 ParameterValue and
native rclcpp ParameterValue result helpers. It checks exact output bytes and
error precedence before measuring allocations. With the optimized helpers, a
successful 128 KiB String needs one output vector allocation; all tested refusal
paths need zero allocations. This covers the helpers, not a complete Node RPC or
middleware deserialization.

Both Python executors require a fresh output directory and the supplied Linux
64 GiB cgroup. They retain actual commands, logs, compiler MMD inputs, live ELF
identity captured before hashing a pinned FD, and readable-ancestor OOM counters.
The standalone executor uses six C++17/20/26 sanitizer profiles across libc++ and
libstdc++. The private Jazzy rootfs executor uses GCC 13 C++17/20, each normally
and with ASan/UBSan. The C++17 probes use the existing native I/O adapter; C++20+
output uses fast_io. Native result execution leaves the frozen SDK and baseline
source, binaries and reports unchanged and checks its owned process/mount cleanup.

Keep failed attempts and original before reports. `--optimized` selects the
standalone allocation assertions; `--phase after` selects native helper allocation
assertions. Before and after inputs must be archived separately.
