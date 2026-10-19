# NVIDIA native ROS and Wasm integration

This package implements four explicit source lanes: complete CPU-visible Isaac
ROS AprilTag records; a finite Isaac Sim JointState graph and complete Guest
records; a real GR00T N1.7 typed inference service; and read-only Jetson deployment
admission. It is source implemented and **unexecuted**. No generator, compiler,
model, CUDA kernel, ROS exchange, simulator, Guest, or remote actor was run by
this package. Installed SDK closure, actual Native exports, Runtime import
admission, full-MD/link and hardware/software compatibility remain separate
qualification work. This is not complete coverage of every NVIDIA product.

The Source lineage is fixed Git data, not a list of mutable documentation URLs:

| Layer | Original repository / revision | Selected real boundary |
| --- | --- | --- |
| Isaac ROS interfaces | NVIDIA-ISAAC-ROS/isaac_ros_common `7bec6ae5dbb2faa11617f40226e8932a4a94e6e4` | full two AprilTag messages + full standard dependencies |
| Isaac ROS detector | NVIDIA-ISAAC-ROS/isaac_ros_apriltag `95df59e5dc9c253b6e8e11471c10fd510b036fdd` | actual AprilTagNode, CUDA/VPI and launch Source |
| NITROS migration | NVIDIA-ISAAC-ROS/isaac_ros_nitros `260cee67e8344db363f7584256ae161d03abfd01` | actual deprecated README; no old NITROS pointer ABI |
| Isaac Sim | isaac-sim/IsaacSim `87c95274f50836068b44eeb030bf62aa2108bdf8` | actual graph node schemas, implementations and native tests |
| GR00T | NVIDIA/Isaac-GR00T `dac54b313160c201c450f97df0fd74ebd379c892` | actual Gr00tPolicy API and all70 Python Source bodies |
| Standard messages | ros2/common_interfaces `56be9f336d3ed54d18b3821d3b2b183123ec17d8` | complete selected geometry/Header/JointState Source |

Git commit/tree/blob membership is retained in the source package's primary
ledger. The Time dependency is the actual copied fixed614f3c rcl_interfaces
Source already admitted by the control Source lane; it is not a generated
header or Native hash. Jetson/Isaac documentation captures are explicitly
website observations and are not relabelled as Git membership or installed SDKs.
Licenses and complete selected original message bodies are preserved.

## 1. Prepare real complete values

Start with each `source-definitions/ros2/catalog.preparation.json`. They use the
actual `sdk/robots/industrial/vendor_profile.py` complete-v1 Source adapter.
Its original constants/defaults/native scalar distinctions and full modern
TypeDescription proof remain intact. No CONTROL marker, fake Native layout,
fixture RIHS, filtered original source, or width-only compatibility is added.
No Native identity is present in preparation catalogs.

Capture the actual current parent/bounded/adapter Source SHA values. Invoke the
real industrial `prepare.py` into a new directory outside the source tree with
`--mode prepare --source-root ROOT --catalog CATALOG --out BUNDLE` and explicit
`--parent-sha256`, `--bounded-sha256`, `--adapter-sha256`. These are real current
arguments, recorded in `recipes.json`; no producer is invoked here. Build the
actual native interface packages and the supplied strong exporter entry against
the selected SDK. Retain whole-MD/link, loader/stop lifecycle and all exported
root/components/Event/Time/Info/source/default/constant proofs. Then use the
real bind mode with the actual native metadata and actual owner report, and
keep its bound manifest/records/native provider products as one source set.
The actual prepare output is OUT/input plus OUT/generated; pass the latter
complete product directory to the exporter/Guest CMake bundle option.
Native identity strings must come from this independent export. A successful
record generation is not a successful Native bind or ROS exchange.

Each native exporter CMake requires actual generated headers and actual
imported TypeSupport targets. Omitted dependencies or permissive linker modes
must be rejected by the current SelectedNative guard. No stub target or empty
success is supplied. Native constructor/codec/generic identity validity and
actual QoS must be checked before any Gateway-owned endpoint is activated.

## 2. Follow the actual import boundary

Isaac ROS / Sim C++ examples include `roswasm/ros2.h`, whose backend selects real
numeric `uwvm_ros2_v1` imports. A typed Publisher/Subscription uses the complete
generated `Codec<T>`; it never passes a native object address to Wasm. Default
examples compile with `UWVM_NVIDIA_POSITIVE=OFF`, expect actual13, and retain
output sentinels/cleanup. Positive artifacts are separate explicit builds.

