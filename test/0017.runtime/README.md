# Runtime regression sources

This directory contains C++ component fixtures and Python CLI/debugger runners.
Keep their source, explicit case inputs and required native packet fixtures.
Build products and execution logs belong in a selected output directory.

## Source expression regressions

[The language expression guide](README.debug-source-language-expressions.md)
describes read-only expressions, copied values and stop/frame identity rules.
The `debug_source_*` C++ fixtures and `run_debug_source_*` runners exercise
producer metadata, scalar expressions, arrays, pointers, strings and variants.
DAP protocol tests and complete live-session runners are in
[the debugger tests](../0018.debugger/readme.md).

A copied value does not create guest-address authority. A reference from a
previous stop must be rejected after stepping, continuing or replacing code.
Producer, address width, stack policy and runtime product are separate inputs;
a previous execution result does not qualify changed inputs.

## Checkpoints and native controls

The `debug_checkpoint_*` fixtures cover capture, preparation, resource ownership
and restoration boundaries. See the stable checkpoint guides beside these
sources. The `debug_native_*` fixtures cover registered native windows, register
projection, bounded stepping and caller lifetime. Their component checks do not
replace a complete VM/JIT/DAP or OS-specific execution.

Obsolete revision reports and duplicate execution output have been retired.
Use current sources and produce fresh results for the selected inputs.
