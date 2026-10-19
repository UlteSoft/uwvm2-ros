# Complete Spot 5.0.1 protobuf field SDK for C and C++

This source package supplies genuine generated field APIs for every production
protobuf message in the fixed Boston Dynamics Spot SDK5.0.1 tree. It does not
replace the Native SDK, expose a robot socket, store credentials in a Guest, or
grant a ROS resource. The Native operator bridge remains the only component
that calls the Boston Dynamics Python SDK. Its nine fixed operations retain
independent allowlist entries, finite native SDK deadlines, nonreusing token/
nonce history and uncertain-outcome handling.

The earlier 2048-byte milestone is preserved unchanged. This separate successor
uses a complete protobuf bound of16384 bytes on both Native serialization and
the authored State/Feedback ROS declarations. Each canonical response has an
explicit32768-byte aggregate bound. The State projection and original protobuf
bytes both remain present. Oversize is90, never a prefix or a reduced schema.
The separately reviewed complete-vendor byte-sequence Source operator produced
the actual new bridge record headers; the historical parent/bounded generators
were not edited. Source generation is not a Native export, RIHS attestation,
robot test, compiler result, Guest execution or performance result.

## Fixed originals and actual products

The captured Spot tree contains149 production SDK proto files and two separate
official self-registration example proto files. All151 original Spot files and
all six imported original Google well-known-type files are retained byte for
byte. The155 production inputs (149+6) were passed to the real official protoc
3.21.12 ARM64 executable and the fixed official nanopb0.4.9.1 plugin.

The actual generated products contain155 C++ Lite implementation/header pairs,
155 nanopb C implementation/header pairs,1319 message descriptions and4409
original fields. No field is manually projected out of the generated SDK.
The complete actual generator output, including descriptor set, metadata,
external options and Python tool bootstrap, is durably archived separately;
the checked-in generated C/C++ source is matched to that actual output by SHA256.

The original source-only producer emitted five C keyword member names. The
2026-10-19 Linux qualification found this at the first genuine full C compile.
The maintained producer now extends the fixed official nanopb generator's real
`NamingStyle.var_name` strategy. It escapes `bool`, `const`, `double`, `int` and
`switch` with collision-checked underscore suffixes. `c-identifiers.json` and
every `field-map.json` entry expose that exact C spelling, including oneof
membership. Original proto names, descriptors, wire tags and all 310 C++ output
files remain byte-identical. The separately retained Linux regeneration receipt
binds the changed C products and producer, rather than rewriting the historical
ARM64 source-generation receipt.

The Guest codec runtime is independently fixed at protobuf v21.12, commit
f0dc78d7e6e331b8c6bb2d5283e06aa26883ca7c. This does not upgrade the Native Spot
SDK5.0.1 or relabel its historical protobuf3.19.4 dependency. Wire compatibility
of real robot traffic remains an actual future qualification requirement.
The real tool had ARM64 Mach-O CPU0x100000c and reported libprotoc3.21.12.
Its published SHA1 and locally measured SHA256 are recorded with its acquisition.

## Full field access

C consumers include robot_state.h and the generated c/ headers. After
uwvm_spot_robot_state_decode succeeds, uwvm_spot_robot_state_value returns a
borrowed const bosdyn_api_RobotState. The entire original power, battery,
communications, fault, E-Stop, kinematic/frame-tree, foot/terrain, manipulator,
service-fault, system-temperature and behavior field graph is accessible through
its generated members. All other original message types are available through
uwvm_spot_proto_type_named and uwvm_spot_proto_decode.

C++ consumers include document.hpp and the generated cpp/ headers. RobotState
owns an actual bosdyn::api::RobotState generated in Lite mode. get() exposes the
complete const generated class, including genuine maps, repeated fields, oneof
accessors, enum values, presence and nested types. Document<T> works for another
actual generated MessageLite type selected by its original full name.

