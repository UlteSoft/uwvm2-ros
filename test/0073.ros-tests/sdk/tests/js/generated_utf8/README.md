# Generated JavaScript UTF-8 regression

This fixture executes the shared SDK generator's bounded and original-control
record codecs inside the real QuickJS Wasm engine. Its synthetic string record
has no native ROS identity. It is a codec/emitter test, not a KUKA service test.

Run preparation, Xmake and the VM inside the established Linux 64 GiB, zero-swap
cgroup while holding the common test lease. `prepare.py` copies a bounded input
closure into a new output directory and preserves the installed SDK. It records
complete source/engine hashes. Select an unused output directory for each width:

```sh
python3 -B prepare.py --sdk-source "$SDK_ROOT" --output "$CASE_ROOT"
export UWVM_ROS_SDK_ROOT="$CASE_ROOT/sdk"
cd "$CASE_ROOT/project"
xmake f -c -p wasm-wasip1 -a wasm32
xmake
```

Repeat with another fresh `CASE_ROOT` and `-a wasm64`. Each project builds one
application `.wasm`. Use the actual source-paired uwvm2-ros executable with AOT,
checked native unwind and explicit standard-stream authority:

```sh
"$UWVM_ROS" -Raot -Rct 0 -Rllvm-cache-path disable \
  -Rllvm-call-stack unwind --runtime-compiler-log err \
  --wasm-set-main-module-name main \
  --wasip1-global-noinherit-system-environment \
  --wasip1-single-create main --wasip1-single-enable main \
  --wasip1-single-noinherit-system-environment main \
  --wasip1-single-grant main stdio-full-rights --run "$WASM_FILE"
```

For wasm64, add `--wasm-feature-enable-memory64` and
`--wasm-feature-enable-table64` before `--run`. The caller selects `WASM_FILE`
from the project's sole application output and records its complete hash.
No gateway, native ROS peer or implicit permission grant is used.

Expected stdout is exactly:

```text
PASS portable-generated-utf8 profiles=2 positive=12 negative=32
```

The twelve positive cases check exact independently specified wire bytes,
round trips and validation for ASCII, multibyte scalars, supplementary-plane
characters, the leading BOM, empty strings and the exact byte bound. The
thirty-two negative cases check embedded NUL, isolated/incorrect surrogate
pairs, oversized values, malformed or truncated UTF-8 and inconsistent wire
lengths. Both profiles retain their specified error class; original-control
also requires error code 22. Success requires a zero exit and compiler evidence
of `call_stack=unwind` and `unwind_check=live`.

The original generator failed in the real guest because QuickJS has no browser
`TextEncoder` global. The repaired generator imports the single portable
`ros/js/wire` implementation. Existing generated catalogs need their own
regeneration and execution checks; this fixture does not qualify them all.
