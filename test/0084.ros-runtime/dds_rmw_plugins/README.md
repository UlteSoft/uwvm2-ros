# Additional native ROS middleware plugins

The fixture runs two installed rclcpp executables in a fresh loopback network
namespace. The request uses the original Fast DDS plugin. The echo uses the
chosen dynamic Fast DDS or int2DDS plugin. Both peers check their RMW
identifier and the received Int32 value. The harness observes both plugin and
vendor library mappings while each peer is alive. The receipt records their
digests, the random challenge, both exit statuses, and any additional output
from the middleware, including participant teardown errors.

Use a completed two-rclcpp receipt from the native DDS interop fixture. Supply
a release-matched overlay containing the original RMW package and any
additional vendor libraries. Obtain the packages from a trusted source. Run
inside the existing qualification cgroup.

```sh
python3 -B test/0084.ros-runtime/dds_rmw_plugins/run.py \
  --baseline "$BASELINE/results.json" --overlay "$OVERLAY/opt/ros/humble" \
  --rmw rmw_int2dds_cpp --output "$FRESH_RESULT_DIR"
```

The exchange covers a reliable, plaintext Int32 topic. The test publishes no
Wasm permissions or vendor files. Graph compatibility, security plugins,
service mappings and production access through a Wasm provider each need
independent verification.

For an additional installed middleware, pass its exact RMW identifier and
`--local-library SONAME PLUGIN_SHA256 LIBRARY_SHA256`. Supply the hashes of
the original RMW plugin and the underlying vendor library separately. The
test rejects missing or mismatched inputs before starting either peer. Keep
the trusted distribution and licensed dependencies local. Both processes
share a disposable PID, IPC and network namespace for discovery. Their
mounts contain the system runtime, selected installations, pinned test inputs
and a fresh output directory. The operator remains responsible for selecting
a compatible RMW build and trusted dependencies, including transitive ones.

For Dashing OpenSplice, use the release's original rmw_opensplice_cpp,
rosidl_typesupport_opensplice_c and rosidl_typesupport_opensplice_cpp packages
with the OpenSplice 6.9 runtime. Place the vendor shared libraries under the
Dashing overlay's lib directory and the original configuration schema at
etc/ospl_metaconfig.xml. Use the two-rclcpp Dashing baseline and select
`--rmw rmw_opensplice_cpp`. The bundled XML selects private IPv4 loopback,
unicast discovery and domain 61. The results bind the test XML and vendor
schema digests. OpenSplice can write diagnostics during participant teardown.
A completed result requires exactly one success line from each process and
retains the surrounding diagnostic lines.
