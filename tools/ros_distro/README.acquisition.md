# Authenticated SDK inputs for native and cross targets

`acquire_sdk.py` prepares an isolated development sysroot for one explicit
`arm64` or `amd64` target. It uses a private APT status, sources, keyrings,
configuration, caches and download directories. It never installs a host
package, executes a package maintainer script, runs a compiler or starts ROS.
An extracted SDK is an input to the distribution lane, not a compatibility
result. Native x86_64 SDKs allow a fair Intel-host native/Wasm comparison;
AArch64 SDKs under QEMU qualify a different execution environment.

Run this producer only inside the already admitted 64 GiB cgroup with
`memory.swap.max=0`. Output and every visible ancestor filesystem retain the configured free-space
reserve, including the parent of a sparse-image mount. Its supplied boot ID,
leaf device/inode, actual membership,
CPU set and ancestor OOM counters must match. It uses the same frozen
`tools/ros_distro/common.py` pidfd owner as the distribution lane and allows one
actual command at a time. Neither Docker/Podman nor systemd is invoked.

## Explicit input contract

Supply a closed JSON object; `validate_spec` is the authoritative validator.
The complete top-level fields are:

| Field | Required meaning |
| --- | --- |
| `format_version`, `kind` | Integer `1`, string `authenticated-apt-sdk-inputs` |
| `target` | Exact `backend`, `distribution`, `ubuntu_codename`, `architecture`, `route`; route is `authenticated-apt-extraction` |
| `cgroup` | Current `memory_max`, `memory_swap_max`, `expected_boot_id`, `expected_leaf`, `expected_device`, `expected_inode`, `expected_cpus` |
| `tools` | Physical pinned `apt_get`, `apt_config`, `apt_helper`, `dpkg`, `dpkg_deb`, `gpgv`, `python` objects (`path`, `sha256`) and absolute `apt_methods` directory |
| `input_roots`, `input_files` | Complete host tool/library/Python/NSS/TLS closure; roots are inventoried before and after; selected files carry path/SHA-256 |
| `tls_ca`, `distribution_lock`, `seed_review` | Explicit physical path/SHA-256 objects |
| `repositories` | Reviewed Ubuntu and ROS sources, keys, full primary fingerprints, Release identity/expiry policy and any required exact InRelease SHA-256 |
| `seeds` | Explicit distinct target package names and optional exact versions |
| `limits` | Positive bounded command/overall time, owned RSS, log/disk reserves, package/index/archive/SDK byte and entry limits |

Each repository has exactly `name`, `role`, `uri`, `suite`, `components`,
`keyring`, `fingerprints`, `expected_inrelease_sha256`, `release_fields`, and
`expiry`. Each keyring has `path`/`sha256`. Expiry exceptions, when supported,
are explicit separately reviewed inputs bound to the exact signed archive.
No key, fingerprint, Release digest or repository trust is embedded in this
producer. Do not replace a failed authentication result with `Trusted=yes`,
`--allow-unauthenticated`, disabled TLS verification or an ambient host source.

`seed_review` has exactly `format_version: 1`,
`kind: "reviewed-apt-sdk-seeds"`, an identical `target`, `seed_groups`, and
`policy: "EMPTY_STATUS_FULL_TRANSITIVE_CLOSURE_NO_POSTINST"`. Its four nonempty
groups are `base_os`, `ros_development`, `build_tools` and `python`; their union
must equal `seeds`. A package suffix must agree with the selected architecture.
The APT scalar architecture, singleton architecture list, signed Release,
selected Packages indices, every deb control architecture and extracted package
provenance must all agree. Native-host selection does not permit mixed target
packages. Architecture-independent packages remain `all`.

Choose actual official URLs for the selected target. Ubuntu `arm64` ports URLs
cannot simply be retained when selecting `amd64`; use the appropriate official
archive/security source, or a separately reviewed signed historical archive.
Distribution names do not prove package availability. Pre-Groovy targets require
a separately locked source/backport producer. Rolling and historical/EOL
sources require independently reviewed exact InRelease digests. Current
repository authentication and complete solver/download evidence decide whether
a source is usable.

## Execute and consume

After preparing the physical spec, invoke the producer with the exact frozen
source and its manifest. Read `--help` for the complete CLI; the main inputs are
`--source`, `--manifest`, `--manifest-sha256`, `--spec`, `--spec-sha256` and a fresh
`--out`. All hashes describe actual retained bytes. Reusing a prior output is
refused; failures remain available for review.

The isolated APT configuration allows three retries for transient download
failures. The existing per-command and overall deadlines still bound the work,
and the effective configuration is retained in the report. Retries preserve
the original signed repository selection, package versions and SHA-256 checks;
they cannot turn a partial extraction into a qualified SDK. The maintained
producer keeps the system's normal IPv4/IPv6 selection. A separately reviewed
recovery producer may explicitly select IPv4 for a failing network route, with
that transport choice recorded in its effective configuration and source pin.

The result preserves effective APT configuration, raw signed InRelease,
independent gpgv status, decoded Release, signed Packages indices, solver plan,
every downloaded deb, its actual control stanza and extracted data, inventories,
command resource/ownership records and cleanup. A successful acquisition never
means a configured OS, native SDK run, Unitree interoperability or Guest grant.
Configure and qualify the SDK through `profile.py`, `probe.py`, `build.py` and `matrix.py`
under the same resource guard, using its actual compiler/loader/architecture.

`tests/ros_distro/acquisition_contracts.py` checks the effective APT parser using
the retained real dump from the earlier failed acquisition plus precise trust,
path, language, architecture and hook mutations. Its `--evidence` argument is
that independently pinned receipt, not a newly fabricated successful dump.
Run these parser contracts with the unified post-implementation test sequence.
