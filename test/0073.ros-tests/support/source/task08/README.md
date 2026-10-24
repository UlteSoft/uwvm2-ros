# Task08 canonical test support

This directory contains newly authored test support. Task10 alone migrates
existing SDK tests and allocates the numbered platform/language entries.
The Task08 migration request records exact source/view hashes and provenance;
equal wrapper bytes from different platform contexts are not deduplicated.

`spot_native_lease_loopback.py` uses genuine Spot5.0.1 SDK clients, wallets,
processors and generated loopback services. It needs explicit `--dependencies`,
`--source` (the immutable staged repository snapshot) and `--output`. It retains
complete protobuf frames, positive lease/Stop/return outcomes, and committed
acquisition/return timeout cases. The finite peer binds only 127.0.0.1. No physical
robot, ROS, Gateway or Wasm Guest is involved. All execution belongs inside the
original shared hard64GiB/swap0 finite-owner harness on SSH linux.
