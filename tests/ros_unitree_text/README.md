# Closed Unitree text source contract

This narrow source successor removes the remaining per-record 2,048-byte
`std::string::reserve` from the C++ Go2 and GO/HG hold examples' `line` helper.
Every STEP and idle AGAIN previously created that string; the actual caller can
poll up to4,096 times. The already reviewed STATE carrier and C Go2 output
formatter are byte-identical and still use their existing fast_io stack views.
No existing Source65 result, VTune evidence, binary or generated schema is
retagged. This revision has no measured timing/RSS claim.

The actual formatter now accepts only character arrays and `uint32_t` words,
with a128-byte owned inline record. The real fast_io integer reserve protocol
and literal extents prove the complete closed call fits before formatting.
Formatting remains entirely fast_io; no custom decimal/float formatter, dynamic
string, global mutable stream or dangling string view is introduced. A returned
record owns its bytes and length. The original `basic_io.h` still performs the
same bounded WASIp1 write/short-write/error handling. There is no new import,
permission, syscall batching assumption or retry of a ROS operation. Independent
STATE/STEP record ordering is unchanged; no partial-write success is invented.

The ordinary C++ contract directly includes this production formatter and the
actual historical fast_io string path. It compares every marker with uint32
zero/max and finite counts, exercises all4,096 idle marker calls, rejects dynamic
or wrong-width types at compile-time, verifies returned/copied ownership, and
refuses ordinary C++ allocation while formatting the new record. These are
local formatter/source facts; no Native, generated type, SDK ABI or Guest/ROS
qualification is modeled. All source contracts and compilers remain UNEXECUTED.

After the whole source cohort has been independently admitted, the caller may
use its actual selected C++20 compiler in a fresh private `/tmp` source/build
copy, with explicit serial CPU/process/memory/cgroup limits:

```sh
cmake -S "$SOURCE/tests/ros_unitree_text" -B "$FRESH_BUILD" \
  -DUWVM_SOURCE_ROOT="$SOURCE" -DUWVM_BUILD_UNITREE_TEXT_SOURCE_CONTRACT=ON \
  -DCMAKE_CXX_COMPILER="$ACTUAL_CXX"
cmake --build "$FRESH_BUILD" --target unitree_text_contract --parallel 1
# Only an explicitly admitted later controller runs the actual local contract.
```

Default OFF creates no contract target, generation or run. The real Direct
producer already compiles source files from their original complete source
paths and captures full-MD includes; its existing C/C++20 flags/import whitelist
are unchanged. A fresh original-source build must retain the new header's real
MD pin. Historical source manifests/binaries must remain historical. Other
Unitree language examples use their own typed-buffer/SDK paths and receive no
formatter or authority change here.
