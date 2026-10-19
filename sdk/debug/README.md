# Owned language frame snapshots

The copied-DATA Host parser fixture has compiled and passed in the admitted SSH
Linux cgroup. The target language producers and canonical Runtime memory-copy
adapter remain development source without target execution qualification. This
directory implements a bounded Guest snapshot format, a strict Host parser,
the memory-copy adapter, and a CPython trace producer. It does not yet register logical frames with the
controller or qualify a debugger command. The Kotlin and Mono teams are writing
producers for this same format. None of these files changes a shared Runtime
gate or introduces a Guest Host import.

## Trust and lifetime

The Host obtains the root pointer by evaluating the real snapshot parameter or
global's DWARF location in the current activation. It then calls `copy_current`
inside `llvm_jit_debug_with_source_memory_host_api`'s existing synchronous
callback. That API retains the actual stop ticket, participant captures,
publication, source binding, code generation, memory ownership and bounds.
The expected position must come from `view.copy_position` in that same callback,
including its exact cooperative location and source-context availability. A
standalone metadata query is not a substitute and may have different context
availability. Position equality itself does not authenticate a fresh stop.
`copy_current` compares the complete activation chain and source position,
copies128 header bytes, copies the complete59008 bytes, compares the two
headers, parses detached data, and rechecks the actual position. It retains no
view, Guest pointer, callback or memory alias after return.
The expected metadata may alias the output's previous actual position; the
adapter keeps it alive until all comparisons finish, then replaces or clears
the output. Failed calls do not leave a previously successful snapshot visible.

Do not call `copy_current` with a sidecar address, a Guest integer, a source-map
label, a cached pointer or an earlier stop. This helper does not evaluate DWARF
or mint the root-location proof. The controller integration must supply that
proof within the same transaction and publish logical frames with the actual
Runtime position. Until that integration exists, this is an implemented parser
and copy seam, not a working language-frame command.

Guest fields are untrusted presentation data. A frame ID, `LIVE` flag, sequence,
layout number, claimed source line or language tag never becomes a Host stop,
publication identity, memory capability or ROS grant. The first profile supports
one locally defined unshared memory. The Runtime's real import/provider census
must also admit its memory reads. The current pure-WASI observation path does
not qualify ROS Host imports. ROS and debugger teams are separately implementing
the actual bounded-copy/no-escaped-alias adapter; no namespace exemption is used.

## Wire layout

All integers are little-endian. C producers use the admitted little-endian
target and compile-time size/offset assertions; other producers must serialize
the same fields explicitly. The Host never casts input bytes to a native
pointer or record. The C reset builder returns0 and leaves an invalid zero
header on a native big-endian producer; check that return before publication.
The Host reader and explicit-LE byte fixtures remain endian-independent.

| Region | Offset | Count | Element size |
| --- | ---: | ---: | ---: |
| Header | 0 | 1 | 128 |
| Frames | 128 | at most16 | 96 |
| Values | 1664 | at most512 | 64 |
| Text and byte arena | 34432 | at most24576 bytes | 1 |

The complete object is always59008 bytes. Magic is `55 57 4c 46 00 00 01 00`.
`schema=1` and `layout_generation=1` describe this representation only. The
layout field is not a GC counter or the Runtime's code generation. All reserved
fields and unused records/arena bytes must be zero. `sequence` is a nonzero even
producer counter that must not wrap. It distinguishes copies; it cannot revive
a stale Runtime activation. Publish `LIVE|INSTRUMENTED` only during the actual
anchor call and invalidate `LIVE` on return. Capture failure sets status1 and
`CAPTURE_ERROR`; the Host returns unavailable instead of publishing partial
typed locals. No flag authorizes a Host operation.

Frames are ordered outermost to innermost. IDs are snapshot-local1 throughN.
Frame0 has parent `UINT32_MAX`; each later frame has the preceding frame's ID as
parent. Missing outer frames must be marked truncated. The Host's frame key must
also include the actual publication/activation position and this copy sequence;
these small Guest IDs have no independent lifetime. File, function, line,
instruction and argument count each have a separate known bit. Unknown fields
are zero. Source filenames/function labels are diagnostic data, not admission.

