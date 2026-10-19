# Complete geometry, covariance, odometry and path source families

This additive official-standard family preserves complete native declarations
and emits the existing C, C++, Rust, JavaScript, AssemblyScript and TinyGo value
APIs. Begin with the [raw import reference](../../docs/abi-v1.md) and the
[complete typed SDK introduction](../OFFICIAL_STANDARD.md). A generated value
or registry describes supported data; only explicit trusted Host grants permit
ROS operations. Default authority is empty. Guest imports never contain update,
debugger or policy management functions.

The source implementation, catalogs, native contract and policy cases are
newly authored source. No generator, native exporter, compiler, Guest, ROS
endpoint, QEMU case, performance collection or official test has run for them.
An independently reviewed source receipt is separate from those real runs.

## Exact complete fields and actual versions

The ROS1 catalog contains 28 complete geometry/nav messages plus Header: 29
original definitions, 27 explicitly registered new Topic types. Header and the
existing builtin Twist are dependencies only. ROS1 native time remains real
`ros::Time` with unsigned sec/nsec; it has no fabricated Message MD5. Complete
Pose, Transform, Odometry and Path retain Quaternion, all covariance36 entries,
all Header fields and separate child_frame_id. There is no diagonal projection.

The actual pinned Jazzy Quaternion declares x/y/z defaults0 and w default1.
The complete ROS2 catalog now preserves that original declaration: 28 geometry
and nav messages plus Header and Time, 30 definitions and 27 registered new
Topics. Quaternion-dependent Pose, PoseArray, Transform, Odometry and Path keep
all original fields, covariance36 values and dependency definitions. Header,
Time and the existing builtin Twist remain dependency-only. ROS1 retains its
original no-default Quaternion and its original identity domain.

The first default family was the exact four float64 slots of this official
ROS2 Quaternion. The maintained [scalar-default successor](../scalar-defaults/README.md)
also supports explicitly catalogued ROS2 bool/integer/finite float declarations
and complete Service/Action record graphs. Constants, declared array/string
or nested-object defaults and unreviewed reserved standard variants remain
refused. Native protocol admission requires actual modern full field/reference/
default proof; the separately owned service factory retains its own admission
boundary. Defaults initialize a new value; complete wire decode still requires
every field and never fills missing bytes. Arbitrary finite Quaternion payloads
are preserved without normalization or replacement by default values.
See [the default contract](DEFAULTS.md) for exact APIs and native admission.

Every ordinary dynamic string has canonical128 UTF-8 bytes; polygon points have
canonical64 elements, PoseArray and Path have canonical16. These are explicit
application capacities, separate from the original unbounded native fields.
Oversized input is refused without truncation, partial destination commit or
automatic privilege/buffer growth. The profile retains its complete32768-byte,
4096-node and16-depth ceilings. Other finite capacities require a separately
admitted source catalog and freshly generated fingerprint.

## Immutable source and licensing

`source-provenance.json` records actual immutable official commits, raw file
extents/SHA256 and independently observed Git blob IDs from actual complete Git
tree JSON. Original upstream comments and definitions are byte-exact. A Git
SHA1 tree identifier is not a SHA256 over a materialized whole source tree.
The complete operational source lock and installed SDK identities must still
be acquired by the original preparation owner; none is guessed here.

ROS1 geometry/nav package.xml files declare BSD and are retained byte-exact;
that pinned common_msgs tree has no root/package LICENSE file. The actual ROS2
geometry_msgs/LICENSE and nav_msgs/LICENSE are retained with their original
package declarations. Header/Time dependencies retain their exact immutable
original source URLs and the separately held source acquisition association.
This source family is not a claim that one SDK or source revision represents
all historical or future ROS distributions.

## Generate and bind the actual SDK

Select one actual matching SDK, source revision, ROS major and fresh owned
output root. All qualification commands on the designated Linux host must stay
inside the verified64 GiB, zero-swap cgroup; local test output belongs under
`/tmp`. The commands below are usage shapes, not an execution receipt.

