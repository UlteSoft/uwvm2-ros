# Complete industrial Wasm example witnesses

These entry points compile the actual selected vendor reactor translation unit,
including its original generated complete record/interface headers. They do not
replace the transport with stubs. Select `UWVM_INDUSTRIAL_C_SOURCE` for
`denied.c` or `UWVM_INDUSTRIAL_TRAJECTORY_SOURCE` for `denied.cpp`, each containing
the quoted absolute path to the reviewed vendor example. Also select the actual
matching `UWVM_INDUSTRIAL_GENERATED_RECORDS="bounded_records.hpp"` and
`UWVM_INDUSTRIAL_GENERATED_INTERFACES="bounded_interfaces.hpp"` for C++.

Use the complete SDK directory structure: `sdk/include`, `sdk/common`, and the
selected `sdk/ros1` or `sdk/ros2` raw ABI directory belong to one source revision.
An isolated copy of `sdk/include` is insufficient. Use the actual WASI sysroot,
generated catalog directory and source root as explicit include paths. C uses
C11; C++ uses C++20 with `-fno-exceptions -fno-rtti`. Preserve finite floating
semantics with `-fno-fast-math -ffp-contract=off`. The actual industrial CMake
reactors reserve a 1 MiB stack, start with 2 MiB of linear memory and have a 32 MiB
linear-memory ceiling. Apply the same linker limits to these executable entry
points; unlike the reactors, these entry points retain normal WASI startup.

Validate each complete Wasm module with a full validator, then use
`sdk/tools/check_wasm_imports.py --backend 2 --allow-module wasi_snapshot_preview1`
to verify actual fixed ROS import signatures. This import inspector is not the
full validator. Execute with the selected Runtime's ordinary run command and
no ROS grants, preopens, network grants, policy files or Gateway configuration.
Record the exact Runtime identity; passing these witnesses does not qualify an
unrelated/current Runtime build.

The C entry checks invalid-mode refusal, denied open, retained primary/cleanup
status and repeated initialization/close refusal. The C++ entry additionally
builds complete Goal, Feedback, JointState and Result values and encodes each;
checks cross-field sequence copies; challenges an invalid source count, a
nested smaller destination and text overflow; and requires previous destination
values to survive refusal. It then requires the actual first ROS open to fail
with 13, confirms zero application receive/goal counters, and rejects subsequent
submit/poll/cancel calls after retirement. No goal is submitted.

These are local Guest/value/default-policy witnesses. A successful Runtime
process exit is not proof of native delivery or independently observed absence
of native effects. Positive Gateway traffic, controller feedback and physical
stop/cancel must be observed in their separate admitted integration lanes.

The [r29 delivery record](../../../documents/ros/continuation-20261018-evidence/fanuc-original-sdk-and-native-values-r29/README.md)
records actual FANUC C11/C++20 builds, four full-validation/import checks,
two no-grant executions, exact product identities and read-back memory limits.
Other vendor instantiations require their own execution evidence.