examples/inspect_state.c and .cc read fields outside the old joint projection:
battery voltage/temperature, contact/terrain, frame-tree entries, fault lists,
arm state and motor power. These summaries are teaching examples; callers can
access every remaining generated field directly. examples/guest_state.c and .cc
are actual ROS Guest entry sources. They use the typed existing Spot State
request/response and decode its native_proto only after transport success,
matching token/nonce, provider error0 and unknown=false. They do not issue a
second State request or retry a failed/uncertain result.

`examples/guest_state_banked.cc` is a C++20 application using the real bounded C
decoder through the maintained C++ Spot client. It builds with ordinary SDK33
WASI Preview1 and `-fno-exceptions`; it does not claim the protobuf Lite generated
C++ class API has acquired single-thread WASI support. Select its explicit State
alias and supply the same 24-byte stdin input. It performs no lease or motion
operation and retains transport, provider, decode and cleanup results.

## C representation and bounded ownership

Original proto3 strings are length-delimited UTF-8, and may contain U+0000.
Nanopb's ordinary char* mapping cannot retain the length after an embedded NUL.
The official generator's external field option therefore maps every original
STRING field to pointer BYTES. A generated original-descriptor field map retains
the original semantic type, and preflight validates strict scalar UTF-8 while
allowing U+0000. Inspect size and bytes; do not call strlen on such a field.

Original ENUM fields map to pointer INT32 so valid unknown negative proto3 enum
values remain representable. The original generated enum declarations/constants
remain available. field-map.json lists every exact mapping. Neither mapping
changes the original proto files or the protobuf field tag/wire type.
All fields use the official pointer allocation mode, with no count/string
truncation or hidden callbacks. Repeated map-entry records remain available in
wire order in C; duplicate map keys retain those entries, whereas the official
C++ map applies its ordinary last-key semantics. Unknown fields remain in the
exact original wire snapshot; nanopb does not itself preserve them in its typed
object. The SDK intentionally offers that original snapshot for lossless relay,
rather than claiming typed re-encoding preserves unknown data.

The caller provides two disjoint max_align_t-aligned banks,4096..1048576 bytes
each, before opening any ROS owner. The decoder uses no ambient malloc: every
actual nanopb realloc/free is scoped to a lexical thread-local bank allocator.
Repeated-array growth consumes that finite bank; it may return90 before the
wire maximum. This is a real storage limit, not silent sequence shortening.
Root objects and their full contents live in one bank. A successful decode
replaces the prior value; any malformed wire22, wire/depth/storage quota90, or
same-thread reentry17 preserves the prior value and wire. Clear invalidates all
borrows and logically releases both banks; it does not promise secure erasure.
Do not copy a live document, mutate a returned const object or share a document
between threads. Separate documents may be owned by separate admitted threads.

The descriptor-driven iterative preflight bounds original message nesting to32
frames, total wire work to32768 field records and wire bytes to16384 before
nanopb recursive decode. It validates known field wire types and original string
UTF-8. Unknown ordinary varint/fixed/length-delimited fields are skipped without
inventing a message schema, and the complete raw bytes are retained. Deprecated
proto2 group wire kinds are explicitly refused22; none of these155 original
production inputs declares groups. Truncated tags/lengths, varint overflow and
invalid UTF-8 fail22. Native protobuf NaN/Inf values are not arbitrarily erased.

## C++ ownership and actual platform requirements

The Lite parser first receives the same descriptor-driven finite wire check,
then uses actual CodedInputStream total-byte and recursion limits. An entire
private generated object is parsed before a unique-ownership swap. Ordinary
std::bad_alloc returns90 and preserves the prior object. The caller sees no
partial tree and no output is committed before full parsing completes.

C++ Arena block hooks do not bound std::string backing allocations. This SDK
does not claim a per-document hard C++ heap cap. A real admitted Guest linear
memory/stack limit remains mandatory; allocator abort or a Runtime trap is a
terminal Guest outcome, not fabricated errno success. Use the C bank API when
an explicit per-document storage budget is required.

