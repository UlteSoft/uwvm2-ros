# One Wasm process observing Clearpath and Elephant Robotics

This C/C++ example combines the selected original Clearpath Noetic platform
Status and Elephant Noetic MycobotAngles interfaces. It reads state only:
each capability is opened/subscribed/polled once, then closed independently.
A refused first alias does not prevent the second alias's attempt or cleanup.
Both defaults have zero ROS authority. The program never starts native drivers,
creates policy aliases, sends motion commands, or controls updating/debugging.

## Prepare one coherent complete source domain

Run trusted preparation on the operator side. Select the exact installed
Noetic catalogs, merge their original declarations, and regenerate once:

~~~sh
UWVM_SOURCE=/absolute/uwvm2-ros
UWVM_PAIR_WORK=/absolute/fresh-pair-parent
mkdir "$UWVM_PAIR_WORK"
python3 -I -S -B "$UWVM_SOURCE/sdk/origin/robots/vendors/merge_catalogs.py" \
  --catalog "$UWVM_SOURCE/sdk/origin/robots/mobile_bases/clearpath/platform/catalogs/noetic/catalog.json" \
  --catalog "$UWVM_SOURCE/sdk/share/platform/elephant/catalogs/noetic/catalog.json" \
  --out "$UWVM_PAIR_WORK/union"
~~~

Use the existing sdk/origin/robots/industrial/prepare.py source-preparation command
with --source-root, the merged union/catalog.preparation.json, a fresh output
directory, and explicitly reviewed parent/bounded/adapter SHA256 arguments.
Its generated/ and input/catalog.json must belong to the same preparation.
The union retains 334 original definitions, including both original ROS1
clearpath_navigation_msgs/Mission.msg and Mission.action, and generates
534 complete records in all six record languages. Constants, registration
choices and bounds remain exact. Source preparation emits no Native identities.

## Build explicit C and C++ WASI commands

This subproject's option defaults OFF and produces no application until selected.
Use a reviewed WASI Preview1 toolchain and serial build:

~~~sh
cmake -S "$UWVM_SOURCE/sdk/test/0073.ros-tests/support/source/task06/vendor/elephant/multi_vendor" \
  -B "$UWVM_PAIR_WORK/build" \
  -DCMAKE_TOOLCHAIN_FILE=/absolute/wasi-sdk/share/cmake/wasi-sdk-p1.cmake \
  -DUWVM_BUILD_CLEARPATH_ELEPHANT=ON \
  -DUWVM_SOURCE_ROOT="$UWVM_SOURCE" \
  -DUWVM_MERGED_GENERATED="$UWVM_PAIR_WORK/prepared/generated" \
  -DUWVM_MERGED_CATALOG="$UWVM_PAIR_WORK/prepared/input/catalog.json"
cmake --build "$UWVM_PAIR_WORK/build" --parallel 1
~~~

The verifier checks every original definition/metadata pair against both selected
catalogs and every generated output against its generation receipt before build.
Do not put either standalone generated directory on the include path or combine
their generated headers. C++ uses each SDK's borrowed whole-value Reader and the
same regenerated complete interface domain.

## Execute with independent, explicitly reviewed aliases

~~~sh
uwvm2-ros -Rint --wasip1-global-noinherit-system-environment \
  --run "$UWVM_PAIR_WORK/build/clearpath_elephant_c.wasm" \
  state clearpath-status mycobot-angles
~~~

The other command is clearpath_elephant_cpp.wasm; raw attempts only both
opens, while state performs one subscription/poll after each successful open.
Aliases must be distinct nonempty policy names of at most 255 bytes, bound by
the operator to the exact original type/backend. Arguments are not addresses,
credentials or grants. Without operator grants both opens return permission
denied13. The process returns13 and writes sixteen bytes to stdout: four
little-endian uint32 statuses [13, 0, 13, 0]. Invalid mode, repeated aliases or
bad argument count returns22 before any open or status output.

On a permitted deployment, stdout still contains all four primary/cleanup
statuses. Primary results are never overwritten by cleanup results. Idle11,
storage90, malformed-record22 and transport failures remain visible; no retries,
allocation growth or mode recovery are implied. A successful data observation
or resource close is not physical safety confirmation or emergency stop.

Export/bind actual Native metadata for this exact union and rebuild the Runtime
and Gateway against that one coherent registry before positive ROS delivery.
Select only ros-vendor-clearpath=y and ros-vendor-elephant=y plus that reviewed
registry in xmake; keep all other vendor options OFF as described in
documents/ros/vendor-build-options.md. SDK compilation alone does not increase
Runtime authority, install native middleware, prove controller behavior, or
qualify other models/releases, official suites, simulation or performance.
