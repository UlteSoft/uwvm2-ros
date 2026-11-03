# Mandatory native runtime fixture migration

This migration is authored source. Build, Wasm execution, native ROS execution,
QEMU and performance evidence remain **UNTESTED** until a separate controlled
Linux run records the exact source and executable hashes. Building `uwvm2-ros`
alone does not compile these nondefault test targets.

The ROS product has no public call-stack option or mutable stack-mode global.
Its initializer must always validate actual generated-chain native unwind and
crash-stack recovery. Runtime fixtures use that contract directly. Their
`instruction` debug safe-point granularity remains valid and is retained.
Native registration, live-probe, trap, stack, source, ownership, pause/resume,
cache, publication, GC and exception assertions remain real execution oracles.

Runtime fixture positional selectors were removed. For example:

- `fixture module.wasm instruction` becomes `fixture module.wasm`.
- `llvm_exception_host_tail_reentry module.wasm unwind throw` becomes
  `llvm_exception_host_tail_reentry module.wasm throw`.
- `debug_wasm_blocked_wait_runtime module.wasm unwind notify resumable 32 64`
  becomes `debug_wasm_blocked_wait_runtime module.wasm notify resumable 32 64`.
- `debug_wasip1_environment_group_runtime main.wasm provider.wasm unwind out`
  becomes `debug_wasip1_environment_group_runtime main.wasm provider.wasm out`.

The common C++ fixture helper rejects legacy selector arguments. It does not
perform, bypass or substitute for the production native-unwind probe. Old
positional forms and the removed public option must fail explicitly.

ROS-aware Python drivers emit a single `mandatory-native` row, omit the removed
public option and reject explicit diagnostic-stack selections. Scripts with a
backend selector require LLVM/full for ROS. Ordinary-upstream instruction,
unwind, none, lazy and tiered matrices remain available only with a separately
built ordinary-upstream executable. Mac-only ROS debugger routes reject `--ros`.
Historical Windows/macOS recipes and old platform-status notes do not establish
current product support.

The four threads translation fixtures, atomic bridge identity fixture and
`memory64_translation_ir.h` consumers retain explicit **test-local offline IR**
choices: `component-native-cfi` and `component-instrumented`. Only their local
`compile_option` fields vary. Runtime host-entry execution remains mandatory
native in both invocations. IR filenames and diagnostics use the component
labels. An instrumented component IR pass is not ROS instruction-mode execution
coverage. Baseline machine-code comparisons require regeneration with the same
fixture hash; historical output must not be renamed to satisfy a new receipt.

`llvm_native_eh_private_leaf_publication_runtime module.wasm
component-instruction-decline serial` is a separately named **BLOCKED** case
(exit 77). An isolated publication-admission fixture with a test-local emission
input has not been implemented. This case runs no native publication/CFI oracle
and must never enter the successful runtime case list. Public, private, rollback,
debug-decline and uncaught cases retain their actual publication and lifetime
assertions.

The reduced-runtime audit keeps its strict production and build checks. Its
former blanket tooling keyword ban is replaced by a finite AST audit of four
canonical positive ROS command authorities. Independent-upstream matrices,
negative selector refusals and offline component vocabulary remain legal.
This source audit does not replace real driver-routing or execution evidence.

Run parsing, focused helper refusals, independent fixture builds and exact guest
oracles on the Linux keeper after the current product build, under its global
lock and actual process cgroup. Record implementation, build, Wasm execution,
native ROS, QEMU and performance statuses separately. Do not count an exit 77,
missing dependency, unavailable backend or source-only check as an execution
pass.
