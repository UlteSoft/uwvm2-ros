# Restricted integer leaf qualification experiment

This is an isolated compiler experiment in `uwvm2-ros-sdk/.tmp`, not a production
runtime change. It investigates the actual `ParseName` → `isalpha` call observed
in the symbol-qualified XML microarchitecture recording. Public Wasm functions
remain separately addressable, as do their raw wrappers and indirect targets.
The ordinary runtime, existing GC private-call specialization and legacy host
interfaces are unchanged.

`qualification_integer_leaf_inline.h` accepts an exact typed-function list from
the owning full materializer. The fixed limits are 65,536 entries/edges and 32
instructions per leaf. Eligible leaves have one basic block, at most eight i32
or i64 parameters, an i32/i64 result, and only integer add/subtract, bitwise
operations, comparisons, selects, integer extensions/truncations and return.
Operands must be defined local integer values or integer constants. Memory,
FP, shifts, division, calls, loops, EH/GC, poison-generating flags, undef/poison,
instruction metadata and incompatible/observable attributes are rejected.

One return-attribute exception is structurally proved: the sole attribute may
be `range(i32 0, 2)` (or its i64 equivalent) only when the actual return operand
is a local zero-extension of i1 to that exact result type. This is the shape
found in the real post-O3 `isalpha` body from the XML profiling guest. An
arbitrary value with the same range annotation, another range, or an additional
attribute is still rejected. No input is newly declared `noundef`, and the
public function and its attributes remain unchanged. Call-site metadata and
attributes continue to disqualify an edge.

Only ordinary direct calls from that same actual typed-entry set are considered.
Tail/musttail calls, invoke/callbr, call-site attributes, metadata and operand
bundles are refused. A private temporary copy loses `NoInline`, `NoMerge` and
`nooutline`; LLVM inlines that copy into the selected caller. The original
public target is untouched. The temporary function must have no users and is
removed before successful materialization. The pass does not provide new
private entry points or native-range aliases. Intel's finalized-code observer
measures the resulting actual machine-code ranges using the same arena owner
as the control VM; it does not reuse stale numeric ranges.

Component-owned dynamic containers use the recoverable fast_io allocator.
LLVM's own internal allocations retain LLVM's upstream allocation policy.
Any pass error prevents publication of the new engine. The restricted profiling
variant still has the previously documented top-level fatal startup-error
path, so this experiment does not establish industrial recoverable startup.

`qualify_integer_leaf_inline.py` runs real LLVM verification and MCJIT A/B code,
with effective `-O3 -g` for both ordinary and ASan/UBSan builds. The matrix checks
rejection of unsafe bodies/edges, the 32/33-instruction boundary and exact owner
identity. It verifies unchanged public target/wrapper/indirect IR and attributes,
absence of temporary functions before materialization, all 256 byte values,
i32 boundary values and 100,000 deterministic random values against the control
and reference semantics. Test pointer carriers only hold LLVM-factory-owned
objects; they do not allocate objects with a C++ standard allocator.

The `integer-leaf-boolean-range-first` receipt records ordinary and ASan/UBSan
MCJIT acceptance of both the original and range-annotated boolean body, with
the additional negative attribute cases. The original stricter experiment
produced zero transformed edges in the XML workload. The retained
`integer-leaf-ir-inspection/profile-leaves-second.json` and
`func165-rejection.json` identify the actual optimized body and that rejection;
this IR inspection stopped before machine-code finalization and is not an
execution or performance measurement. The new exception still requires fresh
VM execution, ownership qualification and measured assembly before any speedup
can be claimed.

`prepare_integer_leaf_vm.py` requires the exact symbol-qualified source manifest
and complete unit receipt. It copies that source into a fresh SDK `.tmp`
directory, applies two unique literal-anchor changes, adds the helper and writes
the complete new manifest, patch and parent hashes. `build_frozen_vm.py` requires
both explicit `--intel-jit-sdk` and `--integer-leaf-qualification` options. It
compiles all three translation units serially with `-O3 -g` and checks full DWARF
sections. No environment variable grants the optimization or profiling scope.

Actual VM acceptance must precede timing: XML correctness, zero-authority
denial, restricted-mode rejection, finalized code LOAD/UNLOAD byte identity,
reset/replacement identity and terminal retained-code lifetime. Both variants
use ordinary full LLVM, max policy, disabled cache and compile-threads zero.
`compare_integer_leaf_vm.py` rotates native/control/candidate order over three
rounds of 31 × 1,000 cycles, preserves every batch, standard deviation and MAD,
and records whole-window load, process and cgroup pressure. Strict rejection
that produces zero transformed edges is a valid negative result and must not
be reported as an optimization. No timing difference within uncontrolled load
noise establishes a speedup or native ROS parity.

Official hardware Hotspots must locate the frozen host C++ source; official
microarchitecture export must show actual guest machine instructions and call
targets. Guest name-section labels are symbols only, not original source lines.
The host's previously observed invalid CPU-time-derived microarchitecture
frequency/utilization values remain unqualified. Hardware-event samples may
skid and are not individual instruction latency measurements. All compilers,
tests, timed workloads and VTune commands remain inside the original 64 GiB,
zero-swap cgroup; limits are never raised to obtain a result.
