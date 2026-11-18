# Native Int32 relay across two DDS domains

This test reuses the two rclcpp executables and their library prefixes from
a completed Jazzy Fast DDS and Humble Cyclone DDS `dds_interop` run. The
runner verifies that receipt, both application binaries, generated Int32
type support and the original native library and source inputs. It compiles a
test-specific Cyclone DDS C observer/relay against the same type support.
Supply the original run directory with its two peer binaries and logs:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py /usr/bin/taskset -c 4-7 \
  /usr/bin/flock "$QUALIFICATION_LOCK" /usr/bin/python3 -B \
  test/0084.ros-runtime/dds_domain_relay/run.py \
  --baseline "$JAZZY_HUMBLE_RUN/results.json" --output "$NEW_DIR"
```

Jazzy sends a fresh positive Int32 on `/wros_dds_request` in domain 61.
Humble listens on that topic in domain 62 and computes its successor for
`/wros_dds_reply`. The native endpoints, topic names and challenge stay the
same across two fresh, loopback-only network namespaces.

* In the control, the observer creates four readers and zero writers. It must
  see the request in domain 61. Domain 62 must see no request, neither domain
  may see a reply and both applications must exhaust their normal deadlines.
* The relay adds a domain 62 request writer and a domain 61 reply writer. It
  copies only validated samples from each fixed source topic to its destination
  and never computes a response. The four readers check the exact challenge
  and successor on each side. The test requires observations of all four legs,
  successful writes in both directions, both peer completion messages and
  zero exit codes.

`results.json` records both peer identities, domain IDs, original executable
hashes, the relay build, receive and write counts, per-peer exits and timing,
and rcutils diagnostic counts with the full log digests. Counts can vary with
discovery and retransmission. This exercise covers plaintext reliable UDPv4,
one message type and two fixed topics. ROS graph compatibility, DDS Security,
other topics, services, actions, and production routing each require separate
evidence.
