# Native object cache: identity and authority

The cache stores relocatable native objects, not a running ROS session. SHA-256
identifies inputs; Ed25519 authenticates the complete object envelope. A hash
alone is not an origin signature. Loading a valid object never restores a prior
process's ROS or WASI permissions.

The following checks have different purposes and must remain separate.

| Input or condition | Required check | Why it matters |
| --- | --- | --- |
| Actual VM executable | SHA-256 of the kernel-selected executing image; retain and recheck that file identity | Version strings and caller-supplied build IDs cannot identify the actual native implementation. |
| Compiled Wasm semantics, registered module name and translated IR | Complete pre-optimization IR hash plus the module/translation contract | Different instructions, import namespace or generated IR must not reuse another object's machine code. |
| Native target | LLVM triple, DataLayout, CPU and feature set | Native instructions and ABI must match the host. |
| Compiler/runtime contract | LLVM version, native ABI and code-generation policy fingerprints | Calling conventions, helper contracts and emission options affect the object. |
| Stored envelope | Format, size bounds, metadata equality, signature verification before decompression/use | Cache storage is untrusted input, even when the directory belongs to the user. |
| Signature origin | Live kernel-origin admission, current generation lease, sealed process and actual executable identity | Public metadata is not permission to obtain signing authority. |
| Native providers | Exact owned builtin provider identities and eligible runtime publication | A similarly named import or arbitrary native callback cannot mint cache authority. |
| Inherited WASI descriptors | Actual descriptor storage, type and relevant filesystem class | A regular procfs inode can expose live process state; ordinary log files cannot be rejected merely for being files. |
| Runtime authority | Current ROS/WASI policy, quotas, response validation and revocation on every applicable operation | A cached object is code, not a capability grant. |

The canonical CLI links its pinned LLVM, libc++, libc++abi, libunwind and
OpenSSL implementation into the final executable. Their actual linked bytes
are therefore covered by that executable's digest; rehashing unrelated source
trees on every cache hit would add no runtime identity. The source build verifies
the pinned dependency manifests independently. A compiler or runtime moved into
a separately updated shared object would need its own authenticated identity
and compatibility contract before that deployment could be qualified.

The native x86_64 qualification build dynamically depends on the system C/math
runtime, ELF loader and libatomic. These supply the selected platform ABI and
are resolved for the current process. They are not serialized ROS handles or
absolute addresses carried from an earlier process. The live unwind probe still
runs before guest execution. A compatible system-library update does not by
itself change the cached relocatable object. A deployment that changes the ABI
or native symbol-resolution contract must be qualified again; a matching file
name does not establish compatibility.

ROS gateway binaries, DDS discovery state, endpoint names and QoS values do not
define the VM's generated native instructions. They are bound and validated
through the current authenticated session and protocol. Hashing an entire ROS
installation into every Wasm object key would invalidate compatible code while
failing to replace those live authority checks.

Source path spelling, executable path spelling, current directory, log contents,
log timestamps and unrelated environment variables are not code identity.
An identical executable or guest copied to another path can reuse its object
when the registered module identity also stays the same. The CLI uses the input
path as the default module name; use `--wasm-set-main-module-name NAME` to keep
that import/permission namespace stable across source relocation. Changing the
namespace changes generated symbol names and is a semantic input, not merely
a file timestamp.
Permissions that violate private cache-storage policy are an authority refusal,
not a source-content change. The cache does not scan unrelated directories to
guess which previous deployment the user intended to compare.

Verbose diagnostics distinguish admission refusal, ordinary absence, I/O error,
compatibility mismatch and failed authentication. A cold lookup logs its current
source/executable/ISA/context/LLVM/ABI/policy fingerprints. Comparing those
fields with a previous launch identifies changed inputs without logging a
private key or inventing a cause when no previous entry exists.

There is no time-to-live for an otherwise valid object. The VM reads its signing
trust root only from the Linux UID keyring; it has no filesystem-secret
fallback. Without administrator provisioning, host reboot loses the ephemeral
key and can require recompilation even when source bytes are unchanged.
The optional [cache-origin helper](../../tools/cache-origin/README.md) restores
the same root from a root-only store before that UID's VM processes start.
It never replaces a different live key or exposes its store to the guest.
Two actual Linux boots must retain both the trusted root and the cache for
cross-reboot reuse. A lost, revoked or deliberately replaced root cannot
authenticate objects signed under the old root. Authentication is mandatory
in every mode; an unchanged source does not justify accepting an unknown signer.

Initial linear-memory data is instantiated from the current validated module,
not restored from the native object. A data-only change may reuse identical
machine code when it does not change emitted IR. The regression requires the
new data to be visible and independently changes an instruction so that reusing
the old object would execute the wrong trap condition.

The focused regression changes instruction bytes, executable bytes and cached
bytes independently. It also checks unchanged relocations, regular-file output,
read-only storage, revoked clock access and harmless pseudo-file refusal. These
tests provide evidence for those cases. They do not establish universal
correctness for every filesystem, kernel, CPU, loader or provider combination.
