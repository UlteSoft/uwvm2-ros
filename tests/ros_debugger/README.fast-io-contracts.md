# Fast_io formatter contracts

The debugger and Unitree example formatters use the project's vendored fast_io.
`fast_io_console_tf_contract.cc` exercises memory rendering, the bounded console
stdin queue and canonical TF validation. `fast_io_tui_contract.cc` exercises the
actual TUI header and compares exact output bytes. Neither executable starts a
Runtime, Guest, Gateway, ROS node or terminal device, and neither grants a Wasm
capability. These fixtures do not replace real ROS or official upstream tests.

Build each translation unit with the actual qualified project compiler and its
current include roots, language mode, target and standard-library selections.
Fast_io requires C++20 or newer. Preserve those compiler arguments in the normal
build receipt; do not substitute a formatter stub or copy the production header
into a different test implementation. Link each fixture as its own executable.
Run it through the normal bounded native-command owner, retain both output
streams and require return code zero and `failures=0`. An authored assertion
count or a successful compilation is not a successful execution result.

Local build outputs and execution scratch must be under `/tmp`. Linux builds,
executables and their descendants must remain in the selected 64 GiB, zero-swap
task cgroup. Select serial compilation unless actual admission leaves room for
additional jobs; preserve the existing memory and physical-disk reserves.

The TUI fixture covers every single octet, exact and insufficient capacities,
valid and invalid UTF-8, atomic multibyte escapes, SGR preservation, rejection of
other terminal controls and the 32,768-byte styled-text boundary. Retained-frame
snapshots cover initial output, unchanged output, cursor movement, one-cell
deltas, clear, resize, invalidation, wide glyphs, combining marks and all four
style attributes, with color enabled and disabled. Its checks exercise actual
returned bytes rather than a terminal emulator's interpretation of them.

For UTF-16 memory rendering, one input byte is shorter than the minimum
two-byte code unit and must report `utf16=<needs 2 bytes>`. An odd extent after
a complete code unit reaches the Unicode length diagnostic. Both cases have
separate assertions. Preserve historical failed reports when correcting an
expectation; run the corrected source again before assigning a new result.

The related Unitree C++ examples format closed uint32 records into a 2,048-byte
automatic `fast_io::obuffer_view`. Their static bounds include all decimal digits,
separators and newlines. Their real generated-type builds and complete observer
byte checks remain part of the SDK/simulation lane, including GO20/HG35 carriers,
UINT32_MAX values, final PASS records and the existing short-write policy.

Reduced string construction and copying alone do not establish a speedup. Use
the actual native/Wasm matched workload and the hardware-only workflow in
[`DIRECT3_VTUNE.md`](../ros_unitree/DIRECT3_VTUNE.md). Retain hardware Hotspots
and Microarchitecture Exploration results and repeat the unprofiled comparison
after an optimization. A software-sampling fallback or QEMU timing is a different
measurement and cannot establish the physical host PMU result.
