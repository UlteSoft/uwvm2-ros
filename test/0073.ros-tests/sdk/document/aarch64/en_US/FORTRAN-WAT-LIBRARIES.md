# Fortran and WAT library acceptance

These routes remain open pending the single shared Flang23 adapter/runtime and
source-pinned WAT assembler/linker. The platform supplies its original complete
schemas; it installs no private compiler and does not replace Fortran with C.
Three distinct codec examples and both real widths are required per component.
The NVIDIA umbrella explicitly selects one component and reuses its modules.

A binary is one application Wasm. A shared library is a separately loaded Wasm
with recorded imports, exports and its own memory. A static archive contains
actual relocatable objects. Use ordinary Xmake binary/shared/static and normal
dependency options. Passing textual assembly through an assembler proves no
archive, native peer or dynamic-module interoperation.

For a static cohort, retain producer source hashes, compiler/assembler version
and argv, object metadata, indexed archive, linker symbol-extraction trace and
consumer Wasm. Require an unreferenced sentinel member to stay unextracted and
execute the consumer. wasm_archive.py performs bounded member admission only;
it never claims compiler, link, extraction or runtime qualification. The helper
rejects an ordinary final module without linking metadata even inside an archive.

For shared cohorts, use exact named io/algorithm/ros_comm modules. Bind scalar
or module-owned token interfaces; preserve full-width handles and explicit
retirement. Match real Fortran bind(C) signatures with actual WAT imports. The
algorithm has default WASI and no added ROS grants; ros_comm alone receives the
manual exact original-Array aliases. Preserve the submitted config bytes.

Execute positive communication, denied calls, close/revoke, transitive loading,
collisions and width/signature mismatch refusals on x86 and QEMU ARM. Independently
observe every original field, including both detections and all72 covariance
values. Use the existing source-paired runtime/gateway/native fixtures. Keep
physical GPU/NITROS and an ARM compiler host as separate open qualifications.
Every job holds the global lease and records its own actual64GiB/zero-swap gate.
