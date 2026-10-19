# Complete control source-domain contracts

This source cohort adds a canonical **counted raw-byte** domain only to the original `type_description_interfaces/msg/TypeSource.raw_file_contents` field. Its native type is still the exact ROS `string`: native ownership, generated field layout, complete TypeDescription, original source declarations/defaults, RIHS and native identities are unchanged. A new adapter identity explicitly binds the canonical domain and aggregate quota. Other strings continue to reject NUL, malformed UTF-8, overlong encodings, surrogates and invalid scalars. Other catalog formats reject the encoding property.

The selected original ROSIDL C description template constructs its source string from joined source lines and appends NUL, then exposes the original source extent. For the selected LF-terminated ASCII files this makes that terminal NUL part of the logical `String.size` view. An independently allocated native String may additionally have an allocation terminator beyond its size; that extra byte is not canonical content. Never use `strlen`, trim NUL, normalize newlines, or equate the source-file hash to a changed getter-view hash. The contracts compare the complete actual getter `size` prefix with the actual service response and counted canonical bytes.

The preparation catalog retains all original fields and defaults and expands IndividualTypeDescription fields to 10, referenced descriptions to 11, sources to 12 and raw source bytes to 6162. `TypeDescription.message` and `GetTypeDescription.response` have explicit aggregate wire limits of 32768. A complete actual value can fit despite a larger cartesian capacity. `Record.maximum` is admitted logical wire extent; `Record.cartesian_maximum` retains complete theoretical field capacities. Neither describes C object `sizeof`, compiler-reserved decoder stack, native allocations, RSS, or total test process memory. The C layout still contains every inline element: budget its actual target `sizeof` and stack before future execution. The generated schema/proof and macros report the distinction honestly.

`wire_limits` is control-only and must map an existing component role (`message`, `request` or `response`) to an actual integer from 1 to 32768. Booleans/floats, unknown roles, empty declarations and quotas below the full minimum are refused. No global message/body/node ceiling is raised. Expanded nodes4096, total generation nodes16384, depth16, grammar work65536 and checked integer arithmetic remain independent. Only an actual new `bounded_record` in the grammar permits intermediate cartesian extents above32KiB; callers cannot activate that traversal mode through a public boolean. New enum values `raw_bytes` and `bounded_record` are appended; all existing values and `custom_wire_node` fields stay unchanged.

For affected C records, `_shape` checks all active original fields and nested shapes, `_measure` checks every nested and aggregate wire cap and commits the size only on success. Invalid shape returns22; valid values exceeding a quota return90. Encode validates and measures before any byte/length write. Decode checks a private complete value and commits only after exact length, nested semantics and all quotas succeed. Failure preserves the caller's original value. It can still require a large private automatic temporary; this source does not claim OOM/trap immunity. Callers must supply distinct owned value, byte buffer and size slot. The `_wire_size` compatibility view returns0 on failure, so use `_measure` for status and empty-value disambiguation. C++ wrappers preserve their old interface and output on failure.

`control_wire.hpp` is an additional closed C++ canonical grammar emitted only by catalogs opting into the new domains. It supplies independent whole-wire validation without native objects, registry lookup, callbacks, host imports or new permissions. It is not a native TypeSupport and cannot authorize ROS activity. Native conversion remains a separately reviewed cohort: it must consume actual `Field.string_encoding`, retain native String data/size, call the admitted aggregate measure before native allocation/commit, and preserve90 distinctly rather than flatten it to invalid22.

## Authored contracts (all unexecuted)

