# Threaded WASIp1 metadata qualification

`test_shared_prestat.py` executes 92 real CLI cases for `fd_prestat_get` and
`fd_prestat_dir_name`, with ordinary and admitted shared-memory execution,
using both interpreter and LLVM JIT. Interface hiding, denial before memory
access, actual preopen authority, absent/invalid descriptors, stdio grants,
zero file-operation rights, ABI padding, alignment, insufficient output space,
and a 5,001-byte virtual directory name are checked. A larger supplied output
capacity does not add a trailing NUL or overwrite its unused bytes.

`test_shared_prestat_memtag.py` uses the existing WasmMTE compiler to build
real C fixtures, retaining its generated shared-memory initialization start.
Its 32 output-span cases check accepted and rejected tags with execution
checking enabled and disabled, in both profiles and both backends. Bad tags
require exit 122 and the actual mismatch diagnostic. Two additional cases
retain the 64 KiB grant rejection for an actual four-page memory maximum;
they require the distinct admission diagnostic and are never counted as tag
traps. The positive shared cases explicitly grant the required 256 KiB.

Run with `--vm`, `--out` and `--wasm-tools`; the tag suite additionally needs
`--clang`. Reuse installed tools, run inside the resource cgroup, and place
all generated artifacts under `.tmp`. These tests do not claim that other
unadmitted shared-profile interfaces or concurrent close/retag races are covered.

`test_shared_fd_metadata.py` adds 160 actual import/call cases for FD status
and irreversible rights reduction. It checks full 64-bit rights, stdio origin
through renumbering, filesystem quotas (reject and synchronous wait), denied
interfaces before output access, the ABI output, and mixed-width argument
decoding. `test_shared_fdmeta_memtag.py` adds 32 tagged-span cases and two
separate shared-memory admission negatives. Its second-granule mismatch must
fail too; checking only the first address is insufficient.

`test_shared_fdmeta_socket.py` checks actual TCP/UDP preopens and reduced socket
rights in 16 cases. `test_shared_fdmeta_parallel.py` runs four actual two-Guest-
thread cases, repeatedly reading metadata while the other Guest reduces
rights; each snapshot must be entirely before or after the reduction. It uses
stable allocation tags and does not claim atomic check/retag/access safety or
TSan coverage. These runners take the same tool paths; the parallel C fixture
also requires `--clang`.

`test_wasi_metadata_padding.py` compares the complete 24-byte FD status and
64-byte file status against the ABI and authorized host metadata. Its 14 cases
cover ordinary WASI32/64 interpreter/JIT and the admitted shared FD status API.
All ABI padding must be zero. No raw host pointers are legitimate output.

`test_shared_filestat.py` adds 108 actual ordinary/shared import and call cases
for `fd_filestat_get`. Complete 64-byte results match the explicitly authorized
directory or input file, including zero padding. It covers denied output,
rights reduction, original stdio identity after renumbering, exact final-byte
fit, outputs after `memory.grow`, and full-span bounds failure. Quota wait is
measured inside the Guest with an explicitly granted monotonic clock.

`test_shared_filestat_socket.py` adds 80 actual TCP/UDP cases: socket metadata
needs its own right; read/write authority does not grant it. It checks discarded
rights, failed restoration, renumbering onto an existing explicitly granted
standard-stream target, filesystem-call quotas, and zero network payload.
`test_shared_filestat_memtag.py` adds 32 ordinary/shared tag cases plus two
separate memory-maximum admission negatives. The 64-byte span includes a
last-granule mismatch; disabled checks preserve the separate allocation state.

`test_shared_filestat_parallel.py` adds eight actual two-Guest cases with
rights reduction or renumbering concurrent with metadata reads and memory
growth. Every successful 64-byte result matches the initial authorized
resource snapshot; every rejected result leaves all 64 sentinel bytes intact.
Expected rights-denial diagnostics are matched separately. `--prepare-only`
can compile the two real C fixtures once; `--reuse-fixtures` requires their
source, arguments and binary SHA-256 to match. Stable tags are used. These
cases do not establish concurrent retag/access safety, other modules' WASI
call support, cross-process authority isolation, or new cross-platform
qualification.
