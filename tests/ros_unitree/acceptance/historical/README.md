# Immutable source inputs for the typed-command continuation

The `.py.txt` files are exact historical **source data**, not execution
entrypoints. Their original cgroup identifiers, old artifact locations and
historical observations must never be used for current admission. The source
input manifest records complete byte counts and SHA256 digests. No executable,
SDK or test result is reconstructed from a guessed identifier.

`command14-continuation-baa933.py.txt` is the actual recovered continuation
whose metadata consumer failed before any compiler job. It incorrectly read
two absent historical members. The maintained
`../derive_typed_command_continuation.py` documents and derives their repairs.

`command14-schema-fixed-0693fb.py.txt` preserves the exact reviewed intermediate
source, including its original source-authoring provenance comment. It is a
factory input so future regeneration does not depend on a vanished `/tmp`
file or the source author's absolute checkout location. The historical source
itself is not a successful build or a current-incarnation executable.

`official-cpp-qualifier-91420b.py.txt` and
`command14-utilities-a7e892.py.txt` preserve the exact original consumer and
build utilities. The postboot factory derives a new consumer with explicit
historical/current comparisons; it does not rewrite the original producer or
its reports. Utilities remain byte-for-byte unchanged.

For source authoring only, generate a fresh candidate with:

```text
python3 -B ../derive_typed_command_postboot.py --out /tmp/FRESH_TYPED_SOURCE
```

Root must then review the emitted source hashes, use the separate stage tool,
admit all actual current physical inputs and invoke preparation and compilation
as distinct guarded phases. Generation, staging and metadata regressions do
not qualify a compiled product, a Wasm instance, native ROS or performance.
