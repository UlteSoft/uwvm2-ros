# Native ROS 2 Zenoh TCP round trip

This fixture starts the signed Jazzy `rmw_zenohd` binary and two native rclcpp
applications in a private loopback namespace. Each application selects
`rmw_zenoh_cpp`, loads its release's vendor library, and opens a TCP connection
to the router at 127.0.0.1:7447. The requester sends a fresh `std_msgs/msg/Int32`
challenge. The echo process validates that value and publishes its successor.
Each application returns success only after checking the exact value in its callback.
The evidence covers plaintext Zenoh on TCP and these two application topics.
The application exchange leaves graph interoperability, security profiles and scene VM
execution unverified.

The runner reuses the `-O3 -g` application binaries from a previously qualified native
DDS build. Its receipt binds the application sources and binaries and identifies
the sysroot and system library paths. This run launches new processes, changes their RMW selection and records
a new challenge and response. The normal DDS application settings belong to the
original receipt. [ROS 2 Zenoh RMW](https://github.com/ros2/rmw_zenoh)

Supply the original ROS Noble `InRelease` and amd64 `Packages.gz` in `$INDEX`,
along with the ROS archive keyring. The runner authenticates both upstream Zenoh
packages against the signed package index, stages their selected runtime files under
`$PACKAGES`, and records SHA-256 identities for the router executable, native
libraries, original configuration files and package license manifests.
Run the following through the repository's 64 GiB qualification cgroup and serial lock.

```sh
python3 -B run.py --baseline "$DDS_NATIVE_RESULTS" --index "$INDEX" --keyring "$KEYRING" \
  --packages "$PACKAGES" --output "$FRESH_RESULTS"
```

Use a Jazzy requester and a Kilted echo baseline for the cross-release case.
`--same-release` launches two separate instances of the Jazzy application for the
same-release control. The package preparer can also run separately.

```sh
python3 -B packages.py jazzy --index "$INDEX" --keyring "$KEYRING" --output "$PACKAGES/jazzy"
```

The negative control starts the identical applications in client mode with the router
absent. The routed case binds the router to IPv4 loopback, disables multicast,
peer gossip and shared memory, and connects both applications to its fixed address.
It checks the RMW identity in each application, samples the process library mappings,
and matches each application's TCP connection with a separate accepted socket owned
by the router. Each case retains its process exit statuses, output, diagnostic counts,
and hashes. `complete` means that both cases finished. `passed` requires the negative
control, both application callbacks, the process mappings and the paired TCP sockets.

The [qualification manifest](qualification.json) records the four ROS package URLs,
versions, SHA-256 digests, declared licenses and copyright file identities. Its two
case entries identify the verified receipts for the Jazzy control and the
Jazzy requester with a Kilted echo. Each receipt covers both router conditions.
