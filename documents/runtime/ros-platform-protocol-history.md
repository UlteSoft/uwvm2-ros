# Historical disabled platform protocol experiments

These retained measurements concern an earlier design. The AgileX port headers
are now disabled with literal `#if 0`; they are not shipped driver implementations
or evidence for the current external vendor-binary architecture. No production
platform extension may contain native driver code.

## Closed AgileX motion conversion

`platform_agilex_motion.h` and `platform_agilex_receiver.h` implement the motion
command and feedback subset reviewed against AgileX `ugv_sdk` commit
`f2704eacdc90357078cd93ec60aae08bb4baab35`. Command identifier `0x111` and feedback
identifier `0x221` each carry exactly eight bytes: four signed big-endian int16
milliscale values. The immutable ordered SI fields are `linear.velocity` (m/s),
`angular.velocity` (rad/s), `lateral.velocity` (m/s) and `steering.angle` (rad).
An unused axis must have both bounds set to zero. Complete vectors, exact units,
finite values and explicit deployment/physical limits are mandatory.

Conversion uses the binary64 significand and integer arithmetic to truncate
the represented SI value toward zero. It neither clamps nor performs an
out-of-range floating-to-integer cast and is independent of host rounding mode.
At decimal cell boundaries this can differ by one count from upstream rounded
floating multiplication; that deliberate difference is part of this contract.
Parity tests use a binary-exact 1/8-SI grid. Feedback is reported without command
clamping and by itself proves neither freshness, authentication nor stopped
state. Flags, other identifiers and incorrect extents are rejected.

The receiver shares common provider generation, controller lease, authority
epoch, strict sequence, digest, deadline and stop-latch validation. It commits a
command sequence only after vendor translation succeeds. Its watchdog deadline
is the earlier of the current controller lease and last accepted command's
absolute deadline. Controller replacement cannot extend an old command. A late
command or replacement lease is refused before the next watchdog tick without
consuming the pending stop event. Only a new command accepted before expiration
can replace the active deadline. Trusted stopped-state evidence and a fresh
controller are required after expiry. The adapter must independently service
that watchdog and recheck time at the bounded physical effect boundary.

The returned fixed zero-motion frame is an inert stop request, not a certified
brake, executed action or stopped-state witness. These allocation-free headers
perform no I/O, socket creation, motor enable, firmware update, calibration or
mode change. Deployment still needs an authenticated transport, an independent
physical safety chain, actual firmware qualification and a reviewed launcher.


## Measured protocol scope

The retained `platform-agilex-optimized` cohort compares the pinned upstream C
codec and this conversion with `-O3 -g`. Both framed paths use the same common
security receiver. Median batch means were 759.669 ns for native/common and
773.755 ns for platform/common (1.01854 times); private-pipe process round trips
were 3133.072 ns and 3126.273 ns respectively. The small latter difference is
measurement noise, not evidence of superior process performance. Raw codecs
were 7.464 ns and 11.678 ns, with additional validation only in the platform
codec. Prepared immutable schema decoding reduced platform/common time from
849.469 ns by approximately 8.9 percent without weakening SHA256 or authority.

These numbers precede the additional command-expiry gate. They are batch-mean
statistics, not per-request tail latency, complete VM timings, ROS workloads or
hardware results. Both processes used CPUs 8 and 10 with recorded core/NUMA
topology and inherited first-touch memory policy; no NUMA binding or real-time
guarantee is claimed. Official VTune hardware Hotspots and Microarchitecture
Exploration retained frozen-source and sampled-assembly reports. The separate
instruction join tool resolves DWARF inline frames to exact manifest hashes.
Later cohorts must retain their own source and binary identities before making
comparisons. All builds, tests and profiles use the existing 64 GiB zero-swap
cgroup; pressure events are retained along with OOM counters.

The later `platform-agilex-range-optimized` cohort includes the command-expiry
gate and removal of redundant physical-range checks after immutable schema
validation. Its binary SHA256 is
`c1e0806a8ca70e23b091c3114cf8dc464857965e732f808a76988987e5bd4b6e`.
The validated codec measured 11.490 ns per operation versus 8.910 ns for the
upstream raw codec. These codecs perform different validation work. With the
same common security receiver, the batch medians were 860.969 ns for the platform
and 869.096 ns for native; isolated medians were 3414.643 ns and 3430.123 ns.

The per-request paired experiment alternated native/platform order across
24,576 pairs using the same process and pipe channels, with independent equal
receiver states. Native/platform framed medians were 853/856 ns and P99 values
1025/1026 ns. Isolated medians were 3421/3423 ns and P99 values 5361/5305 ns.
The largest isolated observations were 795,555/1,017,036 ns. Every observation
was retained; each includes two steady-clock calls. This demonstrates comparable
overhead for this closed protocol workload, with substantial scheduling tails.
It does not establish hard real-time behavior or whole-ROSv2 native parity.
Both worker CPUs were on NUMA node 0, without explicit memory binding.

`platform-agilex-range-instructions` joins sampled instructions to immutable
source hashes and independent DWARF inline frames: 80 hash, 87 decoder, 106
receiver and 101 codec instructions have both samples and source attribution.
Hardware-event skid prevents treating these counts as individual instruction
latencies. The five-ABI codec receipt, sanitizer/allocation tests, VM build and
performance executable have separate identities. In particular, this codec is
not registered as a production hardware adapter by the VM safety build.
