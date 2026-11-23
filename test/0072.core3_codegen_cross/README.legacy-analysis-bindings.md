# Legacy analysis binding regression

`legacy_analysis_bindings.cc` links against the configured bundled LLVMCore and
its dependency closure, using the same C++ runtime and RTTI setting as LLVM.
For the normal ROS provider, compile it with `-O3 -g -fno-rtti` and the generated
LLVM and libc++ include directories. Its exit status must be zero.

The oracle reproduces the original resolver's first-binding behavior, including
changed implementations of an existing analysis ID. It covers initially empty
and partial availability, repeated initialization, late availability, changes at
every dependency position, removal, explicit clearing, reverse initial order,
empty requirements, and moving a populated last-use set. It neither changes
analysis invalidation policy nor treats a missing analysis as available.

The object-comparison smoke check used for this change separately emitted the
same checked-pointer, root-reload, atomic and vector IR through both providers.
The complete native objects were byte-identical. This focused test does not
replace a multi-target compiler qualification or full Wasm conformance suite.
