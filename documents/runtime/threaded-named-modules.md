# Threaded Runtime and ROS named-module source

This cohort supplies a genuine named-module route to the existing Linux threaded
Runtime, CLI signal controller and ROS Host. It is source only. No compiler,
configuration, module scanner, linker, test, Guest, native ROS, SSH or performance
run has qualified these changes. The existing threaded implementation and all
SDK qualification gaps still apply. Rebuild the entire source graph; do not reuse
old BMIs, module initializer objects or an earlier Runtime product.

## Ownership and imports

`uwvm2.uwvm.runtime.initializer.limits` owns the unchanged limit declarations.
The old `initializer:init_limit` partition re-exports that owner. Admission stores
this real type by value. This leaf avoids a cycle from Group through initializer
back to Group. The ordinary initializer specialization remains unchanged.

`uwvm2.runtime.wasm_threads:group` exports the actual Admission, Group, compiler
work, readonly metadata and B1 control classes and their implementation. Its
native C++ friend seam has global C++ linkage, matching Runtime's real private
bridge. The selected Source/storage types are imported from their actual owner;
there is no second Source, registry, World or ready-bit provider.

The actual child context is defined in the initializer's original primary
module. The World partition defines its own context member functions in that
same owner, while `ThreadedChildWorld` retains the original global C++ friend
identity. Per-child tables, globals, TLS, passive segments, active-main/once
initialization, storage accounting and physical join are the original bodies.
No child reruns `_start`, repeats active data writes or becomes a new ROS
principal. The memory grow method now uses its genuine exported linear-memory
result types and original implementation.

`uwvm2.runtime.compiler.uwvm_int.threaded` exports the actual contract, program
and engine, with the original validation and execution implementation. LLVM's
threaded lowering scope remains owned/exported by the actual Translate module;
its private exception ABI stays the original global C++ native bridge. Native
system and LLVM SDK declarations belong to global fragments. Runtime imports
project owners instead of textually redefining them in a global fragment.

`uwvm2.runtime:threaded_profile` exports the genuine Host API and re-exports Group.
The preprocessing-only feature header is evaluated locally by each API,
implementation, loader, CLI registration and controller unit. Imports do not
propagate macros. That predicate describes available source, never permission.
Actual limits remain zero until an explicit successful Host/CLI configuration.

## ROS is part of the route

`uwvm2.imported.ros` is the single Core owner for the real policy, capability,
wire, IPC, audit, cancellation and lifecycle headers. The fast_io module supplies
its existing global declarations; Core does not copy its definitions into a new
module. `uwvm2.uwvm.imported.ros.policy_state` owns the actual CLI policy state.
`uwvm2.uwvm.imported.ros` owns the real Host implementation, owned publisher,
subscription/service transport, debug bridge and loaded-policy validation.

The builtin provenance table has a separate owner,
`uwvm2.uwvm.runtime.threaded_builtin_loader`. It imports the genuine concrete
thread and enabled ROS factories. Keeping it outside the Source interface avoids
Source -> Host -> Runtime API -> Group -> Source recursion. Loader and bridge
share this one actual table, including its existing provider-drain retirement.
No copied function name, opaque pointer or user-supplied World issues authority.

The ROS parameter/callback templates and objects have a single params owner.
All original ROS registration includes remain in the real CLI table. Linux
SO_PEERCRED, exact caller/generation, capability leases, owned endpoint pairing,
body-only quotas, cancellation and fail-closed gates remain the actual Host code.
The original 81 ABI catalog and every optional numeric catalog are unchanged.
Module support neither grants an alias nor permits a disabled Target.

## CLI and lifecycle

The two real params partitions and two callback partitions are registered.
Cross-primary callback declarations and definitions use the same global C++
function identity. Module definitions are retained as real external definitions;
missing Runtime/dispatch objects must fail final link. The run-owned controller
partition imports the real params state and Host API. It preserves the six-value
signal option, strict fast_io parser, lock-free raw handler notification, ordinary
CLI-thread typed push, absolute grace/retire deadlines, and independent process
containment on a pending physical join. There is no fake join, detached worker,
clock/ROS grant or management import.

The Runtime's main implementation, cache policy, Source lifecycle and physical
retirement bodies are unchanged. Only necessary module dependencies and former
header-only availability gates are connected. Threaded persistent-code cache
and ordinary DBG/checkpoint restrictions remain as implemented; a successful
module import does not relax them. Whole-source BMI/initializer consistency is
mandatory across Runtime, all frontends and custom/control registry macros.

## Required qualification

The source contracts exercise real exported definitions, cross-TU state
identity, inaccessible issuers, default deny, full UINT64_MAX association and
failure output preservation. They do not substitute for actual spawn/join,
trap/proc_exit, stop/reset/update/debug races, compiler cancellation, atomic
shared-memory I/O or native ROS. The existing 43-case threaded packet, embedding
cases, signal pipe/reader tests and actual original-rclcpp/default-control
producer must be rebuilt/run on the final combined source under Root's admitted
resource window. Both INT and LLVM paths, ROS1/ROS2 (including simultaneous
manifests), ordinary no-policy hot path, ABI catalog checks, B1 gates, missing
strong-symbol link negatives and actual module initializer retention remain
required. Record real tools, source/build/product and runtime outcomes separately.
