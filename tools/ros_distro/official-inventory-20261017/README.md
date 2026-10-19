# Official ROS and Unitree test scope: source inventory only

`official-suite-project-inventory.json` describes all 26 maintained distribution
targets and the nine Unitree repositories already named in the project's lock.
It is a source/planning artifact. Every actual test count is null, executed count
is zero, and no distribution or official suite is qualified by this inventory.

The data-only observation includes the current official index, all 21 indexed
`distribution.yaml` bodies, both target-platform REPs, and the Lyrical platform
document. The five older ROS 1 targets still need authentic historical manifests.
The full repository metadata is kept separately in
`all-distro-repository-metadata.json`; its indentation-based scalar inventory is
not a YAML execution or acquisition parser. Actual acquisition must use pinned
rosdistro tooling, resolve every source/release reference to immutable input, and
retain a complete physical source/SDK catalog. Repository-row counts are not
package counts or test counts. A ROS 2 `ros2.repos` source checkout is a distinct,
usually narrower universe than every repository in the release index.

For Unitree, `unitree-tree-observations.json` records complete, non-truncated
remote trees selected by existing reference tree IDs. The two file-observation
indexes bind 76 README/build/workflow/example bodies and 26 package manifests;
each remote body was checked against its Git blob ID. These are remote byte
observations, not Linux opened-file pins, native identity qualification, or proof
that dependencies/builds/runtime cases exist. Existing reference commits were
copied from the maintained lock; none was invented. The unsuccessful initial
commit-echo tree-query observation is retained separately as an observation
attempt and is not used as a successful tree authority.

## Execution and discovery implications

ROS 1 catkin's upstream all-tests route is `run_tests` followed by result
inspection; it also builds test executables. Its gtest, nose and rostest results
must be reconciled, using the actual historical interpreter and tools. A plain
CTest invocation is not equivalent. Pre-Groovy rosbuild needs its actual original
workspace/test workflow. ROS 2 Ardent used ament_tools; Bouncy moved to colcon.
These historical distinctions are backed by the official
[catkin test documentation](https://raw.githubusercontent.com/ros/catkin/noetic-devel/doc/howto/format2/run_tests.rst),
[rosmake source documentation](https://docs.ros.org/en/jade/api/rosmake/html/rosmake.engine-pysrc.html)
and [ROS 2 build-system overview](https://raw.githubusercontent.com/ros2/ros_core_documentation/master/source/developer_overview.rst).

Colcon requires explicit failing-test return-code handling and separate result
inspection. CTest JSON discovery and JUnit output also have different minimum
tool versions; old SDK CMake versions cannot be presumed to support both.
The runner must pin an actual compatible tool rather than silently replace a
historical workflow. See the official
[colcon test](https://colcon.readthedocs.io/en/released/reference/verb/test.html),
[test-result](https://colcon.readthedocs.io/en/released/reference/verb/test-result.html)
and [CTest](https://cmake.org/cmake/help/latest/manual/ctest.1.html) references.

CTest wrappers, inner GoogleTest cases, pytest node IDs and native JUnit cases
are separate universes. Do not add their counts together. Record package ignore
markers, deselections, disabled/skipped/xfail cases, missing tests, collection
errors, aborted packages and feature/RMW/platform combinations. An empty test
set, successful build, missing SDK, unsupported hardware or unregistered source
candidate cannot become an all-official-suite pass.

The exact Unitree ROS 2 tree includes Foxy/Humble build workflows and package
ament-lint registration; those workflows do not themselves run a functional
robot acceptance suite. Tag/release jobs publish artifacts and are not test
commands. Several files named `test` in SDK2 Python and MuJoCo are print scripts,
paired protocol examples, or continuous torque publishers. They need original
entrypoint-specific finite observers rather than indiscriminate pytest imports.
Unitree SDK2's root CMake selects a prebuilt native library by target architecture;
missing foreign libraries remain provider gaps. The Isaac Lab project requires
its actual simulator, assets, compatible GPU/driver and task adapters. No robot,
GPU, installation, host-network or release-publishing command is authorized by
this source inventory.

## Remaining actual evidence

Root must select and lock the complete requested project/workflow universe,
acquire actual SDK/tool/dependency inputs, then discover and execute each suite
inside the current owned hard-64-GiB, zero-swap guard with finite storage, output,
time and process cleanup. Every unavailable or incomplete scope remains a named
gap. Passing the project's smaller policy/transport regressions does not replace
these official upstream suites; official native tests also do not by themselves
prove Wasm capability/default-denied behavior.

No candidate import, compiler, test, ROS, QEMU, SSH, simulator, GPU or physical
hardware execution was performed while making this inventory. Only primary
source retrieval, JSON/XML data inspection, source reads and hashes were used.