- `source_domain.c`: eight contract groups for arbitrary raw bytes/final NUL, continued text rejection, exact6162, complete10/11/12 capacities, exact32768 and overquota90, independently constructed oversized wire, truncation/trailing/count atomicity, and overlap/capacity refusal. The independent74113-byte wire deliberately exceeds the production cap; it is a refusal fixture, not an accepted message.
- `source_domain_cpp.cpp`: four groups using the actual generated typed codec and shared grammar; checks typed output atomicity, unchanged enum values, explicit cartesian opt-in, nested quotas, cycles/node/work limits and full aggregate refusal.
- `test_source_domain_policy.py`: eleven authored parser/emitter groups. Root must explicitly provide both pinned generator versions and source roots. It imports actual reviewed sources only when separately run, writes temporary catalogs only in explicit scratch outside the SDK, and compares unchanged catalogs' full C/C++/JS/Rust/Go/AssemblyScript emissions and canonical identities; it also compares complete old control products/schema and generation metadata after removing only the honestly changed bounded-generator source hash. Generator-source provenance hashes necessarily update honestly; that is not an old codec/schema identity change.
- `native_sources_true.cc`: a real installed-SDK witness. It warms actual generated service hash/description/source getters before creating an unchanged-default original Node; performs a real asynchronous `include_type_sources=true` service call; checks all original fields, defaults,11 references and12 source strings against those exact getters; compares complete canonical C capture and decode field by field, and validates the wire through the independent generated grammar. It uses real steady deadlines and actual executor completion, removes local pending state on timeout, and never replays a request. The selected source closure must contain actual6162 raw bytes and a counted terminal NUL. Missing/changed modern APIs/closure cannot be credited as qualification. A real C-aware original Service/AnyServiceCallback ownership port, the rebuilt original rclcpp target PUBLIC-linking its admitted helper, and genuine generated C/C++ TypeSupports are required; no fake POD/deleter/header substitutes are supplied.

The native witness runs local native ROS endpoints only when Root separately authorizes future execution in an isolated explicit domain. It does not install the Guest registry or establish uwvm2 permission, production Host/Gateway, Wasm, default Node, all ROS distributions or general ROS compatibility acceptance. Stage2's older `include_type_sources=false` witness remains historical false-only evidence. A future complete default Node test must also account for actual TimeSource parameter-event subscriptions and every unchanged default endpoint/grant; this package does not quietly disable defaults.

## Future build and run recipe

No commands below have been run for this source HOLD. Use a complete coherently composed private source tree, fresh `/tmp` output and caller-owned resource limits; never generate into the SDK. First apply the reviewed generator/wire/catalog changes, then derive native identities and the native converter in their separately reviewed source cohorts. The source catalog itself grants no authority (`[]`).

1. Future canonical preparation: `python3 /absolute/reviewed/sdk/tools/custom_types.py /absolute/reviewed/sdk/rmw_wasm/default_control/catalog.preparation.json --out /tmp/unique-control-generated --guest-only`. These are the actual existing generator CLI flags; they emit source, not native IDs or permission.
2. Future policy contracts: `python3 /absolute/reviewed/tests/ros_control_source_domain/test_source_domain_policy.py --sdk /absolute/reviewed --before /absolute/preserved/bounded_profile.py --after /absolute/reviewed/sdk/custom-types/bounded_profile.py --old-control /absolute/preserved/catalog.preparation.json --new-control /absolute/reviewed/sdk/rmw_wasm/default_control/catalog.preparation.json --scratch /tmp/private-existing-scratch --parent-sha256 ACTUAL_FULL_CURRENT_PARENT_SHA256 --before-sha256 ACTUAL_FULL_PRESERVED_BEFORE_SHA256 --after-sha256 ACTUAL_FULL_CURRENT_AFTER_SHA256`.
3. Future pure canonical build: `cmake -S /absolute/reviewed/tests/ros_control_source_domain -B /tmp/unique-control-build -DUWVM_CONTROL_GENERATED=/tmp/unique-control-generated -DUWVM_CONTROL_REPOSITORY=/absolute/reviewed` then `cmake --build /tmp/unique-control-build --parallel 1`. Root may separately run the two actual executables with explicit process/stack budgets. CMake never runs them automatically.
4. Future native-only witness: use the actual original53fa source rclcpp C lifecycle port and real installed original interfaces, with `-DUWVM_CONTROL_BUILD_NATIVE_TRUE=ON` and their actual `CMAKE_PREFIX_PATH`. This requires real imported C generator/runtime/C and C++ TypeSupport targets and the admitted C ownership target. It performs a genuine function-scoped EXECUTABLE compile-and-link API probe, never try_run and never a STATIC_LIBRARY shortcut. Root may separately run `uwvm_control_native_sources_true DOMAIN` in an owned isolated ROS domain (0..232). This is actual Native qualification only if its complete original-source/version and execution evidence are retained.

Old profiles remain strict and the old nonextended generator paths remain selected. This is source-recompile compatibility, not a claim that new private IR objects, generator plugins or enum domains form a stable native binary ABI. No guest namespace, ROS action, clock grant, native handle, native hash, source owner, maximum endpoint count or policy grant is minted here.

