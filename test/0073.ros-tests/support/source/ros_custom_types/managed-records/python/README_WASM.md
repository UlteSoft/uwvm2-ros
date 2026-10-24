# Complete Python record tests in a real WASI interpreter

This application runs the generated Python records and the actual SDK codecs
inside CPython 3.14.8 compiled to WASI Preview 1. It tests complete scalar wire
values, exact integer widths, signed zero, strict UTF-8, bounds, framing, nested
records, independent defaults, service/action components and complete ROS1/ROS2
`sensor_msgs/JointState` values. Its pure codec entry creates no ROS Backend,
loads no `_roswasm_native` extension and receives no native middleware authority.

The [actual Linux qualification](../../../../../../0050.high_level_languages/results/native-budget/language-python-records-jointstate-20261019/README.md)
records each producer, source, frozen bytecode, linked object, final Wasm and
Runtime identity. A native Python test, source-generation receipt or retained
DWARF section cannot substitute for execution of the actual linked module.

## Select the producer and build the pure application

Run the following on Linux with an existing, verified CPython **3.14.8** source
and WASI target build and its matching native build-Python. A host Python of a
different version cannot produce this target's frozen bytecode. The qualified
producer used WASI SDK 24.0 and the existing target `libpython3.14.a`; it did not
rebuild the interpreter separately for each small entry optimization profile.

Set absolute paths for the selected checkout, a new empty work directory, the
producer source tree, the configured target build, its native build-Python and
the physical WASI SDK directory:

```sh
export SOURCE=/absolute/path/to/uwvm2-ros
export WORK=/tmp/my-python-record-contract
export CPYTHON_SOURCE=/absolute/path/to/Python-3.14.8
export CPYTHON_TARGET=/absolute/path/to/cross-build/wasm32-wasip1
export BUILD_PYTHON=/absolute/path/to/cross-build/x86_64-pc-linux-gnu/python
export WASI_SDK=/absolute/path/to/wasi-sdk-24.0-x86_64-linux
mkdir "$WORK"
```

Use a separate work directory for each build. The script below reads the actual
target Makefile through the data-only recipe reader, preserves its library
order and generated linker flags, freezes real source modules and links the
original entry object. It does not evaluate Makefile commands or create an old
SDK-path alias. `OPT` may be `O0` or `O2`; native interpreter/archive optimization
remains whatever the selected producer actually built.

```sh
export OPT=O2
python3 -I -B - <<'PY'
import json, os, pathlib, shlex, shutil, subprocess
P = pathlib.Path
source, work = P(os.environ['SOURCE']), P(os.environ['WORK'])
producer, target = P(os.environ['CPYTHON_SOURCE']), P(os.environ['CPYTHON_TARGET'])
native, wasi = P(os.environ['BUILD_PYTHON']), P(os.environ['WASI_SDK'])
opt = os.environ.get('OPT', 'O2')
if opt not in ('O0', 'O2'):
    raise ValueError('select O0 or O2')
lesson = source / 'test/0073.ros-tests/support/source/ros_custom_types/managed-records/python'
lib = work / 'Lib'
shutil.copytree(producer / 'Lib', lib, symlinks=False)
for role, catalog, filename in (
    ('fixture', 'test/0073.ros-tests/support/source/ros_custom_types/managed-records/fixture/catalog.preparation.json', 'bounded_records.py'),
    ('joint1', 'test/0073.ros-tests/support/source/ros_custom_types/official-standard/ros1/catalog.preparation.json', 'joint_ros1_records.py'),
    ('joint2', 'test/0073.ros-tests/support/source/ros_custom_types/official-standard/ros2/catalog.preparation.json', 'joint_ros2_records.py'),
):
    dest = work / role
    subprocess.run(['python3', '-I', '-B', str(source / 'sdk/origin/custom-types/managed_profile.py'),
                    str(source / catalog), '--out', str(dest), '--language', 'python'], check=True)
    shutil.copyfile(dest / 'bounded_records.py', lib / filename)
frozen = work / 'frozen'
subprocess.run([str(native), '-I', '-S', '-B', str(source / 'sdk/origin/python/tools/freeze.py'),
                '--stdlib', str(lib), '--sdk-python', str(source / 'sdk/origin/python'),
                '--entry', str(lesson / 'RecordContract.py'), '--out', str(frozen)], check=True)
recipe = json.loads(subprocess.check_output([
    str(native), '-I', '-S', '-B', str(lesson / 'link_recipe.py'),
    '--makefile', str(target / 'Makefile')]))
v = recipe['variables']
clang = wasi / 'bin/clang'
common = [str(clang), '--target=wasm32-wasip1',
          '--sysroot=' + str(wasi / 'share/wasi-sysroot'), '-g', '-' + opt]
obj = work / 'entry.o'
subprocess.run(common + ['-std=c11', '-Werror', '-MD', '-MF', str(work / 'entry.d'),
    '-DUWVM_DEBUG_LANGUAGE_PYTHON=0', '-I' + str(target),
    '-I' + str(producer / 'Include'), '-I' + str(frozen),
    '-c', str(lesson / 'record_main.c'), '-o', str(obj)], check=True)
libraries = [str((target / x).resolve(strict=True)) if x.endswith('.a') and not P(x).is_absolute()
             else x for x in recipe['library_arguments']]
subprocess.run(common + shlex.split(v['PY_CORE_LDFLAGS']) + [
    '-Wl,--trace', '-Wl,-Map,' + str(work / 'link.map'), str(obj),
    str(target / 'libpython3.14.a'), *libraries, '-o', str(work / 'records.wasm')], check=True)
PY
```

