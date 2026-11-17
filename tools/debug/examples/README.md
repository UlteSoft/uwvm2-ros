# WASIp1 debugger tour

`wasip1_tour.cpp` is a standalone Wasm32 C++ guest with explicit WASIp1 imports.
It needs an existing Clang WebAssembly target and linker; no guest libc or new
SDK installation is required. Compile with `-O0 -gdwarf-4 -nostdlib
-fno-exceptions -Wl,--no-entry -Wl,--export=_start -Wl,--export-memory` and
`--target=wasm32-unknown-unknown`.

Create a dedicated host directory containing only `input.txt`. Give module
`tour` its own WASIp1 single policy. Enable the 13 imported functions explicitly;
grant args, environ, stdout.write, random, and monotonic-clock. Set a fixed argv
and environment. Preopen only that directory as `/tour`, with base rights
24576 (path_open and fd_readdir) and inheriting rights 2 (fd_read). Global
permissions and unrelated host directories remain ungranted.

Set a source breakpoint on `phase = stage` inside `tour_checkpoint`.
The `stage` argument identifies six stops:

1. 0: enable WASIp1 recording in its Recorded calls page.
2. 1: inspect the configured arguments and environment, including linear-memory
   copies. Browse Source, Wasm, Memory, and WASIp1 using Back/Forward.
3. 2: inspect the read-only directory, input bytes, and closed child descriptor.
4. 3: inspect clock/random results, stdout, the loop and function calls. Continuing
   executes `fd_close(3)` to revoke the preopened directory capability.
5. 4: confirm no preopen or child descriptor remains. Only now remove the dedicated
   host `input.txt` and its empty directory. Do not delete any observed path merely
   because the debugger displayed it.
6. 5: inspect `last_error`, which must be WASI BADF (8). No file was reopened.

This is a guest capability close, not a Linux mount or privileged `umount`.
The guest does not delete host files. ROS debug builds deliberately reject
launch-environment mutation by the debugger. Restarting the tour requires
recreating its dedicated input directory before launching again.

Import the exact launch profile through `../import_ide.py`; see
[IDE import](../README.ide-import.md). Keep the profile, broker capability and
IDE workspace outside the preopened directory.