Each frame reserves its root values contiguously before copying descendants.
Each container does the same for its direct children. Root `parent_value` is
`UINT32_MAX`; other parent values are actual earlier record indices in the same
frame. Depth is at most4 and a container has at most32 direct children. The
Host rejects cycles, overlapping/reused/orphan value records, out-of-range
children, cross-frame parentage and unknown masks/kinds. The format represents
trees of copies, not an object-identity graph. A cyclic or deeper Guest object
has an explicitly truncated container value.
When a container's original count is known, its copied child count cannot exceed
that count. Known complete children without truncation must equal the original
count. Dictionary counts refer to pairs, not twice the number of pairs. An
unknown count does not become known merely because child records are present.

Names and UTF-8 data use arena-relative offsets and explicit lengths. Embedded
NUL is data. Invalid UTF-8, surrogate encodings and out-of-range spans are
rejected. A binary value uses its own kind and may contain arbitrary bytes.
Render text through the debugger's existing escaping; do not print arbitrary
source labels as terminal control text. Unknown/unsupported values remain
unavailable and never carry an object or Host address.

Kinds are unknown, None, bool, signed64, binary64, UTF-8, bytes, list, tuple,
dictionary and unsigned64. Scalar bits are exact, including unsigned64 above
`INT64_MAX`, negative zero and nonfinite binary64. String payload is the original
codepoint count; bytes/container payload is the original element/byte count.
Truncated prefixes keep the original count and a truncation flag. The Host
parser counts actual UTF-8 codepoints, including embedded NUL, independently of
encoded byte length. Known complete UTF-8/bytes copies must match the original
count; truncated copies cannot exceed it. Empty and unknown values follow the
same rules. A producer that omitted a nonempty value's original count is refused.
The parser fixture covers multibyte Unicode, arbitrary binary data, empty copies
and truncated prefixes. Its admitted Native Linux result is recorded below;
target language producer and Runtime qualification remain separate. A dictionary's
children are two-element tuples containing the real key and value; no key is
converted with `repr`, `str`, hashing or equality. Unsupported keys remain
unavailable without silently changing the key type.

## Actual CPython producer

