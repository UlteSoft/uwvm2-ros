# LLVM-JIT-only assembly inspection

The built-in debugger may inspect or step only code belonging to an authenticated LLVM-JIT-generated Wasm function. This restriction applies at every debugger permission level. There is no launch option or session command that permits inspection of VM, libc, a preloaded driver, or other shared-library machine code. Use a separate host debugger for those components.

## Live-code ownership

The controller never reads a client-provided native address. It selects a private runtime activation capture and asks the runtime for an owned copy of its current generated function. Before copying live bytes, the runtime:

1. Canonicalizes the capture against its private registry, including shared ownership.
2. Acquires GC/execution admission and pins the actual stopped participant.
3. Revalidates the pause ticket, publication, module, function, generation and runtime epoch.
4. Resolves the exact current LLVM engine and its compiler-identified loaded text endpoints.
5. Accepts only the typed Wasm body or checkpoint resume body; raw-entry adapters and unknown roles are excluded.
6. For an instruction stop, also verifies the real active platform session, worker, PC, stack cursor and owner interval.

A missing owner-table implementation or failed check makes the operation unavailable. Numeric containment, a symbol name, decoded bytes, or a previously returned address does not substitute for this proof.

The production read boundary is in `uwvm_runtime_debug_native_code_api.h`; current-engine resolution and loader publication are in `uwvm_runtime_native_owner_publication_impl.h` and `uwvm_runtime_native_owner_loaded_endpoints.h`.

## Decoding and bounds

MC decoders receive a byte span and a display PC. They never convert the display PC into a readable host pointer. The old raw-address `decode_owned` helper has been removed; component fixtures use `decode_copied`.

Whole-function inspection copies the actual authenticated function extent, subject to representable allocation size. There is no 64 KiB function limit or 8192-instruction table limit. Forward decoding retains scalar indices and one requested page. Negative instruction offsets use proved boundaries from forward decoding; they do not scan live memory backwards or guess a new instruction boundary. ARM literal-pool bytes are excluded by the loaded instruction/data map.

Per-request paging and fixed-size target metadata are transport/decoder bounds, not total function-size or total instruction-count limits. Failed, truncated or unsupported decoding remains unavailable. An unavailable row never authorizes a read beyond its owner.

The exact leased engine supplies the target triple, CPU, features, pointer width, byte order and MC instruction metadata for the same code image. Host preprocessor macros select the linked LLVM registry family; they do not fabricate production target metadata. LLVM 21 pointer getters and LLVM 23 reference getters are normalized from that same leased TargetMachine.

## Inspection versus execution

| Stop origin | Available authority |
|---|---|
| `safepoint-code-view` | Inspect the current authenticated generated function. It is not a native register snapshot or an instruction stop. |
| `native-instruction-stop` | Inspect the authenticated function and the qualified native trap state, subject to permission and platform capabilities. |

The current-PC copy has a bounded reply size; whole-function navigation uses the separately authenticated function image. A cooperative view cannot be promoted into a native trap by supplying a PC, stop label or decoded instruction.

Assembly breakpoints require a genuine native stop, an exact decoded boundary and guest-code provenance. They retain a private capture and bind to the participant, module, function, generation, epoch and owner interval. Displaying an instruction is insufficient to authorize its breakpoint or execution.

Same-owner stepping proves the successor before resuming. A cross-function Wasm transition requires a separate runtime-issued continuation proof. A host/import/DL call can execute normally as an opaque step-over with tracing disabled, then return to its proved Wasm continuation; the call grants no callee assembly, register or memory inspection.

## DAP lifetime and permissions

DAP assembly references are issued for the current stop. Each page checks the stop and owner identity again. Absolute breakpoint addresses must already have been issued by current owned disassembly and are translated to a function-relative request; the runtime independently checks that request. Resume, retirement and replacement invalidate the relevant references and bindings. A retained executable allocation does not retain debugger authority by itself.

`full` permits inspection and authorized control. `read-only` permits inspection of an available stop, without mutation or stepping. `limited` and `none` do not grant assembly inspection. These permissions do not change the LLVM-only boundary.

## Platform qualification

The ownership contract is shared across platforms. The source includes Linux backends for the ROS ABIs, Mach ARM64, opt-in Mach x86-64, and a gated Windows x64 backend. Build switches, SDK support and actual platform qualification determine availability. The presence of a backend, an enum value, a successful parser test, or a build does not establish native stepping correctness.

Keep qualification evidence tied to the exact source, binary, target and adapter version. In particular, copied-byte decoder tests establish decoding bounds; they do not establish live JIT ownership. Runtime tests must separately cover authentic positive controls, forged or stale references, owner edges, host/import calls, replacement and all four permission levels.

These queries run only on debugger inspection/control paths. They do not add calls, metadata queries or generated IR to ordinary GC or Wasm execution.
