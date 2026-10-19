# JavaScript preparation file observations

The real `sdk/javascript/guest/prepare.py` emits eight fields for every source
and preparation input: `path`, `bytes`, `sha256`, `device`, `inode`, `mtime_ns`,
`ctime_ns`, and the raw `os.stat_result.st_mode` as `mode`. The Unitree Frame's
ordinary file observation has the same seven identity/content fields without
`mode`. Direct dictionary equality therefore refuses a genuine prepared job.
The immutable Source58 build report keeps its original result; this correction
does not turn a preparation, failed build, or partially completed job into PASS.

`prepare_legacy_guests.javascript_prepared_file` now requires exactly the SDK
eight-field schema, actual integer identity fields and a regular-file raw mode.
It admits only physical names under the already selected current source,
owned output, or complete input roots. It opens with `O_NOFOLLOW`, `O_NONBLOCK`
and `O_CLOEXEC`, checks the 128 MiB extent before reading, and streams at most
one MiB per guarded read. The SHA256 and all seven fields must equal the original
SDK observation. The descriptor's raw mode must also equal the original mode.
The same descriptor and named file are checked again, including mode and link
count, after the ordinary Frame observation. A mode change cannot disappear
behind a projection to seven fields. Unknown fields are rejected.

Each compile command retains a `javascript_prepared_source` sidecar with the
unchanged `original_sdk_pin`, independently observed `current_file`, and
`descriptor_mode`. After all genuine compile/link/validator commands, every
original `metadata.inputs` row is reopened in the same way and retained in
`javascript_preparation_input_qualifications`. The original preparation report
and its eight-field records remain unchanged. The Frame's full7 ledger and
normal complete-root/input/source preservation still run. No input root,
permission, compiler argument, recipe, official termination object, validator,
or count of the 32 real build jobs changes.

The twelve source-authored policies in `test_javascript_prepared_pin.py` expose
only `suite(producer, guard, out)`. Root must load the producer and factory from
the same complete current manifest, supply an empty owned directory and the
actual original64GiB/swap0 guard, and run the returned suite under its normal
finite owner. Cases cover distinct raw8/current7 records, admitted source/output
and input roots, closed schemas, exact mode/identity/hash, unselected paths,
symlinks, allocation bounds, guard-time mutation, and mutation during the Frame
association. These are filesystem policy cases; no candidate import, test,
compiler, Guest, ROS or hardware execution occurred while authoring them.