The exact Native serialization route is independently qualified: if an installed ROS serializer uses a C-string terminator rather than preserving the generated getter's counted prefix, this strict native witness fails the byte comparison. It never trims or adds bytes to obtain success. The genuine Gateway canonical adapter and the original native ROS serializer are distinct paths; neither is declared tested here.

## Preserved r1 and narrow Native witness correction

The original r1 sources/HOLD remain preserved. This successor changes no generator, catalog, canonical grammar, C/C++ codec contract or Python policy body. The real native witness now checks its genuine steady deadline immediately before `async_send_request`. If the actual send consumes the last budget, it removes that exact SDK pending request ID before refusing; executor spin receives a strictly positive remaining duration. An incomplete spin still removes its own pending ID, without replay or native cancellation claims. Physical native send duration is not preempted by a post-call clock check.

The Native CLI uses the actual pinned fast_io `parse_by_scan` with `dec_get<true,true>`: no leading-space skipping, explicitly accepted redundant leading zeros, no implicit leading plus, strict complete consumption and domain0..232. The input token is bounded to20 bytes; no handwritten decimal parser or `from_chars` fallback is supplied. This native witness target therefore explicitly requires C++20 and the repository's real fast_io headers. The original rclcpp C ownership API probe remains a genuine separate compile-and-link admission and is not falsely credited as an executed witness. All sources and future contracts remain unexecuted.


## Source-only generator loading and honest provenance

The current installed parent generator selects only its fixed reviewed sibling
`sdk/custom-types/bounded_profile.py` for the closed bounded/official/control
format list. A catalog cannot select an arbitrary Python module or provider.
The loader opens that source with O_RDONLY, O_NONBLOCK, O_NOFOLLOW and
O_CLOEXEC, rejects nonregular/oversized input before reading, captures exactly
the initial extent with at most one extra detection byte, and retains that
SAME descriptor through `compile(bytes, real_filename, 'exec', dont_inherit=True)`
and ModuleType execution. Full descriptor and named lstat identity (device,
inode, size, mode, links, owner/group, mtime and ctime) must match before and
after execution. Symlink/FIFO/source replacement or growth refuses.

This source path never invokes SourceFileLoader or a pyc reader/writer.
`sys.dont_write_bytecode` alone is not evidence: it does not prevent reading an
existing cache. Standard-library imports remain the actual Python runtime's
separate dependency closure; this package does not claim to disable every pyc
in the interpreter. Real dataclass/future-annotation resolution sees the exact
module in sys.modules before exec; failure restores its previous binding.
The module records the SHA256 of the bytes actually compiled. Later provenance
reads of an admitted generator require that same source identity and SHA,
rather than reporting a replacement file's digest. Normal non-generator
bounded input behavior and all original eleven DomainPolicy method bodies
remain unchanged.

The policy runner now requires three independent complete lowercase source
SHA256 arguments in addition to the existing explicit SDK/before/after/catalog
paths: `--parent-sha256`, `--before-sha256` and `--after-sha256`. Root must obtain
these from the exact reviewed source snapshots, not from cached bytecode or a
filename alone. Parent is still the actual `--sdk` sibling; before and after
are separately explicit source DATA. No generator/contract has been executed
for this successor. Source provenance necessarily changes honestly, and JSON
source-proof output is not claimed byte-for-byte equal to an older revision.

`test_source_identity.py` supplies separately authored failure contracts for
matching-cache traps, dataclass registration, wrong SHA, symlink/FIFO refusal,
post-exec source drift, binding rollback, guarded later provenance reads and
fixed sibling selection. Run only after independent source review and explicit
execution admission, using a private scratch outside the SDK and an externally
finite process owner. These contracts test Python source identity, not ROS
Native identity, permissions, TypeSupport, generated products or qualification.


The source-identity runner compares resolved scratch against the physically
resolved containing SDK directory. A lexical SDK directory or ancestor alias
(including `/tmp` versus `/private/tmp`) cannot make actual SDK-internal scratch
appear external. Only that directory is resolved: the selected source leaf
remains lexical and still enters the unchanged O_NOFOLLOW loader. A tenth
authored identity group covers SDK-directory aliases, a synthetic
`tmp -> private/tmp` ancestor alias, boundary equality and an actual host `/tmp`
alias when present under the owned fixture. All fixtures for that regression
remain inside previously admitted external scratch; no test has been run.