The fixed original Lite runtime uses real std::mutex and std::call_once. Its
WASI C++ lane therefore requires the actual reviewed SDK33 threaded-EH toolchain,
genuine pthread libc++, admitted shared memory/TLS/thread budgets and the actual
Runtime threaded import closure. There are no no-op mutex or exception stubs.
The ordinary legacy single-thread SDK24 lane is not declared sufficient. The C
lane requires genuine C11 thread-local support and uniform forced nanopb config;
a final executable link and actual Wasm import admission are still required.

## Authority, lifetime and uncertain outcomes

Payload token/nonce values prevent replay in the shared Native operator profile;
they are not Runtime Instance identities. The bridge does not promise per-Guest
lease/feedback isolation without an actual trusted principal channel. Runtime
per-resource service.call authorization and the Native network/operator
allowlist are separate layers. The Native operator must control ROS discovery,
network access and the bridge configuration.

The telemetry Guests own only the capability they explicitly open and attempt
its real close independently after all primary outcomes. A decoded RobotState
E-Stop field reports robot telemetry and is not an E-Stop capability or a proof
of physical safety. Stop/lease-return examples remain separately authorized
operations from the predecessor SDK; parsing a protobuf never adds authority.

The [r36 retained qualification](../../../../../documents/ros/continuation-20261018-evidence/spot-full-field-sdk-and-wasm-r36/README.md)
records actual complete native C/C++ compilation and contracts, four validated
Wasm modules, the full C contract in the frozen r35 interpreter, three
default-denied business applications, and complete C contract runs on user QEMU
x86_64/i686/AArch64/ARM hard-float. All 1,319 C bindings receive a valid empty
proto3 message and an invalid reserved field-zero tag; populated edge cases are
separate tests, not all possible field combinations or the official Spot suite.
Installed ROS SDK/full metadata, positive Gateway/robot interoperability, the
original threaded/EH C++ Lite Wasm lane, other language execution, debugger
inspection and matched performance measurements remain required acceptance.

The existing convenience projection also retains its exact32-joint/128-byte name
and finite-number canonical limits. A whole State value outside those authored
ROS bounds is refused; the bridge never truncates projection members to obtain
a protobuf success. Raw protobuf STRING NUL support in the field decoder does
not expand the existing ROS canonical string domain. A joint-name NUL in that
projection is therefore still refused by the canonical layer, even though the
standalone complete protobuf decoder correctly represents such a field. This
is an explicit bridge-domain limit, not a claim that every vendor value fits.

## Pointer-message merge correction

The unchanged nanopb0.4.9.1 decoder releases an existing pointer submessage when
the same singular field occurs again on wire. This is insufficient for protobuf
merge semantics. The C target therefore compiles the separately attributed
src/nanopb_decode_merged.c derived from that exact official Source. It reuses
existing singular message objects and invokes the original decoder with its
real PB_DECODE_NOINIT flag. Repeated fields still append, and changing a oneof
member still follows the original release/replace path. The original upstream
file remains unchanged. Genuine same-message/same-oneof/different-oneof and
failed-merge C/C++ contracts are authored, not executed. No generated field or
Source is erased to obtain compatibility.

The Source successor also keeps actual FJT owner invocation, returned error
status and complete typed decode inside one BaseException boundary. An invoke
exception can follow Native consumption; it marks the existing row unknown
and cannot authorize another receive. Only a genuinely returned idle11 retains
the row. No cancellation, replay or physical-stop receipt is invented.

The C contract's symbol-resolution gate separately inspects actual C-driver
flags and configuration variants in addition to the shared target-closure/linker gate.
The C++ contract has explicit depth/wire-budget refusal and prior-value checks;
this does not introduce or claim a per-document C++ heap cap. Both remain
unrun. Future command-plan Source is in qualification/; it does not execute.

The original generated schema.c comment says 157 file descriptors. Actual
production descriptor metadata contains155:149 Spot originals and6 WKTs;
the2 separately retained self-registration examples are not generator inputs.
The original actual products and their generation receipt are unchanged.
This is a historical generated-comment count typo, not an extra schema or an
unrecorded regeneration. Correct it only in a future genuine Source generation.
