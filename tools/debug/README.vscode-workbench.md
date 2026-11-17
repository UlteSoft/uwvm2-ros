# Optional native VS Code workbench repair

This repair addresses two presentation problems while retaining VS Code's
native source editor, real DAP call stack, and native breakpoint composition:

- Editing a WATCH expression or variable hides its type label together with
  its name and value. Enter, Escape, blur, and disposal restore the label.
- A UWVM source frame with authenticated `uwvmSourceExact: false` uses VS
  Code's native green arrow and green line highlight. An exact top frame
  remains yellow. Switching Source/Wasm/Assembly without executing does not
  change that precision. Actual caller frames retain their native rendering.

The extension does not patch VS Code automatically. Stock VS Code does not
consume `uwvmSourceExact`: its source marker remains yellow for the top frame.
The repair changes the workbench's source decoration choice, without adding
frames, changing their order, or changing their source coordinates. Native
Disassembly is unaffected; its current JIT instruction remains exact.
Other debugger types do not consume the UWVM field.

## Verified build and limits

The helper accepts only VS Code 1.119.0, commit
`8b640eef5a6c6089c029249d48efa5c99adf7d51`, with the exact verified workbench
SHA-256. Actual GUI verification was performed on Linux x86_64. This does not
establish Windows, macOS, other builds, or other IDE support. A version or
byte mismatch is rejected; after an editor upgrade, a new repair must be
reviewed and tested rather than forcing these replacement anchors.

Changing the workbench can trigger VS Code's installation-integrity warning.
This helper does not alter integrity manifests, suppress warnings, bypass
signatures, or change editor security settings. Prefer a separate copy of the
existing editor for evaluation. This is an optional editor repair, not a VM
requirement or a claim that the extension alone fixes the native editor.

## Prepare, apply, and restore

The `--app-root` is the directory containing VS Code's `package.json` and
`product.json`, commonly `resources/app`. The bundle directory must be new,
outside the application, with an existing parent. Preparation changes no
application files and saves both original and repaired bytes plus a manifest.

```sh
python3 tools/debug/vscode_workbench_compat.py prepare \
  --app-root /usr/share/code/resources/app \
  --bundle /absolute/review-directory/workbench-repair
```

After reviewing the bundle, close windows using the chosen editor copy. The
following operation is explicit and requires write access to that copy:

```sh
python3 tools/debug/vscode_workbench_compat.py apply \
  --app-root /absolute/editor-copy/resources/app \
  --bundle /absolute/review-directory/workbench-repair
```

Restart the chosen editor to load the change. To restore it, close its windows
and use the same bundle and application root:

```sh
python3 tools/debug/vscode_workbench_compat.py restore \
  --app-root /absolute/editor-copy/resources/app \
  --bundle /absolute/review-directory/workbench-repair
```

Both operations verify the current file and both bundle files before making
an atomic replacement. Unknown builds, altered bundles, repeated application,
unexpected current contents, and symbolic-link targets are refused. The
original bytes, including line endings, are preserved for exact restoration.

## Verification

Run tests with the project's existing resource-limited test launcher and a
temporary directory under `.tmp`. `UWVM_TEST_VSCODE_APP` enables an additional
round-trip check using the installed build, copied into an isolated fixture;
the test never applies the repair to that installation.

```sh
UWVM_TEST_VSCODE_APP=/usr/share/code/resources/app \
  python3 -B -m unittest discover -s test/0018.debugger \
  -p test_vscode_workbench_compat.py -v
node test/0018.debugger/test_vscode_workbench_behavior.cjs \
  /absolute/review-directory/workbench-repair/repaired.js
```

GUI acceptance includes WATCH expression commit/cancel with types preserved,
exact and in-statement source positions, level switches without execution,
enabled/disabled/deleted breakpoints, one source gutter marker and one line
highlight, and WASI inspector continuity across native-stop transitions.
