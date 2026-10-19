# Run the synchronous MoonBit SDK models

`tools/run_moonbit_models.py` compiles the actual SDK tests with an explicitly
selected pinned producer, validates each selected real Wasm artifact and invokes
`tools/run_moonbit_models.mjs` on the selected Node engine. It does not use
`moonrun`, UWVM, a Gateway, a ROS grant or a native ROS peer.

The tool itself passed on SSH Linux on 2026-10-19: the default profile ran66
models per target; `--include-owned-service` ran70 per target. Both profiles ran
`wasm` and `wasm-gc`, with zero ROS stub calls. This is 272 model executions,
including 140 distinct method/target combinations. The [retained evidence](../../test/0050.high_level_languages/results/native-budget/language-runtime-control-edges-20261019/README.md)
binds the source, compiler, real artifacts, generated test inventory, exact Node
frames and externally owned process cleanup. Historical predecessor tests do
not replace these current receipts.

## Select admitted tools and sources

Copy a coherent SDK containing `moonbit/`, `abi/abi-v1.json`, and the three
maintained files `tools/check_wasm_imports.py`, `tools/run_moonbit_models.py` and
`tools/run_moonbit_models.mjs`. No caller-supplied inspector or Host plug-in is
accepted. Exact neighboring reader and Node runner SHA256 values are:

- Reader: `327fccbd6bdcfae0ae8da593014f4fb4451747b1ee1c9991996242b798c9bfa4`.
- Node runner: `1f39d35bc20cbd8ad42ea93661cde30b38ce51a97a84cac4958fc18c85da641f`.
- Tested Python tool: `86b52d05a3ffc1aecb9875ad7f41a3b82eed045dc53601877c2b97ed94b7f88a`.

Use the actual0.1.20261012/0.10.15 producer, including its real `lib/core` and
existing bundles. Its executables are pinned independently:

- `moon`: `a31026b83410607909ebae469fa19e46c32201cfe403be6099e1da961a5ebf0f`.
- `moonc`: `fb950cbc125168c49523ef554d2649effd66499c596dafcb15c80b561d45fd8c`.

The tool installs nothing. It copies the complete selected core tree and the
exact `moon`/`moonc` executables into a fresh private tool home. Both producer
executables must resolve that private core. A bin-directory symlink to the
original installation is insufficient. Original tools, SDK and core are inputs;
complete bytes, extents and physical identities are reopened after execution.
Hashes identify bytes and do not authenticate toolchain provenance.

## Caller-owned execution

Replace every `ABS_*` placeholder with an admitted physical path. `SCRATCH`
must be fresh and outside the SDK, tool home and executable parents. Start this
command only inside an existing owner with finite aggregate memory, storage and
whole-stage time limits:

```sh
python3 -I -B ABS_SDK/tools/run_moonbit_models.py \
  --moon ABS_MOON_HOME/bin/moon \
  --moon-home ABS_MOON_HOME \
  --node ABS_NODE \
  --wasm-tools ABS_WASM_TOOLS \
  --scratch SCRATCH
```

Add `--include-owned-service` to select the fifth package and its four private
protocol models per target. The default build explicitly selects only `linear`,
`ros1`, `ros2` and `wire`; optional package errors cannot make an unselected
profile depend on that package. No registry update or download is requested.
This is not a network sandbox; deployment network isolation belongs to the owner.

The original `HOME` is retained. The child environment sets private `MOON_HOME`,
`XDG_CACHE_HOME`, `XDG_CONFIG_HOME`, `XDG_DATA_HOME` and `TMPDIR`, and omits ambient
Node, Python, compiler and loader overrides. Budget the entire source/core copy,
both build trees, raw logs and reports. A previously sampled RSS is not a peak
bound or a resource reservation.

Builds have180-second direct-child timeouts; validation/printing30 seconds and
model runs45 seconds. This developer tool owns direct subprocesses only. It does
not supply pidfd ancestry, a subreaper, unknown-descendant reaping or an aggregate
watchdog. The outer owner must provide full process-tree cleanup, including hard
cancellation and failed runs. Its proof is separate from inner `report.json`.
The accepted runs used the original existing 64GiB Linux cgroup with swap0.

## Exact inventory and execution gates

Builds use `moon test --frozen --build-only --target TARGET -j 1 --debug
--verbose --target-dir PRIVATE_OUTPUT` on a private source directory. They never
execute the default runner. The default profile yields eleven Wasm test modules
per target and selects four. The optional profile yields fourteen and selects
five. All products are retained; unselected products receive no model credit.

| Package | Test source | Mode | Models per target |
| --- | --- | --- | ---: |
| linear | linear_wbtest.mbt | whitebox | 14 |
| ros1 | client_wbtest.mbt | whitebox | 13 |
| ros2 | client_wbtest.mbt | whitebox | 13 |
| wire | wire_test.mbt | blackbox | 26 |
| owned_service, optional | protocol_contract_wbtest.mbt | whitebox | 4 |

The actual generated test-info must contain the complete count, contiguous
indices, exact source association and no skips/attributes. Async, arguments,
benchmarks, unexpected exports/imports and unadmitted tags fail admission.
Every selected artifact passes the independent full Wasm validator and the
maintained GC-aware import/type reader. These DCE model modules do not establish
complete81-function raw ABI coverage.

The Node runner exposes a closed set of bounded string/read/stdout hooks and
the admitted exception tag. File-named hooks read only the explicit test-name
string, never Host files. Every ROS stub throws on invocation. Each real method
must emit exactly its own start/result frames and an empty result message.
Compilation and artifact existence alone do not constitute test execution.

Node receives content identities as path/byte-extent/SHA256. Full nanosecond
physical metadata remains in Python and is rechecked there; it is not compared
after lossy conversion through JavaScript Number. The Node runner is unchanged.

## Actual memory layout

Each selected package reserves memory0, exported as `memory`, minimum256 and
maximum1024 pages. The linear heap starts at8519680; GC JS string builtins are
disabled. Active data must avoid `[0,16)`, `[32,289)` and `[65536,8454160)`.
The pinned linear producer's single `$tlsf/initialize` first constant must lie
at or above the heap floor and all active data. GC artifacts reject TLSF and
ordinary-memory initialization/growth instructions. Entry signatures, memory
flags, exception/start inventories and segment extents are checked in real
artifacts. This is a scoped pinned-producer anchor check, not a proof of every
possible allocator write or arbitrary future producer behavior.

## Reports and separate acceptance lanes

The report retains actual modules, test-info, manifests, outputs, command records,
original/copied tool and source inventories, and complete preservation checks.
A successful default run has 132 selected executions; the optional profile 140.
The observed ROS import count must be zero. Incomplete copy/admission or changed
inputs makes the entire result fail; an incomplete ROS-call count is unknown,
not an invented zero. Caller-owned retirement follows verified retention only.

Keep local Node models, actual UWVM Guest execution, enabled-policy rejection,
authorized native delivery and official client-library compatibility distinct.
The [progressive examples](../examples/moonbit/README.md) describe actual UWVM
baseline tests and the still-required native scenario gates. These local models
supply no native handles, grants, native ROS performance or debugger evidence.
