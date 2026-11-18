# Native cross-release Fast DDS with SROS 2

The fixture compiles one rclcpp String application against each selected ROS
installation. The two instances exchange a fresh request and reply on
`/uwvm_test/pubString` and `/uwvm_test/subString`, using the release's own
`rmw_fastrtps_cpp` and Fast DDS. A third instance of the requesting release uses its own signed
observer identity to request a reader for the active request topic. The
receipt checks the two authorized callbacks, Fast DDS's denial of the reader,
and the observer's zero callback count.

The issuer uses the Jazzy `ros2 security generate_artifacts` command and the
existing gateway/witness policy, extended by one observer enclave. The runner
checks the signatures on each permission and governance document, verifies
that the domain rules apply, and records public certificate and document
digests. Temporary credentials live in a private shared-memory directory for
the run. The application run uses a private loopback network and an explicit
UDPv4 profile. The process maps bind each process to the expected rclcpp, RMW
and Fast DDS libraries.

A separate missing-enclave case checks that both releases fail to start under
`ROS_SECURITY_STRATEGY=Enforce`. The plaintext case exercises the same pair
and verifies that both fresh String payloads appear in the UDP data sent by
their respective publishers.
The Enforce case requires the signed topic data and metadata protections to
select `ENCRYPT`, no visible payload markers, and AES-GCM protected submessages
from each authorized participant to the UDP user-unicast port owned by the other
process. The observer has a third, independently identified port. A child-process
strace records each application's successful loopback UDP sends. The decoder
reassembles the sendmsg buffers, checks the kernel-reported byte count and
identifies the DDS Security CryptoHeader transform and SEC_PREFIX/SEC_BODY/SEC_POSTFIX
sequence. Local output retains source traces. The receipt contains counts and hashes.
[OMG DDS Security 1.1](https://www.omg.org/spec/DDS-SECURITY/1.1/PDF)

Run from the 64 GiB qualification cgroup and serial lock with a fresh output
directory and the archived native-peer sysroots. The issuer rootfs must contain
the Jazzy SROS 2 CLI. The namespace must allow strace to observe its own children.

```sh
python3 -B run.py --issuer-rootfs "$JAZZY_ROOTFS" \
  --request-prefix "$JAZZY_PREFIX" --request-system "$JAZZY_SYSTEM" \
  --echo-prefix "$ECHO_PREFIX" --echo-system "$ECHO_SYSTEM" \
  --loopback-profile ../dds_interop/fastdds-loopback.xml --output "$NEW_RESULTS"
```


## Native release observations

Jazzy Fast DDS 2.14.6 and Lyrical Fast DDS 3.6.2.0 completed the authorized
String exchange and refused the observer's subscription. The plaintext control
contained 17 request markers and 14 replies. Enforce yielded 41 AES-256-GCM
submessages from Jazzy to Lyrical's own UDP port and 38 from Lyrical to Jazzy's
own port. Both application callbacks checked the fresh payloads. The observer
remained active until after the reply and received zero samples. The governed
topic data and metadata were encrypted and the domain RTPS protection was
`SIGN`. The observer recorded one rcutils error-state diagnostic after the
reader refusal. The authorized peers recorded zero ROS warnings, ROS errors
and rcutils error-state reports. The result SHA-256 is
`1adae0ebe11eba81980885d70b2d2cb12e9c6df237c7a4323ae935c18836919e`.

The Jazzy and Humble Fast DDS 2.6.12 plaintext control passed. With Enforce,
Humble reported a mismatch in the permissions CA algorithm advertised by
Jazzy. The captured permission tokens named the same signing scheme
`EC-prime256v1` and `ECDSA-SHA256` respectively. A separate run setting
Fast DDS's legacy permission-token encoding property in the Jazzy XML profile
produced the same native failure. The Jazzy RMW installs its own security
property policy during participant initialization. The signed policy and
encryption requirements in this fixture apply to every Enforce case.
[Fast DDS permission-token encoding](https://github.com/eProsima/Fast-DDS/blob/v2.14.6/src/cpp/security/accesscontrol/Permissions.cpp)
and [Jazzy participant security initialization](https://github.com/ros2/rmw_fastrtps/blob/jazzy/rmw_fastrtps_shared_cpp/src/participant.cpp).

The Jazzy/Humble default-result SHA-256 is
`b8423c852432cbd800e38893c428e94290032a20bb3bcb64c6b6fccb9031003a`.
The XML property experiment produced result SHA-256
`51ece1000fcf9c22149e68ef84731eebd9cdf0ed5b3a3b1ef37fe3d290d83756`.

## Cyclone DDS peer and same-vendor control

Add `--cyclone-echo --cyclone-plan "$CYCLONE_PLAN" --archive-keyring "$ROS_KEYRING"`
to select the echo release's own `rmw_cyclonedds_cpp`. Use the same Jazzy issuer
rootfs, Fast DDS requester, signed gateway/witness policy and loopback profile.
The runner checks the original Cyclone, rclcpp and RMW package members against
the signed ROS index and records the authentication, access-control and
cryptography plugin digests. Cyclone uses the isolated IPv4 loopback interface
and a fixed unicast discovery peer. The application checks the selected RMW
at runtime.

One issuance supplies every case. The mixed pair and the two Cyclone
processes each run a plaintext control and an Enforce exchange. The control's
two processes use their distinct gateway and witness identities. Each Enforce
case requires both authorized callbacks, protected RTPS sends to both peers'
owned UDP ports, and the loaded native libraries and Cyclone security plugins.
The separate unauthorized observer belongs to the mixed pair. Individual
results include timeouts, process exits, marker sightings and trace digests.

Lyrical Cyclone DDS 11.0.1 completed the two-process Enforce control. The
participant traces recorded 45 AES-256-GCM submessages to the echo process
and 39 to the requester. Both callbacks confirmed the exact strings. The
Jazzy Fast DDS 2.14.6 and Lyrical Cyclone pair exchanged both exact strings
in plaintext. Under Enforce, both authorized processes expired without
confirming a sample. Their paired UDP ports received zero observed encrypted
submessages from each other. The Cyclone log recorded seven handshake reply
failures while converting a key string to an ASN.1 integer (code 200).
Fast DDS denied the independent observer's reader. The combined result SHA-256
is `0baac996c23659c7588674f1b54f65b17d1a71b44e6c554fece32c0926f0a2b9`.
