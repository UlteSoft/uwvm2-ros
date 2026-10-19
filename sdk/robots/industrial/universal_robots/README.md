# Universal Robots: selected ROS1, ROS2 and native PolyScope X interfaces

This kit preserves every original custom message, service and action from the
selected official driver/message packages and all reached original standard
definitions. The generated C11, C++20, Rust, JavaScript, AssemblyScript and
TinyGo SDKs retain complete fields, constants, declared defaults and finite
canonical capacities. Source availability does not qualify every UR release,
controller, model, native installation or original vendor test suite.

| Source selection | Immutable commit | Original definitions / expanded records |
| --- | --- | ---: |
| ROS1 Noetic ur_msgs plus official ROS1 dashboard | ur_msgs: 6c6dc66c63bd551e3985281739add762bfaa3aa9; driver: bd14b5c533135886f4f85ef0cabd097a5e1b8d0f | 36 / 56 |
| ROS2 Humble ur_msgs plus official ROS2 dashboard | ur_msgs: 3c584014d9b228e0b2da819db0ca1d887c373e1d; driver: 262330ed9b5467a17c8ceae09567991e22d13944 | 67 / 108 |
| Official urinterfaces, PolyScope X source selection | e88cebb16503bcbe6e7385251e1f83b1fa54e9c8 | 43 / 50 |

ROS1 includes 28 custom interface roots, ROS2 includes 53, and PolyScope X
includes 35. The catalogs and source locks retain unchanged definition bytes,
Git commit/tree association, Git blob SHA1 and independent SHA256. The
upstream directory retains licenses, original interface files and selected
driver implementation/documents. Complete acquisition archives remain in the
separately retained execution evidence. Reused standard sources keep their
original pinned provenance; they are not reported as fresh downloads.

The available official branch inventories are retained. A branch name or similar
message spelling is not a compatibility result. Other ROS releases and
controller-specific behavior need independent source comparison and acceptance.
In particular, urinterfaces is a separate native PolyScope X message family,
not a rename of ur_msgs or an automatically interchangeable driver ABI.

## Begin at the actual imports

Read sdk/include/roswasm/abi/ros1.h or ros2.h and the corresponding controlled
C backend. Run the raw C example with explicit argv OPERATION ALIAS. With default
ROS rights it returns13 at the capability open. There is no implicit ROS graph
discovery, native socket, RTDE/dashboard TCP connection, robot credential or
physical operation. Basic stdin/stdout/stderr are separate from ROS authority.

Every positive operator must independently select the exact module, backend,
policy alias, role, resource, complete source/native/canonical identities, QoS,
resource bounds and finite quota. The generated Source package cannot create a
Native TypeIdentity, installed ROS package, registry or grant. The Runtime and
separately compiled native Gateway need the same coherent bound domain.

The SDK exposes no updater, debugger, credential management, native file/network
management or Runtime policy mutation. Granting a broad robot service such as
RawRequest or SendScript admits its command payload semantics; a bounded string
is not an application command allowlist. Keep these operations ungranted until
the native operator has explicitly reviewed the required robot-side scope.
No example automatically resets a safety mode, resumes a program or retries a
command whose physical outcome is uncertain.

## Whole records, then checked owners

Select exactly one generated family:

* universal-robots-ros1-complete uses the original ROS1 ur_msgs and dashboard.
* universal-robots-ros2-complete uses the selected original ROS2 ur_msgs/dashboard.
* universal-robots-polyscopex-complete uses the selected original urinterfaces.

Use bounded_records.h/.hpp/.rs/.mjs/.ts/.go for complete values and the matching
bounded_interfaces files for the original service/action facades. Each generated
directory belongs in sdk/custom-types so its relative SDK imports remain valid.
Do not concatenate independent generated domains into one native registry.
Merge compatible complete declarations and regenerate a single domain first.

include/io.h starts from the actual C import boundary and supplies whole-value
I/O polling and a pure speed-fraction request constructor. Select exactly one
UWVM_UR_ROS1, UWVM_UR_ROS2 or UWVM_UR_POLYSCOPEX macro with its corresponding
bundle. The fixed 1 KiB scratch is caller-owned and must not overlap a typed
output. One serialized poll either decodes the entire value or preserves the
destination. Idle11 is not delivery. Oversize/refusal cannot grow buffers or
replay a consumed event.

include/io.hpp adds the real typed Reader and includes all generated C++
service/action facades. Construction opens no resource and retains a borrowed
explicit owner. No original native rclcpp/UR object pointer enters Guest memory.
The pure speed constructor accepts only finite fractions in [0,1] and preserves
the entire output on refusal. It constructs the original request field and
width; it sends no robot command and does not assert a physical stopping effect.

The examples preserve primary and cleanup outcomes separately. Application exit
status prefers a primary failure, then cleanup failure. A decoded value remains
a complete observation even if closing its owner later fails. Closing a
publisher, action owner or service client is not physical stop, cancellation
acknowledgement or rollback. Native watchdogs and uncertain-outcome handling
remain independent requirements.

