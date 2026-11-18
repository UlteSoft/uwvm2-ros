# Fixed String relay across two SROS 2 trust domains

This native Linux test joins a Jazzy Fast DDS application and a Lyrical
Cyclone DDS application through a pair of test gateways. The Fast DDS side
uses ROS domain 74. The Cyclone side uses domain 75. Each side runs in a
separate loopback-only network and PID namespace with its own SROS 2 identity
and permissions authorities. Neither network has a route to the other.
Jazzy's SROS 2 tool issues and signs the identities and policies for each run.

Each gateway can read and write one application topic, `/uwvm_test/relay`,
of type `std_msgs/msg/String` in its local domain. The signed governance
requires authenticated participants and encryption of topic data and metadata.
The Jazzy nodes also have local read permission for the rclcpp parameter-event
reader created at startup. The relay code accepts a 33-byte request or a
65-byte correlated reply. It checks the direction, hexadecimal format and
length before forwarding a message. The standard ROS String type remains
unbounded during middleware deserialization. Each execution carries one exchange.
The two applications verify independent, freshly generated challenge and reply
values. Gateways observe the plaintext after DDS or TLS authentication and
republish it under their own identities. This trust boundary requires both
gateways to be trusted.

The gateways use OpenSSL TLS 1.3 over inherited Unix stream sockets. Both
endpoints require a valid peer certificate and an exact DNS subject alternative
name issued by the dedicated relay CA. The test transport forwards the opaque
bytes between those sockets to record counts and digests from both directions.
The fixture opens no network listener. A Landlock read policy limits each native
process to its own DDS enclave credentials. Only the gateways can read a TLS
identity. Authority signing keys stay in ephemeral host storage.

The runner compares four conditions. Without the gateways both native
applications must start under Enforce and time out without a matching callback.
With an untrusted client certificate or an untrusted server certificate,
the opposite gateway must report a verification failure and both applications
must time out. A rejected handshake can emit a TLS alert before application
traffic keys are in use. With both trusted gateways,
every native process must exit successfully. Both applications and both
gateways must confirm the exact strings. Both gateways must record the negotiated
TLS cipher and verified peer name. Each DDS leg also requires AES-GCM RTPS
submessages in both directions at the receiving process's own UDP port. The
receipt checks for plaintext markers in the successful DDS sends and the TLS
record streams. The original direct mixed-vendor result is a separate measurement.

Run the fixture under the shared 64 GiB qualification cgroup and serial lock.
Pass the `sros2_cross --cyclone-echo` result that identifies the native
packages and working Cyclone Enforce control. Its original sysroots must
remain accessible. The host needs OpenSSL development files, clang++-22,
strace, bubblewrap, Landlock-enabled setpriv, and the Jazzy SROS 2 issuer
rootfs. Choose a fresh result directory.

```sh
python3 -B run.py --baseline "$MIXED_DDS_RESULTS" \
  --issuer-rootfs "$JAZZY_ROOTFS" --output "$FRESH_RESULTS"
```

## Observed run

Jazzy Fast DDS 2.14.6 and Lyrical Cyclone DDS 11.0.1 passed all four cases.
Both gateways negotiated TLS_AES_256_GCM_SHA384 and verified the opposite
certificate name. The Fast DDS application sent 23 encrypted submessages to
its gateway and the gateway sent 12 to the application. The corresponding
Cyclone counts were 17 and 23. All four callbacks confirmed their expected
request or reply. Cyclone also logged one remote secure participant refusal
during startup. The result SHA-256 is
`5d1077a96e6b86631c5933b1f84f7307ba7469f8efa2e67024b8ba3a483a1258`.

The first policy omitted Jazzy's read permission for parameter events.
Fast DDS refused that reader at startup. Its result SHA-256 is
`4280d4fa2c64e849281074a7cb21ee7bb769689be58f25984017f7808c197ec0`.
An intermediate receipt classified the correctly rejected server certificate
as failed because it expected encrypted records from a client that had already
sent a TLS alert. Its SHA-256 is
`4416a57a4f13c76e7a3ca90f2a36d1532e858f15ae98fdbd91daf8e89dbe32c5`.
The native direct cross-vendor Enforce failure has its own receipt,
`0baac996c23659c7588674f1b54f65b17d1a71b44e6c554fece32c0926f0a2b9`.