The GR00T TinyGo example uses genuine `sdk/tinygo/rmwservice`: raw twelve
`uwvm_ros2_rmw_srv_v1` imports, UMRE256 activation/inspect, UMRC160 + same-take
URMI192, complete private decode before independent sequence+nonce ACK, and
once Native retirement before independent raw-owner closes. It opens explicit
call/observer/query owners; the old Service business ABI is not a fallback.
The exact SERVICE-root canonical identity is an explicit actual bound-proof
input, distinct from Request/Response IDs. UMRE does not authenticate local
strings: independently matching Host policy, whole Native graph and actual QoS
are still necessary. No model path, GPU pointer, CUDA call or native socket is
an import in either Guest example.

## 3. Denied instance, then a separately authorized instance

Keep ROS disabled and pass no grants in the denied Runtime Source. With basic
WASI I/O alone, the requested alias must be denied13 and independently observed
Native effects must stay zero. The actual Runtime CLI for a positive owner is
`--ros2-single-create main`, `--ros2-single-enable main`, explicit connection
options, `--ros2-single-grant main ALIAS OP RESOURCE TYPE ACTUAL_NATIVE_ID`, then
`--ros2-single-set main ALIAS KEY VALUE` for each real policy bound and actual
`adapter-codec-identity`. A matching Gateway policy must independently bind
worker peer credentials/session, selected complete manifest and exact resources.
`authorization.requirements.json` lists required relations; it is an explanatory
source artifact, not a policy parser input and never installs a grant.

Use only a scoped telemetry topic for AprilTag. Sim command and state aliases
are distinct. GR00T call, `event.observe` observer and availability aliases are
three distinct live owners; their full service/native identity/codec/QoS/source
relations must match. Query rights do not imply a probe client or a graph-list
right. A missing optional namespace or feature returns actual95; it is not a
successful empty response. Native owners also default to13 before SDK/model
initialization. Neither a compiler flag nor enabling WASI grants ROS, network,
CUDA, filesystem, clock, debug, lifecycle or policy-management authority.

## 4. Inspect complete typed values and actual effects

For actual C++ Guest artifacts with admitted DWARF, use the real debugger's
`info modules`, `bs MODULE FILE:LINE`, `print /r value` and concrete fields such
as `value.detections.size`, `value.detections.data[0].pose.pose.covariance[35]`,
`value.header.stamp.sec`, and every JointState sequence size/value. Check full
canonical output against independently observed Native typed values, not a
prefix or opaque blob count. Native request metadata is actual ROS writer ID,
sequence and timestamps; application_nonce in GR00T is only application data.

The Go example prints every action key, shape and value, and retains cleanup
facts. It does **not** claim current Go DWARF support. Other language examples
must use the HLL lane's actual closed SDK/record/producer path before claiming
qualification. A C++ debugger demonstration is not another language's proof.
No WSEA gate is added here; any later final-byte signing opt-in follows the
actual LANGUAGE_SDK_DEBUG_SECURITY_INTEGRATION tutorial and separate authority.

## 5. Keep native lifetime and safety domains explicit

Perception and inference do not actuate. The Sim graph affects only the selected
local simulation, and command subscription requires a second explicit flag.
GR00T produces action tensors; connecting them to a robot requires a distinct
trusted policy/lease/controller mapping and is not done here. Native lease
expiry, local abandon and Gateway ticket retirement do not prove physical robot
stop or GPU cancellation. Keep an external bounded whole-process/cgroup owner;
blocking SDK initialization, kernels, Kit updates and native RMW allocations
cannot be preempted by a Python/C++ local timer. Never replay an ambiguous send,
ACK, model inference or native cleanup. Retain primary/cancel/close outcomes
independently. Jetson deployment does not grant device administration to Wasm.

Optimus has no selected public SDK/API in this package and remains unimplemented.

## Source review corrections retained separately

The earlier r1 Source HOLD remains historical. Time and Header stay complete
canonical records and native dependencies, but are not independently registered
topic roots. The examples select AprilTag array and JointState. Additional
explicit catalog topic rows are retained, with no implicit grant. Telemetry retains an error raised by its actual
void callback even when the enclosing SDK poll returns success.

The fixed GR00T SDK contains implicit Python namespace parents without an
`__init__.py`. The captured-source finder derives their exact finite names from
its admitted source keys and gives them an empty search path. It executes no
namespace body, opens no ambient module and admits no unknown `gr00t` name or
bytecode cache. Concrete modules still execute only their captured SHA-checked
bytes. `run_captured.py` explicitly binds the corrected helper digest.

`groot/tests/source_admission_contract.py` is an authored, unrun Source contract.
It takes the actual fixed SDK root/inventory and explicit helper SHA, checks the
three genuine implicit parents, rejects an unknown neighbor, and checks the
selected registration/callback source facts. It does not import GR00T modules,
load a model, instantiate ROS, or qualify a compiler/Native/Guest execution.