```sh
python3 -B "$UWVM_SOURCE/sdk/tools/custom_types.py" \
  "$UWVM_SOURCE/sdk/custom-types/official-geometry-nav/catalogs/ros2/catalog.preparation.json" \
  --check
python3 -B "$UWVM_SOURCE/sdk/tools/custom_types.py" \
  "$UWVM_SOURCE/sdk/custom-types/official-geometry-nav/catalogs/ros2/catalog.preparation.json" \
  --guest-only --out "$OWNED_OUT/guest-sources"
```

Guest-only generation supplies six source codecs and no native identity or
registry. It cannot authorize an endpoint. First compile/run the actual native
identity exporter against the complete selected installed SDK under its
original process owner. The modern exporter needs real baked hashes and full
TypeDescription; the explicitly selected legacy-introspection exporter needs
full actual native introspection before deriving RIHS, with its separate
provenance. The [vendor preparation pipeline](../../robots/vendors/README.md)
provides that complete administrative source/export/bind flow for a selected
vendor/model. Do not replace its reviewed upstream-lock or native owner inputs
with this transparent unbound fixture inventory.

The direct binder then takes the original actual exporter document and exact
unbound template:

```sh
python3 -B "$UWVM_SOURCE/tests/ros_custom_types/bounded-v2/bind_catalog.py" \
  --template "$UWVM_SOURCE/sdk/custom-types/official-geometry-nav/catalogs/ros2/catalog.preparation.json" \
  --native-metadata "$OWNED_OUT/native-metadata-original.json" \
  --output "$OWNED_OUT/bound-catalog/catalog.json"
python3 -B "$UWVM_SOURCE/sdk/tools/custom_types.py" \
  "$OWNED_OUT/bound-catalog/catalog.json" --out "$OWNED_OUT/native-bound-bundle"
```

Retain the whole actual generated bundle, generation.json, all original source,
export/bind evidence and each staged output extent/hash. Six generated records
use the same APIs described in OFFICIAL_STANDARD.md: C inline caller-owned
values, C++ Codec specialization, Rust Canonical, JS/AS checked Codec and Go
Encode/Decode records. C++ consumes real generated C values and never a native
ROS struct in Wasm. Select real Guest subscription/executor infrastructure from
`robots/vendors/examples/telemetry.*`; the concrete JointState lessons show
complete callback ownership, copied JS-in-Wasm transport and checked closure.
A model-specific fixture must observe all actual selected fields.

## Native contract and unified qualification

Configure `native/CMakeLists.txt` only after actual native-export binding and
same-source generated-product admission. `native_contract.cc` instantiates all
selected28 ROS1 or17 no-default Jazzy family message codecs, checks native identities
and complete metadata, then roundtrips complete values. Meaningful full fixtures
cover all six Twist scalars, all36 covariance entries, stamped Header, every
Point32 polygon coordinate, and ROS1 complete Odometry including child frame,
pose Quaternion and both covariance matrices. Truncated/trailing input and
nested overflow preserve output/destination. These are actual installed-SDK
contract sources, not fake native providers or executed cases.

ROS2 uses real typed introspection; modern support must supply its original
baked hash, while a genuine pre-hash SDK additionally needs actual target OpenSSL
RIHS derivation over its complete matching graph. No invalid modern hash can
downgrade. The separate modern-only native_defaults_contract.cc additionally covers all11
new Quaternion-dependent messages, actual ALL/DEFAULTS_ONLY versus ZERO native
construction, normalized defaults, complete values and negative metadata.
A real complete TypeDescription is mandatory for this ROS2 catalog; a legacy
introspection nullptr is UNKNOWN and cannot attest its declared defaults. All compiler/header/fullMD,
link trace, installed/loaded support libraries, actual ELF/stopped mappings,
whole MF, original PID/cgroup and cleanup evidence must be owned externally.

`test_profile.py` supplies14 injected source-policy cases and remains unexecuted.
The source owner passes its actually admitted parent/bounded modules, this exact
family root and a fresh existing owned scratch directory. Source-policy passes
cannot replace six real language builds, full native publication/Guest receipt,
all26 distribution testing, QEMU architecture coverage, hardware-counter VTune
or each vendor's official full test inventory. Those acceptance obligations and
broader defaults/constants, NaN/Inf/IDL and large-payload support remain required.
