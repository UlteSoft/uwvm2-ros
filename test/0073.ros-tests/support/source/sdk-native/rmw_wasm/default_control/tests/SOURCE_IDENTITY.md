# Explicit source identity for the control policy contracts

`test_source_profile.py` no longer executes a generator as an import-time side
effect or asks SourceFileLoader to select source versus cached bytecode. Its
standalone entry requires actual reviewed SHA-paired parent and bounded source:

```text
python3 /absolute/reviewed/test/0073.ros-tests/support/source/sdk-native/rmw_wasm/default_control/tests/test_source_profile.py --sdk /absolute/reviewed --parent-sha256 ACTUAL_PARENT_FULL_SHA256 --bounded-sha256 ACTUAL_BOUNDED_FULL_SHA256 --scratch /tmp/private-existing-scratch
```

These symbolic arguments describe a future command; no command was run. Root
must replace both digests using the exact selected source snapshots and run
under a separately finite process owner. Scratch must be outside the SDK. Both
actual source files are opened nonblocking/no-follow/close-on-exec, must be
regular and within1MiB, and remain the same complete descriptor/named identity
through capture, direct `compile(bytes, real_filename, 'exec', dont_inherit=True)`
and ModuleType execution. The exact module enters sys.modules before execution
for genuine dataclass/future-annotation resolution. A mismatch or exception
restores its prior binding. Neither loader reads or writes the selected pyc.
Later parent provenance reads must still match the captured source SHA/identity.
Standard-library bytecode remains the separate actual interpreter closure.

All20 original test methods remain. Their parser/schema/default/constants/
identity/business assertions are unchanged, except the current complete control
product set is corrected from historical six to the actual eight outputs:
`bounded_records.h`, `bounded_records.hpp`, `bounded_interfaces.h`,
`bounded_interfaces.hpp`, `control-schema.json`, `generation.json`,
`control_wire.hpp`, and `control_manifest.hpp`. The last two are actual existing
raw/aggregate/common-control outputs; this successor does not invent products
or emit them. Both temporary-generation scopes use only explicit scratch.

The four fixed-source capture helper definitions match the parent and the
source-domain runner. Source proof hashes necessarily change honestly; old
HOLDs/receipts and old source observations stay historical and are not retagged.
This is an authored source policy update with all test/generator/Native/Guest
execution counts zero. It neither changes the Runtime nor grants ROS rights.
