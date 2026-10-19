# Actual SDK33 threaded C++ toolchain source

This wrapper is an explicit source-recompile route for original `rcutils`,
`rmw`, `rcl` and `rclcpp`, their actual generated type support, and the application.
It does not reuse native client-library binaries, supply facade ROS headers, or
introduce authority. All final calls remain Wasm imports checked by uwvm2.
The older `wasi-toolchain.cmake` keeps its original single-thread source role.

The implementation includes the **actual installed** SDK33 pthread toolchain,
whose entire bytes must equal the pinned official source. It retains original
pthread and imported/exported shared-memory flags. SDK33 supplies separate real
EH and no-EH C++ libraries; standard Wasm EH needs `-fwasm-exceptions`,
`-mllvm -wasm-use-legacy-eh=false`, and `-lunwind` at link. This wrapper supplies
those flags and requires actual threaded EH directories. Directory presence and
source hashes do not prove the object contents, compiler version or ABI.

All memory inputs are mandatory, finite, page aligned, and at most 2 GiB per
Wasm32 shared memory. These are linker resource bounds, not capabilities or a
substitute for the Host process/group memory budget. Choose them consistently
with actual thread TLS/stack/data extents and Runtime reservations. A true
linker failure is an error; no missing unresolved symbol is replaced by a stub.

The machine recipe is authored source, not an executed compiler report. Use a
fresh privately owned output tree and already admitted prefix/tool/sysroot
closure. Keep Host tools/generators separate from target headers/libraries.
Do not inherit facade include paths, another native SDK, permissive unresolved
link flags, or an unrelated global compiler cache. The real signal package
performs its independent executable pthread/CV/semaphore/TLS/standard-EH link
probe. Root must then inspect actual final imports/signatures/memory/TLS/tags and
execute the original Guest/Native/correctness/permission/retirement cases.
The current original-rclcpp project hook still refuses incomplete composition.
This wrapper has not been configured, compiled, linked, imported or executed.

Local test products must remain under `/tmp`; on Linux every tool, compiler,
model, Guest, Native peer, QEMU process and profiler must remain in the actual
64 GiB cgroup with swap disabled. Root owns aggregate memory/disk/deadline and
physical-join limits. A source recipe cannot establish those operating-system
properties. The recipe defaults to one build job; a separately observed cgroup
budget can justify more jobs, as the user authorized.
