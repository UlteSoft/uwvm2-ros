# Native74 r5 exact retained metadata closure

This successor preserves Native74 r4 and its failed raw reports. Revision r4
started no compiler: the retained model report also named three `/etc` files
and the resolver file recreated by systemd on reboot. They were outside r4's
explicit persistent-root selection.

The three exact `/etc` file selections retain the stronger same-path, inode,
timestamps and SHA256 requirement, with only device66307 to66308 rebinding.
No `/etc` tree is admitted. The one exact historical resolver observation is
reopened using the complete936-byte SHA256. Its actual current seven fields
are recorded separately, with no inode/time or process continuity claim.
No `/run` tree is admitted. Every consumed current input is rehashed at final
admission, and the Root owner checks the complete retained model input closure
before starting the child namespace. Historical reports remain unchanged.

The producer's compilation, actual ELF/MD/link/target validation, memory
reservations, four-job maximum, grants, source snapshot and whole-root checks
are otherwise the r4 implementation. Only fresh output names are r5. This
source package does not qualify a build, Runtime execution, DDS or physics.
Actual staging must bind the r5 support/producer/owner bytes and fill current
file pins from actual reads; do not edit or reuse r4 staged controls.

See `../native74_r4/README.md` for the unchanged compilation and resource
contracts. The existing completed content-only six-tree prerequisite may be
selected again only while its exact bytes and source/input preservation hold.
