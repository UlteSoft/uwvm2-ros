# ROS 2 pre-release Linux probes

The Alpha 1–8 and Beta 1 archives contain native `talker` and `listener` programs for the `/chatter` topic and the `std_msgs/String` type. Each release has three separate observations. The first compares its original publisher and subscriber. The second compares the original publisher with a ROS 2 Jazzy Fast DDS subscriber. The third sends a fresh Jazzy payload to the original subscriber.

## Extract a release

On x86-64 Linux, use Python 3.11 or newer and GNU readelf. Select any release listed in `archives.json`. The tool checks both the size and SHA-256 of the compressed archive, reads up to 896 MiB of decompressed tar data in memory, and writes up to 112 MiB of ELF members into a fresh directory. The receipt records member digests and external library names. Use a single release directory at a time to bound local storage.

```sh
python3 -B extract.py alpha1 --output /work/alpha1
```

The source URLs, asset IDs and byte counts come from the upstream GitHub release records. SHA-256 values were measured on the downloaded bytes. GitHub provided no digest for these historical assets. For Alpha 7 the fixture selects the revised Fast RTPS Linux package ending in `-1.tar.bz2`. Alpha 8 and Beta 1 use their Fast RTPS Linux packages. [Upstream release archive](https://github.com/ros2/ros2/releases)

Alpha 1–6 link to OpenSplice. The OSRF Trusty repository provides `libopensplice64` version `6.4.0-3~trusty`. Verify the signed repository index with fingerprint `D2486D2DD83DB69272AFE98867170598AF249743`. The amd64 package SHA-256 is `8c2051599931be50ea8d11ce1f7f4d77b5e7cf36c9d9756e5b5995878cc26d5b`. Extract it into a private runtime prefix with `dpkg-deb -x`. The 6.7 runtime from Xenial failed to resolve `DDS::TypeSupportFactory` for the Trusty binaries. [Release maintainer's instructions](https://discourse.openrobotics.org/t/ros2-alpha6-install-problem/2389), [OSRF Trusty index](https://packages.osrfoundation.org/gazebo/ubuntu/dists/trusty/InRelease)

For the Fast RTPS builds, install or privately extract the Ubuntu Xenial amd64 Boost 1.58 shared libraries for chrono, date-time, system and thread. Beta 1 also loads Poco Foundation 9 and its dependencies. The Beta 1 dispatcher also loads the C++ introspection module for each message package dynamically. The extractor includes each of those original modules and its linked dependencies. The closure receipt lists the original ELF SONAMEs for each release. [Beta 1 dispatch source](https://github.com/ros2/rosidl_typesupport/blob/release-beta1/rosidl_typesupport_cpp/src/type_support_dispatch.cpp) [Ubuntu Xenial signed index](https://archive.ubuntu.com/ubuntu/dists/xenial/InRelease)

## Observe actual messages

Build the optional Jazzy witness from this directory in a sourced ROS 2 Jazzy development environment.

```sh
cmake -S . -B /work/chatter-build -DCMAKE_CXX_FLAGS="-O3 -g"
cmake --build /work/chatter-build --target jazzy_chatter
```

Use a Linux VM with no NIC or a private network namespace with only loopback. Enable multicast on the interface and configure its IPv4 address. Match the OpenSplice and Jazzy discovery XML to that interface. The following commands run inside an operator-created user and network namespace with permission to configure its own loopback.

```sh
ip link set lo up
ip address add 192.0.2.1/32 dev lo
ip link set lo multicast on
ip route add 224.0.0.0/4 dev lo src 192.0.2.1
```

Some early Fast RTPS builds attempt to join the same multicast group twice when loopback has two IPv4 addresses. The single-address trial also runs `ip -4 address del 127.0.0.1/8 dev lo` in the private namespace. Create the matching discovery profiles from the upstream OpenSplice template and the repository's existing Fast DDS loopback fixture.

```sh
python3 -B profiles.py --address 192.0.2.1 \
  --opensplice-template /work/opensplice/usr/etc/opensplice/config/ospl_sp_ddsi.xml \
  --output /work/discovery
```

Give each library environment its own directory. The observer supplies the selected closure to the original processes and the Jazzy prefix to the witness. For a native-only observation omit the Jazzy arguments. Use a fresh output directory for each interface setup and release.

```sh
python3 -B observe.py --closure /work/alpha1 --output /work/alpha1-results \
  --legacy-lib /work/opensplice/usr/lib --legacy-lib /work/xenial-libs \
  --opensplice-home /work/opensplice/usr --opensplice-xml /work/discovery/ospl.xml \
  --jazzy-prefix /opt/ros/jazzy --jazzy-witness /work/chatter-build/jazzy_chatter \
  --jazzy-lib /work/jazzy-libs --fastdds-xml /work/discovery/fastdds.xml
```

The observer captures each process on a separate terminal for up to ten seconds, records loaded middleware images, and checks application data. Native and legacy-to-Jazzy success require at least three distinct matching numbered strings. Jazzy-to-legacy success requires the original subscriber to print that run's unpredictable payload and the witness to report ten publications. All successes require the corresponding RMW image to have been observed in each process. The JSON records failures and the raw process logs explain the observed behavior.

## Measured results

Counts are distinct matching application payloads in the recorded single-address namespace trial. Each direction used a fresh process pair. `observations.json` includes the first 1,536 bytes of each process log, full log sizes, executable and full output hashes, matched samples, and middleware identities for each case.

| Linux release | Original pair | Original publisher → Jazzy | Jazzy publisher → original |
|---|---|---|---|
| alpha1 | pass (18) | none observed | none observed |
| alpha2 | pass (18) | none observed | none observed |
| alpha3 | pass (18) | none observed | none observed |
| alpha4 | pass (18) | none observed | none observed |
| alpha5 | pass (18) | none observed | none observed |
| alpha6 | pass (18) | none observed | none observed |
| alpha7 | pass (21) | none observed | none observed |
| alpha8 | pass (20) | none observed | none observed |
| beta1 | pass (21) | none observed | none observed |

The Alpha 1–6 OpenSplice diagnostics each logged 140 malformed packets from vendor 1.15 during original-to-Jazzy observation and 34 in the reverse direction. The application observers recorded zero matching strings in those runs. The logs show that traffic reached the legacy transport.

The Beta 1 run includes the original message introspection libraries for builtin_interfaces, rcl_interfaces, and std_msgs. The initial extraction contained only DT_NEEDED dependencies. Its original publisher terminated during type support selection. With the three original modules present, the original pair exchanged the numbered payloads recorded above.

A separate Jazzy control used the same executable, Fast DDS discovery profile, and single-address loopback configuration. Three Jazzy writers each sent ten copies of a distinct random payload. The Jazzy subscriber received all three payloads.

This probe covers native x86-64 Linux release programs and a single ROS message type and topic. Tests used private paths for the historical vendor libraries and a current host Linux runtime. A zero count denotes the observed result within the configured window.
