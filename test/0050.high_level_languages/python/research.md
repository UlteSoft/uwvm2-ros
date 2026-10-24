# CPython WASI producer contract

The fixture pins CPython 3.14.8 and WASI SDK 24.0. The CPython release archive
SHA-256 is `c2215904f02b175596dc49351585104f4bc20341e1c47378b26a2c274360ce73`.
The separately identified upstream tag commit is
`8e6e75d9102e39bed2a2b279203a396741180f12`; a Git checkout or autoarchive has its
own identity and must not reuse the release-archive hash.

This route compiles the CPython interpreter and its C runtime to core Wasm,
then executes Python bytecode in guest linear memory. The pinned build entry
is `Tools/wasm/wasi`, targeting `wasm32-wasip1`. The matching native build Python,
WASI compiler/sysroot and generated module imports are explicit producer inputs.
A bare Python command still needs interpreter initialization and the selected
standard-library deployment.

Use [build.py](build.py) and the stable debug producer scripts. Keep native,
optimized WASI and debug products separate. Record the exact entry/export,
imports, memory width and source/tool identities for each product. Unsupported
imports must remain explicit failures.

The stock CLI uses an explicit standard-library directory or a verified bundle.
A frozen launcher must include the complete dependency closure of its selected
modules, including delayed imports and required C builtins. A ZIP can supply
Python modules and resources; it does not provide native extension loading.
See [the SDK reuse guide](SDK-reuse.md) and
[the standard-library/ROS constraints](ros-stdlib-source-notes.md).

Wasm instruction and C-interpreter DWARF checks are separate from Python frame,
source-line and local-variable debugging. Preserve source/loader information
when a Python traceback or debugger promises original source text. Unicode,
empty arguments, embedded NUL handling, errors and invalid-input cases remain
explicit fixture data.

These are producer and interpretation requirements, not a claim that current
sources or all Python/ROS APIs have passed execution.