`sdk/python/debug/frame_snapshot.c` installs an owned C trace slot on the one attached
GIL thread. It refuses an existing trace owner and verifies ownership on removal.
It targets the exact admitted CPython3.14.8 full ABI on wasm32, and rejects the
Limited API, free-threaded builds and other target widths. It observes real
line, return and exception events and real frame/back/code/variable C APIs.
The official [frame API](https://docs.python.org/3.14/c-api/frame.html) and
[trace API](https://docs.python.org/3.14/c-api/profiling.html) describe those
interfaces. Fixed-tag upstream source is retained in this package; it still
needs comparison with the actual adapted target build before compilation.

The producer preallocates the complete output, preserves the exact current raised
exception, disables cyclic collection for capture and the anchor, copies real
values, invalidates the snapshot, restores collection state and restores that
same exception. Public frame APIs can allocate, so capture errors are explicitly
unavailable. Exact list/tuple/dict/string/bytes/int/float/bool objects are copied
without property, iterator or representation calls. Other instances, huge
integers and unsupported layouts are unavailable. There is no arbitrary Python
heap traversal or Host-driven Guest evaluation. This is a debug instrumented
profile with measurable overhead, not a zero-cost normal runtime change.
The producer publishes and clears LIVE through volatile stores with compiler
signal fences. Its real anchor is a separate translation unit, compiled with
`-O0 -fno-lto`, so the producer sees an opaque consumer of the whole snapshot.
This Source control is not a code-generation test: retain actual final IR/Wasm,
stores, call/parameter location and a genuine stop before claiming live values.
Optimized/LTO provider profiles remain unsupported until separately qualified.

`sdk/python/embedding/main.c` has a default-off `UWVM_PYTHON_DEBUG_SNAPSHOT` hook for
the real ROS application main. Set it to1 only when linking the actual provider.
The separate `sdk/python/debug/non_ros_main.c` registers no ROS extension and links
no ROS SDK dispatcher; it is the non-ROS entry. It freezes `debug_app` and the
explicit UTF-8 encoding closure using the matching native interpreter, then
runs the real application with its actual argv. Target `sys`, `gc`, `_codecs`,
import bootstrap, startup, libc and libpython closure must be independently
admitted. A normal stdout result alone gives no debugger credit.

The copied Python filename, qualname and bytecode offset describe the actual
producer's current frame. Resolving those labels to an external Python source
file still requires an exact frozen/nested-code mapping and its source identity;
dynamic code may reuse a filename. The existing C anchor's DWARF location does
not establish that separate association. Automatic Python source breakpoints
and logical frame selection remain pending that controller integration.

## Linux build and test handoff

All commands below require the coordinated existing64GiB/swap0 SSH Linux keeper.
They are Source recipes and have not run. Admit every entry/interpreter/stdlib,
source, compiler and output identity before use. Use fresh output directories,
serial commands and the existing resource window; no local Mac execution.

For the independent actual Host parser fixture:

```sh
clang++ -std=c++20 -O1 -g -UNDEBUG sdk/debug/tests/snapshot_reader.cc \
  -o "$FRESH/parser-fixture"
"$FRESH/parser-fixture"
```

This executes the actual parser's scalar/uint64/tree/UTF-8/binary ownership and
malformed-layout cases. It does not exercise a Runtime stop or Python/ROS.
Use the actual admitted compiler path instead of an ambient tool selection in
the final keeper argv. Retain actual exit/stdout/stderr and process retirement.

On 2026-10-18, the actual Linux `g++-15 -std=c++20 -O0 -UNDEBUG` build and fixture
completed successfully with `rejected=37`. The exact three Source pins, compiler
identity, 239 dependency records, command results and closed ownership records
are preserved in the [readback evidence](../../documents/ros/continuation-20261018-evidence/common-debug-parser-r4-peer-actual-linux-root-readback-20261018-r1/integration.json).
This is a Host copied-DATA parser result. It does not qualify any target language
producer, canonical DWARF root, Runtime stop, controller command, GC or ROS path.

For the non-ROS frozen input, run the actual native3.14.8 interpreter:

```sh
"$NATIVE_PYTHON" -I -B sdk/python/debug/freeze_debug.py \
  --entry "$SOURCE/sdk/python/debug/non_ros_probe.py" \
  --stdlib "$CPYTHON/Lib" --out "$FRESH/frozen"
```

Compile `frame_snapshot.c`, `snapshot_anchor.c` and `non_ros_main.c` separately with the matching target
`pyconfig.h`, Python headers, `-std=c17 -g -O0 -fno-lto`, the generated header directory,
and the actual wasm32-wasip1 sysroot. Link their real objects against the
admitted CPython static library and its complete original target link closure,
retaining all DWARF. The original exact link recipe is a required producer
input; this work does not invent a portable replacement library list. A complete
owned build/CLI recipe will be published after that closure and actual final
imports/whole-Wasm validation pass. Before that, the full build remains pending.

Run with application argument17. The real probe computes27 and prints
`PYTHON_DEBUG_COMPLETED 27`. In the debugger, stop at the actual C anchor read,
derive its parameter root with DWARF, and inspect the current snapshot. Require
the three recursive Python frames, actual scalar/structured values, full
`0xfedcba9800000007`, negative-zero bits, UTF-8 with NUL, bytes, the truncated
self-cycle and unavailable custom instance. Resume through real `gc.collect`
and require the subsequent current snapshot. Step/resume, change frame, and
refuse prior-stop/activation/sequence copies. The actual property/representation/
iterator counter must remain zero. No expected-value table may replace those
observations. The controller logical-frame publication and these live-stop
cases still need implementation and execution.

For ROS, use the real TypeQuery/parameter application, two independent grants,
complete request/response correlation, a denied operation and independent
cleanup. Require the actual ROS memory observation adapter before reading.
Finally sign the exact final Wasm after all debug/link/strip/custom-section
changes using WSEA's per-path expected module identity. Signature admission,
actual ROS permissions and debugger read authority remain separate checks.
