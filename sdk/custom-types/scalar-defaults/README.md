# Complete scalar defaults in original ROS2 declarations

This source cohort preserves declared bool, signed/unsigned8/16/32/64 integer
and finite float32/float64 defaults in complete Message, Service and Action
components. It adds no import, permission, endpoint, clock, network operation,
management API or default grant. No candidate generator, parser, compiler,
contract, native exporter, Wasm or ROS peer has been executed for this source.
Source review and integration must precede the unified actual test phase.

Start with the [raw import reference](../../docs/abi-v1.md), then the
[bounded-value SDK](../README.md) and actual separately selected capability
aliases. A complete record/default initializer is a value operation. It cannot
start a robot, discover a service, enable a debugger, install a module or grant
any capability. The Guest never receives native management/update authority.

## Original declaration and native proof

The source parser accepts actual ROS2 scalar default syntax, validates each
original scalar width, and never evaluates source text as Python or another
language. Decimal and explicit0x/0o/0b integers normalize exactly. Boolean
true/false/0/1 (case-insensitive) normalize to the original TypeDescription
annotation spelling True/False. Finite decimal float literals normalize through
the original adapter/IDL-parser value representation; nonfinite values and
float32 overflow are refused. Signed zero and genuine float32 underflow/rounding
are retained. These choices follow the retained original rosidl sources.

RIHS01 excludes defaults. A nonzero/equal native hash therefore cannot attest
the original default. The binding tool and injected vendor planner require the
complete independently exported modern TypeDescription, including every field,
ordered member, native capacity, normalized default and reachable dependency.
Services include Request/Response/root/Event/ServiceEventInfo/Time; Actions also
include Goal/Result/Feedback, both protocol Services and Events, UUID and wrapped
feedback. All real root/component identity values are copied from the actual
export, never computed or filled with fixture values by this code. Hash/source
qualification and native execution remain separate required observations.

The two pure DATA proof functions in the binder/planner must remain identical.
The previous Quaternion-only receipt is retained for that exact standard Message
family. Other no-default catalogs retain their previous receipt/identity domain.
Canonical IR adds a default key only for a declared default. The original
standard Quaternion x/y/z0,w1 shape remains strict even inside a new Service or
Action. Source defaults are neither erased nor replaced by zero aggregates.

The modern Native comparison checks the complete actual counted default text.
Message graphs retain the existing128-record bound; complete Service/Action
protocol graphs use their independent finite256-record bound. Every actual
root and reachable reference is matched, rather than only a root name/hash.
The legacy introspection path still refuses declared defaults: a null original
introspection default pointer is unknown, and pre-RIHS schema derivation cannot
prove default values. Missing modern APIs do not select a silent legacy fallback.

## Value initialization in each supported language

C exposes the existing checked `_default(T*)` initializer; plain zero aggregates
stay zero and this function commits one complete privately initialized value.
C++ exposes its existing `Codec<T>::default_value()` over the same C-value shape.
Rust uses `Default`; JavaScript uses `Codec.defaultValue()`; AssemblyScript
initializes new records; TinyGo uses `DefaultType()` while ordinary Go zero values
remain ordinary zero. Optional Python, Java and C# value emitters consume the
same resolved IR. JavaScript records also serve the selected QuickJS bridge;
this source does not add a QuickJS runtime or claim a MoonBit default emitter.

Each target emits the correct scalar representation. C preserves signed64 MIN
without an overflowing positive literal. JS uses BigInt for both64-bit integer
types. Java uint64 retains every bit in its documented long representation,
including bit63/MAX. Float initialization uses exact typed values/bits to retain
float32 rounding, signed zero and subnormal/underflow behavior without relying
on Java/C# accepting an underflowing decimal token. Python receives the rounded
native float32 value, not an unrounded Python64 approximation of its source.

