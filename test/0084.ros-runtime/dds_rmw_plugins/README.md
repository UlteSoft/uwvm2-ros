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
