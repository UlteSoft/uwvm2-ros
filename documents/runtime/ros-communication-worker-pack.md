# Packaging the communication startup closure

`tools/ros_scene/pack_communication_worker.py` produces a finite image manifest
for the independent communication sandbox. Run it through the existing
`tools/ros_scene/in_existing_cgroup.py` gate. It never invokes `ldd` or executes
an input ELF image. It reads `DT_NEEDED` and `PT_INTERP` using the pinned `readelf`
tool, then copies the actual bytes into the output directory and records their
lengths and full SHA-256 digests. The current helper is scoped to x86_64 Jazzy
images; this is narrower than the portable scene interface contract.

Required inputs are the approved bootstrap, native backend, single-node profile,
extracted ROS root and fresh SDK `.tmp` output directory. The launcher defaults
to the installed `/usr/bin/bwrap` but is also copied and hashed. ELF dependencies
are resolved only within the selected ROS library directory, its `system-libs`
directory and the explicit x86_64 host runtime directories. A library symlink
that resolves outside those roots is rejected. Names requested by `DT_NEEDED`
must be basenames; unresolved libraries fail packaging. A repeated runtime
basename with different content is rejected rather than depending on search
order to choose one.

`--dlopen NAME` declares an exact lazy-loaded ELF library. `--resource PATH`
declares one ROS-prefix-relative data file, including an ament marker, XML or
other resource. Resource bytes receive the nonexecutable configuration role.
There is no recursive directory grant. Runtime and configuration bytes use
separate output files even if their digests happen to match. The image budget
includes the launcher plus two copies of every mounted image, matching sandbox
preparation. Excessive size, more than 256 mounted files, conflicting
destinations and file/directory collisions are rejected.

An optional `--trace FILE` ingests successful `open`/`openat` calls from a complete
`strace -f -qq -s 4096 -e trace=open,openat` capture. Interleaved unfinished/resumed
calls are reconstructed by thread ID. Missing halves or an empty successful-open
set reject the trace. Observed ROS ELF/resource files join the manifest. Unknown
successful paths reject packaging; they are not silently omitted. An explicit
`--transient-probe PATH REASON` can record a reviewed probe expected to be absent
inside the namespace. That disposition grants no file access and is not evidence
that the runtime tolerates its absence. The dynamic linker cache is excluded
because the namespace uses an explicit image-derived library search path.

The output `manifest.json` contains the copied launcher and all images, source
and resolved source names, logical destinations, roles, sizes, hashes, dependency
edges, trace hashes and probe dispositions. `complete: true` means the finite
packaging operation completed; `qualified_runtime: false` is deliberate. This
receipt does not prove that prepare closed every lazy load or that a native ROS
operation can execute after the final filter. Source path selection is trusted
deployment configuration. Final-component `O_NOFOLLOW` does not secure ancestors;
the sandbox's immutable copy and digest verification bind the consumed bytes.

`tools/ros_scene/run_communication_worker_startup.py` takes that manifest and an
explicit live-thread maximum. It freezes its source closure, builds the test
driver with `-O3 -g`, checks every image digest, and runs actual
prepare/namespace/identity/ready/pipe-attachment/attached/pidfd-cleanup. It does
not connect the test pipes to a UDP broker or send endpoint RPCs, and its output
states `startup_only=true`. Native topic/service/action/graph operations must
still run through the real supervisor and directed transport before that
middleware profile is qualified.

`communication_pack_checks.py` tests actual ELF traversal and copied-byte
integrity, missing libraries, path traversal, finite image limits, unknown
successful opens and incomplete traces. Its trace-parser fixtures are synthetic
records and are explicitly described that way in the receipt. They are not
substitutes for an actual ROS initialization trace.