Defaults apply to a newly initialized value. Encode still accepts every valid
nondefault value, and complete decode requires every byte/field and overwrites
every field. It never repairs a missing/truncated payload from defaults. Nested
message and fixed-array elements use their existing full child initializers;
variable arrays start empty. Array, string and nested **declared field defaults**
need their own complete grammar/emission/native proof and remain explicit work.
Constants, aliases, wide strings and recursive schemas remain explicit work too.
This finite scalar cohort is not full vendor, RMW or old rcl compatibility.

## Complete original xArm example

The [preparation catalog](catalog/catalog.preparation.json) copies the complete
original GetSetModbusData.srv, including original comments and all fields:
modbus_data, modbus_length default0, ret_length, host_id default9, both boolean
defaultfalse flags, and the complete ret/message/ret_data Response. The canonical
application capacities64/128/64 are explicit catalog values; they do not rewrite
the original unbounded native declarations or imply hardware authority.
Original source/provenance and licensing remain under
[the xArm SDK](../../robots/xarm/README.md). Select a matching actual upstream
SDK/source/export before binding. No native identity is present in this unbound
catalog. The full vendor factory/transport must separately admit the complete
service and its real identities before an endpoint can exist.

After complete source implementation, use the administrative preparation tools
with owned output roots. Local builds/tests/output must be under `/tmp`. Remote
runs must stay in the freshly verified exact64GiB/swap0 cgroup. These commands
are usage forms, not receipts or executed steps:

```sh
python3 -B "$SOURCE/sdk/tools/custom_types.py" \
  "$SOURCE/sdk/custom-types/scalar-defaults/catalog/catalog.preparation.json" \
  --guest-only --out "$OWNED_TMP/guest-values"
python3 -B "$SOURCE/tests/ros_custom_types/bounded-v2/bind_catalog.py" \
  --template "$SOURCE/sdk/custom-types/scalar-defaults/catalog/catalog.preparation.json" \
  --native-metadata "$ACTUAL_EXPORT" --output "$OWNED_TMP/bound/catalog.json"
python3 -B "$SOURCE/sdk/tools/custom_types.py" "$OWNED_TMP/bound/catalog.json" \
  --out "$OWNED_TMP/complete-values"
```

The generated C/C++/Rust/JS/AS/TinyGo records are the complete value SDK for this
exact catalog. Optional Python/Java/C# generation uses the existing explicit
managed-profile tool. Do not copy a mock identity/default profile into native
execution or treat source generation as an SDK/source/API qualification.

## Authored contracts and unified acceptance

`test_scalar_defaults.py --source-root ABSOLUTE_SOURCE` has eight authored
regression groups with width extrema, all target initializer forms, signed zero,
float32 underflow/overflow, malformed defaults, ROS1 refusal, original xArm
Request/Response fields, no-default IR and whole modern Service metadata mutation
cases. Its constructed dictionaries are DATA fixtures, never native exports.
The actual C++ SDK contract uses original generated xArm objects and freshly
bound generated converters, compares original default initialization with C
records and full wire bytes, then checks nondefault values and truncated/trailing
input output preservation. Its CMake project is explicit and builds only; it
never launches an actor or automatically executes the contract.

Still required: independent complete source peer; coherent generator/SDK build;
all9 language value/roundtrip comparisons and old no-default output checks;
selected actual TypeDescription/identity export; actual original xArm contract;
real Native/Guest service traffic and fault/retirement cases; all upstream and
vendor suites, distributions, QEMU architectures, simulation and native/VTune
hardware performance qualification. None is marked passed by this source.

The maintained Quaternion regression source now follows the same complete
contract: its15 policy methods retain exact official values and strict wire
behavior, assert exact typed C/Rust/AS initialization, and require full Service/
Action source protocol closure instead of the retired blanket refusal. Native
generation still requires actual exported identities and modern descriptions.
Its six separately injected real-native DATA methods remain unchanged. The
shared managed and geometry guidance link this successor explicitly; historical
cohort receipts retain their original source and execution scope. These edits
are authored validation sources and have not been executed.
