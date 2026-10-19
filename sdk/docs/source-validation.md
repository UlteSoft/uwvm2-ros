# SDK source contracts and intentional metadata updates

The maintained [source aggregate](../tools/verify_sources.py) runs bounded
source and pure-model checks before language compilation. A successful source
report does not establish that a compiler emitted valid Wasm, that a mock host
is a real Runtime, or that ROS behavior passed. The guarded unified SDK runner
performs its separate compiler, full-binary-validation and finite-host stages.
All ROS aliases and optional resources remain denied unless explicitly granted.

## Keep the Host binding exact

[ABI metadata](../abi/abi-v1.json) binds the complete Runtime Host source file,
in addition to the ordered ROS1/ROS2 registry names, integer widths, parameter
forms, descriptions and return contracts. An implementation-only Host edit can
therefore invalidate the metadata without changing a public import signature.
This is deliberate: an old whole-file hash must never silently certify new
Host implementation bytes.

Once the Host implementation is held for review, update metadata intentionally:

```sh
python3 -B sdk/tools/abi_contract.py --write
```

Review the complete JSON diff before accepting it. For an implementation-only
change, the expected difference is exactly `authority_source.sha256`; the
function inventories, names, forms, widths and all other metadata stay equal.
If those fields differ, review the real ABI change and every affected raw
language declaration instead of bypassing the checks. Neither
[the extractor](../tools/abi_contract.py) nor
[the binding checker](../tools/check_bindings.py) accepts a stale source hash.
Do not regenerate public raw bindings merely to hide an unrelated metadata
failure.

The current management-lifecycle update retains all 38 ROS1 and 43 ROS2 public
imports. Its metadata authoring changed only the authority-source hash after
independently comparing each ordered registered function, canonical form,
parameter width/name, result and description. That authoring operation is
neither a source-check result nor a compiler/Runtime/ROS test result. The next
source-bound Linux validation must still run both strict tools in check mode.

## Validate the actual TinyGo constraint

Each raw backend file and unified-event backend adapter uses:

```go
//go:build tinygo && (wasm || tinygo.wasm) && ros1 && !ros2
```

The ROS2 file substitutes `ros2 && !ros1`. The `wasm` architecture tag and
TinyGo's actual `tinygo.wasm` pure-target tag are supported alternatives. They
do not imply each other, and neither selects a ROS backend. Adding
`-tags=wasm` to a different architecture target can select conflicting standard
library files; it is not a substitute for the target's supplied platform tag.

[The source-constraint helper](../tools/go_build_constraints.py) parses the
complete expression with Go operator precedence. It accepts only the five
known tags, bounded ASCII syntax and one first-line directive followed by a
blank separator. It checks all 32 assignments against the exact contract:
TinyGo, at least one supported Wasm tag, the selected backend and absence of
the other backend. A matching substring cannot prove this contract. The
checker rejects extra OR branches, missing platform/compiler guards, unknown
tags, both/neither backends and loss of either supported target alternative.
Equivalent operand order and parentheses remain valid. A future supported tag
requires an explicit reviewed policy change and new negative models.

The [11-method source model suite](../tests/go_build_constraint_models.py)
includes the actual two raw files and two event adapters, equivalent
expressions, backend/platform bypasses, malformed/unknown syntax, duplicate or
legacy directives, and finite token/depth/source bounds. These are Boolean
source models, not executions of TinyGo or Go build selection.

## Run the reviewed checks in the guarded environment

The following shapes are intended for the existing Linux owner with its
explicitly selected Python/Node tools, fresh owned output directory and memory
guards. Authoring these commands or model sources does not execute them:

```sh
python3 -B sdk/tests/go_build_constraint_models.py
python3 -B sdk/tools/abi_contract.py
python3 -B sdk/tools/check_bindings.py
python3 -B sdk/rust/tests/check_sources.py
python3 -B sdk/tools/verify_sources.py --node /actual/selected/node \
  --report /actual/fresh/owned-output/sdk-source-report.json
```

The aggregate now has 19 child command entries: eleven Python source/model
checks, including the constraint suite, and eight JavaScript models. Its
successful status remains `SOURCE_AND_MODEL_PASS`. The unified SDK factory
requires that status and an unchanged source snapshot rather than assuming an
old command count. Compiler recipes, the 32 native/pure-model row IDs and the
separate Rust Wasm event fixture are unchanged by this source-check repair.
