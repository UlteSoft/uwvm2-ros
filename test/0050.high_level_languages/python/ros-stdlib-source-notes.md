# CPython standard-library and ROS fixture constraints

Use the pinned CPython 3.14.8 producer described in [research.md](research.md).
Standard-library imports, object construction and real operations are separate
checks. A frozen package entry alone does not establish its complete dependency
closure or runtime support.

| Module | Dependencies and operations to check |
| --- | --- |
| `dataclasses`, `typing` | Annotation evaluation, generated methods, `annotationlib`, delayed imports and required C builtins |
| `weakref`, `collections` | Actual callbacks, GC, finalization, deque operations and strong ownership of callbacks |
| `queue`, `asyncio` | Thread/wait/socket/process dependencies and the exact supported single-thread scheduling contract |
| `traceback` | Original filename, line, loader/source text and Python locals, separately from C-interpreter DWARF |
| `json` | Codec/regex closure, complete UTF-8 and invalid-input behavior, integer boundaries and exact output |

Keep the official single-thread stubs needed by otherwise valid stdlib paths.
Do not infer native guest-thread or complete asyncio support from an import or a
custom polling loop. Native DDS/runtime threads are separate from guest Python
threads. Blocking callbacks, callback order, fairness, cancellation, reentrancy,
exceptions and destruction require explicit tests.

The ROS bridge follows the selected published numeric ABI and C headers.
Memory index is i32; offset/length/handle fields are i64; output slots use the
specified little-endian representation. Zero-extend wasm32 pointers before
widening. Guest object addresses are not wire buffers. Test ownership, byte
lengths, embedded NUL, UTF-8 and borrowed-buffer lifetime independently.

Keep explicit close and failure-path cleanup for transport resources and
callbacks; `weakref.finalize` is auxiliary. Default-deny authority remains in
force. Copying upstream Python source does not create its native extension,
dynamic loader, libffi/ctypes or DDS implementation.

Use the stable Python contract and native integration sources for actual
validation. New results must identify the selected source, producer, runtime,
grants and operations; this document carries no historical pass counts.
