# Actual original-rclcpp source and build composition

This build-time entry recompiles the fixed original `ros2/rclcpp` commit
`53fa7aeea86e8ed484f9465fa14059faf13a71f1` to Wasm. It preserves the original
application sources, public NodeOptions and complete original package tree.
The four explicit source ports provide the genuine WASI signal worker and
correct ownership of the original C GetTypeDescription request/response.
C++ message allocation and destruction keep their original behavior.

`prepare_rclcpp.py` takes an independently admitted real Git executable, its
whole-file SHA-256, a complete local original Git object database, the actual
reviewed uwvm2-ros tree, and the exact reviewed `ports.json` plus its digest.
It verifies the fixed commit, every intermediate binary tree and every blob
object identity, derives the entire inventory from that verified tree closure,
and preflights the entire
finite tree inventory, reads original bodies directly from Git, verifies the
five critical original preimages, and exclusively creates a fresh source tree.
There is no checkout, hook, filter, partial clone, network fetch, source merge or
deletion. Submodules, symbolic links, special files, traversal, duplicate paths,
changed port bodies and existing output directories are refused. Explicit source
limits are 8,192 files, 64 MiB per file and 256 MiB total original source.
Tree traversal separately permits at most 2,048 expanded directories, 16 MiB
of expanded binary tree bodies and depth 64. A recursive `ls-tree` listing is
not used as a substitute for hashing every intermediate tree object.

All original members are retained byte-for-byte except the four named ports.
`rclcpp/CMakeLists.txt` stays byte-exact. `.uwvm-source.json` records every
original Git blob and SHA-256, every actual after body, and which four were
ported. A failed extraction leaves an explicitly incomplete owned directory;
do not build, merge into, retry or certify that partial directory. Start with a
new output after investigating the failure. This tool does not infer any other
package's revision or supply an incomplete ROS dependency tree.

For the later unified test phase, a source preparation invocation is:

```sh
python3 sdk/rmw_wasm/original_build/prepare_rclcpp.py \
  --git /ABSOLUTE/ADMITTED/git \
  --git-sha256 ACTUAL_COMPLETE_GIT_EXECUTABLE_SHA256 \
  --original-repository /ABSOLUTE/COMPLETE/ros2-rclcpp \
  --source-root /ABSOLUTE/REVIEWED/uwvm2-ros \
  --ports /ABSOLUTE/REVIEWED/uwvm2-ros/sdk/rmw_wasm/original_build/ports.json \
  --ports-sha256 ACTUAL_COMPLETE_REVIEWED_PORT_MANIFEST_SHA256 \
  --output /tmp/FRESH_OWNED_ORIGINAL_RCLCPP
```

Local test/build outputs belong in `/tmp`. All Linux build and execution work
belongs inside the Root-owned actual 64 GiB, zero-swap cgroup, with separately
observed process deadlines, physical retirement and disk headroom. Build jobs
may be raised only while the actual shared memory and I/O budget permits it.
The source preparer does not create a cgroup or expand those budgets.

## Actual static package composition

Use the real SDK33 threaded standard-EH toolchain, fixed complete original ROS
dependencies and the static cache/project hook for every affected package.
Build and install the actual `uwvm_rmw_dispatch` first, followed by the threaded
platform and core (`UWVM_RMW_ENABLE_THREADED_PROFILE=ON`). Build the enabled
WASI signal package and enabled C-service-lifecycle package against genuine
generated C APIs. Both require a fresh actual executable link probe. This
documentation is not a replacement for their real archive/link/import evidence.

The original-rclcpp project hook calls
`uwvm_prepare_original_rclcpp_composite()` from `CompositeOriginal.cmake`.
Select `UWVM_BUILD_ORIGINAL_RCLCPP_WASM=ON` explicitly; it defaults to OFF.
The hook verifies real static package targets, the same genuine threaded gate
in core/platform/dispatch, both actual executable-link admission properties,
and both exact fixed-original port attachment helpers. Dependencies are exported
before original `ament_package()`, and the attachment runs after the original
`add_library`. Every original template consumer receives the real C-lifecycle
helper through a PUBLIC dependency, preventing mixed allocation/destruction
semantics across translation units.

The exported `UWVM_ORIGINAL_RCLCPP_COMPOSITE=1` is a build-composition marker.
It grants no capability and attests no Runtime, ABI, memory layout, ROS network
delivery or official upstream suite. The complete original default Node
bootstrap must also prepare the real default-control Registry and immutable
codec composer, actual original-RMW endpoint selections, platform clock/logging
and source-specific signal profile before entering the original application.
Every resulting ROS operation still requires an explicit Host grant. No Node
default is disabled to hide a missing codec, and no finite facade include path,
native JVM/CPython, shadow ROS type or guessed native layout is supplied here.

Final actual qualification must independently bind the complete source tree,
tool/sysroot/archive members, C++ ABI/EH/TLS, full module imports and shared
memory, once-only initial-main and all physical child/native owners. Default
denial, explicitly granted complete default Node, unchanged upstream examples,
official suites, multiple distros/vendors/architectures and performance remain
separate required executions. All newly authored producer/build/contract runs
are zero until their actual results are recorded.
