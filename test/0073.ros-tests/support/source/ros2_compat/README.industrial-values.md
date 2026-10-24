# Independent installed industrial payload values

`industrial_value_oracle.py` generates an independent native value witness for
one explicitly selected complete ROS2 catalog. It supports original Messages,
both Service payloads and all three application Action payloads. It does not
start ROS, build a package, open a device, create an endpoint or grant an alias.
The generated program uses the actual production Native converters and actual
installed ROS serialization only when a separate owner compiles and runs it.

Use an original `uwvm-ros-vendor-complete-bounded-v1` preparation catalog and the
complete `uwvm-ros2-installed-protocol-idl-source-v1` capture produced by the
reviewed installed SDK parser. The IDL capture must contain every selected
interface, dependency and genuine SDK Action protocol association. The source
catalog supplies reviewed finite string and sequence bounds. The witness
refuses an absent dependency, unsupported scalar, unknown shape, duplicate
declaration, missing bound, oversized input, cyclic graph or existing output.
It never imports the production schema parser or codec generator.

```sh
python3 -I -S -B test/0073.ros-tests/support/source/ros2_compat/industrial_value_oracle.py \
  --catalog /absolute/original/catalog.preparation.json \
  --installed-idl /absolute/original/protocol-idl.json \
  --out /fresh/owned/native-values
```

The output contains `contract.cc` and `source-oracle.json`. The latter records
counted SHA256 identities for both original inputs, every independently computed
canonical payload and the exact emitted C++ source. Generation is source
preparation only. Independently preserve original definitions, source locks,
the actual native overlay, compiler/link inputs, complete dependency files,
executable bytes and the original stopped process's kernel mapping associations.

Select this output using `UWVM_PROTOCOL_VALUE_ORACLE` in the existing
`sdk/origin/robots/industrial/pre_rihs` native CMake project, with explicit
`UWVM_ROS2_BUILD_INSTALLED_SOURCE=ON`, `UWVM_INSTALLED_PROTOCOL_PROOF=ON`, the
actual `UWVM_NATIVE_PACKAGES`, original installed source proof, genuinely bound
generation and admitted fast_io headers. Its default is inactive. Run the
resulting `native-protocol-values --mapped-stop` only inside the authorized
cgroup. After all actual conversions and ROS serialization complete, it stops
itself. The independent owner must observe that original child and its mappings
before continuing it and supplying the single `I` byte on stdin.

For every registered payload, three deterministic scenarios fill every native
field, including nested values, fixed arrays, dynamic sequences, UTF-8 strings,
wide integers and signed floating zero. Dynamic scenarios use zero, one and at
most two elements; these are field-value witnesses, not an exhaustive Cartesian
search of every admissible capacity. Original constants and declared default
presence remain the responsibility of the separate installed-source contract.
The SDK's sole empty-message placeholder is excluded from canonical bytes while
remaining a real field of the genuine native object. An empty payload naturally
has repeated zero-byte scenarios.

Python `struct` computes canonical expected bytes. The native program checks
production encode/decode against those bytes and checks actual ROS CDR
serialization/deserialization against the original complete object. It also
feeds malformed lengths, booleans and UTF-8, plus truncation and trailing bytes,
into the production decoder and requires failure with the output object intact.
All assertions remain enabled by the native witness CMake project. fast_io
prints actual payload, scenario and refusal counts. A passed local witness
establishes those concrete conversions for that source/SDK/architecture; it
does not establish controller behavior, ROS network delivery, Wasm execution,
physical safety, an upstream official suite or a performance comparison.
