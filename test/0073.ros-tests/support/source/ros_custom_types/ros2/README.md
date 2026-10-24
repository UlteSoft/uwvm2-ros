# Fixed-layout custom ROS2 topic fixture

This directory prepares the first concrete custom ROS2 Topic increment. It
contains an official interface package, an identity-free preparation catalog,
and a real native DDS fixture. It does not provide arbitrary runtime type
loading, custom Service or Action support, or Guest/IPC acceptance evidence.
Compilation and execution are pending. All actual compilation, native tests,
Guest tests, and ROS tests must run in the existing verified Linux 64 GiB cgroup.

## Generate authoritative native interfaces

The `interfaces` package is named `uwvm_ros_custom_test_msgs`. Build it through
official Jazzy `rosidl_generate_interfaces` in a fresh, task-owned Linux build
and install directory. Preserve the source files, complete compiler dependency
closure, installed generated headers and TypeSupport libraries, and official
type-description/hash artifacts. Record the architecture and cgroup process
proofs required by the current ARM acceptance controller. A recipe, a source
scanner, and an identity string copied into a catalog are not native evidence.

`FixedSample.msg` has only primitives and fixed arrays. Its canonical record is
45 bytes: `uint32 sequence`, a one-byte boolean, `float32 temperature`, three
`float64 acceleration` fields, eight `uint8 tag` fields, and two `int16 thresholds`.
All multibyte values use little-endian encoding. Boolean bytes must be 0 or 1;
all floating-point values must be finite. Native ROS2 CDR is never forwarded.

## Generate the trusted adapter pair

The preparation catalog deliberately omits `native_identity`. Check its layout
with the repository's generator:

```text
python3 sdk/origin/tools/custom_types.py test/0073.ros-tests/support/source/ros_custom_types/ros2/catalog.preparation.json --check
```

After official interface generation, copy this catalog into a fresh task-owned
directory, keep its message file reference correct, and add the exact
`native_identity` from the official generated `FixedSample` type-description
artifact. The identity must have the authoritative `RIHS01_` hash. Run:

```text
python3 sdk/origin/tools/custom_types.py /absolute/fresh/catalog.json --out /absolute/fresh/generated
```

Preserve `generation.json` and all generated files. The shared `manifest.hpp`
and typed `native_ros2.hpp` must come from the same generation. Their separately
pinned codec identity includes the canonical schema; the native RIHS identity
alone does not identify the canonical wire layout. Never manufacture a native
identity to make generation succeed.

## Select support explicitly in the trusted build

Build the Gateway with the following CMake inputs after the official package
is installed and available through its trusted package prefix:

```text
UWVM_ROS_CUSTOM_REGISTRY_HEADER=/absolute/fresh/generated/manifest.hpp
UWVM_ROS_CUSTOM_ROS2_HEADER=/absolute/fresh/generated/native_ros2.hpp
UWVM_ROS_CUSTOM_ROS2_PACKAGES=uwvm_ros_custom_test_msgs
```

The first option selects the shared compile-time registry through
`UWVM_ROS_CUSTOM_TOPIC_MANIFEST`; the second selects typed native codecs through
`UWVM_ROS_CUSTOM_ROS2_HEADER`. Select the same shared manifest when rebuilding
the Worker. Empty default inputs provide no custom support or additional
permission. Adding a generated adapter never creates a ROS capability.

A production Topic capability must still explicitly select ROS2, the exact
absolute resource name, operation, native type and RIHS identity, all existing
quota/QoS/audit fields, and `adapter-codec-identity` matching the generated codec.
The Gateway checks the official generated C++ name, TypeSupport hash and
description name before creating any custom entity. It also checks the actual
resolved topic name and complete existing actual QoS before activating ingress.
The official TypeDescription must match every generated field in declaration
order, its exact primitive type, fixed-array bound and nested name, and the full
unique transitive referenced-record closure. Missing, extra, reordered, dynamic,
default-valued, cyclic or disconnected definitions refuse admission. A genuine
RIHS identity copied beside a different catalog does not satisfy this check.
TypeDescription does not expose constant declarations, so this field comparison
does not establish that native source contains no constants; the catalog parser
refuses constants in its own input.

The comparator follows the official Jazzy
[field/type structures](https://raw.githubusercontent.com/ros2/rosidl/jazzy/rosidl_runtime_c/include/rosidl_runtime_c/type_description/field_type__struct.h)
and [description generation](https://raw.githubusercontent.com/ros2/rosidl/jazzy/rosidl_generator_c/resource/full__description.c.em).

## Prepare and run the native fixture

Configure this directory with the same two generated header inputs. The target
`uwvm-ros2-custom-topic-probe` is C++20 so its diagnostic I/O uses vendored
`fast_io`; the production custom adapter remains C++17 compatible and performs
no I/O. The fixture explicitly selects DDS domain 203 and disabled security for
this isolated native test. Those settings grant no production or Guest rights.
Use a task-specific domain lease through the Linux controller and do not run
concurrently with another domain-203 fixture.

The fixture exercises the real custom adapter against ordinary native ROS2
publishers and subscribers. It checks exact round trips, malformed/truncated
records, trailing bytes, noncanonical booleans, NaN/Infinity, smaller explicit
quotas, wrong native/codec identities, alias/resource rebinding, category
confusion, native nonfinite ingress, and close followed by later native traffic.
Malformed payload cases include actual native receive-counter checks. Closing
the adapter disables ingress and deactivates its bounded queue.
Nonfinite native ingress cases require an independent native subscription to
observe each invalid message, so a missing DDS delivery cannot pass as rejection.

The prepared fixture does not yet inject native actual-QoS or TypeSupport query
failures, execute a Wasm Guest, test authenticated IPC, or prove sandbox-wide
attack coverage. Its successful future native marker must not replace those
separate required acceptance rows. Preserve complete logs, source/artifact
hashes, stopped ARM mappings and process ownership, dependency evidence, and
zero-OOM/swap/cgroup cleanup results before recording a real execution status.