The configured compiler path in an old Makefile may no longer exist after a
producer is moved. Select the physical SDK only after checking it against the
original compiler SHA and build receipt. Do not silently use an unrelated Clang
or replace a missing archive with a host library. The qualification preserves
that explicit relocation and the original Makefile.

Validate and execute with the selected Runtime; enable additional Wasm features
only if the actual module needs them:

```sh
wasm-tools validate "$WORK/records.wasm"
"$UWVM" --wasip1-global-noinherit-system-environment \
  --wasm-set-main-module-name python_records --run "$WORK/records.wasm"
```

Expected complete stdout is:

```text
managed Python record cases=17 checks=81 native/ROS=false host-execution=false
```

The test's assertions must run in the Guest. Corrupting the first independent
scalar golden is a negative control: the qualified build exits with status 1,
an actual `complete_scalar_golden` traceback and no success marker.

## Build a ROS application separately

Use the [managed JointState tutorial](../../../../../../../sdk/origin/robots/vendors/examples/joint_state/README_MANAGED.md)
for the complete real application, generation choices and default/positive
modes. A ROS build uses `sdk/origin/python/embedding/main.c`, the actual selected-backend
`sdk/origin/python/roswasm/_native.c` object and the target interpreter library. Register
the extension before startup and link that same bridge object exactly once.
Check every final numeric ROS import against `sdk/origin/abi/abi-v1.json` with
`sdk/origin/tools/check_wasm_imports.py`, then validate the whole module.

The recorded ROS1/ROS2 builds exercise both O0/O2 entry/bridge profiles and
retained-DWARF/custom-section-free variants. Default mode must print
`PASS_JOINT_STATE_DEFAULT_DENIED`. Positive mode builds are retained, but do not
have native JointState delivery credit in this cohort. Native subscriber
observations, complete values, exact permission decisions and endpoint cleanup
need a separate real Gateway/ROS run.

## Debug and performance scope

Native CPython C DWARF, Python trace callbacks and Python source-frame debugging
are different facilities. These codec builds disable Guest snapshot probing;
retaining their `.debug_info`/`.debug_line` does not certify Python locals.
Follow the [Python frame-probe build guide](../../../../../../../sdk/origin/python/debug/BUILD.md)
for that additional producer route and use actual debugger transcripts as proof.
The [language option guide](../../../../../../../docs/debugger/language-options.md)
explains the fifteen default-on host switches. Disabling an adapter does not
forbid ordinary Wasm execution; keeping all adapters enabled is recommended.

Functional run durations are not native/Wasm speed measurements. Compare the
same checked workload and explicit runtime versions, separate startup/JIT from
the measured kernel and preserve VTune hardware samples and Hotspots /
Microarchitecture reports. A codec pass alone establishes no performance ratio.
