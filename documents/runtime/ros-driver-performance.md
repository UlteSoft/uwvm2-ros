# Encrypted driver attachment performance

This procedure compares Wasm and native clients using the same driver
dispatcher, authorization, schemas, authenticated TLS carrier and external
receiver. It measures a 17-byte request and 24-byte response. It is not a direct
function-call benchmark or a comparison with every native ROS application.
Receiver timestamps exclude process startup and Wasm compilation.

## Reproduce the latency comparison

Build both executables from the same frozen source with `-O3 -g`. Keep the VM's
`full-build-results.json` and the native reference's `native-build.json` beside
their binaries. The tools verify their hashes and build policies. Run builds,
tests and profiles serially through the existing 64 GiB, zero-swap cgroup gate.
Do not bypass that gate to obtain PMU access.

Choose CPUs from the actual topology. On the qualified i9-14900HX, logical CPUs
4 and 5 share a physical core; 4 and 6 belong to different physical cores.
Affinity does not reserve a core exclusively. Record other system load and
deployment resource constraints when reproducing results.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/taskset -c 4-7 /usr/bin/python3 \
  tools/ros_scene/driver_attachment_performance.py \
  --runtime .tmp/vm/uwvm2-ros --native .tmp/native/driver-roundtrip \
  --output .tmp/driver-latency --tls --iterations 100000 --rounds 7 \
  --cpu 4 --peer-cpu 6
```

Keep sampling disabled for the latency baseline. Alternate native and Wasm
runs and retain individual outcomes. Compare the median per-run round trip,
not a profiled VM against an unprofiled native program.

Source cohort `77c5d7c49287148bfed4b11309ccff51ce9a2e80`, seven alternating
trials of 100000 calls, client CPU 4 and receiver CPU 6:

| Client | Median round trip |
| --- | ---: |
| Matched native carrier | 4152.437 ns |
| Wasm through ROSv2 | 4405.868 ns |

The ratio is 1.06103, approximately 6.1% additional latency for this path.
Earlier runs on SMT siblings measured a different scheduling scenario. The
placement improvement is not evidence of a new instruction optimization or
whole-robot deadline qualification.

VM SHA256:
`49fe77bd13a69a6da8e9622eedd348ee90b8a23332a7cb6ebae5cd5657f0e9c0`.
Native SHA256:
`2d193b97d7bea77e57ad30e4b7d17f7cb58b92193f731c5f593057f04572fad9`.
The local receipt is `matched-latency-distinct-cores/results.json` under the
publication-validation workspace. Intermediate binaries, keys and profiling
databases are excluded from source publication.

## Hardware microarchitecture, source and assembly

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/taskset -c 4-7 /usr/bin/python3 \
  tools/ros_scene/driver_attachment_microarchitecture.py \
  --runtime .tmp/vm/uwvm2-ros --native .tmp/native/driver-roundtrip \
  --output .tmp/driver-uarch --iterations 1000000 \
  --cpu 4 --peer-cpu 6 --sampling-interval 1
```

This requests VTune `uarch-exploration` in detailed mode. Reports use the actual
client PID, excluding the provisioner and receiver. Qualification requires
positive hardware samples mapped to source lines and assembly addresses,
top-down metrics, unchanged inputs, and no lost or throttle records in the raw
perf trace. Empty reports and failed collections cannot pass.

The qualified `matched-uarch-bounded/results.json` has 429 native and 455 Wasm
sampled source rows, and 47 and 45 sampled assembly rows. Both traces have zero
lost and throttle records. The successful complete preset used a **1 ms
request**. Its reference event used 2400000 cycles at approximately 2.4192 GHz,
about 0.992 ms nominally; other events have their own recorded periods.
Event-based sampling is not a fixed wall-clock interrupt. This does not claim
that the complete preset runs at 0.01 ms. The separate explicit-period hardware
hotspot result retains its own configuration and is not the complete preset.

Hybrid presets include P-core and E-core events. The optional
`--collector-cpus` accepts an explicit comma-separated list. On this machine,
CPU 4 alone failed event validation; CPUs 4 and 16 completed collection but
produced no client report rows. Both remain failed receipts. The successful run
used the default collector CPU range with client-PID report filters. A 0.1 ms
trial contained throttle records and is diagnostic evidence only.

SEP/PAX filesystem ownership does not prove device access inside a cgroup. The
existing Docker gate denied device opens here; Linux perf hardware sampling
remained usable. The tool did not change the gate, load drivers or acquire
privilege. See Intel's [hardware sampling guide](https://www.intel.com/content/www/us/en/docs/vtune-profiler/user-guide/2026-0/hw-event-based-sampling-collection.html)
and [CPU-mask reference](https://www.intel.com/content/www/us/en/docs/vtune-profiler/user-guide/2023-1/cpu-mask.html).

## Optimization boundaries

Wasm samples identify floating-point environment save/restore and preload
memory identity resolution as measurable costs. The TLS boundary retains the
complete floating-point guard because OpenSSL is an external native library;
removing it requires evidence that guest floating-point state remains isolated.
Memory provenance, exact identities, authorization, deadlines and mandatory
erasure remain enabled in this comparison. Placement is the validated
improvement in these measurements.

Kernel-pointer restrictions leave some kernel samples unresolved. Those rows
are retained without invented source or assembly locations. Changed binaries
need new matched receipts and semantic/security regression checks. These
results qualify this carrier path, not every interface, vendor or ROS package.