All three families retain different original field names and types. For example
ROS1 MasterboardDataMsg.robot_voltage_48V and ROS2 robot_voltage_48v are not
silently renamed. Legacy RobotModeDataMsg explicitly represents only the subset
declared by that upstream message; this complete ROS message SDK is not a
complete native realtime-protocol parser.

## Explicit default-off CMake and xmake selection

All SDK CMake lanes default OFF. To build one source selection:

    cmake -S sdk/robots/industrial/universal_robots -B /absolute/ur-native-ros2 \
      -DCMAKE_BUILD_TYPE=Release -DUWVM_UR_VARIANT=ros2 \
      -DUWVM_UR_BUILD_CONTRACTS=ON \
      -DUWVM_UR_SOURCE_ROOT=/absolute/uwvm2-ros \
      -DUWVM_UR_GENERATED=/absolute/uwvm2-ros/sdk/custom-types/universal-robots-ros2-complete
    cmake --build /absolute/ur-native-ros2 --parallel 1

Use ros1 or polyscopex with its matching bundle in a fresh build directory.
Configuration checks the exact original catalog and every generated product.
A different family refuses before an SDK executable is built. This is Source
integrity checking, not native SDK acceptance.

For genuine WASI Preview1 commands add the real SDK toolchain and enable the
Guest lane:

    -DCMAKE_TOOLCHAIN_FILE=/absolute/wasi-sdk/share/cmake/wasi-sdk-p1.cmake
    -DUWVM_UR_BUILD_WASI_EXAMPLES=ON
    -DCMAKE_EXE_LINKER_FLAGS='-Wl,--gc-sections,--initial-memory=8388608,--max-memory=33554432,-z,stack-size=1048576'

C++ uses its real no-exception profile and LLD is limited to one worker.
ur_guest_c.wasm and ur_guest_cpp.wasm take raw|io ALIAS. raw only opens/closes
the explicit owner; io subscribes, polls once and writes a full canonical value
to stdout on success. There is no partial/status-only substitution.

The Runtime ros-vendor-universal-robots xmake option remains default-off.
Select ROS1/ROS2 independently and supply the actual reviewed native-bound
registry. For A+B use sdk/tools/ros_build_config.py with both --vendor values:
universal-robots and the other selected vendor. The printer explicitly disables
all unselected vendors/platforms. Both selected vendors must name the same
coherent bound registry. Shipping generated SDK source does not add all vendor
code to uwvm2-ros.

## Rust, JavaScript, AssemblyScript and TinyGo commands

examples/rust is a complete local Cargo crate. Select exactly one feature:
ros1, ros2 or polyscopex. Build the actual wasm32-wasip1 target offline with
one job. main.rs takes one explicit alias. read_once returns the complete I/O
record, primary and cleanup; it never calls a native UR Rust binding.

The JavaScript telemetry modules take a genuine copied Guest Session for the
matching ROS major. The closed examples/javascript-VARIANT.json recipe selects
one complete module graph and the actual QuickJS-in-Wasm entry. Use variant
selection when preparing the producer plan, then execute every compiler and
final-link job. A host Node model is not a Wasm test. Counted stdin is a nonzero
u8 UTF-8 alias length followed by exactly that many bytes. Stdout contains two
u32LE values: primary and cleanup. Engine exit0 only means the script ran;
primary13 remains a refused ROS open.

AssemblyScript uses main_VARIANT.ts with the matching genuine NativeApi and
whole I/O Codec. Enable the actual SDK abortGuest binding and a finite
incremental Wasm runtime. The counted-alias stdin format matches JavaScript.
A nonzero result never publishes a partial default object as received data.

examples/go is a complete TinyGo module. Select ros1,urros1 or ros2,urros2 or
ros2,polyscopex tags. These are three exclusive source choices. Each original
generated Go product is retained unchanged in a separate package; one selection
never imports the other two packages. Use the genuine wasi target, one compiler
job, an explicit scheduler/GC profile and one argv alias.

## Source defaults and test scope

The original ROS2 SetForceMode request declares six float32 deviation defaults.
The producer retains every original value and emits each target's actual
fixed-array initialization. Ordinary C zero initialization remains zero data;
call the explicit default factory to obtain declared defaults. Scalars, strings
and nested defaults retain their existing behavior. Fixed floating arrays are
finite, length-exact and bounded to128 elements; unsupported sequence/nested
default syntax refuses before publication. No source expression is evaluated.

Tests include complete C/C++ records, truncated/trailing data, transactional
refusal and pure speed boundaries. The separate array-default contract executes
all six actual Wasm language producers and independently compares all40 bytes,
including negative zero, a float32 subnormal and a tiny float64. These are
authored SDK contracts. They do not replace original vendor suites, prove
positive native ROS traffic, validate a physical controller, or establish
equivalent-work VTune hardware performance.

Recorded actual results, exact binary sizes, cgroup/source/compiler pins and
retention locations must accompany delivery. Unrun native/robot/simulator and
other-release cases remain unqualified; no successful generation receipt is
rewritten into execution credit.
